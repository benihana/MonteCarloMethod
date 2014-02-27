/** This program finds a mathematical function value very close to the minimum using a Monte Carlo (random sampling) method. Given a large number of random samples (maybe 1,000,000) for x between -10 <= x <= 10, evaluate the function x^2 at every point, and find the value of x that yields the smallest number for x^2. **/

#include <time.h>
#include "prodcons.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "mt64.h"

double start;
static sem_t full;  // Semaphore to track the number of full items in the buffer
static sem_t empty; // Semaphore to track the number of empty slots in the buffer
static sem_t mutex; // Semaphore to provide mutual exclusion on the buffer
node * head_node;
const int NUM_WORKER_THREADS;
const int NUM_SAMPLES_PER_THREAD;
const int NUM_ITERATIONS_PER_THREAD;
const double X_MIN_BOUND;
const double X_MAX_BOUND;
const double Y_MIN_BOUND;
const double Y_MAX_BOUND;
int error_flag = 0;
int thread_id = 0;
int error_code;

int main(int argc, char ** argv)
{
	unsigned long long init[4]={0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL};
	unsigned long long length=4;
	init_by_array64(init, length);

	printf("Provide your input on one line in the following order:\n");
	printf("1:x minimum, 2:x maximum, 3:y minimum, 4:y maximum, 5:number of samples, \n");
	printf("6:number of iterations per thread, and 7:number of threads ");
	scanf("%lf %lf %lf %lf %d %d %d", &X_MIN_BOUND, &X_MAX_BOUND, &Y_MIN_BOUND,
			&Y_MAX_BOUND, &NUM_SAMPLES_PER_THREAD, &NUM_ITERATIONS_PER_THREAD, &NUM_WORKER_THREADS);

	// Perform validation on input values
	if(NUM_SAMPLES_PER_THREAD < 1)
	{
		printf("Number of samples per thread must be greater than zero\nExiting...\n");
		error_flag = 1;
	}
	if(NUM_WORKER_THREADS < 1)
	{
		printf("Number of worker threads must be greater than zero\nExiting...\n");
		error_flag = 1;
	}
	if(NUM_ITERATIONS_PER_THREAD < 1)
	{
		printf("Number of iterations per thread must be greater than zero\nExiting...\n");
		error_flag = 1;
	}
	if(X_MIN_BOUND > X_MAX_BOUND)
	{
		printf("X maximum bound must be greater than x minimum bound\nExiting...\n");
		error_flag = 1;
	}
	if(Y_MIN_BOUND > Y_MAX_BOUND)
	{
		printf("Y maximum bound must be greater than y minimum bound\nExiting...\n");
		error_flag = 1;
	}
	if(X_MIN_BOUND > -3)
	{
		printf("X minimum bound must be less than or equal to -3.0\nExiting...\n");
		error_flag = 1;
	}
	if(Y_MIN_BOUND > -2)
	{
		printf("Y minimum bound must be less than or equal to -2.0\nExiting...\n");
		error_flag = 1;
	}
	if(X_MAX_BOUND < 3)
	{
		printf("X maximum bound must be greater than or equal to 3.0\nExiting...\n");
		error_flag = 1;
	}
	if(Y_MAX_BOUND < 2)
	{
		printf("Y maximum bound must be greater than or equal to 2.0\nExiting...\n");
		error_flag = 1;
	}
	// Invalid input was gathered, swiftly exit
	if(error_flag == 1)
	{
		printf("Exiting...\n");
		exit(0);
	}

	pthread_t * producer_threads;
	pthread_t consumer_thread;
	head_node = NULL;

	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, NUM_WORKER_THREADS*NUM_ITERATIONS_PER_THREAD);
	sem_init(&empty, 0, NUM_WORKER_THREADS*NUM_ITERATIONS_PER_THREAD);
	// Drain the full semaphore
	for(thread_id = 0; thread_id < NUM_WORKER_THREADS * NUM_ITERATIONS_PER_THREAD; thread_id++)
		sem_wait(&full);

	printf("Created semaphores in main\n");
	// Dynamically allocate memory for worker threads
	producer_threads = (pthread_t *)malloc(sizeof(pthread_t) * NUM_WORKER_THREADS);
	
	start = clock();

	// Create all worker threads
	for(thread_id = 0; thread_id < NUM_WORKER_THREADS; thread_id++)
	{
		error_code = pthread_create(&producer_threads[thread_id], NULL, producer, (void *) thread_id);
		if(error_code != 0)
		{
			printf("Unable to create producer thread %d\n", thread_id);
			exit(0);
		}
	}

	// Create producer thread
	error_code = pthread_create(&consumer_thread, NULL, consumer, (void *)NUM_WORKER_THREADS);
	if(error_code != 0)
	{
		printf("Unable to create consumer thread\n");
		exit(0);
	}

	// Join each thread to suspend execution of main until all threads have completed their work
	for(thread_id = 0; thread_id < NUM_WORKER_THREADS; thread_id++)
	{
		error_code = pthread_join(producer_threads[thread_id], NULL);
		if(error_code != 0)
		{
			printf("Unable to join producer thread %d\n", thread_id);
			exit(0);
		}
	}

	error_code = pthread_join(consumer_thread, NULL);
	if(error_code != 0)
	{
		printf("Unable to join consumer thread\n");
		exit(0);
	}

	// Deallocate resources
	free((void *) producer_threads);
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);

	return 0;
}
