/* The package `qt`'s header: it reads `qt_wide.h` only where the unit
   including it asked first, a file `qt` does not ship (defect 2556). */
#include <stdint.h>
#if defined(QT_ASK_WIDE)
#include "qt_wide.h"
#endif

typedef struct qt_point {
    int64_t x;
    int64_t y;
} qt_point;

static inline qt_point qt_make(int64_t x, int64_t y) {
    qt_point p = { x, y };
    return p;
}
