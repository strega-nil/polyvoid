#include <pv/hashmap.h>

#include <stdbool.h>
#include <stdlib.h>

#include "utils.c"

int main() {
  PvHashmap hm = pv_hashmap_new(int_hash, int_equals);

  for (int i = 0; i < 100; ++i) {
    insert_unique(&hm, i, i + 1);
  }

  for (int i = 0; i < 100; ++i) {
    PvHashmapRemoveReturn rr = pv_hashmap_remove(&hm, &i);
    if (!rr.key) {
      return 1;
    }
    if (!rr.value) {
      return 2;
    }

    if (*(int*)rr.key != i) {
      return 3;
    }
    if (*(int*)rr.value != i + 1) {
      return 4;
    }

    free(rr.key);
    free(rr.value);
  }

  if (hm.number_of_elements != 0) {
    return 5;
  }
  return 0;
}
