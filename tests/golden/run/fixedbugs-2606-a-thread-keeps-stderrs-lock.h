/* Defect 2606: the program's own C for its case. A thread of the program's
 * takes stderr's lock and keeps it, as a writer that never returns from its
 * write keeps it, while the descriptor itself stays writable. POSIX's
 * `flockfile`, which a platform without it lacks: such a platform is told the
 * header's name is missing and skips the case (`absence.hero`). Nothing here
 * allocates or prints. */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/* Takes stderr's lock and keeps it while `n` is not negative, which is for as
 * long as the program lives. */
static inline int64_t keep_stderr(int64_t n) {
    flockfile(stderr);
    while (n >= 0) sleep(1);
    funlockfile(stderr);
    return n;
}
