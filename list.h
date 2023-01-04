# ifndef LIST_H
# define LIST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

struct int_list { int n; int * element; int size; };
struct float_list { int n; float * element; int size; };
struct double_list { int n; double * element; int size; };
struct char_list { int n; char * element; int size; };
struct string_list { int n; char ** element; int size; };
struct obj_list { int n; void ** element; int size; };

# define list_init(list, size) \
  _Generic((list), \
            struct int_list *: int_list_alloc, \
            struct float_list *: float_list_alloc, \
            struct double_list *: double_list_alloc, \
            struct char_list *: char_list_alloc, \
            struct string_list *: string_list_alloc, \
            struct obj_list *: obj_list_alloc, \
            default: null_function)(list, size)

void int_list_alloc(struct int_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(int)); return;}

void float_list_alloc(struct float_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(float)); return;}

void double_list_alloc(struct double_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(double)); return;}

void char_list_alloc(struct char_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(char)); return;}

void string_list_alloc(struct string_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(char *)); return;}

void obj_list_alloc(struct string_list * list, int n)
{list->size = list->n = n; list->element = calloc(n, sizeof(void *)); return;}

void null_function(void * x, ...) {return;}

# define list_set(list, i, val) \
  _Generic((list), \
    struct int_list *: int_list_set, \
    struct float_list *: float_list_set, \
    struct double_list *: double_list_set, \
    struct char_list *: char_list_set, \
    struct string_list *: string_list_set, \
    struct obj_list *: obj_list_set, \
    default: null_function)(list, i, val)

int int_list_set(struct int_list * list, int i, int val)
{return list->element[i] = val;}

float float_list_set(struct float_list * list, int i, float val)
{return list->element[i] = val;}

double double_list_set(struct double_list * list, int i, double val)
{return list->element[i] = val;}

char char_list_set(struct char_list * list, int i, char val)
{return list->element[i] = val;}

char * string_list_set(struct string_list * list, int i, char * val)
{return list->element[i] = val;}

void * obj_list_set(struct obj_list * list, int i, void * val)
{return list->element[i] = val;}

# define list_read(list, i) \
  _Generic((list), \
            struct int_list *: int_read, \
            struct float_list *: float_read, \
            struct double_list *: double_read, \
            struct char_list *: char_read, \
            struct string_list *: string_read, \
            struct obj_list *: obj_read, \
            default: null_function)(list, i)

int int_read(struct int_list * list, int i) {return list->element[i];}
float float_read(struct float_list * list, int i) {return list->element[i];}
double double_read(struct double_list * list, int i) {return list->element[i];}
char char_read(struct char_list * list, int i) {return list->element[i];}
char * string_read(struct string_list * list, int i) {return list->element[i];}
void * obj_read(struct obj_list * list, int i) {return list->element[i];}

# define list_pop(list, i) \
  _Generic((list), \
            struct int_list *: int_list_pop, \
            struct float_list *: float_list_pop, \
            struct double_list *: double_list_pop, \
            struct char_list *: char_list_pop, \
            struct string_list *: string_list_pop, \
            struct obj_list *: obj_list_pop, \
            default: null_function)(list, i)

int int_list_pop(struct int_list * list, int i)
{
  int val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

float float_list_pop(struct float_list * list, int i)
{
  float val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

double double_list_pop(struct double_list * list, int i)
{
  double val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

char char_list_pop(struct char_list * list, int i)
{
  char val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

char * string_list_pop(struct string_list * list, int i)
{
  char * val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

void * obj_list_pop(struct obj_list * list, int i)
{
  void * val = list->element[i];
  for(int n = i; n < list->n - 1; ++n) list->element[n] = list->element[n + 1];
  (list->n)--;
  return val;
}

# define list_push(list, i, val) \
  _Generic((list), \
            struct int_list *: int_list_push, \
            struct float_list *: float_list_push, \
            struct double_list *: double_list_push, \
            struct char_list *: char_list_push, \
            struct string_list *: string_list_push, \
            struct obj_list *: obj_list_push, \
            default: null_function)(list, i, val)

int int_list_push(struct int_list * list, int i, int val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(int));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

float float_list_push(struct float_list * list, int i, float val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(float));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

float double_list_push(struct double_list * list, int i, double val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(double));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

float char_list_push(struct char_list * list, int i, char val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(char));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

char * string_list_push(struct string_list * list, int i, char * val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(char *));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

void * obj_list_push(struct obj_list * list, int i, void * val)
{
  (list->n)++;
  if(list->n > list->size) {
    list->size = list->n;
    list->element = realloc(list->element, list->n*sizeof(void *));
  }
  for(int n = list->n - 1; n > i; --n) list->element[n] = list->element[n - 1];
  list->element[i] = val;
  return val;
}

# define list_sort(list) \
  _Generic((list), \
            struct int_list *: int_list_sort, \
            struct float_list *: float_list_sort, \
            struct double_list *: double_list_sort, \
            struct char_list *: char_list_sort, \
            struct string_list *: string_list_sort, \
            default: null_function)(list)

int int_compare(const void * a, const void * b)
{return * (int *) a - * (int *) b;}

void int_list_sort(struct int_list * list)
{qsort(list->element, list->n, sizeof(int), int_compare); return;}

int float_compare(const void * a, const void * b)
{return * (float *) a - * (float *) b;}

void float_list_sort(struct float_list * list)
{qsort(list->element, list->n, sizeof(float), float_compare); return;}

int double_compare(const void * a, const void * b)
{return * (double *) a - * (double *) b;}

void double_list_sort(struct double_list * list)
{qsort(list->element, list->n, sizeof(double), double_compare); return;}

int char_compare(const void * a, const void * b)
{return * (char *) a - * (char *) b;}

void char_list_sort(struct char_list * list)
{qsort(list->element, list->n, sizeof(char), char_compare); return;}

int string_compare(const void * a, const void * b)
{return strcmp(*(const char **) a, *(const char **) b);}

void string_list_sort(struct string_list * list)
{qsort(list->element, list->n, sizeof(char *), string_compare); return;}

int __attribute__((weak)) obj_compare(const void * a, const void * b)
{return a - b;}

void obj_list_sort(struct int_list * list)
{qsort(list->element, list->n, sizeof(int), obj_compare); return;}

# define list_free(list) \
  _Generic((list), \
            struct int_list *: int_free, \
            struct float_list *: float_free, \
            struct double_list *: double_free, \
            struct char_list *: char_free, \
            struct string_list *: string_free, \
            struct obj_list *: obj_free, \
            default: null_function)(list)

void int_free(struct int_list * list) {free(list->element); return;}
void float_free(struct float_list * list) {free(list->element); return;}
void double_free(struct double_list * list) {free(list->element); return;}
void char_free(struct char_list * list) {free(list->element); return;}
void string_free(struct string_list * list) {free(list->element); return;}
void obj_free(struct obj_list * list) {free(list->element); return;}

# endif
