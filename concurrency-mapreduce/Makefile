CC = gcc
CFLAGS = -g -pthread

OBJS = mapreduce.o tpool.o

filecounts: $(OBJS) filecounts.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) filecounts.c
test: test_work_queue
	echo "compile test"
test_work_queue: $(OBJS) test_work_queue.c
	$(CC) $(CFLAGS) -o test_work_queue $(OBJS) test_work_queue.c

mapreduce.o: mapreduce.c mapreduce.h
	$(CC) $(CFLAGS) -c mapreduce.c
tpool.o: tpool.c tpool.h
	$(CC) $(CFLAGS) -c tpool.c


.PHONY: clean
clean:
	rm $(OBJS) filecounts