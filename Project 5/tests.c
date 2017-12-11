#include "e.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc != 3) /*we need the program and two numbers*/
	{
		printf("Improper format. Run as follows './e_digits NUM NUM'\n");
		return 0;
	}

	int buffer_len = strtol(argv[2], NULL, 10)+1;	/*get the size of the buffer*/
	int sub_start = strtol(argv[1], NULL, 10);	/*get the start of the substring*/

	char *buffer = malloc((buffer_len+1)*sizeof(char));	/*it will be a string so we need an extra space for the null*/

	e(buffer, buffer_len);	/*get the first x numbers of e*/
	buffer[buffer_len] = 0;	/*ensure the string is null terminated*/

	printf("%s\n", buffer);	/*print out the initial digits*/

	char *substring = malloc((buffer_len-sub_start+2)*sizeof(char));
	substring[sizeof(substring)] = 0;

	strncpy(substring, buffer+sub_start, buffer_len-sub_start);

	printf("%s\n", substring);	/*print out the substring of digits*/


	free(buffer);	/*free the allocated space*/
	return 0;
}
