/* Defect 2558: a C library that installs its fault handler before `main`, as
 * Go's c-archive does from its constructor, so the Heroes runtime's guard
 * finds it and saves it as the handler before its own.
 *
 * POSIX. The library makes a null read and a null call ON PURPOSE on a thread
 * of its own that holds an alternate signal stack, as every thread that runs
 * Go code does, and takes each back the way Go takes a nil dereference back:
 * its handler edits the faulting context and returns (it steps over the read,
 * and it returns from the call with 9), or it jumps out with `siglongjmp`. A
 * fault it did not make it declines the way Go's archive does: it calls the
 * handler it found before its own where that is a function (AddressSanitizer's
 * under `--sanitize`), and otherwise puts it back and returns, so the access
 * faults again (Go's `raisebadsignal`); or, once the program asks it to, it
 * ends the process in silence by the default (Go's `dieFromSignal`, its answer
 * on a thread it has never seen or before its own start-up is done, measured
 * on this Mac the day the defect was repaired).
 *
 * Every fault is made by an instruction of its own in assembly, so its length
 * is known to the handler that steps over it, and no sanitizer instruments it.
 *
 * Windows has no such order for the runtime to keep: a vectored handler a
 * library registers first is asked first by the system, on every thread. So
 * this half registers one first and runs every shape on the calling thread:
 * it steps over each read it makes and returns from the call it makes, by the
 * context it is handed, and declines everything else to the runtime's own. */
#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1 /* glibc names the register file's slots only under it */
#endif
#include <stdint.h>

/* A pointer nobody can prove null. */
static const volatile int64_t *volatile lib_null = 0;

/* What the library is doing, so its handler knows a fault as its own: 0 none,
 * 1 a read it steps over, 2 a call it returns from, 3 a read it jumps out of. */
static volatile int lib_making = 0;

/* `*p` read into a value that starts as `start`, by one instruction whose
 * length the handler knows, so stepping over it leaves `start`. */
static __attribute__((noinline)) int64_t lib_read_at(const volatile int64_t *p, int64_t start) {
    int64_t v = start;
#if defined(__aarch64__) || defined(_M_ARM64)
    __asm__ volatile("ldr %0, [%1]" : "+r"(v) : "r"(p) : "memory");
#else
    __asm__ volatile("movq (%%rcx), %%rax" : "+a"(v) : "c"(p) : "memory");
#endif
    return v;
}
#if defined(__aarch64__) || defined(_M_ARM64)
#define LIB_READ_LENGTH 4
#else
#define LIB_READ_LENGTH 3 /* 48 8b 01 */
#endif

/* `p[1]`, a read at offset 8. */
static __attribute__((noinline)) int64_t lib_read_past(const volatile int64_t *p) {
    int64_t v = 0;
#if defined(__aarch64__) || defined(_M_ARM64)
    __asm__ volatile("ldr %0, [%1, #8]" : "=r"(v) : "r"(p) : "memory");
#else
    __asm__ volatile("movq 8(%%rcx), %%rax" : "=a"(v) : "c"(p) : "memory");
#endif
    return v;
}

/* A call to address zero, and what the call gives back in the result register.
 * On x86-64 the call's return address is pushed below the red zone a leaf may
 * keep its values in, and taken back with it. */
static __attribute__((noinline)) int64_t lib_call_none(void) {
    int64_t v;
#if defined(__aarch64__) || defined(_M_ARM64)
    __asm__ volatile("mov x8, #0\n\tblr x8\n\tmov %0, x0" : "=r"(v) : : "x0", "x8", "x30", "memory");
#else
    __asm__ volatile("subq $128, %%rsp\n\txorl %%eax, %%eax\n\tcallq *%%rax\n\taddq $128, %%rsp" : "=a"(v) : : "memory");
#endif
    return v;
}

#if defined(_WIN32)
#include <windows.h>

static LONG WINAPI lib_veh(EXCEPTION_POINTERS *ep) {
    if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_CONTINUE_SEARCH;
    int making = lib_making;
    lib_making = 0;
    CONTEXT *c = ep->ContextRecord;
    if (making == 1 || making == 3) {
#if defined(_M_ARM64)
        c->Pc += LIB_READ_LENGTH;
#else
        c->Rip += LIB_READ_LENGTH;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (making == 2) {
#if defined(_M_ARM64)
        c->Pc = c->Lr;
        c->X0 = 9;
#else
        c->Rip = *(DWORD64 *)c->Rsp;
        c->Rsp += 8;
        c->Rax = 9;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

__attribute__((constructor)) static void lib_install(void) { AddVectoredExceptionHandler(1, lib_veh); }

static void lib_ends_in_silence(void) {}

static int64_t lib_recovered(int64_t which) {
    if (which == 0) {
        lib_making = 1;
        return lib_read_at(lib_null, 7);
    }
    if (which == 1) {
        lib_making = 2;
        return lib_call_none();
    }
    lib_making = 3;
    return lib_read_at(lib_null, 11);
}

static int64_t lib_read_declined(void) { return lib_read_past(lib_null); }
static int64_t lib_call_declined(void) { return lib_call_none(); }
static int64_t lib_read_here(void) { return lib_read_past(lib_null); }
static int64_t lib_read_on_plain_thread(void) { return lib_read_past(lib_null); }

#else
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#if defined(__APPLE__)
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

static struct sigaction lib_found_segv;
static struct sigaction lib_found_bus;
static sigjmp_buf lib_out;
static volatile int lib_silent = 0;

/* The two moves a recovery makes with the interrupted context. */
static void lib_step_over(ucontext_t *uc) {
#if defined(__APPLE__) && defined(__aarch64__)
    uc->uc_mcontext->__ss.__pc += LIB_READ_LENGTH;
#elif defined(__APPLE__)
    uc->uc_mcontext->__ss.__rip += LIB_READ_LENGTH;
#elif defined(__aarch64__)
    uc->uc_mcontext.pc += LIB_READ_LENGTH;
#else
    uc->uc_mcontext.gregs[REG_RIP] += LIB_READ_LENGTH;
#endif
}

static void lib_return_nine(ucontext_t *uc) {
#if defined(__APPLE__) && defined(__aarch64__)
    uc->uc_mcontext->__ss.__pc = uc->uc_mcontext->__ss.__lr;
    uc->uc_mcontext->__ss.__x[0] = 9;
#elif defined(__APPLE__)
    uc->uc_mcontext->__ss.__rip = *(uint64_t *)uc->uc_mcontext->__ss.__rsp;
    uc->uc_mcontext->__ss.__rsp += 8;
    uc->uc_mcontext->__ss.__rax = 9;
#elif defined(__aarch64__)
    uc->uc_mcontext.pc = uc->uc_mcontext.regs[30];
    uc->uc_mcontext.regs[0] = 9;
#else
    uc->uc_mcontext.gregs[REG_RIP] = *(greg_t *)uc->uc_mcontext.gregs[REG_RSP];
    uc->uc_mcontext.gregs[REG_RSP] += 8;
    uc->uc_mcontext.gregs[REG_RAX] = 9;
#endif
}

static void lib_handler(int sig, siginfo_t *si, void *ctx) {
    int making = lib_making;
    lib_making = 0;
    if (making == 1) {
        lib_step_over((ucontext_t *)ctx);
        return;
    }
    if (making == 2) {
        lib_return_nine((ucontext_t *)ctx);
        return;
    }
    if (making == 3) siglongjmp(lib_out, 1);
    struct sigaction *found = sig == SIGBUS ? &lib_found_bus : &lib_found_segv;
    if ((found->sa_flags & SA_SIGINFO) != 0 && found->sa_sigaction != NULL) {
        found->sa_sigaction(sig, si, ctx);
        return;
    }
    if (found->sa_handler != SIG_DFL && found->sa_handler != SIG_IGN && found->sa_handler != NULL) {
        found->sa_handler(sig);
        return;
    }
    sigaction(sig, found, NULL);
    if (lib_silent) {
        sigset_t one;
        sigemptyset(&one);
        sigaddset(&one, sig);
        pthread_sigmask(SIG_UNBLOCK, &one, NULL);
        raise(sig);
    }
}

__attribute__((constructor)) static void lib_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = lib_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigfillset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, &lib_found_segv);
    sigaction(SIGBUS, &sa, &lib_found_bus);
}

static void lib_ends_in_silence(void) { lib_silent = 1; }

/* The library's own thread: what it is asked to do, and its answer. */
static int lib_task = 0;
static int64_t lib_answer = 0;
static int lib_with_stack = 1;
static char lib_alt[256 * 1024];

static void *lib_body(void *arg) {
    /* An alternate stack where none stands, as Go gives one: a thread a
     * sanitizer started holds the sanitizer's, which it unmaps at the
     * thread's end and which is kept. */
    int mine = 0;
    stack_t now;
    if (lib_with_stack && sigaltstack(NULL, &now) == 0 && (now.ss_flags & SS_DISABLE) != 0) {
        stack_t ss;
        memset(&ss, 0, sizeof ss);
        ss.ss_sp = lib_alt;
        ss.ss_size = sizeof lib_alt;
        mine = sigaltstack(&ss, NULL) == 0;
    }
    if (lib_task == 0) {
        lib_making = 1;
        lib_answer = lib_read_at(lib_null, 7);
    } else if (lib_task == 1) {
        lib_making = 2;
        lib_answer = lib_call_none();
    } else if (lib_task == 2) {
        if (sigsetjmp(lib_out, 1) == 0) {
            lib_making = 3;
            lib_answer = lib_read_at(lib_null, 0);
        } else {
            lib_answer = 11;
        }
    } else if (lib_task == 3) {
        lib_answer = lib_read_past(lib_null);
    } else {
        lib_answer = lib_call_none();
    }
    lib_making = 0;
    if (mine) {
        stack_t off;
        memset(&off, 0, sizeof off);
        off.ss_flags = SS_DISABLE;
        sigaltstack(&off, NULL);
    }
    return arg;
}

static int64_t lib_on_own_thread(int task, int with_stack) {
    pthread_t t;
    lib_task = task;
    lib_with_stack = with_stack;
    if (pthread_create(&t, NULL, lib_body, NULL) != 0) return -1;
    pthread_join(t, NULL);
    return lib_answer;
}

/* 0 a read it steps over (7), 1 a call it returns from (9), 2 a read it
 * jumps out of (11), each on its own thread with an alternate stack. */
static int64_t lib_recovered(int64_t which) { return lib_on_own_thread((int)which, 1); }
/* A read at offset 8 and a null call it did not make, on the same thread. */
static int64_t lib_read_declined(void) { return lib_on_own_thread(3, 1); }
static int64_t lib_call_declined(void) { return lib_on_own_thread(4, 1); }
/* A read at offset 8 on the calling thread, the program's own stack. */
static int64_t lib_read_here(void) { return lib_read_past(lib_null); }
/* A read at offset 8 on a thread of the library's with no alternate stack. */
static int64_t lib_read_on_plain_thread(void) { return lib_on_own_thread(3, 0); }
#endif
