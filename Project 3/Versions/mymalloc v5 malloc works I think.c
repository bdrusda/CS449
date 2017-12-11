/*
    Brendan Drusda
    CS 449
    Project 3 - a custom malloc()
*/

#include <unistd.h>
#include <stdio.h>  //probably won't need this
#include "mymalloc.h"



void *my_firstfit_malloc(int);
void *allocate(int);

//global variables to keep track of beginning and end of linked list
struct node* first;
struct node* last;

//program breaks when we're in 32-bit mode

void *my_firstfit_malloc(int size)
{
    /*Check linked list*/
    struct node* allocation;
    if(first == 0)                  /*See if this is the first allocation*/ //temp probably change to NULl
    {
        allocation = allocate(size);
        first = allocation;
        last = allocation;
//printf("Initial Allocation\t\tSize:%d\tFree:%d\tPrev:%d\tNext:%d\n", allocation->size, allocation->free, allocation->prev, allocation->next);
    }
    else                            /*if it isn't the first allocation*/
    {
        struct node* temp = first;   /*create node pointer to traverse linked list*/
        while(1)
        {
            /*Traverse the linked list looking for free spaces*/
            if(temp->free == 1)          /*check if the node is free*/
            {
                /*See if any spaces are available using first fit*/
                if(temp->size == (size+sizeof(struct node)))      /*if the request (and it's accompanying node) aligns perfectly with this block*/
                {
                    temp->free = 0;             /*mark space as taken*/
                    allocation = temp;    /*return the address of the linked list*/
                    /*first and last don't need to be changed because this node already exists*/
                    break;
                }
                else if(temp->size > (size+sizeof(struct node)))  /*if the request (and it's accompanying node) is smaller than this block*/
                {
                    int leftover_size = (temp->size - (size + sizeof(struct node))); /*the size of the leftovers is: original block size - (requested + node)*/

                    allocation = temp; /*create a new node to hold the allocation*/
                    allocation->free = 0;           /*mark the node as in use*/
                    allocation->size = size;        /*the size of this node becomes the requested size*/

                    if(temp->next == NULL)      /*if we allocated the last block in the linked list*/
                    {
                        allocation = last;
                        temp->free = 1;
                        break;
                    }

                    temp = allocation + (size + sizeof(struct node));   /*move temp to be the 'leftovers' node at the end of the new allocation*/
                    temp->free = 1;             /*mark new node as free*/
                    temp->size = leftover_size; /*save the new size of the block*/

                    temp->next = allocation->next;  /*enusre that everything is pointing the right way*/
                    temp->prev = allocation;
                    allocation->next = temp;
                    (temp->next)->prev = temp;

                    break;
                }
                else                        /*if the request is too big for this block*/
                {
                    temp = temp->next;          /*go to the next node and continue traversal*/
                    break;
                }
            }
            else                        /*if it isn't free*/
            {
                if(temp->next == NULL)  /*check if it is the last node in the linked list*/  //temp this is right I think but it will work with a 0?
                {
                    allocation = allocate(size);  /*allocate space*/
                    allocation->prev = temp;      /*set the previous value to be the previously last node*/
                    last->next = allocation;            /*add node to the end of the linked list*/
                    last = allocation;                  /*set new node to be last*/
                    break;
                }
                else                    /*if there is another piece of memory, check it*/
                {
                    temp = temp->next;  /*go to the next node and continue traversal*/
                }
            }
        }
    }

    return ((void *)allocation+sizeof(struct node)); /*return the address with the node abstracted out of the user's view*/
}

void *allocate(int size)
{
    void *allocation = sbrk(size+sizeof(struct node));

    struct node* curr_node;     /*create the node containing this malloc's info*/
    curr_node = allocation;          /*set it's location to be the bottom of allocate memory*/
    curr_node->size = size;      /*set the size of the node*/
    curr_node->free = 0;         /*mark it as in use*/

    return (allocation);      /*return the address to the parent function*/
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

    //if this is the first or last node, make sure to update those variables
}
