#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

if [ `uname -a | grep -c armv7l` -ge 1 ]
then
	gcc -g -Wall -Wextra -lmraa -lm lab4b.c -o lab4b
else
	gcc -lm -DDUMMY lab4b.c -o lab4b
fi
