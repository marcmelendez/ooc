# include <stdio.h>
# include "../set.h"
# include "../matrix.h"

int main(int argc, char * argv[])
{
  new(A, set);    // Create set
  new(M, matrix); // Create matrix
  insert(A, M);   // Add matrix to set
  insert(A, M);   // M is already in M, so nothing happens
  new(v, vector); // Create vector
  insert(A, v);   // Add vector to set
  new(B, set);    // Create sets B and C
  Object C = clone(A);
  insert(A, B);   // Sets can contain sets
  insert(A, C);
  insert(A, A);   // A set can contain itself

  /* Print some information on the set A */
  display(A, stdout);
  printf("A elements: %d\n", A->nelements);
  printf("A in A? %d\n", contains(A, A));
  printf("find C: %d\n", find(A, C));
  printf("A == A: %d\n", equal(A, A));
  printf("A == B: %d\n", equal(A, B));
  printf("A == C: %d\n", equal(A, C));

  /* Remove elements from the set */
  drop(A, M);
  drop(A, v);
  drop(A, A);

  /* Delete objects and free memory*/
  delete(A);
  delete(B);
  delete(C);
  delete(M);
  delete(v);

  return 0;
}
