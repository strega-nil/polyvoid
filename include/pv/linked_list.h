#ifndef PV_LINKED_LIST_H
#define PV_LINKED_LIST_H

#include "_macros.h"
#include "common.h"

PV_DEFINE_STRUCT(PvLinkedListNode) {
  void* data;
  PvLinkedListNode* next;
  PvLinkedListNode* prev;
};

PV_DEFINE_STRUCT(PvLinkedList) {
  PvLinkedListNode* first;
  PvLinkedListNode* last;
};

PV_EXPORT
PvLinkedList pv_linked_list_new(void);

PV_EXPORT
PvLinkedListNode* pv_linked_list_push(PvLinkedList* self, void* data);

PV_EXPORT
PvLinkedListNode* pv_linked_list_insert_after(
    PvLinkedList* self, PvLinkedListNode* node, void* data);

PV_EXPORT
void* pv_linked_list_remove(PvLinkedList* self, PvLinkedListNode* node);

PV_EXPORT
void pv_linked_list_delete(PvLinkedList* self, PvDeleter* deleter);

#endif // PV_LINKED_LIST_H
