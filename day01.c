#include "lib/input.c"
#include <stdio.h>

int diff(int pos, char d, int n) {
  if (d == 'L') {
    pos -= n % 100;
  } else {
    pos += n % 100;
  }
  return pos % 100;
}

int main() {
  int p1 = 0;
  char *line;
  int pos = 50;

  char dir;
  int amount;
  while (NULL != (line = readline(stdin))) {
    sscanf(line, "%c%d", &dir, &amount);
    pos = diff(pos, dir, amount);
    if (0 == pos) {
      p1++;
    }
  }

  free(line);
  printf("%d\n", p1);
  return 0;
}
