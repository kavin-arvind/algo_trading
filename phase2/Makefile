#------------------------
# Do not modify this file
#------------------------

all: trader

# For part1: marked with a blank trader
market: main.cpp market.cpp worker.cpp blank_trader.cpp header.h
	g++ -std=c++20 main.cpp market.cpp worker.cpp blank_trader.cpp -lpthread -o market
	./market 2 > output.txt

# For part2
trader: main.cpp market.cpp worker.cpp trader.cpp
	g++ -std=c++20 main.cpp market.cpp worker.cpp trader.cpp -lpthread -o trader
	./trader 2 > output.txt

clean:
	rm -rf market trader output.txt
	rm -rf *~

test: test.cpp
	g++ -std=c++20 test.cpp -o test
	./test > test_output.txt