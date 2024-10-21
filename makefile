all: feed manager

feed: frontend/feed.o
	gcc -o frontend/feed frontend/feed.o

frontend/feed.o: frontend/feed.c
	gcc -c frontend/feed.c -o frontend/feed.o

manager: backend/manager.o
	gcc -o backend/manager backend/manager.o

backend/manager.o: backend/manager.c
	gcc -c backend/manager.c -o backend/manager.o

clean:
	rm -f frontend/*.o backend/*.o frontend/feed backend/manager