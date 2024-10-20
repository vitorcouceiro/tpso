all: feed

feed: feed.o
	gcc -o feed feed.o

feed.o: feed.c
	gcc -c feed.c

clean:
	rm -f *.o feed
