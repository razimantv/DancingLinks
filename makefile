all: dominosa sudoku nqueens rectangles

.PHONY: dominosa
dominosa:
	cd ./dominosa ; make dominosa

.PHONY: nqueens
nqueens:
	cd ./nqueens ; make nqueens

.PHONY: sudoku
sudoku:
	cd ./sudoku ; make sudoku

.PHONY: rectangles
rectangles:
	cd ./rectangles ; make rectangles

.PHONY: clean
clean:
	cd ./DancingLinks; make clean
	cd ./dominosa; make clean
	cd ./nqueens; make clean
	cd ./sudoku; make clean
	cd ./rectangles; make clean
