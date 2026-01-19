#include "lib/input.c"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOXES 1000
#define DISTANCE_SAMPLE 1000

typedef struct {
  int x;
  int y;
  int z;
} Box;

typedef struct {
  int b1;
  int b2;
  int64_t d;
} Distance;

typedef struct {
  unsigned int boxes[MAX_BOXES];
  size_t size;
} Circuit;

// Define globally and manipulate via their indices.
Circuit circuits[DISTANCE_SAMPLE];
int C = 0;
Box boxes[MAX_BOXES];
int B = 0;

void circuit_init(int idx) {
  for (int i = 0; i < MAX_BOXES; i++) {
    circuits[idx].boxes[i] = 0;
  }
  circuits[idx].size = 0;
}

void circuit_add(int idx, int n) {
  if (!circuits[idx].boxes[n]) {
    circuits[idx].boxes[n] = 1;
    circuits[idx].size++;
  }
}

int circuit_contains(int idx, int n) { return circuits[idx].boxes[n]; }

void circuit_remove(int idx, int n) {
  if (circuits[idx].boxes[n]) {
    circuits[idx].boxes[n] = 0;
    circuits[idx].size--;
  }
}

void circuits_update(int b1, int b2) {
  // -1 Means that the corresponding box does not yet belong to any circuit.
  int ci_b1 = -1;
  int ci_b2 = -1;

  for (int i = 0; i < C; i++) {
    if (circuit_contains(i, b1)) {
      ci_b1 = i;
    }
    if (circuit_contains(i, b2)) {
      ci_b2 = i;
    }
  }
  if (ci_b1 > -1 && ci_b1 == ci_b2) {
    return;
  } else if (ci_b1 == ci_b2) {
    for (int i = 0; i < C; i++) {
      if (circuits[i].size == 0) {
        circuit_add(i, b1);
        circuit_add(i, b2);
        return;
      }
    }
  } else if (ci_b1 == -1) {
    circuit_add(ci_b2, b1);
  } else if (ci_b2 == -1) {
    circuit_add(ci_b1, b2);
  } else {
    for (int i = 0; i<MAX_BOXES; i++) {
      if (circuit_contains(ci_b2, i)) {
        circuit_add(ci_b1, i);
      }
    }
    circuit_init(ci_b2);
  }
}

int64_t distance(Box *b1, Box *b2) {
  return sqrtl(powl(b1->x - b2->x, 2) + powl(b1->y - b2->y, 2) +
               powl(b1->z - b2->z, 2));
}

int cmpdistances(const void *a, const void *b) {
  Distance *da = (Distance *)a;
  Distance *db = (Distance *)b;
  if (da->d < db->d)
    return -1;
  if (da->d > db->d)
    return 1;
  return 0;
}

int cmpcircuits(const void *a, const void *b) {
  Circuit *ca = (Circuit *)a;
  Circuit *cb = (Circuit *)b;
  if (ca->size < cb->size)
    return -1;
  if (ca->size > cb->size)
    return 1;
  return 0;
}

void printdistance(Distance d) {
  printf("(%d, %d, %d) -> (%d, %d, %d): %ld\n", boxes[d.b1].x, boxes[d.b1].y,
         boxes[d.b1].z, boxes[d.b2].x, boxes[d.b2].y, boxes[d.b2].z, d.d);
}

int main() {
  int p1;
  char *line;

  // Takes too much memory to reserve an array on the stack.
  // Should use a heap to only store the shortest MAX_BOXES distances.
  Distance *distances =
      malloc(sizeof(Distance) * (pow(MAX_BOXES, 2) / 2 - MAX_BOXES / 2));

  int D = 0;
  while (NULL != (line = readline(stdin))) {
    sscanf(line, "%d,%d,%d", &boxes[B].x, &boxes[B].y, &boxes[B].z);
    free(line);
    B++;
  }

  for (int i = 0; i < B - 1; i++) {
    for (int j = i + 1; j < B; j++) {
      distances[D].b1 = i;
      distances[D].b2 = j;
      distances[D].d = distance(&boxes[i], &boxes[j]);
      D++;
    }
  }
  qsort(distances, D, sizeof(Distance), cmpdistances);

  for (int i = 0; i < DISTANCE_SAMPLE; i++) {
    circuit_init(i);
    C++;
  }
  for (int i = 0; i < DISTANCE_SAMPLE; i++) {
    circuits_update(distances[i].b1, distances[i].b2);
  }
  qsort(circuits, C, sizeof(Circuit), cmpcircuits);
  free(distances);
  p1 = circuits[C - 1].size * circuits[C - 2].size * circuits[C - 3].size;
  printf("%d\n", p1);
  return 0;
}
