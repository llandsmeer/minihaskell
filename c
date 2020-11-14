set -e

args="$@"

leg ast.leg > src/ast.c
echo "LEG ast.leg"

cc () {
    gcc -Wall -Wextra -Werror \
        "$@" \
        -fsanitize=address \
        -fno-omit-frame-pointer \
        -Wno-unused-parameter \
        -Wno-unused-function \
        -Wno-missing-field-initializers \
        -g \
        $args
}

mkdir -p build
rm -f build/*.o
for file in $(cd src; echo *.c)
do
    echo "CC  $file"
    cc -c "src/${file}" -o "build/${file}.o"
done

ar rcs build/lib.a build/*.o

runtest () {
    echo "# $1"
    cc test/$1.c -o test/$1.x build/lib.a
    ASAN_OPTIONS=detect_leaks=false test/$1.x
    echo
}

runtest ast_prog
runtest vm_add
runtest compile_id
runtest compile_const
runtest compile_const_free
runtest repl
runtest repl_ctx
