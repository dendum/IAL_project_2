CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g #-fsanitize=address
FILES=hashtable.c test.c test_util.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)
	./test

test_muj: test_muj.c hashtable.c
	$(CC) $(CFLAGS) -o test_muj $^

test_muj_2: test_muj_2.c hashtable.c
	$(CC) $(CFLAGS) -o test_muj_2 $^
	./test_muj_2


clean:
	rm -f test test_muj test_muj_2
