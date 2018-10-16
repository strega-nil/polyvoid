#include <stdbool.h>
#include <stddef.h>

static inline
size_t int_hash(void const* _integer) {
  int const* integer = _integer;
  return *integer;
}
static inline
bool int_equals(void const* _lhs, void const* _rhs) {
  int const* lhs = _lhs;
  int const* rhs = _rhs;
  return *lhs == *rhs;
}
