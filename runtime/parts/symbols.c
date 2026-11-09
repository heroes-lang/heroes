/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/symbols.c — the program's own functions by name on Linux, the
 * `static` ones too (defect 2371, 2026-11-09).
 *
 * The stack guard and its neighbours name the Heroes function a fault or a
 * room ran out in (`parts/stack.c`, `parts/frame.c`, `parts/os.c`), and they
 * ask `dladdr` for a name. Darwin's `dladdr` reads the executable's whole
 * symbol table. glibc's reads `.dynsym` alone, the symbols exported for
 * dynamic linking, which `-rdynamic` fills with every GLOBAL function and
 * never with a `static` one: a `static` symbol is not exported by
 * construction. And the compiler writes functions `static`: a generic
 * function's instance and the per-type functions are copied into every unit
 * that uses them under `HERO_TU_LOCAL` (`selfhost/emit/unit.hero`, panel 093's
 * R2), so on Linux the guard could not see them. Measured in the Linux arm64
 * container: `make_one<Held>`, whose frame held a 128 MiB result, was told
 * as `main`, the global function under it, at `-O0` and `-O2`; a generic
 * recursion was told with no name at all; a room a generic instance ran out
 * of was told unnamed; and a recursion through an instance lost its cycle.
 * Darwin named every one, from the same program.
 *
 * WHAT THIS IS. The executable's own `.symtab`, the table the static linker
 * writes for every function, global and local, read from the file the first
 * time a handler asks and only then: no program pays anything until it is
 * already dying. `hero_own_symbol` answers the name of the function whose
 * bytes hold an address, or NULL, and `parts/stack.c`'s `hero_stack_symbol`
 * asks it before `dladdr`, which still names what lies outside the
 * executable, `libc`'s `__vfprintf` say.
 *
 * WHAT A HANDLER MAY DO HERE. `open`, `fstat` and `close` are on POSIX's
 * async-signal-safe list; `mmap` is a system call with no lock in user
 * space, as `parts/stack.c` already makes it for its alternate stack; and
 * `getauxval` reads the vector the kernel left, with no lock either. Nothing
 * allocates and nothing writes but the table below.
 *
 * WHAT IT TRUSTS, AND WHERE IT DOES NOT ANSWER. The file is
 * `/proc/self/exe`, the image the kernel started even if its path has been
 * replaced since. Every offset and count read from it is checked against the
 * file's size and every table's alignment before it is read, so a file that
 * is not what it claims gives no answer rather than a wild read. The image's
 * load bias is where the kernel put its program headers, `AT_PHDR`, less the
 * address the file gives them, and an address is answered only inside an
 * executable segment of that image, so an address in another object is
 * never matched against this one's table. A binary stripped of `.symtab`, a
 * system with no `/proc`, or a second thread that asks while the first is
 * still reading: no answer, and `dladdr` speaks as it did before this file.
 */

#if defined(__linux__)

#include <fcntl.h>
#include <link.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* The executable's table as its file holds it, and where the file's
 * addresses landed in this process. */
typedef struct HeroOwnSymbols {
    const ElfW(Sym) *syms;
    size_t count;
    const char *names;
    size_t names_size;
    const ElfW(Phdr) *segments;
    size_t segment_count;
    uintptr_t bias;
} HeroOwnSymbols;

/* Written once, by the handler that moves the state from UNREAD to READING,
 * and read only once a load sees READ, which the store that published it
 * orders after every field. */
static HeroOwnSymbols hero_own_symbols;

enum { HERO_OWN_UNREAD, HERO_OWN_READING, HERO_OWN_READ, HERO_OWN_NONE };
static _Atomic int hero_own_symbols_state = HERO_OWN_UNREAD;

/* Whether `count` records of `size` bytes from `offset` lie inside a file of
 * `file_size` bytes, aligned for a record whose alignment is `align`. */
static int hero_own_fits(size_t file_size, uint64_t offset, uint64_t count, size_t size, size_t align) {
    if (offset > file_size || offset % align != 0) return 0;
    if (count > (file_size - (size_t)offset) / size) return 0;
    return 1;
}

/* The file read into `t`, or 0 where any part of it is not what an ELF
 * executable of this process's own class holds. */
static int hero_own_read(HeroOwnSymbols *t) {
    int fd = open("/proc/self/exe", O_RDONLY | O_CLOEXEC);
    if (fd < 0) return 0;
    struct stat st;
    if (fstat(fd, &st) != 0 || st.st_size < (off_t)sizeof(ElfW(Ehdr))) {
        close(fd);
        return 0;
    }
    size_t size = (size_t)st.st_size;
    void *mapped = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (mapped == MAP_FAILED) return 0;
    const unsigned char *file = (const unsigned char *)mapped;
    const ElfW(Ehdr) *eh = (const ElfW(Ehdr) *)mapped;
    int ok = memcmp(eh->e_ident, ELFMAG, SELFMAG) == 0 && eh->e_ident[EI_CLASS] == (sizeof(void *) == 8 ? ELFCLASS64 : ELFCLASS32) &&
             eh->e_shentsize == sizeof(ElfW(Shdr)) && eh->e_phentsize == sizeof(ElfW(Phdr)) &&
             hero_own_fits(size, eh->e_shoff, eh->e_shnum, sizeof(ElfW(Shdr)), _Alignof(ElfW(Shdr))) &&
             hero_own_fits(size, eh->e_phoff, eh->e_phnum, sizeof(ElfW(Phdr)), _Alignof(ElfW(Phdr)));
    if (!ok) {
        munmap(mapped, size);
        return 0;
    }
    const ElfW(Shdr) *sections = (const ElfW(Shdr) *)(file + eh->e_shoff);
    const ElfW(Phdr) *segments = (const ElfW(Phdr) *)(file + eh->e_phoff);

    /* The bias: the kernel's address of the program headers less the
     * address the file gives them, read off the loaded segment that holds
     * their bytes, which every executable has whether or not it carries a
     * `PT_PHDR` of its own. */
    uintptr_t headers_at = (uintptr_t)getauxval(AT_PHDR);
    int biased = 0;
    for (size_t i = 0; i < eh->e_phnum && headers_at != 0; i++) {
        const ElfW(Phdr) *p = &segments[i];
        if (p->p_type == PT_LOAD && p->p_offset <= eh->e_phoff && eh->e_phoff - p->p_offset < p->p_filesz) {
            t->bias = headers_at - (uintptr_t)(p->p_vaddr + (eh->e_phoff - p->p_offset));
            biased = 1;
            break;
        }
    }

    /* The one `SHT_SYMTAB`, and the string table its `sh_link` names. */
    for (size_t i = 0; i < eh->e_shnum && biased; i++) {
        const ElfW(Shdr) *s = &sections[i];
        if (s->sh_type != SHT_SYMTAB || s->sh_entsize != sizeof(ElfW(Sym)) || s->sh_link >= eh->e_shnum) continue;
        const ElfW(Shdr) *strings = &sections[s->sh_link];
        if (strings->sh_type != SHT_STRTAB || strings->sh_size == 0) continue;
        if (!hero_own_fits(size, s->sh_offset, s->sh_size / sizeof(ElfW(Sym)), sizeof(ElfW(Sym)), _Alignof(ElfW(Sym)))) continue;
        if (!hero_own_fits(size, strings->sh_offset, strings->sh_size, 1, 1)) continue;
        const char *names = (const char *)(file + strings->sh_offset);
        if (names[strings->sh_size - 1] != '\0') continue;
        t->syms = (const ElfW(Sym) *)(file + s->sh_offset);
        t->count = (size_t)(s->sh_size / sizeof(ElfW(Sym)));
        t->names = names;
        t->names_size = (size_t)strings->sh_size;
        t->segments = segments;
        t->segment_count = eh->e_phnum;
        return 1;
    }
    munmap(mapped, size);
    return 0;
}

/* The name of the function of this executable whose bytes hold `addr`, or
 * NULL: from a function's first byte to its last, `st_size` saying where it
 * ends, so an address past a function's end is never told as that function. */
static const char *hero_own_symbol(uintptr_t addr) {
    int state = atomic_load_explicit(&hero_own_symbols_state, memory_order_acquire);
    if (state == HERO_OWN_UNREAD) {
        int expected = HERO_OWN_UNREAD;
        if (atomic_compare_exchange_strong(&hero_own_symbols_state, &expected, HERO_OWN_READING)) {
            int found = hero_own_read(&hero_own_symbols);
            atomic_store_explicit(&hero_own_symbols_state, found ? HERO_OWN_READ : HERO_OWN_NONE, memory_order_release);
        }
        state = atomic_load_explicit(&hero_own_symbols_state, memory_order_acquire);
    }
    if (state != HERO_OWN_READ) return NULL;
    const HeroOwnSymbols *t = &hero_own_symbols;
    if (addr < t->bias) return NULL;
    uintptr_t at = addr - t->bias;
    int in_code = 0;
    for (size_t i = 0; i < t->segment_count && !in_code; i++) {
        const ElfW(Phdr) *p = &t->segments[i];
        in_code = p->p_type == PT_LOAD && (p->p_flags & PF_X) != 0 && at >= p->p_vaddr && at - p->p_vaddr < p->p_memsz;
    }
    if (!in_code) return NULL;
    for (size_t i = 0; i < t->count; i++) {
        const ElfW(Sym) *s = &t->syms[i];
        if ((s->st_info & 0xf) != STT_FUNC || s->st_shndx == SHN_UNDEF || s->st_size == 0) continue;
        if (at < s->st_value || at - s->st_value >= s->st_size) continue;
        if (s->st_name == 0 || s->st_name >= t->names_size) continue;
        return t->names + s->st_name;
    }
    return NULL;
}

#endif
