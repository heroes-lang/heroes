/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/siglookup.c — `dlsym` and `dlvsym` answered with this runtime's own
 * definition for every C library name it defines, on Linux and every POSIX
 * system but Darwin (defect 2522's row 3, the author's ruling C of
 * 2026-11-20; panel 251's R1; defect d-p259-clerk-1; design.md §1.12, §4.19).
 *
 * WHAT IT IS FOR. `parts/siginstall.c`, `parts/sigjump_save.c` and
 * `parts/sigjump_make.c` define the C library's installers, its save points and
 * jumps, and `makecontext`, so every call of them by name reaches this runtime,
 * and the thunk can ask whether a handler of C's is running
 * (`parts/sigguard.c`'s fourth question) and which ranges `makecontext` was
 * given, which the jump records read (`parts/sigentries.c`). A library that
 * asks `dlsym` for one of those names, with the C library's own handle or with
 * `RTLD_NEXT`, was answered the C library's own and went around them: a handler
 * a `sigaction` so found installed called the program's function through 24
 * frames of its own, and the function ran inside the signal, 20 runs of 20 at
 * `-O0` and at `-O2` in Linux arm64 (lane-signals' `r2522dl`, 2026-11-20); and
 * a `makecontext` so found named no stack, so two coroutines on stacks carved
 * from the thread's own were read as the thread's and a correct program was
 * refused in the words of a signal handler it does not have, 3 runs of 3 in
 * Linux arm64 (`unseen_carved`, panel 259's clerk, 2026-11-25). So this runtime
 * defines `dlsym` and `dlvsym` too, and answers its own definition for those
 * names, which is what a call of them by name reaches.
 *
 * THE NAMES (`hero_dl_ours`) are every function this runtime defines that the
 * C library also exports, enumerated from the objects (`nm` of the runtime's
 * object against `nm -D` of the image's `libc.so.6`, glibc 2.41 in Linux
 * arm64, 2026-11-25; x86-64's `libc.so.6` read the same, its object unrun):
 * the ten installers, the eleven save points, jumps and
 * context functions, and `dlsym` and `dlvsym` themselves, so that neither
 * hands out the C library's own. Every other name the C library exports at
 * one of their addresses is among them (`signal`, `bsd_signal` and `ssignal`;
 * `longjmp`, `_longjmp` and `siglongjmp`; read from the same `nm -D`). The
 * case `run/fixedbugs-every-name-the-runtime-defines-in-place-of-the-c-librarys-is-answered-by-every-door`
 * reads the names from the executable's own dynamic symbols at run time, so a
 * definition added without its row here fails it, naming the name and the door.
 *
 * THE DOORS answered: `dlsym` and `dlvsym` with `RTLD_DEFAULT`, `RTLD_NEXT`,
 * the C library's handle, `dlopen(NULL)`'s and a `dlmopen` handle, either
 * found through the other, called from the executable or from a library: the
 * executable's dynamic symbols hold all 23, so a library's call of `dlsym` or
 * `dlvsym` reaches the ones below (a library's `RTLD_NEXT`, `RTLD_DEFAULT` and
 * `dlvsym` lookups of `makecontext` answered the runtime's in Linux arm64).
 *
 * WHO IS NOT ANSWERED, and why each:
 *
 * - A LIBRARY THAT DEFINES THE NAME ITSELF, asking for the definition after
 *   its own: an interposer (a preloaded tool, a sanitizer built as a shared
 *   runtime) finds the next `sigaction` that way and calls it from its own.
 *   Answered this runtime's, whose own `sigaction` reaches the interposer's
 *   (`RTLD_NEXT` from the executable), it would call itself until its stack
 *   ended, the loop panel 251 measured on Darwin when the rewrite took an
 *   interposer's slots. Such a lookup is handed to the C library's by a tail
 *   call, so it sees its caller's address, as `RTLD_NEXT` needs.
 * - A NAME NOT AMONG THEM, handed on the same way.
 * - A VERSION NAMING ANOTHER FUNCTION than the one its image gives the name
 *   by default, which `dlvsym` hands on: an older interface, whose arguments
 *   this runtime's definition would misread. glibc 2.41 exports none for
 *   these names on aarch64 or x86-64 (every version of each lies at one
 *   address, the same `nm -D`).
 * - UNDER AddressSanitizer, ANY LOOKUP BEFORE THE SANITIZER HAS STARTED: its
 *   runtime asks `dlsym(RTLD_NEXT, ...)` for every function it intercepts
 *   before it has mapped its shadow, so this file is not instrumented, the C
 *   library's own `dlvsym` is found with no call, and a `.preinit_array`
 *   entry, run before any library's constructor, starts the sanitizer first
 *   and then arms the answer. The sanitizer keeps the C library's own for
 *   what it calls itself.
 *
 * FINDING THE C LIBRARY'S OWN, which these definitions displace in every
 * image. `dlvsym` is read from the dynamic symbol table of the first image
 * after the executable that defines it, in the dynamic linker's list (the
 * executable's `DT_DEBUG`), which is where `RTLD_NEXT` from the executable
 * finds it; no function is called for it, since one called before the
 * sanitizer has started may be an interceptor that cannot answer yet. `dlsym`
 * is asked of it by the version glibc gives it since 2.34, where `libdl`
 * merged into `libc.so.6`, then by the architecture's first. The runtime's own
 * lookups of what it wraps (`hero_dl_next`) go to the C library's `dlsym`
 * directly.
 *
 * WHAT IT DOES NOT REACH, said (defect 2522's rows): a name found by walking an
 * image's symbol table by hand; `__libc_dlsym` and `__libc_dlvsym`, which
 * glibc no longer exports (2.41's `nm -D` lists neither); glibc's internal
 * `__libc_sigaction`, which it exports for itself (`GLIBC_PRIVATE`), another
 * function than `sigaction`, which every door answers as the C library's own
 * (Linux arm64, 2026-11-25), so a handler it installs is past every installer
 * here as one a raw `rt_sigaction` system call installs is; glibc's own
 * internal installs and jumps. A program library defining
 * `dlsym` is told `ffi_defined_elsewhere` (panel 239's member, which reads the
 * link's own duplicate). */

#if !defined(_WIN32) && !defined(__APPLE__)

#include <link.h>

typedef void *(*hero_dl_sym_fn)(void *, const char *);
typedef void *(*hero_dl_vsym_fn)(void *, const char *, const char *);

static _Atomic(hero_dl_sym_fn) hero_dl_real_sym = NULL;
static _Atomic(hero_dl_vsym_fn) hero_dl_real_vsym = NULL;

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

/* Whether two names are the same bytes; compared here, since the sanitizer
 * may answer `strcmp` before it can. */
__attribute__((no_sanitize("address"))) static bool hero_dl_same(const char *a, const char *b) {
    while (*a != '\0' && *a == *b) {
        a += 1;
        b += 1;
    }
    return *a == *b;
}

/* -- the C library's own, found by reading, never by asking --------------- */

/* The executable's own dynamic section, which the link editor defines. */
extern ElfW(Dyn) _DYNAMIC[];

/* An address an image's dynamic entry holds, as the dynamic linker left it:
 * glibc adds the image's base in place where the section is writable, and
 * leaves the offset where it is not (the vDSO, RISC-V); an offset lies below
 * the base of an image loaded anywhere but address 0. */
__attribute__((no_sanitize("address"))) static uintptr_t hero_dl_at(const struct link_map *map, ElfW(Addr) ptr) {
    return ptr < map->l_addr ? map->l_addr + ptr : ptr;
}

/* The default definition of the function `name` in the image `map`, read from
 * its dynamic symbol table by its GNU hash: its address, or NULL where the
 * image defines none. */
__attribute__((no_sanitize("address"))) static void *hero_dl_defined(const struct link_map *map, const char *name) {
    const ElfW(Sym) *syms = NULL;
    const char *strs = NULL;
    const uint32_t *gnu = NULL;
    const ElfW(Half) *versym = NULL;
    for (const ElfW(Dyn) *d = map->l_ld; d != NULL && d->d_tag != DT_NULL; d += 1) {
        if (d->d_tag == DT_SYMTAB) syms = (const ElfW(Sym) *)hero_dl_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_STRTAB) strs = (const char *)hero_dl_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_GNU_HASH) gnu = (const uint32_t *)hero_dl_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_VERSYM) versym = (const ElfW(Half) *)hero_dl_at(map, d->d_un.d_ptr);
    }
    if (syms == NULL || strs == NULL || gnu == NULL || gnu[0] == 0) return NULL;
    uint32_t hash = 5381;
    for (const unsigned char *c = (const unsigned char *)name; *c != '\0'; c += 1) hash = hash * 33 + *c;
    const uint32_t *buckets = (const uint32_t *)((const ElfW(Addr) *)(gnu + 4) + gnu[2]);
    const uint32_t *chain = buckets + gnu[0];
    for (uint32_t i = buckets[hash % gnu[0]]; i != 0 && i >= gnu[1]; i += 1) {
        uint32_t link = chain[i - gnu[1]];
        const ElfW(Sym) *sym = &syms[i];
        if ((link | 1) == (hash | 1) && sym->st_shndx != SHN_UNDEF && ELF64_ST_TYPE(sym->st_info) == STT_FUNC &&
            (versym == NULL || (versym[i] & 0x8000) == 0) && hero_dl_same(strs + sym->st_name, name)) {
            return (void *)(map->l_addr + sym->st_value);
        }
        if ((link & 1) != 0) break;
    }
    return NULL;
}

/* The C library's own `dlvsym`: the first image after the executable, in the
 * dynamic linker's list, that defines it. */
__attribute__((no_sanitize("address"))) static hero_dl_vsym_fn hero_dl_real_v(void) {
    hero_dl_vsym_fn real = atomic_load_explicit(&hero_dl_real_vsym, memory_order_acquire);
    if (real != NULL) return real;
    const struct r_debug *debug = NULL;
    for (const ElfW(Dyn) *d = _DYNAMIC; d->d_tag != DT_NULL; d += 1) {
        if (d->d_tag == DT_DEBUG) debug = (const struct r_debug *)d->d_un.d_ptr;
    }
    const struct link_map *map = debug != NULL && debug->r_map != NULL ? debug->r_map->l_next : NULL;
    for (; map != NULL && real == NULL; map = map->l_next) real = (hero_dl_vsym_fn)hero_dl_defined(map, "dlvsym");
    if (real == NULL) {
        hero_signal_say("panic: the C library's own dlvsym was not found\n");
        _exit(134);
    }
    atomic_store_explicit(&hero_dl_real_vsym, real, memory_order_release);
    return real;
}

/* The C library's own `dlsym`, found once. */
__attribute__((no_sanitize("address"))) static hero_dl_sym_fn hero_dl_real(void) {
    hero_dl_sym_fn real = atomic_load_explicit(&hero_dl_real_sym, memory_order_acquire);
    if (real != NULL) return real;
    real = (hero_dl_sym_fn)hero_dl_real_v()(RTLD_NEXT, "dlsym", "GLIBC_2.34");
    if (real == NULL) real = (hero_dl_sym_fn)hero_dl_real_v()(RTLD_NEXT, "dlsym", HERO_DL_FIRST_VERSION);
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

/* -- the answer ------------------------------------------------------------- */

/* This runtime's definition of `name`, or NULL: every function it defines that
 * the C library exports (above). `makecontext` is defined on glibc's two
 * architectures whose wrapper `parts/sigjump_make.c` writes. Three of them
 * glibc declares deprecated, so their addresses are taken with that warning
 * off: a warning in the runtime is one in every program's build. */
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
#if defined(__aarch64__) || defined(__x86_64__)
        {"makecontext", (void *)(makecontext)},
#endif
        {"dlsym", (void *)dlsym},
        {"dlvsym", (void *)dlvsym},
    };
    if (name == NULL) return NULL;
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

/* THE C LIBRARY'S `dlsym`, displaced: one of the names, asked by anyone but a
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

/* Whether `found` is the definition its own image gives `name` by default,
 * read from that image's dynamic symbol table: a version `dlvsym` was asked
 * for that names the same function the name names. */
static bool hero_dl_by_default(void *found, const char *name) {
    Dl_info info;
    struct link_map *map = NULL;
    if (dladdr1(found, &info, (void **)&map, RTLD_DL_LINKMAP) == 0 || map == NULL) return false;
    return hero_dl_defined(map, name) == found;
}

/* THE C LIBRARY'S `dlvsym`, displaced the same way: one of the names, at a
 * version naming the function its image gives the name by default, is
 * answered this runtime's definition; a version naming another function, and
 * everything else, is the C library's own answer, reached by a tail call. */
__attribute__((no_sanitize("address"))) void *dlvsym(void *restrict handle, const char *restrict name,
                                                     const char *restrict version) {
    hero_dl_vsym_fn real = hero_dl_real_v();
    void *ours = HERO_DL_ARMED() ? hero_dl_ours(name) : NULL;
    if (ours == NULL || hero_dl_caller_defines(__builtin_return_address(0), name, hero_dl_real())) {
        __attribute__((musttail)) return real(handle, name, version);
    }
    void *found = real(handle, name, version);
    if (found == NULL) return NULL;
    if (found != ours && !hero_dl_by_default(found, name)) {
        __attribute__((musttail)) return real(handle, name, version);
    }
    return ours;
}

#endif
