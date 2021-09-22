CC=clang++
CFLAGS= -Wall -std=c++14

scheduler: scheduler.cpp
	$(CC) $(CFLAGS) scheduler.cpp -o scheduler.out

baseline: baseline.cpp
	$(CC) $(CFLAGS) baseline.cpp -o baseline.out
clean:
	rm scheduler.out