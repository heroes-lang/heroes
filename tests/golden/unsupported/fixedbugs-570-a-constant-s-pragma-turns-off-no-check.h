/* Defect 570, 2026-10-10: a constant whose value holds a pragma. */
#include <stdlib.h>
#define QUIET_FIVE _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") 5
