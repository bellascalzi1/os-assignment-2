CC=clang++
CFLAGS= -Wall -std=c++14

scheduler: scheduler.cpp
	$(CC) scheduler.cpp -o scheduler.out

clean:
	rm scheduler.out