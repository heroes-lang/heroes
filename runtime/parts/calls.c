/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/calls.c — the calls that led to an abort, beneath its place (panel
 * 253's R5, ratified 2026-11-19; design.md §1.12, §4.17).
 *
 * An abort names one place, the innermost, by the place the compiler passes
 * (`parts/place.c`). An index out of range in `pick`, called by `middle`,
 * called by `outer`, called by `main`, said `at deep.hero:3:12, in
 * deep.pick` and nothing of the three calls above it; an assert inside a
 * package said the package's line and never the program's call into it. So
 * every stop that ends through `hero_abort` (`parts/panic.c`) tells, beneath
 * its report, the calls of the program that led there, innermost first:
 *
 *     panic: array index out of range
 *       at deep.hero:3:12, in deep.pick
 *       called from 0x100000e48, in deep.middle
 *       called from 0x100000eb4, in deep.outer
 *       called from 0x100000f1c, in deep.main
 *
 * WHAT IT READS. The frame records, from this function's own frame up, inside
 * the thread's stack bounds (`parts/stack.c`'s, measured as the thread is
 * claimed), each return address inside a Heroes function told. Starting at its
 * own frame, the walk meets the runtime's frames first and then the frame
 * that called the runtime, the one the abort was raised in, which is told
 * too: at `-O2` the function whose place the report names is often inlined
 * into it (`pick` and `middle` into `outer`), and that frame's address is the
 * one whose line table holds every inlined call (panel 253's critic, finding
 * 1, by `otool` and `atos`; the sitting's prototype skipped it and lost two
 * calls of three). What is not repeated is the place itself: where that frame
 * is the place's own function, or a function of the Heroes library that
 * stops at its caller's place (`emit/stop_place.hero`), the walk's first
 * lines would say the place again, and they are left out (`hero_calls_choose`).
 *
 * WHAT A LINE SAYS. The address is the executable file's own, the one
 * `atos -o <binary>`, `llvm-symbolizer --obj=<binary>` and `addr2line -e
 * <binary>` take, so a reader holding the binary turns it into a `.hero` line;
 * every build carries line tables (`selfhost/cli/flags.hero`, `debug_words`).
 * The runtime reads none of them (the sitting's ruling): `heroes run` and
 * `heroes test`, the program's parent, do, and print the line in its place
 * (`selfhost/cli/abort_calls.hero`), `called from deep.hero:6, in
 * deep.middle`, every inlined call with its own line. They ask through the
 * file `HEROES_CALLS` names, read once as the program starts and taken out of
 * its environment so no program it starts inherits it (`hero_calls_channel`):
 * the frames go there instead of to stderr, and the parent tells them, so a
 * program's stderr is never a pipe the compiler reads.
 *
 * WHAT IT MAY DO, AT AN ABORT. Every read of a frame is checked against the
 * thread's bounds and must climb; the walk runs on the thread's own stack
 * alone, so a stop told from a signal's alternate stack (the stack guard's)
 * walks nothing. It allocates nothing: the names are the symbol tables' own
 * strings and every line is written from the stack, by `hero_stack_say`
 * (`write(2)`) or to the file by `write`. **On Linux no name is asked of
 * `dladdr`**, which waits while another thread holds the loader inside
 * `dlopen` (glibc 2.41, the sitting's ffi-pragmatist and critic, finding 15:
 * still waiting after 5 s), so an abort beside a library's constructor would
 * hang after its report: the executable's own table answers
 * (`parts/symbols.c`), and every Heroes function lives in the executable.
 * Darwin's `dladdr` returned at once in the same probe and is asked there, as
 * the guard asks it.
 *
 * WHAT IT CANNOT SEE, said. A frame with no record: C built without frame
 * records hides every Heroes caller above it (`-fomit-frame-pointer`, a
 * library's choice; the program's own units and the runtime keep records on
 * every POSIX target, `-fno-omit-frame-pointer` (`selfhost/cli/flags.hero`'s
 * `frame_words`), since x86-64's clang omits them at `-O2`). A binary
 * stripped of its symbol table names no frame. Windows tells no call yet:
 * its arm, at the end of this file, says why.
 */

#if !defined(_WIN32)

#include <errno.h>
#include <fcntl.h>
#if defined(__APPLE__)
#include <mach-o/loader.h>
#else
#include <sys/auxv.h>
#endif

/* At most this many distinct calls are told, innermost first, and at most this
 * many frames read; a call repeated frame after frame, a recursion, is one
 * line with its count. */
#define HERO_CALLS_KEPT 64
#define HERO_CALLS_WALKED 4096

/* The function the report's place names, as its writer was handed it
 * (`parts/place.c`, `hero_report_place`), or NULL where the stop names none.
 * The first place written wins: a report writes one, and one report is
 * written per process. */
static _Atomic(const char *) hero_calls_place = NULL;

static void hero_calls_note_place(const char *function) {
    const char *none = NULL;
    if (function != NULL) atomic_compare_exchange_strong(&hero_calls_place, &none, function);
}

/* The file the program's parent asked the calls to be written to, empty where
 * none was asked. A program the system started with more rights than its
 * caller's (set-user-ID) takes no file from its environment. */
#define HERO_CALLS_VARIABLE "HEROES_CALLS"
static char hero_calls_file[4096];

static void hero_calls_channel(void) {
    const char *given = getenv(HERO_CALLS_VARIABLE);
    if (given == NULL) return;
#if defined(__APPLE__)
    int secure = issetugid();
#else
    int secure = getauxval(AT_SECURE) != 0;
#endif
    size_t n = strlen(given);
    if (!secure && n > 0 && n < sizeof hero_calls_file) memcpy(hero_calls_file, given, n + 1);
    unsetenv(HERO_CALLS_VARIABLE);
}

#if defined(__APPLE__)
/* The executable's Mach-O header, where the system mapped it. Weak, so an
 * image that is not an executable links and walks nothing. */
extern const struct mach_header_64 _mh_execute_header __attribute__((weak_import));

/* How far the system moved the executable from the addresses its file gives:
 * the header's place less the `__TEXT` segment's address in the file, read off
 * the load commands mapped beside it. 0 and false where it is not there. */
static int hero_calls_slide(uintptr_t *slide) {
    const struct mach_header_64 *mh = &_mh_execute_header;
    if (mh == NULL || mh->magic != MH_MAGIC_64) return 0;
    const char *at = (const char *)(mh + 1);
    for (uint32_t i = 0; i < mh->ncmds; i++) {
        const struct load_command *lc = (const struct load_command *)at;
        if (lc->cmdsize < sizeof *lc) return 0;
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *seg = (const struct segment_command_64 *)at;
            if (strncmp(seg->segname, "__TEXT", sizeof seg->segname) == 0) {
                *slide = (uintptr_t)mh - (uintptr_t)seg->vmaddr;
                return 1;
            }
        }
        at += lc->cmdsize;
    }
    return 0;
}
#endif

/* The symbol of the executable's function whose bytes hold `addr`, or NULL,
 * and in `*file` the address the file gives it. Linux: the executable's own
 * table, never `dladdr` (above). Darwin: `dladdr`, an answer only where the
 * image is the executable. */
static const char *hero_calls_symbol(uintptr_t addr, uintptr_t *file) {
#if defined(__linux__)
    const char *own = hero_own_symbol(addr);
    if (own == NULL || !hero_own_file_address(addr, file)) return NULL;
    return own;
#elif defined(__APPLE__)
    Dl_info info;
    uintptr_t slide = 0;
    if (dladdr((void *)addr, &info) == 0 || info.dli_sname == NULL) return NULL;
    if (info.dli_fbase != (const void *)&_mh_execute_header || !hero_calls_slide(&slide)) return NULL;
    *file = addr - slide;
    return info.dli_sname;
#else
    (void)addr;
    (void)file;
    return NULL;
#endif
}

/* A Heroes symbol whose component is the compiler's own marker, a digit
 * (`emit/ctype.hero`'s `SYNTHETIC`): a callback's thunk, `h_0cb_<module>_<name>`,
 * which C calls and which calls the function it names, inlined into it at
 * `-O2`. Its frame is told only where it holds that function's code
 * (`hero_calls_choose`): at `-O0` it is a call no line of the program wrote,
 * and told it reads as a second call of the function it enters (the
 * sitting's ffi-pragmatist). */
static int hero_calls_is_glue(const char *sym) {
    if (sym[0] == '_') sym++;
    return sym[2] >= '0' && sym[2] <= '9';
}

/* A function of the Heroes library, whose stops name its caller's place
 * (`emit/stop_place.hero`, `in_library`): `h_library_...`, the component
 * `source.LIBRARY_MODULE` gives every one of them in every program. */
static int hero_calls_in_library(const char *sym) {
    if (sym[0] == '_') sym++;
    return strncmp(sym, "h_library_", 10) == 0;
}

/* A name as the walk tells it, into a buffer on the stack: the guard's
 * spelling (`hero_stack_say_heroes_name`), the program's own spellings first,
 * cut where the buffer ends. `*invented` says whether the name was one the
 * compiler made beside the module's, an instance's among them. */
#define HERO_CALLS_NAME 512
typedef struct {
    char text[HERO_CALLS_NAME];
    size_t used;
} HeroCallsName;

static void hero_calls_put(HeroCallsName *n, const char *s, size_t len) {
    size_t room = sizeof n->text - 1 - n->used;
    if (len > room) len = room;
    memcpy(n->text + n->used, s, len);
    n->used += len;
    n->text[n->used] = '\0';
}

static void hero_calls_spell(HeroCallsName *n, const char *sym, int *invented) {
    n->used = 0;
    n->text[0] = '\0';
    *invented = 0;
    if (sym[0] == '_') sym++;
    size_t whole = 0;
    const char *spelled = hero_stack_spelled(sym, strlen(sym), &whole);
    if (spelled != NULL) {
        hero_calls_put(n, spelled, whole);
        return;
    }
    const char *rest = sym + 2;
    /* A thunk is told by the function it enters, its marker left out, as the
     * guard tells it (defect 2277). */
    if (rest[0] >= '0' && rest[0] <= '9') {
        const char *marker = strchr(rest, '_');
        if (marker != NULL && marker[1] != '\0') rest = marker + 1;
    }
    const char *us = strchr(rest, '_');
    if (us == NULL || us[1] == '\0') {
        hero_calls_put(n, sym, strlen(sym));
        return;
    }
    size_t len = 0;
    const char *module = hero_stack_spelled(rest, (size_t)(us - rest), &len);
    if (module != NULL) {
        hero_calls_put(n, module, len);
    } else {
        hero_calls_put(n, rest, (size_t)(us - rest));
    }
    hero_calls_put(n, ".", 1);
    const char *named = us + 1;
    if (named[0] >= '0' && named[0] <= '9' && named[1] != '\0') {
        named++;
        *invented = 1;
    }
    hero_calls_put(n, named, strlen(named));
}

/* Whether the spelled name is the function the place names: the same text, or
 * for an instance of a generic function, the place's name with the instance's
 * `_<hash>` after it (`emit/mangle.hero`'s `instance_of`), since a place
 * names an instance by its template's name (`ir/mono.hero`). */
static int hero_calls_names_place(const HeroCallsName *n, int invented) {
    const char *place = atomic_load(&hero_calls_place);
    if (place == NULL) return 0;
    size_t p = strlen(place);
    if (n->used < p || memcmp(n->text, place, p) != 0) return 0;
    if (n->used == p) return 1;
    if (!invented || n->text[p] != '_' || n->used == p + 1) return 0;
    for (size_t i = p + 1; i < n->used; i++) {
        char c = n->text[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return 0;
    }
    return 1;
}

/* One call: the return address less one, inside the call instruction, which
 * is what every symbolizer is asked; the file's address of it; its symbol; how
 * many frames in a row returned there. */
typedef struct {
    uintptr_t file;
    const char *sym;
    uint64_t times;
} HeroCall;

typedef struct {
    HeroCall calls[HERO_CALLS_KEPT];
    int count;
    uint64_t beyond; /* calls past the kept ones */
    int cut;         /* the walk stopped at its bound, not at the stack's end */
} HeroCalls;

/* An AArch64 return address a C library built with pointer authentication
 * saved is signed in its upper bits; a user address is under 2^48 on both
 * AArch64 platforms. x86-64 signs nothing. */
static uintptr_t hero_calls_plain(uintptr_t ret) {
#if defined(__aarch64__)
    return ret & (((uintptr_t)1 << 48) - 1);
#else
    return ret;
#endif
}

/* The frame records from `fp` up, within [lo, hi). */
static void hero_calls_walk(uintptr_t fp, uintptr_t lo, uintptr_t hi, HeroCalls *out) {
    out->count = 0;
    out->beyond = 0;
    out->cut = 0;
    uintptr_t last = 0;
    for (int i = 0;; i++) {
        if (i == HERO_CALLS_WALKED) {
            out->cut = 1;
            return;
        }
        if (fp < lo || fp >= hi || hi - fp < 2 * sizeof(uintptr_t) || (fp & (sizeof(uintptr_t) - 1)) != 0) return;
        uintptr_t next = ((const uintptr_t *)fp)[0];
        uintptr_t ret = hero_calls_plain(((const uintptr_t *)fp)[1]);
        if (ret == 0) return;
        uintptr_t file = 0;
        const char *sym = hero_calls_symbol(ret - 1, &file);
        if (sym != NULL && hero_stack_is_heroes(sym)) {
            int again = out->count > 0 && last == ret - 1;
            if (again && out->beyond == 0) {
                out->calls[out->count - 1].times++;
            } else if (out->beyond == 0 && out->count < HERO_CALLS_KEPT) {
                out->calls[out->count].file = file;
                out->calls[out->count].sym = sym;
                out->calls[out->count].times = 1;
                out->count++;
            } else if (!again) {
                out->beyond++;
            }
            last = ret - 1;
        }
        if (next <= fp) return;
        fp = next;
    }
}

/* WHICH CALLS ARE TOLD, a mark for each kept one: 0 not told; 1 told; 2 told,
 * the place's own function inlined into it, so the parent leaves out of its
 * inlined calls the ones up to the place (`cli/abort_calls.hero`).
 *
 * The report already says its place, so the walk starts above it: past the
 * library's own functions, which stop at their caller's place
 * (`emit/stop_place.hero`), the first frame is the place's function, left
 * out, or a frame it was inlined into, a caller as well and told with its
 * mark. A stop that names no place leaves out nothing but the library's.
 * A thunk is told only where the frame under it is not the function it
 * enters, which is where that function was inlined into it. */
static void hero_calls_choose(const HeroCalls *c, unsigned char *told) {
    int at = 0;
    while (at < c->count && hero_calls_in_library(c->calls[at].sym)) told[at++] = 0;
    if (at < c->count && atomic_load(&hero_calls_place) != NULL) {
        HeroCallsName name;
        int invented = 0;
        hero_calls_spell(&name, c->calls[at].sym, &invented);
        told[at] = hero_calls_names_place(&name, invented) ? 0 : 2;
        at++;
    }
    for (; at < c->count; at++) {
        told[at] = 1;
        if (at > 0 && hero_calls_is_glue(c->calls[at].sym)) {
            HeroCallsName name, under;
            int invented = 0;
            hero_calls_spell(&name, c->calls[at].sym, &invented);
            hero_calls_spell(&under, c->calls[at - 1].sym, &invented);
            if (strcmp(name.text, under.text) == 0) told[at] = 0;
        }
    }
}

/* A number in decimal, into `buf`, its length returned. */
static size_t hero_calls_decimal(uint64_t v, char buf[24]) {
    char digits[24];
    size_t at = sizeof digits;
    do {
        digits[--at] = (char)('0' + (int)(v % 10));
        v /= 10;
    } while (v > 0);
    size_t n = sizeof digits - at;
    memcpy(buf, digits + at, n);
    return n;
}

/* The lines on stderr, as a binary run alone says them. */
static void hero_calls_say(const HeroCalls *c, const unsigned char *told) {
    for (int i = 0; i < c->count; i++) {
        if (told[i] == 0) continue;
        HeroCallsName name;
        int invented = 0;
        char hex[HERO_HEX_BUF];
        hero_calls_spell(&name, c->calls[i].sym, &invented);
        hero_stack_hex(c->calls[i].file, hex);
        hero_stack_say("  called from ");
        hero_stack_say(hex);
        hero_stack_say(", in ");
        hero_stack_say(name.text);
        if (c->calls[i].times > 1) {
            char count[24];
            size_t n = hero_calls_decimal(c->calls[i].times, count);
            hero_stack_say(", ");
            hero_stack_say_n(count, n);
            hero_stack_say(" times");
        }
        hero_stack_say("\n");
    }
    if (c->beyond > 0 || c->cut) {
        hero_stack_say("  and more calls above these");
        if (!c->cut) {
            char count[24];
            size_t n = hero_calls_decimal(c->beyond, count);
            hero_stack_say(", ");
            hero_stack_say_n(count, n);
        }
        hero_stack_say("\n");
    }
}

/* Every byte of `s` to `fd`, or false. */
static int hero_calls_write(int fd, const char *s, size_t n) {
    while (n > 0) {
        ssize_t put = write(fd, s, n);
        if (put < 0 && errno == EINTR) continue;
        if (put <= 0) return 0;
        s += (size_t)put;
        n -= (size_t)put;
    }
    return 1;
}

static int hero_calls_write_text(int fd, const char *s) { return hero_calls_write(fd, s, strlen(s)); }

/* THE PARENT'S FILE (`selfhost/cli/abort_calls.hero` reads it), one record a
 * line, every frame the walk kept and whether a binary alone would tell it:
 *
 *     heroes-calls 1
 *     spell <key>\t<spelling>          the program's spellings, as handed
 *     call <mark> <times> 0x<file address> <name>       `hero_calls_choose`'s mark
 *     beyond <count>                    or `cut`
 *
 * Created, never opened if it is there (`O_EXCL`, no link followed), so a
 * file named by an environment cannot be written over. False where it could
 * not be written, and the lines go to stderr instead. */
static int hero_calls_to_file(const HeroCalls *c, const unsigned char *told) {
    int fd = open(hero_calls_file, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW | O_CLOEXEC, 0600);
    if (fd < 0) return 0;
    int ok = hero_calls_write_text(fd, "heroes-calls 1\n");
    const char *row = hero_stack_spellings;
    while (ok && row != NULL && *row != '\0') {
        const char *end = row;
        while (*end != '\0' && *end != '\n') end++;
        ok = hero_calls_write_text(fd, "spell ") && hero_calls_write(fd, row, (size_t)(end - row)) && hero_calls_write_text(fd, "\n");
        row = *end == '\n' ? end + 1 : end;
    }
    for (int i = 0; ok && i < c->count; i++) {
        HeroCallsName name;
        int invented = 0;
        char hex[HERO_HEX_BUF];
        char count[24];
        hero_calls_spell(&name, c->calls[i].sym, &invented);
        hero_stack_hex(c->calls[i].file, hex);
        size_t n = hero_calls_decimal(c->calls[i].times, count);
        const char *mark = told[i] == 2 ? "call 2 " : told[i] == 1 ? "call 1 " : "call 0 ";
        ok = hero_calls_write_text(fd, mark) && hero_calls_write(fd, count, n) && hero_calls_write_text(fd, " ") &&
             hero_calls_write_text(fd, hex) && hero_calls_write_text(fd, " ") && hero_calls_write(fd, name.text, name.used) &&
             hero_calls_write_text(fd, "\n");
    }
    if (ok && c->cut) ok = hero_calls_write_text(fd, "cut\n");
    if (ok && !c->cut && c->beyond > 0) {
        char count[24];
        size_t n = hero_calls_decimal(c->beyond, count);
        ok = hero_calls_write_text(fd, "beyond ") && hero_calls_write(fd, count, n) && hero_calls_write_text(fd, "\n");
    }
    close(fd);
    return ok;
}

/* Told once per process, by the thread whose report this is. */
static _Atomic int hero_calls_told = 0;

/* Not inlined, so its own frame is a frame record the walk starts from. */
__attribute__((noinline)) static void hero_calls_tell(void) {
    if (atomic_exchange(&hero_calls_told, 1) != 0) return;
#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)
    uintptr_t lo = hero_stack_walk_lo, hi = hero_stack_walk_hi;
#else
    uintptr_t lo = hero_stack_lo, hi = hero_stack_hi;
#endif
    uintptr_t fp = (uintptr_t)__builtin_frame_address(0);
    if (hi == 0 || fp < lo || fp >= hi) return;
    HeroCalls c;
    hero_calls_walk(fp, lo, hi, &c);
    if (c.count == 0) return;
    unsigned char told[HERO_CALLS_KEPT];
    hero_calls_choose(&c, told);
    if (hero_calls_file[0] != '\0' && hero_calls_to_file(&c, told)) return;
    hero_calls_say(&c, told);
}

#else

/* WINDOWS TELLS NO CALL YET, and the report says its place alone, as before
 * (`parts/stack.c`'s *Windows has no walk*). What a Windows arm needs, read
 * and not run, the box being off while this was written (2026-11-20): the
 * walk itself is `RtlCaptureStackBackTrace`, which reads the unwind tables
 * every x64 image carries and needs no frame record, so the frame word is not
 * asked there (`hero_frame_records_asked`); but a PE holds no table of its
 * functions' names, which live in the PDB the link writes beside it
 * (`selfhost/cli/flags.hero`, a `-g` word on the link), so the runtime could
 * tell no frame as the program's without DbgHelp, and the parent's
 * `llvm-symbolizer` reads a Heroes function's lines from that PDB wrongly,
 * the last file's line for every address (defect 509's measurement on the
 * box, `selfhost/cli/symbolized.hero`). Both are owed to the box. */
static void hero_calls_note_place(const char *function) { (void)function; }
static void hero_calls_channel(void) {}
static void hero_calls_tell(void) {}

#endif
