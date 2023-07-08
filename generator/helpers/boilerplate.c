#include<stdio.h>
#include <ctype.h>

void generateHeader(FILE *file, char p[50][20], int count)
{  


     
    char temp[50];
    fprintf(file,"// Auto generated lexical analyser for MA14\n"); // Change this in future.
    fprintf(file,"#include<stdio.h>\n\n");
    fprintf(file,"struct Regex\n{\n\tint **table;\n\tint *end;\n\tint *symbolList;\n\tint *alpArray;\n};\n\n");

    fprintf(file,"enum Keywords\n{\n\tNONE,");
    
    for(int i=0; i<count-1; ++i)
    {   fprintf(file,"\n\t");
        int j=0;
        for(j=0; p[i][j] != '\0'; ++j)
            temp[j] = toupper(p[i][j]);
        temp[j] = '\0';
        
        fprintf(file,"%s",temp);
        fprintf(file, ",");
    }

    fprintf(file,"\n\t");
    int j=0;
    for(j=0; p[count-1][j] != '\0'; ++j)
        temp[j] = toupper(p[count-1][j]);
    temp[j] = '\0';
        
    fprintf(file,"%s",temp);
    fprintf(file,"\n};\n\n");


}

void generateDriverFunctionforKeyword(FILE *file)
{
    fprintf(file,"%s","\nint checkKeyword()\n{\n\tchar test[50];\n\tprintf(\"Enter a keyword: \");\n\tscanf(\"\%s\",test);\n\tint cstate = 0;\n\tfor(int i=0; test[i] != '\\0'; ++i)\n\t{\n\t\tint ch = alpArray[test[i]-'a'];\n\t\tcstate = dfaTable[cstate][ch];\n\t}\n\tprintf(\"\\nThis is:\%d\\n\",outputArray[cstate]);\n\treturn 0;\n}\n\n");
}

void generateDriverFunctionforRegex(FILE *file)
{

fprintf(file, "int checkRegex(){\nprintf(\"Enter a string to check\");\nchar string[50];\nint i=0, currentState = 1;\nwhile (string[i] != '\\0')\n{\n\t    currentState = dTran[0][currentState][symbolList[regexAlphArray[string[i]-'a']]];\n\t++i;\n}\nif(finalStates == currentState)\n{\n\tprintf(\"1\");\n}\nelse\n\tprintf(\"0\");\nreturn 0;\n}");

}