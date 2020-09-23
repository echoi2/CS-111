#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

CFLAGS = -g -Wall -Wextra -lmraa -lm 
CC = gcc

default:
	@./compile.sh
check: default
	@./smoke_test.sh
clean:
	@rm -f *.tar.gz lab4b
dist: 
	@tar -czvf lab4b-905368197.tar.gz lab4b.c Makefile README compile.sh smoke_test.sh
