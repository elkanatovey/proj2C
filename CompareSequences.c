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
#define WRONG_NUMBER_OF_ARGS 5
#define USAGE_ERROR "Usage: <path_to_sequence_file> <m> <s> <g>  \n"
#define FILE_NAME 1
#define BAD_FILE_NAME_ERROR "Error opening file: %s\n"
#define  MAX_LINE_LENGTH 100
#define HEADER_START '>'
#define FIRST_SEQUENCE 1
#define MEMORY_ALLOCTION_ERROR "ERROR: failure to allocate memory\n"
#define M 2
#define S 3
#define G 4
#define CONVERSION_ERROR "Error in weight conversion %s!\n"


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
    free(currentSequence.sequenceName);
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
    currentSequence[newSize - 1].sequenceName = (char*)malloc(sizeof(char)*lineSize+1);
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
                   sizeof(char)*(currentSequence[i].actualSequenceSize+lineSize+1));
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

void calculateScores(int size, Sequence *currentSequence, const int weights[])
{

}

int readLines(const FILE *currentFile, const int weights[3])
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
        }
    }
    calculateScores(i, currentSequence, weights);
    freeSequences(currentSequence, i);
    return 0;
}

/**
 * extract the weights from program args
 * @param argv  program args
 * @param weightArray where to put them
 * @return the new weights
 */
int* calculateWeights(char **argv, int weightArray[])
{
    int k;
    for (k = 2; k < G; ++k)
    {
        char *end = NULL;
        errno = 0;
        int currentCoordinate = (int) strtof(argv[k], &end);
        if (currentCoordinate == 0 && (errno != 0 || end == argv[k]))
        {
            fprintf(stderr, CONVERSION_ERROR, argv[k]);
            return NULL;
        }
        weightArray[k-2] = currentCoordinate;
    }
    return weightArray;
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
	    int weightArray[S] = {0};
	    int *updatedWeights = NULL;
	    updatedWeights = calculateWeights(argv, weightArray);
	    if(updatedWeights == NULL)
        {
            fclose(currentFile);
            return -1;
        }
	    int i = readLines(currentFile, updatedWeights);
//	    free(updatedWeights);
	    if(i != 0)
        {
            fclose(currentFile);
            return -1;
        }
		//do something
		fclose(currentFile);
	}
//    printf("Hello, World!\n");
    return 0;
}


//int k;
//    for(k = 0; k <= i; k++)
//    {
//        printf( "%s\n", currentSequence[k].actualSequence);
////        printf('%d\n',0);
//    }
