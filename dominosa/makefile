include ../makefile.common

dominosa: dominosa.o $(commonobjs) $(commonhdrs)
	$(CC) -o dominosa dominosa.o $(commonobjs) $(CPPFLAGS)

%.o: %.cpp $(commonhdrs)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm *.o dominosa
