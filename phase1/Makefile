#------------------------
# Do not modify this file
#------------------------

all: trader iml

trader: trader.cpp receiver.h linked_list.h map.h
	g++ -std=c++20 trader.cpp -o trader
trader1_1: trader1_1.cpp receiver.h map.h
	g++ -std=c++20 trader1_1.cpp -o trader1_1

iml: iml.cpp
	g++ -std=c++20 iml.cpp -o iml

runpart1: iml trader
	./trader 1 > output.txt &
	sleep 1
	./iml inputs/data1.txt
	diff -Bw outputs/out1.txt output.txt

runpart1_own: iml trader1_1
	./trader1_1 1 > output.txt &
	sleep 1
	./iml input.txt

runpart2: iml trader
	./trader 2 > output.txt &
	sleep 1
	./iml inputs/data2.txt
	diff outputs/out2.txt output.txt

runpart2_own: iml trader
	./trader 2 > part2_output.txt &
	sleep 1
	./iml part2_testcases/input.txt


runpart3: iml trader
	./trader 3 > output.txt &
	sleep 1
	./iml inputs/data3.txt
	diff outputs/out3.txt output.txt

clean:
	rm -rf output.txt trader iml
	rm -rf *~
test_map: map.h linked_list.h
	g++ -g test_map.cpp -o testing
	./testing
