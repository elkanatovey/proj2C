/**
 * @file CompareSequences.c
 * @author  Elkana Tovey <elkana.tovey@mail.huji.ac.il>
 * @version 1.0
 * @date 31 2018
 * @brief System to calculate 
 *
 * @section LICENSE
 * This program is an assignment for course 67136; bla bla bla...
 * @section DESCRIPTION
 * The system calculates 
 * Input  : a text file in 
 * Process: parse the file, a
 * Output : 
 */

//------------------------------ includes ------------------------------
//sys includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// my includes
#define WRONG_NUMBER_OF_ARGS 4
#define USAGE_ERROR "Usage:   \n"
#define FILE_NAME 1
#define BAD_FILE_NAME_ERROR "Error opening file: %s\n"
#define  MAX_LINE_LENGTH 100
#define HEADER_START ">"


typedef struct Sequence
{
    char *sequenceName;
    int sequenceNameSize;
    char *actualSequence;
    int actualSequenceSize;
}Sequence;


int readLines(const FILE *currentFile, char *fileName)
{
    int i = 0;
    Sequence *sequencesPointer = NULL;
//    sequencesPointer =
    char currentLine[MAX_LINE_LENGTH];
    char *sequenceAppender = NULL;
    while(fgets(currentLine, MAX_LINE_LENGTH, (FILE *) currentFile) != NULL)
    {  //possible edge case empty line
//        if(currentLine[0] == HEADER_START)
//        {
//            i++;
//            sequencesPointer = (*sequencesPointer)realloc(sequencesPointer, sizeof(Sequence)*i);
//            sequencesPointer[i-1].
        printf("aaa");
        }
    }
}






int main(int argc, char *argv[]) 
{
	if(argc != WRONG_NUMBER_OF_ARGS) // no files entered
	{
	    printf(USAGE_ERROR);  //usage print and exits
		return 1;
	}
	FILE *currentFile;
	currentFile = fopen(argv[FILE_NAME], "r");
	if(currentFile == NULL)
	{
		fprintf(stderr, BAD_FILE_NAME_ERROR, argv[FILE_NAME]);
		return 1;
	}
	else
	{
		//do something
		fclose(currentFile);
	}
    printf("Hello, World!\n");
    return 0;
}
