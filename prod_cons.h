// Producer-Consumer global definitions including semaphores

#include <semaphore.h>

#ifndef PRODCONS_H
#define PRODCONS_H

extern double start; // Starting time to calculate runtime

// Define semaphore variables for use in multiple files
extern sem_t mutex;
extern sem_t full;
extern sem_t empty;

extern const int NUM_WORKER_THREADS;
extern const int NUM_SAMPLES_PER_THREAD;
extern const int NUM_ITERATIONS_PER_THREAD;
extern const double X_MIN_BOUND;
extern const double X_MAX_BOUND;
extern const double Y_MIN_BOUND;
extern const double Y_MAX_BOUND;

// Define producer and consumer thread prototypes
void * producer(void *);
void * consumer(void *);

#endif
