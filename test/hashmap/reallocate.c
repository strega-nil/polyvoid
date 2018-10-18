#include <pv/hashmap.h>

#include <stdbool.h>
#include <stdlib.h>

#include "utils.c"

int main(void) {
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

  pv_hashmap_delete(&hm, free, free);

  return 0;
}
