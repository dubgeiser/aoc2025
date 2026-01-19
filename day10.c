/*
--[ Part 1 ]--
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


--[ Part 2 ]--
First idea:
Basically the same as Part 1, but the buttons' bitmask/binary representation
Should be literally treated as decimal:
Example:

  [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}

  (3)     -> 0001 ->   1
  (1,3)   -> 0101 -> 101
  (2)     -> 0010 ->  10
  (2,3)   -> 0011 ->  11
  ...

So, I _think_ we can parse the buttons upon reading, and then use BFS again, but
instead of XOR'ing, we add until the target is reached.  With a funcpointer, I'm
guessing that the same `min_presses` func can be used, since buttons and target
are stored as ints in both part 1 and 2...

BUT!!!
Going through the input, the numbers formed by the joltages are too big:
For instance: `52,34,38,25,39,25,39,39,47,30`
  -> 52343825392539394730
While the max value of an uint64 is:
  -> 18446744073709551615

The largest joltage-as-int in the input is:
  -> 3919220618619316202220202187
Which is not feasible to store without losing precision, unless an external
library like gmp wold be used.
And I don't want to that.

Also counting with the buttons as (maximum) 1111111111 would take a considerable
amount of time.
Back to the drawing table...

Maybe just "naively" counting as the puzzle tells us, might be feasible...
Most likely, we're then better of copy-pasting the `min_presses` instead of
trying to reuse it. Because, we can't just use the joltages as a key out of the
box.  But I'm guessing brute forcing it might take up a lot of time...
*/

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
  int bbitmasks[MAX_BUTTONS];
  int BB;
  int joltages[MAX_JOLTAGES];
  int J;
} Machine;

Machine machines[MAX_MACHINES];
int M = 0;

/**
 * Parse the lights-part of the input `s` into machine 'm'
 * Return the number of places in lights, fi: `"[.##.]"` will return 4.
 * This number is also stored in the m.L, it is needed throughout our solution.
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
// This can be done with `1 << (n - 1 - b)` (where b is the value of the light
// index manipulated by the button.
//
// ORIGINAL_input_parse_button2bitmask() is here to remind
// me of the naive, but working, method I cooked up myself.
void input_parse_button2bitmask(Machine *m, char *s) {
  s++;
  s[strlen(s) - 1] = '\0';
  char **parts;
  int size;
  parts = sstr_split(s, ",", (size_t *)&size);
  int v = 0;
  for (int i = 0; i < size; i++) {
    v |= 1 << (m->L - 1 - atoi(parts[i]));
  }
  sstr_list_free(parts, size);
  m->bbitmasks[m->BB++] = v;
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
    input_parse_lights(&machines[M], parts[0]);
    input_parse_joltages(&machines[M], parts[size - 1]);
    for (int i = 1; i < size - 1; i++)
          input_parse_button2bitmask(&machines[M], parts[i]);
    sstr_list_free(parts, size);
    free(line);
    M++;
  }
}

// Not really needed for the solution, but it came in handy, once in a while
// causing much confusion when data structures changed :-)
void print_machine(Machine m) {
  printf("[%d] (", m.lights);
  for (int i = 0; i < m.BB; i++) {
    if (i > 0)
      printf(",");
    printf("%d", m.bbitmasks[i]);
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
    for (int i = 0; i < m.BB; i++) {
      int next = state ^ m.bbitmasks[i];
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

int main() {
  input();
  int p1 = 0;
  for (int m = 0; m < M; m++) {
    print_machine(machines[m]);
    p1 += min_presses(machines[m]);
  }
  printf("%d\n", p1);
  return 0;
}
