#include <pv/hashmap.h>

PV_EXPORT
PvHashmap pv_hashmap_new(PvHasher* hasher, PvEquals* equals) {
  PvHashmap ret = {.hasher = hasher, .equals = equals};

  return ret;
}
