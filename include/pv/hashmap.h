#ifndef PV_HASHMAP_H
#define PV_HASHMAP_H

#include "_macros.h"
#include "common.h"
#include "linked_list.h"

PV_BEGIN_HEADER

PV_DEFINE_STRUCT(PvHashmap) {
  PvHasher* hasher;
  PvEquals* equals;
  size_t number_of_elements;
  // note: must be power of two
  size_t array_length;
  PvLinkedList* array;
};

PV_EXPORT
PvHashmap pv_hashmap_new(PvHasher* hasher, PvEquals* equals);

/**
  Either inserts the key, value pair into the map, if the key is not currently
  in the map; or it sets the value mapped to by key to value.

  NOTE:
    This function does not replace key.
    If you want to free the new key, you'll have to get it from wherever you
    passed it in.

  ```cpp
  void add_0_1_to_hm(PvHashmap* hm) {
    int* key = malloc(sizeof *key);
    int* value = malloc(sizeof *value);
    if (!key || !value) {
      abort();
    }
    *key = 0;
    *value = 1;

    int* old_value;
    if ((old_value = pv_hashmap_insert(key, value))) {
      free(old_value);
    }
  }
  ```

  returns:
    - NULL if the key is not currently in the map
    - the value replaced if the key _is_ in the map
*/
PV_EXPORT
void* pv_hashmap_insert(PvHashmap* self, void* key, void* value);

PV_EXPORT
void* pv_hashmap_get(PvHashmap* self, void const* key);

PV_DEFINE_STRUCT(PvHashmapRemoveReturn) {
  void* key;
  void* value;
};

PV_EXPORT
PvHashmapRemoveReturn pv_hashmap_remove(PvHashmap* self, void const* key);

PV_EXPORT
void pv_hashmap_delete(
  PvHashmap* self,
  PvDeleter* key_deleter,
  PvDeleter* value_deleter);

PV_END_HEADER

#endif // PV_HASHMAP_H
