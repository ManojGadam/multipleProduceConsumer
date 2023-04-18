#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "header.h"
#include "sum.h"
#include "initialiseArrays.h"

void *producerHandle(void *arg);
void *consumerHandle(void *arg);
int main()
{
    shared_Buffer buffer;
    //int head=0,tail=0;
    srand(time(NULL));
    int numOfProducers,numOfConsumers,ringBufferSize,totalItemsProducer;
    long maxSleep;
    printf("Enter input in the form 'numberOfProducers numberOfConsumers maxSleepSeconds TotalItemsPerProducer RingBufferSize':");
    fflush(stdout);
    scanf("%d %d %ld %d %d",&numOfProducers,&numOfConsumers,&maxSleep,&totalItemsProducer,&ringBufferSize);
    if(ringBufferSize < 2 )
    {
        printf("Buffer size should be greater than 1\n");
        return 0;
    }
    else if(maxSleep < 1 ) // confirm
    {
        printf("Sleep time should be greater than 0\n");
        return 0;
    }
    initShared(&buffer,ringBufferSize);
    pthread_t threads[numOfProducers];
    pthread_t consumerThreads[numOfConsumers];
    int producedItemsCount[numOfProducers],consumerItemsCount[numOfConsumers+1],k= numOfConsumers+1;
    initialiseArrays(producedItemsCount,numOfProducers);
    initialiseArrays(consumerItemsCount,numOfConsumers+1);
    for (int i = 0; i < numOfProducers; i++)
    {
        thread_info* t_info = malloc(sizeof(thread_info));
        t_info->sleep = maxSleep;
        t_info->threadId = i+1;
        t_info->sharedBuffer = &buffer;
        t_info->itemsToProduce = totalItemsProducer;
        t_info->numberOfItemsProduced = producedItemsCount;
        //t_info->items = 0;
        pthread_create(&threads[i],NULL,producerHandle,t_info);
    }


    for (int i = 0; i < numOfConsumers; i++)
    {
        thread_info* t1_info = malloc(sizeof(thread_info));
        t1_info->sleep = maxSleep;
        t1_info->threadId = i+1;
        t1_info->sharedBuffer = &buffer;
        t1_info->numberOfItemsConsumed = consumerItemsCount;
        t1_info->numberOfConsumers = numOfConsumers;
        pthread_create(&consumerThreads[i],NULL,consumerHandle,t1_info);
    }

    for (int i = 0; i < numOfProducers; i++)
    {
        int status =pthread_tryjoin_np(threads[i],NULL);
        //Using this if there are more producers than consumers and buffer is full. So eventhough producers didn't complete producing since there will be no consumers program waits indefinetely.Below code prevents that from happening
        if(status == EBUSY && buffer.items == ringBufferSize)
        {
            while(buffer.head != buffer.tail || buffer.items>0)
            {
                if(numOfConsumers > numOfProducers*totalItemsProducer || numOfConsumers == numOfProducers*totalItemsProducer)continue;
                thread_info* t1_info = malloc(sizeof(thread_info));
                t1_info->sleep = maxSleep;
                t1_info->threadId = k;
                t1_info->sharedBuffer = &buffer;
                t1_info->numberOfItemsConsumed = consumerItemsCount;
                t1_info->numberOfConsumers = numOfConsumers;
                pthread_create(&consumerThreads[k],NULL,consumerHandle,t1_info);
                pthread_join(consumerThreads[k],NULL);
            }
        }
        else
        {
            pthread_join(threads[i],NULL);
        }
        
    }
    buffer.produceComplete = 1;
   //using this if producer completes producing but there are still items in buffer
    while(buffer.produceComplete != 1 || buffer.head != buffer.tail || buffer.items>0)
    {
        if(numOfConsumers > numOfProducers*totalItemsProducer || numOfConsumers == numOfProducers*totalItemsProducer)continue;
        //printf("Items in buffer %d\n",buffer.items);
        thread_info* t1_info = malloc(sizeof(thread_info));
        t1_info->sleep = maxSleep;
        t1_info->threadId = k;
        t1_info->sharedBuffer = &buffer;
        t1_info->numberOfItemsConsumed = consumerItemsCount;
        t1_info->numberOfConsumers = numOfConsumers;
        pthread_create(&consumerThreads[k],NULL,consumerHandle,t1_info);
        pthread_join(consumerThreads[k],NULL);
    }
    for (int i = 0; i < numOfConsumers; i++)
    {
        int status1 = pthread_tryjoin_np(consumerThreads[i],NULL);
        if(status1 == EBUSY && buffer.produceComplete ==1 && buffer.head == buffer.tail)
        {
            pthread_cancel(consumerThreads[i]);
        }
        else
        {
            pthread_join(consumerThreads[i],NULL);
        }
    }
    printf("Producers produced %d items\n",sum(producedItemsCount,numOfProducers));
    printf("Consumers consumed %d items\n",sum(consumerItemsCount,numOfConsumers+1));
     free(buffer.buffer);
}








void *producerHandle(void *arg)
{
    thread_info* args = (thread_info*) arg;
    for (int i = 0; i < args->itemsToProduce; i++)
    {
     counting_Semaphore_Wait(&args->sharedBuffer->empty);
     binary_Sempahore_Wait(&args->sharedBuffer->bin);
    int producerSleep = (rand()%(args->sleep))+1;
     sleep(producerSleep);
     args->sharedBuffer->buffer[args->sharedBuffer->head] = args->threadId;
     args->sharedBuffer->items++;
     args->numberOfItemsProduced[args->threadId-1]++;
     printf("Producer %d produced and added to %d position in buffer\n",args->threadId,args->sharedBuffer->head);
     args->sharedBuffer->head = (args->sharedBuffer->head + 1) % (args->sharedBuffer->size);
     binary_Semaphore_Cond(&args->sharedBuffer->bin);
     counting_Semaphore_Cond(&args->sharedBuffer->full);
    }
    free(args);  
    return 0;
}

void *consumerHandle(void *arg)
{
    thread_info *args = (thread_info*)arg;
        counting_Semaphore_Wait(&args->sharedBuffer->full);
        binary_Sempahore_Wait(&args->sharedBuffer->bin);
        int consumed = args->sharedBuffer->buffer[args->sharedBuffer->tail];
        args->sharedBuffer->items--;
        int consumerSleep = (rand()%(args->sleep))+1;
        printf("Consumer %d consumed and removed %d from %d position in buffer\n",args->threadId,consumed,args->sharedBuffer->tail);
        args->sharedBuffer->tail = (args->sharedBuffer->tail + 1) % (args->sharedBuffer->size);
        args->numberOfItemsConsumed[args->threadId-1]++;
        sleep(consumerSleep);
        
        binary_Semaphore_Cond(&args->sharedBuffer->bin);
        
        counting_Semaphore_Cond(&args->sharedBuffer->empty);
        
    free(args);
    return 0;
}

