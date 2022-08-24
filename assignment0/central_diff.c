#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// ===================
//       MACROS
// ===================
#define DELTA_X 1
#define N 10

typedef unsigned short u8;

// ===============
//    Functions
// ===============

int squared(u8 i) { return i * i; }

int derivative(u8 i) {
  int top = squared(i + DELTA_X) - squared(i - DELTA_X);
  int bot = 2 * DELTA_X;

  return top / bot;
}

// ====================
//       Helpers
// ====================

void generate_array(int *array, int (*f)(u8), const size_t start,
                    const size_t end) {
  for (size_t i = start; i <= end; i++) {
    array[i] = f(i);
  }
}

void print_array(int *array, size_t start, size_t end) {
  for (size_t i = start; i <= end; i++)
    printf("%d ", array[i]);
}

int main(int argc, char **argv) {
  printf("Hello world! This is an integer: %d\n", N);

  // Arrays storing x and y values, each of same size
  int *ys = (int *)calloc(N + 1, sizeof(int));
  int *derivatives = (int *)calloc(N + 1, sizeof(int));

  // Generate arrays

  // What I'm doing here is just setting the values according
  // to which hould be filled in, meaning derivatives[0] and derivatives[N]
  // should still be 0. I did this to easily be able to use func pointers in the
  // generate array, and having an easier time with what should be calculated
  generate_array(ys, squared, 0, N);
  generate_array(derivatives, derivative, 1, N - 1);

  printf("Sqaured Array\n");
  print_array(ys, 0, N);

  printf("\n");

  printf("Derivative Array\n");
  print_array(derivatives, 1, N - 1);

  free(ys);
  free(derivatives);

  return 0;
}
