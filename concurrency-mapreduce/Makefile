CC = gcc
CFLAGS = -g -pthread

OBJS = mapreduce.o tpool.o
filecounts: $(OBJS) filecounts.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) filecounts.c

mapreduce.o: mapreduce.c mapreduce.h
	$(CC) $(CFLAGS) -c mapreduce.c
tpool.o: tpool.c tpool.h
	$(CC) $(CFLAGS) -c tpool.c

.PHONY: clean
clean:
	rm $(OBJS) filecounts