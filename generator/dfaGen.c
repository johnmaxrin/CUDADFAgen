#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ahocorasick(int,char[50][20], FILE *);
void generateHeader(FILE *file, char [50][20], int);
void generateDriverFunction(FILE *);
void thompsonRegex(FILE *, char[50][20], int);



int genDFA(char keywords[50][20],int keywordCount, char regex[50][20], int regexCount)
{
    
    FILE *file = fopen("plex.cu","w");

    generateHeader(file,keywords,keywordCount);

    ahocorasick(keywordCount,keywords,file);
    thompsonRegex(file,regex,regexCount);

    generateDriverFunction(file);


    return 0;
}


