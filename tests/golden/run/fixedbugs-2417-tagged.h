/* The header of the library, defect 2409's shape: a union named above ASCII. */
#include <stdint.h>
typedef union { int32_t fd; void *p; } unioné;
struct tagged { int32_t kind; unioné via; };
