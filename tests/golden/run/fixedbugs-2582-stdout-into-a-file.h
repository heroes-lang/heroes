/* Defect 2582: the program's own C for its case. It points the process's
 * stdout at a file for a while and back, so the program can read what its two
 * threads printed and judge every line, where the harness would see only an
 * order of lines that differs from run to run. Each switch flushes stdout
 * first, so a line goes to the file it was printed for. */
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/* stdout written into `path`, emptied first: the descriptor stdout was, to
 * hand back to `stdout_back`, or -1 where the system refused a step. */
static inline int64_t stdout_into(const char *path) {
    if (fflush(stdout) != 0) return -1;
    int saved = dup(1);
    if (saved < 0) return -1;
    int file = open(path, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    if (file < 0) {
        (void)close(saved);
        return -1;
    }
    if (dup2(file, 1) < 0) {
        (void)close(file);
        (void)close(saved);
        return -1;
    }
    (void)close(file);
    return saved;
}

/* stdout as it was before `stdout_into`, what was printed meanwhile in the
 * file: 1, or 0 where the system refused a step. */
static inline int64_t stdout_back(int64_t saved) {
    if (fflush(stdout) != 0) return 0;
    if (dup2((int)saved, 1) < 0) return 0;
    (void)close((int)saved);
    return 1;
}
