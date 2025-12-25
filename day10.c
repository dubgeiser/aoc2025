#include "lib/input.c"

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
  char *token;
  char *pstate;
  token = strtok_r(s, ",", &pstate);
  while (token != NULL) {
    m->buttonwires[m->len_b].buttons[m->buttonwires[m->len_b].size++] = atoi(token);
    token = strtok_r(NULL, ",", &pstate);
  }
  m->len_b++;
}

void input_parse_joltages(Machine *m, char *s) {
  char *token;
  char *pstate;
  token = strtok_r(s, ",", &pstate);
  while (token != NULL) {
    if (token[0] == '{')
      token++;
    if (token[strlen(token) - 1] == '}')
      token[strlen(token) - 1] = '\0';
    m->joltages[m->len_j++] = atoi(token);
    token = strtok_r(NULL, ",", &pstate);
  }
}

void input() {
  char *line;
  char *token;
  char *pstate;
  while (NULL != (line = readline(stdin))) {
    token = strtok_r(line, " ", &pstate);
    input_parse_lights(&machines[M], token);
    while (NULL != (token = strtok_r(NULL, " ", &pstate))) {
      if (token[0] == '(')
        input_parse_buttons(&machines[M], token);
      else if (token[0] == '{')
        input_parse_joltages(&machines[M], token);
      else
        perror("ERROR: Unknown token");
    }
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
