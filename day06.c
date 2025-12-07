#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

  while (NULL != (line = readline(stdin))) {
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
}
