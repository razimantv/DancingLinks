include ../makefile.common

rectangles: rectangles.o $(commonobjs) $(commonhdrs)
	$(CC) -o rectangles rectangles.o $(commonobjs) $(CPPFLAGS)

%.o: %.cpp $(commonhdrs)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f *.o rectangles
