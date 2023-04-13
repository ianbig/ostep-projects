CC = gcc
CFLAGS = -g -pthread

OBJS = mapreduce.o
filecounts: $(OBJS) filecounts.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) filecounts.c

mapreduce.o: mapreduce.c mapreduce.h
	$(CC) $(CFLAGS) -c mapreduce.c

.PHONY: clean
clean:
	rm $(OBJS) filecounts