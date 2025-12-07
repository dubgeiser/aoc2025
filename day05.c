#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGE 200
#define MAX_INGREDIENTS 1000

typedef struct {
  int64_t start;
  int64_t end;
} Range;

int main() {
  int p1 = 0;
  Range ranges[MAX_RANGE];
  int R = 0;
  int64_t ingredients[MAX_INGREDIENTS];
  int I = 0;
  char *line;
  int is_range = 1;

  while (NULL != (line = readline(stdin))) {
    if (strcmp(line, "") == 0) {
      is_range = 0;
      continue;
    }
    if (is_range) {
      sscanf(line, "%ld-%ld", &ranges[R].start, &ranges[R].end);
      R++;
    } else {
      sscanf(line, "%ld", &ingredients[I]);
      I++;
    }
    free(line);
  }

  for (int i = 0; i < I; i++) {
    for (int r = 0; r < R; r++) {
      if (ingredients[i] >= ranges[r].start &&
          ingredients[i] <= ranges[r].end) {
        p1++;
        break;
      }
    }
  }

  printf("%d\n", p1);
}
