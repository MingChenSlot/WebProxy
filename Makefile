all: proxy

objects = server.o list.o

proxy: $(objects)
	gcc -o proxy -Wall -Werror $(objects)

server.o: server.c cache.c http.c socketio.c use_threads.c server.h
	gcc -O3 -c server.c cache.c http.c socketio.c use_threads.c

list.o: list.c list.h
	gcc -O3 -c list.c

clean:
	rm -rf *.o proxy
