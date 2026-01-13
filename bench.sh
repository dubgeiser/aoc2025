#!/usr/bin/env bash

for each in day*.c; do
    TARGET="${each%.*}"
    ./build.sh "$TARGET"
done

time (
for each in build/day*; do
    TARGET=$(basename "$each")
    echo "$TARGET"
    "$each" < data/"$TARGET".dat
    echo
done
)
