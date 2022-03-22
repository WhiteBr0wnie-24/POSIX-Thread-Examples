#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#define NUM_THREADS 8
#define LOOP_ITERATIONS 1000

int main(int argc, char* argv[]);
void* createThread();

#ifdef __APPLE__
    dispatch_semaphore_t    semaphore;
#else
    sem_t                   semaphore;
#endif

pthread_t threads[NUM_THREADS];
int result = 0;

void* createThread(void* id)
{
    int tmp;

    for(int i = 0; i < LOOP_ITERATIONS; i++)
    {
        #ifdef __APPLE__
            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        #else
            sem_wait(semaphore);
        #endif

        tmp = result;
        tmp = tmp + 1;
        result = tmp;

        #ifdef __APPLE__
            dispatch_semaphore_signal(semaphore);
        #else
            sem_post(semaphore);
        #endif
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    // Init the semaphore. The second parameter means that this semaphore is
    // shared between the threads. The third parameter set the semaphore's initial
    // value. Here, it's 1. That means, that a single thread may enter the critical
    // region at any time. The others must wait until that thread exits the region.

#ifdef __APPLE__
    semaphore = dispatch_semaphore_create(1);
#else
    sem_init(&semaphore, 0, 1);
#endif

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, createThread, NULL);
    }

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("The result is: %d\n", result);
#ifndef __APPLE__
    sem_close(&semaphore);
#endif

    return 0;
}