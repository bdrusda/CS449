#include <stdio.h>
#include <stdlib.h>

struct Node
{
	int grade;
	struct Node *next;	/*pointer to the next node*/
};

int main()
{
	int in;
	int total;
	int num_nodes;
	struct Node *head = malloc(sizeof(struct Node));	/*create the first node*/
	struct Node *curr = malloc(sizeof(struct Node));
	head->next = curr;
	printf("Enter a series of integer grades.  Enter -1 to stop");


	while(1)
	{
		scanf("%d", &in);	/*read in the grade*/
		if(in == -1)
		{
			break;
		}
		curr->next = malloc(sizeof(struct Node));	/*allocate a new node*/
		curr->grade = in;							/*assign grade*/
		curr = curr->next;
		num_nodes++;								/*increment the number of nodes*/
	}

	curr = head->next;								/*jump back to the start of the list*/

	total = 0;
	while(curr->next != NULL)						/*while we are not at the last node*/
	{
		total+=curr->grade;
		curr = curr->next;							/*and go to the next node*/
	}

	printf("The average value is: %.2f", (double)total/num_nodes);	/*print the average value*/

/*	while(head->next != NULL)	/*free all of the allocated memory*//*
	{
		free(head->next);
	}*/

	return 0;
}
