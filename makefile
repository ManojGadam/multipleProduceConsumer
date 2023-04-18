CC = gcc 


mul: multipleProducer.o sum.o initialiseArrays.o initShared.o binarySemaphore.o countingSemaphore.o
		$(CC) multipleProducer.o sum.o initialiseArrays.o initShared.o binarySemaphore.o countingSemaphore.o -o mul

multipleProducer.o: multipleProducer.c header.h sum.h
		$(CC) -c multipleProducer.c

sum.o: sum.c sum.h
		$(CC) -c sum.c	

initialiseArrays.o: initialiseArrays.c initialiseArrays.h
		$(CC) -c initialiseArrays.c
		
initShared.o: initShared.c header.h
		$(CC) -c initShared.c

binarySemaphore.o: binarySemaphore.c #binarySemaphore.h
		$(CC) -c binarySemaphore.c	

countingSemaphore.o: countingSemaphore.c #countingSemaphore.h
		$(CC) -c countingSemaphore.c									