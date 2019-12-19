GCC = gcc
FLAGS = -Wall -Wextra --std=gnu99 -pedantic -Werror -g

all: part1 part2 part3

part1: part1_client part1_server

part2: part2_mutex part2_rw part2_sem_posix part2_sem_sysv

part3: 

part1_client:
	$(GCC) $(FLAGS) -o part1_client part1/client.c

part1_server:
	$(GCC) $(FLAGS) -o part1_server part1/server.c

part2_mutex:
	$(GCC) $(FLAGS) -pthread -o part2_mutex part2/alphabet.c part2/mutex.c

part2_rw:
	$(GCC) $(FLAGS) -pthread -o part2_rw part2/alphabet.c part2/rw.c

part2_sem_posix:
	$(GCC) $(FLAGS) -pthread -o part2_sem_posix part2/alphabet.c part2/sem_posix.c

part2_sem_sysv:
	$(GCC) $(FLAGS) -pthread -o part2_sem_sysv part2/alphabet.c part2/sem_sysv.c

clean: 
	 rm -f part1_client part1_server part2_mutex part2_rw part2_sem_posix part2_sem_sysv
