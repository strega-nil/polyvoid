#include <pv/hashmap.h>

#include <stdbool.h>
#include <stdlib.h>

#include "utils.c"

// returns false if not unique
// returns true if unique
static
bool insert_unique(PvHashmap* hm, int key, int value) {
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

int main() {
  PvHashmap hm = pv_hashmap_new(int_hash, int_equals);

  for (int i = 0; i < 100; ++i) {
    bool is_unique = insert_unique(&hm, i, i + 1);
    if (!is_unique) {
      return 1;
    }
  }

  for (int i = 0; i < 100; ++i) {
    int* value = pv_hashmap_get(&hm, &i);
    if (!value) {
      return 2;
    }
    if (*value != i + 1) {
      return 3;
    }
  }

  return 0;
}
