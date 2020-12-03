#include <stdio.h>
#include "../src/vm.h"

struct box_any * add(struct box_list * args) {
    int i = (((struct box_int*)box_list_get(args, 0))->val +
             ((struct box_int*)box_list_get(args, 1))->val);
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
    g->consts = mklist(1);
    box_list_set(g->consts, 0, cadd);
    g->opcodes = &gopcodes[0];
    g->nlocals = 0;
    g->nfree = 0;
    g->nbound = 1;
    g->nopcodes = sizeof(gopcodes) / sizeof(gopcodes[0]);

    // f = g(g(20))
    struct opcode fopcodes[] = {
        { MKINT, 20 },
        { PUSHCONST, 0 },
        { CALL, 0 },
        { END, 0 }
    };
    struct box_fun * f = box_alloc(FUN);
    f->consts = mklist(1);
    box_list_set(f->consts, 0, (struct box_any*)g);
    f->opcodes = &fopcodes[0];
    f->nlocals = 0;
    f->nfree = 0;
    f->nbound = 0;
    f->nopcodes = sizeof(fopcodes) / sizeof(fopcodes[0]);

    struct box_eval * e0, * e = mkeval(0, f, 0, 0);
    e0 = e;

    while ((e = eval_next(e))) {
    };

    if (((struct box_int*)box_list_pop(e0->stack))->val == 21) {
        puts("OK");
    } else {
        puts("ERR");
    }
}
