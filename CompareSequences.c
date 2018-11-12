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
#include <string.h>

// my includes
#define WRONG_NUMBER_OF_ARGS 4
#define USAGE_ERROR "Usage:   \n"
#define FILE_NAME 1
#define BAD_FILE_NAME_ERROR "Error opening file: %s\n"
#define  MAX_LINE_LENGTH 100
#define HEADER_START '>'
#define TITLE_LOCATION 0
#define SEQUENCE_LOCATION 1


typedef struct Sequence
{
    char *sequenceName;
    int sequenceNameSize;
    char *actualSequence;
    int actualSequenceSize;
}Sequence;

/**
 * removes  carriage return and end of line symbols from stringd by null terminating at correct
 * point
 * @param currentLine the current line to clean (usually the buffer)
 * @return the new line size
 */
int cleanLine(char *currentLine)
{
    int lineSize = (int)strlen(currentLine);
    if(currentLine[lineSize-1] == '\n' || currentLine[lineSize-1] == '\r')
    {
        currentLine[lineSize-1] = '\0';
        lineSize = cleanLine(currentLine);
    }
    return lineSize;
}



char* createSpaceInSequence(char *sequenceHolder, int lineSize, char *currentLine)
{
    int size = 0;
    if(sequenceHolder == NULL)
    {
        size = lineSize;
    }
    else
    {
        size = lineSize + (int) sizeof(sequenceHolder);
    }
    char *sequence_temp = (char*)realloc(sequenceHolder, sizeof(char)*size );
    if(sequence_temp == NULL)
    {
        //free etc
        return NULL;
    }
    sequenceHolder = sequence_temp;
    sequenceHolder = strncpy(sequenceHolder, currentLine, sizeof(char)*lineSize);
    return sequenceHolder;
}




int readLines(const FILE *currentFile, char *fileName)
{
    int i = -1;
    char currentLine[MAX_LINE_LENGTH] = {0};
    Sequence *currentSequence;
    currentSequence = (Sequence*)realloc(currentSequence, sizeof(Sequence)*(1));
    while(fgets(currentLine, MAX_LINE_LENGTH, (FILE *) currentFile) != NULL)
    {  //possible edge case empty line
        int lineSize = cleanLine(currentLine);
        if (currentLine[0] == HEADER_START) {
            i++;
            currentSequence = (Sequence*)realloc(currentSequence, sizeof(Sequence)*(i+1));
            currentSequence[i].sequenceNameSize = lineSize;
            currentSequence[i].sequenceName = (char*)malloc(sizeof(char)*lineSize);
            strncpy(currentSequence[i].sequenceName, currentLine, sizeof(char)*lineSize);

            currentSequence[i].actualSequenceSize = 0;
            currentSequence[i].actualSequence = (char*)malloc(sizeof(char)*1);

        }
        else
        {
            currentSequence[i].actualSequence = (char*)realloc(currentSequence[i].actualSequence,
                    sizeof(char)*(currentSequence[i].actualSequenceSize+lineSize));
            currentSequence[i].actualSequenceSize = currentSequence[i].actualSequenceSize+lineSize;
            strncpy(currentSequence[i].actualSequence, currentLine, sizeof(char)*lineSize);
        }
    }
    int k;
    for(k = 0; k <= i; k++)
    {
        printf( "%s\n", currentSequence[k].actualSequence);
//        printf('%d\n',0);
    }

    return 0;
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
	    readLines(currentFile, argv[FILE_NAME]);
		//do something
		fclose(currentFile);
	}
//    printf("Hello, World!\n");
    return 0;
}
