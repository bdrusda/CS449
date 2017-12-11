/*  Brendan Drusda
    Project 2 - mystrings */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
/*Variable delcarations*/
    int upper = 126;
    int lower = 32;
    int file_size;
    int i = 0;
    int j = 0;
/*Input checking*/
    if(argc != 2)
    {
        printf("Incorrect number of command line arguments\n");
        return 0;
    }

/*Creating file*/
    FILE *fPtr;
    fPtr = fopen(argv[1], "r");
    if(fPtr == NULL)
    {
        printf("File not found\n");
        return 0;
    }

/*Getting input from file*/
    fseek(fPtr, 0L, SEEK_END);   /*get the size of the file to fill the buffer*/
    file_size = ftell(fPtr);
    fseek(fPtr, 0L, SEEK_SET);  /*go back to the beginning of the file*/

    char *buffer = malloc(file_size * sizeof(char));   /*allocate space for the file's contents*/
    fread(buffer, sizeof(char), file_size, fPtr);      /*fill the allocated space*/

/*Reading in strings*/
    while(i < file_size)                               /*for the duration of the file*/
    {
        char minSize[5] = {0,0,0,0,0};                 /*ensure the array doesn't carry over any information from the last iteration*/
/*Check the first four characters*/
        for(j = 0; j < 4; j++)                      /*store the first four characters until we know this is a valid length string*/
        {
            if((buffer[i] < upper) && (buffer[i] > lower))
            {
                minSize[j] = buffer[i];
                i++;                                   /*move on to the next character*/
            }
            else                                    /*if there are less than four printable characters consecutively*/
            {
                i++;                                   /*move to the next character*/
                j = 0;                                 /*reset this loop and try the next word*/
                minSize[4] = 1;                        /*value indicating that this is not a string*/
                break;                                 /*go to the next iteration of the for loop(which will start at 0)*/
            }
        }
/*Check the result of the first four characters*/
        if(minSize[4] != 1)                         /*if the first four characters were all printable*/
        {
            printf("%s", minSize);                     /*print the verified first portion of the string*/

        /*Print the remaining characters in the string*/
            while((buffer[i] < upper) && (buffer[i] > lower))    /*print each following character until a nonprintable is reached*/
            {
                printf("%c", buffer[i]);
                i++;
            }
            printf("\n");                           /*print a new line for the next word*/
        }
    }

    free(buffer);                                   /*free the allocated memory*/
}
