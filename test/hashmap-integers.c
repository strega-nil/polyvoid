#include <pv/hashmap.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static size_t int_hash(void const* integer) { return *(int*)integer; }
static bool int_equals(void const* lhs, void const* rhs) { return lhs == rhs; }

int main() {
  PvHashmap hm = pv_hashmap_new(int_hash, int_equals);
  (void)hm;
  return 0;
}