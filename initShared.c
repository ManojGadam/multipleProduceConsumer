#include <stdio.h>
#include <stdlib.h>
#include "header.h"
void initShared(shared_Buffer *buffer,int size)
{
    buffer->buffer = malloc(sizeof(int)*size);
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = size;
    buffer->produceComplete = 0;
    buffer->items = 0;
    binary_Sempahore_init(&buffer->bin,1);
    counting_Sempahore_init(&buffer->empty,size);
    counting_Sempahore_init(&buffer->full,0);              
}
