%{
#include <stdlib.h>
#include <string.h>
#include "polynomial.tab.h"
void yyerror(char *);
%}

%option case-insensitive noyywrap

%%



 /* SINGLE VARIABLE */
[a-zA-Z]	{ yylval.x = yytext[0]; return VARIABLE; }

 /* NUMBERS */
[0-9]*"."?[0-9]*	{ yylval.num = atof(yytext); return NUMBER; }
"pi"				{ yylval.num = 3.14159; return NUMBER; }

 /* TRIGONOMETRICS */
"sin"			{ yylval.tri = strdup(yytext); return TRIG; }
"cos"			{ yylval.tri = strdup(yytext); return TRIG; }
"tan"			{ yylval.tri = strdup(yytext); return TRIG; }

 /* OPERATORS */
"+"				{ yylval.op = yytext[0]; return ADD; }
"-"				{ yylval.op = yytext[0]; return SUB; }
"*"				{ yylval.op = yytext[0]; return MUL; }
"/"				{ yylval.op = yytext[0]; return DIV; }
"^"				{ yylval.op = yytext[0]; return EXP; }
"="				{ yylval.op = yytext[0]; return EQU; }
";"				{ yylval.op = yytext[0]; return SEMI; } 
"("					return LP;
")"					return RP;

 /* FUNCTIONS */
"PRINT"			{ return PRINT; }
"DERIVATIVE" 	{ return DERIVATIVE; }
"ADDITION"		{ return ADDITION; }
"SIMPLIFY"		{ return SIMPLIFY; }
"INTEGRATE"		{ return INTEGRATE; }
"SOLVE"			{ return SOLVE; }
"COMPUTE"		{ return COMPUTE; }

 /* MISCELLANEOUS */
[\n]		{ return yytext[0]; }
[ \t]		{ }
.			{ yyerror("invalid characters haha"); }

%%

/*
int yywrap(){
	return 1;
}
*/

