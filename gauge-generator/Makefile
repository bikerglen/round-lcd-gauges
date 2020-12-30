all: examples clockish

examples: examples.o dialgen.o ptrgen.o
	g++ -o examples examples.o dialgen.o ptrgen.o \
			`pkg-config --cflags --libs MagickWand`

clockish: clockish.o dialgen.o ptrgen.o
	g++ -o clockish clockish.o dialgen.o ptrgen.o \
			`pkg-config --cflags --libs MagickWand`

examples.o: examples.cpp dialgen.h ptrgen.h
	g++ -c examples.cpp `pkg-config --cflags MagickWand`

clockish.o: clockish.cpp dialgen.h ptrgen.h
	g++ -c clockish.cpp `pkg-config --cflags MagickWand`

dialgen.o: dialgen.cpp dialgen.h
	g++ -c dialgen.cpp `pkg-config --cflags MagickWand`

ptrgen.o: ptrgen.cpp ptrgen.h
	g++ -c ptrgen.cpp `pkg-config --cflags MagickWand`
