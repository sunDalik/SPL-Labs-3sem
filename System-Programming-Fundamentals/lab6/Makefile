GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -g

all: client server

#add -lsocket -lnsl to compile those on helios
client:
	$(GCC) $(FLAGS) -o client client.c

server:
	$(GCC) $(FLAGS) -pthread -o server server.c

clean: 
	 rm -f client server
