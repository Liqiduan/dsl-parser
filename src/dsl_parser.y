%{
#include <stdio.h>
#include "dsl.h"

DslElement* result;

void yyerror(char* s)
{
    fprintf(stderr, "%s\n", s);
}

%}

%union {
    DslValue* value;
    DslElement* element;
}

%token <value> VAR NUMBER STR
%token LP RP LC RC
%token SEP
%token SPACE

%type <value> values value name arg body
%type <element> element

%%

start:
     |start element {result = $2;}
     ;

element:name arg body {$$ = DslElementNew($1, $2, $3);}
       ;

name:VAR
    ;

arg:|LP RP {$$ = NULL;}
    |LP SPACE RP {$$ = NULL;}
    |LP values RP {$$ = $2;}
   ;

body:LC RC {$$ = NULL;}
    |LC SPACE RC {$$ = NULL;}
    |LC values RC {$$ = $2;}
    ;

values:value
      |value sep values {$1->next = $3; $$ = $1;}
      ;

sep:SPACE SEP SPACE
   ;

value:NUMBER
     |VAR
     |STR
     |element {$$ = DslValueNewElement($1);}
     ;

%%

int DslParse(char *str, DslElement **dsl)
{
    int ret;

    yy_scan_string(str);

    ret = yyparse();
    if (ret == 0) {
        *dsl = result;
    } else {
        if (result != NULL) {
            free(result);
        }
    }

    result = NULL;
    return ret;
}

