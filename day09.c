#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int64_t x;
  int64_t y;
} Pos ;

int64_t area(Pos a, Pos b) {
  return (labs(a.x - b.x) + 1) * (labs(a.y - b.y) + 1);
}

int main() {
  int64_t p1 = 0;
  int64_t a;
  Pos tiles[500];
  int T;
  char *line;
  T = 0;
  while (NULL != (line = readline(stdin))) {
    sscanf(line, "%ld,%ld", &tiles[T].x, &tiles[T].y);
    free(line);
    T++;
  }
  for (int i = 0; i < T - 1; i++) {
    for (int j = i + 1; j < T; j++) {
      a = area(tiles[i], tiles[j]);
      if (a > p1) {
        p1 = a;
      }
    }
  }
  printf("%ld\n", p1);
  return 0;
}
