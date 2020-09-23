//NAME: Eugene Choi
//EMAIL: echoi2@g.ucla.edu
//ID: 905368197

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

//Long long signed integer type. Capable of containing at least the [−9,223,372,036,854,775,807, +9,223,372,036,854,775,807] range. It is at least 64 bits in size.
long long counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //not sure if this is gonna cause problems?
long lock = 0;
int yield = 0, sync_s = 0, sync_c = 0, sync_m = 0;
void add(long long *pointer, long long value);
void opt_mutex_add(long long *pointer, long long value);
void opt_spin_add(long long *pointer, long long value);
void atomic_add(long long* pointer, long long value);
void* thread_worker(void* arg);

int main(int argc, char * argv[]){
	int option_index = 0, opt = 0, thread_ct = 1, iteration_ct = 1; //thread count is defaulted to 1 b/c 0 threads is essentially an exited process. Therefore a process
																									   //should have 1 or more threads to be able have the program execute (uses thread execution model). 


	static struct option long_options[] = {
	{"threads", required_argument, NULL, 't'},
	{"iterations", required_argument, NULL, 'i'},
	{"yield", no_argument, NULL, 'y'},
	{"sync", required_argument, NULL, 's'},
	{0, 0, 0, 0}};


	while((opt = getopt_long(argc, argv, "t:i:ys:d", long_options, &option_index)) != -1){
		switch(opt){
			case 't':
				thread_ct = atoi(optarg);
				break;
			case 'i':
				iteration_ct = atoi(optarg);
				break;
			case 'y':
				yield = 1;
				break;
			case 's':
				if(*optarg == 'm'){
					sync_m = 1;
					break;
				}
				else if(*optarg == 's'){
					sync_s = 1;
					break;
				}
				else if(*optarg == 'c'){
					sync_c = 1;
					break;
				}
				else{
					break;
				}
			default:
				fprintf(stderr, "Error: The option/options provided are incorrect. The only options with arguments accepted are --threads=th_num, --iterations=it_num, --yield, and --sync=m or --sync=s or --sync=c." );
				exit(1);
		}
	}

	//referenced the timespec class and set up from https://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance
	//as well as from https://stackoverflow.com/questions/3523442/difference-between-clock-realtime-and-clock-monotonic
	struct timespec begin, end;

	if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //sets begin by the value that it grabs for clock_gettime indicating the starting time
		fprintf(stderr, "Error: Could not set start time to variable begin.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(1);
	}
	//printf("thread count is %d\n", thread_ct); //to check if the thread count was unaltered
	pthread_t thread[thread_ct];
	//printf("thread containers created\n"); //used to check if thread containers were created

	int i = 0; //counter used for the loop
	for(; i < thread_ct; i++){
		if(pthread_create(&thread[i], NULL, &thread_worker, &iteration_ct) != 0){  //thread creation happens here
																				   //https://stackoverflow.com/questions/16016920/type-casting-integer-to-void 
																				   //made use of type casting int to a void * b/c that is the parameter of thread_worker
			//printf("Creating threads %d\n", i);
			fprintf(stderr, "Error occurred while creating thread.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
		//printf("complete %d threads\n", i);
	}

	for(i = 0; i < thread_ct; i++){
		if(pthread_join(thread[i], NULL) != 0){
			fprintf(stderr, "Error occurred while joining threads.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}

	if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){
		fprintf(stderr, "Error: Could not set end time to variable end.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(1);
	}

	long long total_time = (end.tv_sec - begin.tv_sec) * 1000000000L; //converting seconds into nanoseconds
														
	total_time += (end.tv_nsec - begin.tv_nsec);

	if(total_time < 0){ //Cause time should never be negative
		fprintf(stderr, "Error: Time cannot be negative.");
		exit(2);
	}

	int total_operations_perf = thread_ct * iteration_ct * 2;

	long long avg_time_per_operations = total_time/total_operations_perf;

	


	char* selected_ops = NULL;
	if(yield == 1 && (sync_s == 0 && sync_c == 0 && sync_m == 0)){
		selected_ops = "add-yield-none";
	}
	else if(yield == 1 && sync_s == 1){
		selected_ops = "add-yield-s";
	}
	else if(yield == 0 && sync_s == 1){
		selected_ops = "add-s";
	}
	else if(yield == 1 && sync_c == 1){
		selected_ops = "add-yield-c";
	} 
	else if(yield == 0 && sync_c == 1){
		selected_ops = "add-c";
	}
	else if(yield == 1 && sync_m == 1){
		selected_ops = "add-yield-m";
	} 
	else if(yield == 0 && sync_m == 1){
		selected_ops = "add-m";
	}
	else{
		selected_ops = "add-none";
	}
	fprintf(stdout, "%s,%d,%d,%d,%lld,%lld,%lld\n", selected_ops, thread_ct, iteration_ct, total_operations_perf, total_time, avg_time_per_operations, counter);


	exit(0);
}

void* thread_worker(void* arg){
	int i = 0;
									//https://www.circuitstoday.com/void-pointers-in-c
	int iteration_ct = *((int*)arg);//needed to make use of this recasting and dereferencing to be able to convert back the int varaible 
									//that was type casted to a void * for the pthread_create parameter to be able to use the original int form of the variable in the function									
									//(int*)ptr - is used for type casting. Where as *((int*)ptr) dereferences the typecasted void pointer variable.
	
	//printf("The value of iteration_ct is %d\n", iteration_ct); //test to check if iteration_ct ever changed from the casting or if it was acting funny
	for(; i < iteration_ct; i++){
		if(sync_m == 1){
			opt_mutex_add(&counter, 1);
		}
		else if(sync_s == 1){
			opt_spin_add(&counter, 1);
		}
		else if(sync_c == 1){
			atomic_add(&counter, 1);
		}
		else{
			add(&counter, 1);
		}
		//printf("printing value of counter after adding %d\n", counter);
	}
	for(i = 0; i < iteration_ct; i++){
		if(sync_m == 1){
			opt_mutex_add(&counter, -1);
		}
		else if(sync_s == 1){
			opt_spin_add(&counter, -1);
		}
		else if(sync_c == 1){
			atomic_add(&counter, -1);
		}
		else{
			add(&counter, -1);
		}
		//printf("printing value of counter after subtracting %d\n", counter);
	}
	return arg;
}

void opt_mutex_add(long long *pointer, long long value){
	if(pthread_mutex_lock(&mutex) != 0){
		fprintf(stderr, "Error: Mutex failed to lock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(1);
	}
	long long sum = *pointer + value;


	if(yield == 1){
		if(sched_yield() < 0){
			fprintf(stderr, "Error: Calling thread could not yield.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}


	*pointer = sum;
	if(pthread_mutex_unlock(&mutex) != 0){
		fprintf(stderr, "Error: Mutex failed to unlock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(1);
	}		

}

void opt_spin_add(long long *pointer, long long value){
	while(__sync_lock_test_and_set(&lock, 1));
	long long sum = *pointer + value;
	if(yield == 1){
		if(sched_yield() < 0){
			fprintf(stderr, "Error: Calling thread could not yield.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}
	*pointer = sum;	
	__sync_lock_release(&lock);
}


void atomic_add(long long* pointer, long long value){
	long long prev, sum;
	do{
		if(yield == 1){
			if(sched_yield() < 0){
				fprintf(stderr, "Error: Calling thread could not yield.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(1);
			}
		}
		prev = *pointer;
		sum = prev + value;
	}while(__sync_val_compare_and_swap(pointer, prev, sum) != prev);
}






void add(long long *pointer, long long value){
	long long sum = *pointer + value;
	if(yield == 1){
		if(sched_yield() < 0){
			fprintf(stderr, "Error: Calling thread could not yield.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}
	*pointer = sum;	
}