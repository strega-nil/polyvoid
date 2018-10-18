#include <pv/hashmap.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.c"

int* new_int(int value) {
  int* ret = malloc(sizeof value);
  if (ret == 0) {
    abort();
  }
  *ret = value;
  return ret;
}

int main(void) {
  PvHashmap hm = pv_hashmap_new(int_hash, int_equals);

  {
    int* key = new_int(0);
    int* value = new_int(1);
    pv_hashmap_insert(&hm, key, value);

    key = new_int(1);
    value = new_int(2);
    pv_hashmap_insert(&hm, key, value);
  }

  {
    int key = 0;
    int* value = pv_hashmap_get(&hm, &key);
    if (!value) {
      return 2;
    }
    if (*value != 1) {
      return 1;
    }

    key = 1;
    value = pv_hashmap_get(&hm, &key);
    if (!value) {
      return 2;
    }
    if (*value != 2) {
      return 1;
    }
  }

  pv_hashmap_delete(&hm, free, free);

  return 0;
}
