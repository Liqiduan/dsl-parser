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

%type <value> values value name arg body 
%type <element> element

%%

start:%empty
     |element {result = $1;}
     ;

element:name arg body {$$ = DslElementNew($1, $2, $3);}
       |name arg {$$ = DslElementNew($1, $2, NULL);}
       ;

name:VAR
    ;

arg:LP RP {$$ = NULL;}
   |LP values RP {$$ = $2;}
   ;

body:LC RC {$$ = NULL;}
    |LC values RC {$$ = $2;}
    ;

values:value
      |value SEP values {$1->next = $3; $$ = $1;}
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

int DslParseInput()
{
    return yyparse();
}

