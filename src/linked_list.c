#include <pv/linked_list.h>

#include <assert.h>
#include <stdlib.h>

PV_EXPORT
PvLinkedList pv_linked_list_new(void) {
  PvLinkedList ret = {NULL, NULL};
  return ret;
}

PV_EXPORT
PvLinkedListNode* pv_linked_list_push(PvLinkedList* self, void* data) {
  PvLinkedListNode* new_node = malloc(sizeof *new_node);
  if (!new_node) {
    return NULL;
  }

  new_node->data = data;

  if (!self->last) {
    assert(!self->first);

    new_node->next = new_node->prev = NULL;
    self->first = self->last = new_node;
  } else {
    assert(self->first);

    PvLinkedListNode* last = self->last;
    self->last = last->next = new_node;

    new_node->prev = last;
    new_node->next = NULL;
  }

  return new_node;
}

PV_EXPORT
PvLinkedListNode* pv_linked_list_insert_after(
    PvLinkedList* self, PvLinkedListNode* node, void* data) {
  PvLinkedListNode* new_node = malloc(sizeof *new_node);
  if (!new_node) {
    return NULL;
  }

  new_node->data = data;

  if (self->last == node) {
    self->last = new_node;
  }

  new_node->prev = node;
  new_node->next = node->next;
  node->next = new_node;

  return new_node;
}

PV_EXPORT
void* pv_linked_list_remove(PvLinkedList* self, PvLinkedListNode* node) {
  if (self->first == node && self->last == node) {
    self->first = self->last = NULL;
  } else if (self->first == node) {
    PvLinkedListNode* next = node->next;
    self->first = next;
    next->prev = NULL;
  } else if (self->last == node) {
    PvLinkedListNode* prev = node->prev;
    self->last = prev;
    prev->next = NULL;
  } else {
    PvLinkedListNode* prev = node->prev;
    PvLinkedListNode* next = node->next;

    prev->next = next;
    next->prev = prev;
  }

  void* ret = node->data;
  free(node);
  return ret;
}

PV_EXPORT
void pv_linked_list_delete(PvLinkedList* self, PvDeleter* deleter) {
  PvLinkedListNode* it = self->first;
  while (it != NULL) {
    PvLinkedListNode* cur = it;
    it = cur->next;

    deleter(cur->data);
    free(cur);
  }
}