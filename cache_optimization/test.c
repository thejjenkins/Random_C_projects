/**
 * Code copied from Magicbat on youtube: https://youtu.be/XSAIgL4bweE?si=23Pp3Z1YeN0Nvxs3
 * Demonstrates the effect of cache optimization by changing the order of nested loops
 * On windows, I used "gcc test.c -o test -O3" to compile where -O3 enables optimizations
 * Then he used "Measure-Command { .\test.exe }" in powershell to measure execution time
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t u32;
typedef float f32;

typedef struct {
  u32 rows, cols;

  // row-major
  f32* data;
} matrix;

// ~1000ms
f32 mat_sum0(const matrix* m) {
  f32 sum = 0.0f;

  for(u32 r = 0; r < m->rows; r++) {
    for(u32 c = 0; c < m->cols; c++) {
      sum += m->data[r * m->cols + c];
    }
  }
  return sum;
}

// ~3000ms
f32 mat_sum1(const matrix* m) {
  f32 sum = 0.0f;
  for(u32 c = 0; c < m->cols; c++) {
    for(u32 r = 0; r < m->rows; r++) {
      sum += m->data[r * m->cols + c];
    }
  }
  return sum;
}

/*
i, j, k -> ~3100ms
i, k, j -> ~580ms
j, k, i -> ~8200ms
*/
void mat_mul(matrix* out, const matrix* a, const matrix* b) {
  out->rows = a->rows;
  out->cols = b->cols;

  memset(out->data, 0, sizeof(f32) * out->rows * out->cols);

  for(u32 i = 0; i < out->rows; i++) {  
    for(u32 k = 0; k < a->cols; k++) {
      for(u32 j = 0; j < out->cols; j++) {
        out->data[j + i * out->cols] += 
          a->data[k + i * a->cols] *
          b->data[j + k * b->cols];
      }
    }
  }
}

#define SIZE 1024

int main(void) {
  matrix a = {SIZE, SIZE};
  a.data = (f32*)malloc(sizeof(f32) * SIZE * SIZE);
  matrix b = {SIZE, SIZE};
  b.data = (f32*)malloc(sizeof(f32) * SIZE * SIZE);
  matrix c = {SIZE, SIZE};
  c.data = (f32*)malloc(sizeof(f32) * SIZE * SIZE);

  for(u32 i = 0; i < SIZE * SIZE; i++) {
    a.data[i] = (f32)rand() / (f32)RAND_MAX;
    b.data[i] = (f32)rand() / (f32)RAND_MAX;
  }

  mat_mul(&c, &a, &b);

  f32 s = mat_sum0(&c);
  printf("%f\n", s);

  free(a.data);
  free(b.data);
  free(c.data);
  return 0;
}