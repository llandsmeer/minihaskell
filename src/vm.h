#pragma once

#define HEADER enum type type

enum type {
    FUN = 1234,
    CFUN,
    CLOSURE,
    CCLOSURE,
    INT,
    EVAL,
};

enum opcode_code {
    PUSHFREE, PUSHBOUND, PUSHLOCAL, PUSHCONST,
    POPLOCAL,
    POP, CALL, RETURN, CLOSE, END,
    // for debugging:
    MKINT,
    PRINT,
    DUP
};

struct opcode {
    enum opcode_code opcode;
    int arg;
};

struct box_any {
    HEADER;
};

typedef struct box_any * (*call_type)(struct box_any **);

struct box_fun {
    HEADER;
    struct box_any ** consts;
    struct opcode * opcodes;
    int stacksize;
    int nlocals;
    int nfree;
    int nbound;
    // needed during compilation:
    int nconsts;
    int nopcodes;
    char ** local_names;
    char ** bound_names;
    char ** free_names;
};

struct box_closure {
    HEADER;
    struct box_fun * fun;
    struct box_any ** free; /* null for functions without free variables */
    struct box_any ** bound;
    int nbound;
};

struct box_cfun {
    HEADER;
    call_type call;
    int narg;
};

struct box_cclosure {
    HEADER;
    struct box_cfun * cfun;
    struct box_any ** bound;
    int nbound;
};

struct box_int {
    HEADER;
    int val;
};

struct box_eval {
    HEADER;
    struct box_fun * code;
    struct box_any ** locals;
    struct box_any ** stack;
    struct box_any ** bound;
    struct box_any ** free;
    struct box_eval * prev;
    int ip;
    int sp;
};


struct box_any ** box_list_alloc(int i);
void * box_alloc(enum type type);

struct box_any * mkcfun(int narg, call_type call);
struct box_any * mkint(int i);
struct box_any * mkclosure(struct box_fun * f, struct box_any ** free);

struct box_eval * mkeval(struct box_eval * prev, struct box_fun * code, struct box_any ** bound, struct box_any ** free);
struct box_eval * eval_next(struct box_eval * es);

void print_box(struct box_any * f);
