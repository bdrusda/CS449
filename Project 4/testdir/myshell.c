/*	
  Brendan Drusda
  CS449 Project 4 - A Shell
  Due November 27 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h>

void handle_errors(int, char*, char*);

int main() 
{
	char delims[11] = "()<>|&;\n\t ";	/*array of delimited characters*/ 

	printf("Welcome to A Shell\n"); 	/*welcome the user to the shell*/
	while(1) 				/*execute until exit command*/
	{
		printf(">");
		char input[250]; 			/*array holding user input*/
		char* token; 				/*the current token*/
		int i = 0;
		int argc = 0;

	/*get user input*/
		fgets(input, 250, stdin);

	/*determine the number of arguments to allocate for*/
		char* get_argc;
		char temp[250];
		strcpy(temp, input); 			/*get a copy of the input*/
		get_argc = strtok(temp, delims);	/*get the command*/
		while(get_argc != NULL) 		/*get the number of arguments*/
		{
			argc++; 				/*increment arg count*/
			get_argc = strtok(NULL, delims);	/*move to next argument*/
		}

	/*save a copy of the input to tokenize*/
		char** args = malloc(sizeof(char*) * (argc+1));	/*allocate enough room for command+argc  string pointers*/
		char to_token[250];
		strcpy(to_token, input);
		for(i = 0; i < argc+1; i++) 		/*ensure values are initialized to NULL*/
		{
			args[i] = 0;
		}

	/*tokenize the user's input*/
		token = strtok(to_token, delims);	/*this should find the first delimiter and return the  values before it.  This is destructive so we use the NULL value inside of strtok to get the rest of input*/

		i = 0;

		while(token != NULL) 			/*save the arguments in the newly allocated array*/
		{
			args[i] = malloc(sizeof(char) * strlen(token));	/*create a new space in memory for  the argument pointer to point to*/
			strcpy(args[i], token); 		/*copy the argument into this space*/
			token = strtok(NULL, delims); 		/*get the next argument*/
			i++; 					/*move to the next space in the array*/
		}

		/*Verify the there was an input*/
		if(args[0] == NULL)			/*check if the first argument is null*/
		{
			continue;				/*if there wasn't an input, wait for one*/
		}
	/*handle commands*/
		if(!strcmp(args[0], "exit")) 		/*handle the exit command*/
		{
			printf("Exiting A Shell\n"); 		/*inform user that the shell is exiting*/
			exit(0); 				/*exit normally*/
		}
		else if(!strcmp(args[0], "cd")) 	/*handle the cd command*/
		{
			/*check that there is atleast one more parameter*/
			if(args[1] == NULL)
			{
				printf("No directory specified\n");
			}

			int result = chdir(args[1]); 		/*tells us if the process was a success ------ NEED to  do more with this*/
			if(result != 0)				/*if there was some error*/
			{
				handle_errors(errno, "chdir", args[1]);
			}
		}
		else 					/*handle external UNIX commands*/
		{
			int id = fork();		/*split the process*/
			if(id == 0) 			/*child process*/
			{
				/*check if there is any input/output redirection*/
				if(strstr(input, ">>"))		/*append*/
				{
					void* result = freopen(args[argc-1], "a", stdout);	/*append to a file*/
					if(result == 0)
					{
						printf("file could not be opened\n");
						continue;
					}
					free(args[argc-1]); 		/*once the command is executed, free the allocated memory*/
					args[argc-1] = NULL;		/*ensure the pointer pointing to the freed memory is null*/
				}
				else if(strstr(input, ">"))	/*write*/
				{
					void* result = freopen(args[argc-1], "w", stdout);	/*write to the file*/
					if(result == 0)
					{
						printf("file could not be opened\n");
						continue;
					}
					free(args[argc-1]); 		/*once the command is executed, free the allocated memory*/
					args[argc-1] = NULL;		/*ensure the pointer pointing to the freed memory is null*/
				}
				else if(strstr(input, "<"))	/*input file*/
				{
					void* result = freopen(args[argc-1], "r", stdout);	/*use this file as input*/
					if(result == 0)
					{
						printf("file could not be opened\n");
						continue;
					}
					free(args[argc-1]); 		/*once the command is executed, free the allocated memory*/
					args[argc-1] = NULL;		/*ensure the pointer pointing to the freed memory is null*/
				}
				
		/*Search the arguments list to see if there was a ">", ">>", or "<"*/
				int result = execvp(args[0], args); 		/*run the process given the list of arguments*/
				if(result == -1)
				{
					handle_errors(errno, "execvp", args[1]);
				}
				exit(0); 			/*when the command is done, kill the process so the parent can resume*/
			}
			else 				/*parent process*/
			{
				int exit_status;
				wait(&exit_status); 		/*wait for child to finish*/

				if(exit_status == -1)
				{
					handle_errors(errno, "wait", args[1]);
				}
			}
		}
	}

	return 0;
}

void handle_errors(int err, char* process, char* dir)
{
	if(!strcmp(process, "chdir"))
	{
		if(err == EACCES)
		{
			printf("chdir: permission denied for specified directory \"%s\"\n", dir);
		}
		else if(err == EFAULT)
		{
			printf("chdir: bad address\n");
		}
		else if(err == EIO)
		{
			printf("chdir: input/output error\n");
		}
		else if(err == ELOOP)
		{
			printf("chdir: too many symbolic links encountered\n");
		}
		else if(err == ENAMETOOLONG)
		{
			printf("chdir: path name too long\n");
		}
		else if(err == ENOENT)
		{
			printf("chdir: could not find the specified directory \"%s\"\n", dir);
		}
		else if(err == ENOTDIR)
		{
			printf("chdir: \"%s\" not a directory\n", dir);
		}
		else if(err == EBADF)
		{
			printf("chdir: bad file number\n");
		}
		else
		{
			printf("chdir: unspecified error: %d\n", err);
		}
	}
	else if(!strcmp(process, "execvp"))
	{
		if(err == E2BIG)
		{
			printf("execvp:argument list too large\n");
		}
		else if(err == EACCES)
		{
			printf("execvp: permission denied for specified file\n");
		}
		else if(err == EFAULT)
		{
			printf("execvp: bad address\n");
		}
		else if(err == EINVAL)
		{
			printf("execvp: ELF contains more than one interpreter\n");
		}
		else if(err == EIO)
		{
			printf("execvp: input/output error\n");
		}
		else if(err == EISDIR)
		{
			printf("execvp: ELF interpreter was a directory\n");
		}
		else if(err == ELIBBAD)
		{
			printf("execvp: ELF interpreter was not in recognized format\n");
		}
		else if(err == ELOOP)
		{
			printf("execvp: too many symbolic links encountered\n");
		}
		else if(err == EMFILE)
		{
			printf("execvp: system limit of maximum files is open\n");
		}
		else if(err == ENAMETOOLONG)
		{
			printf("execvp: file name too long\n");
		}
		else if(err == ENFILE)
		{
			printf("execvp: system limit of total number of open files reached\n");
		}
		else if(err == ENOENT)
		{
			printf("execvp: requested program not found\n");
		}
		else if(err == ENOEXEC)
		{
			printf("execvp: executable is not in a recognized format\n");
		}
		else if(err == ENOMEM)
		{
			printf("execvp: insufficient kernel memory\n");
		}
		else if(err == ENOTDIR)
		{
			printf("execvp: component of path is not a directory\n");
		}
		else if(err == EPERM)
		{
			printf("execvp: user is not superuser\n");
		}
		else if(err == ETXTBSY)
		{
			printf("execvp: executable open for writing for more than one process\n");
		}
		else
		{
			printf("execvp: unspecified error: %d\n", err);
		}
	}
	else
	{
		if(err == ECHILD)
		{
			printf("process has no child\n");
		}
		else if(err == EINTR)
		{
			printf("unblocked signal was caught\n");
		}
		else if(err == EINVAL)
		{
			printf("options argument invalid\n");
		}
		else
		{
			printf("wait: unspecified error: %d\n", err);
		}
	}
}
