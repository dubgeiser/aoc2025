#include "lib/input.c"
#include "lib/sstr.c"

#include <stdbool.h>
#include <stdio.h>

#define MAX_MACHINES 160
#define MAX 20

typedef struct {
  int buttons[MAX];
  size_t size;
} ButtonWiring;

typedef struct {
  bool lights[MAX];
  size_t len_l;
  ButtonWiring buttonwires[MAX];
  size_t len_b;
  int joltages[MAX];
  size_t len_j;
} Machine;

Machine machines[MAX_MACHINES];
size_t M = 0;

void input_parse_lights(Machine *m, char *s) {
  for (size_t i = 0; i < strlen(s); i++) {
    if (s[i] == '[' || s[i] == ']')
      continue;
    m->lights[m->len_l++] = (s[i] == '#');
  }
}

void input_parse_buttons(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  char **parts;
  size_t size;
  parts = sstr_split(s, ",", &size);
  for (size_t i = 0; i < size; i++)
    m->buttonwires[m->len_b].buttons[m->buttonwires[m->len_b].size++] =
        atoi(parts[i]);
  m->len_b++;
  sstr_list_free(parts, size);
}

void input_parse_joltages(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  size_t size;
  char **parts = sstr_split(s, ",", &size);
  for (size_t i = 0; i < size; i++) {
    m->joltages[m->len_j++] = atoi(parts[i]);
  }
  sstr_list_free(parts, size);
}

void input() {
  char *line;
  while (NULL != (line = readline(stdin))) {
    size_t size;
    char **parts = sstr_split(line, " ", &size);
    input_parse_lights(&machines[M], parts[0]);
    input_parse_joltages(&machines[M], parts[size - 1]);
    for (size_t i = 1; i < size - 1; i++) {
      input_parse_buttons(&machines[M], parts[i]);
    }
    sstr_list_free(parts, size);
    free(line);
    M++;
  }
}

void print_machine(Machine m) {
  printf("[");
  for (size_t i = 0; i < m.len_l; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.lights[i]);
  }
  printf("]");

  for (size_t i = 0; i < m.len_b; i++) {
    printf(" (");
    for (size_t j = 0; j < m.buttonwires[i].size; j++) {
      if (j > 0)
        printf(",");
      printf("%d", m.buttonwires[i].buttons[j]);
    }
    printf(")");
  }

  printf(" {");
  for (size_t i = 0; i < m.len_j; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.joltages[i]);
  }
  printf("}\n");
}

int main() {
  input();
  for (size_t i = 0; i < M; i++) {
    print_machine(machines[i]);
  }
  printf("\n");
  return 0;
}
