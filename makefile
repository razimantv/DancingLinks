all: dominosa sudoku nqueens

.PHONY: dominosa
dominosa:
	cd ./dominosa ; make dominosa

.PHONY: nqueens
nqueens:
	cd ./nqueens ; make nqueens

.PHONY: sudoku
sudoku:
	cd ./sudoku ; make sudoku

.PHONY: clean
clean:
	cd ./DancingLinks; make clean
	cd ./dominosa; make clean
	cd ./sudoku; make clean
