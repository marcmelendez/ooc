# ifndef SET_H
# define SET_H
# include "object.h"

/*** Set definition ***/
struct set {
  const struct abstract_object _; /* This item must come first */
  int nelements; /* Number of elements in the set */
  void * element;
  int (* find)(const void * _self, const void * _element);
  void (* insert)(void * _self, const void * _element);
  void (* drop)(void * _self, const void * element);
  int (* equal)(const void * _A, const void * _B);
};

static void * set_constructor(void * _self, va_list * args);
static void * set_destructor(void * _self);
static void * set_clone(const void * _self);
static void * set_display(const void * _self, FILE * fp);

static const Class _set
  = {sizeof(struct set), "set", &_abstract_object,
     set_constructor, set_destructor};

const void * set = &_set;

/*** set operations ***/
int find(const void * _self, const void * _element);
int contains(const void * _self, const void * _element);
void insert(void * _self, const void * _element);
void drop(void * _self, const void * element);
int equal(const void * _A, const void * _B);

/*** specialised overrides ***/
int set_find(const void * self, const void * _element);
void set_insert(void * _self, const void * _element);
void set_drop(void * _self, const void * element);
int set_equal(const void * _A, const void * _B);

/*** Set function implementations ***/
/* Constructor */
static void * set_constructor(void * _self, va_list * args)
{
  struct abstract_object * obj =  abstract_object_constructor(_self, args);
  obj->clone = set_clone;
  obj->display = set_display;
  struct set * self = _self;
  self->nelements = 0;
  self->element = NULL;
  self->find = set_find;
  self->insert = set_insert;
  self->drop = set_drop;
  self->equal = set_equal;
  return _self;
}

/* Destructor */
static void * set_destructor(void * _self)
{
  struct set * self = _self;
  free(self->element);
  return NULL;
}

/* Clone a set */
static void * set_clone(const void * _self)
{
  if(inherits_from(_self, set)) {
    const struct set * self = _self;
    new(A, set);
    A->nelements = self->nelements;
    A->element = calloc(sizeof(void *), A->nelements);
    struct abstract_object ** A_element = A->element;
    struct abstract_object ** self_element = self->element;
    for(int i = 0; i < self->nelements; ++i) {
      A_element[i] = self_element[i];
    }
    return A;
  }
  return NULL;
}

/* Display set object information */
static void * set_display(const void * _self, FILE * fp)
{
  abstract_object_display(_self, fp);

  if(inherits_from(_self, set)){
    const struct set * self = _self;
    fprintf(fp, "Number of elements: %d\n", self->nelements);
  }
  return NULL;
}


/*** Generic set operations ***/
int find(const void * _self, const void * _element)
{
  if(inherits_from(_self, set) && inherits_from(_element, abstract_object)){
    const struct set * self = _self;
    if(self->find) return self->find(_self, _element);
  }
  return -1;
}

int contains(const void * _self, const void * _element)
{
  return find(_self, _element) != -1;
}

void insert(void * _self, const void * _element) {
  if(inherits_from(_self, set) && inherits_from(_element, abstract_object)){
    const struct set * self = _self;
    if(self->insert) self->insert(_self, _element);
  }
  return;
}

void drop(void * _self, const void * _element)
{
  if(inherits_from(_self, set) && inherits_from(_element, abstract_object)){
    const struct set * self = _self;
    if(self->drop) self->drop(_self, _element);
  }
  return;
}

int equal(const void * _A, const void * _B)
{
  if(inherits_from(_A, set) && inherits_from(_B, set)){
    const struct set * A = _A;
    if(A->equal) return A->equal(_A, _B);
  }
  return 0;
}

/* Return the index of an element in a set (or -1 if it was not found) */
int set_find(const void * _self, const void * _element)
{
    const struct set * self = _self;
    const struct abstract_object * element = _element;
    const struct abstract_object ** self_element = self->element;
    for(int i = 0; i < self->nelements; ++i)
      if(self_element[i] == element) return i;
  return -1;
}

/* Add an element to a set */
void set_insert(void * _self, const void * _element)
{
  if(inherits_from(_self, set) && inherits_from(_element, abstract_object)){
    if(!contains(_self, _element)) {
      struct set * self = _self;
      self->nelements++;
      self->element = realloc(self->element, self->nelements*sizeof(void *));
      const struct abstract_object ** self_element = self->element;
      self_element[self->nelements - 1] = _element;
    }
  }
  return;
}

/* Remove an element from a set */
void set_drop(void * _self, const void * _element)
{
   if(contains(_self, _element)) {
    int i = set_find(_self, _element);
    struct set * self = _self;
    struct abstract_object ** self_element = self->element;
    self_element[i] = self_element[self->nelements - 1];
    self_element[self->nelements - 1] = NULL;
    self->nelements--;
  }
  return;
}

/* Are A and B equal sets? */
int set_equal(const void * _A, const void * _B)
{
  const struct set * A = _A;
  const struct set * B = _B;
  if(A->nelements == B->nelements) {
    struct abstract_object ** A_element = A->element;
    for(int i = 0; i < A->nelements; ++i) {
      if(!contains(B, A_element[i])) return 0;
    }
    return 1;
  }
  return 0;
}

# endif
