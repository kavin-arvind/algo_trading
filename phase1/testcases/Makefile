# Define the source files and output files
SOURCES = input1.txt input2.txt input3.txt input4.txt input5.txt input6.txt input7.txt input8.txt input9.txt input10.txt input11.txt input12.txt input13.txt input14.txt input15.txt input16.txt
OUTPUTS = output1.txt output2.txt output3.txt output4.txt output5.txt output6.txt output7.txt output8.txt output9.txt output10.txt output11.txt output12.txt output13.txt output14.txt output15.txt output16.txt

# Define the common compile and run commands
CXX = g++
CXXFLAGS = -std=c++20
TRADER = ../trader
IML = ../iml
SLEEP = sleep 1
DIFF = diff -Bw

# Define the rules
all: $(OUTPUTS)

# Rule to compile the C++ code
iml: $(IML)
	$(CXX) $(CXXFLAGS) $(IML).cpp -o $(IML)

trader: $(TRADER)
	$(CXX) $(CXXFLAGS) $(TRADER).cpp -o $(TRADER)


# Rule to generate an output file for each input file
output%.txt: input%.txt iml trader
	$(TRADER) 1 > ../output.txt &
	$(SLEEP) 
	$(IML) $< 
	$(SLEEP) 

	$(DIFF) ../output.txt $@

# Clean up temporary files
clean:
	rm -f $(IML) ../output.txt

.PHONY: all clean
