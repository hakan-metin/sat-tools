all: lib

lib: libsaucy.a

libsaucy.a : saucy.o
	$(AR) scr $@ $^

saucy.o: saucy.c
	$(CXX) -c -ansi -pedantic -Wall -O3 $< -o $@


clean:
	rm -f saucy.o libsaucy.a
