#include <unistd.h>
#include <stdio.h>
#include "mymalloc.h"

void *test_data(int);
void traverse_data(void*);

int main()
{
    printf("Driver File Tests\n");
        /*Allocate some data, ensure that values are stored correctly*/
        struct node* first = test_data(21);  /*test the first allocation*/
        test_data(1);   /*test making a second allocation*/
        test_data(500); /*dare we attempt a third? NOTE: three hours of debugging has elapses between the second allocation and beginning to attempt this one.*/
        traverse_data(first);
}

void *test_data(int size)
{
    printf("\tTesting Node Values\n");
        printf("\t\tAllocating %d integers\n", size);
        printf("\t\tStarting brk value:\t\t%d\n", sbrk(0)); /*initial brk*/
        int *a = my_firstfit_malloc(size*sizeof(int));      /*allocate 21 integers*/
        int* node_info = (a-4);                             /*get the first address in the node*/
        printf("\t\tFinal brk value:\t\t%d\n", sbrk(0));    /*brk after allocation*/
        printf("\t\tReturned address:\t\t%d\n", a);         /*address given to the user*/
        printf("\t\tSize: %d, Free: %d, Next: %d, Prev: %d\n", *(node_info), *(node_info+1), *(node_info+2), *(node_info+3));   /*print the values stored in the node*/

        return a;
}

void traverse_data(void* a)
{
    struct node* curr;
    curr = (a-16);  /*access the node information in the void pointer*/

    printf("Node sizes: ");
    while(curr != NULL)
    {
        printf("%d ", curr->size);
        curr = curr->next;
    }
    printf("\n");

    curr = (a-16);
    printf("Next and Previous\n");
    while(curr != NULL)
    {
        printf("%d has a previous node of %d and a next node of %d\n", curr, curr->prev, curr->next);
        curr = curr->next;
    }
    printf("\n");
}
