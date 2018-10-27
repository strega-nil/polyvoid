#include <stdbool.h>
#include <stddef.h>

size_t int_hash(void const* _integer) {
  int const* integer;

  integer = _integer;
  return *integer;
}
bool int_equals(void const* _lhs, void const* _rhs) {
  int const* lhs;
  int const* rhs;

  lhs = _lhs;
  rhs = _rhs;
  return *lhs == *rhs;
}

/*
 returns false if not unique
 returns true if unique
*/
bool insert_unique(PvHashmap* hm, int key, int value) {
  int* key_ptr;
  int* value_ptr;
  int* old_value;

  key_ptr = malloc(sizeof key);
  value_ptr = malloc(sizeof value);

  if (!key_ptr || !value_ptr) {
    abort();
  }

  *key_ptr = key;
  *value_ptr = value;

  old_value = pv_hashmap_insert(hm, key_ptr, value_ptr);

  if (old_value) {
    return false;
  }

  return true;
}
