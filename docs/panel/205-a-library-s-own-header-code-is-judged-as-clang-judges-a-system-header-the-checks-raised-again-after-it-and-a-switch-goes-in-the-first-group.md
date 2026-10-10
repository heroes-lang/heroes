# Panel 205: a library's own header code is judged as clang judges a system header, the checks raised again after it, and a switch goes in the first group

Convened 2026-10-10 by the coordinator on the author's yes through the
question widget between 02:36 and 02:37, *yes, at once, 5.43 USD* for the
blind seat, under the author's goal of about 02:30, *at most five open, all
improvement*; for defects 568 and 569, which panel 204's R5 filed for a
sitting of their own, and 570, which this sitting's critic found. **A full
panel** (what a program can bind, what `build` refuses, a sentence of § 13):
the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian,
the blind seat in eight fresh `claude -p` sessions outside the repository
(1.7353 USD of the 5.43), and the completeness critic before the seats and
after them. The tree frozen at **`46c975c4`**, worktree `lane-panel-205`.
Briefs written from 02:37; the critic's first pass from 02:38:37, read at
02:48, its repairs applied before any seat started (eight `-Werror` flags,
not nine; the latching headers are libc's `<stdint.h>` and `<math.h>`; a
package's `-D` already reaches every header; 569 depends on where a library
is installed; a header's pragma turns a check off, filed as 570); the
account's session limit stopped three lanes at about 02:50, the author
logging in a new account by 02:51; seats from 02:51; the historian's reply
copied at 03:05, the ffi-pragmatist's at 03:12, the spec-warden's at 03:13;
the blind seat's sessions from 03:14:46 to 03:15:35; the compiler-engineer's
reply copied at 03:24; the critic's second pass from 03:24:29 to about 03:37,
copied at 03:36; this synthesis from 03:37, every time read from `date`.
Briefs in `205-briefs/`, reports in `205-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **approve** route H for Q2 and 570 (every warning ignored in the groups' region but `macro-redefined`, every check warning raised again after it and in the probe region); **object** on Q1 to any reordering or group form, a package's `-D` the standing route; no veto | H built in its copy: 570 and six shapes beside it refused on two platforms, GMP and libavutil building, the fixpoint by `cmp`, the compiler's own tests 1,542, 7 of 1,402 tracked files moved, all expected; a cold self-build 446.96 G against 446.89 G instructions |
| ffi-pragmatist | **approve** route C (veto any relaxation without it), P2 for Q2, route B for Q1; **object** to `-isystem` and a group form; `-Wsystem-headers` refused on measurement | P2 and C as runtime copies through `HEROES_RUNTIME`, a 182-header Mac census and 85 Linux headers copied into a user directory, 18 glibc names hidden behind a switch, pragmas and `.pc` files in the wild |
| spec-warden | **object, provisional**: approve the re-raise at 0 tokens; object to every Q2 sentence (W1 a veto if proposed, the compiler contradicting it); H2f (+9/+9) only with a built first-group route; **object** to H's silence over a header of the program's own | `heroes measure` on fourteen drafts, 12 shapes of 570, H's compiler on the program's own `own.hero`, the switch's place in plain C and on the emitted unit, Mac and Linux |
| historian (advisory) | **approve** a macro reaching the unit's command line or a prefix with no libc header (Q1), judging a header by where it was found with the checks raised again (Q2); **object** to `-Wsystem-headers`, to a pragma route without the re-raise, and to keeping panel 198's premise as written | gnulib's `config.h` first, Autoconf, CPython, cgo and go#35315, Zig's `defineCMacro`, bindgen, Nim's `localPassC`, D's ImportC, CMake, Bazel, clang modules; GCC's and clang's system-header rules, MSVC `/external`, CMake `SYSTEM`, Meson; Qt 2014, Gentoo, GCC 14; GCC's and clang's pragma rules, LLVM bug 20022 |
| blind seat | without H2f, 1 of 4 readers put `_GNU_SOURCE` in a header of its own named by the first group, 3 bind `sched.h` plainly naming the macro as their doubt; with H2f, 4 of 4; 8 of 8 name the macro, 0 of 8 a `package` | `llm-ergonomist-scoring.md` |
| critic, second pass | H accepts undefined behaviour in a header of the program's own (a missing return, an uninitialised read); P2 misses `mac` (a `_Pragma` in a constant's macro, expanded after the close); S1 (only `sign-conversion` ignored) and S3 (`-Wall` and `sign-conversion`) let the six libraries through and refuse the program's own header's undefined behaviour; route B's two units are hand-written and neither is the emitted one, and a variant Y keeps defect 361's guard and lets a first group set a switch, on the real units; route A serves none of the eight readers and lets another module's package decide a module's verdict | runtime copies S1, S2, S3 and Y, H's and the frozen compiler's binaries copied, the critic's and the seats' cases, Mac and Linux arm64 |

## What the sitting measured

- **570, wider than filed.** Through a header's own pragma, twelve wrong
  bindings build today on this Mac and Linux: a sign (`abs(x: u32)`), a
  handle of another tag (defect 029's class), an integer for a pointer
  (`read_p(p: i64)` against `int *`, which aborts 134 at run time), a
  callback, an undeclared function (`getpid(x: f64)`, no warning at all),
  under `ignored`, `GCC diagnostic ignored`, a downgrade to `warning`,
  `-Wconversion`, `-Weverything` and a push with no pop; plus `mac` (a
  `_Pragma` in a constant's macro) and `callee` (a macro naming the bound
  function that expands to a `_Pragma`). A balanced push and pop, or `#pragma
  clang system_header`, leave the checks standing (the spec-warden). GCC's
  and clang's documentation: a pragma outranks the command line (the
  historian). The emitter already raises four warnings again around its
  probes (`selfhost/emit/extern_probe.hero:136`).
- **569, by where a library is installed.** GMP and libavutil, avcodec,
  avformat, swscale and libfdt are refused on this Mac by `-Wsign-conversion`
  inside their own code, through pkg-config's `-I`; on Linux arm64 the same
  `g.hero` prints `6`, `/usr/include` being a system directory clang keeps
  system whatever `-I` says, by design (GCC's and clang's documentation, the
  historian); `CPATH` refuses and `C_INCLUDE_PATH` builds on this Mac. So
  panel 198's *as `-I`* does not reach them, and the note at
  `selfhost/cli/header_refused.hero:290` and the premise at
  `selfhost/cli/package_words.hero:36-44` (*no warning is turned off for a
  header*) are false for every system directory. Every binding check fires
  on the compiler's own lines, not inside the header, and a system directory
  changes none of their answers (the compiler-engineer).
- **The relaxations, measured on the same cases** (the critic): H (the whole
  region silent but `macro-redefined`) lets a header of the program's own
  with a missing return or an uninitialised read build and print garbage;
  P2 (`-Wall`, `sign-conversion`, `shorten-64-to-32` and the two pointer
  warnings ignored in the region) lets the truncation through and misses
  `mac`; S1 (`sign-conversion` alone) and S3 (`-Wall` and `sign-conversion`)
  let the six libraries through, refuse the program's own header's undefined
  behaviour and truncation, and agree with P2 header by header on the
  182-header Mac census (10 fail, every arm) and on the 85 Linux headers; S1
  leaves clang's raw `-Wall` text on two correct programs (`jq.h`, `jv.h`),
  S3 none. Only H refuses `mac`, by raising the checks again in the probe
  region as well as after the close.
- **568: where a switch must stand.** A macro libc reads once at its first
  header (`_GNU_SOURCE` at glibc's `features.h`, `_POSIX_C_SOURCE` at
  Darwin's `sys/cdefs.h`) works only before every libc header; the
  compiler's prefix reads `<stdint.h>` (`heroes_runtime.h:56`) and `<math.h>`
  (`SEEDS`) first, so `sched_getcpu` is refused `ffi_unknown_name` through a
  header defining `_GNU_SOURCE`, *`gnu.h` declares no `sched_getcpu`*, a
  false message, and `_POSIX_C_SOURCE` does nothing on this Mac. Moving the
  prefix as the ffi-pragmatist wrote it (route B, a hand-written unit with no
  guard) reopens defect 361's guard on the real unit (`INT64_C`, `HUGE_VAL`
  undeclared after its pop; the compiler-engineer); the critic's **Y** keeps
  the guard: `heroes_runtime.h` spells its integer types from clang's
  builtins and includes no libc header, its `UINT64_C` and `INT64_MAX`
  become the builtins', `<math.h>` leaves the prefix, and after the close
  `<stdint.h>`, `<math.h>` and fallbacks for `INT64_C` and `UINT64_C`. On the
  real units it hides `strlcpy` under the switch on this Mac, declares
  `sched_getcpu` on Linux, holds a `_Static_assert(INT64_C(42) == 42)`
  against a hostile group, and `runtime.c` compiles under it with 0
  warnings; every guarded standard name the emitted C uses owes a fallback
  (`INT64_C` 21,961 times in the seed, `UINT64_C` 4,504, `INT64_MIN` 133),
  and `FP_*` differs between glibc and Darwin, a question.
- **Route A, a package's `-D`**, works today and its cache key names it
  (defect 161's test), but it reaches every unit of the program
  (`selfhost/cli/produce.hero:142`): a module binding `wcwidth` from
  `wchar.h` is told *`wchar.h` declares no `wcwidth`*, false, unless `main`
  names `ncursesw` (the ffi-pragmatist, the critic on Linux). None of the
  blind seat's eight readers wrote a `package` or a `.pc`.
- **The sentence.** H2f, *... one that needs another's names comes after it,
  and one defining `_GNU_SOURCE` first.*, +9 legacy and +9 cl100k over panels
  203 and 204's landed sentences; with it 4 of 4 readers put the switch in a
  header of their own named by the first group, without it 1 of 4. No Q2
  sentence is true: W1 is false on this Mac by an environment variable, W2
  under H.
- **Found beside** (the critic): `<ucontext.h>`'s `deprecated` attribute
  fires clang's `-Wdeprecated-declarations` at the compiler's own probe line
  on a correct program at exit 0, under every arm, a cause of its own,
  unfiled (`grep` of `issues/`).

## Disagreements, stated plainly

- **How much of a header's code goes unjudged.** H silences everything; the
  spec-warden objects that a header of the program's own then runs a missing
  return; the critic measured S1 and S3 meeting every criterion H and P2 meet
  on the six libraries and both censuses while refusing that header. The
  resolution takes S3's region with H's two raise points, the combination
  nobody built, unbuilt here and built before it lands.
- **Route A or Y.** The compiler-engineer recommends the package's `-D` and
  objects to reordering on its measurement of route B; the critic shows that
  measurement concerned route B as written, not Y, which keeps the guard;
  route A reaches every module and serves no reader measured. The resolution
  takes Y with H2f, built and measured before it lands, and scopes route A's
  `-D` to the modules whose groups name the package.
- **Panel 198.** Its word list stands; its *as `-I`* reason was a premise
  about warnings in headers that clang's own design falsifies for system
  directories. The resolution keeps `-I` for the program's own directories
  and judges a header's own code by S3 wherever it was found, so the verdict
  stops depending on the install route, and corrects the two false texts.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, defect 570 and its rows: every warning an FFI check rests on is
   raised again after the groups' close and again in the probe region**
   (H's two raise points, `selfhost/emit/macro_guard.hero` and
   `extern_probe.hero`): the command line's eight `-Werror=` words read from
   `flags.flags()`'s one list, and the three clang makes errors by default
   (`int-conversion`, `implicit-function-declaration`,
   `incompatible-function-pointer-types`). It refuses the twelve shapes and
   `mac` with the messages a plain header gets. `callee`'s macro is asked of
   clang (is the bound name an object-like macro whose expansion holds a
   `_Pragma`) and refused with a true message; H's `tree2` chain, which
   degrades every `ffi_parameter_type` message, is not taken. 570 widens to
   its fourteen rows.
2. **R2, defect 569: a header's own code is judged by S3 wherever it was
   found.** Inside the groups' region the guard pushes the diagnostic state
   and ignores `-Wall` and `-Wsign-conversion` alone; `return-type`,
   `uninitialized`, `conditional-uninitialized`, `shorten-64-to-32`, the
   pointer-type errors and panel 204's `macro-redefined` stay errors there;
   the guard pops at the close and R1 raises the checks again. The six
   libraries build on this Mac as on Linux, `CPATH` and `C_INCLUDE_PATH`
   agree, the program's own header's undefined behaviour stays refused. The
   note at `header_refused.hero:290` and the premise at
   `package_words.hero:36-44` are rewritten true; panel 198's word list and
   *as `-I`* stand; `tests/golden/unsupported/fixedbugs-360-…` is rewritten by
   hand (it pins the policy this reverses). Measured at the landing on the
   182 and 85 headers and the 1,402 tracked files.
3. **R3, defect 568: a module's first group can set a switch.** Y lands:
   `heroes_runtime.h` takes its integer types and limits from clang's
   builtins and includes no libc header, `<math.h>` leaves the prefix,
   `<stdint.h>` and `<math.h>` come after the groups' close with fallbacks
   for every guarded standard name the emitted C uses; on this Mac, Linux
   arm64 and Windows' clang (the CI's legs, the box where it fails), the
   runtime's tests, `run` whole and the fixpoint. A name a header declares
   only under a feature-test macro is told so (*`sched.h` declares
   `sched_getcpu` only under `_GNU_SOURCE`: define it in a header of your own
   that this module's first group names*), never *declares no*. A package's
   `-D` reaches only the units of modules whose groups name that package.
   Spec § 13 gains H2f, +9/+9 on the vendored tables, the real count by one
   `--refresh` at the landing on the author's yes; design.md §4.19 the
   spec-warden's switch sentence. Panel 076's refusal of a compiler-wide
   `-D_GNU_SOURCE` stands. If Y does not build clean, H2f and the §4.19
   sentence do not land and 568 stays open.
4. **R4, refused**: `-isystem` for a package's directories (it blinds panel
   204's R2 and still depends on the install route); `-Wsystem-headers` (17
   Mac headers fail and 55 of 165 print raw text; with R2 every Linux unit,
   an inference); H's silent region and P2's extra ignores (undefined
   behaviour and a truncation in the program's own header pass); a group
   form naming a macro (H3: a new form, unable to carry `200112L`); route B as
   written (it reopens 361's guard); route A as the answer (no reader writes
   it, and its reach is program-wide); W1, W2 and every other Q2 sentence.
5. **R5, filed now** as defects 571 and 572: the `deprecated` attribute firing clang's raw warning
   at the compiler's own probe line on a correct program (`blocking`), and a
   package's `-D` reaching every module's unit with a false *declares no* in
   another module (`blocking`, landing with R3).

**The conservative alternative, the author's to choose instead**: R1 alone
(the checks raised again), 569 and 568 left open with their false texts
corrected; or the compiler-engineer's H whole for Q2 (the library headers'
warnings all silenced) with route A for Q1, which builds today.

**Corrected 2026-10-10 by the landing of defect 572** (`b87d2bc2`, lane b18-guard): R3's *a package's `-D` reaches only the units of
modules whose groups name that package* is narrower than what landed. Asked first by the module's own groups, the repair broke
`run/fixedbugs-413-libuv-*`: the library module's unit reads `uv.h` for the record it spells and lost libuv's `-I`. So a package's words
reach the units that read a header its groups name, and no other; where two modules read one header, the package one group names reaches
both. On Linux arm64 the seats' four `wcwidth` cases are told *`wchar.h` declares `wcwidth` only under `_XOPEN_SOURCE`*, with or without
`main` naming `ncursesw` (the lane's measurement, not re-run by the coordinator). Taken by the coordinator as the robust reading of R3, a
unit that reads a header needing that header's package; the author may overturn it.

## Process notes

- **The compiler-engineer ran `pkill -f "census.sh"` and `pkill -f "xargs -P
  2"` at about 03:00**, patterns that can match another session's processes,
  and said so; lane b18-infer, whose `census.sh` ran that night, was told at
  03:24 to check any census spanning 03:00 and re-run it if cut.
- **The account's session limit at about 02:50** stopped three lanes; all
  were resumed at 02:51 on the author's new account.
- The critic's second pass found the ffi-pragmatist's *P2 plus C keeps every
  wrong binding refused* false on `mac`, and both seats' route-B units
  hand-written; the blind seat's task named neither the macro nor a header of
  the program's own.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at H's landing gate the census of the 1,402 tracked files moves exactly its 7 programs (re-read for S3's region and Y); `selfhost/` grows by at most 70 lines over at most 3 files for R1 and R2 | the landing |
| ffi-pragmatist | with P2 and C landed, `p205/gmpmac/g.hero` prints `6`, `gsign.hero` and `hole570/leak.hero` are refused, the 182-header census moves exactly 6 headers to OK and 0 to FAIL, the 20 `examples/` programs with an `extern` build with 0 warnings (scored against S3) | the landing |
| ffi-pragmatist | with route B (here Y) landed, on Linux arm64 a module whose first group is an own header defining `_GNU_SOURCE` binds `sched_getcpu() -> i32` and prints `true`; `sched_getcpu(x: i32) -> i64` refused | the landing, the CI's Linux legs |
| spec-warden | P1: H2f reads +10 to +14 real at the landing's `--refresh` | the landing |

**Scored 2026-10-10, at the landing**, from lane b18-guard's commit bodies, which the coordinator read and did not re-run:

- the spec-warden's P1: H2f read +17 real at the one `--refresh`, 9,887 to 9,904 at 13:46 (`83ada2b8`), **a miss by 3**;
- the compiler-engineer's census: 19 exit codes moved over the 1,456 tracked files with an `extern`, every one a case of the lane (18 of
  570, 1 of 569; `ee4dda85`), against exactly 7, **a miss**; its size: R1's and R2's commits (`bd1136f9`, `ee4dda85`) add 673 lines and
  remove 33 over 10 files of `selfhost/`, git's count with tests and comments, the coordinator's `git show --numstat`, against at most 70
  over 3, **a miss**;
- the ffi-pragmatist's first, scored against S3, the region R2 took: gmp's `g.hero` prints `6` and `gsign` is refused, **a hit**; the 182
  Mac headers read 16 refused and 2 with raw warnings before and 10 and 0 after (`ee4dda85`), net counts that do not say which headers
  moved where, so *exactly 6 to OK and 0 to FAIL* is **unscored**; `hole570/leak.hero` and the 20 `examples/` programs are named in no
  commit body read, **unscored**;
- the ffi-pragmatist's second, `sched_getcpu` on Linux arm64: at the CI's Linux legs after the push, **unscored**.

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R5 as written above, the author answering
through the question widget between 03:36 and 03:38 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*Solo R1*, *Route H + pkg-config* and *I want to read it first*, on the
coordinator's summary of each route; in the same widget *yes, one* to the
`--refresh` R3's sentence needs at the landing. Recorded as a reading
(CLAUDE.md § 4). The author may overturn it.
