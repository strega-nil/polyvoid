#include <pv/linked_list.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  PvLinkedList ll = pv_linked_list_new();

  {
    int i;
    int* p;

    for (i = 0; i < 25; ++i) {
      p = malloc(sizeof i);
      if (!p) {
        return 1;
      }
      *p = i;
      if (!pv_linked_list_push_back(&ll, p)) {
        return 1;
      }
    }

    for (i = 1; i < 25; ++i) {
      p = malloc(sizeof i);
      if (!p) {
        return 1;
      }
      *p = -i;
      if (!pv_linked_list_push_front(&ll, p)) {
        return 1;
      }
    }
  }

  {
    PvLinkedListNode* it;
    int i = -24;

    for (it = ll.first; it != NULL; it = it->next) {
      if (*(int*)it->data != i) {
        fprintf(stderr, "Failed on the %d'th iteration", i + 25);
        return 2;
      }
      i += 1;
    }
    if (i != 25) {
      return 3;
    }
  }

  pv_linked_list_delete(&ll, free);

  return 0;
}
