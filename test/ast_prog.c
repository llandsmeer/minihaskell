#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../src/ast.h"

int main() {
    int fd = open("test/prog", O_RDONLY);
    close(0);
    dup(fd);
    while (yyparse()) {
        print_node(last);
        printf("\n");
    }
    return 0;
}
