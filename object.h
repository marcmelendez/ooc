# ifndef OBJECT_H
# define OBJECT_H

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>

# define VA_ARGS(...) , ##__VA_ARGS__
# define new(varname, vartype, ...) \
         void * _##varname = new_object(vartype VA_ARGS(__VA_ARGS__), NULL); \
         struct vartype * varname = _##varname;

# define Object void *

/* Abstract class types */
# define MAX_NAME_SIZE 16
typedef struct Class {
  size_t size;
  char name[MAX_NAME_SIZE];
  const void * parent;
  void * (* constructor) (void * self, va_list * arguments);
  void * (* destructor) (void * self);
} Class;

/* Create an instance of a specified class */
void * new_object(const void * _class, ...)
{
  const Class * class = _class; /* Cast void pointer to Class pointer */
  void * objectPointer = calloc(1, class->size); /* Memory allocation */

  /* Check for errors */
  if(objectPointer == NULL) {
    fprintf(stderr, "Error: new: unable to allocate memory for object.\n");
    exit(-1);
  }

  /* Make object identify as belonging to class */
  * (const Class **) objectPointer = class;
  /* NOTE: because of the previous line, all root object structs MUST be written
     like this:

     struct MyRootObject {
       const void * class; // This line must come first
       ...
     }
  */

  /* Call the constructor (if it exists) providing the arguments */
  if(class->constructor) {
    va_list args;
    va_start(args, _class);
    objectPointer = class->constructor(objectPointer, &args);
    va_end(args);
  }

  return objectPointer;
}

/* Free memory allocated to an object */
void delete(void * _self)
{
  const struct Class ** objectPointer = _self;

  if(_self && *objectPointer && (*objectPointer)->destructor)
    _self = (*objectPointer)->destructor(_self);

  free(_self);

  return;
}

/* Size of an object in bytes */
size_t object_size(const void * _self)
{
  const struct Class * const * self = _self;
  if(_self && *self)
    return (*self)->size;
  return (size_t) 0;
}

/* Is an object of type class? */
int is_a(void * _self, const void * _class)
{
  struct Class ** self = _self;
  const struct Class * class = _class;
  return (*self) == class;
}

/* Search ancestors for class type */
int inherits_from(const void * _self, const struct Class * class)
{
  const struct Class * const * self = _self;
  const struct Class * ancestor_class = (*self);
  while(ancestor_class) {
    if(ancestor_class == class) return 1;
    ancestor_class = ancestor_class->parent;
  }
  return 0;
}

/* Abstract object */
struct abstract_object {
  const void * class; /* This item must come first */
  int (* differs) (void * self, void * b);
  void * (* clone) (const void * self);
  void * (* display) (const void * self, FILE * fp);
};

static void * abstract_object_constructor(void * _self, va_list * args);
static void * abstract_object_destructor(void * _self);
int abstract_object_differs(void * _a, void * _b);
static void * abstract_object_clone(const void * _self);
void * abstract_object_display(const void * _self, FILE * fp);
void * vector_cross(const void * _v, const void * _w);

static const Class _abstract_object
  = {sizeof(struct abstract_object), "abstract object", NULL,
     abstract_object_constructor, abstract_object_destructor};

const void * abstract_object = &_abstract_object;

/* Abstract object constructor */
static void * abstract_object_constructor(void * _self, va_list * args)
{
  struct abstract_object * self = _self;
  self->differs = abstract_object_differs;
  self->clone = abstract_object_clone;
  self->display = abstract_object_display;
  return _self;
}

/* Abstract object destructor */
static void * abstract_object_destructor(void * _self)
{
  return NULL;
}

/* Does abstract object a differ from abstract object b? */
int abstract_object_differs(void * _a, void * _b)
{
  struct abstract_object * a = _a;
  struct abstract_object * b = _b;
  return a != b;
}

/* Make a copy of an object */
static void * abstract_object_clone(const void * _self)
{
  const struct abstract_object * self = _self;
  if(self) {
    new(new_abstract_object, abstract_object);
    return new_abstract_object;
  }
  return NULL;
}

/* Display information about an abstract object */
void * abstract_object_display(const void * _self, FILE * fp)
{
  const struct abstract_object * self = _self;
  const struct Class * class = self->class;
  const struct Class * parent = class->parent;
  fprintf(fp, "Object: %p\n", self);
  fprintf(fp, "Class: %s\n", class->name);
  fprintf(fp, "Size: %lu\n", class->size);
  if(class->parent) fprintf(fp, "Parent: %s\n", parent->name);

  return (void *) _self;
}
/* Display object */
void * display(const void * _self, FILE * fp)
{
  const struct abstract_object * self = _self;
  if(inherits_from(self, abstract_object)) {
    if(self->display) return self->display(self, fp);
  }
  return NULL;
}

/* Make a copy of an instance of some class */
void * clone(const void * _self)
{
  const struct abstract_object * self = _self;
  if(inherits_from(self, abstract_object)) {
    if(self && self->clone)
      return self->clone(self);
  }
  return NULL;
}

/* Does the instance differ from instance b? */
int differs(void * _self, void * b)
{
  const struct abstract_object * self = _self;
  if(inherits_from(self, abstract_object)) {
    if(self && self->differs)
      return self->differs(_self, b);
  }
  return 1;
}
# endif
