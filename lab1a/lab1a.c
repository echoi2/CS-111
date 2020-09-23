//NAME: Eugene Choi
//EMAIL: echoi2@g.ucla.edu
//UID: 905368197

#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <poll.h>

struct termios save_beginning; //Struct that saves the current terminal mode
pid_t pid;
int pipe_ts[2]; //pipe from terminal to shell
int pipe_st[2]; //pipe from shell to terminal
void set_to_nc_input(); //setting mode to non-cannonical input mode
void reset_input_mode(); //resets back to cannonical input mode (restores original state)
void read_input_k_to_b(); //reading in input from input from keyboard into buffer
void sig_handler(int sig);
void read_input_kb(int* exit_D_or_C);
void read_input_shell();
 

int main(int argc, char* argv[]){
  int option_index = 0;
  int opt = 0;
  int shell_flag = 0;
  int child_status;
  char* name_of_file;
  static struct option long_options[] = {
    {"shell", required_argument, NULL, 's'},
    {0,0,0,0}};
  while((opt = getopt_long(argc, argv, "s", long_options, &option_index)) != -1){
    switch(opt){
    case 's':
      shell_flag = 1;
      name_of_file = optarg;
      break;
    default:
      fprintf(stderr,"Error: The argument/arguments provided are incorrect. The only arguments excepted are --shell and --debug.");
      reset_input_mode();
      exit(1);
    }
  }
  set_to_nc_input();
  if(shell_flag == 1){
    if((pipe(pipe_ts)) != 0){
      fprintf(stderr,"Failed to create a pipe from the terminal to the shell.\n");
      reset_input_mode();
      exit(1);
    }
    if((pipe(pipe_st)) != 0){
      fprintf(stderr,"Failed to create a pipe from the shell to the terminal.\n");
      reset_input_mode();
      exit(1);
    }
    signal(SIGPIPE, sig_handler); //Now if SIGPIPE appears anywhere after, then this signal function call will handle it.

    if((pid = fork()) == -1){
      fprintf(stderr,"Error while forking");
      reset_input_mode();
      exit(1);
    }
    //This is the child process
    if(pid == 0){
      close(pipe_ts[1]); //closes the write from terminal to shell
      close(pipe_st[0]); //closes the read from shell to terminal
      dup2(pipe_ts[0],0);
      dup2(pipe_st[1],1);
      dup2(pipe_st[1],2);
      close(pipe_st[1]); //Replaced using dup2, so now we just close because it's not being used.
      close(pipe_ts[0]);
      char* arguments[2]; //An array of character pointers to NULL-terminated strings. Your application must ensure that the last member of this array is a NULL
                          //pointer. The value in argv[0] must point to a filename that's associated with the process being started. Neither argv nor the value in 
                          //argv[0] can be NULL. 
      arguments[0] = name_of_file;
      arguments[1] = NULL; 
      if((execvp(name_of_file, arguments)) == -1){ //executes the shell and also checks whether or not execution is successful
		fprintf(stderr,"Error: Execution of shell in child process was not successful.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
    }
    else if (pid > 0){ //parent process
      close(pipe_ts[0]);
      close(pipe_st[1]);
      int exit_D_or_C;
      //https://pubs.opengroup.org/onlinepubs/009695399/functions/poll.html
      struct pollfd fds[2];
      int poll_val;
      fds[0].fd = 0; //keyboard
      fds[1].fd = pipe_st[0]; //pipe that returns output from the shell. (Read in value from shell)
      fds[0].events = POLLIN | POLLHUP | POLLERR;
      fds[1].events = POLLIN | POLLHUP | POLLERR;
      while(1){
		poll_val = poll(fds, 2, 0);
		if(poll_val > 0){
			if(fds[0].revents & POLLIN){	    
				read_input_kb(&exit_D_or_C);
			}
			if(fds[1].revents & POLLIN){
				read_input_shell();
			}
			if(exit_D_or_C == 1){
				close(pipe_ts[1]);
				close(pipe_st[0]);
				if(kill(pid, SIGHUP) < 0){
					waitpid(pid, &child_status, 0);
	 		 		fprintf(stderr, "Error: Problem with child process.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  				reset_input_mode();	
	 				exit(1);
				}
				waitpid(pid, &child_status, 0);
				fprintf(stderr,"SHELL EXIT SIGNAL=%d STATUS=%d ", WIFSIGNALED(child_status), WEXITSTATUS(child_status));
				reset_input_mode();
				exit(0);
			} //This is case 0x04
			if(exit_D_or_C == 2){
				if(kill(pid, SIGINT) < 0){
					waitpid(pid, &child_status, 0);
					fprintf(stderr, "Error: Problem with child process.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  				reset_input_mode();	
	 				exit(1);
				}
			}
	  }
	  	else if(poll_val < 0){
	  		fprintf(stderr, "Error: Poll creation failed.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  		reset_input_mode();
	  		exit(1);
		}	
	  } //closes while(1) loop
	
    } //closes else if (pid > 0)
  }
  else{
    read_input_k_to_b();
  }
  reset_input_mode();
  exit(0);
}



void set_to_nc_input(){ 
  int grabbedAttributes = tcgetattr(0, &save_beginning);
  if(grabbedAttributes < 0){
    fprintf(stderr, "Error: Could not grab attributes.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
    reset_input_mode();
    exit(1);
  }
  atexit(reset_input_mode);
  struct termios copy_att;
  tcgetattr(0, &copy_att); //grabs and copies attributes from file descriptor into copy_att
  copy_att.c_iflag = ISTRIP;
  copy_att.c_oflag = 0;
  copy_att.c_lflag = 0;
  int setting_changes = tcsetattr(0, TCSANOW, &copy_att);
  if(setting_changes < 0){
    fprintf(stderr, "Error: Could not change attributes.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
    reset_input_mode();
    exit(1);
  }
}

void reset_input_mode(){
  if(tcsetattr(STDIN_FILENO, TCSANOW, &save_beginning) < 0){
    fprintf(stderr, "Error: Could not restore original terminal attributes.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
    exit(1);
  }
}

void read_input_k_to_b(){ //reading input from keyboard to buffer
  char curr_buf[16];
  char CRLF[2] = {'\r','\n'};
  char curr_char;
  ssize_t readable;
  int i = 0;
  while((readable = read(0, &curr_buf, 16)) > 0){
    i = 0;
    while(i < readable){
      curr_char = curr_buf[i];
      switch(curr_char){
      case '\r':
      case '\n':
		if((write(1, &CRLF[0], 1)) < 0){
	  		fprintf(stderr, "Error: Could not write from CRLF to stdin.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  		reset_input_mode();
	  		exit(1);
		}
		if((write(1, &CRLF[1], 1)) < 0){
	  		fprintf(stderr, "Error: Could not write from CRLF to stdin.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  		reset_input_mode();
	  		exit(1);
		}
		break;
      case 0x04:
		reset_input_mode();
		exit(0);
		break;
      default:
		if((write(1, &curr_char, 1)) < 0){
	  		fprintf(stderr, "Error: Could not write from curr_buf to stdin.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	  		reset_input_mode();
	  		exit(1);
		}
		break;	
      }
      i++;
    }
  }
}

void sig_handler(int sig){
  if(sig == SIGPIPE){
    fprintf(stderr,"Recieved a SIGPIPE. Getting ready to shut-down.");
    exit(0);
    //some additional stuff here
  }
}

void read_input_kb(int* exit_D_or_C){ //reading input from keyboard
  char curr_buff[256];
  char curr_char;
  ssize_t readable;
  char CRLF[2] = {'\r','\n'};
  readable = read(0, &curr_buff, 256);
  int i = 0;
  while(i < readable){
    curr_char = curr_buff[i];
    switch(curr_char){
    case '\r':
    case '\n':
      if((write(1, &CRLF[0], 1)) < 0){
	fprintf(stderr, "Error: Could not echo CR to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	reset_input_mode();
	exit(1);
      }
      if((write(1, &CRLF[1], 1)) < 0){
	fprintf(stderr, "Error: Could not echo LF to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	reset_input_mode();
	exit(1);
      }
      if((write(pipe_ts[1], &CRLF[1], 1)) < 0){
	fprintf(stderr, "Error: Could not forward LF to shell.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
	reset_input_mode();
	exit(1);
      }
      break;
    case 0x04:
      *exit_D_or_C = 1;
      break;
    case 0x03:
      *exit_D_or_C = 2;
      break;
    default:
      if((write(1, &curr_char, 1)) < 0){
		fprintf(stderr, "Error: Could not echo input to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
      if((write(pipe_ts[1], &curr_char, 1)) < 0){
		fprintf(stderr, "Error: Could not forward input to shell.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
      break;
    }
    i++;
  }
}

void read_input_shell(){ //reading input from shell
  char curr_buff[256];
  char curr_char;
  ssize_t readable;
  char CRLF[2] = {'\r','\n'};
  readable = read(pipe_st[0], &curr_buff, 256);
  int i = 0;
  while(i < readable){
    curr_char = curr_buff[i];
    if((curr_char == '\n')||(curr_char == '\r')){
      if((write(1, &CRLF[0], 1)) < 0){
		fprintf(stderr, "Error: Could not echo CR to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
      if((write(1,&CRLF[1],1)) < 0){
		fprintf(stderr, "Error: Could not echo CR to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
    }
    else{
      if((write(1, &curr_char, 1)) < 0){
		fprintf(stderr, "Error: Could not write from shell to stdout.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		reset_input_mode();
		exit(1);
      }
    }
    i++; 
  }
}

