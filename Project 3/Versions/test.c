#include <stdio.h>
#include "mymalloc.h"
#define MAGIC_NUM 4242

void* my_malloc_brk_up(int);
void* my_firstfit_malloc(int);
void my_free(void*);
void deallocate(struct node*);
struct node* coalesce(struct node*, struct node*);
struct node* nextfit_create(int);


struct node *first_node;
struct node *last_node;
struct node *next_fit_node;
int bool_init_needed = 1;

void* my_firstfit_malloc(int size){
	void *ptr;
	struct node* node;

	if(bool_init_needed){ //used to get things running again!
		bool_init_needed = 0; //No more init needed!
		//pointer updates
		ptr = my_malloc_brk_up(size); //This is the first ever created space
		if(ptr == NULL){ //unsuccessful
			return NULL;
		}

		node = ptr-sizeof(struct node); //This is the address of the first (and last, in this case) node
		first_node = node;
		last_node = node;
		next_fit_node = first_node; //next fit's gotta start somewhere

	} else { //if init is not needed:
		node = nextfit_create(size);
		if (node != NULL){
			ptr = node;
			ptr += sizeof(struct node);//set ptr

		} else { //Couldnt find a spot, create more (with sbrk)!

			ptr = my_malloc_brk_up(size); //Push up brk
			if (ptr == NULL){ //unsuccessful
				return NULL;
			}

			node = ptr-sizeof(struct node); //Get node-space
			last_node->next = node; //the very last node is getting a next node because brk went UP
			node->prev = last_node; //this node's previous node is the last node
			last_node = node; //update last node to this one
		}

	}

	return ptr;
}

struct node* nextfit_create(int size){
	struct node* result;
	struct node* tmp; //acts as starting point in do/while AND as a tmp node during creation
	struct node* tmp2;
	void *ptr;
	int remaining_size;

	tmp = next_fit_node;
	result = next_fit_node;

	do{
		if(!result->free){
			if(result->size > (size+sizeof(struct node))){ //it must be bigger than the size and space lost to a new node creation
				remaining_size = result->size - size - sizeof(struct node); //avail size minus requested size minus a new node allocation

				result->free = 0; //TRUE
				result->size = size;

				ptr = result;   //0x804b000
				ptr += (size + sizeof(struct node));

				tmp = ptr;
				tmp->free = 1; //FREE!
				tmp->size = remaining_size;
				tmp->next = result->next;
				tmp->prev = result;
				//Update neighbors
				result->next = tmp;
				(tmp->next)->prev = tmp;

				next_fit_node = first_node; //update next fit node

				return result; //return from here!
			}
		}

		if (result->next == NULL){ //end of list
			result = first_node;
		} else { //middle of list somewhere
			result = result->next;
		}

	}while(result != tmp);



	return NULL;
}

void my_free(void* ptr){
	struct node* node;

	node = ptr-sizeof(struct node);

		deallocate(node); //deallocate it (handles coalescing and brk decrementing)

}

void deallocate(struct node* node){
	struct node* prev;
	struct node* next;
	prev = node->prev;
	next = node->next;

	node->free = 1; //False, not in use; "Free"

	if ( (!(next == NULL)) && ((next->free)) ){
		node = coalesce(node, next);
		prev = node->prev;
		next = node->next;
	}
	if ( (!(prev == NULL)) && ((prev->free)) ){
		node = coalesce(prev, node);
		prev = node->prev;
		next = node->next;
	}

	if( next == NULL ){
		sbrk( -(node->size + sizeof(struct node)) ); //set to its current location -size and -stuctsize

		if (first_node == last_node){
			bool_init_needed = 1; //need init on next pass!
			first_node = NULL;
			last_node = NULL;
			next_fit_node = NULL;

		} else {

			last_node = prev;

			last_node->next = NULL;
		}
	}

}

struct node* coalesce(struct node* node, struct node* next){
	struct node* tmp;

	node->size = node->size + next->size + sizeof(struct node); //two sizes plus the size of the node!

	node->next = next->next;

	tmp = next->next;

	if(next == last_node){
		last_node = node;
	} else { //not end of list, so tmp is not null!
		tmp->prev = node;
	}

	return node;
}

void *my_malloc_brk_up(int size){

	void *old_brk;
	old_brk = (void *)sbrk(size+sizeof(struct node)); //Size = Requested + Node

	if (old_brk < 0){ //sbrk returns -1 if unsuccessful
		return NULL; //unsuccessful!
	}

	struct node* new_node;
	new_node = old_brk; //node pointer address becomes old_brk
	new_node->free = 1; //TRUE, mem in use
	new_node->size = size; //Requested size
	new_node->next = NULL; //Next and Prev are determined elsewhere (my_nextfit_malloc())
	new_node->prev = NULL;

	return old_brk+sizeof(struct node); //Beginning of requested space
}
