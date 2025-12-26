#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define SSTR_LIST_INITIAL_CAPACITY 4

/**
 * Split `str` on `delim` and return the parts.
 * Store the size of the parts in `out_size`
 * Return `NULL` and set `out_size` to 0 if string couldn't be split.
 * Fail if no memory could be allocated.
 */
char **sstr_split(const char *str, const char *delim, size_t *out_size);

/**
 * Free memory for a given list of strings
 */
void sstr_list_free(char **list, size_t size);

/**
 * Copy the token identified by `start` and `len` and add it to the `list`.
 * Grow (or initialize) `capacity` if there is not enough space.
 * Return whether or not this succeeded.
 *
 * Note that this function might have to reallocate memory for `list`, so the
 * caller needs to pass a reference to a `char **`, so that it has the updated
 * pointer if this is the case.
 * A simple 'double the capacity' algo is used for growing the capacity.
 */
static int sstr_append(char ***list, size_t *size, size_t *capacity,
                       const char *start, size_t len);

char **sstr_split(const char *str, const char *delim, size_t *out_size) {
  char **parts = NULL;
  size_t size = 0;
  size_t capacity = 0;
  size_t delim_len = strlen(delim);
  if (delim_len == 0) {
    *out_size = 0;
    return NULL;
  }
  const char *start = str;
  const char *cur_delim;
  while ((cur_delim = strstr(start, delim)) != NULL) {
    size_t len = (size_t)(cur_delim - start);
    if (len > 0) {
      if (!sstr_append(&parts, &size, &capacity, start, len))
        goto fail;
    }
    start = cur_delim + delim_len;
  }

  // Process last part
  if (*start != '\0') {
    if (!sstr_append(&parts, &size, &capacity, start, strlen(start)))
      goto fail;
  }
  *out_size = size;
  return parts;

fail:
  sstr_list_free(parts, size);
  *out_size = 0;
  return NULL;
}

void sstr_list_free(char **list, size_t size) {
  for (size_t i = 0; i < size; i++)
    free(list[i]);
  free(list);
}

static int sstr_append(char ***list, size_t *size, size_t *capacity,
                       const char *start, size_t len) {
  // Grow (or initialize) the capacity of the list.
  if (*size == *capacity) {
    size_t newcap = *capacity ? *capacity * 2 : SSTR_LIST_INITIAL_CAPACITY;
    char **tmp = realloc(*list, newcap * sizeof(**list));
    if (!tmp)
      return 0;
    *list = tmp;
    *capacity = newcap;
  }

  // Copy token (pointed to by `start` with length `len`) to parts
  char *token = malloc(len + 1);
  if (!token)
    return 0;
  memcpy(token, start, len);
  token[len] = '\0';
  (*list)[(*size)++] = token;
  return 1;
}
