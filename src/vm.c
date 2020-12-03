#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if DEBUG
#include <execinfo.h>
void assert_(int ok, char * x) {
    if (ok) return;
    printf("\n=== assert(%s) failed ===\n", x);
    void *array[10];
    size_t size;
    size = backtrace(array, 10);
    backtrace_symbols_fd(array, size, 2);
    exit(1);
}
#define str_(x) #x
#define assert(x) assert_(!!(x), str_(x))
#else
#include <assert.h>
#endif

#include "vm.h"

const char * opcode_str[] ={
    "PUSHFREE", "PUSHBOUND", "PUSHLOCAL", "PUSHCONST",
    "POPFREE", "POPBOUND", "POPLOCAL",
    "POP", "CALL", "RETURN", "CLOSE", "END",
    // for debugging:
    "MKINT",
    "PRINT",
    "DUP",
    "CALL0"
};

// TODO
//  - Resumable Exceptions - add past end code?
//  - -> resumption points / don't actually pop/push until everything's fine??
//  - efficiency -> pledge to release resource?

#define ERROR(x) {puts(x);exit(1);}

void print_box(struct box_any * f) {
    if (f==0) {
        printf("NULL POINTER!!\n");
        return;
    }
    switch (f->type) {
        case FUN:
            printf("<Function %p>", f);
            break;
        case CLOSURE:
            printf("<Closure %p %d+%d/%d+%d>",
                    f,
                    box_list_len(((struct box_closure*)f)->free),
                    box_list_len(((struct box_closure*)f)->bound),
                    ((struct box_closure*)f)->fun->nfree,
                    ((struct box_closure*)f)->fun->nbound
                    );
            break;
        case CFUN:
            printf("<CFunction %p>", f);
            break;
        case CCLOSURE:
            printf("<CClosure %p>", f);
            break;
        case EVAL:
            printf("<Eval %p>", f);
            break;
        case INT:
            printf("<Int %d>", ((struct box_int*)f)->val);
            break;
        case LIST:
            printf("<LIST>");
            break;
    }
    printf("\n");
}

// CONSTRUCTORS

struct box_any *
mkcfun(int narg, call_type call) {
    struct box_cfun * cf = box_alloc(CFUN);
    cf->call = call;
    cf->narg = narg;
    return (struct box_any*)cf;
}

struct box_any *
mkint(int i) {
    struct box_int * x = box_alloc(INT);
    x->val = i;
    return (struct box_any*)x;
}

struct box_any *
mkclosure(struct box_fun * f, struct box_list * free) {
    if (f->nfree == 0) {
        ERROR("can not capture 0 free variables");
    }
    struct box_closure * c = box_alloc(CLOSURE);
    c->free = f->nfree ? box_list_copy(free) : 0;
    c->bound = mklist(0);
    c->fun = f;
    return (struct box_any*)c;
}

struct box_eval *
mkeval(
        struct box_eval * prev,
        struct box_fun * code,
        struct box_list * bound,
        struct box_list * free) {
    struct box_eval * es = box_alloc(EVAL);
    es->prev = prev;
    es->code = code;
    es->ip = 0;
    es->bound = bound;
    es->free = free;
    es->locals = mklist(code->nlocals);
    es->stack = mklist(0);
    return es;
};

// LIST

struct box_list * mklist(int len) {
    struct box_list * l = box_alloc(LIST);
    l->len = len;
    l->cap = len;
    l->items = calloc(len, sizeof(struct box_any *));
    return l;
}

void box_list_push(struct box_list * l, struct box_any * x) {
    assert(l);
    l->len += 1;
    if (l->len > l->cap) {
        l->cap += 8;
        l->items = realloc(l->items, l->cap*sizeof(struct box_any *));
        for (int i = l->len; i < l->cap; i++) {
            l->items[i] = 0;
        }
        assert(l->items);
    }
    l->items[l->len-1] = x;
}

struct box_any * box_list_pop(struct box_list * l) {
    assert(l);
    assert(l->len > 0);
    l->len -= 1;
    assert(l->items[l->len]);
    return l->items[l->len];
}

void box_list_set(struct box_list * l, int idx, struct box_any * x) {
    assert(l);
    if (idx < 0) idx += l->len;
    if (!(idx >= 0 && idx < l->len)) {
        l = 0;
        print_box((struct box_any*)&l[0]);
    }
    assert(idx >= 0 && idx < l->len);
    assert(x);
    l->items[idx] = x;
}

struct box_any * box_list_get(struct box_list * l, int idx) {
    assert(l);
    if (idx < 0) idx += l->len;
    assert(idx >= 0 && idx < l->len);
    assert(l->items[idx]);
    return l->items[idx];
}

struct box_list * box_list_copy(struct box_list * orig) {
    struct box_list * l = mklist(orig->len + 1);
    for (int i = 0; i < orig->len; i++) {
        box_list_set(l, i, box_list_get(orig, i));
    }
    return orig;
}

int box_list_len(struct box_list * l) {
    if (!l) return 0;
    return l->len;
}


// EVAL

static
struct box_eval *
eval_app(struct box_eval * current, struct box_any * f, struct box_any * x) {
    if (f->type == FUN) {
        struct box_fun * g = (struct box_fun *)f;
        if (g->nfree != 0) {
            ERROR("can not apply to function with free arguments");
            puts("b");
        }
        if (g->nbound < 1) {
            ERROR("fun nbound < 1???\n");
        } else if (g->nbound == 1) {
            struct box_list * args = mklist(1);
            box_list_set(args, 0, x);
            return mkeval(current, g, args, 0);
        } else {
            struct box_closure * c = box_alloc(CLOSURE);
            c->fun = g;
            c->free = 0;
            c->bound = mklist(1);
            box_list_set(c->bound, 0, x);
            box_list_push(current->stack, (struct box_any*)c);
            return current;
        }
    } else if (f->type == CLOSURE) {
        struct box_closure * c = (struct box_closure*)f;
        if (c->fun->nfree > 0 && box_list_len(c->free) == 0) {
            ERROR("Can not apply to closure with missing free capture");
        } else if (box_list_len(c->bound) + 1 == c->fun->nbound
                && box_list_len(c->bound) == 0) {
            // for closures over free instead bound variables
            struct box_list * args = mklist(1);
            box_list_set(args, 0, x);
            return mkeval(current, c->fun, args, c->free);
        } else if (box_list_len(c->bound) + 1 == c->fun->nbound) {
            struct box_list * args = box_list_copy(c->bound);
            box_list_push(args, x);
            struct box_eval * res = mkeval(current, c->fun, args, c->free);
            return res;
        } else {
            struct box_closure * c = (struct box_closure*)f;
            struct box_closure * k = box_alloc(CLOSURE);
            k->fun = c->fun;
            k->free = c->free;
            k->bound = box_list_copy(c->bound);
            box_list_push(k->bound, x);
            box_list_push(current->stack, (struct box_any*)k);
            return current;
        }
    } else if (f->type == CFUN) {
        struct box_cfun * g = (struct box_cfun *)f;
        if (g->narg == 1) {
            struct box_list * args = mklist(1);
            box_list_set(args, 0, x);
            struct box_any * res = g->call(args);
            box_list_push(current->stack, (struct box_any*)res);
            return current;
        } else {
            struct box_cclosure * c = box_alloc(CCLOSURE);
            c->cfun = g;
            c->bound = mklist(1);
            box_list_set(c->bound, 0, x);
            box_list_push(current->stack, (struct box_any*)c);
            return current;
        }
    } else if (f->type == CCLOSURE) {
        struct box_cclosure * c = (struct box_cclosure*)f;
        if (box_list_len(c->bound) + 1 == c->cfun->narg) {
            struct box_list * args = box_list_copy(c->bound);
            box_list_push(args, x);
            struct box_any * res = c->cfun->call(args);
            box_list_push(current->stack, (struct box_any*)res);
            return current;
        } else {
            struct box_cclosure * c = (struct box_cclosure*)f;
            struct box_cclosure * k = box_alloc(CCLOSURE);
            k->cfun = c->cfun;
            k->bound = box_list_copy(c->bound);
            box_list_push(k->bound, x);
            box_list_push(current->stack, (struct box_any*)k);
            return current;
        }
    } else {
        ERROR("eval_app(f, x), x != FUN or CLOSURE\n");
        exit(1);
    }
}

struct box_eval *
eval_next(struct box_eval * es) {
    struct box_any * f, * x;
    struct box_fun * g;
    struct box_list * l;
    int n;
    int arg = es->code->opcodes[es->ip].arg;
    enum opcode_code code = es->code->opcodes[es->ip].opcode;
    assert(es->ip < es->code->nopcodes);
#ifdef DEBUG
    printf("[%02x/%02d] %10s(%02d) % 10d %p [%d] <- ",
            (int)(((unsigned long)es >> 8) & 0xff),
            es->ip,
            (code >= 0 && code < 20) ? opcode_str[code] : "<ERROR>",
            code,
            arg, es, box_list_len(es->stack));
    if (box_list_len(es->stack) > 0) {
        print_box(box_list_get(es->stack, -1));
    } else {
        puts("");
    }
#endif
    es->ip++;
    switch (code) {
        case PUSHFREE:
            box_list_push(es->stack, box_list_get(es->free, arg));
            break;
        case PUSHBOUND:
            assert(arg >= 0 && arg < es->code->nbound);
            box_list_push(es->stack, box_list_get(es->bound, arg));
            break;
        case PUSHCONST:
            box_list_push(es->stack, box_list_get(es->code->consts, arg));
            break;
        case PUSHLOCAL:
            box_list_push(es->stack, box_list_get(es->locals, arg));
            break;
        case POPFREE:
            box_list_set(es->free, arg, box_list_pop(es->stack));
            break;
        case POPBOUND:
            box_list_set(es->bound, arg, box_list_pop(es->stack));
            break;
        case POPLOCAL:
            if (box_list_len(es->locals) < es->code->nlocals) {
#ifdef DEBUG
                puts("expand local storage");
#endif
                // optimize
                while (box_list_len(es->locals) < es->code->nlocals) {
                    box_list_push(es->locals, 0);
                }
            }
            box_list_set(es->locals, arg, box_list_pop(es->stack));
            break;
        case POP:
            (void)box_list_pop(es->stack);
            break;
        case CALL:
            f = box_list_pop(es->stack);
            x = box_list_pop(es->stack);
            return eval_app(es, f, x);
        case CLOSE:
            f = box_list_pop(es->stack);
            assert(f->type == FUN);
            n = ((struct box_fun*)f)->nfree;
            l = mklist(n);
            for (int i = 0; i < n; i++) {
                box_list_set(l, i, box_list_get(es->stack, i - n));
            }
            for (int i = 0; i < n; i++) {
                // optimize
                box_list_pop(es->stack);
            }
            x = mkclosure((struct box_fun*)f, l);
            box_list_push(es->stack, x);
            break;
        case RETURN:
            x = box_list_pop(es->stack);
            box_list_push(es->prev->stack, x);
            return es->prev;
        case END:
            return es->prev;
        case MKINT:
            box_list_push(es->stack, mkint(arg));
            break;
        case PRINT:
            x = box_list_pop(es->stack);
            print_box(x);
            break;
        case DUP:
            box_list_push(es->stack, box_list_get(es->stack, -1));
            break;
        case CALL0:
            g = (struct box_fun*)box_list_pop(es->stack);
            assert(g->type == FUN);
            assert(g->nfree == 0);
            assert(g->nbound == 0);
            return mkeval(es, g, 0, 0);
    }
    return es;
}
