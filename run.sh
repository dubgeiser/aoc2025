#!/usr/bin/env bash

cc -pedantic -std=c11 -Wall -Wextra -o solve "$1.c" -lm
./solve < data/"$1.dat"
exit 0
