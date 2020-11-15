#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "../src/ast.h"
#include "../src/compile.h"

struct box_any * eval(struct box_eval * ctx, struct ast_node * n) {
    box_append_code(ctx->code, last);
    if (ctx->ip > 0) {
        ctx->ip -= 1;
    }
    struct box_eval * e = ctx;
    while ((e = eval_next(e)));
    assert(ctx->sp <= 1);
    if (ctx->sp == 1) {
        struct box_any * result = ctx->stack[--ctx->sp];
        return result;
    }
    return 0;
}

int main() {
    int fd = open("test/repl_ctx.in", O_RDONLY);
    close(0);
    dup(fd);

    struct box_fun * module = mkfun(0);
    struct box_eval * ctx = mkeval(0, module, 0, 0);

    while (yyparse()) {
        printf(">>> ");
        print_node(last);
        puts("");
        struct box_any * res = eval(ctx, last);
        print_box(res);
        puts("");
    }
}
