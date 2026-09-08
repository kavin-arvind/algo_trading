# My project started afresh

### iml.cpp
Its internal Market Layer that communicates with the exchange and bring us the orders we need.

### data.txt
It simulates the actual exchanges such as NSE, NASDAQ, etc where we see different people placing order

### trader.cpp
Has a receiver that communicates with iml.cpp and getting info about orders. Implements the logic of algorithmic trading and we'll have a seperate thing to place order from this logic

### receiver.h
Receiver associated with trader.cpp to just get in the orders.

### market.cpp
This is the implementation of the exchange market, (the one that would be used as NSE or NASDAQ exchanges)

# Instructions to run
To generate the ```Makefile```:
```bash
cmake -S . -B build
```

To generate and also to make executables:
```bash
cmake --build build -j$(nproc)
```



                         AlgoTrader
                             │
              ┌──────────────┴──────────────┐
              │                             │
             Core                       Applications
              │                             │
      ┌───────┼────────┐             ┌──────┴──────┐
      │       │        │             │             │
    Stock   Order   Receiver       trader        market
                                    │              │
                               TraderFactory   MarketFactory
                                    │              │
                          ┌─────────┼──────┐   ┌────┼─────┐
                          ▼         ▼      ▼   ▼    ▼     ▼
                       Part1     Part2   ... Part1 Part2  ...
                       Trader    Trader       Market Market


AlgoTraderRepeat/
│
├── CMakeLists.txt
│
├── include/
│   └── algotrader/
│       │
│       ├── core/
│       │   ├── Stock.h
│       │   ├── Order.h
│       │   ├── Receiver.h
│       │   └── Properties.h
│       │
│       ├── trader/
│       │   ├── Trader.h              ← BASE CLASS
│       │   ├── TraderFactory.h
│       │   │
│       │   ├── Part1Trader.h         ← DERIVATIVE
│       │   ├── Part2Trader.h         ← DERIVATIVE
│       │   └── Part3Trader.h         ← DERIVATIVE
│       │
│       └── market/
│           ├── Market.h               ← BASE CLASS
│           ├── MarketFactory.h
│           ├── Part1Market.h          ← DERIVATIVE
│           └── Part2Market.h          ← DERIVATIVE
│
├── src/
│   │
│   ├── core/
│   │   ├── Stock.cpp
│   │   ├── Order.cpp
│   │   └── Receiver.cpp
│   │
│   ├── trader/
│   │   ├── Trader.cpp
│   │   ├── TraderFactory.cpp
│   │   ├── Part1Trader.cpp
│   │   ├── Part2Trader.cpp
│   │   └── Part3Trader.cpp
│   │
│   └── market/
│       ├── Market.cpp
│       ├── MarketFactory.cpp
│       ├── Part1Market.cpp
│       └── Part2Market.cpp
│
├── apps/
│   ├── trader/
│   │   └── main.cpp
│   └── market/
│       └── main.cpp
│
├── tests/
│
├── inputs/
│   └── data1.txt
│
└── build/