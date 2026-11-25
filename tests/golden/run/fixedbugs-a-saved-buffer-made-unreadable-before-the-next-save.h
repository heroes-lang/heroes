/* fixedbugs-a-saved-buffer-made-unreadable-before-the-next-save.h: a save point fills a buffer, and right after it
 * returns the program makes the buffer unreadable and never jumps to it; then another save point and a signal's
 * handler run, which is where route A reads the buffers saved before (the runtime's record of the save points,
 * where the C library's layout does not verify). `how` 0: a `jmp_buf` in a page of static memory made unreadable
 * with `mprotect`, the page found inside a static array at run time, so no alignment the linker may refuse is
 * asked; 1: the same with `getcontext`; 2: a `jmp_buf` in a page registered as the alternate stack, never used
 * as one, unmapped while registered (the alternate stack before it is registered again at the end); 3: the same with
 * `getcontext`. Correct C. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

static unsigned char ur_static[(size_t)3 << 16];
static volatile int ur_seen;
static void ur_handler(int sig) {
    (void)sig;
    ur_seen += 1;
}

__attribute__((noinline)) static int64_t ur_run(int32_t how) {
    size_t size = (size_t)1 << 16;
    uintptr_t page = (uintptr_t)sysconf(_SC_PAGESIZE);
    if (page == 0 || page > size || (page & (page - 1)) != 0) return -3;
    void *mem = (void *)(((uintptr_t)ur_static + page - 1) & ~(page - 1));
    if (how < 2) size = page;
    stack_t was;
    memset(&was, 0, sizeof was);
    if (how >= 2) {
        mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (mem == MAP_FAILED) return -1;
        stack_t alt;
        alt.ss_sp = mem;
        alt.ss_size = size;
        alt.ss_flags = 0;
        if (sigaltstack(&alt, &was) != 0) return -5;
    }
    if (how == 0 || how == 2) {
        if (setjmp(*(jmp_buf *)mem) != 0) return -6;
    } else {
        if (getcontext((ucontext_t *)mem) != 0) return -6;
    }
    if (how >= 2) {
        munmap(mem, size);
    } else {
        mprotect(mem, size, PROT_NONE);
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = ur_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    jmp_buf other;
    if (setjmp(other) != 0) return -7;
    raise(SIGUSR1);
    if (how < 2) mprotect(mem, size, PROT_READ | PROT_WRITE);
    if (how >= 2 && sigaltstack(&was, NULL) != 0) return -8;
    return ur_seen == 1 ? 7 : -9;
}
