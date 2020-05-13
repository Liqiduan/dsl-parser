%{
#include "dsl.h"
#include "dsl_parser.y.h"
%}

YYSTYPE yylval;

%%

[a-zA-Z]+ {
    yylval.value = DslValueNewVar(yytext);
    return VAR;
    }

\".*\" {
    yylval.value = DslValueNewStr(yytext);
    return STR;
    }

(0x[0-9a-fA-F]+)|([0-9]+) {
    yylval.value = DslValueNewNumber(atoi(yytext));
    return NUMBER;
    }

"(" {return LP;}
")" {return RP;}
"{" {return LC;}
"}" {return RC;}

"," {return SEP;}
[ \t\s\n]* {return SPACE;}

%%
