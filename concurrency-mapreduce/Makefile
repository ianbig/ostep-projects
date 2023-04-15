CC = gcc
CFLAGS = -g -pthread

OBJS = mapreduce.o tpool.o hashmap.o
TESTS = test_thread_pool

filecounts: $(OBJS) filecounts.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) filecounts.c
test: $(TESTS)
	echo "compile test"
test_thread_pool: tpool.c tpool.h test_thread_pool.c
	$(CC) $(CFLAGS) -o $@ tpool.c test_thread_pool.c -DDEBUG
test_value: $(OBJS) test_value.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) test_value -DDEBUG

mapreduce.o: mapreduce.c mapreduce.h
	$(CC) $(CFLAGS) -c mapreduce.c
tpool.o: tpool.c tpool.h
	$(CC) $(CFLAGS) -c tpool.c
hashmap.o: hashmap.h hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c


.PHONY: clean
clean:
	rm $(OBJS) filecounts $(TESTS)