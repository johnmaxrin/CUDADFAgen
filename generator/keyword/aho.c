#include<stdio.h>
void ahocorasick(int count, char data[50][20], FILE *file)
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
        for (i = 0; data[tempnow-1][i] != '\0'; ++i)
            ;
        len += i;
        --tempnow;
    } // Finding the total number of letters entire data has. Len has the total number of letters (a.k.a states)

    tempnow = count;
    while (tempnow)
    {
        for (int i = 0; data[tempnow-1][i] != '\0'; ++i)
        {
            if (alpArray[data[tempnow-1][i] - 'a'] == -1)
                alpArray[data[tempnow-1][i] - 'a'] = clen++;
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
        for(size=0; data[i][size]!='\0'; ++size);

        int currentState = 0;

        for (int j = 0; j < size; ++j)
        {
            int ch = alpArray[data[i][j] - 'a'];
 
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