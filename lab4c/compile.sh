#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

if [ `uname -a | grep -c armv7l` -ge 1 ]
then
	gcc -g -Wall -Wextra -lmraa -lm lab4c_tcp.c -o lab4c_tcp
	gcc -g -Wall -Wextra -lmraa -lm -lssl -lcrypto lab4c_tls.c -o lab4c_tls
else
	gcc -lm -DDUMMY lab4c_tcp.c -o lab4c_tcp
	gcc -lm -DDUMMY -lssl -lcrypto lab4c_tls.c -o lab4c_tls
fi	
