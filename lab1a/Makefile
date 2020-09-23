#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
CFLAGS = -g -Wall -Wextra
CC = gcc

lab1a: lab1a.c
	@$(CC) $(CFLAGS) lab1a.c -o lab1a
clean:
	@rm -f lab1a *tar.gz* *.txt* 
dist: 
	@tar -czvf lab1a-905368197.tar.gz Makefile README lab1a.c
