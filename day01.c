#include "lib/input.c"
#include <stdio.h>

int diff(int pos, char d, int n) {
  if (d == 'L')
    n = -n;
  return (pos + n) % 100;
}

int turn(int pos, char d, int n) {
  int count = 0;
  int inc = (d == 'L') ? -1 : 1;
  for (int i = 0; i < n; i++) {
    pos = (pos + inc + 100) % 100;
    if (pos == 0) {
      count++;
    }
  }
  return count;
}

int main() {
  int p1 = 0;
  int p2 = 0;
  char *line;
  int pos = 50;

  char dir;
  int amount;
  while (NULL != (line = readline(stdin))) {
    sscanf(line, "%c%d", &dir, &amount);
    free(line);
    p2 += turn(pos, dir, amount);
    pos = diff(pos, dir, amount);
    if (0 == pos) {
      p1++;
    }
  }

  printf("%d\n", p1);
  printf("%d\n", p2);
  return 0;
}
