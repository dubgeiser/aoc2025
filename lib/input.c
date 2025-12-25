#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// strtok_r is POSIX, but _not_ C11, need prototype.
extern char *strtok_r(char *, const char *, char **);

char *readline(FILE *fp) {
  const size_t CHUNK = 128;
  size_t size = CHUNK;
  size_t len = 0;
  char *buf = malloc(size);
  if (!buf)
    return NULL;
  while (fgets(buf + len, (int)(size - len), fp)) {
    len += strlen(buf + len);

    // Got the complete line
    if (len > 0 && buf[len - 1] == '\n') {
      buf[len - 1] = '\0';
      return buf;
    }
    // Line > CHUNCK, allocate more space
    size_t new_size = size + CHUNK;
    char *new_buf = realloc(buf, new_size);
    if (!new_buf) {
      free(buf);
      return NULL;
    }
    buf = new_buf;
    size = new_size;
  }

  free(buf);
  return NULL;
}
