/* Defect 2622: a thread holding the dynamic loader for ever, beside a fault
 * the runtime tells (2026-11-20).
 *
 * POSIX: the thread `dlopen`s a FIFO the program made, and the loader opens
 * it and reads for its header while holding the lock `dladdr` takes (glibc's
 * `_dl_open`, then `_dl_map_object` and `open_verify`); the program opens the
 * other end once the loader has its own open, so that open returns, and
 * writes nothing, so the read waits for ever; and the FIFO is unlinked, so
 * nothing is left behind. Darwin's loader gives the FIFO back at once
 * (measured on this Mac: `dlopen` returned NULL, and `dladdr` answered at
 * once), so there it holds nothing and the case is its fault alone.
 *
 * Windows: the thread takes the loader's lock itself, `LdrLockLoaderLock`,
 * as a thread inside a DLL's entry point that never returns holds it.
 *
 * 1 where the loader was reached, 0 where it could not be.
 *
 * `fixedbugs_2622_hold_loader_beside_a_fault` holds it too, but for the one
 * leg where the fault's report is not the runtime's and asks the loader
 * itself: under AddressSanitizer on Windows a null read is ASan's to name,
 * and its report waits on the loader's lock before its stack (measured on the
 * box, clang 23.1.1, `--sanitize -O0`: `access-violation on unknown address`
 * and two lines more, then nothing until a 30 s kill). There it holds
 * nothing and answers 1, so the leg judges ASan's words alone; on Linux the
 * same leg ended with its report while the loader was held. */
#include <stdint.h>
#include <string.h>

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define FIXEDBUGS_2622_SANITIZED 1
#endif
#endif
#if defined(__SANITIZE_ADDRESS__) && !defined(FIXEDBUGS_2622_SANITIZED)
#define FIXEDBUGS_2622_SANITIZED 1
#endif

#if defined(_WIN32)
#include <windows.h>

typedef LONG(NTAPI *fixedbugs_2622_lock)(ULONG flags, ULONG *disposition, ULONG_PTR *cookie);

static DWORD WINAPI fixedbugs_2622_holder(LPVOID ready) {
    fixedbugs_2622_lock lock = (fixedbugs_2622_lock)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "LdrLockLoaderLock");
    ULONG_PTR cookie = 0;
    LONG held = lock != NULL ? lock(0, NULL, &cookie) : -1;
    if (held >= 0) SetEvent((HANDLE)ready);
    Sleep(INFINITE);
    return 0;
}

static inline int64_t fixedbugs_2622_hold_loader(void) {
    HANDLE ready = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (ready == NULL) return 0;
    HANDLE t = CreateThread(NULL, 0, fixedbugs_2622_holder, ready, 0, NULL);
    if (t == NULL) return 0;
    CloseHandle(t);
    return WaitForSingleObject(ready, 10000) == WAIT_OBJECT_0 ? 1 : 0;
}
#else
#include <dlfcn.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static char fixedbugs_2622_path[64];

static void *fixedbugs_2622_opener(void *arg) {
    (void)arg;
    (void)dlopen(fixedbugs_2622_path, RTLD_NOW);
    return NULL;
}

static inline int64_t fixedbugs_2622_hold_loader(void) {
    snprintf(fixedbugs_2622_path, sizeof fixedbugs_2622_path, "/tmp/heroes-2622-%ld", (long)getpid());
    (void)unlink(fixedbugs_2622_path);
    if (mkfifo(fixedbugs_2622_path, 0600) != 0) return 0;
    pthread_t t;
    if (pthread_create(&t, NULL, fixedbugs_2622_opener, NULL) != 0) {
        (void)unlink(fixedbugs_2622_path);
        return 0;
    }
    (void)pthread_detach(t);
    /* Opened for writing, without waiting, once the loader has the FIFO open
     * for reading, and never written: at most 10 s. */
    int fd = -1;
    for (int i = 0; i < 10000 && fd < 0; i++) {
        fd = open(fixedbugs_2622_path, O_WRONLY | O_NONBLOCK | O_CLOEXEC);
        if (fd < 0) {
            struct timespec ms = {0, 1000000};
            nanosleep(&ms, NULL);
        }
    }
    (void)unlink(fixedbugs_2622_path);
    return fd >= 0 ? 1 : 0;
}
#endif

static inline int64_t fixedbugs_2622_hold_loader_beside_a_fault(void) {
#if defined(_WIN32) && defined(FIXEDBUGS_2622_SANITIZED)
    return 1;
#else
    return fixedbugs_2622_hold_loader();
#endif
}

/* A null read inside the C library's own code. */
static inline int64_t fixedbugs_2622_null_in_libc(void) {
    const char *volatile p = NULL;
    return (int64_t)strlen(p);
}
