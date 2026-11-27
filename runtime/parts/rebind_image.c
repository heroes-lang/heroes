/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_image.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): the rewrite of every loaded image's slots, and
 * the constructor that starts the route before `main` (panel 255's R1,
 * defects 2679 and 2702; design.md §1.12, §4.19). Needs the files `rebind.c`
 * names before it. */

#if defined(__APPLE__)

static void *hero_rebind_dlsym(void *handle, const char *name);
static void *hero_rebind_ns_address(NSSymbol symbol);
static void hero_rebind_lookup_and_bind(const char *name, void **address, NSModule *module);
static void hero_rebind_lookup_and_bind_with_hint(const char *name, const char *hint, void **address, NSModule *module);
static void hero_rebind_lookup_and_bind_fully(const char *name, void **address, NSModule *module);

/* The functions a slot bound to a name (Mach-O's spelling, a leading `_`) is
 * given instead, every one on either route: a save point records only for a
 * family whose decode did not verify (rule 3), decided before the first image
 * is rewritten, and on either route tells where the thread runs (defect
 * 2855: a span of a handler's frames left by a jump shrinks there, and
 * until then a save the route did not record was seen by nothing, so a
 * coroutine's escape to such a save inside an old span read as re-entering
 * it). */
static const struct {
    const char *name;
    void *with;
    int family;
} hero_rebind_table[] = {
    {"_sigaction", (void *)hero_rebind_sigaction, 0},
    {"_signal", (void *)hero_rebind_signal, 0},
    {"_sigset", (void *)hero_rebind_sigset, 0},
    {"_bsd_signal", (void *)hero_rebind_bsd_signal, 0},
    {"_sigvec", (void *)hero_rebind_sigvec, 0},
    {"_siginterrupt", (void *)hero_rebind_siginterrupt, 0},
    {"_dlsym", (void *)hero_rebind_dlsym, 0},
    {"_NSAddressOfSymbol", (void *)hero_rebind_ns_address, 0},
    {"__dyld_lookup_and_bind", (void *)hero_rebind_lookup_and_bind, 0},
    {"__dyld_lookup_and_bind_with_hint", (void *)hero_rebind_lookup_and_bind_with_hint, 0},
    {"__dyld_lookup_and_bind_fully", (void *)hero_rebind_lookup_and_bind_fully, 0},
    {"_siglongjmp", (void *)hero_rebind_siglongjmp, 0},
    {"_longjmp", (void *)hero_rebind_longjmp, 0},
    {"__longjmp", (void *)hero_rebind__longjmp, 0},
    {"_setcontext", (void *)hero_rebind_setcontext, 0},
    {"_swapcontext", (void *)hero_rebind_swapcontext, 0},
    {"_makecontext", (void *)hero_rebind_makecontext, 0},
    {"_setjmp", (void *)hero_rebind_setjmp, HERO_REBIND_JMP},
    {"__setjmp", (void *)hero_rebind__setjmp, HERO_REBIND_JMP},
    {"_sigsetjmp", (void *)hero_rebind_sigsetjmp, HERO_REBIND_JMP},
    {"_getcontext", (void *)hero_rebind_getcontext, HERO_REBIND_CTX},
};

static bool hero_rebind_wanted(size_t i) {
    (void)i;
    return true;
}

/* The function a slot bound to `name`, Mach-O's spelling, is given instead,
 * or NULL for a name this file leaves alone. */
static void *hero_rebind_replacement(const char *name) {
    for (size_t i = 0; i < sizeof hero_rebind_table / sizeof hero_rebind_table[0]; i += 1) {
        if (strcmp(name, hero_rebind_table[i].name) == 0) return hero_rebind_wanted(i) ? hero_rebind_table[i].with : NULL;
    }
    return NULL;
}

/* The same, for C's spelling, the one `dlsym` is asked with. */
static void *hero_rebind_answer(const char *name) {
    for (size_t i = 0; i < sizeof hero_rebind_table / sizeof hero_rebind_table[0]; i += 1) {
        if (strcmp(name, hero_rebind_table[i].name + 1) == 0) {
            return hero_rebind_wanted(i) ? hero_rebind_table[i].with : NULL;
        }
    }
    return NULL;
}

/* Whether the image `caller` lies in exports `name` itself, asked of that
 * image alone (`RTLD_FIRST`): an image asking for the definition after its
 * own, or for its own. */
static bool hero_rebind_caller_defines(const void *caller, const char *name, hero_rebind_dlsym_fn real) {
    Dl_info info;
    if (dladdr(caller, &info) == 0 || info.dli_fname == NULL) return false;
    void *self = dlopen(info.dli_fname, RTLD_NOLOAD | RTLD_FIRST);
    if (self == NULL) return false;
    void *own = real(self, name);
    dlclose(self);
    return own != NULL;
}

/* `dlsym` ANSWERED FOR THE NAMES ABOVE (defect 2522's row 3, the author's
 * ruling C of 2026-11-20). An image's `dlsym` of `sigaction`, by the C
 * library's handle, `RTLD_NEXT` or `RTLD_DEFAULT`, answered the C library's
 * own, never the rewritten slot, and a handler it installed called the
 * program's function through 24 frames, which ran inside the signal, 20 runs
 * of 20 at `-O0`, `-O2` and `--sanitize` on this Mac (lane-signals-2,
 * 2026-11-20). So every name the table rewrites is answered this file's
 * function, which is what the image's slot for it reaches, wherever the C
 * library would have answered one at all. Handed to the C library's `dlsym`
 * by a tail call, so it sees its caller as `RTLD_NEXT` and `RTLD_SELF` need:
 * every other name, and a lookup made by an image that exports the name
 * itself. An image that interposes is never rewritten, so its lookups reach
 * the C library's `dlsym` as they did. */
static void *hero_rebind_dlsym(void *handle, const char *name) {
    hero_rebind_dlsym_fn real = atomic_load_explicit(&hero_rebind_real_dlsym, memory_order_relaxed);
    void *ours = name != NULL ? hero_rebind_answer(name) : NULL;
    if (ours == NULL || hero_rebind_caller_defines(__builtin_return_address(0), name, real)) {
        __attribute__((musttail)) return real(handle, name);
    }
    return real(handle, name) != NULL ? ours : NULL;
}

/* dyld's legacy lookups, which the table rewrites too. */
typedef void *(*hero_rebind_ns_address_fn)(NSSymbol);
typedef void (*hero_rebind_bind_fn)(const char *, void **, NSModule *);
typedef void (*hero_rebind_bind_hint_fn)(const char *, const char *, void **, NSModule *);
static _Atomic(hero_rebind_ns_address_fn) hero_rebind_real_ns_address = NULL;
static _Atomic(hero_rebind_bind_fn) hero_rebind_real_bind = NULL;
static _Atomic(hero_rebind_bind_hint_fn) hero_rebind_real_bind_hint = NULL;
static _Atomic(hero_rebind_bind_fn) hero_rebind_real_bind_fully = NULL;

/* What `dlsym` answers for the function at `found`, asked by the name its
 * image gives that address exactly: this file's function for a name the
 * table rewrites, `found` itself for every other. By the name and not by the
 * address `dlsym` gave before `main`: under `--sanitize` that address is the
 * sanitizer's wrapper, and these lookups answer libsystem_c's own (`sigaction`
 * on this Mac, 2026-11-25). */
static void *hero_rebind_answer_named(void *found) {
    Dl_info info;
    if (found == NULL || dladdr(found, &info) == 0 || info.dli_saddr != found || info.dli_sname == NULL) return found;
    void *ours = hero_rebind_answer(info.dli_sname);
    return ours != NULL ? ours : found;
}

/* What `dlsym` answers for the name `named`, Mach-O's spelling, whose C
 * library's own is `found`. */
static void *hero_rebind_answer_spelled(const char *named, void *found) {
    void *ours = found != NULL && named != NULL ? hero_rebind_replacement(named) : NULL;
    return ours != NULL ? ours : found;
}

/* DYLD'S LEGACY LOOKUPS, ANSWERED AS `dlsym` IS (defect d-p259-clerk-1).
 * `NSLookupAndBindSymbol`, `NSLookupSymbolInImage` and their kin hand out an
 * opaque symbol whose address `NSAddressOfSymbol` gives, and
 * `_dyld_lookup_and_bind` and its two kin write the address: each answered the
 * C library's own of every name the table rewrites, never the rewritten slot,
 * on this Mac (macOS 26, 2026-11-25), deprecated since 10.5 and still
 * answered. Each is the C library's own call, its answer then given by name. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
static void *hero_rebind_ns_address(NSSymbol symbol) {
    hero_rebind_ns_address_fn real = atomic_load_explicit(&hero_rebind_real_ns_address, memory_order_relaxed);
    return real != NULL ? hero_rebind_answer_named(real(symbol)) : NULL;
}

static void hero_rebind_lookup_and_bind(const char *name, void **address, NSModule *module) {
    hero_rebind_bind_fn real = atomic_load_explicit(&hero_rebind_real_bind, memory_order_relaxed);
    if (real != NULL) real(name, address, module);
    if (address != NULL) *address = hero_rebind_answer_spelled(name, *address);
}

static void hero_rebind_lookup_and_bind_with_hint(const char *name, const char *hint, void **address,
                                                  NSModule *module) {
    hero_rebind_bind_hint_fn real = atomic_load_explicit(&hero_rebind_real_bind_hint, memory_order_relaxed);
    if (real != NULL) real(name, hint, address, module);
    if (address != NULL) *address = hero_rebind_answer_spelled(name, *address);
}

static void hero_rebind_lookup_and_bind_fully(const char *name, void **address, NSModule *module) {
    hero_rebind_bind_fn real = atomic_load_explicit(&hero_rebind_real_bind_fully, memory_order_relaxed);
    if (real != NULL) real(name, address, module);
    if (address != NULL) *address = hero_rebind_answer_spelled(name, *address);
}
#pragma clang diagnostic pop

/* Whether an image interposes (an `__interpose` section in any segment): its
 * calls of the C library's names are its own wrappers' way to the C library,
 * and are left as they are (the `--sanitize` repair, above). */
static bool hero_rebind_interposes(const struct mach_header_64 *mh) {
    const uint8_t *p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *seg = (const struct segment_command_64 *)lc;
            const struct section_64 *sec = (const struct section_64 *)(seg + 1);
            for (uint32_t j = 0; j < seg->nsects; j += 1) {
                if (strncmp(sec[j].sectname, "__interpose", 16) == 0) return true;
            }
        }
        p += lc->cmdsize;
    }
    return false;
}

/* Every lazy and non-lazy symbol pointer of the image whose symbol this file
 * replaces, rewritten, its page made writable by a copy first. An image of
 * the shared cache is left alone: its binds are not rewritable. */
static void hero_rebind_image(const struct mach_header *header, intptr_t slide) {
    const struct mach_header_64 *mh = (const struct mach_header_64 *)header;
    if ((mh->flags & MH_DYLIB_IN_CACHE) != 0 || hero_rebind_interposes(mh)) return;
    const struct segment_command_64 *linkedit = NULL;
    const struct symtab_command *st = NULL;
    const struct dysymtab_command *dy = NULL;
    const uint8_t *p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        if (lc->cmd == LC_SEGMENT_64 && strcmp(((const struct segment_command_64 *)lc)->segname, SEG_LINKEDIT) == 0) {
            linkedit = (const struct segment_command_64 *)lc;
        } else if (lc->cmd == LC_SYMTAB) {
            st = (const struct symtab_command *)lc;
        } else if (lc->cmd == LC_DYSYMTAB) {
            dy = (const struct dysymtab_command *)lc;
        }
        p += lc->cmdsize;
    }
    if (linkedit == NULL || st == NULL || dy == NULL || dy->nindirectsyms == 0) return;
    uintptr_t base = (uintptr_t)slide + (uintptr_t)linkedit->vmaddr - (uintptr_t)linkedit->fileoff;
    const struct nlist_64 *syms = (const struct nlist_64 *)(base + st->symoff);
    const char *strs = (const char *)(base + st->stroff);
    const uint32_t *indirect = (const uint32_t *)(base + dy->indirectsymoff);
    p = (const uint8_t *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i += 1) {
        const struct load_command *lc = (const struct load_command *)p;
        p += lc->cmdsize;
        if (lc->cmd != LC_SEGMENT_64) continue;
        const struct segment_command_64 *seg = (const struct segment_command_64 *)lc;
        const struct section_64 *sec = (const struct section_64 *)(seg + 1);
        for (uint32_t j = 0; j < seg->nsects; j += 1, sec += 1) {
            uint32_t type = sec->flags & SECTION_TYPE;
            if (type != S_LAZY_SYMBOL_POINTERS && type != S_NON_LAZY_SYMBOL_POINTERS) continue;
            void **slots = (void **)((uintptr_t)slide + (uintptr_t)sec->addr);
            uint32_t n = (uint32_t)(sec->size / sizeof(void *));
            for (uint32_t k = 0; k < n; k += 1) {
                uint32_t si = indirect[sec->reserved1 + k];
                if ((si & (INDIRECT_SYMBOL_ABS | INDIRECT_SYMBOL_LOCAL)) != 0) continue;
                void *with = hero_rebind_replacement(strs + syms[si].n_un.n_strx);
                if (with == NULL) continue;
                (void)vm_protect(mach_task_self(), (vm_address_t)&slots[k], sizeof(void *), 0,
                                 VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
                slots[k] = with;
            }
        }
    }
}

/* -- before `main` ------------------------------------------------------- */

/* Priority 0, before every constructor of the program's own: the key made,
 * the C library's own taken (by `RTLD_DEFAULT`, so under an interposer its
 * wrapper, which reaches the C library; under AddressSanitizer the three jumps
 * the C library's own behind ASan's wrappers, `hero_rebind_uninterposed`), the
 * images loaded now measured for
 * route A, the decode calibrated (which decides which save points the table
 * rewrites), every image rewritten now and every one dyld loads later, and
 * every handler already installed trampolined. */
#if defined(HERO_REBIND_ASAN)
/* The C library's own function behind an interposer's wrapper `f`, or `f`
 * where no loaded image interposes it. dyld binds every other image's slot for
 * a name an interposing image replaces to the replacement, and `dlsym`
 * answers the same, whatever handle it is asked with; the interposer's own
 * `__interpose` tuples, {replacement, replaced}, are bound to the original
 * (measured on this Mac, 2026-11-27: ASan's `wrap__longjmp`, `wrap_longjmp`
 * and `wrap_siglongjmp`, in its `__DATA`, each beside libsystem_platform's
 * own). A jump through ASan's wrapper asks `__asan_handle_no_return` on every
 * stack, a coroutine's included, where ASan clears nothing and writes its
 * warning into the program's stderr (defect d-lane-sig-family-8); this
 * runtime clears what the jump leaves itself (`hero_rebind_leaving`). */
static void *hero_rebind_uninterposed(void *f) {
    const char *const segments[2] = {"__DATA", "__DATA_CONST"};
    for (uint32_t i = 0; f != NULL && i < _dyld_image_count(); i += 1) {
        const struct mach_header_64 *h = (const struct mach_header_64 *)_dyld_get_image_header(i);
        for (int s = 0; s < 2; s += 1) {
            unsigned long size = 0;
            void *const *t = (void *const *)getsectiondata(h, segments[s], "__interpose", &size);
            for (unsigned long k = 0; t != NULL && k + 1 < size / sizeof(void *); k += 2) {
                if (t[k] == f && t[k + 1] != NULL) return t[k + 1];
            }
        }
    }
    return f;
}
#endif

__attribute__((constructor(0))) static void hero_rebind_start(void) {
    pthread_key_t key;
    if (pthread_key_create(&key, NULL) == 0) {
        atomic_store_explicit(&hero_rebind_pending, key, memory_order_relaxed);
        atomic_store_explicit(&hero_rebind_pending_keyed, 1, memory_order_release);
    }
    atomic_store(&hero_rebind_real_sigaction, (hero_rebind_action_fn)dlsym(RTLD_DEFAULT, "sigaction"));
    atomic_store(&hero_rebind_real_signal, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "signal"));
    atomic_store(&hero_rebind_real_sigset, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "sigset"));
    atomic_store(&hero_rebind_real_bsd_signal, (hero_rebind_signal_fn)dlsym(RTLD_DEFAULT, "bsd_signal"));
    atomic_store(&hero_rebind_real_sigvec, (hero_rebind_vec_fn)dlsym(RTLD_DEFAULT, "sigvec"));
    atomic_store(&hero_rebind_real_siginterrupt, (hero_rebind_interrupt_fn)dlsym(RTLD_DEFAULT, "siginterrupt"));
    atomic_store(&hero_rebind_real_dlsym, (hero_rebind_dlsym_fn)dlsym(RTLD_DEFAULT, "dlsym"));
    atomic_store(&hero_rebind_real_siglongjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "siglongjmp"));
    atomic_store(&hero_rebind_real_longjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "longjmp"));
    atomic_store(&hero_rebind_real__longjmp, (hero_rebind_jump_fn)dlsym(RTLD_DEFAULT, "_longjmp"));
#if defined(HERO_REBIND_ASAN)
    atomic_store(&hero_rebind_real_siglongjmp,
                 (hero_rebind_jump_fn)hero_rebind_uninterposed((void *)atomic_load(&hero_rebind_real_siglongjmp)));
    atomic_store(&hero_rebind_real_longjmp,
                 (hero_rebind_jump_fn)hero_rebind_uninterposed((void *)atomic_load(&hero_rebind_real_longjmp)));
    atomic_store(&hero_rebind_real__longjmp,
                 (hero_rebind_jump_fn)hero_rebind_uninterposed((void *)atomic_load(&hero_rebind_real__longjmp)));
#endif
    atomic_store(&hero_rebind_real_setjmp, (hero_rebind_save_fn)dlsym(RTLD_DEFAULT, "setjmp"));
    atomic_store(&hero_rebind_real__setjmp, (hero_rebind_save_fn)dlsym(RTLD_DEFAULT, "_setjmp"));
    atomic_store(&hero_rebind_real_sigsetjmp, (hero_rebind_sigsave_fn)dlsym(RTLD_DEFAULT, "sigsetjmp"));
    atomic_store(&hero_rebind_real_getcontext, (hero_rebind_get_fn)dlsym(RTLD_DEFAULT, "getcontext"));
    atomic_store(&hero_rebind_real_setcontext, (hero_rebind_set_fn)dlsym(RTLD_DEFAULT, "setcontext"));
    atomic_store(&hero_rebind_real_swapcontext, (hero_rebind_swap_fn)dlsym(RTLD_DEFAULT, "swapcontext"));
    atomic_store(&hero_rebind_real_makecontext, (hero_rebind_make_fn)dlsym(RTLD_DEFAULT, "makecontext"));
    atomic_store(&hero_rebind_real_ns_address, (hero_rebind_ns_address_fn)dlsym(RTLD_DEFAULT, "NSAddressOfSymbol"));
    atomic_store(&hero_rebind_real_bind, (hero_rebind_bind_fn)dlsym(RTLD_DEFAULT, "_dyld_lookup_and_bind"));
    atomic_store(&hero_rebind_real_bind_hint,
                 (hero_rebind_bind_hint_fn)dlsym(RTLD_DEFAULT, "_dyld_lookup_and_bind_with_hint"));
    atomic_store(&hero_rebind_real_bind_fully, (hero_rebind_bind_fn)dlsym(RTLD_DEFAULT, "_dyld_lookup_and_bind_fully"));
    for (uint32_t i = 0; i < _dyld_image_count(); i += 1) {
        hero_rebind_regions_of(_dyld_get_image_header(i), _dyld_get_image_vmaddr_slide(i));
    }
    hero_rebind_calibrate();
    _dyld_register_func_for_add_image(hero_rebind_image);
    for (int sig = 1; sig < NSIG; sig += 1) {
        if (sig != SIGKILL && sig != SIGSTOP) hero_rebind_retramp(sig);
    }
}

#endif
