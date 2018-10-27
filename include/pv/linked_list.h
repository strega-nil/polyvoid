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
PvLinkedListNode* pv_linked_list_push_front(PvLinkedList* self, void* data);

PV_EXPORT
PvLinkedListNode* pv_linked_list_push_back(PvLinkedList* self, void* data);

PV_EXPORT
PvLinkedListNode* pv_linked_list_insert_before(
    PvLinkedList* self, PvLinkedListNode* PV_node, void* data);

PV_EXPORT
PvLinkedListNode* pv_linked_list_insert_after(
    PvLinkedList* self, PvLinkedListNode* PV_node, void* data);

PV_EXPORT
void* pv_linked_list_remove(PvLinkedList* self, PvLinkedListNode* PV_node);

PV_EXPORT
void pv_linked_list_delete(PvLinkedList* self, PvDeleter* PV_del);

#endif /* PV_LINKED_LIST_H */
