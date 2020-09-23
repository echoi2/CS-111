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

int sync_s = 0, sync_m = 0, opt_yield = 0, yield_i = 0, yield_d = 0, yield_l = 0, iteration_ct = 1, thread_ct = 1;
char* rand_key_holder = NULL; //need to have it here to free later
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long lock = 0;

void sig_handler(int sig);
void create_elements();
void* thread_worker(void* arg);

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
	{0, 0, 0, 0}};

	while((opt = getopt_long(argc, argv, "t:i:y:s:d",long_options, &option_index)) != -1){
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
			default:
				fprintf(stderr, "Error: The option/options provided are incorrect. The only options with arguments accepted are --threads=th_num, --iterations=it_num, --yield=[idl], and --sync=m or --sync=s" );
				exit(1);
		}
	}

	//creating a circular doubly empty linked list that uses a dummy_head
	dummy_head_list = malloc(sizeof(SortedList_t)); 
	dummy_head_list->next = dummy_head_list;
	dummy_head_list->prev = dummy_head_list;
	dummy_head_list->key = NULL;
	create_elements();

	struct timespec begin, end;

	if(clock_gettime(CLOCK_MONOTONIC, &begin) < 0){ //sets begin by the value that it grabs for clock_gettime indicating the starting time
		fprintf(stderr, "Error: Could not set start time to variable begin.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
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
			fprintf(stderr, "Error occurred while creating thread.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}

	for(j = 0; j < thread_ct; j++){
		if(pthread_join(thread[j], NULL) != 0){
			fprintf(stderr, "Error occurred while joining threads.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}



	if(clock_gettime(CLOCK_MONOTONIC, &end) < 0){
		fprintf(stderr, "Error: Could not set end time to variable end.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(1);
	}

	if(SortedList_length(dummy_head_list) != 0){
		fprintf(stderr, "Error: List size was not 0 after completion.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
		exit(2);
	}

	int total_operations_perf = thread_ct * iteration_ct * 3;

	

	long long total_time = (end.tv_sec - begin.tv_sec) * 1000000000L; //converting seconds into nanoseconds
														
	total_time += (end.tv_nsec - begin.tv_nsec);

	if(total_time < 0){ //Cause time should never be negative
		fprintf(stderr, "Error: Time cannot be negative.");
		exit(2);
	}

	


	long long avg_time_per_operations = total_time/total_operations_perf;

	



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
	
	fprintf(stdout, "%s,%d,%d,1,%d,%lld,%lld\n", selected_ops, thread_ct, iteration_ct, total_operations_perf, total_time, avg_time_per_operations);
	//have to free element, dummy_head_list, rand_key_holder
	//int release_ct = 0;
	/*while(release_ct < (thread_ct * iteration_ct)){
		free(elements[release_ct].key);
		release_ct++;
	}*/ //might not need to free because i did not malloc

	free(rand_key_holder);
	free(elements);
	free(dummy_head_list);
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
		rand_key_holder[i] = (char) rand_num;
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
	SortedListElement_t* elements_array_partition = arg;
	SortedListElement_t* lookup_ptr = NULL;

	for( ;i < iteration_ct;i++){ //inserts a partition of the elements array into the list
		if(sync_s == 1){
			while(__sync_lock_test_and_set(&lock, 1));
			SortedList_insert(dummy_head_list, &elements_array_partition[i]);
			__sync_lock_release(&lock);
		}


		else if(sync_m == 1){
			if(pthread_mutex_lock(&mutex) != 0){
				fprintf(stderr, "Error: Mutex failed to lock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(1);
			}
			
			SortedList_insert(dummy_head_list, &elements_array_partition[i]);
			
			if(pthread_mutex_unlock(&mutex) != 0){
				fprintf(stderr, "Error: Mutex failed to unlock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(1);
			}
		}
		else{
			SortedList_insert(dummy_head_list, &elements_array_partition[i]);
		}
	}

	if(sync_s == 1){
		while(__sync_lock_test_and_set(&lock, 1));
		if((list_length = SortedList_length(dummy_head_list)) < 0){
			fprintf(stderr, "Error: List length cannot be negative.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(2);
		}
		__sync_lock_release(&lock);
	}

	else if(sync_m == 1){
		if(pthread_mutex_lock(&mutex) != 0){
			fprintf(stderr, "Error: Mutex failed to lock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
		if((list_length = SortedList_length(dummy_head_list)) < 0){
			fprintf(stderr, "Error: List length cannot be negative.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(2);
		}
		if(pthread_mutex_unlock(&mutex) != 0){
			fprintf(stderr, "Error: Mutex failed to unlock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(1);
		}
	}

	else{
		if((list_length = SortedList_length(dummy_head_list)) < 0){
			fprintf(stderr, "Error: List length cannot be negative.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
			exit(2);
		}
	}

	for(i = 0; i < iteration_ct; i++){
		if(sync_s == 1){
			while(__sync_lock_test_and_set(&lock, 1));
			lookup_ptr = SortedList_lookup(dummy_head_list, elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(2);
			}
			__sync_lock_release(&lock);
		}


		else if(sync_m == 1){
			if(pthread_mutex_lock(&mutex) != 0){
				fprintf(stderr, "Error: Mutex failed to lock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(1);
			}

			lookup_ptr = SortedList_lookup(dummy_head_list, elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(2);
			}
			if(pthread_mutex_unlock(&mutex) != 0){
				fprintf(stderr, "Error: Mutex failed to unlock.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(1);
			}
		}


		else{
			lookup_ptr = SortedList_lookup(dummy_head_list, elements_array_partition[i].key);
			if(lookup_ptr != NULL){
				if((deleted = SortedList_delete(lookup_ptr)) == 1){
					fprintf(stderr, "Error: Deletion failed.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
					exit(2);
				}
			}
			else{
				fprintf(stderr, "Error: Element did not exist.\nMessage: %s\nExiting with error number: %d\n", strerror(errno), errno);
				exit(2);
			}
		}

	}
	return arg;
}

void sig_handler(int sig){
	if(sig == SIGSEGV){
    fprintf(stderr,"Recieved a Segmentation Fault.");
    exit(2);
  }
}