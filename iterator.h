# ifndef ITERATOR_H
# define ITERATOR_H
# include "object.h"

# ifndef VARIABLE_TYPE
# define VARIABLE_TYPE
typedef enum {POINTER = 0, INT, FLOAT, DOUBLE, CHAR, STRING} variable_type;
# endif

union iterator_value {
  int i;
  float f;
  double d;
  char c;
  char * s;
  void * p;
};

struct iterator {
  const struct abstract_object _; /* This item must come first */
  variable_type val_type;
  union iterator_value val;
  union iterator_value (* next)(void * _self);
  union iterator_value (* prev)(void * _self);
  union iterator_value (* set)(void * _self, union iterator_value val);
  union iterator_value (* get)(void * _self);
};

static void * iterator_constructor(void * _self, va_list * args);
static void * iterator_clone(const void * _self);
static void * iterator_display(const void * _self, FILE * fp);

union iterator_value iterator_next(void * _self);
union iterator_value iterator_prev(void * _self);
variable_type iterator_type(const void * _self);
union iterator_value iterator_set(void * _self, union iterator_value val);
union iterator_value iterator_get(void * _self);

static const Class _iterator
  = {sizeof(struct iterator), "iterator", &_abstract_object,
     iterator_constructor, NULL};

const void * iterator = &_iterator;

static void * iterator_constructor(void * _self, va_list * args)
{
  struct abstract_object * obj =  abstract_object_constructor(_self, args);
  obj->clone = iterator_clone;
  obj->display = iterator_display;
  struct iterator * self = _self;

  variable_type val_type = va_arg(*args, variable_type);

  if(val_type >= 0 && val_type <= STRING) {
    self->val_type = (variable_type) val_type;
  } else {
    self->val_type = POINTER;
  }
  self->val = (union iterator_value) NULL;

  self->next = iterator_next;
  self->prev = iterator_prev;
  self->set = iterator_set;
  self->get = iterator_get;

  return _self;
}

static void * iterator_clone(const void * _self)
{
  if(inherits_from(_self, iterator)) {
    const struct iterator * self = _self;
    new(i, iterator);
    i->val_type = self->val_type;
    i->val = self->val;
    return i;
  }
  return NULL;
}

static void * iterator_display(const void * _self, FILE * fp)
{
  abstract_object_display(_self, fp);

  if(inherits_from(_self, iterator)){
    const struct iterator * self = _self;
    fprintf(fp, "Current value: ");
    switch(self->val_type) {
      case INT:
        fprintf(fp, "%d\n", self->val.i);
        break;
      case FLOAT:
        fprintf(fp, "%f\n", self->val.f);
        break;
      case DOUBLE:
        fprintf(fp, "%f\n", self->val.d);
        break;
      case CHAR:
        fprintf(fp, "%c\n", self->val.c);
        break;
      case STRING:
        fprintf(fp, "%s\n", self->val.s);
        break;
      default:
        fprintf(fp, "%p\n", self->val.p);
    }
  }
  return NULL;
}

union iterator_value iterator_next(void * _self)
{
  struct iterator * self = _self;
  switch(self->val_type) {
    case INT:
      self->val.i++;
      break;
    case CHAR:
      self->val.c++;
      break;
    case FLOAT:
      self->val.f += 1.0f;
      break;
    case DOUBLE:
      self->val.d += 1.0;
      break;
    case STRING:
      self->val.s++;
      break;
    case POINTER:
      self->val.p++;
  }

  return self->val;
}

union iterator_value iterator_prev(void * _self)
{
  struct iterator * self = _self;
  switch(self->val_type) {
    case INT:
      self->val.i--;
    case CHAR:
      self->val.c--;
    case FLOAT:
      self->val.f -= 1.0f;
      break;
    case DOUBLE:
      self->val.d -= 1.0;
      break;
    case STRING:
      self->val.s--;
    case POINTER:
      self->val.p--;
  }

  return self->val;
}

variable_type iterator_type(const void * _self)
{
  if(inherits_from(_self, iterator)) {
    const struct iterator * self = _self;
    return self->val_type;
  }
  return -1;
}

union iterator_value iterator_set(void * _self, union iterator_value val)
{
  struct iterator * self = _self;
  self->val = val;

  return self->val;
}

union iterator_value iterator_get(void * _self)
{
  struct iterator * self = _self;
  return self->val;
}

union iterator_value next(void * _self)
{
  if(inherits_from(_self, iterator)) {
    struct iterator * self = _self;
    if(self->next) return self->next(_self);
  }
  return (union iterator_value) NULL;
}

union iterator_value prev(void * _self)
{
  if(inherits_from(_self, iterator)) {
    struct iterator * self = _self;
    if(self->prev) return self->prev(_self);
  }
  return (union iterator_value) NULL;
}

union iterator_value set(void * _self, union iterator_value val)
{
  if(inherits_from(_self, iterator)) {
    struct iterator * self = _self;
    if(self->set) return self->set(_self, val);
  }
  return (union iterator_value) NULL;
}

union iterator_value get(void * _self)
{
  if(inherits_from(_self, iterator)) {
    struct iterator * self = _self;
    if(self->get) return self->get(_self);
  }
  return (union iterator_value) NULL;
}

# endif
