GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all: part1 part2 part3

part1: part1_client part1_server

part2: 

part3: 

part1_client:
	$(GCC) $(FLAGS) -o part1_client part1/client.c

part1_server:
	$(GCC) $(FLAGS) -o part1_server part1/server.c

clean: 
	 rm -f part1_client part1_server
