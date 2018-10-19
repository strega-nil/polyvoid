#include <pv/linked_list.h>

#include <assert.h>
#include <stdlib.h>

PV_EXPORT
PvLinkedList pv_linked_list_new(void) {
  PvLinkedList ret;
  ret.first = ret.last = NULL;
  return ret;
}

PV_EXPORT
PvLinkedListNode* pv_linked_list_push_back(PvLinkedList* self, void* data) {
  PvLinkedListNode* new_node;
  PvLinkedListNode* last;

  new_node = malloc(sizeof *new_node);
  last = self->last;

  if (!new_node) {
    return NULL;
  }
  new_node->data = data;

  self->last = new_node;
  if (!self->first) {
    self->first = new_node;
  }

  new_node->prev = last;
  new_node->next = NULL;

  if (last) {
    last->next = new_node;
  }

  return new_node;
}

PV_EXPORT
PvLinkedListNode* pv_linked_list_push_front(PvLinkedList* self, void* data) {
  PvLinkedListNode* new_node;
  PvLinkedListNode* first;

  new_node = malloc(sizeof *new_node);
  first = self->first;

  if (!new_node) {
    return NULL;
  }
  new_node->data = data;

  self->first = new_node;
  if (!self->last) {
    self->last = new_node;
  }

  new_node->next = first;
  new_node->prev = NULL;

  if (first) {
    first->prev = new_node;
  }

  return new_node;
}

PV_EXPORT
PvLinkedListNode* pv_linked_list_insert_after(
    PvLinkedList* self, PvLinkedListNode* node, void* data) {
  PvLinkedListNode* new_node;

  new_node = malloc(sizeof *new_node);
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
PvLinkedListNode* pv_linked_list_insert_before(
    PvLinkedList* self, PvLinkedListNode* node, void* data) {
  PvLinkedListNode* new_node;

  new_node = malloc(sizeof *new_node);
  if (!new_node) {
    return NULL;
  }

  new_node->data = data;

  if (self->first == node) {
    self->first = new_node;
  }

  new_node->next = node;
  new_node->prev = node->prev;
  node->prev = new_node;

  return new_node;
}

PV_EXPORT
void* pv_linked_list_remove(PvLinkedList* self, PvLinkedListNode* node) {
  PvLinkedListNode* prev;
  PvLinkedListNode* next;
  void* ret;

  if (self->first == node && self->last == node) {
    self->first = self->last = NULL;
  } else if (self->first == node) {
    next = node->next;

    self->first = next;
    next->prev = NULL;
  } else if (self->last == node) {
    prev = node->prev;

    self->last = prev;
    prev->next = NULL;
  } else {
    prev = node->prev;
    next = node->next;

    prev->next = next;
    next->prev = prev;
  }

  ret = node->data;
  free(node);
  return ret;
}

PV_EXPORT
void pv_linked_list_delete(PvLinkedList* self, PvDeleter* deleter) {
  PvLinkedListNode* it;

  it = self->first;
  while (it != NULL) {
    PvLinkedListNode* cur = it;
    it = cur->next;

    deleter(cur->data);
    free(cur);
  }
}
