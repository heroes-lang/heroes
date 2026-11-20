/* Defect 2606: the program's own C for its case on every platform, POSIX's
 * calls and Windows' side by side, so the shape the POSIX-only cases bind
 * through `unistd.h` is judged on the Windows box too (platforms.md, a twin
 * on a header of the program's own). A thread takes stderr's lock and keeps
 * it, as a writer that never returns from its write keeps it, while the
 * descriptor itself stays writable. Nothing here allocates or prints. */
#include <stdint.h>
#include <stdio.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Waits `us` microseconds, to the millisecond on Windows. */
static inline int64_t pause_us(int64_t us) {
#if defined(_WIN32)
    Sleep((DWORD)(us / 1000));
#else
    usleep((useconds_t)us);
#endif
    return 0;
}

/* Takes stderr's lock and keeps it while `n` is not negative, which is for as
 * long as the program lives: `flockfile` on POSIX, `_lock_file` on Windows. */
static inline int64_t keep_stderr(int64_t n) {
#if defined(_WIN32)
    _lock_file(stderr);
    while (n >= 0) Sleep(1000);
    _unlock_file(stderr);
#else
    flockfile(stderr);
    while (n >= 0) sleep(1);
    funlockfile(stderr);
#endif
    return n;
}
