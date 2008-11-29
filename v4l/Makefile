CC=g++

all:  windowtry

clean:
	rm windowtry xeg *.o

v4l.o: Makefile v4l.cpp
	${CC} -c -Wall v4l.cpp

window.o: Makefile window.cpp window.h
	${CC} -c -Wall -I/usr/X11R6/include window.cpp

windowtry.o: Makefile windowtry.cpp
	${CC} -c -Wall windowtry.cpp

windowtry: Makefile windowtry.o window.o v4l.o
	${CC} -o windowtry windowtry.o window.o v4l.o -L/usr/X11R6/lib -lX11

