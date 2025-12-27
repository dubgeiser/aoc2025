# Advent of Code 2025
Doing AoC 2025 in C.
It's been about 35 years since I've programmed anything in C :seenoevil:

## Day 2
`strtok_r()` is reentrant, meaning it is thread safe and it can be used in _nested_ parsing situations.
However, `strtok_r()` is _not_ C11 compliant (see `man 3 strtok_r`) and as such needs a prototype:
```c
extern char *strtok_r(char *, const char *, char **);
```
Since I'm not too bothered my code wouldn't work on a Windows machine, I'm sticking with POSIX `strtok_r`.
It's worth noting that on Windows, you could use `strtok_s` if I quick-Google'd that correctly.

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

## Day 11
Graph with no cycles, path counting only specific paths:
Given function `int C(from, to)` that count the number of paths between `from` and `to`, you can find all the paths from `a` to `d` that must go via `b` and `c`:
```
C(a, b) * C(b, c) * C(c, d) // Combination/Product of all the counts going passing b first and then c
+
C(a, c) * C(c, b) * C(b, d) // Same, but in the other direction, so passing c first and then b
```
Took the time to write a slightly reusable way of splitting strings, in `lib/sstr.c/`
Found it a much easier puzzle than day 10 (of which, for now, I've only managed to parse the input :-) )
Part 2 was trickier; first the caching that I didn't add in part 1 and then finding out how to count all the correct paths.
Then I was thinking about combinations/products and realized that I might just have to multiply all paths in one direction, adding them to all paths in the other direction.
And let's just say that my company naming servers 'srv' instead of 'svr' did not help either in the beginning :-D
