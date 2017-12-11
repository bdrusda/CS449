/*
    Brendan Drusda
    CS 449
    Project 3 - a custom malloc()
*/

#include <unistd.h>
#include <stdio.h>
#include "mymalloc.h"

int main()  //Temp -   this probably won't be here when the program is actually made, as malloc won't have a main function, just the tester file
{
    printf("Starting brk value:\t\t%d\n", sbrk(0));

    printf("Allocating 21 integers\n");
    int *a = my_firstfit_malloc(21*sizeof(int));
    printf("Returned address:\t\t%d\n", a);

/*  Extra allocations
    printf("Allocating 1 integer:\t\t%d\t",sbrk(0));
    int *b = my_firstfit_malloc(sizeof(int));
    printf("Returned address: %d\n",b);

    printf("Allocating 23 integers:\t\t%d\t",sbrk(0));
    int *c = my_firstfit_malloc(23*sizeof(int));
    printf("Returned address: %d\n",c);

    printf("Allocating 1000 integers:\t%d\t",sbrk(0));
    int *d = my_firstfit_malloc(1000*sizeof(int));
    printf("Returned address: %d\n",d); */

    printf("Final brk value:\t\t%d\n", sbrk(0));
}

void *my_firstfit_malloc(int size)
{
    //Temp - should there be error checking?
    struct node curr_node;  /*create the node containing this malloc's info*/
    curr_node.size = size;      /*set the size of the node*/
    curr_node.free = 0;         /*mark it as in use*/
    //temp printf("%d\t%d\t%d\t%d\n", curr_node.size, curr_node.free, curr_node.next, curr_node.prev);
    //temp printf("Size:\t%d\tsizeof(struct node):\t%d\n", size, sizeof(struct node));
    return (sbrk(size+sizeof(curr_node))+sizeof(curr_node));      /*allocate the size requested + the node, abstracting the node from the user when returning*/
}

void my_free(void *ptr)
{
    /*navigate under the address and change the free flag*/
    /*while there, check the size and free that many bytes*/
    /*okay so it takes the first address, but how do we know how much to free?*/
}
