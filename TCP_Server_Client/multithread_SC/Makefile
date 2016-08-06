.PHONY:all
all:client server

client:client.c
	gcc -o $@ $^ -g 

server:server.c
	gcc -o $@ $^ -g -lpthread

.PHONY:clean
clean:
	rm -f server client
