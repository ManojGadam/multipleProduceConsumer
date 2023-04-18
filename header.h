#include <stdio.h>
#include <pthread.h>
typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
}binary_Semaphore;

typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
}counting_Semaphore;

typedef struct{
    int *buffer;
    int head;
    int tail;
    int size;
    binary_Semaphore bin;
    counting_Semaphore empty;
    counting_Semaphore full;
    int items;
    int produceComplete;
}shared_Buffer;

typedef struct
{
    int threadId;
    long sleep;
    int itemsToProduce;
    int* numberOfItemsProduced;
    int* numberOfItemsConsumed;
    int numberOfConsumers;
    shared_Buffer *sharedBuffer;
}thread_info;

void initShared(shared_Buffer *buffer,int size);
void binary_Sempahore_init(binary_Semaphore *bin,int value);
void binary_Sempahore_Wait(binary_Semaphore *bin);
void binary_Semaphore_Cond(binary_Semaphore *bin);
void counting_Sempahore_init(counting_Semaphore *cou,int value);
void counting_Semaphore_Wait(counting_Semaphore *cou);
void counting_Semaphore_Cond(counting_Semaphore *cou);
