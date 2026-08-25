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