# include <stdio.h>
# include <math.h>
# include "../iterator.h"

struct time_iterator {
  const struct iterator _;
  double t0;
  double dt;
  long int step;
};

static void * time_iterator_clone(const void * _self);
static void * time_iterator_display(const void * _self, FILE * fp);
union iterator_value time_iterator_next(void * _self);
union iterator_value time_iterator_prev(void * _self);
union iterator_value time_iterator_set(void * _self, union iterator_value val);
union iterator_value time_iterator_step(void * _self, double val);

static void * time_iterator_constructor(void * _self, va_list * args)
{
  struct abstract_object * obj =  abstract_object_constructor(_self, args);
  obj->clone = time_iterator_clone;
  obj->display = time_iterator_display;

  struct iterator * self = _self;
  self->val_type = DOUBLE;
  self->val = (union iterator_value) 0.0;

  self->next = time_iterator_next;
  self->prev = time_iterator_prev;
  self->set = time_iterator_set;

  struct time_iterator * tself = _self;
  tself->t0 = 0.0;
  tself->dt = 1.0;
  tself->step = 0;

  return _self;
}

static const Class _time_iterator
  = {sizeof(struct time_iterator), "time iterator", &_iterator,
     time_iterator_constructor, NULL};

const void * time_iterator = &_time_iterator;

static void * time_iterator_clone(const void * _self)
{
  if(inherits_from(_self, time_iterator)) {
    const struct iterator * self = _self;
    const struct time_iterator * tself = _self;
    new(i, time_iterator);
    i->t0 = tself->t0;
    i->dt = tself->dt;
    i->step = tself->step;
    struct iterator * iti = _i;
    iti->val_type = self->val_type;
    iterator_set(i, self->val);
    return i;
  }
  return NULL;
}

static void * time_iterator_display(const void * _self, FILE * fp)
{
  iterator_display(_self, fp);

  if(inherits_from(_self, time_iterator)){
    const struct time_iterator * self = _self;
    fprintf(fp, "Initial time: %f\n", self->t0);
    fprintf(fp, "Time step: %f\n", self->dt);
  }

  return NULL;
}

union iterator_value time_iterator_next(void * _self)
{
  if(inherits_from(_self, time_iterator)){
    struct time_iterator * self = _self;
    self->step++;
    iterator_set(self,
                 (union iterator_value) (self->t0
                                         + self->dt*((double) self->step)));
  }
  struct iterator * self = _self;
  return self->val;
}

union iterator_value time_iterator_prev(void * _self)
{
  if(inherits_from(_self, iterator)){
    struct time_iterator * self = _self;
    self->step--;
    iterator_set(self,
                 (union iterator_value) (self->t0
                                         + self->dt*((double) self->step)));
  }
  struct iterator * self = _self;
  return self->val;
}

union iterator_value time_iterator_set(void * _self, union iterator_value val)
{
  if(inherits_from(_self, iterator)){
    struct time_iterator * self = _self;
    self->step = 0;
    self->t0 = val.d;
    iterator_set(self, val);
  }
  struct iterator * self = _self;
  return self->val;
}

union iterator_value time_iterator_dt(void * _self, double val)
{
  if(inherits_from(_self, iterator)){
    struct time_iterator * self = _self;
    self->dt = val;
  }
  struct iterator * self = _self;
  return self->val;
}

int main() {
  /* Standard for loop */
  new(it, iterator, INT);

  for(set(it, (union iterator_value) 10); get(it).i < 20; next(it))
    printf("i = %d\n", get(it).i);

  delete(it);

  /* Time loops */
  new(time, time_iterator);
  time_iterator_dt(time, 0.01);
  display(time, stderr);

  for(double t = set(time, (union iterator_value) 5.0).d;
      t < 10.0; t = next(time).d) {
    printf("%f  %f  %f\n", t, cos(t), sin(t));
  }

  printf("\n");

  for(double t = set(time, (union iterator_value) 5.0).d;
      t > 0.0; t = prev(time).d) {
    printf("%f  %f  %f\n", t, cos(t), sin(t));
  }

  Object s = clone(time);
  display(s, stderr);
  delete(s);

  delete(time);
}
