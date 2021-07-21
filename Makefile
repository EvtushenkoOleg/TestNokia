nameProg=testNokia
nameExe=$(nameProg).exe
nameTests=test1.csv

.PHONY: all clean test

all:
	g++ -o $(nameExe) $(nameProg).cpp
clean:
	rm -rf *.exe
test:
	for testIndex in $(nameTests) ; do \
		./$(nameExe) $$testIndex ; \
	done