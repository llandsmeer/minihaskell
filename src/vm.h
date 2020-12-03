#pragma once

#define HEADER enum type type

enum type {
    FUN = 1234,
    CFUN,
    CLOSURE,
    CCLOSURE,
    INT,
    EVAL,
    LIST,
};

enum opcode_code {
    PUSHFREE, PUSHBOUND, PUSHLOCAL, PUSHCONST,
    POPFREE, POPBOUND, POPLOCAL,
    POP, CALL, RETURN, CLOSE, END,
    // for debugging:
    MKINT,
    PRINT,
    DUP,
    CALL0
};

struct opcode {
    enum opcode_code opcode;
    int arg;
};

struct box_any {
    HEADER;
};

struct box_list {
    HEADER;
    int len;
    int cap;
    struct box_any ** items;
};

typedef struct box_any * (*call_type)(struct box_list *);

struct box_fun {
    HEADER;
    struct box_list * consts;
    int nlocals;
    int nfree;
    int nbound;
    // needed during compilation:
    int nopcodes;
    char ** local_names;
    char ** bound_names;
    char ** free_names;
    struct opcode * opcodes;
};

struct box_closure {
    HEADER;
    struct box_fun * fun;
    struct box_list * free; /* null for functions without free variables */
    struct box_list * bound;
};

struct box_cfun {
    HEADER;
    call_type call;
    int narg;
};

struct box_cclosure {
    HEADER;
    struct box_cfun * cfun;
    struct box_list * bound;
};

struct box_int {
    HEADER;
    int val;
};

struct box_eval {
    HEADER;
    struct box_fun * code;
    struct box_list * locals;
    struct box_list * stack;
    struct box_list * bound;
    struct box_list * free;
    struct box_eval * prev;
    int ip;
};

void * box_alloc(enum type type);

struct box_list * mklist(int len);
void box_list_push(struct box_list *, struct box_any *);
struct box_any * box_list_pop(struct box_list *);
void box_list_set(struct box_list *, int, struct box_any *);
struct box_any * box_list_get(struct box_list *, int);
struct box_list * box_list_copy(struct box_list * orig);
int box_list_len(struct box_list *);

struct box_any * mkcfun(int narg, call_type call);
struct box_any * mkint(int i);
struct box_any * mkclosure(struct box_fun * f, struct box_list * free);

struct box_eval * mkeval(struct box_eval * prev, struct box_fun * code, struct box_list * bound, struct box_list * free);
struct box_eval * eval_next(struct box_eval * es);

void print_box(struct box_any * f);
