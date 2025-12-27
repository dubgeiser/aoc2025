#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 5
#define MAX_COLS 4000

void consume_ops(char *line, char ops[]) {
  int i = 0;
  for (char *p = line; *p != '\0'; p++) {
    if (*p != ' ') {
      ops[i++] = *p;
    }
  }
}

// Read numbers in line into row.
// Return how many numbers (ie. cols) encountered.
// Line will be consumed and starting point will be lost.
int consume_row(char *line, int64_t row[]) {
  int i = 0;
  for (char *next; *line != '\0'; line = next) {
    long val = strtol(line, &next, 10);
    if (next == line) { // no more numbers
      break;
    }
    row[i++] = (int)val;
  }
  return i;
}

int64_t add(int64_t a, int64_t b) { return a + b; }
int64_t mul(int64_t a, int64_t b) { return a * b; }

int main() {
  int64_t p1 = 0;
  char *line;
  int64_t sheet[MAX_ROWS][MAX_COLS];
  char ops[MAX_COLS];
  int R = 0;
  int C = 0;

  // Part 2
  char grid[MAX_ROWS][MAX_COLS];
  int gridW = 0;
  int gridH = 0;

  while (NULL != (line = readline(stdin))) {
    // Part 2
    if (gridW == 0)
      gridW = strlen(line);
    for (int i = 0; i < gridW; i++)
      grid[gridH][i] = line[i];
    gridH++;

    if (line[0] == '+' || line[0] == '*') {
      consume_ops(line, ops);
    } else {
      C = consume_row(line, sheet[R]);
      R++;
    }
    free(line);
  }

  int64_t (*op)(int64_t, int64_t);
  for (int c = 0; c < C; c++) {
    int64_t tot = ops[c] == '+' ? 0 : 1;
    op = ops[c] == '+' ? add : mul;
    for (int r = 0; r < R; r++) {
      tot = op(tot, sheet[r][c]);
    }
    p1 += tot;
  }
  printf("%ld\n", p1);

  // Part 2
  char cur;
  int64_t p2 = 0;
  for (int c = 0; c < gridW; c++) {
    char snum[MAX_ROWS];
    int snum_i = 0;
    int64_t subtotal;
    int64_t (*op)(int64_t, int64_t);
    for (int r = 0; r < gridH; r++) {
      cur = grid[r][c];
      if (cur >= '0' && cur <= '9') {
        snum[snum_i++] = cur;
        continue;
      }
      // Only the last line matters for non-digits
      if (r < gridH - 1)
        continue;

      // An empty column, add the subtotal to the total
      if (cur == ' ' && snum_i == 0) {
        p2 += subtotal;
        continue;
      }

      snum[snum_i] = '\0';
      int64_t n = atoll(snum);
      if (cur == '+' || cur == '*') {
        op = cur == '+' ? add : mul;
        subtotal = n;
      } else {
        subtotal = op(subtotal, n);
      }
    }

    // Last column, don't forget to add subtotal once more
    if (c == gridW - 1)
      p2 += subtotal;
  }
  printf("%ld\n", p2);
}
