all: term_scroll

term_scroll: term_scroll.cpp libviewport.so
	g++ -I../.. -L. -pthread -lviewport term_scroll.cpp -o term_scroll

libviewport.so: ../../ViewPort.cpp ../../ViewPort.h
	g++ -fPIC ../../ViewPort.cpp -shared -o libviewport.so -Wl,--whole-archive -Wl,--no-whole-archive

.PHONY: run clean

run: term_scroll
	LD_LIBRARY_PATH=. ./term_scroll

clean:
	rm -f libviewport.so term_scroll
