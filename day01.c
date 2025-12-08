#include "lib/input.c"
#include <stdio.h>

int diff(int pos, char d, int n) {
  if (d == 'L')
    n = -n;
  return (pos + n) % 100;
}

int main() {
  int p1 = 0;
  char *line;
  int pos = 50;

  char dir;
  int amount;
  while (NULL != (line = readline(stdin))) {
    sscanf(line, "%c%d", &dir, &amount);
    free(line);
    pos = diff(pos, dir, amount);
    if (0 == pos) {
      p1++;
    }
  }

  printf("%d\n", p1);
  return 0;
}
