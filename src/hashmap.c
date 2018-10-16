#include <pv/hashmap.h>

PV_DEFINE_STRUCT(HashmapNode) {
  void* key;
  size_t hash;
  void* value;
};

#define MASK_OF_SIZE(size) ((1 << (size)) - 1)

PV_EXPORT
PvHashmap pv_hashmap_new(PvHasher* hasher, PvEquals* equals) {
  PvHashmap ret = {
    .hasher = hasher,
    .equals = equals,
    .number_of_elements = 0,
    .array_length = 0,
    .array = NULL };

  return ret;
}

static void reallocate_memory(PvHashmap* self) {
  size_t const old_size = self->array_length;
  PvLinkedList* const old_array = self->array;

  size_t new_size;
  if (old_size == 0) {
    new_size = 8;
  } else {
    new_size = old_size * 2;
  }

  size_t const mask = MASK_OF_SIZE(new_size);
  PvLinkedList* const new_array = calloc(new_size, sizeof *new_array);
  if (!new_array) {
    abort();
  }

  for (
      PvLinkedList* list = old_array;
      list != old_array + old_size;
      ++list) {
    for (
        PvLinkedListNode* it = list->first;
        it != NULL;
        it = it->next) {
      HashmapNode* const node = it->data;
      size_t const bucket = node->hash & mask;
      pv_linked_list_push(&new_array[bucket], node);
    }
    pv_linked_list_delete(list, pv_nothing_deleter);
  }

  self->array = new_array;
  self->array_length = new_size;
}

PV_EXPORT
void* pv_hashmap_insert(PvHashmap* self, void* key, void* value) {
  if (self->array_length <= self->number_of_elements) {
    reallocate_memory(self);
  }

  size_t hash = self->hasher(key);
  size_t bucket_idx = hash & MASK_OF_SIZE(self->array_length);

  PvLinkedList* bucket = &self->array[bucket_idx];

  for (
      PvLinkedListNode* it = bucket->first;
      it != NULL;
      it = it->next) {
    HashmapNode* node = it->data;
    if (self->equals(key, node->key)) {
      void* old_value = node->value;
      node->value = value;
      return old_value;
    }
  }

  HashmapNode* node = malloc(sizeof *node);
  if (!node) {
    abort();
  }
  node->key = key;
  node->hash = hash;
  node->value = value;

  pv_linked_list_push(bucket, node);
  ++self->number_of_elements;

  return NULL;
}

PV_EXPORT
void pv_hashmap_delete(
    PvHashmap* self,
    PvDeleter* key_deleter,
    PvDeleter* value_deleter) {
  for (
      PvLinkedList* list = self->array;
      list != self->array + self->array_length;
      ++list) {
    for (
        PvLinkedListNode* it = list->first;
        it != NULL;
        it = it->next) {
      HashmapNode* node = it->data;
      key_deleter(node->key);
      value_deleter(node->value);
      free(node);
    }
    pv_linked_list_delete(list, pv_nothing_deleter);
  }
}
