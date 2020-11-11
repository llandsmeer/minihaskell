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

struct ast_node * mkstmtvar(struct ast_node * name, struct ast_node * expr);
struct ast_node * mklam(struct ast_node * args, struct ast_node * expr);
struct ast_node * mkapp(struct ast_node * f, struct ast_node * x);
struct ast_node * mkatomload(char * x);
struct ast_node * mkatomint(char * x);
struct ast_node * mkstmtfun(struct ast_node * name, struct ast_node * args, struct ast_node * expr);
struct ast_node * mkbinop(struct ast_node * op, struct ast_node * lhs, struct ast_node * rhs);
struct ast_node * mknamelist();
struct ast_node * addnamelist(struct ast_node * l, struct ast_node * x);
struct ast_node * mkstmtbinop(struct ast_node * op, struct ast_node * lhs, struct ast_node * rhs, struct ast_node * e);

extern struct ast_node * last;
void print_node(struct ast_node * node);
int yyparse();
