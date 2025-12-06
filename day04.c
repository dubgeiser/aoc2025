#include "lib/input.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 140

int reduce(int grid[SIZE][SIZE], int gridn[SIZE][SIZE]) {
  int total;
  int r, c;
  int count = 0;
  for (r = 0; r < SIZE; r++) {
    for (c = 0; c < SIZE; c++) {
      if (grid[r][c] == 0)
        continue;
      total = 0;
      if (c > 0)
        total += grid[r][c - 1];
      if (c > 0 && r > 0)
        total += grid[r - 1][c - 1];
      if (r > 0)
        total += grid[r - 1][c];
      if (r > 0 && c < SIZE - 1)
        total += grid[r - 1][c + 1];
      if (c < SIZE - 1)
        total += grid[r][c + 1];
      if (c < SIZE - 1 && r < SIZE - 1)
        total += grid[r + 1][c + 1];
      if (r < SIZE - 1)
        total += grid[r + 1][c];
      if (r < SIZE - 1 && c > 0)
        total += grid[r + 1][c - 1];

      if (total < 4) {
        count++;
        gridn[r][c] = 0;
      }
    }
  }
  return count;
}

int main() {
  int p1, p2 = 0;
  int grid[SIZE][SIZE] = {0};
  int gridn[SIZE][SIZE] = {0};
  char *line;
  int r = 0;
  int c = 0;
  while (NULL != (line = readline(stdin))) {
    for (c = 0; c < SIZE; c++) {
      if (line[c] == '@') {
        grid[r][c] = 1;
      }
    }
    free(line);
    r++;
  }

  p1 = reduce(grid, gridn);
  printf("%d\n", p1);

  int rr;
  memcpy(gridn, grid, sizeof(grid));
  while ((rr = reduce(grid, gridn)) > 0) {
    p2 += rr;
    memcpy(grid, gridn, sizeof(grid));
  }
  printf("%d\n", p2);
  return 0;
}
