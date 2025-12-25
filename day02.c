#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

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

int64_t sum_silly_ids(Range r) {
  int64_t sum = 0;

  for (int64_t n = r.start; n <= r.end; n++) {
    char s[32];
    snprintf(s, sizeof(s), "%lld", (long long)n);
    int len = strlen(s);
    for (int blk = 1; blk <= len / 2; blk++) {
      if (len % blk != 0) {
        continue;
      }
      int ok = 1;
      for (int i = blk; i < len; i += blk) {
        if (strncmp(s, s + i, blk) != 0) {
          ok = 0;
          break;
        }
      }
      if (ok) {
        sum += n;
        break;
      }
    }
  }
  return sum;
}

int main() {
  int64_t p1 = 0;
  int64_t p2 = 0;
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
    p2 += sum_silly_ids(ranges[i]);
  }

  printf("%ld\n", p1);
  printf("%ld\n", p2);

  return 0;
}
