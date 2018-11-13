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
#define FIRST_SEQUENCE 1
#define MEMORY_ALLOCTION_ERROR "ERROR: failure to allocate memory\n"


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


/**
 * frees header and sequence pointers in a Sequence struct
 * @param currentSequence the individual sequence
 */
void freeSequencePointers(Sequence currentSequence)
{
    free(currentSequence.actualSequence);
    currentSequence.actualSequence = NULL;
    free(currentSequence.sequenceName);
    currentSequence.sequenceName = NULL;
}

/**
 * Frees sequence by sequence
 * @param currentSequence the total sequence pointer
 * @param size the number of existing sequences
 */
void freeSequences(Sequence *currentSequence, int size)
{
    int i;
    for (i = size; i >=0 ; i--) {
        freeSequencePointers(currentSequence[i]);
    }
    free(currentSequence);
    currentSequence = NULL;
}


/**
 * Dynamically add new sequences if necessarry
 * @param currentSequence the sequence pointer
 * @param newSize the new size
 * @param header the name
 * @param lineSize size of current line
 * @return our updated sequence pointer
 */
Sequence* createNewSequence(Sequence *currentSequence, int newSize, char header[], int lineSize)
{
    Sequence *tempSequence = realloc(currentSequence, sizeof(Sequence)*newSize);
    if(tempSequence == NULL)
    {
        if(newSize > FIRST_SEQUENCE)
        {
            freeSequences(currentSequence, (newSize - 1));
        }
        else
        {
            free(currentSequence);
        }
        fprintf(stderr, MEMORY_ALLOCTION_ERROR);
        return NULL;
    }
    currentSequence = tempSequence;
    currentSequence[newSize - 1].sequenceNameSize = lineSize;
    currentSequence[newSize - 1].sequenceName = NULL;
    currentSequence[newSize - 1].actualSequence = NULL;
    currentSequence[newSize - 1].sequenceName = (char*)malloc(sizeof(char)*lineSize);
    currentSequence[newSize - 1].actualSequence = (char*)malloc(sizeof(char)*1);
    if(currentSequence[newSize - 1].actualSequence == NULL ||
        currentSequence[newSize - 1].sequenceName == NULL)
    {
        freeSequences(currentSequence, (newSize - 1));
        fprintf(stderr, MEMORY_ALLOCTION_ERROR);
        return NULL;
    }
    strncat(currentSequence[newSize - 1].sequenceName, header, sizeof(char)*lineSize);
    currentSequence[newSize - 1].actualSequenceSize = 0;
    return currentSequence;
}


int growLineSize(Sequence *currentSequence, int i, int lineSize, char currentLine[])
{
    char *tempSequence = NULL;
    tempSequence = (char*)realloc(currentSequence[i].actualSequence,
                   sizeof(char)*(currentSequence[i].actualSequenceSize+lineSize));
    if(tempSequence == NULL)
    {
        freeSequences(currentSequence, i-1);
        fprintf(stderr, MEMORY_ALLOCTION_ERROR);
        return -1;
    }
    currentSequence[i].actualSequence = tempSequence;
    currentSequence[i].actualSequenceSize = currentSequence[i].actualSequenceSize+lineSize;
    strncat(currentSequence[i].actualSequence, currentLine, sizeof(char)*lineSize);
    return 0;
}


int readLines(const FILE *currentFile, char *fileName)
{
    int i = -1;
    char currentLine[MAX_LINE_LENGTH] = {0};
    Sequence *currentSequence = NULL;
    currentSequence = (Sequence*)malloc(sizeof(Sequence)*(1));
    if(currentSequence == NULL)
    {
        fprintf(stderr, MEMORY_ALLOCTION_ERROR);
        return -1;
    }
    else
    while(fgets(currentLine, MAX_LINE_LENGTH, (FILE *) currentFile) != NULL)
    {  //possible edge case empty line
        int lineSize = cleanLine(currentLine);
        if (currentLine[0] == HEADER_START) {
            i++;
            currentSequence = createNewSequence(currentSequence, (i + 1), currentLine, lineSize);
            if(currentSequence == NULL)
            {
                return -1;  // freed in createNewLines
            }
        }
        else
        {
            if(growLineSize(currentSequence, i, lineSize, currentLine))
            {
                return -1;
            }
//            currentSequence[i].actualSequence = (char*)realloc(currentSequence[i].actualSequence,
//                    sizeof(char)*(currentSequence[i].actualSequenceSize+lineSize));
//            currentSequence[i].actualSequenceSize = currentSequence[i].actualSequenceSize+lineSize;
//            strncat(currentSequence[i].actualSequence, currentLine, sizeof(char)*lineSize);
        }
    }
    int k;
    for(k = 0; k <= i; k++)
    {
        printf( "%s\n", currentSequence[k].actualSequence);
//        printf('%d\n',0);
    }
    freeSequences(currentSequence, i);
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
