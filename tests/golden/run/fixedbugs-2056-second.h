/* Defect 2056, 2026-10-14: the second, whose group binds the shared file's
 * macros: its include is skipped, the file read already. */
#include "fixedbugs-2056-common.h"
static inline int bee(void) { return 2; }
