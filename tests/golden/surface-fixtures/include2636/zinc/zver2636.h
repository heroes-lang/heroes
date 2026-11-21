/* Defect 2636: a header of the fixture's own declaring a name the platform's
 * zlib defines, standing for another release's header, which the package
 * `zp` binds and does not ship. */
#ifndef ZVER2636_H
#define ZVER2636_H
#include <stdint.h>

typedef struct zver_mark {
    int64_t major;
    int64_t minor;
} zver_mark;

const char *zlibVersion(void);
#endif
