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
  PvHashmap ret;

  ret._hasher = hasher;
  ret._equals = equals;
  ret._elements_until_reallocation = 0;
  ret._array_length_log2 = SIZE_MAX;
  ret._array = NULL;

  return ret;
}

static void reallocate_memory(PvHashmap* self) {
  PvHasher* hasher;

  size_t old_length_log2;
  size_t new_length_log2;
  size_t mask;
  PvHashmapNode** old_array;
  PvHashmapNode** new_array;

  hasher = self->_hasher;
  old_length_log2 = self->_array_length_log2;
  old_array = self->_array;

  if (old_length_log2 == SIZE_MAX) {
    new_length_log2 = 3;
    new_array = calloc(LENGTH(new_length_log2), sizeof *new_array);
    self->_array = new_array;
    self->_array_length_log2 = new_length_log2;
    self->_elements_until_reallocation = LENGTH(new_length_log2);
    return;
  }

  new_length_log2 = old_length_log2 + 1;

  mask = MASK(new_length_log2);
  new_array = calloc(LENGTH(new_length_log2), sizeof *new_array);
  if (!new_array) {
    abort();
  }

  {
    PvHashmapNode** list;
    PvHashmapNode* node;
    for (list = old_array;
        list != old_array + LENGTH(old_length_log2);
        ++list) {
      for (node = *list; node != NULL; node = node->next) {
        size_t bucket_idx;

        bucket_idx = hasher(node->key) & mask;
        node->next = new_array[bucket_idx];
        new_array[bucket_idx] = node;
      }
    }
  }

  free(old_array);

  self->_array = new_array;
  self->_array_length_log2 = new_length_log2;
  self->_elements_until_reallocation = LENGTH(old_length_log2);
}

PV_EXPORT
void* pv_hashmap_insert(PvHashmap* self, void* key, void* value) {
  PvEquals* equals;
  size_t bucket_idx;
  PvHashmapNode** bucket;
  PvHashmapNode* node;

  equals = self->_equals;

  if (self->_elements_until_reallocation == 0) {
    reallocate_memory(self);
  }

  bucket_idx = self->_hasher(key) & MASK(self->_array_length_log2);

  bucket = &self->_array[bucket_idx];

  for (node = *bucket; node != NULL; node = node->next) {
    if (equals(key, node->key)) {
      void* old_value = node->value;
      node->value = value;
      return old_value;
    }
  }

  node = malloc(sizeof *node);
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
  PvEquals* equals;
  size_t bucket_idx;

  equals = self->_equals;
  bucket_idx = self->_hasher(key) & MASK(self->_array_length_log2);

{
  PvHashmapNode* node;
  for (node = self->_array[bucket_idx]; node != NULL;
       node = node->next) {
    if (equals(key, node->key)) {
      return node->value;
    }
  }
}

  return NULL;
}

PV_EXPORT
PvHashmapRemoveReturn pv_hashmap_remove(PvHashmap* self, void const* key) {
  PvEquals* equals;
  PvHashmapRemoveReturn ret;
  size_t bucket_idx;

  equals = self->_equals;

  ret.key = ret.value = NULL;

  bucket_idx = self->_hasher(key) & MASK(self->_array_length_log2);

  {
    PvHashmapNode* prev = NULL;
    PvHashmapNode* node;
    for (node = self->_array[bucket_idx]; node != NULL;
        node = node->next) {
      if (equals(key, node->key)) {
        ret.key = node->key;
        ret.value = node->value;
        if (!prev) {
          self->_array[bucket_idx] = node->next;
        } else {
          prev->next = node->next;
        }
        free(node);
        ++self->_elements_until_reallocation;
        return ret;
      } else {
        prev = node;
      }
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
  PvHashmapNode** first;
  PvHashmapNode** last;
  PvHashmapNode** list;

  first = self->_array;
  last = first + LENGTH(self->_array_length_log2);

  for (list = first; list != last; ++list) {
    PvHashmapNode* node;

    node = *list;
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
