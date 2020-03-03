/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
static task queue[QUEUE_SIZE];
pthread_mutex_t mutex; //mutex lock
static int count = 0;
static int full = 0;
static int empty = 0;
sem_t sem; //semaphone

// the worker bee
pthread_t bee[NUMBER_OF_THREADS]; // thread pool

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task tmp)
{
    pthread_mutex_lock(&mutex);
    if (count == QUEUE_SIZE)
    {
        pthread_mutex_unlock(&mutex);
        return 1; // can't enter the queue
    }
    else
    {
        queue[full++] = tmp;
        full %= QUEUE_SIZE; //if full == QUEUE_SIZE, then turn it to 0
        count ++;
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    pthread_mutex_lock(&mutex);
    task worktodo = queue[empty++];
    empty %= QUEUE_SIZE; //if empty == QUEUE_SIZE, then turn it to 0
    count --;
    pthread_mutex_unlock(&mutex);
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    task worktodo;
    while (1)
    {
        sem_wait(&sem);
        worktodo = dequeue();
        execute(worktodo.function, worktodo.data);
    }
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    int flag;
    task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;
    flag = enqueue(worktodo); // judge whether it is successful to enqueue
    if (flag)
        return flag; //error exists
    sem_post(&sem);
    return flag; //good

}

// initialize the thread pool
void pool_init(void)
{
    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    for (int i=0;i< NUMBER_OF_THREADS; i++)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for(int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_cancel(bee[i]);
        pthread_join(bee[i],NULL);
    }
}
