#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../src/ast.h"
#include "../src/compile.h"

struct ctx {
    struct ctx * next;
    char * name;
    struct box_fun * expr;
};


int main() {
    struct opcode fopcodes[] = {
        { PUSHCONST, 0 },
        { CALL0, 0 },
        { PRINT, 0 },
        { END, 0 }
    };
    struct box_fun * fmain = box_alloc(FUN);
    fmain->consts = box_list_alloc(1);
    fmain->opcodes = &fopcodes[0];
    fmain->stacksize = 1000;
    fmain->nlocals = 0;
    fmain->nfree = 0;
    fmain->nbound = 0;
    fmain->nconsts = 1;
    fmain->nopcodes = sizeof(fopcodes) / sizeof(fopcodes[0]);

    //struct ctx head = { 0 };
    int fd = open("test/repl.in", O_RDONLY);
    close(0);
    dup(fd);
    while (yyparse()) {
        puts("=====================================");
        print_node(last);
        puts("");
        if (last->type != STMTASSIGN) {
            printf("Not an assigment:");
            print_node(last);
            printf("\n");
            continue;
        } else {
            struct box_fun * g = mkfun(mklam(mknamelist(), last->x.stmtassign.expr));
            fmain->consts[0] = (struct box_any*)g;
            struct box_eval * e = mkeval(0, fmain, 0, 0);
            while ((e = eval_next(e))) {};
        }
    }
    return 0;
}
