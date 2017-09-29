/*
 * an example grammar for ANTLR v3
 */

grammar Parser;
options {
	language = C;
}

tokens {
	PLUS = '+';
	MINUS = '-';
	MULT = '*';
	DIV = '/';
	ASSIGN = '=';
}

@header {
	#include <string.h>
	#define NUM ('z'-'a')
	#define MOD	'a'
}

@members {
	long arr[NUM] = {0};
}

translationUnit: calc+ ;

calc
    : v = expr NEWLINE { printf("\%ld\n", $v.value); }
	| a = ID ASSIGN b = expr NEWLINE
		{
			if (strlen($a.text->chars) > 1)
				fprintf(stderr, "Only one byte long variables!\n");
			int index = (*$a.text->chars | 0x20) \% MOD;
			arr[index] = $b.value;
		}
	| NEWLINE
	;

expr returns [long value]
	: a = addmin {$value = $a.value;}
	;

addmin returns [long value]
	: a = muldiv { $value = $a.value; }
	  ( PLUS b = muldiv { $value += $b.value; }
	  | MINUS b = muldiv { $value -= $b.value; }
	  )*
	;

muldiv returns [long value]
	: a = num { $value = $a.value; }
	  ( MULT b = num { $value *= $b.value; }
	  | DIV b = num { $value /= $b.value; }
	  )*
	;

num returns [long value]
	: a = NUMBER { $value = atol($a.text->chars); }
	| b = ID {
				if (strlen($b.text->chars) > 1)
					fprintf(stderr, "Only one byte long variables!\n");
				int index = (*$b.text->chars | 0x20) \% MOD;
				$value = arr[index];
			 }
	;

ID:		('a'..'z'|'A'..'Z')+ ;

NUMBER: ('0'..'9')+ ;

NEWLINE: '\r'? '\n' ;

WHITESPACE: (' '|'\r'|'\t'|'\u000C') {$channel=HIDDEN;} ;
