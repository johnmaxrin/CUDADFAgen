%{

#include<stdio.h>
#include<string.h>
#include "include.h"
void yyerror(char *);
#include "y.tab.h"

char *test;
%}

%%

\"[a-zA-z]+\"              { 
                            
                                test = strdup(yytext+1);

                                if(test[yyleng - 2] != '"')
                                    printf("improperly terminated");
                                else
                                    test[yyleng- 2] = 0;
                                strcpy(yylval.value,test);  
                                return KEYWORD;
                                
                           }


[\n \t]                     ;
.                          {printf("Error\n");}


%%

int yywarp(void)
{
	return 1;
}

