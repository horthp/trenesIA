CC=g++

CFLAGS= -std=c++11 -Wall

all: bep

bep: BEP.cpp

	$(CC) $(CFLAGS) BEP.cpp -o BEP

clean:
	rm -rf *o BEP
