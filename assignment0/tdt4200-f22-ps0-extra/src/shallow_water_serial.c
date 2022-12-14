#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../inc/argument_utils.h"

#define BUFSIZE 256
#define DOMAIN_LOOP(x, n) for (x = 0; x < n + 2; x++)

typedef int64_t int_t;
typedef double real_t;

int_t N, max_iteration, snapshot_frequency;

const real_t domain_size = 10.0, gravity = 9.81, density = 997.0;

real_t *mass[2] = {NULL, NULL}, *mass_velocity_x[2] = {NULL, NULL},
       *velocity_x = NULL, *acceleration_x = NULL, dx, dt;

#define PN(x) mass[0][(x)]
#define PN_next(x) mass[1][(x)]
#define PNU(x) mass_velocity_x[0][(x)]
#define PNU_next(x) mass_velocity_x[1][(x)]
#define U(x) velocity_x[(x)]
#define DU(x) acceleration_x[(x)]

void domain_save(int_t iteration);

// TODO 4: Implement the swap function to swap the content of two variables
void swap(real_t **m1, real_t **m2) {
  real_t **temp = m1;
  m1 = m2;
  m2 = temp;
}

int main(int argc, char **argv) {
  OPTIONS *options = parse_args(argc, argv);

  if (!options) {
    fprintf(stderr, "Argument parsing failed\n");
    exit(1);
  }

  // TODO 1: Get N, max_iteration and snapshot_frequency from the options struct
  // and store the values in the global fields with the same names
  N = options->N;
  max_iteration = options->max_iteration;
  snapshot_frequency = options->snapshot_frequency;
  printf("N: %ld\n", N);
  printf("Max iter: %ld\n", max_iteration);
  printf("snapshot_frequency: %ld\n", snapshot_frequency);

  // TODO 2: Allocate memory for the mass, velocity and acceleration arrays.
  // There should be space for N+2 elements of the real_t type in each of the
  // arrays. The arrays should also be initialized to be filled with zeroes.

  mass[0] = (real_t *)calloc(N + 2, sizeof(real_t));
  mass[1] = (real_t *)calloc(N + 2, sizeof(real_t));

  mass_velocity_x[0] = (real_t *)calloc(N + 2, sizeof(real_t));
  mass_velocity_x[1] = (real_t *)calloc(N + 2, sizeof(real_t));

  velocity_x = (real_t *)calloc(N + 2, sizeof(real_t));
  acceleration_x = (real_t *)calloc(N + 2, sizeof(real_t));

  // Data initialization
  for (int_t x = 1; x <= N; x++) {
    PN(x) = 1e-3;
    PNU(x) = 0.0;

    real_t c = x - N / 2;
    if (sqrt(c * c) < N / 20.0) {
      PN(x) -= 5e-4 * exp(-4 * pow(c, 2.0) / (real_t)(N));
    }

    PN(x) *= density;
  }

  dx = domain_size / (real_t)N;
  dt = 0.1 * dx;

  for (int_t i = 0; i <= max_iteration; i++) {
    //  TODO 3a: Update the edges of the domain based on the boundary conditions
    //  for PN and PNU.
    PN(N + 1) = PN(N - 1);    // (7)
    PN(0) = PN(2);            // (8)
    PNU(0) = -PNU(2);         // (9)
    PNU(N + 1) = -PNU(N - 1); // (10)

    int_t x;

    // TODO 3b: Update the acceleration over the entire domain and the borders
    DOMAIN_LOOP(x, N) {
      DU(x) = PN(x) * pow(U(x), 2.0) +
              0.5 * gravity * (pow(PN(x), 2.0) / density); // (5)
    }

    // TODO 3c: Update the next PNU over the entire domain
    DOMAIN_LOOP(x, N) {
      PNU_next(x) = 0.5 * (PNU(x + 1) + PNU(x - 1)) -
                    dt / 2 * dx * (DU(x + 1) - DU(x - 1));
    }

    // TODO 3d: Update the next PN over the entire domain
    DOMAIN_LOOP(x, N) {
      PN_next(x) = 0.5 * (PN(x + 1) + PN(x - 1)) -
                   dt / 2 * dx * (PNU(x + 1) - PNU(x - 1));
    }

    // TODO 3e: Update the U over the entire domain
    DOMAIN_LOOP(x, N) {
      U(x) = PNU(x) / PN(x); // (6)
    }

    if (i % snapshot_frequency == 0) {
      printf("Iteration %ld of %ld (%.2lf%% complete)\n", i, max_iteration,
             100.0 * (real_t)i / (real_t)max_iteration);

      domain_save(i);
    }

    // TODO 4: Implement  the swap function
    swap(&mass[0], &mass[1]);
    swap(&mass_velocity_x[0], &mass_velocity_x[1]);
  }

  // TODO 5: Free the heap-allocated memory
  free(options);
  free(mass[0]);
  free(mass[1]);
  free(mass_velocity_x[0]);
  free(mass_velocity_x[1]);
  free(velocity_x);
  free(acceleration_x);

  exit(EXIT_SUCCESS);
}

void domain_save(int_t iteration) {
  int_t index = iteration / snapshot_frequency;
  char filename[BUFSIZE];
  memset(filename, 0, BUFSIZE * sizeof(char));
  sprintf(filename, "data/%.5ld.bin", index);

  FILE *out = fopen(filename, "wb");
  if (!out) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    exit(1);
  }
  fwrite(&PN(1), sizeof(real_t), N, out);
  fclose(out);
}
