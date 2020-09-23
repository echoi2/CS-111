#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
CFLAGS = -g -Wall -Wextra 
CC = gcc

lab0: lab0.c
	@$(CC) $(CFLAGS) lab0.c -o lab0

check: lab0 readTest writeTest correctArgsIOTest priorityTest incorrectArgsTest storingTest
	@if [ -e successes.txt ] && [ ! -e failures.txt ]; then \
		echo "Smoke Test Passed!"; \
		rm -f successes.txt; \
	else \
		echo "Smoke Test Failed!"; \
		rm -f successes.txt failures.txt; \
	fi

#checks to see if you try and read from an unreadable file that it should exit with 2.
readTest:
	@echo "Hola Amigos" > testA.txt; \
	chmod u-r testA.txt; \
	./lab0 --input testA.txt &> /dev/null; \
	if [ $$? -ne 2 ]; then \
		echo "$@ FAILED. Did not exit with 2.\n" >> failures.txt; \
		rm -f testA.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 2.\n" >> successes.txt; \
		rm -f testA.txt; \
	fi

#checks to see if you try and write to an unwritable file that it should exit with 2.
writeTest:
	@touch testA.txt; \
	chmod u-w testA.txt;\
	./lab0 --output testA.txt &> /dev/null; \
	if [ $$? -ne 3 ]; then \
		echo "$@ FAILED. Did not exit with 3.\n" >> failures.txt; \
		rm -f testA.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 3.\n" >> successes.txt; \
		rm -f testA.txt; \
	fi

#checks that if correct arguments are taken in for input and output, it should return 0.
correctArgsIOTest:
	@echo "Hola Amigos" > testA.txt; \
	touch testB.txt; \
	chmod 777 testA.txt; \
	chmod 777 testB.txt; \
	./lab0 --input testA.txt --output testB.txt &> /dev/null; \
	if [ $$? -ne 0 ]; then \
		echo "$@ FAILED. Did not exit with 0.\n" >> failures.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 0.\n" >> successes.txt; \
	fi

#checks to see that exit 4 should happen 1st when all 4 options are chosen. This also checks for if segfault is caught.
priorityTest:
	@echo "Hola Amigos" > testA.txt; \
        touch testB.txt; \
        chmod 777 testA.txt; \
        chmod 777 testB.txt; \
	./lab0 --input testA.txt --output testB.txt --segfault --catch &> /dev/null; \
	if [ $$? -ne 4 ]; then \
		echo "$@ FAILED. Did not exit with 4.\n" >> failures.txt; \
                rm -f testA.txt testB.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 4.\n" >> successes.txt; \
                rm -f testA.txt testB.txt; \
	fi

#checks that the options/arguments input for ./lab0 are correct.
incorrectArgsTest:
	@./lab0 --joe_Rogan &> /dev/null; \
	if [ $$? -ne 1 ]; then \
		echo "$@ FAILED. Did not exit with 1.\n" >> failures.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 1.\n" >> successes.txt; \
	fi

#Shows that if test passes, even though output is put as the first option, the arguments are actually being stored
#to variables and then being checked in the desired order which begins with --input first.
storingTest:
	@echo "Hola Amigos" > testA.txt; \
	touch testB.txt; \
	chmod u-r testA.txt; \
	chmod u-w testB.txt; \
	./lab0 --output testB.txt --input testA.txt &> /dev/null; \
	if [ $$? -ne 2 ]; then \
		echo "$@ FAILED. Did not exit with 2.\n" >> failures.txt; \
		rm -f testA.txt testB.txt; \
	else \
		echo "$@ SUCCEEDED. Did exit with 2.\n" >> successes.txt; \
		rm -f testA.txt testB.txt; \
	fi
dist:
	@tar -czvf lab0-905368197.tar.gz Makefile README lab0.c *.png
clean:
	@rm -f lab0-905368197.tar.gz lab0 *.txt* *~ functions.sh
