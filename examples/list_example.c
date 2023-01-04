# include <stdio.h>
# include "../list.h"

int main()
{
  /* Integer list */
  struct int_list intl;
  list_init(&intl, 10);
  for(int i = 0; i < intl.n; ++i) list_set(&intl, i, i + 1);
  list_pop(&intl, 2);
  list_set(&intl, 4, -4);
  list_push(&intl, 6, -6);
  printf("Integer list: ");
  for(int i = 0; i < intl.n; ++i) printf("%d, ", list_read(&intl, i));
  printf("\n");
  list_free(&intl);

  /* Pointer to string list */
  struct string_list * slist = calloc(1, sizeof(struct string_list));
  list_init(slist, 0);
  list_push(slist, 0, "Five");
  list_push(slist, 0, "Four");
  list_push(slist, 0, "Three");
  list_push(slist, 0, "Two");
  list_push(slist, 0, "One");
  printf("String list: ");
  for(int i = 0; i < slist->n; ++i) printf("%s, ", list_read(slist, i));
  list_sort(slist);
  printf("\nSorted alphabetically: ");
  for(int i = 0; i < slist->n; ++i) printf("%s, ", list_read(slist, i));
  printf("\n");
  list_free(slist);
  free(slist);

  return 0;
}

