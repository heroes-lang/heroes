# Panel 208: a name its header marks deprecated builds silent, and one it marks unavailable is refused on its extern line

Convened 2026-10-10 by the coordinator at 15:08 for defect 584, filed at 13:56
from lane b18-guard's finding beside defect 571, under the author's deadline of
18:00 for the next step. **Lean sitting by the author's choice at 15:07**: the
compiler-engineer, the ffi-pragmatist and the historian, one completeness
critic pass after the reports, no blind seat and no paid run. The tree frozen
at **`391628b6`** (branch `lane-b18-guard`), each seat in its own copy made by
`git archive` under `.claude/worktrees/scratch-b15/208-<seat>/`. Briefs
written from 15:07 to 15:08; the seats launched at about 15:08; the
historian's report written by 15:21, the compiler-engineer's and the
ffi-pragmatist's by 15:57, the compiler-engineer finishing at 15:59 by its
`date`; the critic launched at about 16:01, its pass written by 16:06; this synthesis from 16:07, every time read from `date`. Briefs in `208-briefs/`, reports
in `208-reports/`. What the lean sitting gave up: a measurement of what a
reader of the spec expects of a deprecated binding.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **approve** (S) as S2 (both deprecation groups and `warning` quiet everywhere) plus an `unavailable` reader to exit 1; **object** (R), (M), (F), (N); no veto | (S) and (R) built in its copy: (S) 5 files +5/-114, S2 6 files +22/-119, the compiler's own tests 1,560; (R) 3 files +95/-2, refusing 7 of 9 deprecated uses and accepting `sprintf` and an enumerator; cost within noise for both |
| ffi-pragmatist | **approve** (P), a route unlisted: every bound name the header marks deprecated, by either group, refused on the `extern` line with the header's message; **object** (R), (S), (M), (F), (N); no veto | the SDK's 748 deprecated functions, Linux arm64's 58, OpenSSL's 1,037 and 963; none of the 21 `examples/` with an `extern` binds one; route edits made on emitted units, (R) leaving 3 of 6 at exit 0; the Linux runs of the edits unrun |
| historian (advisory) | **approve** (S) with the advice carried outside the build (Go's split, a route unlisted); **object** (R), (F), (N) and (M)'s refusing default | Zig's `@deprecated` (accepted 2025-02-11, merged 2025-02-27, reverted 2025-02-28), Go's FAQ and cgo's raw C warnings, Rust's `deny(warnings)` anti-pattern and `--cap-lints`, Swift's SE-0443, Nim's `-w`, C3, V, macOS 13's `sprintf` and `-Werror` |
| critic (one pass) | no verdict; the records already call these programs correct (panel 205's R5, defect 396's `run` goldens); deprecation closes no memory-safety class; (P) reaches a name never called but its verdict lands at a line of the compiler's C, a reader nobody priced; two unlisted routes | `grep` of `docs/panel/`, `issues/` and `tests/golden/`; the compiler-engineer's emitted unit for `RSA_new` made an error by hand, exit 1 at the C line; `_POSIX_C_SOURCE` hiding `strlcpy` and `arc4random` in plain C |

## What the sitting measured

- **Why `sprintf`'s call is silent** (the compiler-engineer and the
  ffi-pragmatist, independently): this Mac's SDK turns `_FORTIFY_SOURCE 2` on
  by default, and `secure/_stdio.h` makes `sprintf` a function-like macro
  expanding to `__builtin___sprintf_chk`, so the program's call names nothing
  deprecated; `(sprintf)(...)` or `-D_FORTIFY_SOURCE=0` warns. Not clang's
  system-header rule (`dep.h` warns by `-I` and by `-isystem`), not 571's
  guard. The historian's lead (`_POSIX_C_SOURCE`) is the condition of the
  declaration's attribute, a second switch.
- **A refusal at the call (R) is inexact by construction**: it reads names
  after the header's macros, so it refuses `twice` and accepts `sprintf`, and a
  deprecated enumerator or constant is read through an accessor under the
  quiet region and never refused.
- **A refusal at the binding (P) is exact over every measured shape**: the
  compiler's per-name probe names every bound C name once, in a form no macro
  hides; it refuses a name bound and never called.
- **Found beside**: 587 (`#pragma clang deprecated`, `-Wdeprecated-pragma`,
  raw at the compiler's own lines), 588 (`unavailable` is exit 2), 589
  (glibc's `ld` warning for `mktemp`, raw, Linux), all three filed at 15:58;
  and the compiler-engineer's s3, `main`'s temporary of a deprecated record
  warning raw at the emitted unit (571's gap, unfiled until this synthesis).
  `__attribute__((warning))` cannot carry a verdict: clang says it at `-O0`
  and not at `-O2`.
- **The records already call such a program correct** (the critic): panel
  205's R5 filed 571 as *the `deprecated` attribute firing clang's raw
  warning at the compiler's own probe line on a correct program
  (`blocking`)*, and its four run cases (a function bound and not called, an
  enumerator read, a record bound, `getcontext` through `_XOPEN_SOURCE`)
  build at exit 0 since 571's repair (`aeed52ec`'s body); defect 396's `run`
  goldens bind OpenSSL 3's deprecated `SHA256_Init` and call it correct, with
  `OPENSSL_SUPPRESS_DEPRECATED` in a first-group header
  (`tests/golden/run/fixedbugs-396-openssl.h:5-8`). (P) or (R) would refuse
  what both call correct; that (P) refuses 571's four is an inference from
  its rule, unrun.
- **Deprecation is not a memory-safety instrument** (the critic, on the
  ffi-pragmatist's own lists): `strcpy`, `strcat`, `strncpy`, `memcpy` and
  `scanf` are marked on neither platform, `sprintf` on this Mac only. A
  refusal of deprecated names closes no class of corruption, so design.md
  §1.12 does not lift it above Principle 0; the thesis argument (models write
  these calls) is unmeasured, the lean sitting having no blind seat.

## Disagreements, stated plainly

- **Silence or a refusal at the binding.** The compiler-engineer and the
  historian take silence; the ffi-pragmatist takes (P), exact over every
  measured shape where (R) is not, with the library's own switch as the way
  out. What decides it is the question the critic says the sitting never
  asked, *is a deprecated C name a mistake in the program or advice about
  it?*, and the project has answered it twice in ratified records: a correct
  program (panel 205's R5, defect 396). Deprecation also differs by platform
  and by library version while the program does not, and it reaches none of
  the unsafe calls a refusal would be for. The resolution takes silence, the
  most robust reading of what is already ratified, and records (P) as the
  author's to choose instead.
- **The advice is lost under silence.** The historian's Go split (a separate
  `heroes` question carrying it) and the critic's split by whose header
  deprecates are both unlisted until this sitting; the first is refused today
  by `.claude/rules/cli-surface.md`'s stopping rule (nothing types it, no
  effect is measured), the author's to admit as `heroes probe` was; the
  second still calls advice a mistake, only in a narrower place.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what the alternative would be is below the list.

1. **R1, a name its header marks deprecated binds and builds silent**, the
   program's own lines as the compiler's: S2, both clang groups
   (`-Wdeprecated-declarations`, `-Wdeprecated-pragma`) and
   `-Wattribute-warning` ignored over the whole unit after the groups'
   close, nothing else widened (panel 205's R2 region and R1's raised checks
   unchanged). Defect 584 closes with it, 587 with it, and the compiler-
   engineer's s3 (`main`'s temporary of a deprecated record, 571's gap, the
   same cause) as 584's shape. One `run` case per shape: a function called,
   bound and not called, a record in a signature and as a local, an
   enumerator, a macro under `#pragma clang deprecated`, `sprintf` with
   fortification off, each building silent at exit 0.
2. **R2, a name its header marks `unavailable` is refused on its `extern`
   line**, exit 1, the header's own words in a note, the seventh member of
   `.claude/rules/c-boundary.md`'s class (the compiler-engineer's 11-line
   reader); defect 588 closes with it, one `unsupported` case annotated.
3. **R3, glibc's link warning is silent as a deprecation is** (defect 589):
   the landing measures the narrowest linker word that drops a section's
   `.gnu.warning` text on the CI's Linux linkers and nothing else; if none
   is narrower than every linker warning, 589 stays open with that
   measurement and goes to the author.
4. **R4, design.md §4.19 gains one sentence**, *a name its header marks
   deprecated binds and builds silent: a deprecation is advice about a
   program, not a mistake in it, and this language has no warning level
   (panel 208)*; no spec sentence (none is measured as needed; the lean
   sitting had no reader to measure it with).
5. **R5, refused**: (R), inexact by construction; (P), against two ratified
   records and a verdict that moves with the platform and the library's
   version; (M) and (F), a form and a flag for no measured need, (F) against
   the stopping rule; (N), a warning level; the Go split, refused by the
   stopping rule until the author admits it; the split by whose header, which
   still calls advice a mistake.

**The alternative, the author's to choose instead**: (P), the ffi-pragmatist's
route, every bound name its header marks deprecated refused on the `extern`
line with the header's message and the library's switch in a first-group
header as the way out, panel 205's R5 and defect 396's header amended beneath,
a reader mapping clang's line in the compiler's C back to the binding still to
be built and priced.

## Process notes

- The brief cited design.md `:3744`, the trunk's numbering at `9743597b`;
  the frozen tree's is `:3800` (the ffi-pragmatist, the critic).
- The ffi-pragmatist stopped its Linux container at 15:56 with the route
  edits unrun there; its earlier Linux shape runs stand.
- The historian has no shell and could not read the clock.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the landing, `heroes build shapes/s10_sprintf.hero` exits 0 and prints nothing | the landing |
| ffi-pragmatist | under (P) `r584/call.hero` is refused on macOS's CI leg; under silence it builds silent on every leg | the CI's legs after the push |
| historian | if a refusal lands without a way out, an issue asking to call a deprecated C function on purpose is filed before the second milestone tag after it | the second tag |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R5 as written above, the author answering
through the question widget between 16:07:41 and 16:08:05 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R5* over
*Alternativa (P)* and *I want to read it first*, on the coordinator's summary
of each route. Recorded as a reading (CLAUDE.md § 4). The author may overturn
it. Landed by lane b19-dep, opened at 16:08 from `391628b6`.
