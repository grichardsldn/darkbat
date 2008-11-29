CC=g++

all:  windowtry

clean:
	rm windowtry xeg *.o

window.o: Makefile window.cpp window.h
	${CC} -c -Wall -I/usr/X11R6/include window.cpp

windowtry.o: Makefile windowtry.cpp
	${CC} -c -Wall windowtry.cpp

windowtry: Makefile windowtry.o window.o
	${CC} -o windowtry windowtry.o window.o -L/usr/X11R6/lib -lX11

xeg: xeg.o events.o Makefile
	${CC} -o xeg xeg.o events.o -L/usr/X11R6/lib -lX11

xeg.o: xeg.c xeg.h Makefile
	${CC} -c -Wall -I/usr/X11R6/include xeg.c

events.o: events.c xeg.h Makefile
	${CC} -c -Wall -I/usr/X11R6/include events.c
