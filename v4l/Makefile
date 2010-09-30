CC=g++

all:  windowtry

clean:
	rm windowtry xeg *.o

UDPSocket.o: Makefile UDPSocket.cpp UDPSocket.h
	${CC} -c -Wall UDPSocket.cpp

v4l.o: Makefile v4l.cpp
	${CC} -c -Wall v4l.cpp

window.o: Makefile window.cpp window.h
	${CC} -c -Wall -I/usr/X11R6/include window.cpp

windowtry.o: Makefile windowtry.cpp
	${CC} -c -Wall windowtry.cpp

windowtry: Makefile windowtry.o window.o
	${CC} -o windowtry windowtry.o window.o -L/usr/X11R6/lib -lX11

v4l: Makefile v4l.o UDPSocket.o
	${CC} -o v4l v4l.o UDPSocket.o

