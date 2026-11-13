/* Defect 2494: the program's own C for its case. A signal handler written in
 * C, which only counts in a lock-free atomic, what a handler may touch, and
 * the count read back. */
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <unistd.h>

static _Atomic int64_t counted = 0;

static void count_one(int sig) {
    (void)sig;
    atomic_fetch_add(&counted, 1);
}

static inline int64_t counted_so_far(void) { return atomic_load(&counted); }
