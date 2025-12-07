#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

// strtok_r is POSIX, but _not_ C11, need prototype.
extern char *strtok_r(char *, const char *, char **);

typedef struct {
  int64_t start;
  int64_t end;
} Range;

// !!! Unknown for n < 0 !!!
int64_t countdigits(int64_t n) {
  if (n < 10) {
    return 1;
  }
  return 1 + countdigits(n / 10);
}

int is_eligible(int64_t n) { return countdigits(n) % 2 == 0; }

int is_eligible_range(Range r) {
  return is_eligible(r.start) || is_eligible(r.end);
}

int64_t sum_invalid_ids(Range r) {
  if (!is_eligible_range(r)) {
    return 0;
  }
  int64_t sum = 0;
  int64_t half;
  for (int64_t n = r.start; n <= r.end; n++) {
    if (!is_eligible(n)) {
      continue;
    }

    half = countdigits(n) / 2;
    int64_t div = 1;
    for (int i = 0; i < half; i++) {
      div *= 10;
    }
    if (n / div == n % div) {
      sum += n;
    }
  }
  return sum;
}

int main() {
  int64_t p1 = 0;
  Range ranges[SIZE];
  char *token, *pstate;       // tokenizing on ","
  char *subtoken, *subpstate; // (sub)tokenizing on "-"
  char *line = readline(stdin);
  int R = 0;

  token = strtok_r(line, ",", &pstate);
  while (NULL != token) {
    subtoken = strtok_r(token, "-", &subpstate);
    ranges[R].start = strtoll(subtoken, NULL, 10);
    subtoken = strtok_r(NULL, "-", &subpstate);
    ranges[R].end = strtoll(subtoken, NULL, 10);
    token = strtok_r(NULL, ",", &pstate);
    R++;
  }
  free(line);

  for (int i = 0; i < R; i++) {
    p1 += sum_invalid_ids(ranges[i]);
  }

  printf("%ld\n", p1);

  return 0;
}
