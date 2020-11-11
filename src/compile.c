#include <assert.h>
#include "compile.h"

// TODO:
//  - assignments & local vars
//  - bound vs free
//  - global execution environment
//  - simple repl step function (rep?)
//

#define ERROR(x) {puts(x);exit(1);}
#define NOARG 0

static // now I'm an official three-star programmer
void append(void *** l, int * n, void * x) {
    *n += 1;
    *l = realloc(*l, *n * sizeof(void*));
    assert(*l);
    *l[*n-1] = x;
}
#define APPEND(l, n, x) append((void***)&(l), &(n), (x))

static
void addop(struct box_fun * f, enum opcode_code code, int arg) {
    f->nopcodes += 1;
    f->opcodes = realloc(f->opcodes, f->nopcodes*sizeof(struct opcode));
    f->opcodes[f->nopcodes-1].opcode = code;
    f->opcodes[f->nopcodes-1].arg = arg;
}

static
void compile(struct box_fun * f, struct ast_node * n) {
    int idx;
    struct box_fun * g;
    switch (n->type) {
        case STMTASSIGN:
            ERROR("Assignments not supported");
            break;
        case LAM:
            g = mkfun(n);
            APPEND(f->consts, f->nconsts, g);
            addop(f, PUSHCONST, f->nconsts-1);
            break;
        case APP:
            compile(f, n->x.app.x);
            compile(f, n->x.app.f);
            addop(f, CALL, NOARG);
            break;
        case ATOMLOAD:
            idx = -1;
            for (int i = 0; i < f->nbound; i++) {
                if (strcmp(n->x.atomload.name, f->bound_names[i]) == 0) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) {
                ERROR("NON BOUND VARIABLES NOT SUPPORTED");
            }
            addop(f, PUSHBOUND, idx);
            break;
        case ATOMINT:
            APPEND(f->consts, f->nconsts, mkint(atoi(n->x.atomint.value)));
            addop(f, PUSHCONST, f->nconsts);
            break;
        case NAMELIST:
            ERROR("NAMELIST SHOULD NOT BE IN FINAL AST");
            break;
    }
}

struct box_fun * mkfun(struct ast_node * f) {
    assert(f->type == LAM);
    struct box_fun * box = box_alloc(FUN);
    box->consts = 0;
    box->opcodes = 0;
    box->stacksize = 0;
    box->nlocals = 0;
    box->nconsts = 0;
    box->nfree = 0;
    box->nbound = f->x.lam.args->x.namelist.nnames;
    box->bound_names = f->x.lam.args->x.namelist.names;
    compile(box, f->x.lam.expr);
    addop(box, RETURN, 0);
    addop(box, END, 0);
    return box;
}
