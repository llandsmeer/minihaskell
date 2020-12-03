#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "vm.h"

static struct {
    struct box_any ** objects;
    int nobjects;
    int cap;
} mem = {
    0,
    0,
    0,
};

static size_t box_size(enum type type) {
    switch (type) {
        case FUN: return sizeof(struct box_fun);
        case CLOSURE: return sizeof(struct box_closure);
        case CFUN: return sizeof(struct box_cfun);
        case CCLOSURE: return sizeof(struct box_cclosure);
        case INT: return sizeof(struct box_int);
        case EVAL: return sizeof(struct box_eval);
        case LIST: return sizeof(struct box_list);
    }
    assert(0);
}

void * box_alloc(enum type type) {
    struct box_any * box = malloc(box_size(type));
    box->type = type;
    if (mem.nobjects + 1 > mem.cap) {
        mem.cap += 16;
        mem.objects = realloc(mem.objects, mem.cap * sizeof(struct box_any *));
    }
    mem.objects[mem.nobjects] = box;
    mem.nobjects += 1;
    return box;
}
