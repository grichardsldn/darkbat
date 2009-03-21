all: main

clean:
	rm main *.o
CC=g++

source.o: Makefile source.cpp
	${CC} -c source.cpp

line.o: Makefile line.cpp
	${CC} -c line.cpp

main.o: Makefile main.cpp
	${CC} -c main.cpp

UDPSocket.o: Makefile UDPSocket.cpp
	${CC} -c UDPSocket.cpp

model.o: Makefile model.cpp
	${CC} -c model.cpp

window.o: Makefile window.cpp
	${CC} -c window.cpp

view.o: Makefile view.cpp
	${CC} -c view.cpp

main: Makefile main.o line.o model.o window.o view.o UDPSocket.o source.o
	${CC} -o main main.o line.o model.o window.o UDPSocket.o view.o source.o -lm -lpthread -L/usr/X11R6/lib -lX11


