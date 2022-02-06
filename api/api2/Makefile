OBJECTS=UDPSocket.o dkb_api.o

CFLAGS=-g

all: carpet dkbtest

carpet: ${OBJECTS} carpet.o Makefile
	g++ -o carpet carpet.o ${OBJECTS} -pthread

dkbtest: ${OBJECTS} dkbtest.o Makefile
	g++ -o dkbtest dkbtest.o ${OBJECTS} -lpthread

clean:
	rm carpet dkbtest dkbtest.o carpet.o $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

# DO NOT DELETE
