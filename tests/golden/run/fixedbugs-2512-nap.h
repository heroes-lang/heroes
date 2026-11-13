/* Defect 2512's child: a nap of `ms` milliseconds, on every platform, which
 * load can only lengthen, so a child that naps a second has lived at least a
 * second whatever the machine is doing. Windows' `Sleep`, elsewhere
 * `nanosleep` asked again for what a signal left. */
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

static inline void case2512_nap(int64_t ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec left = { (time_t)(ms / 1000), (long)((ms % 1000) * 1000000) };

    while (nanosleep(&left, &left) != 0) {
    }
#endif
}
