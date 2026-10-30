/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_compiler.h: the doors the compiler binds for itself, which no program
 * needs (a golden case binds one to hold it to its contract).
 *
 * A HEADER OF ITS OWN, AND NOT LINES OF `hero_os.h`: every unit a program is
 * compiled into reads `hero_os.h` and saves every word it writes around its
 * groups' headers (`heroes_guard_open.h`, held to `selfhost/emit/
 * guarded_names.hero`), and these are the compiler's alone, bound in its own
 * groups, so no program's unit pays for them. Adding a function moves no ABI
 * stamp (panel 089's reading): an older runtime is an undefined symbol at the
 * link. `runtime.c` includes this file, so clang checks the definitions
 * against it and the runtime's key covers it. */
#ifndef HERO_COMPILER_H
#define HERO_COMPILER_H

#include "heroes_runtime.h"

/* `name` set to `value` in this process's environment, which every child it
 * starts afterwards inherits (`hero_run_go` passes no block of its own):
 * HERO_OS_OK, or HERO_OS_FAILED where the system refused or either is not
 * UTF-8 on Windows. Defect 509: the compiler names the symbolizer a program
 * it runs under `--sanitize` uses (`selfhost/cli/symbolized.hero`). */
int64_t hero_env_put(const char *name, const char *value);

/* The key of a file's bytes, as the compiler's build cache takes it (defect
 * 484, `parts/key.c`, bound by `selfhost/module/reading.hero`): the sixteen
 * characters `reading.keyed` makes in Heroes, byte for byte, without building
 * the text it hashed. For a file of UTF-8, `digest` of its bytes; for one that
 * is not, `n` and the last fifteen characters of `digest` of the shown read's
 * whole answer, its marks then its text (`hero_file_read_shown`,
 * `hero_os.h`). `*status` is that read's: HERO_OS_OK or HERO_OS_NOT_TEXT with
 * the key, and HERO_OS_NOT_FOUND or HERO_OS_FAILED with "". */
HeroStr hero_file_key(const char *path, int64_t *status);

/* The key of a text in hand, `cli/digest.hero`'s over its bytes, as
 * `hero_file_key` keys a file of UTF-8 (`parts/key.c`, bound by
 * `selfhost/emit/provenance.hero`): each module's text as the compilation
 * holds it, keyed where it is rather than read from the disk again. */
HeroStr hero_text_key(HeroStr text);

/* The binary32 nearest a decimal's value, read once in the C locale
 * (`parts/f64.c`, bound by `selfhost/emit/literal.hero`, defect 2276): what an
 * `f32` literal holds. A double narrowed to a float rounds twice, and at a
 * decimal just off a binary32 midpoint the second rounding takes the far
 * neighbour. */
float hero_f32_of_decimal(const char *text);

#endif
