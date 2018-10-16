#ifndef PV_COMMON_H
#define PV_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "_macros.h"

PV_BEGIN_HEADER

typedef void PvDeleter(void* value);

typedef size_t PvHasher(void const*);
typedef bool PvEquals(void const*, void const*);

PV_INLINE_FUNCTION
void pv_nothing_deleter(void* __pv_parm) { (void)__pv_parm; }

PV_END_HEADER

#endif
