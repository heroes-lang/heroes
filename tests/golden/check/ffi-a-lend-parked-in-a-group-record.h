#ifndef FFI_A_LEND_PARKED_IN_A_GROUP_RECORD_H
#define FFI_A_LEND_PARKED_IN_A_GROUP_RECORD_H

#include <stdint.h>

/* A group's record holding a `cstr` is legal by § 13 — there the fields are
   the header's. What defect 067 found is that BUILDING one is an argument
   position that keeps rather than reads. */
typedef struct {
    const char *p;
} Box;

static inline int64_t read_it(const char *p) {
    return p ? (int64_t)(unsigned char)p[0] : -1;
}

#endif
