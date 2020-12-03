#include "../src/compile.h"

struct ast_node * mkid(const char * bound) {
    struct ast_node * args = addnamelist(mknamelist(), mkatomload(strdup(bound)));
    struct ast_node * expr = mkatomload(strdup(bound));
    return mklam(args, expr);
}

int main() {
    struct ast_node * id = mkid("x");
    print_node(id);
    puts("");
    struct box_fun * f = mkfun(id);
    print_box((struct box_any*)f);

    // main
    struct opcode fopcodes[] = {
        { MKINT, 42 },
        { PUSHCONST, 0 },
        { PUSHCONST, 0 },
        { CALL, 0 },
        { CALL, 0 },
        { PUSHCONST, 0 },
        { CALL, 0 },
        { DUP, 0},
        { PRINT, 0 },
        { END, 0 }
    };
    struct box_fun * fmain = box_alloc(FUN);
    fmain->consts = mklist(1);
    box_list_set(fmain->consts, 0, (struct box_any*)f);
    fmain->opcodes = &fopcodes[0];
    fmain->nlocals = 0;
    fmain->nfree = 0;
    fmain->nbound = 0;
    fmain->nopcodes = sizeof(fopcodes) / sizeof(fopcodes[0]);

    struct box_eval * e0, * e = mkeval(0, fmain, 0, 0);
    e0 = e;

    while ((e = eval_next(e))) {
    };

    if (((struct box_int*)box_list_pop(e0->stack))->val == 42) {
        puts("OK");
    } else {
        puts("ERR");
    }
}
