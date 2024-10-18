%{
#include "y.tab.h"
%}

%%

[ \t\n]+       ;
"SELECT"      { return SELECT; }
"FROM"        { return FROM; }
"WHERE"       { return WHERE; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval = strdup(yytext); return IDENTIFIER; }
[0-9]+        { yylval = atoi(yytext); return NUMBER; }
"="           { return '='; }

.             { return yytext[0]; }

%%

int yywrap() {
    return 1;
}