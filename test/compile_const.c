#include "../src/compile.h"

struct ast_node * mkconst() {
    struct ast_node * args = addnamelist(addnamelist(mknamelist(),
            mkatomload("x")), mkatomload("y"));
    struct ast_node * expr = mkatomload(strdup("x"));
    return mklam(args, expr);
}

int main() {
    struct ast_node * id = mkconst();
    print_node(id);
    puts("");
    struct box_fun * f = mkfun(id);
    print_box((struct box_any*)f);

    // main
    struct opcode fopcodes[] = {
        { MKINT, 2 },
        { MKINT, 1 },
        { PUSHCONST, 0 },
        { CALL, 0 },
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

    struct box_eval * e0,* e = mkeval(0, fmain, 0, 0);
    e0 = e;

    while ((e = eval_next(e))) {
    };

    if (((struct box_int*)e0->stack[0])->val == 1) {
        puts("OK");
    } else {
        puts("ERR");
    }
}
