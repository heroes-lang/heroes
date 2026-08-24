/* os.c — the program's three edges, in C because the language cannot reach them
 * (design.md §1.11 Tier 1, `hero_os.h` for why they are not built-ins).
 *
 * Included by `runtime.c` into the one translation unit, like every other part.
 * Nothing here is generic and nothing here allocates outside `str`: the file
 * reader builds its result with `hero_str_from_bytes`, which is the single
 * allocation point §4.20 insists on.
 */

/* **stdout is bytes, on every platform.**
 *
 * Windows opens the standard streams in *text* mode, where the C runtime turns
 * every `\n` into `\r\n` on the way out. The spec says `print` writes its values
 * "with no separator and exactly one trailing newline" — one — and on Windows it
 * was writing two bytes for that one newline, behind the language's back. A
 * corpus program's `main.expected` is compared byte for byte across three
 * platforms, and it caught this on the third leg's second run.
 *
 * `_setmode` is the documented switch and it is called once, before anything is
 * written. Nothing changes on POSIX, where there was never a translation. */
#if defined(_WIN32)
#include <fcntl.h>
#include <io.h>
static void hero_stdout_is_bytes(void) {
    static int done = 0;
    if (!done) {
        done = 1;
        _setmode(_fileno(stdout), _O_BINARY);
        _setmode(_fileno(stderr), _O_BINARY);
    }
}
#else
static void hero_stdout_is_bytes(void) {}
#endif

#include <stdio.h>
#include <stdlib.h>

/* The arguments, as the generated `main` received them. Static, like every other
 * piece of runtime state, so a decoy runtime linked beside this one cannot reach
 * them (runtime.c's own reason for one translation unit). */
static int hero_argc = 0;
static char **hero_argv = NULL;

void hero_args_set(int argc, char **argv) {
    hero_stdout_is_bytes();
    hero_argc = argc;
    hero_argv = argv;
}

/* argv[0] is the program's own name, and `args()` promises "the arguments after
 * the program name" — the off-by-one that every language's first CLI program
 * gets wrong, answered once, here. */
int64_t hero_args_count(void) {
    return hero_argc > 0 ? (int64_t)hero_argc - 1 : 0;
}

HeroStr hero_args_at(int64_t index) {
    if (index < 0 || index >= hero_args_count()) {
        hero_panic("argument index out of range");
    }
    return hero_str_from_cstr(hero_argv[index + 1]);
}

/* The same argument, BORROWED and unconverted — the one input a program cannot
 * decline to receive, handed over as bytes so the program can decide (panel 089's
 * ffi-pragmatist, queued there and landed 2026-08-24).
 *
 * `hero_args_at` above converts EAGERLY, which is why no `cstr` ever reaches a
 * Heroes program and why `validated` — which rescues every other C string in the
 * language — cannot rescue argv. Measured before this existed: `./prog $'\xff\xfe'`
 * was `panic: hero_str_from_bytes: not well-formed UTF-8`, exit 134, for an
 * argument the shell chose and the program never asked for.
 *
 * **Out of range is NULL and not a panic**, and that is the whole difference from
 * its neighbour: a caller that must branch cannot branch on a panic. `args_at`
 * keeps its abort because an out-of-range index is the program's own mistake
 * (§4.9's rule for every index in this language); this one is reached only by the
 * library's own loop, which cannot go out of range, and NULL is what makes it
 * checkable rather than fatal if it ever does.
 *
 * Borrowed, not owned: the bytes belong to `main`'s argv and outlive every
 * Heroes value built from them. Nothing here allocates, so nothing here can leak;
 * the copy happens in `validated`, through the one allocation point §4.20
 * insists on. */
const char *hero_args_raw(int64_t index) {
    if (index < 0 || index >= hero_args_count()) {
        return NULL;
    }
    return hero_argv[index + 1];
}

/* The whole file, read with `fseek`/`ftell`/`fread`.
 *
 * A binary read (`"rb"`), because a `str` is bytes: §4.3 measures and indexes a
 * string in bytes, so translating CRLF here would make `len` disagree with the
 * file on one platform and not the other. */
/* **The runtime does nothing to a path, and that is the portability** (recorded
 * 2026-08-14, after a day in which four separate things broke on Windows over path
 * separators and none of them was here).
 *
 * There is no `strcat`, no separator literal, no normalisation: the string the
 * program wrote reaches `fopen` verbatim. Windows accepts `/` in every filesystem
 * API — it is `cmd.exe`, not the OS, that reads a leading `/` as a switch — so a
 * program writing `read_file("data/in.txt")` works on all three platforms, and
 * `examples/adventure/main.hero` proves it on the Windows CI leg with a literal
 * `examples/adventure/walkthrough.txt`.
 *
 * **Written as a claim that can die**: if a platform is ever added whose `fopen`
 * refuses `/`, this comment is wrong and the corpus is what says so — the adventure
 * example fails on that leg and nowhere else. Until then, adding separator handling
 * here would be the repair that creates the defect.
 *
 * Both modes are binary (`rb`/`wb`) for the sibling reason: text mode on Windows
 * translates newlines, and a language whose `str` is measured in bytes cannot have
 * a file grow one byte per line on one platform. */
HeroStr hero_file_read(const char *path, int64_t *status) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        *status = HERO_OS_NOT_FOUND;
        return hero_str_from_bytes("", 0);
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    long size = ftell(file);
    if (size < 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    char *buffer = hero_alloc((size_t)size + 1);
    size_t got = fread(buffer, 1, (size_t)size, file);
    fclose(file);
    if (got != (size_t)size) {
        hero_release(buffer);
        *status = HERO_OS_FAILED;
        return hero_str_from_bytes("", 0);
    }
    /* **The bytes are not text, and that is a value rather than a death** (panel
     * 087). `hero_str_from_bytes` aborts on ill-formed UTF-8, and that abort is
     * load-bearing everywhere else — `hero_str_chars` takes one byte off an
     * invalid sequence and depends on it for the language-wide well-formedness
     * invariant — so the repair is a pre-check at the *caller*, never a weaker
     * conversion. Asking first is what turns the fourth program state into a
     * `status` the wrapper can translate.
     *
     * Before this, `read_file` was typed `-> str?` (spec:179) and killed the
     * process on a photograph: a program handling both `.ok` and `.err` reached
     * neither arm. The Rust bootstrap's own record walk survived the same input
     * because `read_to_string` returns `Err`, and the Heroes port transcribed
     * that guard faithfully to `suite_records.hero:148` — where it could never
     * fire. `docs/defects/002` is the record. */
    if (!hero_utf8_valid(buffer, (int64_t)got)) {
        hero_release(buffer);
        *status = HERO_OS_NOT_TEXT;
        return hero_str_from_bytes("", 0);
    }
    HeroStr text = hero_str_from_bytes(buffer, (int64_t)got);
    hero_release(buffer);
    *status = HERO_OS_OK;
    return text;
}

int64_t hero_file_write(const char *path, HeroStr text) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        return HERO_OS_FAILED;
    }
    int64_t len = hero_str_len(text);
    if (len > 0) {
        size_t put = fwrite(hero_str_cstr(text), 1, (size_t)len, file);
        if (put != (size_t)len) {
            fclose(file);
            return HERO_OS_FAILED;
        }
    }
    return fclose(file) == 0 ? HERO_OS_OK : HERO_OS_FAILED;
}

/* The error stream, written whole. No status, and that absence is the point.
 *
 * WHY IT IS HERE RATHER THAN A POSIX BINDING IN THE COMPILER (author decision
 * 2026-08-24). `selfhost/cli_io.hero` reached `write(2)` through
 * `extern "unistd.h"`, so `seed/heroes.c` `#include`d that header and the
 * self-hosted compiler could not be built on a platform without POSIX headers —
 * measured on the archive's own CI leg, which had been green while testing a
 * compiler that did not have the problem. `fwrite` to `stderr` is C89 and needs
 * no POSIX at all, and putting it here keeps the platform question in the one
 * file in this project that is allowed to know what machine it is on. It also
 * removes the seed's only POSIX include.
 *
 * WHY IT RETURNS NOTHING. `write(2)` may write fewer bytes than it was given,
 * and two of the compiler's three call sites discarded the count — a §1.12 hole
 * where a truncated diagnostic would look like a complete one. The loop below is
 * the answer: this writes all of it or the process has no error channel left, so
 * there is no count for a caller to ignore.
 *
 * NO `fflush`, AND THAT IS MEASURED RATHER THAN ASSUMED. C11 7.21.3p7 says the
 * standard error stream is not fully buffered, and the case that matters was run
 * before this was written: `fwrite(m, 1, n, stderr)` followed by `abort()`
 * delivers the text to a file **and** through a pipe, exit 134 (2026-08-23, the
 * measurement in this decision's queue item). The instrument that catches a
 * regression already exists and is large: every `tests/golden/check/` case
 * compares the diagnostic text that leaves through here byte for byte — 65
 * `check` plus 76 `annotations` checks in the net — so a truncating write is 141
 * red checks rather than a silent short line. */
void hero_write_err(HeroStr text) {
    int64_t len = hero_str_len(text);
    if (len <= 0) {
        return;
    }
    hero_stdout_is_bytes();
    const char *bytes = hero_str_cstr(text);
    size_t left = (size_t)len;
    while (left > 0) {
        size_t put = fwrite(bytes, 1, left, stderr);
        if (put == 0) {
            /* Nowhere left to say so: a CLI whose error channel has failed
             * cannot report that its error channel has failed. */
            return;
        }
        bytes += put;
        left -= put;
    }
}

/* The truncation is deliberate and is the reason `exit` is not a plain `extern`:
 * C's `exit` takes an `i64`, Heroes' `i64` is `int64_t`, and binding one to the
 * other is `conflicting types for 'exit'` (panel 030 R3, reproduced on clang 21).
 * A shell reads the low 8 bits anyway. */
_Noreturn void hero_exit(int64_t code) {
    exit((int)code);
}
