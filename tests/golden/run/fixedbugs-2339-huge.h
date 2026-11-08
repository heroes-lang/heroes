/* A record of one gibibyte and eight bytes (defect 2339), past the room large
 * values take off the stack, one gibibyte for the whole program
 * (`runtime/parts/frame.c`), so the prologue of a function holding one asks
 * for more than the room can hold and stops before taking anything. Never
 * built: the stop comes first. */
#include <stdint.h>

typedef struct heroes_huge {
    int64_t first;
    int64_t rest[1 << 27];
} heroes_huge;
