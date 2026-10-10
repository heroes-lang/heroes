# Panel 204: C reads a module's headers in the order its groups are written, and the spec says so

Convened 2026-10-10 by the coordinator on the author's choice through the
question widget, *zero defects*, which named this sitting, and their answer
to its blind seat's budget, *10 dollars*, both between 01:06 and 01:14 by the
clocks read before and after; for defect 563, which panel 202's R4 sent to a
sitting of its own. **A full panel** (what § 4 says of declaration order,
which programs `build` accepts and what they mean): the compiler-engineer,
the ffi-pragmatist, the spec-warden, the historian, the blind seat in
eighteen fresh `claude -p` sessions outside the repository (4.5739 USD of the
10), and the completeness critic before the seats and after them. The tree
frozen at **`635e8f67`**, worktree `lane-panel-204`. Briefs written from
01:16; the critic's first pass from 01:17, stopped with the coordinator's
process at about 01:2x and resumed at 01:28, read at 01:29, its repairs
applied before any seat started (the include order is not the written order
today; every unit opens with the compiler's own prefix; panel 091's owed
sentence was filed, an open task; panel 202's R4 condition unmet); seats from
01:30; the blind seat's first round from 01:31:51 to 01:33:02 and its second
from 01:51:21 to 01:52:34; the historian's reply copied at 01:46, the
spec-warden's at 01:50, the ffi-pragmatist's at 02:04, the
compiler-engineer's at 02:14; the critic's second pass from 02:15 to 02:28,
copied at 02:29; this synthesis from 02:30, every time read from `date`.
Briefs in `204-briefs/`, reports in `204-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **approve** the route it built: the written order made true, `-Werror=macro-redefined`, an order search on a refused header; **veto** per-group units; **object** to a byte sort and to a comparison over orders | `heroes-p3` in its copy: +149 lines over four files, a warm self-build 400.19 G against 400.24 G instructions, 0 of the 107 two-header files moved, 309 `--emit-c` outputs byte-identical, the compiler's own tests 1,544 |
| ffi-pragmatist | **veto** a byte sort and per-group units; **object** to a comparison that refuses; **approve** the written order stated, made true and searched by the messages | real libraries on this Mac and Linux arm64 (readline, raylib, GLFW and OpenGL, GMP, libjpeg, pcre2, glibc); a pair census on the real unit, 4,643 Mac pairs and 3,570 Linux; Zig, cgo, Nim |
| spec-warden | **approve, provisional**, F7 on the route with a detector; **object** to G1m and eight other drafts; **veto** G3; F5 the conservative draft | `heroes measure` on eighteen drafts, the cases on today's compiler, a prototype of the emitter's walk on the critic's 20 files |
| historian (advisory) | **object** to a byte sort, to per-group units, and to leaving today's reordering unstated; precedent for the written order, a sentence naming the prefix, and messages that name an order | C11 7.1.2p4, POSIX and glibc's feature-test rule, libjpeg's own documentation, clang-format's sorting (cfe-dev 2016, Chromium 2017, LLVM 41740), GCC's `gcc-order-headers`, cgo, bindgen, Zig, Cython, Vala, GHC, D's ImportC, Clang modules |
| blind seat | today's spec: 2 of 2 read per-group isolation and mispredict a value and a verdict; F7: 2 of 2 the same; F7 and G1m: 2 of 2 predict all four programs as the compiler builds them; writing: 8 of 8 put `stdio.h` first in both arms (the task named it first) | `llm-ergonomist-scoring.md` |
| critic, second pass | `cfgone/main2` violates C11 6.10.3p2's constraint, so it is not correct C; `dual` is legal C, both orders silent, different values; a detector by text or macros refuses a correct GLFW program; the flag refuses Expect with libjpeg in both orders, 0 newly refused of 7,140 Linux pairs; p3's note keeps the false advice above its own and never reaches `ffi_unknown_name` | the two compilers copied and run on every seat's case, C-level comparisons on the real unit, Linux arm64 |

## What the sitting measured

- **The order today.** `selfhost/emit/externs.hero` `headers()` (`:101-133`)
  walks the bound functions and constants first and the record-only groups
  after, so `extern "stdio.h"` holding only `record CFile tag FILE`, written
  above `jpeglib.h`, is refused *unknown type name 'FILE'* (the critic);
  every unit opens with the compiler's own prefix (`heroes_runtime.h`,
  `<math.h>`, `<hero_os.h>`, the guard), so a header that fails alone can
  build and a feature-test switch can never come first; a repeated header is
  emitted once.
- **Order is how C configures a library.** libjpeg's own documentation:
  *before including jpeglib.h, include system headers that define at least
  the typedefs FILE and size_t*; GNU readline needs `stdio.h` first; GMP
  declares its `FILE *` functions only after it (Linux); GLFW chooses its GL
  header by what came before (the ffi-pragmatist, the historian). On the real
  unit 45 of 4,643 Mac pairs and 6 of 3,570 Linux pairs fail in one order
  only, and a byte sort picks the failing order in 20 and 5 of them; of the
  pairs compiling both ways, 768 Mac and 67 Linux differ in text, the read
  ones the libraries' own configuration or spellings of one value.
- **`cfgone/main2` is not correct C** (the critic, C11 §6.10.3p2, a
  *Constraint*): `a.h`'s `#define LIMIT 100` after `b.h`'s `LIMIT 10`
  redefines an object-like macro differently; clang only warns, and the
  program prints `3 10` with the warning on a program Heroes calls correct.
  **`dual`** (two `#ifndef K` defaults, the compiler-engineer's) violates
  nothing: both orders build silently, `5 105` and `9 109`.
- **What a detector would refuse.** Comparing two orders by preprocessed text
  or by final macros refuses a correct GLFW and OpenGL module and misses
  `cfgone` (its final `LIMIT` is 100 either way); a comparison of the bound
  names alone separates the three, unbuilt; no comparison of two fixed orders
  sees the critic's `mpq`; the historian found no tool with a record of it.
- **The compiler-engineer's route, built**: the emitter walks the
  declarations once in written order (record-only groups keep their place,
  `rec` builds); `-Werror=macro-redefined` refuses `main2` and, measured by
  the critic, newly refuses 0 of 125 Mac headers alone and 0 of 7,140 Linux
  pairs, 4 Mac ordered pairs and one real module, Expect's `EXP_ABORT` with
  libjpeg's `JPEG_LIB_VERSION` (`EXTERN` defined twice), refused in both
  orders; after clang refuses a header, `cli/header_order.hero` tries the
  module's other groups on its other side, at most n - 1 `-fsyntax-only`
  runs on the failure path, and names the group to write above (`jpeg/ba`:
  *write that group above this one*). Its note keeps *repair the header* above
  the correction, its headline says *cannot be compiled in one unit* where an
  order compiles, its move on `main2` changes `cap(50)` from 10 to 50 without
  saying so, and it is never asked on the `ffi_unknown_name` path, where
  GLFW's and GMP's false *declares no* are told (the critic).
- **The blind seat**: under today's spec and under F7, 4 of 4 readers read
  per-group isolation off § 4 and § 13's *against that header*
  (`spec:348-349`) and predict `limit_ab` `3 10` (it prints `3 50`) and
  `jpeg_ab` refused (it prints 1); under F7 and G1m, 2 of 2 predict all four
  programs as built. Given today's message, 2 of 2 repair `jpeg_ba` rightly
  while doubting it against § 4; given `rec`, 2 of 2 conclude order does not
  count and write a wrapper header, which builds. Writing, 8 of 8 put
  `stdio.h` first in both arms, the task having named it first.
- **Found beside, each its own cause** (the ffi-pragmatist, sorted by the
  critic): a feature-test macro (`_GNU_SOURCE`) cannot reach a header behind
  the compiler's prefix, so glibc's `sched_getcpu` is bound only through an
  unchecked prototype, a wrong one building and running; and `-Werror` fires
  inside GMP's and libavutil's own header code on this Mac, refusing correct
  programs, the policy panel 198 ruled (`-isystem` handed on as `-I`).

## Disagreements, stated plainly

- **`main2`: refuse or accept.** The compiler-engineer refuses it by the flag;
  the ffi-pragmatist accepts it with the warning told in Heroes' words. C11
  makes it a constraint violation (a diagnostic required, going on allowed),
  and design.md `:3744` says *this language has no warning level: a
  diagnostic is exit 1 or nothing*, so the second is not available; the
  resolution refuses, with a message the critic's findings make true.
- **F7 or the written order.** The spec-warden's F7 (*never changes what a
  program means*) is false on `dual`, legal C with two meanings, and its
  detector would refuse the correct GLFW module or miss `cfgone`; the
  spec-warden's own condition (*F7 becomes an objection if the detector
  refuses a program the sitting judges correct*) is met. The resolution takes
  the written order, stated.
- **G1m.** The spec-warden objects (+27, *a message's job*), resting on the
  writing arm; the critic shows that arm primed by its own brief and the
  measured effect on reading: 0 of 4 readers without it, 2 of 2 with it,
  predict the programs as the compiler builds them. Measurement beats
  opinion (CLAUDE.md § 12); G1m is taken.
- **The flag's reach** meets panel 198's warnings policy: a package's own
  header code is held to the program's warnings, which refuses GMP and
  libavutil on this Mac and, with the flag, Expect with libjpeg. One policy,
  one ruling, filed for a sitting below.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, the written order of a module's groups is its include order, after
   the compiler's own prefix, made true and said.**
   - The emitter walks a module's declarations once in written order
     (`headers()`, the compiler-engineer's `externs.hero`), a record-only
     group in its place, a repeated header at its first group.
   - Spec § 4's line 113 reads the spec-warden's F2, *Declaration order
     never matters but a group's (section 13); mutual recursion needs no
     forward declarations.*, and § 13's line 348 gains G1m, *C reads a
     module's headers in the order its groups are written, so one that
     needs another's names comes after it.* +33 legacy and +34 cl100k, a
     lower bound; the real count by one `--refresh` at the landing on the
     author's yes, re-argued above +50.
   - design.md §4.19 gains the sentence panel 091 owed, from the
     spec-warden's draft with the critic's corrections (the prefix named; a
     header of the program's own as the one place a switch or an `#undef`
     goes; no claim that such a header builds in either order), and §4.2
     `:949` the same narrowing; the open task's first item closes with a
     dated line, the task open for its other four.
2. **R2, a macro two headers define differently is refused** (C11
   §6.10.3p2): `-Werror=macro-redefined` in the compile and probe flags
   (`cli/flags.hero`; every cache key moves once, `cache` alone), told at exit
   1 with a true message: the macro, both headers and their lines; what each
   order would make the bound names mean; a `guess` fix drafting a header of
   the program's own that includes both, with an `#undef` between them. Its
   reach is measured again at the landing (the Linux pairs on the real unit,
   the tracked tree) and Expect with libjpeg is its pinned case.
3. **R3, a refused header is told the order that compiles**: the
   compiler-engineer's search lands, and with it: the correcting note
   *replaces* *repair the header* and the false headline where an order
   compiles; the search is asked on the `ffi_unknown_name` path too (GLFW's
   and GMP's false *declares no*); a move that changes what a bound name
   means says so; where no other group's order compiles, the C11 standard
   headers are tried as a prerequisite (the ffi-pragmatist's: `stdio.h` for
   `jpeglib.h` alone) and the note says to add a group naming it above; where
   none does, a header of the program's own (`guess`), never *repair the
   header* for a header the author did not write (`pcre2.h`'s configuration
   macro). Cost only on the failure path, a build that compiles within noise
   (measured: 400.19 G against 400.24 G).
4. **R4, refused**: a byte-sorted order (the vetoes; it refuses readline,
   libjpeg, GMP and GLFW orders written right and changes `cfgone/main2` and
   `mpq`'s values); per-group units (the vetoes; §4.19 `:2366`, every header
   with a prerequisite refused); a comparison over orders (by text or macros
   it refuses a correct GLFW module, by two fixed orders it misses `mpq`, by
   bound names it is unbuilt); F7 (false on `dual`); G1e (it writes today's
   reordering down as a rule); G3 (a new form, vetoed); the warning route
   (no warning level).
5. **R5, filed now, `blocking`, for a sitting of their own** (both reach a
   ruled policy or the unit's layout): a feature-test macro cannot reach a
   header behind the compiler's prefix, so a GNU function is bound only
   through an unchecked prototype; and a package's own header code held to
   the program's `-Werror` refuses GMP and libavutil on this Mac, against
   panel 198's ruling, with R2's flag in the same policy: defects 568 and
   569. 563 widened by the
   GLFW and GMP false `ffi_unknown_name` and the false *repair the header* for
   readline and `pcre2.h`, rows of its cause, landing with R3.

**The conservative alternative, the author's to choose instead**: F5 (§ 4's
order clause removed, -6) and no G1m, the emitter's walk and the messages
only, `main2` left to clang's warning; the spec-warden's recommended F7 with a
bound-names detector to be built, the next most robust.

## Process notes

- **Two seats broke a rule, each said so in its report.** The spec-warden
  wrote `/tmp/x_never.c` and removed it in the same command, outside the
  repository's root (CLAUDE.md § Hard stops); the coordinator found no such
  file at 01:50. The compiler-engineer ran `git -C tree diff --stat` in its
  copy at about 01:35, whose `.git` file pointed at `lane-panel-204`'s
  worktree, refreshing that index's stat (nothing staged, HEAD unmoved, read
  with `--no-optional-locks`), and moved the pointer out of its copy. The
  critic's second pass was told not to run git in a copied tree.
- **The coordinator's process ended at about 01:2x** and stopped the critic's
  first pass and three lanes; all were resumed at 01:28 from their
  transcripts, nothing lost.
- **The load reached 44 on 8 cores at 01:42**, two censuses running wide;
  the coordinator capped every seat and lane at three processes, and the
  ffi-pragmatist reports breaking that cap twice for about a minute each.
- **The coordinator's brief was wrong twice** and the critic's first pass
  caught both: it assumed the written order is the include order, and its
  grep missed the open task panel 091's sentence had become (a backtick
  between `#include` and `order`).

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the gate of the batch landing R1 to R3, `selfhost/` grows by at most 160 lines over at most four files, and `emission`, `emit`, `run`, `warnings` read 0 failed with nothing re-blessed; R3's widening (the `ffi_unknown_name` path, the prerequisite search) is beyond the prototype, so its count is re-read there | the landing |
| ffi-pragmatist | under the written order made true, `p/gmp/sg.hero` (Linux) and `p/gl/glfirst.hero` exit 0 printing `0` and `32768`; under a byte sort both exit 1 | the landing, the CI's Linux legs |
| spec-warden | P3 (only if G1m is taken): readers with G1m write the `stdio.h` group above at least 3 of 4 times, without it at most 2; scored here as primed, so owed again on a task that names the headers in the wrong order | a later sitting |
| historian | a binding tool that sorted third-party headers for years without breakage reports would turn its reading | open |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R5 as written above, the author answering
through the question widget between 02:29 and 02:33 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*Conservativo (F5)*, *F7 con un rilevatore* and *I want to read it first*,
on the coordinator's summary of each route; in the same widget *yes, one*
to the `--refresh` R1's sentences need at the landing. Recorded as a reading
(CLAUDE.md § 4). The author may overturn it.
