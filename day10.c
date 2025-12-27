#include "lib/input.c"
#include "lib/sstr.c"

#include <stdbool.h>
#include <stdio.h>

#define MAX_MACHINES 158
#define MAX_LIGHTS 10
#define MAX_BUTTONS 13
#define MAX_BWIRES 10
#define MAX_JOLTAGES 10

typedef struct {
  int buttons[MAX_BUTTONS];
  int size;
} ButtonWiring;

typedef struct {
  bool lights[MAX_LIGHTS];
  int len_l;
  ButtonWiring buttonwires[MAX_BWIRES];
  int len_b;
  int joltages[MAX_JOLTAGES];
  int len_j;
} Machine;

Machine machines[MAX_MACHINES];
int M = 0;

void input_parse_lights(Machine *m, char *s) {
  for (int i = 0; i < (int)strlen(s); i++) {
    if (s[i] == '[' || s[i] == ']')
      continue;
    m->lights[m->len_l++] = (s[i] == '#');
  }
}

void input_parse_buttons(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  char **parts;
  int size;
  parts = sstr_split(s, ",", (size_t *)&size);
  for (int i = 0; i < size; i++)
    m->buttonwires[m->len_b].buttons[m->buttonwires[m->len_b].size++] =
        atoi(parts[i]);
  m->len_b++;
  sstr_list_free(parts, size);
}

void input_parse_joltages(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  int size;
  char **parts = sstr_split(s, ",", (size_t *)&size);
  for (int i = 0; i < size; i++) {
    m->joltages[m->len_j++] = atoi(parts[i]);
  }
  sstr_list_free(parts, size);
}

void input() {
  char *line;
  while (NULL != (line = readline(stdin))) {
    int size;
    char **parts = sstr_split(line, " ", (size_t *)&size);
    input_parse_lights(&machines[M], parts[0]);
    input_parse_joltages(&machines[M], parts[size - 1]);
    for (int i = 1; i < size - 1; i++) {
      input_parse_buttons(&machines[M], parts[i]);
    }
    sstr_list_free(parts, size);
    free(line);
    M++;
  }
}

void print_machine(Machine m) {
  printf("[");
  for (int i = 0; i < m.len_l; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.lights[i]);
  }
  printf("]");

  for (int i = 0; i < m.len_b; i++) {
    printf(" (");
    for (int j = 0; j < m.buttonwires[i].size; j++) {
      if (j > 0)
        printf(",");
      printf("%d", m.buttonwires[i].buttons[j]);
    }
    printf(")");
  }

  printf(" {");
  for (int i = 0; i < m.len_j; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.joltages[i]);
  }
  printf("}\n");
}

int main() {
  input();
  for (int m = 0; m < M; m++) {
    print_machine(machines[m]);
  }
  printf("\n");
  return 0;
}
