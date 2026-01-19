#include "lib/input.c"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SIZE 150

typedef unsigned int uint;

typedef struct {
  uint row;
  uint col;
} Pos;

typedef struct {
  uint row;
  uint col;
  int64_t result;
} Qpos;

char grid[SIZE][SIZE];
uint R = 0;
uint C = 0;

Pos visited[SIZE * SIZE];
uint V = 0;

Qpos qcache[SIZE * SIZE];
uint Q = 0;

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

int64_t cache_get(uint row, uint col) {
  for (uint i = 0; i < Q; i++) {
    if (row == qcache[i].row && col == qcache[i].col) {
      return qcache[i].result;
    }
  }
  return -1;
}

void cache_set(uint row, uint col, int64_t result) {
  qcache[Q].row = row;
  qcache[Q].col = col;
  qcache[Q].result = result;
  Q++;
}

int64_t qdownward(uint row, uint col) {
  int64_t result = cache_get(row, col);
  if (result > -1) {
    return result;
  }

  int64_t timelines = 0;
  for (uint i = row; i < R; i++) {
    if (grid[i][col] == '.') {
      continue;
    }
    result = qdownward(i, col - 1);
    cache_set(i, col - 1, result);
    timelines += result;
    result = qdownward(i, col + 1);
    cache_set(i, col + 1, result);
    timelines += result;
    return timelines;
  }
  return 1;
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

  printf("%ld\n", qdownward(sr, sc));
  return 0;
}
