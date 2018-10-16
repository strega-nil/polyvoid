#include <pv/hashmap.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static size_t int_hash(void const* _integer) {
  int const* integer = _integer;
  return *integer;
}
static bool int_equals(void const* _lhs, void const* _rhs) {
  int const* lhs = _lhs;
  int const* rhs = _rhs;
  return *lhs == *rhs;
}

int* new_int(int value) {
  int* ret = malloc(sizeof value);
  if (ret == 0) {
    abort();
  }
  *ret = value;
  return ret;
}

int main() {
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
