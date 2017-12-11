#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc != 3) /*we need the program and two numbers*/
    {
        printf("Improper format. Run as follows './e_digit NUM NUM'\n");
        return 0;
    }

    int end = strtol(argv[2], NULL, 10);	/*get the size of the buffer - use atoi  instead?*/
	int off = strtol(argv[1], NULL, 10);	/*get the start of the substring*/
    int len = (end-off)+1;
    char* buf = malloc((len)*sizeof(int));

    int file = open("/dev/e", O_RDONLY);    /*open the driver to read from it*/
    /*           fdesc. out buf          bytes wanted                    offset*/
    //int n = read(file,  out,    sizeof(int)*(buffer_len-sub_start),   sub_start);
    int n = read(file, buf, len+1);

    if(n != len)
    {
        printf("incorrect output\n");
        return 1;
    }

    int i;
    for(i = off; i<len; i++)
    {
        printf("%c", buf[i]);
    }
    printf("\n");

    free(buf);

    return 0;
}
