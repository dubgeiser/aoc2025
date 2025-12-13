#!/usr/bin/env bash

cc -pedantic -std=c11 -Wall -Wextra -o solve "$1.c" -lm
cat data/"$1.dat" | ./solve
exit 0
