# include <stdio.h>
# include "../vector.h"

int main()
{
  void * vptr, * vptr2;

  /* Vector v */
  new(v, vector);
  vector_set_dim(v, 3);
  v->dat[0] = 1.12;
  v->dat[1] = 0;
  v->dat[2] = -2;
  printf("v = "); vector_print(v, stdout); printf("\n");
  display(v,stdout);
  printf("v inherits from vector? %d\n", inherits_from(v, vector));
  printf("v inherits from abstract object? %d\n",
         inherits_from(v, abstract_object));

  /* Vector w */
  struct vector * w = clone(v);
  w->dat[0] = 0.0;
  w->dat[1] = 1;
  w->dat[2] = -5;
  printf("\nw = "); vector_print(w, stdout); printf("\n");
  display(w, stdout);

  /* Vector operations */
  printf("\nv + w = "); vector_print(vptr = vector_add(v,w), stdout);
  free(vptr);
  printf("\nv - w = ");
  vector_print(vptr = vector_subtract(v,w), stdout);
  free(vptr);
  printf("\n3 v = ");
  vector_print(vptr2 = vector_prod(3, vptr = vector_add(v,w)), stdout);
  free(vptr);
  free(vptr2);
  printf("\n");
  fprintf(stderr, "<v, w> = %f\n", vector_dot(v, w));
  fprintf(stderr, "||v|| = %f\n", vector_norm(v));
  fprintf(stderr, "v x w = ");
  vector_print(vptr = vector_cross(v, w), stdout);
  free(vptr);
  printf("\n");

  /* Clean up */
  delete(v);
  delete(w);

  return 0;
}
