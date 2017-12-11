#include <unistd.h>
#include <stdio.h>
#include "mymalloc.h"

void* test_data(int);
void traverse_data(void*);
void free_or_used(void*);
void test_free_and_more_allocation();

int main()
{
    printf("Driver File Tests\n");
        /*Allocate some data, ensure that values are stored correctly*/
        test_data(21);  /*test the first allocation*/
        test_data(1);   /*test making a second allocation*/
        test_data(500);
        printf("\n");
        test_free_and_more_allocation();
}

void* test_data(int size)
{
    printf("\tTesting Node Values\n");
        printf("\t\tAllocating %d integers\n", size);
        printf("\t\tStarting brk value:\t\t%d\n", sbrk(0)); /*initial brk*/
        int *a = my_firstfit_malloc(size*sizeof(int));      /*allocate 21 integers*/
        int* node_info = (a-4);                             /*get the first address in the node*/
        printf("\t\tFinal brk value:\t\t%d\n", sbrk(0));    /*brk after allocation*/
        printf("\t\tReturned address:\t\t%d\n", a);         /*address given to the user*/
        printf("\t\tSize: %d, Free: %d, Next: %d, Prev: %d\n", *(node_info), *(node_info+1), *(node_info+2), *(node_info+3));   /*print the values stored in the node*/

        return(a);
}

void traverse_data(void* a)
{
    struct node* curr;
    curr = (a-16);  /*access the node information in the void pointer*/

    printf("\tNode sizes: ");
    while(curr != NULL)
    {
        printf("%d ", curr->size);
        curr = curr->next;
    }
    printf("\n\n");

    curr = (a-16);
    printf("\tNext and Previous\n");
    while(curr != NULL)
    {
        printf("\t\t%d has a previous node of %d and a next node of %d\n", curr, curr->prev, curr->next);
        curr = curr->next;
    }
    printf("\n");
}

void test_free_and_more_allocation()
{
    int *b = my_firstfit_malloc(12*sizeof(int));
    int *c = my_firstfit_malloc(11*sizeof(int));
    int *d = my_firstfit_malloc(10*sizeof(int));
    int *e = my_firstfit_malloc(9*sizeof(int));
    int *f = my_firstfit_malloc(8*sizeof(int));
    int *g = my_firstfit_malloc(7*sizeof(int));
    int *h = my_firstfit_malloc(6*sizeof(int));
    int *i = my_firstfit_malloc(5*sizeof(int));
    int *j = my_firstfit_malloc(4*sizeof(int));
    int *k = my_firstfit_malloc(3*sizeof(int));
    int *l = my_firstfit_malloc(2*sizeof(int));
    int *m = my_firstfit_malloc(1*sizeof(int));
    int *n = my_firstfit_malloc(0*sizeof(int));

    traverse_data(b);

    printf("\tTesting Free\n");
        printf("\t\tEnsuring all values are in use\n");
            free_or_used(b);
        printf("\t\tTesting free on the first allocation\n");
            my_free(b);
            free_or_used(b);
        printf("\t\tTesting free on the second allocation for coalescing\n");
            my_free(c);
            free_or_used(b);
        printf("\t\tTesting free on the last allocation\n");
            my_free(n);
            free_or_used(b);
        printf("\t\tTesting free on new last allocation\n");
            my_free(m);
            free_or_used(b);
        printf("\t\tTesting free on inner allocation\n");
            my_free(g);
            free_or_used(b);
        printf("\t\tTesting free on previous for coalescing\n");
            my_free(f);
            free_or_used(b);
        printf("\t\tTesting free on next for coalescing\n");
            my_free(h);
            free_or_used(b);
        printf("\t\tTesting free on previous and next for coalescing\n");
            my_free(e);
            my_free(i);
            free_or_used(b);

        printf("\n\tTesting Allocations with free spaces\n");
        printf("\t\tToo large to fit in first free space, but small enough to fit in second\n");
            int* w = my_firstfit_malloc(117);
            free_or_used(b);
        printf("\t\tSmall enough to fit in first free space\n");
            int* x = my_firstfit_malloc(1);
            free_or_used(b);
        printf("\t\tToo big to fit anywhere\n");
            int* y = my_firstfit_malloc(500);
            free_or_used(b);
        printf("\t\tPerfect fit\n");
            int* z = my_firstfit_malloc(75);
            free_or_used(b);

        my_free(d);
        my_free(j);
        my_free(k);
        my_free(l);
        my_free(w);
        my_free(x);
        my_free(y);
        my_free(z);
}

void free_or_used(void* a)
{
    struct node* curr;
    curr = (a-16);  /*access the node information in the void pointer*/

    printf("\t\t\tSize:Status (F/N)\t");
    while(curr != NULL)
    {
        if(curr->free == 1)
        {
            printf("%d:F\t", curr->size);
        }
        else
        {
            printf("%d:N\t", curr->size);
        }
        curr = curr->next;
    }
    printf("\n");
}
