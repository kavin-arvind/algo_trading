# Low-Latency Multi-Process Trading System in C++

## 1. Project Overview

### Project title
**Low-Latency Multi-Process Trading System in C++**

### Goal
Build a simplified trading/exchange system that processes market data, detects trading opportunities, creates orders, and executes them through a simulated exchange.

The main purpose is **not** to build a sophisticated trading strategy. The main purpose is to build and optimize a realistic low-latency C++ pipeline and demonstrate:

- Modern C++
- Multithreading and atomics
- Lock-free data structures
- Inter-process communication (IPC)
- Shared memory
- Linux systems programming
- Networking
- Cache-conscious programming
- Custom memory management
- Performance measurement and optimization

The project should evolve incrementally. Start with a correct simple implementation and then replace individual components with increasingly optimized implementations while measuring the effect of every change.

---

# 2. High-Level Architecture

The final system should contain multiple processes rather than being one large C++ executable.

```text
                         +----------------------+
                         |   Market Simulator   |
                         |----------------------|
                         | Reads input data     |
                         | Generates updates    |
                         +----------+-----------+
                                    |
                                    | IPC / Shared Memory / UDP
                                    v
                         +----------------------+
                         | Market Data Handler  |
                         |----------------------|
                         | Parse messages       |
                         | Convert to structs   |
                         | Timestamp events     |
                         +----------+-----------+
                                    |
                                    v
                         +----------------------+
                         | Lock-Free Queue      |
                         |----------------------|
                         | SPSC ring buffer     |
                         +----------+-----------+
                                    |
                                    v
                         +----------------------+
                         | Trading Strategy     |
                         |----------------------|
                         | Maintain state      |
                         | Detect opportunities |
                         | Generate orders      |
                         +----------+-----------+
                                    |
                                    v
                         +----------------------+
                         | Order Manager        |
                         |----------------------|
                         | Order IDs            |
                         | Track state          |
                         | Send/cancel orders   |
                         +----------+-----------+
                                    |
                                    | IPC / Shared Memory / UDP
                                    v
                         +----------------------+
                         | Exchange Simulator   |
                         |----------------------|
                         | Order book           |
                         | Matching engine      |
                         | Fills/rejections     |
                         +----------+-----------+
                                    |
                                    v
                         +----------------------+
                         | Statistics / Report  |
                         |----------------------|
                         | P&L                  |
                         | Latency              |
                         | Throughput           |
                         +----------------------+
```

A practical first version can have three processes:

```text
market -> trader -> exchange
```

The market process produces market updates, the trader consumes them and generates orders, and the exchange matches the orders.

Later, the trader can be split into more components if useful.

---

# 3. Main Design Principle

The project should have **two layers**:

## Functional layer
Make the system correct.

Examples:

- Market updates arrive correctly.
- Orders are generated correctly.
- Orders are matched correctly.
- Partial fills work.
- P&L is correct.
- Processes recover from normal errors.

## Performance layer
Optimize individual components.

Examples:

- Replace mutex-based queue with lock-free queue.
- Replace socket IPC with shared-memory IPC.
- Replace heap allocation with an object pool.
- Reduce cache misses.
- Avoid false sharing.
- Pin threads to CPUs.
- Reduce copies.
- Measure p50/p99/p99.9 latency.

Do not optimize everything immediately. Keep an easy-to-understand baseline implementation so that every optimization can be compared against it.

---

# 4. Components

## 4.1 Market Simulator

### Responsibility
The market process acts as the source of market data.

It should:

1. Read market events from input files.
2. Convert each event into a market-data message.
3. Assign or preserve timestamps.
4. Publish the message to the trader.
5. Optionally replay data at different speeds.

### Example input

```text
1000001,AAPL,BID,18210,100
1000005,AAPL,ASK,18212,50
1000007,MSFT,BID,42150,80
1000010,AAPL,BID,18209,200
```

Use integer prices rather than floating point whenever possible.

Example:

```cpp
int64_t price_ticks = 18210;
```

where one tick may represent 0.01 units.

### Features to add later

- Replay at maximum speed.
- Replay at a configured rate.
- Burst generation.
- Multiple symbols.
- Multiple exchanges.
- Synthetic market-data generation.
- Configurable message rate.

---

# 5. Market Data Message

Use a compact fixed-size structure for the hot path.

Example:

```cpp
enum class Side : uint8_t {
    Buy,
    Sell
};

struct MarketUpdate {
    uint64_t timestamp;
    uint32_t symbol_id;
    int64_t price_ticks;
    uint32_t quantity;
    Side side;
};
```

Avoid putting dynamically allocated objects such as `std::string` in the hot-path message.

Instead of:

```cpp
std::string symbol;
```

use:

```cpp
uint32_t symbol_id;
```

and maintain a symbol table elsewhere.

This lets you discuss:

- object size
- alignment
- padding
- copying cost
- cache utilization

---

# 6. Market Data Handler

The market-data handler consumes the raw IPC/network messages and converts them into internal messages.

Responsibilities:

- Validate message format.
- Deserialize binary messages if necessary.
- Convert wire-format fields into internal representation.
- Timestamp reception.
- Push the resulting event into the internal queue.

Keep parsing out of the strategy logic.

A useful separation is:

```text
wire format
    |
    v
parser
    |
    v
MarketUpdate
    |
    v
queue
    |
    v
strategy
```

---

# 7. Lock-Free SPSC Ring Buffer

This should be one of the core technical pieces of the project.

## Purpose
Provide a fast producer-consumer queue without using a mutex.

Start with a single-producer/single-consumer design:

```cpp
template <typename T, size_t Capacity>
class SPSCQueue;
```

Use a fixed-size circular buffer:

```text
                producer
                   |
                   v
      +---+---+---+---+---+---+
      | 0 | 1 | 2 | 3 | 4 | 5 |
      +---+---+---+---+---+---+
          ^               ^
          |               |
        read            write
          |
       consumer
```

## Requirements

- No dynamic allocation during normal operation.
- Producer and consumer operate concurrently.
- Correct memory ordering.
- Detect full/empty states.
- Fixed capacity.
- Cache-line-aware layout.

## C++ concepts to demonstrate

- `std::atomic`
- acquire/release ordering
- CAS where appropriate
- cache-line alignment
- false sharing
- memory visibility between threads

Do not use `memory_order_seq_cst` everywhere without understanding it. Document why each memory ordering choice is safe.

## Baseline comparison
Implement a mutex-based queue first.

Compare:

```text
Mutex Queue
vs
Lock-Free SPSC Queue
```

Measure:

- throughput
- average latency
- p50
- p99
- p99.9
- CPU usage

---

# 8. Trading Strategy

The strategy should be simple enough that the systems work remains the focus.

A good initial strategy is **cross-market arbitrage**.

Example:

```text
Exchange A: AAPL bid/ask = 182.10 / 182.12
Exchange B: AAPL bid/ask = 182.15 / 182.17
```

Potential opportunity:

```text
Buy from A at 182.12
Sell on B at 182.15
Gross spread = 0.03
```

The strategy should include configurable thresholds for:

- minimum spread
- maximum order size
- maximum inventory
- transaction costs
- risk limits

Do not spend most of the project on sophisticated finance. The strategy only needs to create realistic traffic through the system.

---

# 9. Order Manager

The order manager converts strategy decisions into exchange orders.

Example:

```cpp
struct Order {
    uint64_t order_id;
    uint32_t symbol_id;
    int64_t price_ticks;
    uint32_t quantity;
    Side side;
    OrderType type;
};
```

Responsibilities:

- Generate order IDs.
- Maintain outstanding orders.
- Track submitted/acknowledged/partially-filled/filled/cancelled states.
- Send orders to the exchange.
- Process execution reports.
- Maintain inventory.

Keep order objects compact and preferably fixed-size in the latency-sensitive path.

---

# 10. Exchange Simulator / Matching Engine

The exchange receives orders and maintains an order book.

## Basic order book

```text
ASKS
182.20   100
182.18   150
182.16    50
----------------
182.14    75
182.12   100
182.10   200
BIDS
```

The matching engine should support at least:

- limit orders
- buy/sell side
- price-time priority
- partial fills
- full fills
- cancellations
- execution reports

## Initial implementation
Use a straightforward STL-based design, for example:

```cpp
std::map<int64_t, PriceLevel>
```

Then build more specialized structures if benchmarking shows a reason to do so.

## Price level

A price level can contain multiple orders:

```text
price = 18210
    |
    +--> order 101, qty 100
    +--> order 104, qty 50
    +--> order 107, qty 25
```

Maintain price-time priority.

---

# 11. Alternative Order Book Implementations

One of the strongest parts of the project can be comparing implementations.

### Version A: `std::map`

Simple and correct baseline.

### Version B: Array of price levels

Useful when the simulated price range is bounded.

### Version C: Custom structure

Design something specialized for the workload.

Benchmark:

```text
insert order
cancel order
match order
best bid/ask lookup
```

The goal is not to invent a magical data structure. The goal is to show that you measured the workload and chose a representation deliberately.

---

# 12. Inter-Process Communication

Implement multiple IPC mechanisms and compare them.

## Stage 1: Unix domain socket

Simple baseline for local process communication.

```text
market process
     |
     | Unix socket
     v
trader process
```

## Stage 2: Shared memory

Use Linux shared memory mechanisms such as:

- `shm_open`
- `mmap`

Conceptually:

```text
+--------------------------+
|      shared memory       |
|                          |
|  ring buffer             |
|  metadata                |
|  producer index          |
|  consumer index          |
+--------------------------+
      ^                ^
      |                |
    market           trader
```

The ring buffer must be carefully designed because both processes access the same memory.

## Stage 3: UDP loopback

Use UDP to model a market-data feed.

This gives you experience with:

- sockets
- packet boundaries
- serialization
- system calls
- kernel/user-space transitions

Compare:

```text
Unix Domain Socket
Shared Memory
UDP Loopback
```

Do not claim that one is universally better. Measure your own implementation.

---

# 13. Shared-Memory Design

A shared-memory region should contain data that both processes understand.

Example concept:

```cpp
struct SharedQueueHeader {
    std::atomic<uint64_t> head;
    std::atomic<uint64_t> tail;
    uint64_t capacity;
};
```

Followed by a fixed array of messages.

Important considerations:

- Alignment.
- Relative offsets instead of raw pointers when appropriate.
- Lifetime of objects in shared memory.
- Process synchronization.
- Initialization and cleanup.
- What happens if a process crashes.

Avoid putting process-local pointers into shared-memory structures unless the design explicitly guarantees address compatibility.

---

# 14. Memory Management

The trading path should avoid unnecessary heap allocations.

## Baseline
Use ordinary STL allocation.

## Optimized version
Implement an object pool / fixed-size memory pool.

Example interface:

```cpp
class OrderPool {
public:
    Order* allocate();
    void deallocate(Order* order);
};
```

Investigate:

```text
new/delete
vs
memory pool
```

Measure allocation cost and impact on latency.

Possible extensions:

- freelist
- fixed-size block allocator
- cache-aligned allocation
- per-thread pools

Do not build a custom allocator unless the benchmark shows a useful reason to do so. The purpose is learning and measurement, not complexity for its own sake.

---

# 15. Cache Awareness

This should be an explicit optimization section of the project.

Study and benchmark:

- cache locality
- data layout
- structure padding
- alignment
- false sharing
- sequential vs random access
- AoS vs SoA

Example false-sharing experiment:

```cpp
struct Bad {
    std::atomic<uint64_t> producer;
    std::atomic<uint64_t> consumer;
};
```

versus a cache-separated version using appropriate alignment/padding.

Then run multiple threads and measure the effect.

Document the results instead of assuming the optimization is beneficial.

---

# 16. CPU and Scheduling Optimization

After the basic system works, experiment with Linux CPU affinity.

Example conceptual layout:

```text
CPU 2 -> market-data handling
CPU 3 -> strategy
CPU 4 -> exchange
```

Investigate:

- CPU affinity
- thread scheduling
- migration between CPUs
- context switches
- CPU utilization

Useful Linux tools include:

```bash
taskset
numactl
perf
```

Do not add thread pinning purely as a buzzword. Benchmark the effect.

---

# 17. Latency Measurement

Latency measurement is one of the most important parts of the project.

Define events such as:

```text
T0 = market update generated
T1 = market update received
T2 = queue push complete
T3 = strategy begins processing
T4 = strategy generates order
T5 = exchange receives order
T6 = order matched
T7 = execution report received
```

Then measure:

```text
feed latency       = T1 - T0
queue latency      = T3 - T2
strategy latency   = T4 - T3
order latency      = T5 - T4
round-trip latency = T7 - T0
```

Report:

- minimum
- median / p50
- p90
- p99
- p99.9
- maximum

Tail latency matters. A low average with large p99 spikes is not the same as consistently low latency.

---

# 18. Benchmarking

Every important optimization should have a benchmark.

## Queue benchmark

```text
Mutex queue
Lock-free SPSC queue
```

Metrics:

- operations/sec
- ns/op
- p99 latency

## IPC benchmark

```text
Unix socket
Shared memory
UDP loopback
```

## Allocation benchmark

```text
new/delete
Object pool
```

## Order-book benchmark

```text
std::map
Array-based price levels
Custom implementation
```

## CPU/cache benchmark

Measure with tools such as:

```bash
perf stat
perf record
```

Potential counters to inspect:

- cycles
- instructions
- cache references
- cache misses
- branch instructions
- branch misses
- context switches

---

# 19. Important C++ Skills to Demonstrate

The codebase should naturally demonstrate the following.

## Core C++

- RAII
- constructors/destructors
- copy/move semantics
- rule of 0/3/5
- references and pointers
- object lifetime
- templates
- `constexpr`
- `enum class`
- `std::span` where appropriate
- `std::array`
- `std::vector`
- `std::unordered_map`
- `std::map`
- smart pointers where ownership requires them

## Performance-related C++

- avoiding unnecessary copies
- move semantics
- inlining awareness
- fixed-size data structures
- allocation avoidance
- contiguous storage
- alignment
- object layout

## Concurrency

- `std::thread`
- `std::atomic`
- mutexes
- condition variables where appropriate
- memory ordering
- CAS
- lock-free data structures
- false sharing

## Systems C++

- POSIX APIs
- sockets
- shared memory
- `mmap`
- file I/O
- process creation / management
- signals where useful

---

# 20. Linux Skills

The project should be developed primarily on Linux.

Become comfortable with:

```text
g++ / clang++
gdb
perf
strace
objdump
nm
readelf
taskset
numactl
```

Also understand useful compiler options such as:

```bash
-O2
-O3
-g
-pthread
-flto
-march=native
```

Do not blindly enable every optimization. Keep benchmark builds reproducible and document important flags.

---

# 21. Suggested Directory Structure

```text
low-latency-trading-system/
│
├── CMakeLists.txt
├── README.md
├── LICENSE
├── docs/
│   ├── architecture.md
│   ├── protocol.md
│   ├── benchmarks.md
│   ├── design-decisions.md
│   └── experiments.md
│
├── config/
│   ├── symbols.conf
│   └── strategy.conf
│
├── data/
│   ├── market_data.csv
│   └── test_data.csv
│
├── include/
│   ├── common/
│   ├── market/
│   ├── trader/
│   ├── exchange/
│   ├── ipc/
│   └── data_structures/
│
├── src/
│   ├── market/
│   ├── trader/
│   ├── exchange/
│   ├── ipc/
│   ├── common/
│   └── main/
│
├── tests/
│   ├── unit/
│   ├── integration/
│   └── stress/
│
├── benchmarks/
│   ├── queue_benchmark.cpp
│   ├── ipc_benchmark.cpp
│   ├── orderbook_benchmark.cpp
│   └── allocator_benchmark.cpp
│
├── scripts/
│   ├── run_benchmark.sh
│   ├── collect_perf.sh
│   └── plot_results.py
│
└── results/
    ├── queue/
    ├── ipc/
    ├── orderbook/
    └── allocator/
```

The exact organization can change. The important thing is to separate production code, tests, benchmarks, documentation, and experimental results.

---

# 22. Build System

Use **CMake**.

Recommended baseline:

```text
C++20
CMake 3.20+
GCC or Clang
Linux
```

Keep debug and release builds separate.

Example:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```

and:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Use sanitizers in debug/testing builds where applicable:

```text
AddressSanitizer
UndefinedBehaviorSanitizer
ThreadSanitizer
```

Note that ThreadSanitizer can materially affect timing, so do not use sanitizer builds for latency measurements.

---

# 23. Testing Strategy

The system should have more than a happy-path test.

## Unit tests

Test:

- queue correctness
- order validation
- matching logic
- partial fills
- cancellation
- P&L
- serialization/deserialization

## Integration tests

Example:

```text
market -> trader -> exchange
```

with known input and expected output.

## Concurrency tests

Stress:

- producer/consumer behavior
- queue wraparound
- full queue
- empty queue
- process restart scenarios where practical

## Performance regression tests

Track benchmarks between versions.

A performance improvement should not silently break correctness.

---

# 24. Error Handling

Explicitly handle:

- malformed market-data messages
- invalid orders
- full queues
- disconnected peers
- IPC initialization failure
- shared-memory initialization failure
- exchange rejection
- process termination

Separate the hot path from unusual error paths where appropriate.

Do not add exceptions to every tiny operation merely because they are available. Use a consistent error-handling strategy and understand its cost.

---

# 25. Observability

Logging is necessary for debugging, but logging in the hot path can distort latency.

Use two modes:

## Debug mode
Detailed logs.

## Benchmark mode
Minimal or disabled hot-path logging.

Consider:

- counters
- sampled logging
- per-stage latency counters
- end-of-run reports

Example report:

```text
Messages processed : 10,000,000
Orders generated   :    850,000
Orders filled      :    790,000
Total P&L          :      ...

Latency:
feed        p50=...
p99=...
strategy    p50=...
round-trip  p50=...
```

---

# 26. Project Phases

## Phase 1 — Correct baseline

Build a single-process simulator.

Implement:

- market input
- strategy
- orders
- matching engine
- P&L

No advanced optimization yet.

### Goal
Have a completely correct reference implementation.

---

## Phase 2 — Split into processes

Create:

```text
market
trader
exchange
```

Use a simple IPC mechanism.

### Goal
Understand process boundaries and message passing.

---

## Phase 3 — Networking

Implement local socket communication.

Try:

- Unix domain sockets
- UDP loopback

### Goal
Build familiarity with practical low-level communication.

---

## Phase 4 — Lock-free queue

Implement:

```text
SPSC ring buffer
```

Benchmark it against a mutex queue.

### Goal
Learn atomics and memory ordering.

---

## Phase 5 — Shared memory

Move selected communication to:

```text
shared memory + ring buffer
```

### Goal
Learn fast IPC and shared-state design.

---

## Phase 6 — Memory optimization

Add:

- object pool
- fixed-size buffers
- allocation avoidance

### Goal
Reduce work on the hot path.

---

## Phase 7 — Cache optimization

Investigate:

- alignment
- false sharing
- layout
- locality

### Goal
Understand the CPU effects of data layout.

---

## Phase 8 — Order book optimization

Benchmark multiple implementations.

### Goal
Connect algorithmic data structures with actual workload performance.

---

## Phase 9 — CPU affinity

Experiment with thread pinning.

### Goal
Understand scheduling and CPU placement.

---

## Phase 10 — Profiling and final benchmark suite

Use:

- Google Benchmark or an equivalent microbenchmark framework
- `perf`
- GDB

Create a repeatable benchmark suite.

### Goal
Be able to explain exactly where time is spent and what each optimization changed.

---

# 27. Performance Experiments to Include

At minimum, include these experiments in the repository.

### Experiment 1: Queue

```text
mutex queue
vs
lock-free queue
```

### Experiment 2: IPC

```text
Unix socket
vs
shared memory
```

### Experiment 3: Allocation

```text
new/delete
vs
object pool
```

### Experiment 4: Data layout

```text
normal layout
vs
cache-aware layout
```

### Experiment 5: False sharing

```text
shared cache line
vs
cache-separated variables
```

### Experiment 6: CPU affinity

```text
normal scheduler
vs
CPU pinning
```

### Experiment 7: Order book

```text
std::map
vs
specialized price-level structure
```

Every experiment should answer:

1. What was changed?
2. What did you expect?
3. What did you measure?
4. Why did the result happen?
5. What trade-offs were introduced?

---

# 28. What Not to Do

Avoid adding advanced technology merely to make the project sound impressive.

For example, do not add:

- distributed databases
- Kubernetes
- microservices everywhere
- complicated cloud architecture
- machine learning that is unrelated to the latency problem
- huge amounts of financial mathematics
- a complicated GUI

Those features do not improve the main goal of the project.

A small Linux C++ system that is extremely well measured is more valuable for this objective.

---

# 29. What Makes This Project HFT-Relevant

The project demonstrates a chain like:

```text
Market data
   -> parsing
   -> IPC/networking
   -> lock-free queue
   -> strategy
   -> order creation
   -> IPC/networking
   -> matching engine
   -> execution report
```

and lets you investigate the latency and throughput of every step.

The most valuable part is not the final trading P&L.

The strongest evidence of systems skill is being able to explain things such as:

- Why was this queue lock-free?
- Why is it SPSC instead of MPMC?
- Why was this memory ordering selected?
- Why does shared memory help compared with a socket?
- Where are the copies happening?
- Where are allocations happening?
- What causes false sharing here?
- What does the CPU cache do for this data structure?
- Why does p99 latency matter?
- Why did an optimization improve average latency but worsen tail latency?
- Why did one order-book representation beat another?
- What did `perf` show?

Those are the conversations this project should prepare you for.

---

# 30. Final Resume-Level Project Description

Once the project is genuinely implemented and benchmarked, a possible resume description is:

> **Low-Latency Multi-Process Trading System — C++**  
> Built a multi-process trading system consisting of a market-data simulator, trading strategy, order manager, and matching engine, using shared-memory IPC and lock-free SPSC queues for low-latency communication. Implemented a price-time-priority order book, custom memory pooling, cache-aware data layouts, and Linux performance instrumentation; benchmarked IPC, queue, allocation, and order-book implementations using latency distributions and hardware performance counters.

Do not use this wording until the features are actually implemented and measured.

---

# 31. Suggested Final Technology Stack

```text
Language:        C++20
OS:              Linux
Build:            CMake
Compiler:         GCC / Clang
Testing:          GoogleTest (or equivalent)
Benchmarking:     Google Benchmark (or custom controlled benchmarks)
Profiling:        perf
Debugger:         GDB
IPC:              Unix sockets + shared memory
Networking:       UDP/TCP as experiments
Concurrency:      std::thread + std::atomic
Data structures:  STL + custom ring buffers / order book
Memory:           object pool / fixed-size allocator
Visualization:    Python + matplotlib for benchmark plots
```

---

# 32. Recommended Priority Order

If time is limited, prioritize in this order:

```text
1. Correct trading engine
2. Multi-process architecture
3. IPC
4. SPSC lock-free queue
5. Atomics + memory ordering
6. Benchmarking
7. Order book optimization
8. Memory pool
9. Cache / false-sharing experiments
10. CPU affinity
11. UDP/networking experiments
12. Extra optimizations
```

Do not skip the benchmark stage. The project becomes much stronger when every optimization is backed by measurements.

---

# 33. Definition of Done

The project is strong enough for an HFT-focused resume when all of the following are true:

- [ ] Multiple processes communicate correctly.
- [ ] Market data can be replayed at high speed.
- [ ] A strategy generates orders from market data.
- [ ] Exchange matching is correct.
- [ ] Partial fills and cancellations work.
- [ ] P&L and inventory are correct.
- [ ] Shared-memory IPC works.
- [ ] A lock-free SPSC queue is implemented and tested.
- [ ] Memory ordering choices are documented.
- [ ] Heap allocation on the main hot path is minimized.
- [ ] At least one object/memory pool is benchmarked.
- [ ] Cache/false-sharing behavior is investigated.
- [ ] Multiple order-book designs are benchmarked.
- [ ] Latency is measured end-to-end.
- [ ] p50/p99/p99.9 statistics are reported.
- [ ] `perf` is used to inspect CPU behavior.
- [ ] Tests cover correctness and concurrency.
- [ ] README contains architecture and benchmark results.
- [ ] Design decisions are documented.

The project does **not** need every possible HFT optimization to be complete. A smaller project with rigorous measurements and a deep understanding of the implementation is preferable to a huge project full of unexplained optimizations.