set -e
#peg minihaskell.peg > minihaskell.c
leg minihaskell.leg > minihaskell.c
gcc main.c -Wno-unused-parameter
./a.out < prog
gcc -g -Wall -Wextra rt.c -Wno-unused-parameter
./a.out
