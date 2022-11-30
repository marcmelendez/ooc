# include <stdio.h>
# include "../object.h"

int main()
{
  new(a, abstract_object);
  new(b, abstract_object);
  Object c = clone(a);

  display(a, stdout);
  display(b, stdout);
  display(c, stdout);

  printf("a == a ? %d\n", !differs(a,a));
  printf("a == b ? %d\n", !differs(a,b));
  printf("a == c ? %d\n", !differs(a,c));

  printf("is b an abstract object? %d\n", is_a(b, abstract_object));
  printf("Does b inherit from abstract object? %d\n",
         inherits_from(b, abstract_object));

  printf("Size of a: %lu\n", object_size(a));

  delete(a);
  delete(b);
  delete(c);

  return 0;
}
