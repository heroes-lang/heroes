/* Defect 2607: the program's own C for its case. Two flags, each raised by one
 * thread and waited for by the other, the wait bounded at 5 s, so a case can
 * hold a worker running until `main` has printed and ask whether it saw that
 * in time. Nothing here allocates or prints. */
#include <stdatomic.h>
#include <stdint.h>
#include <unistd.h>

static atomic_int two_flags[2];

/* Raises flag `which`, 0 or 1. */
static inline void flag_raise(int64_t which) { atomic_store(&two_flags[which & 1], 1); }

/* 1 once flag `which` is raised, 0 where 5 s passed first. */
static inline int64_t flag_waited(int64_t which) {
    for (int i = 0; i < 500; i++) {
        if (atomic_load(&two_flags[which & 1]) != 0) return 1;
        usleep(10000);
    }
    return 0;
}
