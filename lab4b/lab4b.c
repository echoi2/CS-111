//NAME: Eugene Choi
//EMAIL: echoi2@g.ucla.edu
//ID: 905368197

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef DUMMY
#define MRAA_GPIO_IN 0
#define MRAA_GPIO_EDGE_RISING 1


typedef int mraa_aio_context;
typedef int mraa_gpio_context;
typedef void mraa_result_t;
typedef int mraa_gpio_dir_t;
typedef int mraa_gpio_edge_t;

mraa_aio_context mraa_aio_init(__attribute__((unused)) int p){
	return 0;
}

mraa_gpio_context mraa_gpio_init(__attribute__((unused)) int k){
	return 60;
}

mraa_result_t mraa_gpio_dir(__attribute__((unused)) mraa_gpio_context d, __attribute__((unused)) mraa_gpio_dir_t direct){
}

mraa_result_t mraa_gpio_isr(__attribute__((unused)) mraa_gpio_context d, __attribute__((unused)) mraa_gpio_edge_t e, __attribute__((unused)) void(*fptr)(void*), __attribute__((unused)) void* args){	
}

int mraa_aio_read(__attribute__((unused)) mraa_aio_context c){
	return 650;
}

void mraa_aio_close(__attribute__((unused)) mraa_aio_context c){
}

void mraa_gpio_close(__attribute__((unused)) mraa_gpio_context d){
}

#else
#include <mraa.h>
#include <mraa/gpio.h>
#include <mraa/aio.h>
#endif
 
int opt_stop_or_start = 1, opt_off = 0; //opt_stop_or_start defaulted to 1 which is start
int opt_scale_c_or_f = 1, period = 1, opt_log = 0, log_fd; //opt_scale_c_or_f = 0 is c & = 1 is f (scale defaulted to 1 for f), period defaulted to 1 second
char* filename = NULL;

const int B = 4275; //thermistor value
const int R0 = 100000; //nominal base value

mraa_aio_context temp_sensor;
mraa_gpio_context button;

struct timespec ts;
struct tm * tm;

float convert_temp();
void read_input();
void options_compare(char compare_buff[]);
void button_pressed(); //also works for shutdown when the OFF option is read from stdin

int main(int argc, char * argv[]){
	int option_index = 0, opt = 0;

	static struct option long_options[] = {
	{"scale", required_argument, NULL, 's'},
	{"period", required_argument, NULL, 'p'},
	{"log", required_argument, NULL, 'l'},
	{0,0,0,0}};

	while((opt = getopt_long(argc, argv, "s:p:l:d",long_options, &option_index)) != -1){
		switch(opt){	
			case 's':
				if(*optarg == 'C'){
					opt_scale_c_or_f = 0;
					break;
				}
				else if (*optarg == 'F'){
					break;
				}
				else{
					fprintf(stderr, "Error: Incorrect scale option\n");
					exit(1);
				}



			case 'p':
				period = atoi(optarg); //sets period to user defined/desired seconds
				if((period == 0) || (period < 0)){
					fprintf(stderr, "Error: Period should not be negative.\n");
					exit(1);
				}
				break;



			case 'l':
				filename = optarg;
				opt_log = 1;
				if(filename == NULL){
					fprintf(stderr, "Error: Invalid filename.\n");
					exit(1);
				}
				else{
					if((log_fd = creat(filename, 0777)) == -1){
						fprintf(stderr, "Error: Could not create log file.\n");
						exit(1);
					}
					break;
				}



			default:
				fprintf(stderr, "Error: The option/options provided are incorrect. The only options with arguments accepted are --SCALE=f or c, --PERIOD=seconds, and --LOG=filename.\n" ); 
				exit(1);
		}
	}

	//initialization of temp_sensor & button as mraa_aio_context and mraa_gpio_context variables respectively.
	temp_sensor = mraa_aio_init(0);
	button = mraa_gpio_init(73);
	mraa_gpio_dir(button, MRAA_GPIO_IN);
	mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &button_pressed, NULL);
	//initializing pollfd
	int poll_value;
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN | POLLHUP | POLLERR;

	//after initializing, we want to grab temperature and local times
	
	
	time_t next_report_time;
	while(1){
		clock_gettime(CLOCK_REALTIME, &ts); //https://linux.die.net/man/3/localtime
		tm = localtime(&(ts.tv_sec));
		float temperature_val = convert_temp();
		if(opt_stop_or_start == 1 && ts.tv_sec >= next_report_time){ 
			fprintf(stdout,"%02d:%02d:%02d %.1f\n",tm->tm_hour, tm->tm_min, tm->tm_sec, temperature_val); //https://stackoverflow.com/questions/19431674/rounding-floats-to-nearest-10th
			if(opt_log == 1){
				dprintf(log_fd,"%02d:%02d:%02d %.1f\n",tm->tm_hour, tm->tm_min, tm->tm_sec, temperature_val);
			}
			next_report_time = ts.tv_sec + period;
		}

		poll_value = poll(fds, 1, 0);
		if(poll_value > 0){
			if(fds[0].revents & POLLIN){
				read_input();
			}
			if(opt_off == 1){
				button_pressed();
			}
		}

		else if(poll_value < 0){
			fprintf(stderr, "Error: poll failed.\n");
			exit(1);
		}
	}

	mraa_aio_close(temp_sensor);
	mraa_gpio_close(button);
	exit(0);
}

//Referenced from https://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/#software
//and https://iotdk.intel.com/docs/master/mraa/aio_8h.html for the mraa_aio_read
float convert_temp(){
	int input_voltage = mraa_aio_read(temp_sensor);
	if(input_voltage < 0){
		fprintf(stderr, "Error: mraa_aio_read returned an error.\n");
		exit(1);
	}
	float R = 1023.0/input_voltage-1.0;
	R = R0*R;
	float temperature_val_c = 1.0/(log(R/R0)/B+1/298.15)-273.15;

	//https://www.rapidtables.com/convert/temperature/how-kelvin-to-celsius.html
	if(opt_scale_c_or_f == 1){ //https://blog.prepscholar.com/convert-celsius-to-fahrenheit
		float temperature_val_f = (temperature_val_c*1.8)+32;
		return temperature_val_f;
	} 
	return temperature_val_c;
}

void read_input(){
	char char_buff[2048];
	char compare_buff[2048]; // buffer used to compare against START, STOP, OFF, SCALE=F, SCALE=C, PERIOD=seconds, LOG line of text options when they are input into stdin while program is running
	char curr_char;
	char* hold = compare_buff; //Holds the position for the new command after the 1st command is read in up to the \n character.
	int readable;
	if((readable = read(0, &char_buff, 2048)) == -1){ //reads from standard input and store into char_buf
		fprintf(stderr, "Error: Could not read from stdin\n");
		exit(1);
	}
	int k = 0;
	while(k < readable){
		curr_char = char_buff[k];
		switch(curr_char){
			case '\n': //stop adding characters to comparison buffer and send them over to be compared to the options mentioned in the comment for compare_buff
				compare_buff[k] = '\0';
				options_compare(hold);
				memset(compare_buff, 0, sizeof(compare_buff)); //https://www.quora.com/What-is-the-best-way-to-clear-an-array-using-C-language wanted to clear to avoid having multiple null terminating characters
				hold = &compare_buff[k+1]; 
				break;
			default: //if you aren't at the end of char_buff, then add the curr_char into compare_buff
				compare_buff[k] = curr_char;
				break;
		}
		k++;
	}
} 

void options_compare(char compare_buff[]){
	int new_period = 0;
	char * comparator = compare_buff;
	char * temporary  = NULL;
	//printf("%s\n",comparator); //for checking
	if(strcmp(comparator, "START") == 0){

		if(opt_stop_or_start == 0){
			opt_stop_or_start = 1; //sets from stop to start
		}
		if(opt_log == 1){
			dprintf(log_fd,"START\n");
		}
	}

	else if(strcmp(comparator, "STOP") == 0){
		if(opt_stop_or_start == 1){
			opt_stop_or_start = 0; //sets from start to stop
			//printf("got here! \n");
		}
		if(opt_log == 1){
			//printf("got here! \n");
			dprintf(log_fd,"STOP\n");
		}
	}


	else if(strcmp(comparator, "OFF") == 0){
		if(opt_off == 0){
			opt_off = 1; //sets to off mode
		}
		if(opt_log == 1){
			dprintf(log_fd,"OFF\n");
		}
	}


	else if(strcmp(comparator, "SCALE=F") == 0){
		if(opt_scale_c_or_f == 0){
			opt_scale_c_or_f = 1; //changes it from c to f
		}
		if(opt_log == 1){
			dprintf(log_fd,"SCALE=F\n");
		}
	}



	else if(strcmp(comparator, "SCALE=C") == 0){
		if(opt_scale_c_or_f == 1){
			opt_scale_c_or_f = 0; //changes it from f to c
		}
		if(opt_log == 1){
			dprintf(log_fd,"SCALE=C\n");
		}
	}



	else if(strstr(comparator, "PERIOD=") != NULL){
		temporary = &comparator[7]; //makes temporary point to where the new value of period should be obtained from in the compare_buff
		if((new_period = atoi(temporary)) <= 0){ //setting new period value to whatever temporary was pointing to.
			fprintf(stderr,"Error: The new period value cannot be zero or negative\n");
			exit(1);
		}
		period = new_period;
		if(opt_log == 1){
			dprintf(log_fd,"%s\n", comparator);
		}
	}

	//https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
	
	else if(strstr(comparator, "LOG") != NULL){
		if(opt_log == 1){ //onlt write to log_fd if opt_log was selected from command line
			dprintf(log_fd, "%s\n", comparator);
		}
	}

	else{ //if the compare_buff passed has no matches, then the option is invalid and we can log
		if(opt_log == 1){
			dprintf(log_fd, "%s\n", comparator);
			dprintf(log_fd, "Error: Option provided was not valid. Please try another.\n");
		}
		fprintf(stdout, "Error: Option provided was not valid. Please try another.\n");
	}
}

void button_pressed(){
	clock_gettime(CLOCK_REALTIME, &ts);
	tm = localtime(&(ts.tv_sec));
	fprintf(stdout,"%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
	if(opt_log == 1){
		dprintf(log_fd,"%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
	exit(0);
}