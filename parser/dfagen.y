%{
    #include<stdio.h>
    #include<string.h>

    void yyerror(char *);
    int yylex(void);

    int keywordcount = 0;
    int regexcount = 0;
    
    char keywords[50][20];
    char keywordTokens[50][20];
    char regex[50][20];
    char regexTokens[50][20];

    int genDFA(char [50][20] , int ,char [50][20], int);

%}

%union{
        char value[50];
        int tokenindex;
}

%token <value> KEYWORD PATTERN TOKEN
%token COLON SEMICOLON

%%


prgm :  lines                                {genDFA(keywords,keywordcount,regex,regexcount);}
        ;

lines :                                            
        line
        |                                
        lines line                                   
        ;

line :                                              
        keyword 
        |
        regex 
        ;

keyword:    
        KEYWORD COLON TOKEN                {strcpy(keywords[keywordcount],$1); strcpy(keywordTokens[keywordcount++],$3);}
        ;
        
regex: 
        PATTERN COLON TOKEN                {strcpy(regex[regexcount],$1); strcpy(regexTokens[regexcount++],$3);}
        ;



%%

extern FILE *yyin;

void yyerror(char *e)
{
    printf("Error: %s\n",e);
}

int main(int argc, char *argv[])
{
    FILE *file;
    file = fopen(argv[1], "r");

    yyin = file;

    do{
        yyparse();
    }while(!feof(yyin));
}


