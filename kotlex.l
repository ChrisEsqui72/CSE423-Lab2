 /* Lexical analyzer file for k0 sublanguage of Kotlin */
%option noyywrap
%option noinput
%option nounput
%{
	#include <stdio.h>
	#include <string.h>
        #include <stdlib.h>
        #include "ytab.h"
	extern FILE *yyin;
	int rows = 1, words = 0, chars = 0, start_row = 0;
        char *strbuf = NULL;
        char *final_sval = NULL;
        int str_len;
        int str_cap;
%}

whitespace [ \t\r]+
basic_types "byte"|"short"|"int"|"long"|"float"|"double"|"string"
shebang (#\!).*
%x COMMENT
%x STRING
%x MSTRING

%%

\n                      { rows++; }             /* increment rows */
{whitespace}            ;                       /* disregard whitespace */
"//".*                  ;                       /* disregard single-line comments */
{shebang}               ;                       /* disregard shebang line */

 /* COMMENT state for handling single and multi-line comments */ 
"/*"            { BEGIN(COMMENT); start_row = rows; }             /* go into COMMENT state*/
<COMMENT>"*/"   { BEGIN(INITIAL); }             /* return to INITIAL state*/
<COMMENT>(.)    ;                               /* do nothing with characters inside comment */
<COMMENT>(\n)   { rows++; }                     /* increment rows when new-line seen */
<COMMENT><<EOF>> { printf("Unterminated comment at line %d\n", start_row); exit(1); }

 /* all allowed keywords (hard, soft, modifier) */
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"do"            { return DO; }
"else"          { return ELSE; }
"false"         { return FALSE; }
"for"           { return FOR; }
"fun"           { return FUN; }
"if"            { return IF; }
"in"            { return IN; }
"null"          { return NULL_; }               /* NULL is reserved in C, using NULL_ instead */
"return"        { return RETURN; }
"true"          { return TRUE; }
"val"           { return VAL; }
"var"           { return VAR; }
"when"          { return WHEN; }
"while"         { return WHILE; }
"import"        { return IMPORT; }
"const"         { return CONST; }

 /* all allowed operators and punctuation */
"==="           { return REFER_EQUAL; }         /* referential equality */
"!=="           { return REFER_NOT_EQUAL; }     /* referential inequality */
"=="            { return EQUAL; }
"!="            { return NOT_EQUAL; }
">="            { return GREATER_EQUAL; }
"<="            { return LESS_EQUAL; }
"+="            { return PLUS_EQUAL; }
"-="            { return MINUS_EQUAL; }
"++"            { return INC; }                 /* unary increment, int only, postfix */
"--"            { return DEC; }                 /* unary decrement, int only, postfix */
"&&"            { return AND; }
"||"            { return OR; }
"!!"            { return ASSERT_NON_NULL; }     /* unary, asserts non-nullable */
"?."            { return SAFE_CALL; }           /* method call / field if non-nullable */
"?:"            { return ELVIS; }               /* produce right side if left is null */
"..<"           { return RANGE_EXCL; }          /* exclusive range */
".."            { return RANGE_INCL; }          /* inclusive range */
"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"%"             { return MOD; }
">"             { return GREATER; }
"<"             { return LESS; }
"!"             { return NOT; }
"?"             { return NULLABLE; }            /* unary suffix produces nullable type */
"\."            { return DOT; }
"["[0-9]+"]"    { return ARRAY_ACCESS; }
"("{basic_types}")"     { return TYPE_CAST; }
"{"             { return RIGHT_CURL; }
"}"             { return LEFT_CURL; }
{basic_types}   { return TYPE; }
"("             { return LPARA; }
")"             { return RPARA; }

 /* not implementing type casts yet as I literally have no idea how to deal with that */

 /* literals */
 /* float literal */
 ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([Ff])? {
        chars += strlen(yytext);
        return FLOAT_LITERAL;
 }

 /* decimal integer literal */
[0-9]+ {
        chars += strlen(yytext);
        return INT_LITERAL;
 }

 /* hex integer literal */
0[xX][0-9a-fA-F]+ {
        chars += strlen(yytext);
        return INT_LITERAL;
}

 /* character literal */
\'(\\[ntr0b\'\"\\\$]|[^\'\\])\'  { 
        chars += strlen(yytext); 
        return CHAR_LITERAL;
}

 /* MSTRING state to handle stripping/consuming of string and putting into sval */
\"\"\"                  { BEGIN(MSTRING); start_row = rows; str_len = 0; str_cap = 64; strbuf = malloc(str_cap); if (strbuf==NULL){ fprintf(stderr, "malloc failed\n"); exit(1); } strbuf[0] = '\0'; }
<MSTRING>\"\"\"         { BEGIN(INITIAL); final_sval = strbuf; strbuf = NULL; str_len = 0; str_cap = 64; return STRING_LITERAL; }
<MSTRING>\\n            { append_string("\n", 1); }
<MSTRING>\\t            { append_string("\t", 1); }
<MSTRING>\\r            { append_string("\r", 1); }
<MSTRING>\\\"           { append_string("\"", 1); }
<MSTRING>\\\\           { append_string("\\", 1); }
<MSTRING>[^\\\"]+       { append_string(yytext, yyleng); }
<MSTRING>\\(.)          { fprintf(stderr, "Lexical error: MLL invalid escape sequence \\%c at row %d\n", yytext[1], rows); exit(1); }
<MSTRING><<EOF>>        { fprintf(stderr, "Lexical error: MLL EOF unterminated string starting at row %d\n", start_row); exit(1); }

 /* STRING state to handle stripping/consuming of string and putting into sval */
\"              { BEGIN(STRING); start_row = rows; str_len = 0; str_cap = 64; strbuf = malloc(str_cap); if (strbuf==NULL){ fprintf(stderr, "malloc failed\n"); exit(1); } strbuf[0] = '\0'; }
<STRING>\"      { BEGIN(INITIAL); final_sval = strbuf; strbuf = NULL; str_len = 0; str_cap = 64; return STRING_LITERAL; }
<STRING>\\n     { append_string("\n", 1); }
<STRING>\\t     { append_string("\t", 1); }
<STRING>\\r     { append_string("\r", 1); }
<STRING>\\\"    { append_string("\"", 1); }
<STRING>\\\\    { append_string("\\", 1); }
<STRING>[^\\\"\n]+ { append_string(yytext, yyleng); }
<STRING>\\(.)   { fprintf(stderr, "Lexical error: SLL invalid escape sequence \\%c at row %d\n", yytext[1], rows); exit(1); }
<STRING>\n      { fprintf(stderr, "Lexical error: SLL unterminated string starting at row %d\n", start_row); exit(1); }
<STRING><<EOF>> { fprintf(stderr, "Lexical error: EOF unterminated string starting at row %d\n", start_row); exit(1); }

 /* identifiers */
[a-zA-Z_][a-zA-Z0-9_]*  { words++; chars += strlen(yytext); return IDENTIFIER; } /* catch for identifiers (var names, fun names, etc. ) */

 /* all non-allowed keywords (hard, soft, modifier), operators, punctuation */
. {
        fprintf(stderr, "Lexical error: illegal character '%s' at row %d\n", yytext, rows);
        exit(1);
}

<<EOF>> {
        printf("%d rows %d words %d chars \n", rows, words, chars);
	return 0;
}
%%

void append_string(const char *text, int length){
        int needed;

        if(length <= 0) /* means an empty append, do nothing */
                return;

        needed = str_len + length + 1; /* compute how much space needed to append to str_buf */

        if (needed > str_cap){ /* if needed is larger than the cap set */
                int new_cap = str_cap;

                if (new_cap == 0) /* sanity check */
                        new_cap = 64;

                while (needed > new_cap) /* just double until new_cap is larger than needed */
                        new_cap *= 2;
                
                strbuf = realloc(strbuf, new_cap); /* change the size of strbuf to match new_cap */
                if(strbuf == NULL){
                        fprintf(stderr, "Error: realloc failed while building string literal\n");
                        exit(1);
                }
                str_cap = new_cap;
        }

        memcpy(strbuf + str_len, text, length); /* copy text into the end of the buffer */
        str_len += length; /* update length */
        strbuf[str_len] = '\0'; /* keep it null terminated */
}