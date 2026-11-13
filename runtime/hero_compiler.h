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
 * starts afterwards inherits: HERO_OS_OK, or HERO_OS_FAILED where the system
 * refused or either is not UTF-8 on Windows (defect 509).
 *
 * **NO SOURCE OF THIS TREE CALLS IT SINCE FEATURE 2424**, which gives a child
 * its variables at the spawn (`hero_run_env`, `hero_os.h`; panel 242's R6),
 * and R6 retires it from this header. It stays for one reader alone, the
 * committed `seed/heroes.c`, emitted before its five callers moved, which
 * binds it (eleven references, `grep -c hero_env_put seed/heroes.c` on
 * 2026-11-10): the first command of CLAUDE.md § Commands compiles that seed
 * against this runtime, and every route a refresh of the seed is rebuilt by
 * (`seed/README.md`, `one-stage`, `abi-put-back`, `scaffold`) compiles the
 * seed before it against the refresh's own runtime. So it leaves in the first
 * commit after the refresh whose seed no longer names it, and not before. */
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

/* The SHA-256 of the bytes of the file at `path`, whatever they are, as 64
 * lower-case hexadecimal digits (panel 235's R6, `parts/sha256.c`, bound by
 * `selfhost/module/fingerprint.hero`): a package's fingerprint covers every
 * file of its tree, a header or an image as well as a module, and a file that
 * is not UTF-8 is no `str` to hash in Heroes. `*status` is HERO_OS_OK, or the
 * read's HERO_OS_NOT_FOUND or HERO_OS_FAILED with "". */
HeroStr hero_file_sha256(const char *path, int64_t *status);

/* The same over a text in hand: the listing of a tree's files the
 * fingerprint is the SHA-256 of. */
HeroStr hero_text_sha256(HeroStr text);

/* 1 where this machine's clang probes a frame larger than a page only when a
 * compile asks it to, with `-fstack-clash-protection`, and 0 where its
 * prologue probes unasked (defect 2253, `parts/run.c`, bound by
 * `selfhost/cli/flags.hero`). */
int64_t hero_frame_probes_asked(void);

/* The binary32 nearest a decimal's value, read once in the C locale
 * (`parts/f32-literal.c`, bound by `selfhost/emit/literal.hero`, defect 2276): what an
 * `f32` literal holds. A double narrowed to a float rounds twice, and at a
 * decimal just off a binary32 midpoint the second rounding takes the far
 * neighbour. */
float hero_f32_of_decimal(const char *text);

/* A lock one run of the compiler holds on a file beside a folder it works in
 * (defect 2453, `parts/lock.c`, bound by `selfhost/fetch/run_lock.hero`;
 * panel 246's R1): a group declares it as a handle, `record Lock tag
 * hero_lock`, which `hero_lock_take` acquires and `hero_lock_drop` consumes. */
typedef struct hero_lock hero_lock;

/* The lock is another run's: held by a live run, in this process or another,
 * or, asked `fresh`, a file of that name stands already. Beside the
 * HERO_OS_* answers, none of which it takes. */
#define HERO_LOCK_HELD 6

/* An exclusive lock on the file at `path`, taken without waiting, held until
 * `hero_lock_drop` or the end of this process however it ends, and never held
 * by a program this process starts once it runs. `fresh` nonzero makes the
 * file and answers HERO_LOCK_HELD where one stands; zero opens one that
 * stands, HERO_OS_NOT_FOUND where none does. `*status` is HERO_OS_OK with the
 * lock held, else HERO_LOCK_HELD, HERO_OS_UNSUPPORTED where the file system
 * keeps no lock (a file a fresh take made removed again), HERO_OS_NOT_FOUND
 * or HERO_OS_FAILED, `hero_fs_why` saying why. A block comes back whatever the
 * status, and is given back through `hero_lock_drop` once. */
hero_lock *hero_lock_take(const char *path, int64_t fresh, int64_t *status);

/* The block given back, and its lock where it holds one: with `keep` zero the
 * file is removed first, while still held, and with `keep` nonzero it stays,
 * so the folder beside it reads as a dead run's to the next run. HERO_OS_OK,
 * or HERO_OS_FAILED where the removal or the close failed, the lock given back
 * either way. */
int64_t hero_lock_drop(hero_lock *lock, int64_t keep);

#endif
