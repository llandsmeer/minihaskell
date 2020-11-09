#include <stdio.h>
#include <stdlib.h>

struct ast {
    const char * name;
    struct ast * left;
    struct ast * right;
};

struct ast * last;
struct ast * node(const char * name, struct ast * left, struct ast * right) {
    struct ast * n = malloc(sizeof(struct ast));
    n->name = name;
    n->left = left;
    n->right = right;
    last = n;
    return n;
};

void print_node(struct ast * node) {
    if (!node->left && !node->right) { printf("%s", node->name); return; }
    printf("(%s", node->name);
    if (node->left)  { printf(" "); print_node(node->left); }
    if (node->right) { printf(" "); print_node(node->right); }
    printf(")");
}

#define YYSTYPE struct ast *

#include "minihaskell.c"

int main() {
    while (yyparse()) {
        print_node(last);
        printf("\n");
    }
    return 0;
}
