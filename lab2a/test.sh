#!/bin/bash

#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
#used to learn how to write a test script: https://medium.com/@codingmaths/bin-bash-what-exactly-is-this-95fc8db817bf#:~:text=%2Fbin%2Fbash%20is%20the%20most,well%20developed%20and%20better%20syntax.

#https://stackoverflow.com/questions/14219092/bash-script-and-bin-bashm-bad-interpreter-no-such-file-or-directory (fixed my script compilation problem)
sed -i -e 's/\r$//' ./test.sh 

rm -f lab2_add.csv
rm -f lab2_list.csv
	
#Run your program for ranges of iterations (100, 1000, 10000, 100000) values
./lab2_add --iterations=100 --threads=1 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=100 --threads=2 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100 --threads=4 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100 --threads=12 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=12 >> lab2_add.csv

#Re-run your tests, with yields, for ranges of threads (2,4,8,12) and iterations (10, 20, 40, 80, 100, 1000, 10000, 100000)
./lab2_add --iterations=10 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=10 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=4 --yield >> lab2_add.csv
./lab2_add --iterations=10 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=10 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=20 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=40 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=80 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=12 --yield >> lab2_add.csv

#Compare the average execution time of the yield and non-yield versions a range threads (2, 8) and of iterations (100, 1000, 10000, 100000)
./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv
./lab2_add --iterations=100 --threads=2 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=100 --threads=8 >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=100000 --threads=8 >> lab2_add.csv


#Plot (using the supplied data reduction script), for a single thread, the average cost per operation (non-yield) as a function of the number of iterations.
./lab2_add --iterations=100 --threads=1  >> lab2_add.csv
./lab2_add --iterations=1000 --threads=1  >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1  >> lab2_add.csv
./lab2_add --iterations=100000 --threads=1  >> lab2_add.csv
./lab2_add --iterations=100 --threads=1 --yield >> lab2_add.csv
./lab2_add --iterations=1000 --threads=1 --yield >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 --yield >> lab2_add.csv
./lab2_add --iterations=100000 --threads=1 --yield >> lab2_add.csv

#Use your --yield options to confirm that, even for large numbers of threads (2, 4, 8, 12) and iterations (10,000 for mutexes and CAS, only 1,000 for spin locks) that reliably failed in the unprotected scenarios, all three of these serialization mechanisms eliminate the race conditions in the add critical section.
./lab2_add --iterations=10000 --threads=2 --yield --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --yield --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --yield --sync=m >> lab2_add.csv
./lab2_add --iterations=1000 --threads=2 --yield --sync=s >> lab2_add.csv
./lab2_add --iterations=1000 --threads=4 --yield --sync=s >> lab2_add.csv
./lab2_add --iterations=1000 --threads=8 --yield --sync=s >> lab2_add.csv
./lab2_add --iterations=1000 --threads=12 --yield --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --yield --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --yield --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --yield --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --yield --sync=c >> lab2_add.csv

#Using a large enough number of iterations (e.g. 10,000) to overcome the issues raised in the question 2.1.3, test all four (no yield) versions (unprotected, mutex, spin-lock, compare-and-swap) for a range of number of threads (1,2,4,8,12)
./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --sync=m >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --sync=s >> lab2_add.csv
./lab2_add --iterations=10000 --threads=1 --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=2 --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=4 --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=8 --sync=c >> lab2_add.csv
./lab2_add --iterations=10000 --threads=12 --sync=c >> lab2_add.csv

#Run your program with a single thread, and increasing numbers of iterations (10, 100, 1000, 10000, 20000)
./lab2_list --iterations=10 --threads=1 >> lab2_list.csv
./lab2_list --iterations=100 --threads=1 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=1 >> lab2_list.csv
./lab2_list --iterations=10000 --threads=1 >> lab2_list.csv
./lab2_list --iterations=20000 --threads=1 >> lab2_list.csv
./lab2_list --iterations=10 --threads=2 >> lab2_list.csv
./lab2_list --iterations=100 --threads=2 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=2 >> lab2_list.csv
./lab2_list --iterations=10000 --threads=2 >> lab2_list.csv
./lab2_list --iterations=20000 --threads=2 >> lab2_list.csv
./lab2_list --iterations=10 --threads=4 >> lab2_list.csv
./lab2_list --iterations=100 --threads=4 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=4 >> lab2_list.csv
./lab2_list --iterations=10000 --threads=4 >> lab2_list.csv
./lab2_list --iterations=20000 --threads=4 >> lab2_list.csv
./lab2_list --iterations=10 --threads=8 >> lab2_list.csv
./lab2_list --iterations=100 --threads=8 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=8 >> lab2_list.csv
./lab2_list --iterations=10000 --threads=8 >> lab2_list.csv
./lab2_list --iterations=20000 --threads=8 >> lab2_list.csv
./lab2_list --iterations=10 --threads=12 >> lab2_list.csv
./lab2_list --iterations=100 --threads=12 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 >> lab2_list.csv
./lab2_list --iterations=10000 --threads=12 >> lab2_list.csv
./lab2_list --iterations=20000 --threads=12 >> lab2_list.csv

#Run your program and see how many parallel threads (2,4,8,12) and iterations (1, 10,100,1000) it takes to fairly consistently demonstrate a problem
./lab2_list --iterations=1 --threads=2 >> lab2_list.csv
./lab2_list --iterations=10 --threads=2 >> lab2_list.csv
./lab2_list --iterations=100 --threads=2 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=2 >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 >> lab2_list.csv
./lab2_list --iterations=10 --threads=4 >> lab2_list.csv
./lab2_list --iterations=100 --threads=4 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=4 >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 >> lab2_list.csv
./lab2_list --iterations=10 --threads=8 >> lab2_list.csv
./lab2_list --iterations=100 --threads=8 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=8 >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 >> lab2_list.csv
./lab2_list --iterations=10 --threads=12 >> lab2_list.csv
./lab2_list --iterations=100 --threads=12 >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 >> lab2_list.csv

#run it again using various combinations of yield options and see how many threads (2,4,8,12) and iterations (1, 2,4,8,16,32) it takes to fairly consistently demonstrate the problem
./lab2_list --iterations=1 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=i >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=i >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=i >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=i >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=i >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=d >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=d >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=d >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=d >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=d >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=il >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=il >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=il >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=il >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=il >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=dl >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=dl >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=dl >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=dl >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=dl >> lab2_list.csv

#Using your --yield options, demonstrate that either of these protections seems to eliminate all of the problems, even for moderate numbers of threads (12) and iterations (32)
./lab2_list --iterations=1 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=i --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=i --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=i --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=i --sync=s >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=d --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=d --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=d --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=d --sync=s >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=il --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=il --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=il --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=il --sync=s >> lab2_list.csv
	
./lab2_list --iterations=1 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=dl --sync=m >> lab2_list.csv

./lab2_list --iterations=1 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=2 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=4 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=8 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=1 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=2 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=4 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=8 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=16 --threads=12 --yield=dl --sync=s >> lab2_list.csv
./lab2_list --iterations=32 --threads=12 --yield=dl --sync=s >> lab2_list.csv

#Choose an appropriate number of iterations (e.g. 1000) to overcome start-up costs and rerun your program without the yields for a range of # threads (1, 2, 4, 8, 12, 16, 24).
./lab2_list --iterations=1000 --threads=1 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=2 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=4 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=8 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=16 --sync=m >> lab2_list.csv
./lab2_list --iterations=1000 --threads=24 --sync=m >> lab2_list.csv
	
./lab2_list --iterations=1000 --threads=1 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=2 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=4 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=8 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=12 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=16 --sync=s >> lab2_list.csv
./lab2_list --iterations=1000 --threads=24 --sync=s >> lab2_list.csv
