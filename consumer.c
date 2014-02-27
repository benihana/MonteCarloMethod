#include "prodcons.h"
#include "linked_list.h"
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

void * consumer(void * tid)
{
	int iteration = 0; 
	double x;
	double y;
	double fn_val;

	for(iteration = 0; iteration < NUM_ITERATIONS_PER_THREAD * NUM_WORKER_THREADS; iteration++)
	{
		node * item;
		// Wait on the full counter
		sem_wait(&full);
		// Wait on the mutex semaphore
		sem_wait(&mutex);
		// Remove an item from the buffer
		item = get_last(&head_node);

		if(iteration == 0)
		{
			x = item->x_loc;
			y = item->y_loc;
			fn_val = item->fn_val;
		}
		else
		{
			if(fn_val > item->fn_val)
			{
				x = item->x_loc;
				y = item->y_loc;
				fn_val = item->fn_val;
			}
		}

		remove_node(&head_node);
		// Release the mutex semaphore
		sem_post(&mutex);
		// Post to the empty counter
		sem_post(&empty);
	}

	// Calculate total run time and display 
	printf("Run time is %lf seconds.\n", (clock() - start) / (double)CLOCKS_PER_SEC);
	printf("x: %lf, y: %lf, f(x,y) = %lf\n", x, y, fn_val);
	pthread_exit(NULL);
}
