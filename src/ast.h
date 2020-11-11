#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast_node {
    enum {
        STMTASSIGN,
        LAM,
        APP,
        ATOMLOAD,
        ATOMINT,
        NAMELIST
    } type;
    union {
        struct {
            char * name;
            struct ast_node * expr;
        } stmtassign;
        struct {
            char * name;
            struct ast_node * args;
            struct ast_node * expr;
        } lam;
        struct {
            struct ast_node * f;
            struct ast_node * x;
        } app;
        struct {
            char * name;
        } atomload;
        struct {
            char * value;
        } atomint;
        struct {
            int nnames;
            char ** names;
        } namelist;
    } x;
};

extern struct ast_node * last;
void print_node(struct ast_node * node);
int yyparse();
