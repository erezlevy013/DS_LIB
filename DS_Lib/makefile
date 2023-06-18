#makefile Lib Data Struct

TARGET = libDS.so
CC = gcc
CFLAGS = -ansi -g -pedantic -fPIC
OBJS = GenericVector.o GenericHeap.o GenericQueue.o GenericDoubleList.o ListItr.o ListFunc.o HashMap.o

$(TARGET): $(OBJS)
		$(CC)  -shared -o $(TARGET) $(OBJS) -lm
		
GenericVector.o: GenericVector.c GenericVector.h
		$(CC) $(CFLAGS) -c GenericVector.c
		
GenericHeap.o: GenericHeap.c GenericHeap.h GenericVector.h
		$(CC) $(CFLAGS) -c GenericHeap.c
		 		
GenericQueue.o: GenericQueue.c GenericQueue.h
		$(CC) $(CFLAGS) -c GenericQueue.c
	
GenericDoubleList.o: GenericDoubleList.c GenericDoubleList.h Internal.h
		$(CC) $(CFLAGS) -c GenericDoubleList.c 	

ListItr.o: ListItr.c ListItr.h GenericDoubleList.h Internal.h 
		$(CC) $(CFLAGS) -c ListItr.c 

ListFunc.o: ListFunc.c ListFunc.h ListItr.h
		$(CC) $(CFLAGS) -c ListFunc.c

HashMap.o: HashMap.c HashMap.h GenericDoubleList.h ListItr.h ListFunc.h Internal.h  
		$(CC) $(CFLAGS) -c HashMap.c 

clean: 
		rm -f $(TARGET) $(OBJS)




