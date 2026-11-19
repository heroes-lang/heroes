/* Defect 2552 (panel 251's R2) and defect 2587: the program's own C for its
 * cases. It makes the process's stdout the write end of a pipe the program
 * keeps open and never reads, as a pager not yet scrolled does, so a thread
 * that prints fills it and then blocks inside `write`, holding stdout's lock
 * for as long as the program lives. The read end is kept open on purpose, so
 * no write meets SIGPIPE; nothing here allocates or prints. */
#include <stdint.h>
#include <unistd.h>

/* 1 once stdout is the unread pipe, 0 where the system refused either call. */
static inline int64_t unread_stdout(void) {
    int ends[2];

    if (pipe(ends) != 0) return 0;
    if (dup2(ends[1], 1) < 0) return 0;
    (void)close(ends[1]);
    return 1;
}
