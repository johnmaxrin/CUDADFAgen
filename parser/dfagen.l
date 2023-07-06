%{
#include "y.tab.h"
#include<string.h>
char *temp;
%}

%%
\"[a-z]+\"              { 
                            temp = strdup(yytext+1);
                            if(temp[yyleng - 2] != '"')
                                printf("Call YYERROR\n");
                            else
                                temp[yyleng - 2] = 0;
                            strcpy(yylval.value,temp);
                            return KEYWORD;
    
                        }


[r]\"[a-z|*()]+\"           {
                            temp = strdup(yytext+2);
                            if(temp[yyleng - 3] != '"')
                                printf("Call YYERROR\n");
                            else
                                temp[yyleng - 3] = 0;
                            strcpy(yylval.value,temp);
                            return PATTERN;
                        }

\:                      {return COLON;}

\<[A-Z]+\>              {
                            temp = strdup(yytext+1);
                            if(temp[yyleng - 2] != '>')
                                printf("Call YYERROR\n");
                            else
                                temp[yyleng-2] = 0;
                            strcpy(yylval.value,temp);
                            return TOKEN;
                        }
[ \t\n]                   ;
%%




int yywrap()
{
    return(1);
}