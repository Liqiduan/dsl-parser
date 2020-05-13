%{
#include <stdio.h>
%}

%token NUMBER

%%

start:;


%%

yyerror(char* s)
{
    fprintf(stderr, "%s\n", s);
}
