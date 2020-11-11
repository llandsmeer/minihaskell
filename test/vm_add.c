#include <stdio.h>
#include "../src/vm.h"

struct box_any * add(struct box_any ** args) {
    int i = (((struct box_int*)args[0])->val +
             ((struct box_int*)args[1])->val);
    return mkint(i);
}

int main() {
    struct box_any * cadd = mkcfun(2, add);

    // g = \x -> print(x + 1) ; return x + 1
    struct opcode gopcodes[] = {
        { PUSHBOUND, 0},
        { MKINT, 1 },
        { PUSHCONST, 0},
        { CALL, 0 },
        { CALL, 0 },
        { DUP, 0 },
        { PRINT, 0 },
        { RETURN, 0 },
        { END, 0 }
    };
    struct box_fun * g = box_alloc(FUN);
    g->consts = box_list_alloc(1);
    g->consts[0] = cadd;
    g->opcodes = &gopcodes[0];
    g->stacksize = 0;
    g->nlocals = 0;
    g->nfree = 0;
    g->nbound = 1;

    // f = g(g(20))
    struct opcode fopcodes[] = {
        { MKINT, 20 },
        { PUSHCONST, 0 },
        { CALL, 0 },
        { END, 0 }
    };
    struct box_fun * f = box_alloc(FUN);
    f->consts = box_list_alloc(1);
    f->consts[0] = (struct box_any*)g;
    f->opcodes = &fopcodes[0];
    f->stacksize = 1000;
    f->nlocals = 0;
    f->nfree = 0;
    f->nbound = 0;

    struct box_eval * e0, * e = mkeval(0, f, 0, 0);
    e0 = e;

    while ((e = eval_next(e))) {
    };

    if (((struct box_int*)e0->stack[0])->val == 21) {
        puts("OK");
    } else {
        puts("ERR");
    }
}
