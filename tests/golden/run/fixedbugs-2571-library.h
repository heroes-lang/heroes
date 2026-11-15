/* Defect 2571: a C library that installs its SIGTRAP and SIGILL handler before
 * `main`, from its constructor, so the Heroes runtime's lease handler finds it
 * and saves it as the handler before its own.
 *
 * POSIX. The library makes traps ON PURPOSE and takes each back the way every
 * recovery from a signal is written, by editing the interrupted context and
 * returning: it answers a trap by writing 5 into the result register and
 * sending the thread past the trap, and it steps over an illegal instruction.
 * A trap it did not make it declines the way Go's archive declines a fault: it
 * calls the handler it found before its own where that is a function, and
 * otherwise puts it back and returns, so the trap runs again under the
 * default and the process dies of it.
 *
 * Every trap it makes is an instruction of its own in assembly, whose address
 * it writes down just before it runs, so the handler sends the thread to the
 * instruction after it however the platform reports the pc: at the trap for
 * `brk`, `udf` and `ud2`, past it for an x86-64 `int3` on Linux (measured in
 * the Linux x86-64 image the day the defect was repaired).
 *
 * Windows. A trap is an exception there, and the runtime's lease arm is a
 * filter of last resort that calls the filter it found first and keeps that
 * filter's answer when it is to continue. So this half sets its filter from
 * its constructor too, and recovers and declines the same shapes through it. */
#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1 /* glibc names the register file's slots only under it */
#endif
#include <stdint.h>

/* The address of the trap the library is about to make. */
static void *volatile lib_trap_at = 0;

/* What the library is doing, so its handler knows a trap as its own: 0 none,
 * 1 a trap it answers with 5, 2 an illegal instruction it steps over. */
static volatile int lib_making = 0;

#if defined(__aarch64__) || defined(_M_ARM64)
#define LIB_TRAP_LENGTH 4 /* brk #0x7 */
#define LIB_ILL_LENGTH 4  /* udf #0 */
#else
#define LIB_TRAP_LENGTH 1 /* int3 */
#define LIB_ILL_LENGTH 2  /* ud2 */
#endif

/* A trap the library answers: SIGTRAP on both architectures, `brk` on arm64
 * and `int3` on x86-64. The result register holds 0 at the trap and the
 * handler writes 5 into it, so 5 is what only the handler can return. */
static __attribute__((noinline)) int64_t lib_trap_answered(void) {
    int64_t v;
    lib_making = 1;
#if defined(__aarch64__) || defined(_M_ARM64)
    __asm__ volatile("adr x9, 1f\n\tstr x9, [%1]\n\tmov x0, #0\n1:\tbrk #0x7\n\tmov %0, x0"
                     : "=r"(v)
                     : "r"(&lib_trap_at)
                     : "x0", "x9", "memory");
#else
    __asm__ volatile("leaq 1f(%%rip), %%rcx\n\tmovq %%rcx, (%1)\n\txorl %%eax, %%eax\n1:\tint3"
                     : "=a"(v)
                     : "r"(&lib_trap_at)
                     : "rcx", "memory");
#endif
    return v;
}

/* An illegal instruction the library steps over: SIGILL on both, `udf` on
 * arm64 and `ud2` on x86-64. The 7 it holds before is what it returns after. */
static __attribute__((noinline)) int64_t lib_illegal_stepped(void) {
    int64_t v = 7;
    lib_making = 2;
#if defined(__aarch64__) || defined(_M_ARM64)
    __asm__ volatile("adr x9, 1f\n\tstr x9, [%1]\n1:\tudf #0" : "+r"(v) : "r"(&lib_trap_at) : "x9", "memory");
#else
    __asm__ volatile("leaq 1f(%%rip), %%rcx\n\tmovq %%rcx, (%1)\n1:\tud2" : "+r"(v) : "r"(&lib_trap_at) : "rcx", "memory");
#endif
    return v;
}

/* A trap the library did not make, which its handler declines:
 * `__builtin_trap()`, `brk` on arm64 (SIGTRAP) and `ud2` on x86-64 (SIGILL). */
static __attribute__((noinline)) void lib_trap_declined(void) {
    lib_making = 0;
    __builtin_trap();
}

/* The library's own thread: which of the two it makes there, and its answer. */
static int lib_task = 0;
static int64_t lib_result = 0;

#if defined(_WIN32)
#include <windows.h>

static LPTOP_LEVEL_EXCEPTION_FILTER lib_found = NULL;

static LONG WINAPI lib_filter(EXCEPTION_POINTERS *ep) {
    int making = lib_making;
    lib_making = 0;
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    CONTEXT *c = ep->ContextRecord;
    DWORD64 at = (DWORD64)(uintptr_t)lib_trap_at;
    if (making == 1 && code == EXCEPTION_BREAKPOINT) {
#if defined(_M_ARM64)
        c->Pc = at + LIB_TRAP_LENGTH;
        c->X0 = 5;
#else
        c->Rip = at + LIB_TRAP_LENGTH;
        c->Rax = 5;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (making == 2 && code == EXCEPTION_ILLEGAL_INSTRUCTION) {
#if defined(_M_ARM64)
        c->Pc = at + LIB_ILL_LENGTH;
#else
        c->Rip = at + LIB_ILL_LENGTH;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return lib_found != NULL ? lib_found(ep) : EXCEPTION_CONTINUE_SEARCH;
}

__attribute__((constructor)) static void lib_install(void) { lib_found = SetUnhandledExceptionFilter(lib_filter); }

static DWORD WINAPI lib_body(LPVOID arg) {
    (void)arg;
    lib_result = lib_task == 0 ? lib_trap_answered() : lib_illegal_stepped();
    return 0;
}

/* 0 the trap it answers, 1 the illegal instruction it steps over, made on a
 * thread the library starts. */
static int64_t lib_on_own_thread(int64_t task) {
    lib_task = (int)task;
    HANDLE t = CreateThread(NULL, 0, lib_body, NULL, 0, NULL);
    if (t == NULL) return -1;
    WaitForSingleObject(t, INFINITE);
    CloseHandle(t);
    return lib_result;
}

#else
#include <pthread.h>
#include <signal.h>
#include <string.h>
#if defined(__APPLE__)
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

static struct sigaction lib_found_trap;
static struct sigaction lib_found_ill;

/* The two registers a recovery here moves: the pc, and the result register. */
static void lib_resume_at(ucontext_t *uc, uintptr_t pc) {
#if defined(__APPLE__) && defined(__aarch64__)
    uc->uc_mcontext->__ss.__pc = pc;
#elif defined(__APPLE__)
    uc->uc_mcontext->__ss.__rip = pc;
#elif defined(__aarch64__)
    uc->uc_mcontext.pc = pc;
#else
    uc->uc_mcontext.gregs[REG_RIP] = (greg_t)pc;
#endif
}

static void lib_answer(ucontext_t *uc, int64_t v) {
#if defined(__APPLE__) && defined(__aarch64__)
    uc->uc_mcontext->__ss.__x[0] = (uint64_t)v;
#elif defined(__APPLE__)
    uc->uc_mcontext->__ss.__rax = (uint64_t)v;
#elif defined(__aarch64__)
    uc->uc_mcontext.regs[0] = (uint64_t)v;
#else
    uc->uc_mcontext.gregs[REG_RAX] = (greg_t)v;
#endif
}

static void lib_handler(int sig, siginfo_t *si, void *ctx) {
    int making = lib_making;
    lib_making = 0;
    ucontext_t *uc = (ucontext_t *)ctx;
    uintptr_t at = (uintptr_t)lib_trap_at;
    if (making == 1 && sig == SIGTRAP) {
        lib_resume_at(uc, at + LIB_TRAP_LENGTH);
        lib_answer(uc, 5);
        return;
    }
    if (making == 2 && sig == SIGILL) {
        lib_resume_at(uc, at + LIB_ILL_LENGTH);
        return;
    }
    struct sigaction *found = sig == SIGILL ? &lib_found_ill : &lib_found_trap;
    if ((found->sa_flags & SA_SIGINFO) != 0 && found->sa_sigaction != NULL) {
        found->sa_sigaction(sig, si, ctx);
        return;
    }
    if (found->sa_handler != SIG_DFL && found->sa_handler != SIG_IGN && found->sa_handler != NULL) {
        found->sa_handler(sig);
        return;
    }
    sigaction(sig, found, NULL);
}

__attribute__((constructor)) static void lib_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = lib_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTRAP, &sa, &lib_found_trap);
    sigaction(SIGILL, &sa, &lib_found_ill);
}

static void *lib_body(void *arg) {
    lib_result = lib_task == 0 ? lib_trap_answered() : lib_illegal_stepped();
    return arg;
}

/* 0 the trap it answers, 1 the illegal instruction it steps over, made on a
 * thread the library starts, which holds no alternate signal stack. */
static int64_t lib_on_own_thread(int64_t task) {
    pthread_t t;
    lib_task = (int)task;
    if (pthread_create(&t, NULL, lib_body, NULL) != 0) return -1;
    pthread_join(t, NULL);
    return lib_result;
}
#endif
