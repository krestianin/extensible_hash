# Makefile targets
all: td

td: ExtensibleHashTable.o Bucket.o TestDriver.o
	g++ -std=c++17 -Wall -o td ExtensibleHashTable.o Bucket.o TestDriver.o

ExtensibleHashTable.o: ExtensibleHashTable.cpp ExtensibleHashTable.h Bucket.h
	g++ -std=c++17 -Wall -c ExtensibleHashTable.cpp

Bucket.o: Bucket.cpp Bucket.h
	g++ -std=c++17 -Wall -c Bucket.cpp

TestDriver.o: TestDriver.cpp ExtensibleHashTable.h
	g++ -std=c++17 -Wall -c TestDriver.cpp

clean:
	rm -f *.o td
