/* fixedbugs-every-name-the-runtime-defines-in-place-of-the-c-librarys-is-answered-by-every-door.h: every function the
 * runtime defines or rewrites in place of the C library's, asked of every door by name a program can open, each answer
 * held to the function a call of the name reaches (defect d-p259-clerk-1).
 *
 * ON LINUX THE NAMES ARE READ FROM THE WORLD at run time, so a definition the runtime adds is judged with no row
 * here: every function the executable exports, bound global (a sanitizer's interceptor is bound weak, and the
 * runtime's definitions displace it), that the C library's image exports too, each read from the two images' dynamic
 * symbol tables; what a call reaches is the executable's definition, and the version `dlvsym` is asked for is the
 * one the C library gives the name by default. The doors: `dlsym` with `RTLD_DEFAULT`, `RTLD_NEXT`, the C library's
 * handle, the program's own and a `dlmopen` handle of the base namespace; the `dlsym` `dlsym` hands out; `dlvsym`
 * with `RTLD_DEFAULT`, `RTLD_NEXT` and the C library's handle; and the `dlvsym` `dlsym` hands out.
 *
 * ON DARWIN the runtime defines none of them: it rewrites the slots the table of `runtime/parts/rebind_image.c`
 * names, and answers `dlsym` and dyld's legacy lookups from the same table, so the names are that table's, written
 * here, and what a call reaches is the address the program's code loads from its own slot (the setjmp family's
 * slots are left alone where their decode verified, and then every door answers the C library's own, as a call
 * reaches). The doors: `dlsym` with `RTLD_DEFAULT`, `RTLD_NEXT`, `RTLD_SELF`, the C library's handle and the
 * program's own; the `dlsym` `dlsym` hands out; `NSLookupAndBindSymbol` with `NSAddressOfSymbol`; and
 * `_dyld_lookup_and_bind`. `dlsym` with `RTLD_MAIN_ONLY` may answer nothing, since the executable exports none of
 * them, and never another function.
 *
 * Prints one line per name and door answered otherwise, then one line saying whether every one was answered as a
 * call reaches; the names must hold `sigaction`, `dlsym` and `makecontext`, so an empty reading is never a pass.
 * `RTLD_NEXT`, `dlvsym` and `dlmopen` are glibc's only under `_GNU_SOURCE`, which glibc reads at its first header,
 * so it is defined here first. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ucontext.h>

typedef void *(*every_sym_fn)(void *, const char *);

static int every_wrong = 0;
static int every_core = 0;

/* One door's answer for `name`, held to `call`; NULL allowed only where `none_ok`. */
static void every_judge(const char *name, const char *door, void *got, void *call, int none_ok) {
    if (got == call || (none_ok && got == NULL)) return;
    every_wrong += 1;
    printf("%s through %s: %s\n", name, door, got == NULL ? "nothing" : "another function");
}

static void every_counted(const char *name) {
    if (strcmp(name, "sigaction") == 0 || strcmp(name, "dlsym") == 0 || strcmp(name, "makecontext") == 0) {
        every_core += 1;
    }
}

#if defined(__APPLE__)
#include <mach-o/dyld.h>

/* The address handed through a call, so the code loads it from its own slot. */
__attribute__((noinline)) static void *every_slot(void *p) {
    __asm__ volatile("" : "+r"(p));
    return p;
}

/* `name` through every door, held to `call`. Darwin's legacy lookups are deprecated; that warning is off here. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static void every_name(const char *name, void *call) {
    void *library = dlopen("/usr/lib/libSystem.B.dylib", RTLD_NOW);
    void *own = dlopen(NULL, RTLD_NOW);
    every_sym_fn ask = (every_sym_fn)dlsym(RTLD_NEXT, "dlsym");
    char named[96];
    snprintf(named, sizeof named, "_%s", name);
    NSSymbol symbol = NSIsSymbolNameDefined(named) ? NSLookupAndBindSymbol(named) : NULL;
    void *bound = NULL;
    NSModule module = NULL;
    _dyld_lookup_and_bind(named, &bound, &module);
    every_judge(name, "dlsym with RTLD_DEFAULT", dlsym(RTLD_DEFAULT, name), call, 0);
    every_judge(name, "dlsym with RTLD_NEXT", dlsym(RTLD_NEXT, name), call, 0);
    every_judge(name, "dlsym with RTLD_SELF", dlsym(RTLD_SELF, name), call, 0);
    every_judge(name, "dlsym with RTLD_MAIN_ONLY", dlsym(RTLD_MAIN_ONLY, name), call, 1);
    every_judge(name, "dlsym with the C library's handle", library != NULL ? dlsym(library, name) : NULL, call, 0);
    every_judge(name, "dlsym with the program's own handle", own != NULL ? dlsym(own, name) : NULL, call, 0);
    every_judge(name, "the dlsym of dlsym", ask != NULL ? ask(RTLD_NEXT, name) : NULL, call, 0);
    every_judge(name, "NSLookupAndBindSymbol", symbol != NULL ? NSAddressOfSymbol(symbol) : NULL, call, 0);
    every_judge(name, "_dyld_lookup_and_bind", bound, call, 0);
    every_counted(name);
}

/* The table's names, each with what a call of it reaches. */
static inline void every_door(void) {
#define EVERY(n) every_name(#n, every_slot((void *)(n)))
    EVERY(sigaction);
    EVERY(signal);
    EVERY(sigset);
    EVERY(bsd_signal);
    EVERY(sigvec);
    EVERY(siginterrupt);
    EVERY(dlsym);
    EVERY(NSAddressOfSymbol);
    EVERY(_dyld_lookup_and_bind);
    EVERY(_dyld_lookup_and_bind_with_hint);
    EVERY(_dyld_lookup_and_bind_fully);
    EVERY(siglongjmp);
    EVERY(longjmp);
    EVERY(_longjmp);
    EVERY(setcontext);
    EVERY(swapcontext);
    EVERY(makecontext);
    EVERY(setjmp);
    EVERY(_setjmp);
    EVERY(sigsetjmp);
    EVERY(getcontext);
#undef EVERY
    printf("%s\n", every_wrong == 0 && every_core == 3 ? "every name answered as a call reaches"
                                                       : "a name answered otherwise, or the names not found");
    fflush(stdout);
}
#pragma clang diagnostic pop

#else
#include <link.h>

typedef void *(*every_vsym_fn)(void *, const char *, const char *);

/* An image's dynamic symbols. */
typedef struct {
    const struct link_map *map;
    const ElfW(Sym) *syms;
    const char *strs;
    const ElfW(Half) *versym;
    const ElfW(Verdef) *verdef;
    uint32_t count;
} every_image;

/* An address a dynamic entry holds: the dynamic linker adds the base in place where the section is writable. */
static uintptr_t every_at(const struct link_map *map, ElfW(Addr) p) { return p < map->l_addr ? map->l_addr + p : p; }

/* The image `handle` names, read: its count of symbols is the GNU hash's, past the last chain of its highest
 * bucket. */
static every_image every_read(void *handle) {
    every_image im = {NULL, NULL, NULL, NULL, NULL, 0};
    struct link_map *map = NULL;
    if (handle == NULL || dlinfo(handle, RTLD_DI_LINKMAP, &map) != 0 || map == NULL) return im;
    im.map = map;
    const uint32_t *gnu = NULL;
    for (const ElfW(Dyn) *d = map->l_ld; d->d_tag != DT_NULL; d += 1) {
        if (d->d_tag == DT_SYMTAB) im.syms = (const ElfW(Sym) *)every_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_STRTAB) im.strs = (const char *)every_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_GNU_HASH) gnu = (const uint32_t *)every_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_VERSYM) im.versym = (const ElfW(Half) *)every_at(map, d->d_un.d_ptr);
        if (d->d_tag == DT_VERDEF) im.verdef = (const ElfW(Verdef) *)every_at(map, d->d_un.d_ptr);
    }
    if (gnu == NULL || gnu[0] == 0) return im;
    const uint32_t *buckets = (const uint32_t *)((const ElfW(Addr) *)(gnu + 4) + gnu[2]);
    const uint32_t *chain = buckets + gnu[0];
    uint32_t last = 0;
    for (uint32_t b = 0; b < gnu[0]; b += 1) {
        if (buckets[b] > last) last = buckets[b];
    }
    if (last < gnu[1]) {
        im.count = gnu[1];
        return im;
    }
    while ((chain[last - gnu[1]] & 1) == 0) last += 1;
    im.count = last + 1;
    return im;
}

/* Whether symbol `i` is a function the image defines and exports by default, and, where `global` asks it, binds
 * global. */
static int every_exported(const every_image *im, uint32_t i, int global) {
    const ElfW(Sym) *sym = &im->syms[i];
    if (sym->st_shndx == SHN_UNDEF || ELF64_ST_TYPE(sym->st_info) != STT_FUNC) return 0;
    if (global && ELF64_ST_BIND(sym->st_info) != STB_GLOBAL) return 0;
    return im->versym == NULL || (im->versym[i] & 0x8000) == 0;
}

/* The C library's default version of `name`, or NULL where it exports no such function. */
static const char *every_version(const every_image *libc, const char *name, int *found) {
    *found = 0;
    for (uint32_t i = 0; i < libc->count; i += 1) {
        if (!every_exported(libc, i, 0) || strcmp(libc->strs + libc->syms[i].st_name, name) != 0) continue;
        *found = 1;
        if (libc->versym == NULL || libc->verdef == NULL) return NULL;
        ElfW(Half) index = libc->versym[i] & 0x7fff;
        for (const ElfW(Verdef) *v = libc->verdef;; v = (const ElfW(Verdef) *)((const char *)v + v->vd_next)) {
            if (v->vd_ndx == index) {
                const ElfW(Verdaux) *aux = (const ElfW(Verdaux) *)((const char *)v + v->vd_aux);
                return libc->strs + aux->vda_name;
            }
            if (v->vd_next == 0) break;
        }
        return NULL;
    }
    return NULL;
}

/* `name` through every door, held to `call`. */
static void every_name(const char *name, void *call, const char *version, void *libc) {
    void *own = dlopen(NULL, RTLD_NOW);
    void *base = dlmopen(LM_ID_BASE, "libc.so.6", RTLD_NOW);
    every_sym_fn ask = (every_sym_fn)dlsym(RTLD_NEXT, "dlsym");
    every_vsym_fn vask = (every_vsym_fn)dlsym(RTLD_NEXT, "dlvsym");
    every_judge(name, "dlsym with RTLD_DEFAULT", dlsym(RTLD_DEFAULT, name), call, 0);
    every_judge(name, "dlsym with RTLD_NEXT", dlsym(RTLD_NEXT, name), call, 0);
    every_judge(name, "dlsym with the C library's handle", dlsym(libc, name), call, 0);
    every_judge(name, "dlsym with the program's own handle", own != NULL ? dlsym(own, name) : NULL, call, 0);
    every_judge(name, "dlsym with a dlmopen handle", base != NULL ? dlsym(base, name) : NULL, call, 0);
    every_judge(name, "the dlsym of dlsym", ask != NULL ? ask(RTLD_NEXT, name) : NULL, call, 0);
    if (version != NULL) {
        every_judge(name, "dlvsym with RTLD_DEFAULT", dlvsym(RTLD_DEFAULT, name, version), call, 0);
        every_judge(name, "dlvsym with RTLD_NEXT", dlvsym(RTLD_NEXT, name, version), call, 0);
        every_judge(name, "dlvsym with the C library's handle", dlvsym(libc, name, version), call, 0);
        every_judge(name, "the dlvsym of dlsym", vask != NULL ? vask(RTLD_NEXT, name, version) : NULL, call, 0);
    }
    every_counted(name);
}

/* Every function the executable exports bound global that the C library exports too. */
static inline void every_door(void) {
    void *libc = dlopen("libc.so.6", RTLD_NOW | RTLD_NOLOAD);
    every_image exe = every_read(dlopen(NULL, RTLD_NOW));
    every_image lib = every_read(libc);
    for (uint32_t i = 0; exe.syms != NULL && lib.syms != NULL && i < exe.count; i += 1) {
        if (!every_exported(&exe, i, 1)) continue;
        const char *name = exe.strs + exe.syms[i].st_name;
        int found = 0;
        const char *version = every_version(&lib, name, &found);
        if (found) every_name(name, (void *)(exe.map->l_addr + exe.syms[i].st_value), version, libc);
    }
    printf("%s\n", every_wrong == 0 && every_core == 3 ? "every name answered as a call reaches"
                                                       : "a name answered otherwise, or the names not found");
    fflush(stdout);
}
#endif
