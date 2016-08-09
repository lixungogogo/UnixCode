.PHONY:all
all:server client

server:server.c 
	gcc -o $@ $^ -g -lpthread

client:client.c
	gcc -o $@ $^ -g
.PHONY:clean
clean:
	rm -f server client
