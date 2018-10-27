#ifndef PV_COMMON_H
#define PV_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "_macros.h"

PV_BEGIN_HEADER

typedef void PvDeleter(void*);

typedef size_t PvHasher(void const*);
typedef bool PvEquals(void const*, void const*);

PV_EXPORT
void pv_nothing_deleter(void*);

PV_END_HEADER

#endif
