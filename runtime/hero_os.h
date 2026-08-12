/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_os.h — the program's three edges: files, arguments, exit status.
 *
 * A SEPARATE HEADER, AND THAT IS THE POINT (design.md §1.11, panel 036).
 *
 * These are not built-ins. `read_file`, `write_file`, `args` and `exit` are
 * written **in Heroes**, in `crates/heroes/src/library/source.hero`, over
 * `extern` declarations against this file — so they arrive by the same door as
 * SQLite and libm, and clang checks their signatures against these declarations
 * exactly as it checks a binding against `<sqlite3.h>`.
 *
 * Three judges argued for that over making them built-ins, and the record is
 * worth keeping beside the code. §1.7's mechanical test: a `T?` is a
 * per-translation-unit generated struct, so the C runtime cannot name one and
 * `read_file -> str?` cannot be a runtime entry point without a second
 * composition form in the emitter. And the precedent runs the same way — Pascal
 * predeclared file I/O, command-line access and termination; Kernighan's 1981
 * paper itemises all three as defects; Oberon predefines none of them; Nim, whose
 * surface this project copies, reaches the same ergonomics from an auto-imported
 * module rather than from the compiler.
 *
 * So what is here is only what the language cannot express: the syscalls, and a
 * status code the Heroes side turns into a `T?`.
 */

#ifndef HERO_OS_H
#define HERO_OS_H

#include "heroes_runtime.h"

/* The status codes the Heroes wrappers translate into error codes. Small
 * integers rather than errno: errno's values are platform-specific, and the
 * Heroes side must produce the same `e.code` string everywhere. */
#define HERO_OS_OK 0
#define HERO_OS_NOT_FOUND 1
#define HERO_OS_FAILED 2

/* The whole file, as an owned `str` (+1). `*status` says whether it worked; on
 * anything but HERO_OS_OK the returned string is empty and owns nothing.
 *
 * Owning rather than borrowing because §4.20 says so: a `str` that came from a
 * foreign pointer must be copied through `hero_str_from_bytes`, or its bytes
 * outlive nothing and the magic word that catches a fabricated `HeroStr` is
 * absent. */
HeroStr hero_file_read(const char *path, int64_t *status);

/* The text, written whole, replacing whatever was there. Returns a status. */
int64_t hero_file_write(const char *path, HeroStr text);

/* The arguments after the program's name: `hero_args_count()` of them, each
 * `hero_args_at(i)` an owned `str` (+1).
 *
 * `hero_args_set` is called by the generated `main` before anything else. Out of
 * range is a panic rather than an empty string — an out-of-range index aborts
 * everywhere else in this language (§4.9), and this is the same rule. */
void hero_args_set(int argc, char **argv);
int64_t hero_args_count(void);
HeroStr hero_args_at(int64_t index);

/* Ends the program with this status, and never returns.
 *
 * `_Noreturn` is not decoration: without it clang's flow analysis treats the
 * call as ordinary and `-Werror=conditional-uninitialized` fires on code after
 * it. C itself took 22 years and WG14 N1453 to admit that, and the paper's
 * nominated functions are exactly this one and its neighbours.
 *
 * **It bypasses `hero_runtime_check_leaks()`**, deliberately and on the record: a
 * program that asks to stop now is not asking for its allocations to be audited,
 * and the alternative — running the gate here — would report a leak for every
 * live value in every frame the exit unwinds past. Panel 036 flagged it; the
 * cost is that a golden which exits via `exit` loses its leak check, and the
 * corpus keeps one path that does not. */
_Noreturn void hero_exit(int64_t code);

#endif /* HERO_OS_H */
