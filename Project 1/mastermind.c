/*  Brendan Drusda
    Project 1 - Mastermind */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

char* keyGen(char key[]);

int main()
{
/*Variables*/
    /*loop variables*/
    int i;
    int j;
    /*Game variables*/
    char userIn[5];     /*user input*/
    char key[5];        /*answer key*/
    char tIn[5];        /*temp user input*/
    char tKey[5];       /*temp answer key*/
    int perfect;        /*value holding the number of letters perfectly guessed*/
    int rightLetter;    /*value holding the number of letters that are correct, but in the wrong spot*/
    srand((unsigned int)time(NULL));  /*set the seed for rand so the outcome isn't the same every time the program is run*/

/*Game Implementation*/
    /*Welcome user*/
    printf("Welcome to Mastermind!\n\n");

    /*Ask if user wants to play*/
    printf("Would you like to play? (yes/no)\n");

    while(1)    /*loop running the game*/
    {
        while(1)    /*loop getting input*/
        {
            scanf("%s", &userIn);               /*get input*/
            if(strcmp(userIn,"yes") == 0)       /*if it's a yes, we are satisfied*/
            {
                break;
            }
            else if(strcmp(userIn,"no") == 0)   /*if it's a no*/
            {
                printf("Exiting program.");        /*exit the program*/
                return 0;
            }
            else                                /*otherwise prompt for a valid input*/
            {
                printf("Invalid input, please enter \"yes\" or \"no\"\n");
            }
        }

    /*Generate 4 colors*/
        *key = *(keyGen(key));

    /*Get guess*/
        int guessNum = 1;
        while(1)    /*loop containing actual gameplay*/
        {
            perfect = 0;                        /*set/reset guess statistics*/
            rightLetter = 0;

            printf("Enter guess %d:", guessNum);       /*prompt user for guess*/
            scanf("%s", &userIn);                      /*get guess*/
            userIn[4] = '\0';                          /*set the end of the string*/
            strcpy(tKey, key);                         /*fill temporary arrays that can be manipulated while checking the guesses*/
            strcpy(tIn, userIn);

/*Get colors in correct place*/
            for(i = 0; i < 4; i++)          /*iterate through guess and key simultaneously*/
            {
                if(tIn[i] == tKey[i])           /*if the guess and key match perfectly*/
                {
                    perfect++;                      /*increment perfect*/
                    tIn[i] = '2';                     /*replace with placeholders so they cannot be counted in the rest of this iteration*/
                    tKey[i] = '3';
                }
            }

/*Get colors in wrong place*/
            for(i = 0; i < 4; i++)           /*iterate through guess*/
            {
                for(j = 0; j < 4; j++)           /*iterate through key*/
                {
                    if(tIn[i] == tKey[j])            /*if guess and key match imperfectly*/
                    {
                        rightLetter++;                  /*increment rightLetter*/
                        tIn[i] = '2';                     /*replace with placeholders so they cannot be counted in the rest of this iteration*/
                        tKey[j] = '3';
                    }
                }
            }

/*Print gues statistics*/
            printf("Colors in the correct place: %d\n", perfect);
            printf("Colors correct but in wrong position: %d\n\n", rightLetter);

/*Check for win*/
            if(perfect == 4)                /*if the user has guessed all 4*/
            {
                printf("You have won!\n");      /*prompt with win message*/
                break;                          /*break from gameplay loop*/
            }
/*Check for loss*/
            guessNum++;                     /*increment guesses*/
            if(guessNum >= 10)              /*if the user has surpassed 10 rounds*/
            {
                printf("You have lost.\n");     /*prompt with loss message*/
                break;                          /*break from gameplay loop*/
            }
        }
        printf("Would you like to play again?\n");  /*prompt user to play again and restart loop*/
    }
    return 0;   /*end main*/
}

/*Method to generate the winning key*/
char* keyGen(char key[])
{
    int gen;
    int i;
    for(i = 0; i < 4; i++)
    {
        gen = rand()%6+1;   /*generate a random number*/

        if(gen == 1)        /*translate it to a corresponding character*/
            key[i] = 'r';
        else if(gen == 2)
            key[i] = 'o';
        else if(gen == 3)
            key[i] = 'y';
        else if(gen == 4)
            key[i] = 'g';
        else if(gen == 5)
            key[i] = 'b';
        else
            key[i] = 'p';
    }
    key[4] = '\0';          /*terminate the string*/
    return key;             /*return 4 letter sequence*/
}
