#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 8
#define LOOP_ITERATIONS 1000

int main(int argc, char* argv[]);
void* createThread();

pthread_mutex_t lock;
pthread_t threads[NUM_THREADS];
int result = 0;

void* createThread(void* id)
{
    int tmp;

    for(int i = 0; i < LOOP_ITERATIONS; i++)
    {
        pthread_mutex_lock(&lock);

        tmp = result;
        tmp = tmp + 1;
        result = tmp;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, createThread, NULL);

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    return 0;
}