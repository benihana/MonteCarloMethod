#include "mt64.h"
#include "linked_list.h"
#include "prodcons.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void * producer(void * tid)
{
	int iteration = 0;
	int sample = 0;
	double best_x_loc = (X_MAX_BOUND-X_MIN_BOUND) * genrand64_real1() + X_MIN_BOUND;
	double best_y_loc = (Y_MAX_BOUND-Y_MIN_BOUND) * genrand64_real1() + Y_MIN_BOUND;
	double x_sq = best_x_loc * best_x_loc;
	double y_sq = best_y_loc * best_y_loc;
	double best_fn_val = (4.0 - 2.1 * x_sq + (x_sq * x_sq) / 3.0) * x_sq + best_x_loc * best_y_loc + (-4.0 + 4.0 * y_sq) * y_sq;
	double x;
	double y;
	double fn_val;

	for(iteration = 0; iteration < NUM_ITERATIONS_PER_THREAD; iteration++)
	{
		for(sample = 0; sample < NUM_SAMPLES_PER_THREAD; sample++)
		{
			// Create a sample
			x = (X_MAX_BOUND-X_MIN_BOUND) * genrand64_real1() + X_MIN_BOUND;
			y = (Y_MAX_BOUND-Y_MIN_BOUND) * genrand64_real1() + Y_MIN_BOUND;
			x_sq = x * x;
			y_sq = y * y;
			fn_val = (4.0 - 2.1 * x_sq + (x_sq * x_sq) / 3.0) * x_sq + x * y + (-4.0 + 4.0 * y_sq) * y_sq;

			if(fn_val < best_fn_val)
			{
				best_x_loc = x;
				best_y_loc = y;
				best_fn_val = fn_val;
			}
		}

		// Wait for the empty counter
		sem_wait(&empty);
		// Wait for the mutex semaphore
		sem_wait(&mutex);
		// Put the item in the buffer (the list)
		add(&head_node, best_x_loc, best_y_loc, best_fn_val);
		// Post to the mutex semaphore
		sem_post(&mutex);
		// Post to the full counter
		sem_post(&full);
	}
	pthread_exit(NULL);
}
