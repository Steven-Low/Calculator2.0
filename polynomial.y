%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "header.h"
void yyerror(char *);
int yylex(void);


%}

%union {
	float num;
	char op;
	char x;
	char* tri;
	struct node *n;
}

%token <x> VARIABLE
%token <num> NUMBER
%token <tri> TRIG
%token <op> ADD SUB MUL DIV EXP EQU SEMI
%token LP RP
%token PRINT ADDITION SIMPLIFY DERIVATIVE INTEGRATE SOLVE COMPUTE

%type <n> stmt expr term efactor factor equation equations

%%
program: 
		program stmt '\n'         { printf("\n"); } /* to cycle the program */  
		| program '\n'	 		  {  }      
		| 						  /* epsilon here is very very important !!! */			
		;
		
stmt    : equations function	  {  }
		| equation function		  {  }
		| equation
		;

function: PRINT					  { display(final); }   // expanded equation
		| DERIVATIVE 			  { derivative(final); simplify(final); display(final); }
		| INTEGRATE 			  { integrate(final); simplify(final); display(final); }
		| ADDITION				  { simplify(final); display(final); }
		| SIMPLIFY				  { simplify(final); display(final); }
		| SOLVE					  { simplify(final); solve(final); }
		| COMPUTE NUMBER 		  { simplify(final); printf("Answer: %.5f\n",computeNode(final,$2)); }
		| COMPUTE 				  { simplify(final); printf("Answer: %.5f\n",computeNode(final,0)); } // default is 0
		;
		
equations: equation SEMI equation { $$ = add($1, $3);     final = $$; }
		;
		
equation: equation EQU expr  	  { $$ = subtract($1,$3); final = $$; } 
		| expr					  { $$ = $1; 			  final = $1; }
		;

expr	: expr ADD term 		  { $$ = add($1,$3); }
		| expr term				  { $$ = add($1,$2); }  // default is addition
		| expr SUB term  		  { $$ = subtract($1,$3); }
		| SUB term  			  { $$ = subtract(NULL,$2); }
		| term					  { $$ = $1; }
		;

term 	: term MUL efactor	  	  { $$ = multiply($1,$3); }
		| term DIV efactor		  { $$ = divide($1,$3); }
		| efactor				  { $$ = $1; }
		;

efactor	: efactor EXP NUMBER  	  { $$ = setExponent($1,$3); }
		| efactor EXP SUB NUMBER  { $$ = setExponent($1,$4 * -1); }
		| factor				  { $$ = $1; }
		;

factor	: NUMBER				  { $$ = createNode($1, '\0', 0, NULL, NULL); }
		| VARIABLE   			  { $$ = createNode(1, $1, 1, NULL, NULL); } 
		| LP expr RP			  { $$ = $2; }
		| TRIG LP expr RP 	  	  { $$ = createTrigNode($1, $3); }
		;
%%

int main(){
	printf("How to use? \n \
functions:  \n \
	1)print		\n \
	2)addition	\n \
	3)simplify  \n \
	4)integrate	\n \
	5)derivative\n \
	6)solve 	\n \
	7)compute   \n \
prompt: equation {space} function\n\n ");
			
	printf("%s","Enter prompt below: \n");
	yyparse();
	return 0;
}

void yyerror(char *s){
	fprintf(stderr,"%s\n",s);
}