#include <stdio.h>
#include <stdlib.h>
#include "ytab.h"
#include <string.h>
#include <regex.h>

/* Provided by flex */
extern FILE *yyin;
extern char *yytext;
extern char *final_sval;
int yylex(void);
extern int rows, words, chars;
const char *forbidden_words[] = {"as", "as?", "class", "!in", "is", "!is", "object", "package", "super", "this", "throw", "try", "typealias", "typeof", "by", "catch", "constructor", "delegate", "dynamic", "field", "file", "finally", "get",
                                 "init", "param", "property", "reciever", "set", "setparam", "value", "where", "abstract", "actual", "annotation", "companion", "crossinline", "data", "enum", "except", "external", "final", "infix", "inline", "inner", "internal",
                                 "lateinit", "noinline", "open", "operator", "out", "private", "protected", "public", "reified", "sealed", "suspend", "tailrec", "vararg"};

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
    while (head)
    {
        struct token *t = head->t;
        printf("|--------------------------------------------------------------------------------------------------------------------------------|\n");
        printf(" %d Return Code, %s Actual Text, %d Line Number, %s Filename, %d Integer Value, %lf Double Value, %s String\n", t->category, t->text, t->lineno, t->filename, t->ival, t->dval, t->sval);
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

int determine_forbidden(char *text)
{
    for (int i = 0; i < (sizeof(forbidden_words) / sizeof(forbidden_words[0])); i++)
    {
        if (strcmp(text, forbidden_words[i]) == 0)
        {
            return FORBIDDEN;
        }
    }

    return 0;
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
    int forbidden_tester;
    int integer_value = 0;
    float double_value = 0;
    char *string_value = NULL;
    while ((return_code = yylex()) != 0)
    { // loops through each token until yylex returns 0, meaning EOF

        forbidden_tester = determine_forbidden(yytext); // checks if yytext is in forbidden_keyword global array
        if (forbidden_tester == 258)
        { // forbidden is defined as 258
            fprintf(stderr, "Lexical error: illegal character '%s' at line %d from file %s\n", yytext, rows, argv[1]);
            exit(1);
        }

        else
        {
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
    }

    fclose(f); // close the input file

    print_tokens(head); // print tokens starting from head
    free_tokens(head);  // free tokens starting from head

    return 0;
}
