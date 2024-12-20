include ../makefile.common

spangram: spangram.o Trie.o $(commonobjs) $(commonhdrs)
	$(CC) -o spangram spangram.o Trie.o $(commonobjs) $(CPPFLAGS)

%.o: %.cpp $(commonhdrs)
	$(CC) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm -f *.o spangram
