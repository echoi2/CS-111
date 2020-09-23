#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

CFLAGS = -g -Wall -Wextra -lpthread -lprofiler
CC = gcc

default:
	@$(CC) $(CFLAGS) lab2_list.c SortedList.c -o lab2_list
tests:
	@chmod 777 ./tests.sh
	@-./tests.sh
profile:
	@rm -f profile.out
	@LD_PRELOAD=/usr/lib64/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --iterations=1000 --threads=12 --sync=s 
	@/bin/pprof --text ./lab2_list ./raw.gperf > profile.out
	@/bin/pprof --list=thread_worker ./lab2_list ./raw.gperf >> profile.out
	@rm -f ./raw.gperf
graphs:
	@./lab2_list.gp
clean:
	@rm -f lab2_list *.tar.gz
dist: default tests profile graphs
	@tar -czvf lab2b-905368197.tar.gz lab2_list.c SortedList.h SortedList.c Makefile lab2b_list.csv profile.out lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png tests.sh README lab2_list.gp