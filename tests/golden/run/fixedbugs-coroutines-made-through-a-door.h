/* fixedbugs-coroutines-made-through-a-door.h: the coroutines of `fixedbugs-coroutines-switched-by-hand.h`, whose
 * `makecontext` and `swapcontext` are the ones a door by name hands out (`door_open`), so that no call of either by
 * its name is made: the shape of defect d-p259-clerk-1 (panel 259's clerk, `unseen_carved`). The doors: 1 `dlsym`
 * with `RTLD_DEFAULT`, 2 with `RTLD_NEXT`, 3 with the C library's own handle, 4 with the program's own (`dlopen` of
 * no file), 5 the `dlsym` that `dlsym` hands out, asked with `RTLD_NEXT`, 6 the system's other lookup by name:
 * glibc's `dlvsym` at the version glibc gives the names on this machine, Darwin's `NSLookupAndBindSymbol` and
 * `NSAddressOfSymbol`. Each is asked before the hand header is read, its two names defined to the pointers here.
 * `dlfcn.h`'s `RTLD_NEXT` and glibc's `dlvsym` are glibc's only under `_GNU_SOURCE`, which glibc reads at its first
 * header, so it is defined here first, as the hand header defines it. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <ucontext.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#define DOOR_C_LIBRARY "/usr/lib/libSystem.B.dylib"
#else
#define DOOR_C_LIBRARY "libc.so.6"
#if defined(__aarch64__)
#define DOOR_VERSION "GLIBC_2.17"
#else
#define DOOR_VERSION "GLIBC_2.2.5"
#endif
#endif

typedef void *(*door_sym_fn)(void *, const char *);
typedef void (*door_make_fn)(ucontext_t *, void (*)(void), int, ...);
typedef int (*door_swap_fn)(ucontext_t *, const ucontext_t *);
static door_make_fn door_make = NULL;
static door_swap_fn door_swap = NULL;

/* `name` asked of door `how`, or NULL. The legacy lookups Darwin keeps are marked deprecated; that warning is held
 * off here alone. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static void *door_ask(int32_t how, const char *name) {
    if (how == 1) return dlsym(RTLD_DEFAULT, name);
    if (how == 2) return dlsym(RTLD_NEXT, name);
    if (how == 3 || how == 4) {
        void *handle = dlopen(how == 3 ? DOOR_C_LIBRARY : NULL, RTLD_NOW);
        return handle != NULL ? dlsym(handle, name) : NULL;
    }
    if (how == 5) {
        door_sym_fn ask = (door_sym_fn)dlsym(RTLD_NEXT, "dlsym");
        return ask != NULL ? ask(RTLD_NEXT, name) : NULL;
    }
    if (how == 6) {
#if defined(__APPLE__)
        char named[64];
        snprintf(named, sizeof named, "_%s", name);
        NSSymbol symbol = NSIsSymbolNameDefined(named) ? NSLookupAndBindSymbol(named) : NULL;
        return symbol != NULL ? NSAddressOfSymbol(symbol) : NULL;
#else
        return dlvsym(RTLD_NEXT, name, DOOR_VERSION);
#endif
    }
    return NULL;
}
#pragma clang diagnostic pop

/* The coroutines' `makecontext` and `swapcontext` taken from door `how`: 0, or -1 where it found either not. */
static inline int32_t door_open(int32_t how) {
    door_make = (door_make_fn)door_ask(how, "makecontext");
    door_swap = (door_swap_fn)door_ask(how, "swapcontext");
    return door_make != NULL && door_swap != NULL ? 0 : -1;
}

#define makecontext(...) door_make(__VA_ARGS__)
#define swapcontext(...) door_swap(__VA_ARGS__)
#include "fixedbugs-coroutines-switched-by-hand.h"
#undef makecontext
#undef swapcontext
