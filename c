set -e

leg ast.leg > src/ast.c

runtest () {
    gcc -Wall -Wextra -Werror \
        src/ast.c \
        src/vm.c \
        test/$1.c \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-missing-field-initializers \
        -g -o test/$1.x
    test/$1.x | tee test/$1.out
}

runtest ast_prog
runtest vm_add
