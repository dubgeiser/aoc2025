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

typedef struct {
  char *in;
  char *out;
  size_t total;
} ConnectionCount;

// TODO is this enough, maybe have a PathCount * and cache_size?
ConnectionCount cache[MAX_DEVICES * MAX_DEVICES];
size_t cache_size;

int64_t cache_get(char *in, char *out) {
  for (size_t i = 0; i < cache_size; i++) {
    if (in == cache[i].in && out == cache[i].out)
      return cache[i].total;
  }
  return -1;
}

void cache_set(char *in, char *out, size_t total) {
  cache[cache_size].in = in;
  cache[cache_size].out = out;
  cache[cache_size].total = total;
  cache_size++;
}

size_t count_paths(char *in, char *out) {
  if (strcmp(in, out) == 0)
    return 1;
  int64_t cached = cache_get(in, out);
  if (cached >= 0) {
    return (size_t)cached;
  }
  int curr = connections_get(in);
  if (curr == -1)
    return 0;
  size_t total = 0;
  for (size_t i = 0; i < cnx[curr].size; i++) {
    total += count_paths(cnx[curr].outs[i], out);
  }
  cache_set(in, out, total);
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

  // We're assuming there are no cycles in our connections.
  // We can sum all the paths from srv->dac->fft->out
  // and all the paths from srv->fft->dac->out
  //
  // "All the paths from srv->dac->fft->out" translates to
  // a product of all path counts: srv->dac * dac->fft * fft->out
  size_t p2 = count_paths("svr", "dac") * count_paths("dac", "fft") *
              count_paths("fft", "out");
  p2 += count_paths("svr", "fft") * count_paths("fft", "dac") *
        count_paths("dac", "out");
  printf("%ld\n", p2);

  // Final Cleanup
  for (size_t i = 0; i < C; i++) {
    free(cnx[i].in);
    sstr_list_free(cnx[i].outs, cnx[i].size);
  }

  return 0;
}
