#pragma once
#include "ast.h"
#include "vm.h"

struct box_fun * mkfun(struct ast_node * f);
void box_append_code(struct box_fun * f, struct ast_node * x);

