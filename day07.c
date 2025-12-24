#include "lib/input.c"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SIZE 150

typedef unsigned int uint;
typedef struct {
  uint row;
  uint col;
} Pos;

char grid[SIZE][SIZE];
uint R = 0;
uint C = 0;

Pos visited[SIZE * SIZE];
uint V = 0;

bool is_visited(uint row, uint col) {
  for (uint i = 0; i < V; i++) {
    if (row == visited[i].row && col == visited[i].col) {
      return true;
    }
  }
  return false;
}

void visit(uint row, uint col) {
  visited[V].row = row;
  visited[V].col = col;
  V++;
}

void downward(uint row, uint col) {
  for (uint i = row; i < R; i++) {
    if (grid[i][col] == '.')
      continue;
    if (is_visited(i, col))
      return;
    downward(i, col - 1);
    downward(i, col + 1);
    visit(i, col);
    return;
  }
}

int main() {
  uint sr;
  uint sc;
  char *line;
  while (NULL != (line = readline(stdin))) {
    if (C == 0) {
      C = strlen(line);
    }
    for (uint c = 0; c < C; c++) {
      grid[R][c] = line[c];
      if (line[c] == 'S') {
        sr = R;
        sc = c;
      }
    }
    free(line);
    R++;
  }

  downward(sr, sc);
  printf("%d\n", V);
  return 0;
}
