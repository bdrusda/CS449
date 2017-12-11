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
    printf("Allocating 50 integers:\t\t%d\t",sbrk(0));
    my_firstfit_malloc(50*sizeof(int));
    printf("%d\n",sbrk(0));

    printf("Allocating 1 integer:\t\t%d\t",sbrk(0));
    my_firstfit_malloc(sizeof(int));
    printf("%d\n",sbrk(0));

    printf("Allocating 23 integers:\t\t%d\t",sbrk(0));
    my_firstfit_malloc(23*sizeof(int));
    printf("%d\n",sbrk(0));

    printf("Allocating 1000 integers:\t%d\t",sbrk(0));
    my_firstfit_malloc(1000*sizeof(int));
    printf("%d\n",sbrk(0));
}

void *my_firstfit_malloc(int size)
{
    //Temp - should there be error checking?

    return sbrk(size);
}

void my_free(void *ptr)
{
    /*okay so it takes the first address, but how do we know how much to free?*/
}
