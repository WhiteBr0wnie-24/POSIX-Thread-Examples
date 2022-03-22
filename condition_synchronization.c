#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#define LOOP_ITERATIONS 10000
#define MULTIPLIER 100

int main(int argc, char* argv[]);
void* createThreadA();
void* createThreadB();

int result = 0;

#ifdef __APPLE__
    dispatch_semaphore_t    semaphore;
#else
    sem_t                   semaphore;
#endif

void* createThreadB(void* id)
{
    // Thread B waits for the semaphore which will only allow thread B
    // access once thread A finishes its work

    #ifdef __APPLE__
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    #else
        sem_wait(semaphore);
    #endif

    result = result * MULTIPLIER;
    
    return NULL;
}

void* createThreadA(void* id)
{
    // Simulate a long-running task...
    sleep(1);

    for(int i = 0; i < LOOP_ITERATIONS; i++)
        result = result + 1;

    // Once done, thread A increments the semaphore so that B can run

    #ifdef __APPLE__
        dispatch_semaphore_signal(semaphore);
    #else
        sem_post(semaphore);
    #endif

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t threadA;
    pthread_t threadB;

    // Create a semaphore with a starting value of 0.
    // One thread A finishes its task, it increments the
    // semaphore so that thread B can run.

#ifdef __APPLE__
    semaphore = dispatch_semaphore_create(0);
#else
    sem_init(&semaphore, 0, 0);
#endif

    // Create the two threads
    pthread_create(&threadA, NULL, createThreadA, NULL);
    pthread_create(&threadB, NULL, createThreadB, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    
#ifndef __APPLE__
    sem_close(&semaphore);
#endif

    // Should be 1.000.000
    printf("The result is: %d\n", result);

    return 0;
}