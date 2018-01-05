include ../makefile.common

sudoku: sudoku.o $(commonobjs) $(commonhdrs)
	$(CC) -o sudoku sudoku.o $(commonobjs) $(CPPFLAGS)

%.o: %.cpp $(commonhdrs)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f *.o sudoku
