#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ahocorasick(int,char[50][20], FILE *);
void generateHeader(FILE *file, char [50][20], int);
void generateDriverFunction(FILE *);




int genDFA(char keywords[50][20], int keywordCount, char **regex, int regexCount)
{
    
    FILE *file = fopen("plex.cu","w");

    printf("%s\n",*keywords);

    generateHeader(file,keywords,keywordCount);

    ahocorasick(keywordCount,keywords,file);
    generateDriverFunction(file);

    return 0;
}


