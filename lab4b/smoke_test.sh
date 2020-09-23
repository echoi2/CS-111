#!/bin/bash
#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID:905368197

PGM="lab4b"
LOG="LOGFILE"

let failures=0

./$PGM --scale=F --period=5 --log=$LOG <<-EOF  
START
STOP
LOG I'm Here
START
SCALE=C
PERIOD=3
OFF
EOF
if [ $? -ne 0 ]
then
	echo "FAILED. Did not exit with 0."
	let failures+=1
else
	echo "SUCCEEDED. Did exit with 0."
fi

if [ ! -s LOGFILE ]
then
	 echo "FAILED. Log file not created."
	 let failures+=1
else
	for c in START STOP "LOG I'm Here" START SCALE=C PERIOD=3 OFF SHUTDOWN
	do
		grep "$c" LOGFILE > /dev/null
		if [ $? -ne 0 ]
		then
			echo "FAILED. Command $c not logged."
			let failures+=1
		else
			echo "SUCCESS. Command $c was processed and logged."
		fi
	done
fi





./$PGM --Hello &> /dev/null;
if [ $? -ne 1 ]
then
	echo "FAILED. Did not exit with 1."
	let failures+=1
else
	echo "SUCCEEDED. Did exit with 1."
fi



./$PGM --scale=C --period=1 --log=$LOG <<-EOF
STOP
START
LOG Nice Day
LOG Hello Sir
OFF
EOF
if [ $? -ne 0 ]
then
	echo "FAILED. Did not exit with 0."
	let failures+=1
else
	echo "SUCCEEDED. Did exit with 0."
fi

if [ ! -s LOGFILE ]
then
	echo "FAILED. Log file not created."
	let failures+=1
else
	for c in STOP START "LOG Nice Day" "LOG Hello Sir" OFF SHUTDOWN
	do
		grep "$c" LOGFILE > /dev/null
		if [ $? -ne 0 ]
		then
			echo "FAILED.Command $c not logged."
			let failures+=1
		else
			echo "SUCCESS. Command $c was processed and logged."
		fi
	done
fi




if [ $failures -eq 0 ]
then
	echo "Smoke test passed."
else
	echo "Smoke test failed."
fi
