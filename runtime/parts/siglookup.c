/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/siglookup.c — `dlsym` answered with this runtime's own definition for
 * the twenty C library names it defines, on Linux and every POSIX system but
 * Darwin (defect 2522's row 3, the author's ruling C of 2026-11-20; panel
 * 251's R1; design.md §1.12, §4.19).
 *
 * WHAT IT IS FOR. `parts/siginstall.c` and `parts/sigjump.c` define the C
 * library's installers and its save points and jumps, so every call of them
 * by name reaches this runtime, and the thunk can ask whether a handler of
 * C's is running (`parts/sigguard.c`'s fourth question). A library that asks
 * `dlsym` for one of those names, with the C library's own handle or with
 * `RTLD_NEXT`, was answered the C library's own and went around them: a
 * handler a `sigaction` so found installed called the program's function
 * through 24 frames of its own, and the function ran inside the signal, 20
 * runs of 20 at `-O0` and at `-O2` in Linux arm64 (lane-signals' `r2522dl`,
 * 2026-11-20). So this runtime defines `dlsym` too, and answers its own
 * definition for those names, which is what a call of them by name reaches.
 *
 * WHO IS NOT ANSWERED, and why each:
 *
 * - A LIBRARY THAT DEFINES THE NAME ITSELF, asking for the definition after
 *   its own: an interposer (a preloaded tool, a sanitizer built as a shared
 *   runtime) finds the next `sigaction` that way and calls it from its own.
 *   Answered this runtime's, whose own `sigaction` reaches the interposer's
 *   (`RTLD_NEXT` from the executable), it would call itself until its stack
 *   ended, the loop panel 251 measured on Darwin when the rewrite took an
 *   interposer's slots. Such a lookup is handed to the C library's `dlsym` by
 *   a tail call, so it sees its caller's address, as `RTLD_NEXT` needs.
 * - A NAME NOT AMONG THE TWENTY, handed on the same way.
 * - UNDER AddressSanitizer, ANY LOOKUP BEFORE THE SANITIZER HAS STARTED: its
 *   runtime asks `dlsym(RTLD_NEXT, ...)` for every function it intercepts
 *   before it has mapped its shadow, so this file is not instrumented, and a
 *   `.preinit_array` entry, run before any library's constructor, starts the
 *   sanitizer first and then arms the answer. The sanitizer keeps the C
 *   library's own for what it calls itself, as it did before this file.
 *
 * FINDING THE C LIBRARY'S OWN `dlsym`, which this definition displaces in
 * every image: `dlvsym` by the version glibc gives it since 2.34, where
 * `libdl` merged into `libc.so.6`, then by the architecture's first version.
 * The runtime's own lookups of what it wraps (`hero_dl_next`) go to it
 * directly, never through the definition below.
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): `dlvsym`, whose own
 * definition would leave the C library's `dlsym` unfindable by name; a
 * `sigaction` found by walking an image's symbol table by hand; a raw
 * `rt_sigaction` system call; glibc's own internal installs and jumps. A
 * program library defining `dlsym` is told `ffi_defined_elsewhere` (panel
 * 239's member, which reads the link's own duplicate). */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <link.h>

typedef void *(*hero_dl_sym_fn)(void *, const char *);

static _Atomic(hero_dl_sym_fn) hero_dl_real_sym = NULL;

/* The version glibc gave `dlsym` at its first release on this architecture,
 * tried after 2.34's. */
#if defined(__x86_64__)
#define HERO_DL_FIRST_VERSION "GLIBC_2.2.5"
#elif defined(__aarch64__) || defined(__powerpc64__)
#define HERO_DL_FIRST_VERSION "GLIBC_2.17"
#elif defined(__riscv)
#define HERO_DL_FIRST_VERSION "GLIBC_2.27"
#elif defined(__arm__)
#define HERO_DL_FIRST_VERSION "GLIBC_2.4"
#elif defined(__s390x__)
#define HERO_DL_FIRST_VERSION "GLIBC_2.2"
#else
#define HERO_DL_FIRST_VERSION "GLIBC_2.0"
#endif

/* The C library's own `dlsym`, found once. Not instrumented: the sanitizer's
 * own start reaches it before its shadow exists. */
__attribute__((no_sanitize("address"))) static hero_dl_sym_fn hero_dl_real(void) {
    hero_dl_sym_fn real = atomic_load_explicit(&hero_dl_real_sym, memory_order_acquire);
    if (real != NULL) return real;
    real = (hero_dl_sym_fn)dlvsym(RTLD_NEXT, "dlsym", "GLIBC_2.34");
    if (real == NULL) real = (hero_dl_sym_fn)dlvsym(RTLD_NEXT, "dlsym", HERO_DL_FIRST_VERSION);
    if (real == NULL) {
        hero_signal_say("panic: the C library's own dlsym was not found\n");
        _exit(134);
    }
    atomic_store_explicit(&hero_dl_real_sym, real, memory_order_release);
    return real;
}

/* The definition of `name` after the executable's, the C library's own where
 * no library interposes it: what `parts/siginstall.c` and `parts/sigjump.c`
 * wrap, asked of the C library's `dlsym`, which `dlsym` below would answer
 * with this runtime's own. Its caller is this executable, which is what
 * `RTLD_NEXT` is measured from. */
static void *hero_dl_next(const char *name) { return hero_dl_real()(RTLD_NEXT, name); }

/* Whether two names are the same bytes; compared here, since the sanitizer
 * may answer `strcmp` before it can. */
__attribute__((no_sanitize("address"))) static bool hero_dl_same(const char *a, const char *b) {
    while (*a != '\0' && *a == *b) {
        a += 1;
        b += 1;
    }
    return *a == *b;
}

/* This runtime's definition of `name`, one of the twenty, or NULL. Three of
 * them glibc declares deprecated, so their addresses are taken with that
 * warning off: a warning in the runtime is one in every program's build. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
__attribute__((no_sanitize("address"))) static void *hero_dl_ours(const char *name) {
    static const struct {
        const char *name;
        void *ours;
    } table[] = {
        {"sigaction", (void *)sigaction},
        {"__sigaction", (void *)__sigaction},
        {"signal", (void *)signal},
        {"bsd_signal", (void *)bsd_signal},
        {"ssignal", (void *)ssignal},
        {"__sysv_signal", (void *)__sysv_signal},
        {"sysv_signal", (void *)sysv_signal},
        {"sigset", (void *)sigset},
        {"sigignore", (void *)sigignore},
        {"siginterrupt", (void *)siginterrupt},
        {"setjmp", (void *)(setjmp)},
        {"_setjmp", (void *)(_setjmp)},
        {"__sigsetjmp", (void *)(__sigsetjmp)},
        {"getcontext", (void *)(getcontext)},
        {"longjmp", (void *)(longjmp)},
        {"_longjmp", (void *)(_longjmp)},
        {"siglongjmp", (void *)(siglongjmp)},
        {"__longjmp_chk", (void *)__longjmp_chk},
        {"setcontext", (void *)(setcontext)},
        {"swapcontext", (void *)(swapcontext)},
    };
    for (size_t i = 0; i < sizeof table / sizeof table[0]; i += 1) {
        if (hero_dl_same(name, table[i].name)) return table[i].ours;
    }
    return NULL;
}
#pragma clang diagnostic pop

/* 0 until the answer may be given: at once, except under the sanitizer, where
 * the `.preinit_array` entry below sets it once the sanitizer has started. */
#if defined(HERO_JMP_ASAN)
static _Atomic int hero_dl_armed = 0;
void __asan_init(void);
static void hero_dl_arm(void) {
    __asan_init();
    atomic_store_explicit(&hero_dl_armed, 1, memory_order_release);
}
__attribute__((section(".preinit_array"), used)) static void (*hero_dl_arm_early)(void) = hero_dl_arm;
#define HERO_DL_ARMED() (atomic_load_explicit(&hero_dl_armed, memory_order_acquire) != 0)
#else
#define HERO_DL_ARMED() 1
#endif

/* Whether `caller` lies in a library, not the executable, that defines `name`
 * itself: asked of that library's own handle, its answer's image compared with
 * the caller's. */
static bool hero_dl_caller_defines(const void *caller, const char *name, hero_dl_sym_fn real) {
    Dl_info info;
    struct link_map *map = NULL;
    if (dladdr1(caller, &info, (void **)&map, RTLD_DL_LINKMAP) == 0 || map == NULL) return false;
    if (map->l_name == NULL || map->l_name[0] == '\0') return false;
    void *self = dlopen(map->l_name, RTLD_NOLOAD | RTLD_LAZY);
    if (self == NULL) return false;
    void *own = real(self, name);
    Dl_info where;
    bool defines = own != NULL && dladdr(own, &where) != 0 && where.dli_fbase == info.dli_fbase;
    dlclose(self);
    return defines;
}

/* THE C LIBRARY'S `dlsym`, displaced: one of the twenty, asked by anyone but a
 * library that defines it, is answered this runtime's definition wherever the
 * C library would have answered one at all; everything else is the C
 * library's own answer, reached by a tail call. */
__attribute__((no_sanitize("address"))) void *dlsym(void *restrict handle, const char *restrict name) {
    hero_dl_sym_fn real = hero_dl_real();
    void *ours = HERO_DL_ARMED() ? hero_dl_ours(name) : NULL;
    if (ours == NULL || hero_dl_caller_defines(__builtin_return_address(0), name, real)) {
        __attribute__((musttail)) return real(handle, name);
    }
    return real(handle, name) != NULL ? ours : NULL;
}

#endif
