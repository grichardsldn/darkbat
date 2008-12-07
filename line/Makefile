all: main
CC=g++

line.o: Makefile line.cpp
	${CC} -c line.cpp

main.o: Makefile main.cpp
	${CC} -c main.cpp

model.o: Makefile model.cpp
	${CC} -c model.cpp

window.o: Makefile window.cpp
	${CC} -c window.cpp

view.o: Makefile view.cpp
	${CC} -c view.cpp

main: Makefile main.o line.o model.o window.o view.o
	${CC} -o main main.o line.o model.o window.o view.o -lm -lpthread -L/usr/X11R6/lib -lX11


