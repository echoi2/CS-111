#! /usr/bin/gnuplot
#
#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#	8. run time per lock operation(ns)
#
# output:
#	lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations
#	lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations
#	lab2b_3.png ... successful iterations vs. threads for each synchronization method
#	lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists
#	lab2b_4.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

#total operations per second for all threads combined
set title font ",10"
set title "List-1: Throughput vs. # of Threads for Mutex and Spin-Lock Sync. List Opts"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput (total_operations/sec)"
set logscale y 10
set output 'lab2b_1.png'

#https://stackoverflow.com/questions/51220749/difference-between-0-9-and-0-9 used to understand one of the regex used in the .gp file from 2a that is relevant here [0-9]*
plot \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'list w/spin' with linespoints lc rgb 'green', \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'list w/mutex' with linespoints lc rgb 'red'

#mean time per mutex wait and mean time per operation for mutex-synchronized list operations
set title "List-2: Mean Time Per Mutex Wait & Mean Time Per Opt For Mutex-Sync."
set xlabel "Threads"
set logscale x 2
set ylabel "Average Time per Opt. (ns)"
set logscale y 10
set output 'lab2b_2.png'

plot \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
      title 'list w/mutex wait-for-lock time' with linespoints lc rgb 'green', \
      "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
      title 'list w/mutex completion time' with linespoints lc rgb 'red'


#successful iterations vs. threads for each synchronization method
set title "List-3: Successful Iterations v. Threads For Each Sync. Method"
set xlabel "Threads"
set logscale x 2
set ylabel "Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'

plot \
     "< grep 'list-id-none,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
     title 'list w/yield=id no protection' with points lc rgb 'green', \
     "< grep 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
     title 'list w/yield=id & mutex protection' with points lc rgb 'red', \
     "< grep 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
     title 'list w/yield=id spin-lock protection' with points lc rgb 'blue'


#throughput vs. number of threads for mutex synchronized partitioned lists
set title "List-4: Throughput vs. # of Threads For Mutex Synchronized Partitioned Lists"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput (total_operations/sec)"
set logscale y 10
set output 'lab2b_4.png'

plot \
      "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'mutex list=1' with linespoints lc rgb 'green', \
      "< grep 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'mutex list=4' with linespoints lc rgb 'red', \
      "< grep 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'mutex list=8' with linespoints lc rgb 'blue', \
      "< grep 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'mutex list=16' with linespoints lc rgb 'pink'


#throughput vs. number of threads for spin-lock-synchronized partitioned lists
set title "List-5: Throughput vs. # of Threads For Spin-Lock-Synchronized Partitioned Lists"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput (total_operations/sec)"
set logscale y 10
set output 'lab2b_5.png'

plot \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'spin-lock list=1' with linespoints lc rgb 'green', \
      "< grep 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'spin-lock list=4' with linespoints lc rgb 'red', \
      "< grep 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'spin-lock list=8' with linespoints lc rgb 'blue', \
      "< grep 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
      title 'spin-lock list=16' with linespoints lc rgb 'pink'