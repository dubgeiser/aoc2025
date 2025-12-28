#include "lib/input.c"
#include "lib/sstr.c"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MACHINES 158
#define MAX_BUTTONS 13
#define MAX_JOLTAGES 10

typedef struct {
  int lights;
  int L;
  int buttons[MAX_BUTTONS];
  int B;
  int joltages[MAX_JOLTAGES];
  int J;
} Machine;

Machine machines[MAX_MACHINES];
int M = 0;

/**
 * Parse the lights-part of the input `s` into machine 'm'
 * Return the number of places in lights, fi: `"[.##.]"` will return 4.
 * This number is also stored in the m.n, it is needed throughout our solution.
 */
int input_parse_lights(Machine *m, char *s) {
  int v = 0;
  int size = 0;
  for (; *s; s++) {
    if (*s == '#' || *s == '.') {
      v <<= 1;
      if (*s == '#')
        v |= 1;
      size++;
    }
  }
  m->lights = v;
  m->L = size;
  return size;
}

/**
 * Parse button press `s` to a bitmask of `n` positions.
 * For instance:
 * ```
 *  s = "(0,1,2)"
 *  n = 4
 * -> 0b1110 -> 0*2^0 + 1*2^1 + 1*2^2 + 1*2^3 + 0*4^4
 *           -> 0 + 2 + 4 + 8 + 0
 *           -> 14
 * ```
 */
int ORIGINAL_input_parse_button2bitmask(char *s, int n) {
  s++;
  s[strlen(s) - 1] = '\0';
  char **parts;
  int size;
  char bp_s[n];
  for (int i = 0; i < n; i++) {
    bp_s[i] = '0';
  }
  parts = sstr_split(s, ",", (size_t *)&size);
  // I Haven't checked if the elements of a button press are always sorted.
  // Making the binary representation first.
  for (int i = 0; i < size; i++) {
    int idx = atoi(parts[i]);
    bp_s[idx] = '1';
  }
  int v = 0;
  for (int i = 0; i < n; i++) {
    v <<= 1;
    if (bp_s[i] == '1')
      v |= 1;
  }
  sstr_list_free(parts, size);
  return v;
}

// After jumping through hoops in an earlier implementation, I've asked jipiti
// for a more compact way to generate a bitmask given only the index
// ("button[press]") and the number of positions `n` in the bitmask.
// This can be done with `1 << (n - 1 - b)` (where b is the value of 1 light
// index in the button.
//
// ORIGINAL_input_parse_button2bitmask() is here to remind
// me of the naive, but working, method I cooked up myself.
int input_parse_button2bitmask(char *s, int n) {
  s++;
  s[strlen(s) - 1] = '\0';
  char **parts;
  int size;
  parts = sstr_split(s, ",", (size_t *)&size);
  int v = 0;
  for (int i = 0; i < size; i++) {
    v |= 1 << (n - 1 - atoi(parts[i]));
  }
  sstr_list_free(parts, size);
  return v;
}

void input_parse_joltages(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  int size;
  char **parts = sstr_split(s, ",", (size_t *)&size);
  for (int i = 0; i < size; i++) {
    m->joltages[m->J++] = atoi(parts[i]);
  }
  sstr_list_free(parts, size);
}

void input() {
  char *line;
  while (NULL != (line = readline(stdin))) {
    int size;
    char **parts = sstr_split(line, " ", (size_t *)&size);
    int size_l = input_parse_lights(&machines[M], parts[0]);
    input_parse_joltages(&machines[M], parts[size - 1]);
    int bp_i = 0;
    for (int i = 1; i < size - 1; i++)
      machines[M].buttons[bp_i++] =
          input_parse_button2bitmask(parts[i], size_l);
    sstr_list_free(parts, size);
    free(line);
    M++;
  }
}

// Not really needed for the solution, but it came in handy, once in a while
// causing much confusion when data structures changed :-)
void print_machine(Machine m) {
  printf("[%d] (", m.lights);
  for (int i = 0; i< m.B; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.buttons[i]);
  }
  printf(") ");

  printf(" {");
  for (int i = 0; i < m.J; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.joltages[i]);
  }
  printf("}\n");
}

// For finding the minimum amount of button presses we can use BFS:
//   - Nodes: All the possible states of the lights (2^n)-1
//   - Start: 0 (all lights off)
//   - Edge: XOR with button (cost: 1)
//   - Target: lights as given in the input of the list of machines.
// Return -1 if the target couldn't be reached.
int min_presses(Machine m) {
  int max_states = 1 << m.L; // 2^n

  // Distance: number of presses to reach a certain lights state.
  // The index will be the light state, the value will be the number of presses.
  int *dist = malloc(max_states * sizeof(int));
  for (int i = 0; i < max_states; i++)
    dist[i] = INT_MAX;
  int *queue = malloc(max_states * sizeof(int));
  int head = 0, tail = 0;
  dist[0] = 0;
  queue[tail++] = 0;
  while (head < tail) {
    int state = queue[head++];
    if (state == m.lights)
      break;
    for (int i = 0; m.buttons[i]; i++) {
      int next = state ^ m.buttons[i];
      if (dist[next] == INT_MAX) {
        dist[next] = dist[state] + 1;
        queue[tail++] = next;
      }
    }
  }
  int result = dist[m.lights];
  free(dist);
  free(queue);
  return (result == INT_MAX) ? -1 : result;
}

/*

--[ Part 1]--
General idea: convert to bit masks (part 1 ignores joltage part).

  [.##.] - (3) (1,3) (2) (2,3) (0,2) (0,1)

becomes (bit representation):
  [0110] - (0001) (0101) (0010) (0011) (1010) (1100)
which is in ints:
     [6] -   1      5      2      3      10     12

A "button press" is an XOR operation.
-> Pressing a button twice is useless (it goes back to original state)
-> XOR is commutative... pressing (1) and then (1, 3) is the same as pressing
   (1, 3) and then (1)
-> Not too much memory needed, since number of lights will not exceed 10
   2^10 -> 1024

We then find the minimum amount of presses necessary to arrive at the expected
state [.##.], ie. [0110].
The lights start all off, so we literally start from 0.
*/
int main() {
  input();
  int p1 = 0;
  for (int m = 0; m < M; m++) {
    p1 += min_presses(machines[m]);
  }
  printf("%d\n", p1);
  return 0;
}
