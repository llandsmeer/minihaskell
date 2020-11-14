set -e

args="$@"

leg ast.leg > src/ast.c

runtest () {
    echo "# $1"
    gcc -Wall -Wextra -Werror \
        src/ast.c \
        src/vm.c \
        src/compile.c \
        test/$1.c \
        -fsanitize=address \
        -fno-omit-frame-pointer \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-missing-field-initializers \
        -g -o test/$1.x \
        $args
    ASAN_OPTIONS=detect_leaks=false test/$1.x
    echo
}

runtest ast_prog
runtest vm_add
runtest compile_id
runtest compile_const
runtest compile_const_free
runtest repl
