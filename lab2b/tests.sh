#!/bin/bash

#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
#used to learn how to write a test script: https://medium.com/@codingmaths/bin-bash-what-exactly-is-this-95fc8db817bf#:~:text=%2Fbin%2Fbash%20is%20the%20most,well%20developed%20and%20better%20syntax.

#https://stackoverflow.com/questions/14219092/bash-script-and-bin-bashm-bad-interpreter-no-such-file-or-directory (fixed my script compilation problem)
rm -f lab2b_list.csv
#Mutex synchronized list operations, 1,000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv


#Spin-lock synchronized list operations, 1,000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

#Run your program with --yield=id, 4 lists, 1,4,8,12,16 threads, and 1, 2, 4, 8, 16 iterations
./lab2_list --threads=1 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=1 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=1 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=1 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=1 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

#Run your program with --yield=id, 4 lists, 1,4,8,12,16 threads, and 10, 20, 40, 80 iterations, --sync=s and --sync=m
#sync=s
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

#sync=m
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv

#Rerun both synchronized versions, without yields, for 1000 iterations, 1,2,4,8,12 threads, and 1,4,8,16 lists.
#sync=s w/o yield

./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv

#sync=m w/o yield

./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
