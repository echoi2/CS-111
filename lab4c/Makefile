#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
CFLAGS = -g -Wall -Wextra -lmraa -lm -lssl -lcrypto
CC = gcc

default:
	@./compile.sh
clean:
	@rm -f *.tar.gz lab4c_tcp lab4c_tls
dist:
	@tar -czvf lab4c-905368197.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README compile.sh
