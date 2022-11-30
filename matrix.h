# ifndef MATRIX_H
# define MATRIX_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <math.h>
# include "object.h"
# include "vector.h" /* We want to enable matrix times vector */

/*** Matrix object definition ***/
struct matrix {
  const struct abstract_object _; /* This item must come first */
  int rows, cols; /* Dimensionality */
  real * dat;
};

static void * matrix_constructor(void * _self, va_list * args);
static void * matrix_destructor(void * _self);
static void * matrix_clone(const void * _self);

static const Class _matrix
  = {sizeof(struct matrix), "matrix", &_abstract_object,
     matrix_constructor, matrix_destructor};

const void * matrix = &_matrix;

/*** Matrix operations ***/
void matrix_set_dim(void * _self, int rows, int cols);
void * vector_to_matrix(void * _v);
void matrix_print(const void * _self, FILE * fp);
void * matrix_add(const void * _A, const void * _B);
void * matrix_subtract(const void * _A, const void * _B);
void * matrix_prod(const real lambda, const void * _M);
void * matrix_dot(const void * _A, const void * _B);
void * matrix_transpose(const void * _A);


/*** Function definitions ***/
static void * matrix_constructor(void * _self, va_list * args)
{
  struct abstract_object * obj =  abstract_object_constructor(_self, args);
  obj->clone = matrix_clone;
  struct matrix * self = _self;
  self->rows = 0;
  self->cols = 0;
  self->dat = NULL;
  return _self;
}

static void * matrix_destructor(void * _self)
{
  struct matrix * self = _self;
  free(self->dat);
  return NULL;
}

/* Special version of clone for copying vectors */
static void * matrix_clone(const void * _self)
{
  const struct matrix * self = _self;
  if(inherits_from(self, matrix)) {
    new(A, matrix);
    matrix_set_dim(A, self->rows, self->cols);
    for(int i = 0; i < self->rows*self->cols; ++i) A->dat[i] = self->dat[i];
    return A;
  }
  return NULL;
}

/* Set dimensions of matrix and allocate memory */
void matrix_set_dim(void * _self, int rows, int cols)
{
  struct matrix * self = _self;
  if(inherits_from(self, matrix)) {
    self->rows = rows;
    self->cols = cols;
    int dim = rows*cols;
    if(self->dat == NULL)
      self->dat = (real *) calloc(dim, sizeof(real));
    else
      self->dat = (real *) realloc(self, dim);
  }

  return;
}

/* Convert vector to matrix */
void * vector_to_matrix(void * _v)
{
  struct vector * v = _v;
  if(inherits_from(v, vector)) {
    new(M, matrix);
    matrix_set_dim(M, v->dim, 1);
    for(int i = 0; i < v->dim; ++i) M->dat[i] = v->dat[i];
    return M;
  }

  return NULL;
}

/* Formatted print of matrix elements */
void matrix_print(const void * _self, FILE * fp)
{
  const struct matrix * self = _self;
  if(inherits_from(self, matrix) && self->rows > 0 && self->cols > 0) {
    for(int i = 0; i < self->rows; ++i) {
      fprintf(fp, "  [ ");
      for(int j = 0; j < self->cols; ++j)
        fprintf(fp, " % 1.2e ", self->dat[self->cols*i + j]);
      fprintf(fp, " ]\n");
    }
  } else fprintf(fp, "[]");
  return;
}

/* Matrix addition */
void * matrix_add(const void * _A, const void * _B)
{
  const struct matrix * A = _A;
  const struct matrix * B = _B;
  if(inherits_from(A, matrix) && inherits_from(B, matrix)
     && A->rows == B->rows && A->cols == B->cols) {
     new(M, matrix);
     matrix_set_dim(M, A->rows, A->cols);
     for(int i = 0; i < M->rows*M->cols; ++i)
       M->dat[i] = A->dat[i] + B->dat[i];
     return M;
  }
  return NULL;
}

/* Matrix subtraction */
void * matrix_subtract(const void * _A, const void * _B)
{
  const struct matrix * A = _A;
  const struct matrix * B = _B;
  if(inherits_from(A, matrix) && inherits_from(B, matrix)
     && A->rows == B->rows && A->cols == B->cols) {
     new(M, matrix);
     matrix_set_dim(M, A->rows, A->cols);
     for(int i = 0; i < M->rows*M->cols; ++i)
       M->dat[i] = A->dat[i] - B->dat[i];
     return M;
  }
  return NULL;
}

/* Scalar times matrix */
void * matrix_prod(const real lambda, const void * _A)
{
  const struct matrix * A = _A;
  if(inherits_from(A, matrix)) {
    new(M, matrix);
    matrix_set_dim(M, A->rows, A->cols);
    for(int i = 0; i < M->rows*M->cols; ++i)
      M->dat[i] = lambda*A->dat[i];
    return M;
  }
  return NULL;
}

/* Matrix product (it also allows matrix times vector) */
void * matrix_dot(const void * _A, const void * _B)
{
  const struct matrix * A = _A;
  const struct matrix * B = _B;
  const struct vector * u = _B;

  if(inherits_from(A, matrix) && inherits_from(B, matrix)
     && A->cols == B->rows) {
    new(M, matrix);
    matrix_set_dim(M, A->rows, B->cols);
    for(int i = 0; i < M->rows; ++i)
      for(int j = 0; j < M->cols; ++j)
        for(int k = 0; k < A->cols; ++k)
          M->dat[M->cols*i + j] += A->dat[A->cols*i + k]*B->dat[B->cols*k + j];
    return M;
  } else if (inherits_from(A, matrix) && inherits_from(u, vector)
             && A->cols == u->dim) {
    new(v, vector);
    vector_set_dim(v, A->rows);
    for(int i = 0; i < v->dim; ++i)
      for(int j = 0; j < A->cols; ++j)
        v->dat[i] += A->dat[A->cols*i + j]*u->dat[j];
    return v;
  }

  return NULL;
}

/* Matrix transpose */
void * matrix_transpose(const void * _A)
{
  const struct matrix * A = _A;
  if(inherits_from(A, matrix)) {
    new(M, matrix);
    matrix_set_dim(M, A->cols, A->rows);
    for(int i = 0; i < M->rows; ++i)
      for(int j = 0; j < M->cols; ++j)
        M->dat[M->cols*i + j] = A->dat[A->cols*j + i];
    return M;
  }
  return NULL;
}
# endif
