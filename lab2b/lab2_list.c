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
#include <stdbool.h>
#include "SortedList.h"

SortedList_t* dummy_head_list;
SortedListElement_t* elements;

int sync_s = 0, sync_m = 0, opt_yield = 0, list_opt = 0, list_num = 1, yield_i = 0, yield_d = 0, yield_l = 0, iteration_ct = 1, thread_ct = 1;
char* rand_key_holder = NULL; //need to have it here to free later
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t* mutex;
//long lock = 0;
int* lock;
long long total_lock_acq_time = 0; //The acquisition time for a lock for all threads combined
//long long total_lock_acq_time2 = 0;
void sig_handler(int sig);
void create_elements();
void* thread_worker(void* arg);
void free_dynam_alloc_mem();
void sum_for_total_lock_acq_time(time_t tv_sec_begin, time_t tv_sec_end, long int tv_nsec_begin, long int tv_nsec_end);
int key_hash(const char* key);

int main(int argc, char * argv[]){
	int option_index = 0, opt = 0;
	char* yield_args = NULL;
	char* sync_arg = NULL;
	signal(SIGSEGV, sig_handler);

	static struct option long_options[] = {
	{"threads", required_argument, NULL, 't'},
	{"iterations", required_argument, NULL, 'i'},
	{"yield", required_argument, NULL, 'y'},
	{"sync", required_argument, NULL, 's'},
	{"lists", required_argument, NULL, 'l'},
	{0, 0, 0, 0}};

	while((opt = getopt_long(argc, argv, "t:i:y:s:l:d",long_options, &option_index)) != -1){
		switch(opt){
			case 't':
				thread_ct = atoi(optarg);
				break;
			case 'i':
				iteration_ct = atoi(optarg);
				break;
			case 'y':
				yield_args = optarg; //contains the args for yield
				unsigned int i = 0;
				while(i < strlen(yield_args)){
					if(yield_args[i] == 'i'){
						yield_i = 1;
						opt_yield |= INSERT_YIELD;
					}
					else if(yield_args[i] == 'd'){
						yield_d = 1;
						opt_yield |= DELETE_YIELD;
					}
					else if(yield_args[i] == 'l'){
						yield_l = 1;
						opt_yield |= LOOKUP_YIELD;
					}
					i++;
				}
				break; 
			case 's':
				sync_arg = optarg;
				if(*sync_arg == 'm'){
					sync_m = 1;
					break;
				}
				else if(*sync_arg == 's'){
					sync_s = 1;
					break;
				}
				break;
			case 'l':
				list_opt = 1;
				list_num  = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Error: The option/options provided are incorrect. The only options with arguments accepted are --threads=th_num, --iterations=it_num, --yield=[idl], --sync=m or --sync=s, and lists=#." );
				exit(1);
		}
	}

	//creating a circular doubly empty linked list that uses a dummy_head and making the correct amount depending on how many lists are requested
	dummy_head_list = malloc(sizeof(SortedList_t) * list_num); 
	int list_init_counter = 0;
	while(list_init_counter < list_num){
		dummy_head_list[list_init_counter].next = &dummy_head_list[list_init_counter];
		dummy_head_list[list_init_counter].prev = &dummy_head_list[list_init_counter];
		dummy_head_list[list_init_counter].key = NULL;
		list_init_counter++;
	}

	if(sync_m == 1){ //initializing the number of mutexes needed according to the requested amount of lists
		int lock_init_counter = 0;
		mutex = malloc(sizeof(pthread_mutex_t) * list_num);
		while(lock_init_counter < list_num){
			//https://linux.die.net/man/3/pthread_mutex_init
			if(pthread_mutex_init(&mutex[lock_init_counter], NULL) != 0){
				fprintf(stderr, "Error: Could not initialize mutex.  Exiting with error number: %d\n", 1);
				exit(1);
			}
			lock_init_counter++;
		}
	}

	else if(sync_s == 1){ //initializing the number of spin locks needed according to the requested amount of lists
		int lock_init_counter = 0;
		lock = malloc(sizeof(int) * list_num);
		while(lock_init_counter < list_num){
			lock[lock_init_counter] = 0;
			lock_init_counter++;
		}
	}


	//initializing the mutex for each list
	create_elements();



	struct timespec begin, end;

	if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //sets begin by the value that it grabs for clock_gettime indicating the starting time
		fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
		exit(1);
	}

	pthread_t thread[thread_ct];
	int j = 0;
	for(; j < thread_ct; j++){
		if(pthread_create(&thread[j], NULL, &thread_worker, (void *) (elements + (iteration_ct * j))) != 0){  //thread creation happens here
																				   //elements + (iteration_ct * i) is passing in the address of the elements in the elements array
																				   //https://stackoverflow.com/questions/16016920/type-casting-integer-to-void 
																				   //made use of type casting int to a void * b/c that is the parameter of thread_worker
			//printf("Creating threads %d\n", j);
			fprintf(stderr, "Error occurred while creating thread. Exiting with error number: %d\n", 1);
			exit(1);
		}
	}

	for(j = 0; j < thread_ct; j++){
		if(pthread_join(thread[j], NULL) != 0){
			fprintf(stderr, "Error occurred while joining threads. Exiting with error number: %d\n", 1);
			exit(1);
		}
	}



	if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){
		fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
		exit(1);
	}

	int iter = 0;
	while(iter < list_num){
		if(SortedList_length(&dummy_head_list[iter]) != 0){
			fprintf(stderr, "Error: List size was not 0 after completion. Exiting with error number: %d\n", 2);
			exit(2);
		}
		iter++;
	}
	atexit(free_dynam_alloc_mem);
	int total_operations_perf = thread_ct * iteration_ct * 3;
	int total_lock_operations_perf = thread_ct + (2 * thread_ct * iteration_ct);

	

	long long total_time = (end.tv_sec - begin.tv_sec) * 1000000000L; //converting seconds into nanoseconds
														
	total_time += (end.tv_nsec - begin.tv_nsec);

	if(total_time < 0){ //Cause time should never be negative
		fprintf(stderr, "Error: Time cannot be negative.");
		exit(2);
	}

	


	long long avg_time_per_operations = total_time/total_operations_perf;

	long long avg_wait_for_lock = total_lock_acq_time/total_lock_operations_perf;
	



	char* selected_ops = NULL;
	if(yield_i == 1  && yield_l == 1 && yield_d == 1 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-idl-none";
	}
	else if(yield_i == 1  && yield_l == 1 && yield_d == 0 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-il-none";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 0 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-i-none";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 1 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-id-none";
	}
	else if(yield_i == 0  && yield_l == 1 && yield_d == 1 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-dl-none";
	}
	else if(yield_i == 0  && yield_l == 1 && yield_d == 0 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-l-none";
	} 
	else if(yield_i == 0  && yield_l == 0 && yield_d == 1 && (sync_s == 0 && sync_m == 0)){
		selected_ops = "list-d-none";
	}



	else if(yield_i == 1  && yield_l == 1 && yield_d == 1 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-idl-s";
	}
	else if(yield_i == 1  && yield_l == 1 && yield_d == 0 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-il-s";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 0 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-i-s";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 1 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-id-s";
	}
	else if(yield_i == 0  && yield_l == 0 && yield_d == 0 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-none-s";
	} 
	else if(yield_i == 0  && yield_l == 1 && yield_d == 1 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-dl-s";
	}
	else if(yield_i == 0  && yield_l == 1 && yield_d == 0 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-l-s";
	} 
	else if(yield_i == 0  && yield_l == 0 && yield_d == 1 && (sync_s == 1 && sync_m == 0)){
		selected_ops = "list-d-s";
	}


	else if(yield_i == 1  && yield_l == 1 && yield_d == 1 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-idl-m";
	}
	else if(yield_i == 1  && yield_l == 1 && yield_d == 0 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-il-m";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 0 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-i-m";
	}
	else if(yield_i == 1  && yield_l == 0 && yield_d == 1 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-id-m";
	}
	else if(yield_i == 0  && yield_l == 0 && yield_d == 0 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-none-m";
	} 
	else if(yield_i == 0  && yield_l == 1 && yield_d == 1 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-dl-m";
	}
	else if(yield_i == 0  && yield_l == 1 && yield_d == 0 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-l-m";
	} 
	else if(yield_i == 0  && yield_l == 0 && yield_d == 1 && (sync_s == 0 && sync_m == 1)){
		selected_ops = "list-d-m";
	}

	else{
		selected_ops = "list-none-none";
	}
	
	fprintf(stdout, "%s,%d,%d,%d,%d,%lld,%lld,%lld\n", selected_ops, thread_ct, iteration_ct, list_num, total_operations_perf, total_time, avg_time_per_operations, avg_wait_for_lock);
	//have to free element, dummy_head_list, rand_key_holder
	//int release_ct = 0;
	/*while(release_ct < (thread_ct * iteration_ct)){
		free(elements[release_ct].key);
		release_ct++;
	}*/ //might not need to free because i did not malloc

	exit(0);
}



void create_elements(){ //creates the required number of elements and initializes each
	int element_ct = thread_ct * iteration_ct;
	elements = malloc(sizeof(SortedListElement_t) * element_ct);
	int i = 0;
	rand_key_holder = malloc(sizeof(char) * element_ct); //Create space for a random key array that will contain the values for the random key char
	//check on this 


	//https://kahdev.wordpress.com/2008/07/21/generating-random-characters-in-c/ learned how to make random chars for keys via this reference

	srand(time(NULL));
	int rand_num = 0;
	int counter = 0;
	for(; counter < element_ct; counter++){ //creates random character and populates the malloc'd array for containing random keys 
		rand_num = 26 * (rand()/(RAND_MAX + 1.0));
		rand_num += 97;
		rand_key_holder[counter] = (char) rand_num;
	}


	while(i < element_ct){ //iterates the entire rand_key_holder array one at a time to initialize a random key + \0 byte as the key for each element.
		char* key_insert[2];
		key_insert[0] = &rand_key_holder[i];
		key_insert[1] = '\0';
		elements[i].key = *key_insert;
		i++;
	}
}




void* thread_worker(void* arg){
	int list_length = 0;
	int deleted = 0;
	int i = 0;
	int key_location = 0;
	struct timespec begin, end;

	SortedListElement_t* elements_array_partition = arg;
	SortedListElement_t* lookup_ptr = NULL;



	///// insertion /////
	for( ;i < iteration_ct;i++){ //inserts a partition of the elements array into the list
		

		//essentially mods the key value with the number of lists to return back a list location to place the key.
		key_location = key_hash(elements_array_partition[i].key);


		if(sync_s == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}
			while(__sync_lock_test_and_set(&lock[key_location], 1));
			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}
			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);


			SortedList_insert(&dummy_head_list[key_location], &elements_array_partition[i]);
			__sync_lock_release(&lock[key_location]);
		}


		else if(sync_m == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(pthread_mutex_lock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to lock. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}
			
			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);



			SortedList_insert(&dummy_head_list[key_location], &elements_array_partition[i]);
			
			if(pthread_mutex_unlock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to unlock. Exiting with error number: %d\n", 1);
				exit(1);
			}
		}
		else{
			SortedList_insert(&dummy_head_list[key_location], &elements_array_partition[i]);
		}
	}





	///// finding list_length for each list /////

	for(key_location = 0; key_location < list_num; key_location++){
		if(sync_s == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}
			while(__sync_lock_test_and_set(&lock[key_location], 1));
			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}
			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);





			if((list_length = SortedList_length(&dummy_head_list[key_location])) < 0){
				fprintf(stderr, "Error: List length cannot be negative. Exiting with error number: %d\n", 2);
				exit(2);
			}
			__sync_lock_release(&lock[key_location]);
		}

		else if(sync_m == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(pthread_mutex_lock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to lock. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}
			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);



			if((list_length = SortedList_length(&dummy_head_list[key_location])) < 0){
				fprintf(stderr, "Error: List length cannot be negative. Exiting with error number: %d\n", 2);
				exit(2);
			}
			if(pthread_mutex_unlock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to unlock. Exiting with error number: %d\n", 1);
				exit(1);
			}
		}

		else{
			if((list_length = SortedList_length(&dummy_head_list[key_location])) < 0){
				fprintf(stderr, "Error: List length cannot be negative. Exiting with error number: %d\n", 2);
				exit(2);
			}
		}
	}












	///// lookup and deletion for each list /////
	for(i = 0; i < iteration_ct; i++){
		key_location = key_hash(elements_array_partition[i].key);
		if(sync_s == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}
			while(__sync_lock_test_and_set(&lock[key_location], 1));
			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}
			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);






			lookup_ptr = SortedList_lookup(&dummy_head_list[key_location], elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed. Exiting with error number: %d\n", 2);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist. Exiting with error number: %d\n", 2);
				exit(2);
			}
			__sync_lock_release(&lock[key_location]);
		}


		else if(sync_m == 1){
			if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //time a thread spends waiting before acquiring the lock
				fprintf(stderr, "Error: Could not set start time to variable begin. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(pthread_mutex_lock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to lock. Exiting with error number: %d\n", 1);
				exit(1);
			}

			if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){ //the time that is clocked in after the thread has acquired the lock
				fprintf(stderr, "Error: Could not set end time to variable end. Exiting with error number: %d\n", 1);
				exit(1);
			}

			sum_for_total_lock_acq_time(begin.tv_sec, end.tv_sec, begin.tv_nsec, end.tv_nsec);






			lookup_ptr = SortedList_lookup(&dummy_head_list[key_location], elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed. Exiting with error number: %d\n", 2);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist. Exiting with error number: %d\n", 2);
				exit(2);
			}
			if(pthread_mutex_unlock(&mutex[key_location]) != 0){
				fprintf(stderr, "Error: Mutex failed to unlock. Exiting with error number: %d\n", 1);
				exit(1);
			}
		}


		else{
			lookup_ptr = SortedList_lookup(&dummy_head_list[key_location], elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed. Exiting with error number: %d\n", 2);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist. Exiting with error number: %d\n", 2);
				exit(2);
			}
		}

	}
	return arg;
}


void sum_for_total_lock_acq_time(time_t tv_sec_begin, time_t tv_sec_end, long int tv_nsec_begin, long int tv_nsec_end){
	long long temp = 0;
	temp = (tv_sec_end - tv_sec_begin) * 1000000000L;
	temp += (tv_nsec_end - tv_nsec_begin);
	total_lock_acq_time += temp;
}

//https://www.tutorialspoint.com/cprogramming/c_data_types.htm The reason why you can mod char with integers in c is because char is considered an integer type in c.
int key_hash(const char* key){ 
	int ret_val = *key%list_num;
	return ret_val;
}

void free_dynam_alloc_mem(){
	free(rand_key_holder);
	free(elements);
	free(dummy_head_list);
	if(sync_s == 1){
		free(lock);
	}
	else if (sync_m == 1){
		free(mutex);
	}
}
void sig_handler(int sig){
	if(sig == SIGSEGV){
    fprintf(stderr,"Recieved a Segmentation Fault.");
    exit(2);
  }
}