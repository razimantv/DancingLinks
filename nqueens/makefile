include ../makefile.common

nqueens: nqueens.o $(commonobjs) $(commonhdrs)
	$(CC) -o nqueens nqueens.o $(commonobjs) $(CPPFLAGS)

%.o: %.cpp $(commonhdrs)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f *.o nqueens
