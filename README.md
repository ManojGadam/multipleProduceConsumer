# multipleProduceConsumer

This lab performs multiple producer multiple consumer problem by implementing semaphores and printing the number of produced and consumed items.

## How to build
To build this shell run "make" in linux shell or bash


## How to launch and quit

To launch this shell use "./mul" after building the shell

## Examples

Here are some example commands and their outputs from the program

1. 1 1 1 1 0
Buffer size should be greater than 1

2. 1 1 0 1 2 
Sleep time should be greater than 0

3. 1 1 1 0 2
Producers produced 0 items
Consumers consumed 0 items

4. 1 1 1 1 2
Producers produced 1 items
Consumers consumed 1 items

5. 2 2 2 2 4
Producers produced 4 items
Consumers consumed 4 items

6. 3 1 2 1 3
Producers produced 3 items
Consumers consumed 3 items


7. 2 3 2 1 3
Producers produced 2 items
Consumers consumed 2 items

8. 5 1 2 1 5
Producers produced 5 items
Consumers consumed 5 items


9.5 1 2 1 3
Producers produced 5 items
Consumers consumed 5 items

10 5 2 2 2 15
Producers produced 10 items
Consumers consumed 10 items
