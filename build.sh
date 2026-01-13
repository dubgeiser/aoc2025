#!/usr/bin/env bash

if [ ! -d build ]; then
    mkdir build
fi
cc -pedantic -std=c11 -Wall -Wextra -o"build/$1" "$1.c" -lm
exit 0
