/* A planted mistake (defect 2414): `lib_set` takes three arguments, and
 * this header's own call gives it two. */
#ifndef OSENV_ENV_H
#define OSENV_ENV_H

#include "lib.h"

static inline int osenv_set(const char *name, const char *value) {
    return lib_set(name, value);
}

#endif
