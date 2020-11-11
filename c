set -e

leg ast.leg > src/ast.c

runtest () {
    echo "# $1"
    gcc -Wall -Wextra -Werror \
        src/ast.c \
        src/vm.c \
        src/compile.c \
        test/$1.c \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-missing-field-initializers \
        -g -o test/$1.x
    test/$1.x | tee test/$1.out
    echo
}

runtest ast_prog
runtest vm_add
runtest compile_id
runtest compile_const
runtest compile_const_free
