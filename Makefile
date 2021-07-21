nameProg=testNokia
nameExe=$(nameProg).exe
nameTests=test_default.csv test_true.csv test_notCorrectNameCell.csv test_notCorrectTable.csv test_divZero.csv

.PHONY: all clean test

all:
	g++ -o $(nameExe) $(nameProg).cpp
clean:
	rm -rf *.exe
test:
	for testIndex in $(nameTests) ; do \
		./$(nameExe) $$testIndex ; \
	done