#include "lib/input.c"
#include <stdio.h>
#include <string.h>

int main() {
  int p1 = 0;
  char *line;
  int max, n;
  while (NULL != (line = readline(stdin))) {
    max = 0;
    for (size_t i = 0; i < strlen(line) - 1; i++) {
      for (size_t j = i + 1; j < strlen(line); j++) {
        n = 10 * (line[i] - '0') + (line[j] - '0');
        if (n > max) {
          max = n;
        }
      }
    }
    p1 += max;
  }
  printf("%d", p1);
  return 0;
}
