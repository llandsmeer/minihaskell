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
        { CALL, 0 },
        { PUSHCONST, 0 },
        { CALL, 0 },
        { PRINT, 0 },
        { END, 0 }
    };
    struct box_fun * fmain = box_alloc(FUN);
    fmain->consts = box_list_alloc(1);
    fmain->consts[0] = (struct box_any*)f;
    fmain->opcodes = &fopcodes[0];
    fmain->stacksize = 1000;
    fmain->nlocals = 0;
    fmain->nfree = 0;
    fmain->nbound = 0;

    struct box_eval * e = mkeval(0, fmain, 0, 0);

    while ((e = eval_next(e))) {
        //printf("<%p>\n", e);
    };
}
