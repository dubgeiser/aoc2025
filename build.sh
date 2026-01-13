#!/usr/bin/env bash

if [ ! -d build ]; then
    mkdir build
fi
cc -pedantic -std=c11 -Wall -Wextra -o"build/$1" -lm "$1.c"
exit 0
