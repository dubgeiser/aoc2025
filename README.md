# Advent of Code 2025
Doing AoC 2025 in C.
It's been about 35 years since I've programmed anything in C :seenoevil:

## Day 4
Note that performance impact of `malloc()` and `free()` when reading a 140x140 line file is neglible, it _is_ way faster reading fixed size lines this way:
```C
int p1 = 0;
int grid[SIZE][SIZE] = {0};
char line[SIZE + 2];   // +1 for '\n' and +1 for '\0'
int r = 0;
int c = 0;

while (r < SIZE && fgets(line, sizeof line, stdin)) {
  for (c = 0; c < SIZE; c++) {
    //...
  }
  r++;
}
```
