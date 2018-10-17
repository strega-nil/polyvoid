#include <stdbool.h>
#include <stddef.h>

static inline size_t int_hash(void const* _integer) {
  int const* integer = _integer;
  return *integer;
}
static inline bool int_equals(void const* _lhs, void const* _rhs) {
  int const* lhs = _lhs;
  int const* rhs = _rhs;
  return *lhs == *rhs;
}

// returns false if not unique
// returns true if unique
static inline bool insert_unique(PvHashmap* hm, int key, int value) {
  int* key_ptr = malloc(sizeof key);
  int* value_ptr = malloc(sizeof value);

  if (!key_ptr || !value_ptr) {
    abort();
  }

  *key_ptr = key;
  *value_ptr = value;

  int* old_value = pv_hashmap_insert(hm, key_ptr, value_ptr);

  if (old_value) {
    return false;
  }

  return true;
}
