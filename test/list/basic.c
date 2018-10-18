#include <pv/linked_list.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  PvLinkedList ll = pv_linked_list_new();

  for (int i = 1; i < 25; ++i) {
    int* p = malloc(sizeof i);
    if (!p) {
      return 1;
    }
    *p = i;
    if (!pv_linked_list_push_back(&ll, p)) {
      return 1;
    }
  }

/*
  for (int i = 1; i < 25; ++i) {
    int* p = malloc(sizeof i);
    if (!p) {
      return 1;
    }
    *p = -i;
    if (!pv_linked_list_push_front(&ll, p)) {
      return 1;
    }
  }
  */

  int i = 1;
  for (PvLinkedListNode* it = ll.first; it != NULL; it = it->next) {
    if (*(int*)it->data != i) {
      return 2;
    }
    i += 1;
    fprintf(stderr, "hi, made it here: %d\n", i);
  }
  if (i != 25) {
    return 3;
  }

  pv_linked_list_delete(&ll, free);

  return 0;
}
