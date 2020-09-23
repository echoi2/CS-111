#NAME: Eugene Choi
#EMAIL: echoi2@g.ucla.edu
#ID: 905368197

default:
	@rm -f lab3b #used to remove the old symbolic link because you can't make another symbolic link with same name if one already exists
	@ln -s lab3b.py lab3b #creates a symbolic link to lab3b.py
	@chmod 777 lab3b #gives rwx permission to be able to actually run the "executable". 
clean:
	@rm -f *tar.gz lab3b
dist: 
	@tar -czvf lab3b-905368197.tar.gz lab3b.py Makefile README