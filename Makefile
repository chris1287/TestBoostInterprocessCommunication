CC=g++
STD=-std=c++0x
CFLAGS=-I/usr/include/boost
LIBS=-lpthread -lrt -lboost_program_options

all: test_queue

test_queue: test_queue.o
	$(CC) $(STD) $(LIBS) test_queue.o -o test_queue

test_queue.o: test_queue.cpp
	$(CC) $(STD) $(CFLAGS) -c test_queue.cpp

clean:
	rm -f *.o test_queue
