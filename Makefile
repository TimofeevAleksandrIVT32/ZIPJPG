CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

OBJS = main.o archive_definition.o

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o res

main.o: main.c archive_definition.h
	$(CC) $(CFLAGS) -c main.c

archive_definition.o: archive_definition.c archive_definition.h
	$(CC) $(CFLAGS) -c archive_definition.c

clean:
	rm -f $(OBJS) res
