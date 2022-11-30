# include <stdio.h>
# include "../vector.h"
# include "../matrix.h"

# define init_object(obj) void * obj = NULL;
# define set_object(obj, value) delete(obj); obj = value;

int main()
{
  /* Matrix [v] from vector v */
  new(v, vector);
  vector_set_dim(v, 3);
  v->dat[0] = 0.0; v->dat[1] = 1.0; v->dat[2] = 2.0;
  printf("v = "); vector_print(v, stdout); printf("\n");
  Object mv = vector_to_matrix(v);
  printf("[v] = \n"); matrix_print(mv, stdout);

  /* Matrix M */
  new(M, matrix);
  matrix_set_dim(M, 3, 3);
  for(int i = 0; i < M->rows; ++i)
    for(int j = 0; j < M->cols; ++j)
      M->dat[M->cols*i + j] = i + j;
  printf("M = \n"); matrix_print(M, stdout);

  /* Matrix A = 3I */
  new(A, matrix);
  matrix_set_dim(A, 3, 3);
  for(int i = 0; i < 3; ++i) A->dat[3*i+i] = 1.0;
  A = matrix_prod(3, A);
  printf("A = \n"); matrix_print(A, stdout);

  /* Dummy matrix B */
  init_object(B);

  /* More operations */
  printf("[v]^T = ");
  set_object(B, matrix_transpose(mv)); matrix_print(B, stdout);
  printf("M + A = \n");
  set_object(B, matrix_add(M, A)); matrix_print(B, stdout);
  printf("M - A = \n");
  set_object(B, matrix_subtract(M, A)); matrix_print(B, stdout);
  printf("M [v] = \n");
  set_object(B, matrix_dot(M, mv)); matrix_print(B, stdout);
  printf("M v = ");
  set_object(B, matrix_dot(M, v)); vector_print(B, stdout); printf("\n");
  delete(B);

  /* Clean up */
  delete(v);
  delete(mv);
  delete(A);

  return 0;
}
