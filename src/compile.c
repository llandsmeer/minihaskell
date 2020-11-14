#include <assert.h>
#include "compile.h"

#define ERROR(x) {puts(x);exit(1);}
#define NOARG 0

static // now I'm an official three-star programmer
void append(void *** l, int * n, void * x) {
    *n += 1;
    *l = realloc(*l, *n * sizeof(void*));
    assert(*l);
    (*l)[*n-1] = x;
}
#define APPEND(l, n, x) append((void***)&(l), &(n), (x))

static
void addop(struct box_fun * f, enum opcode_code code, int arg) {
    assert(f);
    assert(f->type == FUN);
    assert(code >= 0 && code <= 255);
    assert(arg >= 0 && arg <= 255);
    f->nopcodes += 1;
    f->opcodes = realloc(f->opcodes, f->nopcodes*sizeof(struct opcode));
    assert(f->opcodes);
    f->opcodes[f->nopcodes-1].opcode = code;
    f->opcodes[f->nopcodes-1].arg = arg;
}

static
int lookup_name(char ** names, int n, const char * name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(names[i], name) == 0) {
            return i;
            break;
        }
    }
    return -1;
}

static
void compile_load(struct box_fun * f, char * name) {
    int idx;
    idx = lookup_name(f->bound_names, f->nbound, name);
    if (idx >= 0) {
        addop(f, PUSHBOUND, idx);
        return;
    }
    idx = lookup_name(f->local_names, f->nlocals, name);
    if (idx >= 0) {
        addop(f, PUSHLOCAL, idx);
        return;
    }
    if (idx == -1) {
        idx = lookup_name(f->free_names, f->nfree, name);
        if (idx == -1) {
            idx = f->nfree;
            APPEND(f->free_names, f->nfree, name);
        }
        addop(f, PUSHFREE, idx);
        return;
    }
}

static
void compile_store(struct box_fun * f, char * name) {
    addop(f, DUP, 0);
    int idx;
    idx = lookup_name(f->bound_names, f->nbound, name);
    if (idx >= 0) {
        addop(f, POPBOUND, idx);
        return;
    }
    idx = lookup_name(f->free_names, f->nfree, name);
    if (idx >= 0) {
        addop(f, POPFREE, idx);
        return;
    }
    idx = lookup_name(f->local_names, f->nlocals, name);
    if (idx == -1) {
        idx = f->nlocals;
        APPEND(f->local_names, f->nlocals, name);
    }
    addop(f, POPLOCAL, idx);
}

static
void compile(struct box_fun * f, struct ast_node * n) {
    struct box_fun * g;
    switch (n->type) {
        case STMTASSIGN:
            compile(f, n->x.stmtassign.expr);
            compile_store(f, n->x.stmtassign.name);
            break;
        case LAM:
            g = mkfun(n);
            APPEND(f->consts, f->nconsts, g);
            if (g->nfree > 0) {
                for (int i = 0; i < g->nfree; i++) {
                    compile_load(f, g->free_names[i]);
                }
            }
            addop(f, PUSHCONST, f->nconsts-1);
            if (g->nfree > 0) {
                addop(f, CLOSE, NOARG);
            }
            break;
        case APP:
            compile(f, n->x.app.x);
            compile(f, n->x.app.f);
            addop(f, CALL, NOARG);
            break;
        case ATOMLOAD:
            compile_load(f, n->x.atomload.name);
            break;
        case ATOMINT:
            APPEND(f->consts, f->nconsts, mkint(atoi(n->x.atomint.value)));
            addop(f, PUSHCONST, f->nconsts-1);
            break;
        case NAMELIST:
            ERROR("NAMELIST SHOULD NOT BE IN FINAL AST");
            break;
    }
}

struct box_fun * mkfun(struct ast_node * f) {
    struct box_fun * box = box_alloc(FUN);
    box->consts = 0;
    box->opcodes = 0;
    box->stacksize = 0;
    box->local_names = 0;
    box->nlocals = 0;
    box->nopcodes = 0;
    box->nconsts = 0;
    box->nfree = 0;
    box->stacksize = 1024;
    box->free_names = 0;

    if (!f) {
        box->nbound = 0;
        box->bound_names = 0;
    } else if (f->type == LAM) {
        box->nbound = f->x.lam.args->x.namelist.nnames;
        box->bound_names = f->x.lam.args->x.namelist.names;
        compile(box, f->x.lam.expr);
        addop(box, RETURN, 0);
    } else {
        box->nbound = 0;
        box->bound_names = 0;
        compile(box, f);
    }

    addop(box, END, 0);
    return box;
}

void box_append_code(struct box_fun * f, struct ast_node * x) {
    assert(x->type != LAM);
    assert(f->nopcodes <= 1 || f->opcodes[f->nopcodes-2].opcode != RETURN);
    assert(f->nopcodes == 0 || f->opcodes[f->nopcodes-1].opcode == END);
    if (f->nopcodes > 0) {
        f->nopcodes -= 1;
    }
    //f->opcodes = realloc(f->opcodes, f->nopcodes*sizeof(struct opcode));
    compile(f, x);
    addop(f, END, 0);
}
