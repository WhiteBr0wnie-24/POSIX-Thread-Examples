#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

int main(int argc, char* argv[]);
void* createThreadA();
void* createThreadB();

int result = 0;

#ifdef __APPLE__
    dispatch_semaphore_t    s1;
    dispatch_semaphore_t    s2;
#else
    sem_t                   s1;
    sem_t                   s2;
#endif

void* createThreadB(void* id)
{
    while(true)
    {
        #ifdef __APPLE__
            dispatch_semaphore_wait(s2, DISPATCH_TIME_FOREVER);
        #else
            sem_wait(s2);
        #endif

        printf("pong! ");

        #ifdef __APPLE__
            dispatch_semaphore_signal(s1);
        #else
            sem_post(s1);
        #endif
    }
    
    return NULL;
}

void* createThreadA(void* id)
{
    while(true)
    {
        #ifdef __APPLE__
            dispatch_semaphore_wait(s1, DISPATCH_TIME_FOREVER);
        #else
            sem_wait(s1);
        #endif

        printf("ping ");


        #ifdef __APPLE__
            dispatch_semaphore_signal(s2);
        #else
            sem_post(s2);
        #endif
    }

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
    s1 = dispatch_semaphore_create(1);
    s2 = dispatch_semaphore_create(0);
#else
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);
#endif

    // Create the two threads
    pthread_create(&threadA, NULL, createThreadA, NULL);
    pthread_create(&threadB, NULL, createThreadB, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    
#ifndef __APPLE__
    sem_close(&semaphore);
#endif

    return 0;
}