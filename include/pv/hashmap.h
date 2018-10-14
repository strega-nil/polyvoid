#ifndef PV_HASHMAP_H
#define PV_HASHMAP_H

#include "_macros.h"
#include "common.h"
#include "linked_list.h"

PV_BEGIN_HEADER

PV_DEFINE_STRUCT(PvHashmap) {
  PvHasher* hasher;
  PvEquals* equals;
  PvLinkedList* array;
};

PV_DEFINE_STRUCT(PvHashmapInsertReturnType) {
  bool inserted;
  void* position;
};

PV_EXPORT
PvHashmap pv_hashmap_new(PvHasher* hasher, PvEquals* equals);

PV_EXPORT
PvHashmapInsertReturnType pv_hashmap_insert(PvHashmap* self, void* value);

PV_END_HEADER

#endif // PV_HASHMAP_H