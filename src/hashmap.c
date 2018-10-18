#include <pv/hashmap.h>

#include <stdint.h> // for SIZE_MAX

PV_DEFINE_STRUCT(PvHashmapNode) {
  void* key;
  void* value;
  PvHashmapNode* next;
};

#define MASK(log2) ((((size_t)1) << (log2)) - 1)
#define LENGTH(log2) (((size_t)1) << (log2))

PV_EXPORT
PvHashmap pv_hashmap_new(PvHasher* hasher, PvEquals* equals) {
  PvHashmap ret = {._hasher = hasher,
                   ._equals = equals,
                   ._elements_until_reallocation = 0,
                   ._array_length_log2 = SIZE_MAX,
                   ._array = NULL};

  return ret;
}

static void reallocate_memory(PvHashmap* self) {
  PvHasher* const hasher = self->_hasher;

  size_t const old_length_log2 = self->_array_length_log2;
  PvHashmapNode** const old_array = self->_array;

  if (old_length_log2 == SIZE_MAX) {
    size_t const new_length_log2 = 3;
    PvHashmapNode** const new_array =
        calloc(LENGTH(new_length_log2), sizeof *new_array);
    self->_array = new_array;
    self->_array_length_log2 = new_length_log2;
    self->_elements_until_reallocation = LENGTH(new_length_log2);
    return;
  }

  size_t new_length_log2 = old_length_log2 + 1;

  size_t const mask = MASK(new_length_log2);
  PvHashmapNode** const new_array =
      calloc(LENGTH(new_length_log2), sizeof *new_array);
  if (!new_array) {
    abort();
  }

  for (PvHashmapNode** list = old_array;
       list != old_array + LENGTH(old_length_log2);
       ++list) {
    for (PvHashmapNode* node = *list; node != NULL; node = node->next) {
      size_t const bucket_idx = hasher(node->key) & mask;
      node->next = new_array[bucket_idx];
      new_array[bucket_idx] = node;
    }
  }

  free(old_array);

  self->_array = new_array;
  self->_array_length_log2 = new_length_log2;
  self->_elements_until_reallocation = LENGTH(old_length_log2);
}

PV_EXPORT
void* pv_hashmap_insert(PvHashmap* self, void* key, void* value) {
  PvEquals* const equals = self->_equals;

  if (self->_elements_until_reallocation == 0) {
    reallocate_memory(self);
  }

  size_t const hash = self->_hasher(key);
  size_t const bucket_idx = hash & MASK(self->_array_length_log2);

  PvHashmapNode** const bucket = &self->_array[bucket_idx];

  for (PvHashmapNode* node = *bucket; node != NULL; node = node->next) {
    if (equals(key, node->key)) {
      void* old_value = node->value;
      node->value = value;
      return old_value;
    }
  }

  PvHashmapNode* node = malloc(sizeof *node);
  if (!node) {
    abort();
  }
  node->key = key;
  node->value = value;
  node->next = *bucket;
  *bucket = node;

  --self->_elements_until_reallocation;

  return NULL;
}

PV_EXPORT
void* pv_hashmap_get(PvHashmap* self, void const* key) {
  PvEquals* const equals = self->_equals;

  size_t const hash = self->_hasher(key);
  size_t const bucket_idx = hash & MASK(self->_array_length_log2);

  for (PvHashmapNode const* node = self->_array[bucket_idx]; node != NULL;
       node = node->next) {
    if (equals(key, node->key)) {
      return node->value;
    }
  }

  return NULL;
}

PV_EXPORT
PvHashmapRemoveReturn pv_hashmap_remove(PvHashmap* self, void const* key) {
  PvEquals* const equals = self->_equals;

  PvHashmapRemoveReturn ret = {.key = NULL, .value = NULL};

  size_t const hash = self->_hasher(key);
  size_t const bucket_idx = hash & MASK(self->_array_length_log2);

  PvHashmapNode* previous = NULL;
  for (PvHashmapNode* node = self->_array[bucket_idx]; node != NULL;
       node = node->next) {
    if (equals(key, node->key)) {
      ret.key = node->key;
      ret.value = node->value;
      if (!previous) {
        self->_array[bucket_idx] = node->next;
      } else {
        previous->next = node->next;
      }
      free(node);
      ++self->_elements_until_reallocation;
      return ret;
    } else {
      previous = node;
    }
  }

  return ret;
}

PV_EXPORT
size_t pv_hashmap_size(PvHashmap const* self) {
  return LENGTH(self->_array_length_log2) - self->_elements_until_reallocation;
}

PV_EXPORT
void pv_hashmap_delete(
    PvHashmap* self, PvDeleter* key_deleter, PvDeleter* value_deleter) {
  PvHashmapNode** const first = self->_array;
  PvHashmapNode** const last = self->_array + LENGTH(self->_array_length_log2);

  for (PvHashmapNode** list = first; list != last; ++list) {
    PvHashmapNode* node = *list;
    while (node) {
      key_deleter(node->key);
      value_deleter(node->value);
      PvHashmapNode* old_node = node;
      node = node->next;
      free(old_node);
    }
  }

  free(first);
}
