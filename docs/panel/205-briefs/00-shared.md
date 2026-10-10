# Panel 205, the shared brief: what surrounds a group's header in a module's unit (568, 569)

Written by the coordinator on 2026-10-10 from 02:37 (`date`), on the tree
frozen at `46c975c4` (worktree `lane-panel-205`: panel 204 ratified, defects
558 to 569 filed). Convened on the author's yes through the question widget
between 02:36 and 02:37, *yes, at once, 5.43 USD* for the blind seat (what is
left of the 10 approved for panel 204, 4.5739 spent), under the author's goal
of about 02:30, *at most five open, all improvement*. Every fact names its
command or file; a number marked **carried** is a report's, a question for the
seat.

**Lane: full**: what a program can bind, what `build` refuses and what the
messages say, and possibly a sentence of spec § 13. The compiler-engineer,
the ffi-pragmatist, the spec-warden, the historian, the blind seat run by the
coordinator as fresh `claude -p` sessions outside the repository (5.43 USD),
the critic before the seats and after them.

## Q1, defect 568: a macro that must precede every header

Every unit a module compiles opens with the compiler's own headers before any
group's: `heroes_runtime.h` (which includes `stdbool.h`, `stddef.h`,
`stdint.h`, `runtime/heroes_runtime.h:54-56`), then `SEEDS`, `math.h` and
`hero_os.h` (`selfhost/emit/externs.hero:79-80`), then the guard. A
feature-test macro must be defined before any header (POSIX 2.2.1.1; glibc's
feature_test_macros(7); both carried from panel 204's historian). Run by the
coordinator at 02:37 with the trunk's compiler: `sw.hero` names `cfg.h`
(`#define _POSIX_C_SOURCE 200112L`) first and binds `strlcpy` from
`string.h` second; it builds at exit 0, the switch having no effect (in plain
C that header before `<string.h>` hides `strlcpy`, panel 204's critic,
carried). **Carried** from panel 204's ffi-pragmatist (Linux arm64, its
`p/gnu/`, copied read only under `.claude/worktrees/scratch-b15/p205/gnu/`):
glibc's `sched_getcpu` is refused `ffi_unknown_name` bound plainly, through a
header of the program's own defining `_GNU_SOURCE`, and with that group
first; only a hand-written prototype builds, and a wrong one (`long
sched_getcpu(int)`) builds and runs. Panel 076's ffi-pragmatist refused
`-D_GNU_SOURCE` in the build flags on a measurement (it re-types
`strerror_r`; carried, grep `docs/panel/076-*`). **How should a program set
a macro that must come before every header, and what should the unit look
like so it can?**

## Q2, defect 569: a warning inside a package's own header code

The compiler compiles every unit with `-Wall` and nine `-Werror=` flags
(`selfhost/cli/flags.hero:114-127`, `sign-conversion` and
`shorten-64-to-32` among them). Panel 198 (ratified 2026-10-09,
`docs/panel/198-a-package-s-answer-is-judged-word-by-word-and-value-by-value.md:79-86`)
hands a package's `-isystem <dir>` on as `-I<dir>`, *so no warning is turned
off for a header (`ffi_header_refused`'s promise, panel 188's premise)*; the
note `selfhost/cli/header_refused.hero:290` tells it. Run by the coordinator at
02:37: `p205/gmpmac/g.hero` (`extern "gmp.h" package "gmp"`, a constant) is
refused `ffi_header_refused`, *clang refuses it at line 1882: operand of ?
changes signedness: 'const int' to 'size_t' ... [-Werror,-Wsign-conversion]*.
**Carried** from panel 204's ffi-pragmatist: FFmpeg's `libavutil` likewise
(`p205/gmpmac/av.hero`); a header of the program's own with a diagnostic
pragma around the include builds. Panel 204 R2 (ratified at 02:33) adds
`-Werror=macro-redefined`, which refuses Expect's `EXP_ABORT` with libjpeg's
`JPEG_LIB_VERSION` in both orders (`EXTERN` defined twice, the critic's
measurement), the same policy. **Should a warning inside a package's or a
system header's own code refuse a program, which warnings, and what does
that do to panel 198's ruling and panel 204's R2?**

Routes to weigh, a list to widen: for Q1, the prefix after the groups' headers;
a header of the program's own placed before the prefix; a group form naming a
macro; a `-D` per platform; splitting the prefix. For Q2, `-isystem` for a
package's directories (reversing panel 198's *as `-I`*); a diagnostic pragma
around each group's `#include` in the emitted unit, the compiler's own code
keeping every `-Werror`; `-Werror` scoped to the compiler's own emitted lines;
and what panel 204's R2 keeps between two groups' headers.

## The rules every seat works under

As panel 200's shared brief states them (`docs/panel/200-briefs/00-shared.md`,
§ The rules every seat works under), with your folder
`.claude/worktrees/scratch-b15/205-<seat>/`, your copy rsynced from
`.claude/worktrees/lane-panel-205/` excluding `.claude/worktrees`, **and these**:
never run a git command inside your copy (its `.git` file points at a real
worktree: remove it right after the copy); the seed is ABI 30 and so is the
runtime, so your compiler is `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes` in your copy, then `./heroes build selfhost/main.hero -o heroes`
after an edit; three lanes work beside you (`lane-b18-close`, `lane-b18-ffi`,
`lane-b18-infer`): never touch them; at most three processes of yours at a
time, one container at a time; Docker `heroes-linux-arm64:latest` is free.
No paid run (the blind seat is the coordinator's). No file outside the
repository's root, `/tmp` included. **Time box**: report within 50 minutes of
starting, the unmeasured said plainly; keep notes in your folder as you go.

## Corrections and additions from the critic's first pass, binding

Read at 02:48 (`date`): `docs/panel/205-reports/completeness-critic-pass1.md`,
every one from a command it ran, the coordinator re-running its item 11 at
02:49; applied before any seat starts and binding over the text above where
they disagree. Read it whole; its routes A to F and its questions are part of
your brief. In short:
- **Eight** `-Werror=` flags (`flags.hero:115-118`, `:121-124`), not nine;
  the emitted unit raises four more by pragma around its probes
  (`selfhost/emit/extern_probe.hero:136`); panel 204's R2 flag is ratified,
  not landed.
- **Q1**: the prefix headers that latch a switch are libc's `<stdint.h>`
  (`heroes_runtime.h:56`) and `<math.h>`, not clang's `stddef.h` and
  `stdbool.h`; the compiler already sets feature macros before every header
  (`-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS`, `flags.hero:112-113`;
  `_GNU_SOURCE` for the runtime's own unit, `runtime/runtime.c:92-94`); and a
  route exists today, a `.pc` of the program's own answering `-D_GNU_SOURCE`
  through `package` (Linux: builds, a wrong prototype still refused), so
  defect 568's *only a hand-written prototype builds* is false. A macro a
  header tests itself (`_XOPEN_SOURCE` for `<ucontext.h>`) works today
  through a program's own header; `_FILE_OFFSET_BITS=64` moves nothing on
  Linux arm64; a group with a header and no member is refused.
- **Q2**: GMP and libavutil come through `-I` on this Mac, not `-isystem`,
  so panel 198's *as `-I`* is not what refuses them; on Linux arm64 the same
  `g.hero` builds and prints `6`, `gmp.h` sitting in a system directory
  clang silences whatever `-I` says; so **a verdict depends on where a
  library is installed** (Homebrew against apt), and `C_INCLUDE_PATH`
  against `CPATH` flips it on this Mac; the note at
  `header_refused.hero:290` and panel 188's premise
  (`package_words.hero:40-42`) are false for any header in a system
  directory; panel 204's R2 is blind there too (its Linux *0 newly refused*
  may be clang's silence, a question). A seat measuring `-I` against
  `-isystem` copies the headers into a user directory first.
- **Defect 570, filed at 02:50 and in this sitting's scope**: a header's own
  `#pragma clang diagnostic ignored "-Wsign-conversion"` turns the sign check
  off for the rest of the unit (`abs(x: u32)` against `int abs(int)` builds
  and prints `5`; through a plain header it is refused
  `ffi_parameter_type`); any route that relaxes a header's warnings must
  raise again, after the groups' headers, every warning an FFI check rests on
  (route C).
- Three rows of *which warnings* today: the eight `-Werror` refuse; a plain
  `-Wall` warning in a header prints clang's raw text at exit 0; a header
  attribute (`deprecated`) fires on the compiler's own emitted line. 1,401
  files on disk carry an `extern "` group head, 79 naming a `package`
  (the critic's grep, not `git ls-files`).
