%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"
extern FILE* yyin;

int yylex(void);
void yyerror(char *); 
nodeType *genkeyword(char *);
void freeNode(nodeType *p);
int genDFA(nodeType *p, int count);
nodeType* addKeyword(nodeType *p, nodeType *list);

nodeType keywordListArray[50];
int count=0;

%}

%union{
    char value[50];
    nodeType *nPtr;
}


%token <value>KEYWORD;
%type <nPtr> keywordList keyword ;
%%

    dfa:
        dfa keywordList                     {genDFA($2,count);}
        |
        ;


    keywordList:
                keywordList keyword         {$$ = addKeyword($2,keywordListArray);}
                |
                ;

    
    keyword:
                KEYWORD         {$$ = genkeyword($1);}
                ;
%%

nodeType* addKeyword(nodeType *p, nodeType *list)
{
    nodeType node;
    node.type = p->type;
    strcpy(node.keyword.value,p->keyword.value);
    list[count++] = node;

    freeNode(p);
    return list;
}

void freeNode(nodeType *p) {
    if (!p) return;
    free (p);
}




nodeType *genkeyword(char *value)
{
    nodeType *p;
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    
    p->type = KEYWORDNODE;
    strcpy(p->keyword.value,value);
    return p; 
}

void yyerror(char *err)
{
    printf("Error: %s\n",err);
}

int main(int c, char *argv[])
{
    FILE* file = fopen(argv[1], "r");
    yyin = file; 
    yyparse();
    return 0;
}