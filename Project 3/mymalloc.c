/*
    Brendan Drusda
    CS 449
    Project 3 - a custom malloc()
*/

/*Includes*/
#include <unistd.h>
#include "mymalloc.h"

/*Prototypes*/
void *my_firstfit_malloc(int);
void *allocate(int);
void myfree(void*);
struct node* coalesce(struct node*, struct node*);

/*Globals*/
struct node* first;
struct node* last;

void *my_firstfit_malloc(int size)
{
    /*Check linked list*/
    struct node* allocation;
    if(first == NULL)   /*See if this is the first allocation*/
    {
        allocation = allocate(size);
        first = allocation;
        last = allocation;
    }
    else                /*if it isn't the first allocation*/
    {
        struct node* temp = first;                                  /*create node pointer to traverse linked list*/
        while(temp != NULL)
        {
            /*Traverse the linked list looking for free spaces*/
            if(temp->free == 1)         /*check if the node is free*/
            {
                /*See if any spaces are available using first fit*/
                if(temp->size == (size+sizeof(struct node)))    /*if the request (and it's accompanying node) aligns perfectly with this block*/
                {
                    temp->free = 0;                                 /*mark space as taken*/
                    allocation = temp;                              /*return the address of the linked list*/
                    /*first and last don't need to be changed because this node already exists*/
                    break;
                }
                else if(temp->size > (size+sizeof(struct node)))/*if the request (and it's accompanying node) is smaller than this block*/
                {
                    int leftover_size = (temp->size - (size + sizeof(struct node))); /*the size of the leftovers is: original block size - (requested + node)*/

                    allocation = temp;                              /*create a new node to hold the allocation*/
                    allocation->free = 0;                           /*mark the node as in use*/
                    allocation->size = size;                        /*the size of this node becomes the requested size*/

                    if(temp->next == NULL)  /*if we allocated the last block in the linked list*/
                    {
                        allocation = last;
                        temp->free = 1;
                        break;
                    }

                    temp = ((void*)allocation) + (size + sizeof(struct node));   /*move temp to be the 'leftovers' node at the end of the new allocation*/
                    temp->free = 1;                                 /*mark new node as free*/
                    temp->size = leftover_size;                     /*save the new size of the block*/

                    temp->next = allocation->next;                  /*enusre that everything is pointing the right way*/
                    temp->prev = allocation;
                    allocation->next = temp;
                    (temp->next)->prev = temp;
                    allocation->prev->next = allocation;
                    break;
                }
                else                                            /*if the request is too big for this block*/
                {
                    temp = temp->next;                              /*go to the next node and continue traversal*/
                }
            }
            else                        /*if it isn't free*/
            {
                if(temp->next == NULL)  /*check if it is the last node in the linked list*/
                {
                    allocation = allocate(size);                    /*allocate space*/
                    allocation->prev = temp;                        /*set the previous value to be the previously last node*/
                    last->next = allocation;                        /*add node to the end of the linked list*/
                    last = allocation;                              /*set new node to be last*/
                    break;
                }
                else                    /*if there is another piece of memory, check it*/
                {
                    temp = temp->next;                              /*go to the next node and continue traversal*/
                }
            }
        }
    }

    return ((void *)allocation+sizeof(struct node));                /*return the address with the node abstracted out of the user's view*/
}

void *allocate(int size)
{
    void *allocation = sbrk(size+sizeof(struct node));

    struct node* curr_node;                                         /*create the node containing this malloc's info*/
    curr_node = allocation;                                         /*set it's location to be the bottom of allocate memory*/
    curr_node->size = size;                                         /*set the size of the node*/
    curr_node->free = 0;                                            /*mark it as in use*/

    return (allocation);                                            /*return the address to the parent function*/
}

void my_free(void *ptr)
{
    /*navigate under the address and change the free flag*/
    struct node* info = (ptr-16);                                   /*access the contents*/
    struct node* prev = info->prev;
    struct node* next = info->next;
    info->free = 1;                                                 /*mark the node as free*/

    /*Coalesce previous*/
    if(prev != NULL)    /*ensure this isn't the first node*/
    {
        if(prev->free == 1)                                         /*check if previous is free*/
        {
            info = coalesce(prev, info);
            prev = info->prev;                                      /*ensure prev and next are in order*/
            next = info->next;
        }
    }

    /*Coalesce next*/
    if(next != NULL)    /*ensure this isn't the last node*/
    {
        if(next->free == 1) /*check if next is free*/
        {
            info = coalesce(info, next);
            prev = info->prev;                                      /*ensure prev and next are in order*/
            next = info->next;
        }
    }
    else                /*if we are freeing the last node*/
    {
        sbrk(-(sizeof(struct node) + info->size));                  /*return memory to the heap*/
        if(first == last)   /*if this is the only node*/
        {
            first = NULL;
            last = NULL;
        }
        else                /*otherwise*/
        {
            last = prev;                                            /*update the new last node*/
            last->next = NULL;
        }
    }
}

struct node* coalesce(struct node* curr, struct node* next)
{
    curr->size = (curr->size + sizeof(struct node) + next->size);   /*update size*/

    curr->next = next->next;                                        /*remove next reference to next*/

    if(next->next == NULL)  /*if next was somehow free and the last node*/
    {
        last = next;
    }
    else                    /*if there was a node after next*/
    {
        next->next->prev = curr;                                    /*remove prev reference to next*/
    }

    return curr;
}
