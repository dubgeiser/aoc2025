#include "lib/input.c"
#include "lib/sstr.c"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 593

typedef struct {
  char *in;
  char **outs;
  size_t size;
} Connection;

Connection cnx[MAX_DEVICES];
size_t C;

int connections_get(char *in) {
  for (size_t i = 0; i < C; i++) {
    if (strcmp(in, cnx[i].in) == 0) {
      return i;
    }
  }
  return -1;
}

size_t count_paths(char *in, char *out) {
  if (strcmp(in, out) == 0)
    return 1;
  int curr = connections_get(in);
  if (curr == -1)
    return 0;
  size_t total = 0;
  for (size_t i = 0; i < cnx[curr].size; i++) {
    total += count_paths(cnx[curr].outs[i], out);
  }
  return total;
}

int main() {
  char *line;
  C = 0;
  while (NULL != (line = readline(stdin))) {
    size_t size = 0;
    char **parts = sstr_split(line, ": ", &size);
    free(line);
    if (parts == NULL) {
      perror("Could not split");
    }
    cnx[C].in = parts[0];
    cnx[C].outs = sstr_split(parts[1], " ", &cnx[C].size);
    C++;
    // NOT `free(parts[1])`, keeping the device (in) strings
    free(parts[1]);
    free(parts);
  }

  printf("%ld\n", count_paths("you", "out"));

  // Final Cleanup
  for (size_t i = 0; i < C; i++) {
    free(cnx[i].in);
    sstr_list_free(cnx[i].outs, cnx[i].size);
  }

  return 0;
}
