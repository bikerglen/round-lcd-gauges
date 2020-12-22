basic: basic.o display.o
	g++ -o basic basic.o display.o -llept -lpigpio

basic.o: basic.c display.h
	g++ -c basic.c

display.o: display.c display.h
	g++ -c display.c
