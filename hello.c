#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 16
#define MAX_LENGTH 40000
#define CHUNK_SIZE (MAX_LENGTH / NUM_THREADS) * 2

void* createThread(void *args);
void longRunningTask(unsigned start, unsigned end, unsigned chunk);
int main(int argc, char* argv[]);

struct arg_struct {
    int threadID;
    int start;
    int end;
};

void* createThread(void *args)
{
    struct arg_struct *vals = args;
    longRunningTask(vals->start, vals->end, CHUNK_SIZE);
    pthread_exit(NULL);
}

void longRunningTask(unsigned start, unsigned end, unsigned chunk)
{
    unsigned* matrix = malloc(chunk * chunk * sizeof(unsigned));

    if(matrix == NULL)
        return;

    int index = 0;

    for(int i = start; i < end; i++)
    {
        for(int u = start; u < end; u++)
            matrix[index++] = i * u;
    }

    free(matrix);
}

int main(int argc, char* argv[])
{
    printf("Starting sequential task...");

    clock_t begin_sequential = clock();

    longRunningTask(0, MAX_LENGTH, MAX_LENGTH * MAX_LENGTH);

    clock_t end_sequential = clock();
    double seq_exec_time = (double)(end_sequential - begin_sequential) / CLOCKS_PER_SEC;
    printf("Done! Result: %lf seconds\n", seq_exec_time);

    printf("Starting parallel task (%d threads)...", NUM_THREADS);

    clock_t begin_parallel = clock();
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        int start = i * CHUNK_SIZE;
        int end = (start + CHUNK_SIZE) - 1;

        struct arg_struct thread_args;
        thread_args.start = start;
        thread_args.end = end;
        thread_args.threadID = i;

        int rc = pthread_create(&threads[i], NULL, createThread, (void*)&thread_args);

        if (rc)
        {
            printf("\nCould not create thread with id: %d!\n", i);
            return -1;
        }
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end_parallel = clock();
    double par_exec_time = (double)(end_parallel - begin_parallel) / CLOCKS_PER_SEC;
    printf("Done! Result: %lf seconds\n", par_exec_time);

    return 0;
}