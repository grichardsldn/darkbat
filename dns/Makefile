OBJECTS=UDPSocket.o dns.o main.o

CFLAGS=-g

all: $(OBJECTS) main

main: Makefile $(OBJECTS)
	g++ -o main $(OBJECTS)

clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

# DO NOT DELETE
