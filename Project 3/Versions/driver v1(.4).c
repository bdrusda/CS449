#include <unistd.h>
#include <stdio.h>

void test_data(int);

int main()
{
    printf("Driver File Tests\n");
        /*Allocate some data, ensure that values are stored correctly*/
        test_data(21);  /*test the first allocation*/
        test_data(1);   /*test making a second allocation*/
        //test_data(500);
}

void test_data(int size)
{
    printf("\tTesting Node Values\n");
        printf("\t\tAllocating %d integers\n", size);
        printf("\t\tStarting brk value:\t\t%d\n", sbrk(0)); /*initial brk*/
        int *a = my_firstfit_malloc(size*sizeof(int));      /*allocate 21 integers*/
        int* node_info = (a-4);                             /*get the first address in the node*/
        printf("\t\tFinal brk value:\t\t%d\n", sbrk(0));    /*brk after allocation*/
        printf("\t\tReturned address:\t\t%d\n", a);         /*address given to the user*/
        printf("\t\tSize: %d, Free: %d, Next: %d, Prev: %d\n", *(node_info), *(node_info+1), *(node_info+2), *(node_info+3));   /*print the values stored in the node*/
}
