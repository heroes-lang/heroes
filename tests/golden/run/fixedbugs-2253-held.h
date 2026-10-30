/* Defect 2253's record: 128 MiB, past the 8 MiB of Darwin's and Linux's
 * `main` and the 64 MiB every Windows program is linked with, so the frame
 * that holds it passes every leg's stack at both levels.
 *
 * One word is named and the rest left to the struct (`partial` in the cases),
 * so no comparison or hash of the program's walks its sixteen million words.
 * It is handed to C by value and read through a pointer the optimiser cannot
 * see into, so its bytes exist at `-O2` as at `-O0`: the copy is the caller's,
 * and its address leaves for a function nothing in the unit can name. */
#include <stdint.h>

typedef struct heroes_held {
    int64_t first;
    int64_t rest[(1 << 24) - 1];
} heroes_held;

static int64_t heroes_held_read(const heroes_held *h) { return h->first; }
static int64_t (*volatile heroes_held_reader)(const heroes_held *) = heroes_held_read;

static inline int64_t heroes_held_first(heroes_held h) { return heroes_held_reader(&h); }
