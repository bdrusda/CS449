/*
    Brendan Drusda
    CS 449
    Project 3 - a custom malloc()
*/

#include <unistd.h>
#include <stdio.h>
#include "mymalloc.h"





//global variables to keep track of beginning and end of linked list






int main()  //Temp -   this probably won't be here when the program is actually made, as malloc won't have a main function, just the tester file
{
    printf("Starting brk value:\t\t%d\n", sbrk(0));

    printf("Allocating 21 integers\n");
    int *a = my_firstfit_malloc(21*sizeof(int));
    printf("Returned address:\t\t%d\n", a);

/*    for(i = 0; i < 300; i++)
    {
        a[i] = i;
        printf("a[%d] = %d\n", i, a[i]);
    }*/

    printf("\n%d\n", a[-1]);



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

    //temp checking the values stored through memory
    int* test = (a-6);
    printf("Value of A:%d\tValue of A-24:%d\tThis should be some value: %d\t%d\n", a, a-6, test);
}

void *my_firstfit_malloc(int size)
{
    //Check linked list
        //See if any spaces are available using first first
            //If they are, split the node into two
                //One node containing the allocated data
                //One node containing the free spaces       Maybe just implement this using internal fragmentation at least at first
            //If they aren't allocate a new piece of memory using sbrk()

    //Temp - should there be error checking?

    struct node curr_node;  /*create the node containing this malloc's info*/
    curr_node.size = size;      /*set the size of the node*/
    curr_node.free = 0;         /*mark it as in use*/
//This node just isnt in the address up there
    struct node next;
    struct node prev;
    curr_node.next = &next;
    curr_node.prev = &prev;
    //temp printf("%d\t%d\t%d\t%d\n", curr_node.size, curr_node.free, curr_node.next, curr_node.prev);
    //temp printf("Size:\t%d\tsizeof(struct node):\t%d\n", size, sizeof(struct node));
    return (sbrk(size+sizeof(curr_node))+sizeof(curr_node));      /*allocate the size requested + the node, abstracting the node from the user when returning*/
}

void my_free(void *ptr)
{
    /*navigate under the address and change the free flag*/
    /*while there, check the size and free that many bytes*/
    /*okay so it takes the first address, but how do we know how much to free?*/

    //Check previous node to see if it's free
    //Check next node to see if it's free
        //If either of these are, coalesce the data into one block
        //If next is null (being that this node is touching brk) use sbrk(negative value) to return memory to the heap
}
