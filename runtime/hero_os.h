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
 * UTF-8 by definition (spec § 3 Types), so this is the one failure a caller cannot
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

/* The compiler's read of a source file it will tell about, never a program's
 * (panel 189, defect 227): bound in `selfhost/module/reading.hero`'s own group,
 * so no emitted program names it and the ABI stamp does not move (panel 089's
 * reading: adding a function is self-guarding, an old runtime is an undefined
 * symbol at link). `*status` is `hero_file_read`'s, and so is the result for
 * every status but one. For HERO_OS_NOT_TEXT the result is the file as a `str`
 * can hold it, preceded by its MARKS: each byte that is not part of a
 * well-formed UTF-8 sequence is one U+FFFD in the text, and the marks give two
 * characters for every U+FFFD of the text in order, that byte in upper-case
 * hexadecimal or `--` for a U+FFFD the file itself held. `*marks` is how many
 * bytes of the result are marks; the text is the rest. Zero for every other
 * status. `parts/os.c` says why each choice was made. */
HeroStr hero_file_read_shown(const char *path, int64_t *status, int64_t *marks);

/* One environment variable read as the shown read reads a file (defect 243):
 * HERO_OS_NOT_FOUND and "" where it is unset, HERO_OS_OK and its value where
 * that is UTF-8, HERO_OS_NOT_TEXT with marks and the shown value where it is
 * not, the marks counted in `*marks` as `hero_file_read_shown` counts them.
 * Bound in `selfhost/cli/process.hero`'s own group, like the read above. */
HeroStr hero_env_shown(const char *name, int64_t *status, int64_t *marks);

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

/* The same argument read as the shown read reads a file (defect 281):
 * HERO_OS_OK and the argument where it is UTF-8, HERO_OS_NOT_TEXT with marks
 * and the shown text where it is not, counted as `hero_file_read_shown`
 * counts them, so the compiler can name an argument of its own by its bytes.
 * Bound in `selfhost/cli/process.hero`'s own group, like the reads above; out
 * of range is `hero_args_at`'s panic. */
HeroStr hero_args_shown(int64_t index, int64_t *status, int64_t *marks);

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
 * `parts/fs.c`; `selfhost/` sees the names below and no platform word.
 * (This said *these six names* until 2026-09-30, when `hero_fs_why` made them
 * seven; a count in prose expires in silence, so the sentence carries none.)
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

/* The running executable's own path, read as the shown read reads a file:
 * HERO_OS_OK and the path, HERO_OS_NOT_TEXT with marks, or HERO_OS_NOT_FOUND
 * where this machine gives none (defect 277). `parts/fs.c` names each
 * platform's call. Bound in `selfhost/cli/toolchain.hero`'s own group. */
HeroStr hero_exe_path_shown(int64_t *status, int64_t *marks);
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

/* The operating system's OWN number for why the last `hero_fs_rename`,
 * `hero_fs_mkdir_all`, `hero_fs_remove` or `hero_file_write` on this thread
 * failed: `errno` where a C call failed, `GetLastError()` where a Win32 one did,
 * and 0 after one that succeeded. `hero_run_why`'s counterpart for the files a
 * build writes (defect 134): a publish that failed said *cannot publish* and
 * nothing else, where the number separates a full disk, a read-only directory
 * and a file another process holds open. `parts/os.c` holds it.
 *
 * **Below zero it is the runtime's own reason**, the negation of the number
 * the system gives for that reason: a path refused before any system call
 * (empty, or past HERO_FS_PATH_MAX), a link's chain past forty or its target
 * not UTF-8, a name the wide API cannot be handed. No system answers below
 * zero, so the sign alone says whose reason it is; until defect 346 these
 * were set as the system's own numbers, and told as the system's. */
int64_t hero_fs_why(void);

/* REPLACING A FILE WHOSE NAME THE AUTHOR GAVE, WHOLE OR NOT AT ALL (defect
 * 136): the source `fmt --in-place` and `check --apply --in-place` rewrite, and
 * an `-o`. `parts/replace.c` carries why a rename owes the file it replaces
 * everything besides its bytes, and what each call below answers; the policy
 * that asks them is `selfhost/cli/publish.hero`'s. Each sets `hero_fs_why`. */

/* The file a write to `path` lands in: `path`, or the file its chain of links
 * finally names, which need not exist. "" when the chain cannot be followed. */
HeroStr hero_fs_landing(const char *path);

/* What is at `path`, the link itself where it is one. */
#define HERO_FS_UNKNOWN -1
#define HERO_FS_ABSENT 0
#define HERO_FS_FILE 1
#define HERO_FS_DIRECTORY 2
#define HERO_FS_LINK 3
#define HERO_FS_OTHER 4
int64_t hero_fs_kind(const char *path);

/* How many names the file at `path` has; -1 when it cannot be asked. */
int64_t hero_fs_links(const char *path);

/* 1 when `path` could be opened for writing, as `fopen` would ask, else 0. */
int64_t hero_fs_writable(const char *path);

/* `text` into a NEW file at `staged`, flushed, and given the owner, group,
 * permission bits, flags, ACL and extended attributes of `like` unless `like`
 * is "". HERO_STAGE_DONE, or the step that failed; a failed stage leaves no
 * file. */
#define HERO_STAGE_DONE 0
#define HERO_STAGE_CREATE 1
#define HERO_STAGE_WRITE 2
#define HERO_STAGE_OWNER 3
#define HERO_STAGE_MODE 4
#define HERO_STAGE_ATTRIBUTES 5
#define HERO_STAGE_FLUSH 6
#define HERO_STAGE_FLAGS 7
int64_t hero_file_stage(const char *staged, HeroStr text, const char *like);

/* `staged` put in place of `path`, and the directory holding them flushed. */
int64_t hero_fs_replace(const char *staged, const char *path);

/* A staged file that could not be put in place, removed even where it carries
 * an ACL that denies deleting it. HERO_OS_OK or HERO_OS_FAILED. */
int64_t hero_file_unstage(const char *staged);

/* What the cases build their files with: the permission bits (Windows keeps
 * read-only alone, 0444 or 0666), `chmod`, a second name, a symbolic link, an
 * ACL entry denying this user the file's deletion, and the one flag each
 * platform carries that a case can set (nodump; hidden on Windows). A shape a
 * platform or a filesystem does not have answers HERO_OS_UNSUPPORTED. */
#define HERO_OS_UNSUPPORTED 4
int64_t hero_fs_mode(const char *path);
int64_t hero_fs_set_mode(const char *path, int64_t mode);
int64_t hero_fs_link(const char *existing, const char *name);
int64_t hero_fs_symlink(const char *target, const char *name);
int64_t hero_fs_deny_delete(const char *path);
int64_t hero_fs_flagged(const char *path);
int64_t hero_fs_set_flag(const char *path);

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

/* The name at that index read as `hero_file_read_shown` reads a file: a name
 * that is not UTF-8 is HERO_OS_NOT_TEXT with marks, never a panic (defect
 * 239). Bound in `selfhost/cli/process.hero`'s own group. */
HeroStr hero_dir_at_shown(int64_t index, int64_t *status, int64_t *marks);

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

/* A ceiling of `bytes` on any file the NEXT child writes, so that a write past
 * it fails as a full disk fails (defect 136's case in the net). 1 where the
 * next child will be held to it, 0 where the platform has no such limit
 * (Windows); 0 bytes clears it. `parts/run.c` carries why. */
int64_t hero_run_limit_writes(int64_t bytes);
int64_t hero_run_go(const char *program, const char *in_path,
                    const char *out_path,
                    const char *err_path, int64_t *status);

/* The operating system's OWN number for why the last `hero_run_go` could not
 * start a child: `GetLastError()` on Windows, `errno` on POSIX. It is 0 after a
 * call that started one, so it is read only where `*status` is not HERO_OS_OK.
 *
 * It exists because the number was being thrown away at the one place it was
 * the whole answer. On 2026-09-21 the Windows CI leg reported 101 of 136 `run`
 * cases as `did not build (exit -1)` with an empty stderr; -1 is the harness's
 * sentinel for a child that never started, and the runtime had held
 * `GetLastError()` in its hand at that exact point and returned without asking
 * it. A sharing violation, an out-of-memory and a missing file are three
 * different repairs and they were one silence. */
int64_t hero_run_why(void);

/* The signal that ended the last child, or on Windows the exception code a
 * crashed child exited with; 0 where the child ended itself (defect 170).
 * `parts/run.c` carries why. */
int64_t hero_run_signal(void);

/* A number this process has not answered before, so a caller can name a file
 * that no earlier call can still be holding open. `parts/run.c` carries why it
 * is here rather than in the language (panel 174, route B). */
int64_t hero_run_serial(void);

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

/* The same, on a stack of at least `bytes` that the caller chooses (panel 184
 * R5): the compiler runs every command on one. A refusal is answered rather
 * than panicked, -1 with the operating system's own number in `*why`, so the
 * caller can say what it needed the thread for. `parts/spawn.c` says why. */
int64_t hero_thread_spawn_sized(int64_t (*body)(int64_t), int64_t arg, int64_t bytes,
                                int64_t *why);
int64_t hero_thread_join(int64_t handle);

/* How many may run at once, so a program can ask instead of meeting the panic.
 * A value rather than a form — `hero_word_bits`'s precedent. */
int64_t hero_thread_limit(void);

#endif /* HERO_OS_H */
