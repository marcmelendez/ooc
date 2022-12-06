# ifndef VECTOR_H
# define VECTOR_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <math.h>
# include "object.h"

# ifndef REAL
# define REAL DOUBLE
# define real double
# define real_val(x) x
# endif

struct vector {
  const struct abstract_object _; /* This item must come first */
  int dim; /* Dimensionality */
  real * dat;
};

static void * vector_constructor(void * _self, va_list * args);
static void * vector_destructor(void * _self);
static void * vector_clone(const void * _self);
static void * vector_display(const void * _self, FILE * fp);

static const Class _vector
  = {sizeof(struct vector), "vector", &_abstract_object,
     vector_constructor, vector_destructor};

const void * vector = &_vector;

/*** Vector operations ***/

void vector_set_dim(void * _self, int dim);
void vector_print(const void * _self, FILE * fp);
void * vector_add(const void * _v, const void * _w);
void * vector_subtract(const void * _v, const void * _w);
void * vector_prod(const real lambda, const void * _v);
real vector_dot(const void * _v, const void * _w);
real vector_norm(const void * _v);

static void * vector_constructor(void * _self, va_list * args)
{
  struct abstract_object * obj =  abstract_object_constructor(_self, args);
  obj->clone = vector_clone;
  obj->display = vector_display;
  struct vector * self = _self;
  self->dim = 0;
  self->dat = NULL;
  return _self;
}

static void * vector_destructor(void * _self)
{
  struct vector * self = _self;
  free(self->dat);
  return NULL;
}

/* Special version of clone for copying vectors */
static void * vector_clone(const void * _self)
{
  if(inherits_from(_self, vector)) {
    const struct vector * self = _self;
    new(w, vector);
    vector_set_dim(w, self->dim);
    for(int i = 0; i < self->dim; ++i) w->dat[i] = self->dat[i];
    return w;
  }
  return NULL;
}

/* Displaying vectors */
static void * vector_display(const void * _self, FILE * fp)
{
  abstract_object_display(_self, fp);

  if(inherits_from(_self, vector)) {
    const struct vector * self = _self;
    fprintf(fp, "dim: %d\n", self->dim);
  }
  return NULL;
}


/* Set vector dimensionality */
void vector_set_dim(void * _self, int dim)
{
  struct vector * self = _self;
  if(inherits_from(self, vector)) {
    self->dim = dim;
    if(self->dat == NULL)
      self->dat = (real *) calloc(dim, sizeof(real));
    else
      self->dat = (real *) realloc(self, dim);
  }

  return;
}

/* Display a vector */
void vector_print(const void * _self, FILE * fp)
{
  const struct vector * self = _self;
  if(inherits_from(self, vector) && self->dim > 0) {
    fprintf(fp, "(%f", self->dat[0]);
    for(int i = 1; i < self->dim; ++i)
      fprintf(fp, ", %f", self->dat[i]);
    fprintf(fp, ")");
  } else fprintf(fp, "()");

  return;
}

/* Add and subtract vectors */
void * vector_add(const void * _v, const void * _w)
{
  const struct vector * v = _v;
  const struct vector * w = _w;
  struct vector * uminus = (struct vector *) v;
  struct vector * uplus = (struct vector *) w;
  if(v->dim > w->dim) {
    uminus = (struct vector *) w;
    uplus = (struct vector *) v;
  }

  if(inherits_from(v, vector) && inherits_from(w, vector)) {
    new(vplusw, vector);
    vector_set_dim(vplusw, uplus->dim);
    for(int i = 0; i < uminus->dim; ++i)
      vplusw->dat[i] = uminus->dat[i] + uplus->dat[i];
    for(int i = uminus->dim + 1; i < uplus->dim; ++i)
      vplusw->dat[i] = uplus->dat[i];
    return vplusw;
  }
  return NULL;
}

void * vector_subtract(const void * _v, const void * _w)
{
  const struct vector * v = _v;
  const struct vector * w = _w;
  struct vector * uminus = (struct vector *) v;
  struct vector * uplus = (struct vector *) w;
  if(v->dim > w->dim) {
    uminus = (struct vector *) w;
    uplus = (struct vector *) v;
  }

  if(inherits_from(v, vector) && inherits_from(w, vector)) {
    new(vminusw, vector);
    vector_set_dim(vminusw, uplus->dim);
    for(int i = 0; i < uminus->dim; ++i)
      vminusw->dat[i] = uminus->dat[i] + uplus->dat[i];
    for(int i = uminus->dim + 1; i < uplus->dim; ++i)
      vminusw->dat[i] = uplus->dat[i];
    return vminusw;
  }
  return NULL;
}

/* Real number times a vector */
void * vector_prod(const real lambda, const void * _v)
{
  const struct vector * v = _v;
  if(inherits_from(v, vector)) {
    new(lambda_v, vector);
    vector_set_dim(lambda_v, v->dim);
    for(int i = 0; i < lambda_v->dim; ++i)
      lambda_v->dat[i] = lambda*v->dat[i];
    return lambda_v;
  }
  return NULL;
}

/* Dot product and norm of a vector */
real vector_dot(const void * _v, const void * _w)
{
  const struct vector * v = _v;
  const struct vector * w = _w;
  real dotproduct = 0;
  if(inherits_from(v, vector) && inherits_from(w, vector)) {
    for(int i = 0; i < fmax(v->dim, w->dim); ++i)
      dotproduct += v->dat[i]*w->dat[i];
  }
  return dotproduct;
}

real vector_norm(const void * _v)
{
  const struct vector * v = _v;
  if(inherits_from(v, vector)) return sqrt(vector_dot(v, v));
  return real_val(0.0);
}

/* Cross product */
void * vector_cross(const void * _v, const void * _w)
{
  const struct vector * v = _v;
  const struct vector * w = _w;
  new(cross, vector);
  vector_set_dim(cross, 3);
  if(inherits_from(v, vector) && inherits_from(w, vector)) {
    real a[3] = {0}; for(int i = 0; i < v->dim; ++i) a[i] = v->dat[i];
    real b[3] = {0}; for(int i = 0; i < w->dim; ++i) b[i] = w->dat[i];
    cross->dat[0] = a[1]*b[2] - a[2]*b[1];
    cross->dat[1] = a[2]*b[0] - a[0]*b[2];
    cross->dat[2] = a[0]*b[1] - a[1]*b[0];
  }
  return cross;
}
# endif
