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
 * written **in Heroes**, in `selfhost/library_source.hero`, over
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
/* The file was there, it read whole, and its bytes are not text. A `str` is
 * UTF-8 by definition (spec:51), so this is the one failure a caller cannot
 * discover by looking at the value it got back — and until panel 087 it was not
 * a failure at all but an abort inside `hero_str_from_bytes`, unreachable by any
 * Heroes branch. The Tier-2 wrapper answers `not_text` for it since 2026-09-03 —
 * the code `validated` already gives a cstr in the same state — after collapsing
 * it into `read_failed` for two weeks (panel 087 left the naming to the author,
 * who chose the robust form: a code that says what happened). */
#define HERO_OS_NOT_TEXT 3

/* The whole file, as an owned `str` (+1). `*status` says whether it worked; on
 * anything but HERO_OS_OK the returned string is empty and owns nothing — which
 * HERO_OS_NOT_TEXT keeps: the buffer is released before the empty str is built.
 *
 * Owning rather than borrowing because §4.20 says so: a `str` that came from a
 * foreign pointer must be copied through `hero_str_from_bytes`, or its bytes
 * outlive nothing and the magic word that catches a fabricated `HeroStr` is
 * absent. */
HeroStr hero_file_read(const char *path, int64_t *status);

/* The text, written whole, replacing whatever was there. Returns a status. */
int64_t hero_file_write(const char *path, HeroStr text);

/* One blob to the error stream, written whole, no newline added and no status
 * returned (author decision 2026-08-24; `runtime/parts/os.c` carries the whole
 * argument). It exists so the compiler's own `eprint` does not have to bind
 * POSIX: `selfhost/cli/io.hero` reached `write(2)` through `extern "unistd.h"`,
 * which put that header into `seed/heroes.c` and made the self-hosted compiler
 * unbuildable on a platform without POSIX headers.
 *
 * No status because there is nothing a caller could do with one, and because the
 * count `write(2)` returns was being discarded at two of three call sites — a
 * short write that looked like a whole one. This writes all of it or the process
 * has no error channel left. */
void hero_write_err(HeroStr text);

/* The arguments after the program's name: `hero_args_count()` of them, each
 * `hero_args_at(i)` an owned `str` (+1).
 *
 * `hero_args_set` is called by the generated `main` before anything else. Out of
 * range is a panic rather than an empty string — an out-of-range index aborts
 * everywhere else in this language (§4.9), and this is the same rule. */
void hero_args_set(int argc, char **argv);
int64_t hero_args_count(void);
HeroStr hero_args_at(int64_t index);

/* The same argument, BORROWED and unconverted, so a program can decide what to do
 * about bytes that are not text (panel 089). `hero_args_at` converts eagerly and
 * aborts on ill-formed input — right for a program that knows its arguments are
 * text, wrong for the one input a program cannot decline to receive.
 *
 * Out of range is **NULL rather than a panic**, unlike its neighbour: a caller
 * that must branch cannot branch on a panic. The bytes belong to `main`'s argv,
 * so nothing here allocates and nothing here can leak. */
const char *hero_args_raw(int64_t index);

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

/* How wide C's `long` is on THIS target, in bits.
 *
 * **A width is not a constant to be tabled; it is a question about the target**
 * — `emit_c_spellings.hero` has said so in prose since the port, and then passed
 * the literal 64 at every call site, which is the one option that comment calls
 * out as forbidden ("derived, never tabled"). The bootstrap derived it from
 * `size_of::<c_ulong>()`; Heroes has no such thing, and an `extern constant`
 * cannot carry it either, because declaring `ULONG_MAX` at one width fails the
 * width assertion on the other platform. So it comes from here, which is the
 * one place that can ask C directly. Author decision 2026-08-26, `/decide`
 * answer `4a`.
 *
 * **What it is FOR is a diagnostic, and that is why the wrong answer is quiet.**
 * The compiler tells an author which Heroes type a C `long` is, and 64 is right
 * on Darwin and Linux and wrong on Windows, where `long` is 32 bits under LLP64.
 * Nothing crashes: the author is simply told `i64` where the header means
 * `i32`, on the one platform where this project has a CI leg and no local
 * machine. */
int64_t hero_word_bits(void);

/* THE FILESYSTEM, AND WHY IT IS NOT AN `extern` IN `selfhost/`.
 *
 * The compiler used to create a directory by writing `mkdir -p 'build/x'` for a
 * shell. Panel 097 vetoed the obvious repair — binding `mkdir` from
 * `sys/stat.h` and `_mkdir` from `direct.h` — because it cannot be written:
 * `direct.h` is `ffi_missing_header` off Windows, this language has no `#if`,
 * and `struct stat`'s `st_mode` is two bytes on Darwin against four on glibc,
 * so no `record` spelling passes both. The platform arm lives in
 * `parts/fs.c`; `selfhost/` sees these six names and no platform word.
 *
 * Each answers HERO_OS_OK or HERO_OS_FAILED, and each is the shell utility's
 * meaning rather than the C call's: an existing directory is a successful
 * `mkdir_all`, an absent file is a successful `remove`. The difference matters
 * because `mkdir`'s errno says EEXIST for a directory that is already there AND
 * for a file sitting where the directory should be, so the runtime asks
 * `hero_fs_is_directory` rather than trusting the code. */
#define HERO_FS_PATH_MAX 4096

int64_t hero_fs_exists(const char *path);
int64_t hero_fs_is_directory(const char *path);
int64_t hero_fs_mkdir_all(const char *path);
int64_t hero_fs_remove(const char *path);

/* 1 when `path` is newer than `reference`, 0 when it is not, -1 when either is
 * not there — a distinction `find -newer` could not make. */
int64_t hero_fs_newer_than(const char *path, const char *reference);

/* Replace `to` with `from`. Atomic on POSIX, and the cache depends on that: an
 * object is published by renaming it into place after its dependencies are
 * recorded, so a reader never sees it absent. The Windows arm asks
 * `MoveFileEx` for the same guarantee and `parts/fs.c` records that it is NOT
 * VERIFIED there. */
int64_t hero_fs_rename(const char *from, const char *to);

/* RUNNING A PROGRAM, BY ARGUMENT LIST RATHER THAN BY SENTENCE.
 *
 * Three calls because one is not writable: `argv: [str]` is `error[ffi_type]`
 * and a NUL-packed string is `error[unknown_escape]`, so there is no Heroes
 * value that can carry an argument list (panel 098). Push the words, run, reset.
 * `hero_run_words[0]` is argv[0] by convention, so the program's own name is
 * pushed first like every other word.
 *
 * An empty `out_path` or `err_path` INHERITS that stream, so the child writes
 * where this process writes — the right default for `heroes run`, whose whole
 * point is the program's own output. To throw a stream away, ask
 * `hero_run_discard_path()` for the local spelling and pass it as the path:
 * "discard" is a word the runtime knows and `selfhost/` does not.
 *
 * READ `*status` BEFORE THE RETURN VALUE. HERO_OS_OK means the program ran and
 * the return value is its exit code (or 128 + the signal that killed it);
 * HERO_OS_NOT_FOUND means it never started, which `system()` could not
 * distinguish from a program that legitimately exits 127. */
HeroStr hero_run_discard_path(void);

/* `.exe` on Windows, "" elsewhere. A binary built without it is created fine
 * and then cannot be started, because CreateProcess's search appends the
 * executable extensions rather than trying the bare name. */
HeroStr hero_run_exe_suffix(void);

/* THIS PROCESS'S OWN ID, as the operating system numbers it (`getpid` /
 * `GetCurrentProcessId`). The test harness puts it in the name of its scratch
 * directory so that two harnesses started together never share one
 * (M-robustness-guards step 5). The collision was found by doing it: two runs
 * in one `build/harness`, and a blessing wrote the other run's capture to disk
 * while both reported success. ABI 18 -> 19 for this declaration. */
int64_t hero_os_pid(void);
/* LISTING A DIRECTORY, AND REMOVING A TREE.
 *
 * The test harness reached these through `find … -print0` and `rm -rf`, which
 * are POSIX utilities and die on cmd.exe. The listing cannot be returned as a
 * `[str]` for the reason the argument list cannot be passed as one, so it is
 * built here and read back a name at a time.
 *
 * `want` is HERO_DIR_FILES or HERO_DIR_DIRECTORIES; `recursive` walks down.
 * Names come back RELATIVE to the directory scanned, joined with `/`, and
 * neither `.` nor `..` is ever among them. -1 means the directory could not be
 * read — a failure the shell's `2>/dev/null` used to hide. */
#define HERO_DIR_FILES 0
#define HERO_DIR_DIRECTORIES 1

int64_t hero_dir_scan(const char *root, int64_t want, int64_t recursive);
HeroStr hero_dir_at(int64_t index);

/* Release the listing once its names have been read. A listing nobody releases
 * is a leak the gate reports at exit. */
void hero_dir_release(void);
int64_t hero_dir_remove_tree(const char *path);

void hero_run_reset(void);
void hero_run_arg(HeroStr word);

/* Seconds after which the next `hero_run_go` kills its child and answers 124 —
 * coreutils' `timeout` code, on purpose, so a caller that used to read that
 * program's answer reads the same number. 0 is no limit and is the default.
 * `parts/run.c` carries the reason this is a runtime call and not a program. */
void hero_run_limit(int64_t seconds);
int64_t hero_run_go(const char *program, const char *in_path,
                    const char *out_path,
                    const char *err_path, int64_t *status);

/* -- threads (design.md Part 7.13; panels 111, 113 and 114) ------------------
 *
 * A FOURTH EDGE, and the header's opening line names three. It is here rather
 * than in a header a program writes because there is no header that spells
 * threads on all three platforms: `pthread.h` is absent under clang targeting
 * MSVC and C11's `<threads.h>` is absent from the macOS SDK, both measured
 * 2026-09-06, so the intersection is empty. `parts/spawn.c` carries the arms
 * and the reasoning.
 *
 * The handle is an `int64_t` index and never a `pthread_t`, which has three
 * spellings and no portable one. Joining twice, or joining a handle nobody was
 * given, are named panics here rather than undefined behaviour in C.
 *
 * WHAT MAY CROSS IS DECIDED BY THE CHECKER, not by this comment: a Heroes
 * program binds `hero_thread_spawn` through this file, so
 * `selfhost/check/ffi.hero` judges the callback's own parameters and result,
 * and a `[T]` or a `{K: V}` in that signature is `error[ffi_type]` on the
 * author's line. That is Part 7.13's isolation obtained from the type rule. */
int64_t hero_thread_spawn(int64_t (*body)(int64_t), int64_t arg);
int64_t hero_thread_join(int64_t handle);

/* How many may run at once, so a program can ask instead of meeting the panic.
 * A value rather than a form — `hero_word_bits`'s precedent. */
int64_t hero_thread_limit(void);

#endif /* HERO_OS_H */
