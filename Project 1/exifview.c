/*  Brendan Drusda
    Project 1 - EXIF Viewer*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exif_structs.h"

void get_sub_tags(FILE* fPtr);

int main(int argc, char* argv[])
{
    /*Declarations*/
    int i;
    unsigned int val1;
    unsigned int val2;
    float val3;
    unsigned short count;
    unsigned int loc;
    struct exif_header exif;
    struct tiff_tag tag;
    FILE *fPtr;

    /*Verify the correct number of command line arguments were used*/
    if(argc < 2 || argc > 2)
    {
        printf("Please provide the JPEG file in your command line arguments(./exifview img1.jpg)\n");
        return 0;
    }

    /*Create and verify file*/
    fPtr = fopen(argv[1], "rb");
    if(fPtr == NULL)
    {
        printf("File not found\n");
        return 0;
    }

    /*--Retrieve EXIF Info--*/
    fread(&exif, sizeof(struct exif_header), 1, fPtr);
    /*Verify EXIF Info is Correct*/
    if(exif.JPEG_marker != 0xD8FF)      /*Verify JPEG marker*/
    {
        printf("File not a JPEG, exiting\n");
        return 0;
    }

    if(exif.APP1 != 0xE1FF)              /*Verify APP1 block*/
    {
        printf("No APP1 block recognized, exiting\n");
        return 0;
    }

    exif.string[4] = '\0';
    if(strcmp(exif.string, "Exif"))     /*Verify Exif String is in correct place*/
    {
        printf("File does not contain Exif data, exiting\n");
        return 0;
    }

    if(strcmp(exif.endianness, "II*"))   /*Verify endianness*/
    {
        printf("Endianness not supported, exiting\n");
        return 0;
    }

/*--Retrieve TIFF Tags--*/
    /*Get count of tags*/
    fread(&count, sizeof(count), 1, fPtr);

    for(i = 0; i < count; i++)
    {
        /*Read tag into structure*/
        fread(&tag, sizeof(struct tiff_tag), 1, fPtr);
        if(tag.iden == 0x010F)   /*if the identifier 0x010F (Manufacturer String)*/
        {
            loc = ftell(fPtr);                                  /*save location prior to moving to offset*/
            char* buf = malloc(tag.num_items * sizeof(char));   /*allocate memory for info*/
            if(buf == NULL)                                     /*verify memory was allocated*/
            {
                printf("Memory couldnot be allocated, exiting");
                return 0;
            }

            fseek(fPtr, tag.offset+12, SEEK_SET);               /*jump to the offset in the tag*/
            fread(buf, sizeof(char), tag.num_items, fPtr);      /*read in the string*/

            printf("Manufacturer:\t%s\n", buf);                 /*print info*/
            free(buf);                                          /*free allocated memory*/
            fseek(fPtr, loc, SEEK_SET);                         /*jump back to previous point*/
        }
        else if(tag.iden == 0x0110)  /*if the identifier is 0x0110 (Camera Model String)*/
        {
            loc = ftell(fPtr);  /*save location prior to moving to offset*/
            fseek(fPtr, tag.offset+12, SEEK_SET);
            char* buf = malloc(tag.num_items * sizeof(char));
            if(buf == NULL)
            {
                printf("Memory couldnot be allocated, exiting");
                return 0;
            }

            fseek(fPtr, tag.offset+12, SEEK_SET);
            fread(buf, sizeof(char), tag.num_items, fPtr);

            printf("Model:\t\t%s\n", buf);
            free(buf);
            fseek(fPtr, loc, SEEK_SET);
        }
        else if(tag.iden == 0x8769) /*if the identifier is 0x8769 (Exif sub block address)*/
        {
            fseek(fPtr, tag.offset+12, SEEK_SET);

            struct tiff_tag sub;    /*create new structure to hold sub block info*/
            fread(&count, sizeof(count), 1, fPtr);

            for(i = 0; i < count; i++)
            {
                fread(&sub, sizeof(struct tiff_tag), 1, fPtr);
        /*Recognize Tag Identifiers*/
                /* DEBUG printf("%X\n", sub.iden); */
                if(sub.iden == 0xA002)      /*Width*/
                {
                    printf("Width:\t\t%d pixels\n", sub.offset);
                }
                else if(sub.iden == 0xA003) /*Height*/
                {
                    printf("Height:\t\t%d pixels\n", sub.offset);
                }
                else if(sub.iden == 0x8827) /*ISO Speed*/
                {
                    printf("ISO Speed:\tISO %d\n", sub.offset);
                }
                else if(sub.iden == 0x829a) /*Exposure Time*/
                {
                    loc = ftell(fPtr);
                    fseek(fPtr, sub.offset+12, SEEK_SET);

                    fread(&val1, sizeof(val1), 1, fPtr);
                    fread(&val2, sizeof(val2), 1, fPtr);

                    fseek(fPtr, loc, SEEK_SET);
                    printf("Exposure Time:\t%d/%d second\n", val1, val2);   /*written in the form of 1/80 second*/
                }
                else if(sub.iden == 0x829d) /*F-Stop*/
                {
                    loc = ftell(fPtr);
                    fseek(fPtr, sub.offset+12, SEEK_SET);

                    fread(&val1, sizeof(val1), 1, fPtr);
                    fread(&val2, sizeof(val2), 1, fPtr);
                    val3 = (double)val1/val2;

                    fseek(fPtr, loc, SEEK_SET);
                    printf("F-Stop:\t\tf/%1.1f\n", val3);   /*written in form of f/2.8*/
                }
                else if(sub.iden == 0x920A) /*Lens Focal Length*/
                {
                    loc = ftell(fPtr);
                    fseek(fPtr, sub.offset+12, SEEK_SET);

                    fread(&val1, sizeof(val1), 1, fPtr);
                    fread(&val2, sizeof(val2), 1, fPtr);

                    val3 = (double)val1/val2;
                    printf("Focal Length:\t%2.0f mm\n", val3);  /*written in form of 40 mm*/

                    fseek(fPtr, loc, SEEK_SET);
                }
                else if(sub.iden == 0x9003) /*Date Taken*/
                {
                    loc = ftell(fPtr);
                    char* buf = malloc(sub.num_items * sizeof(char));
                    if(buf == NULL)
                    {
                        printf("Memory couldnot be allocated, exiting");
                        return 0;
                    }

                    fseek(fPtr, sub.offset+12, SEEK_SET);               /*jump to the offset in the sub*/
                    fread(buf, sizeof(char), sub.num_items, fPtr);     /*read in the string*/
                    printf("Date Taken:\t%s\n", buf);
                    free(buf);

                    fseek(fPtr, loc, SEEK_SET);
                }
            }
        }
    }
    fclose(fPtr);   /*close the file*/
    return 0;
}
