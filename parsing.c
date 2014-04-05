#include<stdio.h>
#include<stdlib.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

#include<string.h>

static char buffer[2048];

char* readline(char* prompt) {
    fputs("lispy> ", stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

void add_history(char* unused) {}

#else

#include<editline/readline.h>
#include<editline/history.h>

#endif
#include "mpc.h"

int main(int argc, char** argv) {
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");
    /* Define them with the following Language */
    mpca_lang(MPC_LANG_DEFAULT,
      "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Lispy);    
    /*mpca_lang(MPC_LANG_DEFAULT,
              "                                                    \
               number   : /-?[0-9]+/ ;                             \
               operator : '+' | '-' | '*' | '/' ;                  \
               expr     : <number> | '(' <operator> <expr>+ ')' ;  \
               lispy    : /^/ <operator> <expr>+ /$/ ;             \
              ",
              Number, Operator, Expr, Lispy);
    */
    puts("Lispy version 0.0.0.0.2");
    puts("Press Ctrl+C to exit");

    while(1) {
        char* input = readline("lispy> ");
        add_history(input);
        // Parse the user input
        mpc_result_t r;
        if (mpc_parse("stdin", input, Lispy, &r)) {
            // Success, print the AST
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            // Failure, print the error
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        free(input);
    }
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
    return 0;
}
