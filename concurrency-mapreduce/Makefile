CC = gcc
CFLAGS = -g -pthread

OBJS = mapreduce.o tpool.o hashmap.o
TESTS = test_value test_list test_hashmap

filecounts: $(OBJS) filecounts.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) filecounts.c

test: $(TESTS)
	echo "compile test"
test_thread_pool: tpool.c tpool.h test_thread_pool.c
	$(CC) $(CFLAGS) -o $@ tpool.c test_thread_pool.c -DDEBUG
test_value: hashmap.h hashmap.c test_value.c
	$(CC) $(CFLAGS) -o $@ hashmap.c test_value.c -DDEBUG
test_list: hashmap.h hashmap.c test_list.c
	$(CC) $(CFLAGS) -o $@ hashmap.c test_list.c -DDEBUG
test_hashmap: hashmap.h hashmap.c test_hashmap.c
	$(CC) $(CFLAGS) -o $@ test_hashmap.c hashmap.c -DDEBUG

mapreduce.o: mapreduce.c mapreduce.h
	$(CC) $(CFLAGS) -c mapreduce.c
tpool.o: tpool.c tpool.h
	$(CC) $(CFLAGS) -c tpool.c
hashmap.o: hashmap.h hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c


.PHONY: clean
clean:
	rm $(OBJS) filecounts $(TESTS)