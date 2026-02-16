#include <stdio.h>
#include <stdlib.h>
#include "ytab.h"
#include <string.h>
#include <regex.h>

/* Provided by flex */
extern FILE *yyin;
extern char *yytext;
extern char *final_sval;
extern char *final_raw;
int yylex(void);
extern int rows, words, chars;

/* token list */
struct token
{
    int category;   /* the integer code returned by yylex */
    char *text;     /* the actual string (lexeme) matched */
    int lineno;     /* the line number on which the token occurs */
    char *filename; /* the source file in which the token occurs */
    int ival;       /* for integer constants, store binary value here */
    double dval;    /* for real constants, store binary value here */
    char *sval;     /* for string constants, malloc space, de-escape, store the string (less quotes and after escapes) here */
};

struct tokenList
{
    struct token *t;        /* token holder */
    struct tokenList *next; /* pointer to next token */
};

/* populate token with data */
struct token *create_token(int code, char *string, int lineno, char *file, int integer_value, float double_value, char *string_value)
{
    struct token *tok = malloc(sizeof(struct token));
    if (!tok)
        return NULL;

    tok->category = code;
    tok->text = strdup(string); // copy yytext
    tok->lineno = lineno;
    tok->filename = file;
    tok->ival = integer_value;
    tok->dval = double_value;
    tok->sval = string_value;

    return tok;
}

void free_tokens(struct tokenList *head)
{
    while (head)
    {
        struct tokenList *next = head->next;

        free(head->t->text);
        free(head->t->sval);
        free(head->t);
        free(head);

        head = next;
    }
}

void print_tokens(struct tokenList *head)
{
    printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    while (head)
    {
        struct token *t = head->t;
        printf(" %d Return Code,    %s Actual Text,     %d Line Number,     %s Filename,    %d Integer Value,   %lf Double Value,   %s String\n", t->category, t->text, t->lineno, t->filename, t->ival, t->dval, t->sval);
        head = head->next;
    }
}

struct tokenList *create_node(struct token *tok)
{
    struct tokenList *node = malloc(sizeof(struct tokenList));
    if (!node)
        return NULL;

    node->t = tok;
    node->next = NULL;
    return node;
}

void append_token(struct tokenList **head, struct tokenList **tail, struct token *tok)
{
    struct tokenList *node = create_node(tok);
    if (!node)
        return;

    if (*head == NULL)
    {
        *head = node;
        *tail = node;
    }
    else
    {
        (*tail)->next = node;
        *tail = node;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror("fopen");
        return 1;
    }

    yyin = f;

    struct tokenList *head = NULL;
    struct tokenList *tail = NULL;

    int return_code;
    int integer_value = 0;
    float double_value = 0;
    char *string_value = NULL;
    while ((return_code = yylex()) != 0)
    { // loops through each token until yylex returns 0, meaning EOF
        /* handles literal translation to their respective values */
        switch (return_code)
        {
        case INT_LITERAL:
            integer_value = strtol(yytext, NULL, 0); // change from string to integer
            break;
        case FLOAT_LITERAL:
            double_value = strtof(yytext, NULL); // change from double to integer
            break;
        case STRING_LITERAL:
            string_value = final_sval; // change string_value to accumulated sval from buffer
            yytext = final_raw;
            break;
        case FORBIDDEN:
            fprintf(stderr, "Lexical error: illegal character '%s' at line %d from file %s\n", yytext, rows, argv[1]);
            exit(1);
            break;
        case NONIDENTIFIER:
            fprintf(stderr, "Lexical error: Filename %s, line #%d, text: '%s', identifiers cannot begin with a number\n", argv[1], rows, yytext);
            exit(1);
            break;
        default:
            break;
        }
        /* malloc a token with all data needed */
        struct token *tok = create_token(return_code, yytext, rows, argv[1], integer_value, double_value, string_value);
        if (!tok)
        {
            fprintf(stderr, "Token allocation failed\n");
            break;
        }

        append_token(&head, &tail, tok); // add token to linked list

        /* reset these values for next token */
        integer_value = 0;
        double_value = 0;
        string_value = NULL;
    }

    fclose(f); // close the input file

    print_tokens(head); // print tokens starting from head

    final_sval = NULL; // cleanup sval
    final_raw = NULL; // cleanup raw
    free_tokens(head);  // free tokens starting from head

    return 0;
}
