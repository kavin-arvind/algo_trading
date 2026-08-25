Phase 1 is as follows

### iml.cpp
Its internal Market Layer that communicates with the exchange and bring us the orders we need.

### data.txt
It simulates the actual exchanges such as NSE, NASDAQ, etc where we see different people placing order. But this file just includes history of orders placed.

### trader.cpp
Has a receiver that communicates with iml.cpp and getting info about orders. Implements the logic of algorithmic trading and we'll have a seperate thing to place order from this logic.
Here, we just try to predict and test our trading logic from the history.

### receiver.h
Receiver associated with trader.cpp to just get in the orders.