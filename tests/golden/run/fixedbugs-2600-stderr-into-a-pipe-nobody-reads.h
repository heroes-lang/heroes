/* Defect 2600: the program's own C for its stderr case, defect 2552's header
 * for stdout written for stderr. It makes the process's stderr the write end
 * of a pipe the program keeps open and never reads, so a thread that writes
 * there fills it and then blocks inside `write`, holding stderr's lock for as
 * long as the program lives. The read end is kept open on purpose, so no write
 * meets SIGPIPE; nothing here allocates or prints. */
#include <stdint.h>
#include <unistd.h>

/* 1 once stderr is the unread pipe, 0 where the system refused either call. */
static inline int64_t unread_stderr(void) {
    int ends[2];

    if (pipe(ends) != 0) return 0;
    if (dup2(ends[1], 2) < 0) return 0;
    (void)close(ends[1]);
    return 1;
}
