#ifndef CALC_AST_H
#define CALC_AST_H

#include <stdlib.h>

#include "list.h"

//THIS HEADER FILE WAS TAKEN FROM LECTURE 5 CALCULATOR CODE WITH SOME MODIFICATIONS
typedef struct calc_ast {
    // Op is one of / // & && < > ;
    char* op;
    //arg stores left and right branches of trees
    struct calc_ast* arg0;
    struct calc_ast* arg1;
    // value is used on the leafs to store final command 
    list* value;
} calc_ast;

calc_ast* make_ast_value(list* toks);
calc_ast* make_ast_op(char* op, calc_ast* a0, calc_ast* a1);
void free_ast(calc_ast* ast);
int ast_eval(calc_ast* ast);

#endif
