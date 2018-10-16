#include <pv/linked_list.h>

#include <stdio.h>
#include <stdlib.h>

int sum(int first, int last) {
  int sum = 0;
  for (; first < last; ++first) {
    sum += first;
  }
  return sum;
}

int main() {
  PvLinkedList ll = pv_linked_list_new();

  for (int i = 1; i < 25; ++i) {
    int* p = malloc(sizeof i);
    *p = i;
    pv_linked_list_push(&ll, p);
  }

  int sum_ll = 0;
  for (PvLinkedListNode* it = ll.first; it != ll.last; it = it->next) {
    sum_ll += *(int*)it->data;
  }

  pv_linked_list_delete(&ll, free);

  return sum_ll == sum(1, 25);
}
