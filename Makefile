CC = g++
CFLAGS = -Wall -Werror -std=c++11 -g 

OBJS = main.o

lab12: $(OBJS)
	$(CC) $(CFLAGS) -o lab12 $(OBJS)
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
.PHONY: clean
clean:
	rm -rf $(OBJS) lab12
