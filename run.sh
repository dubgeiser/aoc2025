#!/usr/bin/env bash

cc -pedantic -std=c11 -Wall -Wextra -lm -o solve "$1.c"
./solve < data/"$1.dat"
exit 0
