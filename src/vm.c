#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "vm.h"

// TODO
//  - Resumable Exceptions
//  - efficiency -> pledge to release resource?

#define ERROR(x) {puts(x);exit(1);}

// ALLOC

static size_t box_size(enum type type) {
    switch (type) {
        case FUN: return sizeof(struct box_fun);
        case CLOSURE: return sizeof(struct box_closure);
        case CFUN: return sizeof(struct box_cfun);
        case CCLOSURE: return sizeof(struct box_cclosure);
        case INT: return sizeof(struct box_int);
        case EVAL: return sizeof(struct box_eval);
    }
    ERROR("box_size: unknown type");
}

void * box_alloc(enum type type) {
    struct box_any * box = malloc(box_size(type));
    box->type = type;
    return box;
}

struct box_any ** box_list_alloc(int i) {
    return calloc(i, sizeof(struct box_any*));
}

// MISC

static void print_box(struct box_any * f) {
    if (!f) {
        printf("NULL POINTER!!\n");
        return;
    }
    switch (f->type) {
        case FUN:
            printf("<Function %p>", f);
            break;
        case CLOSURE:
            printf("<Closure %p>", f);
            break;
        case CFUN:
            printf("<CFunction %p>", f);
            break;
        case CCLOSURE:
            printf("<CClosure %p>", f);
            break;
        case EVAL:
            printf("<Eval %p>", f);
            break;
        case INT:
            printf("<Int %d>", ((struct box_int*)f)->val);
            break;
    }
    printf("\n");
}

// CONSTRUCTORS

struct box_any *
mkcfun(int narg, call_type call) {
    struct box_cfun * cf = box_alloc(CFUN);
    cf->call = call;
    cf->narg = narg;
    return (struct box_any*)cf;
}

struct box_any *
mkint(int i) {
    struct box_int * x = box_alloc(INT);
    x->val = i;
    return (struct box_any*)x;
}

struct box_any *
mkclosure(struct box_fun * f, struct box_any ** free) {
    if (f->nfree == 0) {
        ERROR("can not capture 0 free variables");
    }
    struct box_closure * c = box_alloc(FUN);
    c->free = box_list_alloc(f->nfree);
    memcpy(c->free, free, f->nfree * sizeof(struct box_any*));
    c->bound = 0;
    c->fun = f;
    if (f->nbound > 1) {
        c->bound = box_list_alloc(f->nbound-1);
    } else {
        c->bound = 0;
    }
    return (struct box_any*)c;
}

struct box_eval *
mkeval(
        struct box_eval * prev,
        struct box_fun * code,
        struct box_any ** bound,
        struct box_any ** free) {
    struct box_eval * es = box_alloc(EVAL);
    es->prev = prev;
    es->code = code;
    es->ip = 0;
    es->sp = 0;
    es->bound = bound;
    es->free = free;
    es->locals = box_list_alloc(code->nlocals);
    es->stack = box_list_alloc(code->stacksize);
    return es;
};

// EVAL

static
struct box_eval *
eval_app(struct box_eval * current, struct box_any * f, struct box_any * x) {
    if (f->type == FUN) {
        struct box_fun * g = (struct box_fun *)f;
        if (g->nfree != 0) {
            ERROR("can not apply to function with free arguments");
            puts("b");
        }
        if (g->nbound < 1) {
            ERROR("fun nbound < 1???\n");
        } else if (g->nbound == 1) {
            struct box_any ** args = box_list_alloc(1);
            args[0] = x;
            return mkeval(current, g, args, 0);
        } else {
            struct box_closure * c = box_alloc(CLOSURE);
            c->fun = g;
            c->free = 0;
            c->nbound = 1;
            c->bound = box_list_alloc(g->nbound-1);
            c->bound[0] = x;
            current->stack[current->sp++] = (struct box_any*)c;
            return current;
        }
    } else if (f->type == CLOSURE) {
        struct box_closure * c = (struct box_closure*)f;
        if (c->fun->nfree > 0 && !c->free) {
            ERROR("Can not apply to closure with missing free capture");
        } else if (c->nbound + 1 == c->fun->nbound && c->nbound == 0) {
            struct box_any ** args = box_list_alloc(1);
            args[0] = x;
            return mkeval(current, c->fun, args, c->free);
        } else if (c->nbound + 1 == c->fun->nbound) {
            struct box_any ** args = box_list_alloc(c->fun->nbound);
            memcpy(args, c->bound, c->nbound*sizeof(struct box_any *));
            args[c->nbound] = x;
            struct box_eval * res = mkeval(current, c->fun, args, c->free);
            free(args);
            return res;
        } else {
            struct box_closure * c = (struct box_closure*)f;
            struct box_closure * k = box_alloc(CLOSURE);
            k->fun = c->fun;
            k->nbound = c->nbound + 1;
            k->free = c->free;
            k->bound = box_list_alloc(c->fun->nbound-1);
            memcpy(k->bound, c->bound, c->nbound*sizeof(struct box_any *));
            k->bound[c->nbound] = x;
            current->stack[current->sp++] = (struct box_any*)k;
            return current;
        }
    } else if (f->type == CFUN) {
        struct box_cfun * g = (struct box_cfun *)f;
        if (g->narg == 1) {
            struct box_any * res = g->call(&x);
            current->stack[current->sp++] = (struct box_any*)res;
            return current;
        } else {
            struct box_cclosure * c = box_alloc(CCLOSURE);
            c->cfun = g;
            c->nbound = 1;
            c->bound = box_list_alloc(g->narg-1);
            c->bound[0] = x;
            current->stack[current->sp++] = (struct box_any*)c;
            return current;
        }
    } else if (f->type == CCLOSURE) {
        struct box_cclosure * c = (struct box_cclosure*)f;
        if (c->nbound + 1 == c->cfun->narg) {
            struct box_any ** args = box_list_alloc(c->cfun->narg);
            memcpy(args, c->bound, c->nbound*sizeof(struct box_any *));
            args[c->nbound] = x;
            struct box_any * res = c->cfun->call(args);
            free(args);
            current->stack[current->sp++] = (struct box_any*)res;
            return current;
        } else {
            struct box_cclosure * c = (struct box_cclosure*)f;
            struct box_cclosure * k = box_alloc(CCLOSURE);
            k->cfun = c->cfun;
            k->nbound = c->nbound + 1;
            k->bound = box_list_alloc(c->cfun->narg-1);
            memcpy(k->bound, c->bound, c->nbound*sizeof(struct box_any *));
            k->bound[c->nbound] = x;
            current->stack[current->sp++] = (struct box_any*)k;
            return current;
        }
    } else {
        ERROR("eval_app(f, x), x != FUN or CLOSURE\n");
        exit(1);
    }
}

struct box_eval *
eval_next(struct box_eval * es) {
    struct box_any * f, * x;
    int arg = es->code->opcodes[es->ip].arg;
    switch (es->code->opcodes[es->ip++].opcode) {
        case PUSHFREE:
            es->stack[es->sp++] = es->free[arg];
            break;
        case PUSHBOUND:
            es->stack[es->sp++] = es->bound[arg];
            break;
        case PUSHCONST:
            es->stack[es->sp++] = es->code->consts[arg];
            break;
        case PUSHLOCAL:
            es->stack[es->sp++] = es->locals[arg];
            break;
        case POPLOCAL:
             es->locals[arg] = es->stack[--es->sp];
            break;
        case POP:
            es->sp--;
            break;
        case CALL:
            f = es->stack[--es->sp];
            x = es->stack[--es->sp];
            return eval_app(es, f, x);
        case CLOSE:
            // So - how do we handle exceptions here? Resumable Exceptions?
            f = es->stack[--es->sp];
            es->sp -= ((struct box_fun*)f)->nfree;
            x = mkclosure((struct box_fun*)f, &es->stack[es->sp]);
            es->stack[es->sp++] = x;
            break;
        case RETURN:
            x = es->stack[--es->sp];
            es->prev->stack[es->prev->sp++] = x;
            return es->prev;
        case END:
            return es->prev;
        case MKINT:
            es->stack[es->sp++] = mkint(arg);
            break;
        case PRINT:
            x = es->stack[--es->sp];
            print_box(x);
            break;

    }
    return es;
}