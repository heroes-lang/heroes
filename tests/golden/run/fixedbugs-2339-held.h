/* Defect 2253's record, 128 MiB, crossing C by value (defect 2339): as a
 * result, a parameter, and both. C's convention keeps a by-value record's
 * storage on its caller's frame, an argument's copy and a result's slot
 * alike, so the frame that passes every leg's stack is the calling
 * function's. Each function is reached through a pointer the optimiser cannot
 * see into, so that copy exists at `-O2` as at `-O0`; none of them runs, the
 * caller's frame passing the stack first. */
#include <stdint.h>

typedef struct heroes_held {
    int64_t first;
    int64_t rest[(1 << 24) - 1];
} heroes_held;

static int64_t heroes_held_read(const heroes_held *h) { return h->first; }
static int64_t (*volatile heroes_held_reader)(const heroes_held *) = heroes_held_read;

static heroes_held heroes_held_made(int64_t n) {
    heroes_held h = {0};
    h.first = n;
    return h;
}
static heroes_held (*volatile heroes_held_maker)(int64_t) = heroes_held_made;

static heroes_held heroes_held_bumped(heroes_held h, int64_t by) {
    h.first += by;
    return h;
}
static heroes_held (*volatile heroes_held_bumper)(heroes_held, int64_t) = heroes_held_bumped;

static inline int64_t heroes_held_first(heroes_held h) { return heroes_held_reader(&h); }
static inline heroes_held heroes_held_make(int64_t n) { return heroes_held_maker(n); }
static inline heroes_held heroes_held_bump(heroes_held h, int64_t by) { return heroes_held_bumper(h, by); }
