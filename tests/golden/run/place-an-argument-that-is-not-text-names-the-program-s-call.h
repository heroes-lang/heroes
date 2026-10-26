/* Runs the program at `program` with one argument, the bytes 0xff 0xfe, which no
 * Heroes `str` can hold, waits for it, and answers its status as a shell reads
 * it, 128 and the signal for a death by one. POSIX alone: a platform without
 * `spawn.h` judges the case where the header is (`.claude/rules/platforms.md`).
 * `static inline` and `noinline` as every header in this directory. */
#include <spawn.h>
#include <stdint.h>
#include <sys/wait.h>

extern char **environ;

static inline __attribute__((noinline)) int64_t spawn_self_unreadable(const char *program) {
    char name[4096];
    char unreadable[3] = {(char)0xff, (char)0xfe, 0};
    size_t n = 0;
    while (program[n] != 0 && n + 1 < sizeof name) {
        name[n] = program[n];
        n++;
    }
    name[n] = 0;
    char *argv[3] = {name, unreadable, 0};
    pid_t pid;
    if (posix_spawn(&pid, name, 0, 0, argv, environ) != 0) return 1;
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return 1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return 1;
}
