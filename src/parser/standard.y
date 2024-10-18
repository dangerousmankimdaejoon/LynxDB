%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
%}

%token SELECT FROM WHERE
%token COLUMN OBJECT VARIABLE

%%
query:
    SELECT column_list FROM OBJECT WHERE condition {
        printf("Query executed: SELECT %s FROM %s WHERE %s\n", $2, $4, $6);
    }
    ;

column_list:
    COLUMN {
        $$ = $1;
    }
    | COLUMN ',' column_list {
        $$ = $1;
    }
    ;

condition:
    COLUMN '=' VARIABLE {
        $$ = $1;
    }
    ;
%%

void yyerror(const char *s) {
    lx_log("ERROR", strerror(errno), "parse error");
}