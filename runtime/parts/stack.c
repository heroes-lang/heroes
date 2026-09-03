/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/stack.c — deep recursion stops with a word (design.md §1.12; CLAUDE.md
 * §12; panel 104, ratified 2026-09-03; M-robustness-guards step 4).
 *
 * Until this file, a Heroes program that ran off the end of its stack died in
 * silence: exit 139 on the Mac and on Linux, and on Windows exit 127 — the
 * number `hero_run_go` also uses for "program not found". Nothing in the
 * language, the runtime or the emitted C saw it; `--sanitize` did, and named
 * the `.hero` line. §1.12 makes not crashing a GOAL of the language, so a
 * five-line recursion that falsified it with no message was the one defect the
 * showcase site could not survive being asked about.
 *
 * WHAT THIS IS. A guard-page handler, installed once from `hero_args_set` — so
 * the emitted `main` and the ABI stamp do not change — that turns the fault
 * into `panic: stack exhausted in <module.function>`, exit 134, on the same
 * path every other abort in the language takes. Zero cost per call: the
 * kernel delivers the fault, and until it does no instruction runs here.
 *
 * WHAT THE TWO SEATS MEASURED AND THIS FILE OBEYS (docs/panel/104):
 *
 * - TWO WITNESSES, both required. On `si_addr` alone a wild store 8 KiB under
 *   the stack was reported as "stack exhausted", a false abort; requiring the
 *   interrupted stack pointer to be down there with it rejected that store and
 *   lost none of the four real overflows.
 * - SAVE AND CHAIN, never reset. A library that installed its own handler in a
 *   constructor (a write barrier, a GC) lost it under "restore the default and
 *   return" and died at 138. The previous disposition is saved at install and
 *   a fault that is not ours is handed to it; only a saved SIG_DFL/SIG_IGN is
 *   reset so the fault re-executes and dies as it always did.
 * - NOTHING UNDER THE SANITIZER. ASan installs its own SIGSEGV/SIGBUS handler
 *   before `main` and its report names the `.hero` line; ours would replace a
 *   richer report with a poorer one. Decided at compile time, below.
 * - SIGBUS AS WELL AS SIGSEGV: a guard-page hit on Darwin is SIGBUS as often
 *   as SIGSEGV (measured 138 twice).
 * - THE ALTERNATE STACK IS SIZED IN BYTES, mmap'd, with a PROT_NONE page below
 *   it. `SIGSTKSZ` is 131,072 here and 8,192 on glibc 2.41, which is a second
 *   overflow inside the first if the handler symbolises; and a signal stack
 *   has no guard page of its own (panel 070's pothole).
 * - THE FRAME WALK NAMES THE FUNCTION. The faulting pc may be inside libc
 *   (`__vfprintf`, measured); the walk follows the frame-pointer chain of the
 *   interrupted stack — intact, since we are on the alternate one — bounded by
 *   the stack range and monotone, until a frame is a Heroes function
 *   (`h_<module>_<name>`, which the mangler makes deterministic, CLAUDE.md §7).
 *   On Linux `dladdr` reads only `.dynsym`, so the link line carries
 *   `-rdynamic` (`selfhost/cli/flags.hero`); without it the message loses the
 *   name and nothing else.
 * - THREADS ARE A RECORDED HOLE, not a fix. The alternate stack is per thread;
 *   a C library's own thread that overflows dies as it did before this file
 *   (138 today, 132 with the handler installed on the main thread only —
 *   measured). M-isolated-threads installs one per runtime-created thread; a
 *   library's thread stays the boundary's.
 *
 * The depth counter the brief offered as a fallback is not here and will not
 * be: +55–60% per call at -O0, and at -O2 it forbids the transformation that
 * turns the recursion into a loop (0.81 s against 0.000 s, measured), for
 * coverage this handler already has.
 *
 * WHAT DEFECT 007 ADDED (docs/defects/007, 2026-09-03). The second witness
 * above was written for a frame that is touched AFTER `sp` moves, and a
 * frame larger than a page is not: clang's prologue calls `___chkstk_darwin`,
 * which probes the frame-to-be page by page while `sp` still stands where the
 * caller left it. `heroes check` on a legal program died at 139 in silence
 * that way while `heroes parse` on the same file panicked with a name — the
 * difference being only which function's frame straddled the guard. The
 * witness now has two shapes (see the handler); the wild store panel 104
 * rejected was re-run against both and still re-raises, exit 139.
 */

#if defined(__has_feature)
#  if __has_feature(address_sanitizer)
#    define HERO_STACK_GUARD_YIELDS_TO_ASAN 1
#  endif
#endif
#if defined(__SANITIZE_ADDRESS__)
#  define HERO_STACK_GUARD_YIELDS_TO_ASAN 1
#endif

#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)

/* ASan owns the signal; its report is the better one. */
static void hero_stack_guard_install(void) {}

#elif !defined(_WIN32)

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/mman.h>
#if defined(__APPLE__)
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

/* The main thread's stack, measured once at startup: `lo` is its lowest
 * address, and the guard region sits just below. */
static uintptr_t hero_stack_lo = 0;
static uintptr_t hero_stack_hi = 0;

/* What was installed before us, for SIGSEGV and SIGBUS. */
static struct sigaction hero_stack_prev_segv;
static struct sigaction hero_stack_prev_bus;

/* How far below `lo` a fault still counts as exhaustion: clang emits no stack
 * probes on Darwin, so a frame larger than the guard page can land past it. */
#define HERO_STACK_WINDOW ((uintptr_t)1 << 20)

static void hero_stack_bounds(void) {
#if defined(__APPLE__)
    pthread_t self = pthread_self();
    uintptr_t top = (uintptr_t)pthread_get_stackaddr_np(self);
    size_t size = pthread_get_stacksize_np(self);
    hero_stack_hi = top;
    hero_stack_lo = top - size;
#else
    pthread_attr_t attr;
    void *addr = NULL;
    size_t size = 0;
    if (pthread_getattr_np(pthread_self(), &attr) == 0) {
        pthread_attr_getstack(&attr, &addr, &size);
        pthread_attr_destroy(&attr);
    }
    hero_stack_lo = (uintptr_t)addr;
    hero_stack_hi = (uintptr_t)addr + size;
#endif
}

/* `write(2)` and nothing else: inside a signal handler, `fprintf` is not ours
 * to call (the faulting thread may hold stdout's lock). */
static void hero_stack_say(const char *s) {
    size_t n = strlen(s);
    while (n > 0) {
        ssize_t put = write(2, s, n);
        if (put <= 0) return;
        s += (size_t)put;
        n -= (size_t)put;
    }
}

static int hero_stack_is_heroes(const char *sym) {
    if (sym == NULL) return 0;
    if (sym[0] == '_') sym++; /* Mach-O's leading underscore */
    return sym[0] == 'h' && sym[1] == '_';
}

/* `h_<module>_<name>[_<hash>]` → `<module>.<name>[_<hash>]`. The mangler
 * sanitises the module to [A-Za-z0-9], so the first `_` after `h_` ends it
 * (CLAUDE.md §7). A symbol that is not a Heroes function is written as it is. */
static void hero_stack_say_heroes_name(const char *sym) {
    if (sym[0] == '_') sym++;
    if (sym[0] == 'h' && sym[1] == '_' && sym[2] != '\0') {
        const char *rest = sym + 2;
        const char *us = strchr(rest, '_');
        if (us != NULL && us[1] != '\0') {
            char module[128];
            size_t n = (size_t)(us - rest);
            if (n >= sizeof module) n = sizeof module - 1;
            memcpy(module, rest, n);
            module[n] = '\0';
            hero_stack_say(module);
            hero_stack_say(".");
            hero_stack_say(us + 1);
            return;
        }
    }
    hero_stack_say(sym);
}

/* The faulting pc, fp and sp from the interrupted context — the only lines in
 * this file that know a register file. An unknown architecture answers zeros,
 * which fails the second witness and re-raises the fault as before. */
static void hero_stack_regs(void *ctx, uintptr_t *pc, uintptr_t *fp, uintptr_t *sp) {
    ucontext_t *uc = (ucontext_t *)ctx;
#if defined(__APPLE__) && defined(__aarch64__)
    *pc = (uintptr_t)uc->uc_mcontext->__ss.__pc;
    *fp = (uintptr_t)uc->uc_mcontext->__ss.__fp;
    *sp = (uintptr_t)uc->uc_mcontext->__ss.__sp;
#elif defined(__APPLE__) && defined(__x86_64__)
    *pc = (uintptr_t)uc->uc_mcontext->__ss.__rip;
    *fp = (uintptr_t)uc->uc_mcontext->__ss.__rbp;
    *sp = (uintptr_t)uc->uc_mcontext->__ss.__rsp;
#elif defined(__linux__) && defined(__x86_64__)
    *pc = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
    *fp = (uintptr_t)uc->uc_mcontext.gregs[REG_RBP];
    *sp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
#elif defined(__linux__) && defined(__aarch64__)
    *pc = (uintptr_t)uc->uc_mcontext.pc;
    *fp = (uintptr_t)uc->uc_mcontext.regs[29];
    *sp = (uintptr_t)uc->uc_mcontext.sp;
#else
    (void)uc;
    *pc = 0;
    *fp = 0;
    *sp = 0;
#endif
}

/* The Heroes function on the interrupted stack nearest the fault: the pc's own
 * symbol if it is one, else the first frame up the chain that is. Every fp is
 * checked against the stack range before it is read, and the walk must go up. */
static const char *hero_stack_blame(uintptr_t pc, uintptr_t fp) {
    Dl_info info;
    const char *first = NULL;
    if (dladdr((void *)pc, &info) != 0) {
        if (hero_stack_is_heroes(info.dli_sname)) return info.dli_sname;
        first = info.dli_sname;
    }
    for (int i = 0; i < 64; i++) {
        if (fp < hero_stack_lo - HERO_STACK_WINDOW || fp + 16 > hero_stack_hi || (fp & 7) != 0) break;
        uintptr_t next_fp = *(uintptr_t *)fp;
        uintptr_t ret = *(uintptr_t *)(fp + 8);
        if (ret == 0) break;
        if (dladdr((void *)(ret - 1), &info) != 0 && hero_stack_is_heroes(info.dli_sname)) return info.dli_sname;
        if (next_fp <= fp) break;
        fp = next_fp;
    }
    return first;
}

/* A fault that is not ours goes to whoever was there before us — chained with
 * the saved disposition, so a library's write barrier keeps working — and a
 * saved default is restored so the fault re-executes and dies as it would have. */
static void hero_stack_pass_on(int signum, siginfo_t *si, void *ctx) {
    struct sigaction *prev = signum == SIGBUS ? &hero_stack_prev_bus : &hero_stack_prev_segv;
    if ((prev->sa_flags & SA_SIGINFO) != 0 && prev->sa_sigaction != NULL) {
        prev->sa_sigaction(signum, si, ctx);
        return;
    }
    if (prev->sa_handler != SIG_DFL && prev->sa_handler != SIG_IGN && prev->sa_handler != NULL) {
        prev->sa_handler(signum);
        return;
    }
    struct sigaction dfl;
    memset(&dfl, 0, sizeof dfl);
    dfl.sa_handler = SIG_DFL;
    sigaction(signum, &dfl, NULL);
}

static void hero_stack_handler(int signum, siginfo_t *si, void *ctx) {
    uintptr_t addr = (uintptr_t)si->si_addr;
    uintptr_t pc, fp, sp;
    hero_stack_regs(ctx, &pc, &fp, &sp);
    int in_guard = addr >= hero_stack_lo - HERO_STACK_WINDOW && addr < hero_stack_lo;
    int sp_low = sp >= hero_stack_lo - HERO_STACK_WINDOW && sp < hero_stack_lo + 4096;
    /* THE SECOND WITNESS HAS TWO SHAPES, and this file knew one of them until
     * docs/defects/007. A function whose frame is larger than a page does not
     * touch it after moving `sp`: clang's prologue calls `___chkstk_darwin`
     * (and `-fstack-clash-protection` emits the same loop inline), which
     * PROBES every page of the frame-to-be while `sp` still stands where the
     * caller left it. When a probe reaches the guard, `si_addr` is in the
     * guard and `sp` is a whole frame above it — `heroes check` measured
     * 6,472 bytes, a frame of 8,576 in `grammarexpr.primary` — so `sp_low`
     * said no, the fault went to SIG_DFL, and the compiler died at 139 with
     * nothing on stderr on a legal program. The shape's own witness is the
     * address: a probe lands BELOW `sp` by less than one frame, and the wild
     * store panel 104 rejected (8 KiB under the stack, from a shallow frame)
     * is megabytes below `sp`, so it still re-raises as before. */
    int probing = addr < sp && sp - addr < HERO_STACK_WINDOW;
    if (in_guard && (sp_low || probing)) {
        const char *who = hero_stack_blame(pc, fp);
        hero_stack_say("panic: stack exhausted");
        if (who != NULL) {
            hero_stack_say(" in ");
            hero_stack_say_heroes_name(who);
        }
        hero_stack_say("\n");
        abort();
    }
    hero_stack_pass_on(signum, si, ctx);
}

static void hero_stack_guard_install(void) {
    static int done = 0;
    if (done) return;
    done = 1;
    hero_stack_bounds();

    /* 256 KiB rather than SIGSTKSZ, with a guard page of its own below. */
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    size_t size = 256 * 1024;
    char *mem = mmap(NULL, size + page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) return;
    mprotect(mem, page, PROT_NONE);
    stack_t ss;
    ss.ss_sp = mem + page;
    ss.ss_size = size;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) return;

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = hero_stack_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGSEGV, &sa, &hero_stack_prev_segv) != 0) return;
    if (sigaction(SIGBUS, &sa, &hero_stack_prev_bus) != 0) return;
}

#else

/* WINDOWS. A vectored exception handler, registered LAST so a library that
 * uses structured exceptions sees the fault first (panel 104's pragmatist,
 * from Rust's `sys/pal/windows/stack_overflow.rs`). `SetThreadStackGuarantee`
 * reserves stack for the handler to run on; without it there is one page. The
 * message is written with `_write`, never `fprintf`. There is no frame walk
 * here yet — `SymFromAddr` needs dbghelp initialised before the fault and a
 * PDB beside the binary, and neither is measured on the box today — so the
 * line names the failure and not the function; the site's copy says so. */
#include <windows.h>
#include <io.h>

static LONG WINAPI hero_stack_veh(EXCEPTION_POINTERS *ep) {
    if (ep->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
        const char *line = "panic: stack exhausted\n";
        _write(2, line, (unsigned int)strlen(line));
        abort();
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void hero_stack_guard_install(void) {
    static int done = 0;
    if (done) return;
    done = 1;
    ULONG guarantee = 64 * 1024;
    SetThreadStackGuarantee(&guarantee);
    AddVectoredExceptionHandler(0, hero_stack_veh);
}

#endif
