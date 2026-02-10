CC = gcc
CFLAGS = -Wall -g

all: lex

lex: main.o lex.yy.o
	$(CC) $(CFLAGS) -o lex main.o lex.yy.o

lex.yy.c: kotlex.l
	flex kotlex.l

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	rm -f *.o lex lex.yy.c
