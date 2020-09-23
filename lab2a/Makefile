#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

CFLAGS = -g -Wall -Wextra -lpthread
CC = gcc

build:
	@$(CC) $(CFLAGS)  lab2_add.c -o lab2_add    # -lpthread tells gcc that your program must be linked with pthreads library
	@$(CC) $(CFLAGS)  lab2_list.c SortedList.c -o lab2_list  
tests: 
	@chmod 777 ./test.sh
	@-./test.sh
	
graphs:
	@./lab2_add.gp 
	@./lab2_list.gp
clean:
	@rm -f lab2_add lab2_list *.tar.gz
dist:build tests graphs
	@tar -czvf lab2a-905368197.tar.gz lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile lab2_add.csv lab2_list.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png README lab2_add.gp lab2_list.gp test.sh