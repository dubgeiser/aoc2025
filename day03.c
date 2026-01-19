#include "lib/input.c"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int joltage1(char *line) {
  int max = 0;
  int n;
  for (size_t i = 0; i < strlen(line) - 1; i++) {
    for (size_t j = i + 1; j < strlen(line); j++) {
      n = 10 * (line[i] - '0') + (line[j] - '0');
      if (n > max) {
        max = n;
      }
    }
  }
  return max;
}

int64_t joltage2(char *line) {
  const int K = 12;
  int idx = 0;
  int pos = 0;
  int L = strlen(line);
  int64_t max = 0;
  while (idx < K) {
    char maxd = '0';
    int maxp = pos;
    for (int i = pos; i <= L - (K - idx); i++) {
      if (line[i] > maxd) {
        maxd = line[i];
        maxp = i;
      }
    }
    max = max * 10 + (maxd - '0');
    idx++;
    pos = maxp + 1;
  }
  return max;
}

int main() {
  int p1 = 0;
  int64_t p2 = 0;
  char *line;
  while (NULL != (line = readline(stdin))) {
    p1 += joltage1(line);
    p2 += joltage2(line);
    free(line);
  }
  printf("%d\n", p1);
  printf("%ld\n", p2);
  return 0;
}
