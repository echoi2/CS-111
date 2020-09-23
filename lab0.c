/*NAME: Eugene Choi
  Email: echoi2@g.ucla.edu
  ID: 905368197
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define has_arg 1
#define no_arg 0

void createSF();
void sfCatcher(int sig);

int main(int argc, char* argv[]){
  int opt = 0;
  int optIndex = 0;
  int input_flag = 0;
  int output_flag = 0;
  int sig_flag = 0;
  int catch_flag = 0;
  ssize_t readable;
  char char_buf;
  char* input_fname = NULL;
  char* output_fname = NULL;
  
  /* Found from https://linux.die.net/man/3/getopt_long
     Formatting for option struct
     struct option{
       const char * name;
       int has_arg; //if value is 0 (no arguments). if value is 1(requires arguments)
                    //if valuse is 2 (arguments are optional)
       int *flag; //basically points to val and uses that as how results are returned 
                  //for a long option. If flag is NULL, getopt_long() returns val. Else
		  //returns 0.
       int val; //the value to return
     }
  */
  
  //referenced from notes from Discussion 1A posted on CCLE pg 31-37 and from linux.die.net/man/3/getopt_long.
  static struct option long_options[] = {
    {"input", has_arg, NULL, 'i'},
    {"output", has_arg, NULL, 'o'},
    {"segfault", no_arg, NULL, 's'},
    {"catch", no_arg, NULL, 'c'},
    {0, 0, 0, 0}};

  //referenced from notes from Discussion 1A posted on CCLE pg 31-37 and from linux.die.net/man/3/getopt_long.
  while((opt = getopt_long(argc, argv, "i:o:sc", long_options, &optIndex)) != -1){
    switch(opt){
    case 'i':
      input_flag = 1;
      input_fname = optarg;
      break;
    case 'o':
      output_flag = 1;
      output_fname = optarg;
      break;
    case 's':
      sig_flag = 1;
      break; 
    case 'c':
      catch_flag = 1;
      break;
    default:
      fprintf(stderr, "Bad Arguements\n. Correct Arguements: ./lab0 [--input filename] [--output filename] [--segfault] [--catch].\n");
      exit(1);
    }
  }
  /* Order for operations when multiple arguments are specified.
     1. do any file redirection.
     2. register the sig handler.
     3. cause segfault.
     4. if no segfault was caused, copy stdin to stdout.
     5. if both input and output options have invalid arguments, exit code should be first invalid arg. detected. (Done in the same section as #1)*/

  if(input_flag == 1){ //input redirect referenced from fd_juggiling.html link provided to us.
    int ifd = open(input_fname, O_RDONLY);
    if(ifd >= 0){
      close(0);
      dup(ifd);
      close(ifd);
    }
    else{
      fprintf(stderr, "Error with --input. Could not open input file: %s.\n(Error number %d) Error message returned: %s", input_fname, errno, strerror(errno));
      exit(2);
    }
  }
  
  if(output_flag == 1){ //output redirect referenced from fd_juggling.html link provided to us.
    int ofd = creat(output_fname, 0666);
    if(ofd >= 0) {
      close(1);
      dup(ofd);
      close(ofd);
    }
    else{
      fprintf(stderr, "Error with --output. Could not create output file: %s.\n(Error number %d) Error message returned: %s", output_fname, errno, strerror(errno));
      exit(3);
    }
  }
  
  if(catch_flag == 1){
    signal(SIGSEGV, sfCatcher); //The second argument is a funtion that takes in an integer
                                //argument and handles the signal specified by the first argument.
  }
  
  if(sig_flag == 1){
    createSF();
  }

  //if you reach here, then that means no sigfault was caused. Now copying stdin to stdout.
  //Referenced from my own work done back in 35L for an assignment that worked with file IO.
  while((readable = read(0, &char_buf, 1)) > 0){
    write(1, &char_buf, 1);
  }
  exit(0);
}

void createSF(){
  char* ptr = NULL;
  *ptr = 'e';
}
  
void sfCatcher(int sig){
  //referenced from notes from Discussion 1A pg 48.
  if(sig == SIGSEGV){
    fprintf(stderr, "Caught signal %d which is a segmentation fault.\n", sig);
    exit(4);
  }
}
