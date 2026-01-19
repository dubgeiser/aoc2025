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

int cmprange(const void *a, const void *b) {
  Range *ra = (Range *)a;
  Range *rb = (Range *)b;
  if (ra->start < rb->start)
    return -1;
  if (ra->start > rb->start)
    return 1;
  return 0;
}

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

  // Sort ranges, then merge them, fi sample input:
  //
  // {3,5} {10,14} {16,20} {12,18} will become sorted:
  // {3,5} {10,14} {12,18} {16,20}
  //
  // {3,5} -> 3 numbers, no overlap: p2 = 5-3+1 = 3
  // {10,14} -> 12 <= 14 -> 18>14 -> merge {10,18}
  // {16,20} -> ... -> merge -> {10,20}
  // => {3,5} -> 3 numbers
  // => {10,20} -> 11 numbers
  // total: 14 numbers.
  qsort(ranges, R, sizeof(Range), cmprange);
  int64_t p2 = 0;
  int64_t start = ranges[0].start;
  int64_t end = ranges[0].end;
  for (int i = 1; i < R; i++) {
    if (ranges[i].start <= end) {
      // Overlap -> extend current end if necessary.
      if (ranges[i].end > end) {
        end = ranges[i].end;
      }
    } else {
      // No overlap -> add current range length
      p2 += end - start + 1;
      // New range
      start = ranges[i].start;
      end = ranges[i].end;
    }
  }
  // Do not forget the last range
  p2 += end - start + 1;
  printf("%ld\n", p2);
}
