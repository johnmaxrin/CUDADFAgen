#include "../include.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ahocorasick(int,nodeType*, FILE *);
void generateHeader(FILE *file, nodeType*, int);
void generateDriverFunction(FILE *);


void generateHeader(FILE *file,nodeType* p, int count)
{   
    char temp[50];
    fprintf(file,"// Auto generated lexical analyser for MA14\n"); // Change this in future.
    fprintf(file,"#include<stdio.h>\n\n"); 
    fprintf(file,"enum\n{\n\tNONE,");
    for(int i=0; i<count-1; ++i)
    {   fprintf(file,"\n\t");
        int j=0;
        for(j=0; p[i].keyword.value[j] != '\0'; ++j)
            temp[j] = toupper(p[i].keyword.value[j]);
        temp[j] = '\0';
        
        fprintf(file,"%s",temp);
        fprintf(file, ",");
    }

    fprintf(file,"\n\t");
    int j=0;
    for(j=0; p[count-1].keyword.value[j] != '\0'; ++j)
        temp[j] = toupper(p[count-1].keyword.value[j]);
    temp[j] = '\0';
        
    fprintf(file,"%s",temp);
    fprintf(file,"\n};\n\n");


}

int genDFA(nodeType *p, int count)
{
    
    FILE *file = fopen("plex.cu","w");
    generateHeader(file,p,count); 
    ahocorasick(count,p,file);
    generateDriverFunction(file);

    return 0;
}

void generateDriverFunction(FILE *file)
{
    fprintf(file,"%s","\nint main()\n{\n\tchar test[50];\n\tprintf(\"Enter a keyword: \");\n\tscanf(\"\%s\",test);\n\tint cstate = 0;\n\tfor(int i=0; test[i] != '\\0'; ++i)\n\t{\n\t\tint ch = alpArray[test[i]-'a'];\n\t\tcstate = dfaTable[cstate][ch];\n\t}\n\tprintf(\"\\nThis is:\%d\\n\",outputArray[cstate]);\n\treturn 0;\n}\n");
}

void ahocorasick(int count, nodeType *data, FILE *file)
{

    // Variable Section
    int tempnow = 0, ualphaid = 0; // Number of words
    int alpArray[26];
    // End of variable section.

    // Init section
    for (int i = 0; i < 26; ++i)
        alpArray[i] = -1; // Initializing alphArray with -1.
    // End of init section



    tempnow = count;
    int len = 0, clen = 0;
    while (tempnow)
    {
        int i = 0;
        for (i = 0; data[tempnow-1].keyword.value[i] != '\0'; ++i)
            ;
        len += i;
        --tempnow;
    } // Finding the total number of letters entire data has. Len has the total number of letters (a.k.a states)

    tempnow = count;
    while (tempnow)
    {
        for (int i = 0; data[tempnow-1].keyword.value[i] != '\0'; ++i)
        {
            if (alpArray[data[tempnow-1].keyword.value[i] - 'a'] == -1)
                alpArray[data[tempnow-1].keyword.value[i] - 'a'] = clen++;
        }

        --tempnow;
    }

    const int flen = len;
    const int fclen = clen;
    int dfaTable[flen + count][fclen];

    for (int i = 0; i < flen + count; ++i)
        for (int j = 0; j < fclen; ++j)
            dfaTable[i][j] = -1;
    // dfaTable init with -1.

    int state = 1, lastState=0;
    tempnow = count;
    int out[50]={0};
    for (int i = 0; i < tempnow; ++i)
    {
        int size = 0;
        for(size=0; data[i].keyword.value[size]!='\0'; ++size);

        int currentState = 0;

        for (int j = 0; j < size; ++j)
        {
            int ch = alpArray[data[i].keyword.value[j] - 'a'];
 
            if (dfaTable[currentState][ch] == -1)
                dfaTable[currentState][ch] = state++;
 
            currentState = dfaTable[currentState][ch];
        }
 
        out[currentState]  = (i+1);
    } // Done with the building of DFA table.


    // Print the DFA Table and output array into file.
    fprintf(file,"static int dfaTable[%d][%d] = {\n",flen+count,fclen);

    for(int i=0; i<flen+count; ++i)
    {
        fprintf(file,"\t{");
        for(int j=0; j<fclen-1; ++j)
        {
            fprintf(file,"%d, ",dfaTable[i][j]);
        }

        fprintf(file,"%d},\n",dfaTable[i][fclen-1]);
    }

    fprintf(file,"\n};\n\n");

    fprintf(file,"static int outputArray[%d] = {\n\t",50);
    for(int i=0; i<50; ++i)
    {
        fprintf(file,"%d, ",out[i]);
        i%5 == 4 ? fprintf(file,"\n\t") : fprintf(file,"%s","");
    }

    fprintf(file,"\n};\n\n");

    fprintf(file,"static int alpArray[26] = {\n\t");
    for(int i=0; i<26; ++i)
    {
        fprintf(file,"%d, ",alpArray[i]);
        i%5 == 4 ? fprintf(file,"\n\t") : fprintf(file,"%s","");
    }
    fprintf(file,"\n};\n\n");



}
