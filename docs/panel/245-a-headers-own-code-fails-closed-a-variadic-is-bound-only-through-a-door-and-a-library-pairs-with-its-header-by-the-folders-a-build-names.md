# 245: a header's own code fails closed, a variadic is bound only through a door, and a library pairs with its header by the folders a build names

**Status** a full sitting on the three systemic defects of the C boundary, convened 2026-11-10 by
the coordinator of batch 30 on the author's instruction of that day, meant as: *close all the
systemic defects too; hold one sitting with all of them in it*. Seats as panels 242 to 244 sat, by
the author's answer *four seats, no blind seat*; the gate asked once for the milestone, at panel
242. The resolution is `provisional — author ratification pending`.
**Seats** compiler-engineer, ffi-pragmatist, spec-warden, historian; the completeness critic on the
briefs before any seat (25 items, the briefs rewritten on them, the first versions kept as
`245-briefs/*.before-critic.md`) and on the reports after (25 items). Then, by `/panel` 3c (a route
that does not build is not adopted), two build seats on the routes the critic named most robust:
the ffi-pragmatist built Q1's fail-closed region with Q2's S and D in one compiler
(`245-briefs/build-q1q2.md`, `245-reports/build-q1q2.md`), the compiler-engineer Q3's P1 with its
P4 (`245-briefs/build-q3.md`, `245-reports/build-q3.md`).
**Base** `7fb6868b20dfaecbdc7cf501b75b79aef7b14cf3`, batch 30's round, its stage 2 compiler printing
`sources 0fe313ba58ba197c`; each seat built its own in `.claude/worktrees/245-<seat>`.
**Briefs** `docs/panel/245-briefs/`, **reports** `docs/panel/245-reports/`, **evidence**
`docs/panel/245-reports/evidence.tar.xz` (its note beside it).

## The proposal

Three questions (`245-briefs/00-shared.md`, widened by the critic's first pass):

1. **Q1, defect 2464.** A header's own code, a door's body above all since panel 243's R1 made a
   door the only route to a variadic, builds at exit 0 with clang's raw warnings: deprecation,
   `-Wattribute-warning`, and words of undefined behaviour (`1 << 40`, `a[5]` of an `int[2]`).
   Where the rule lives and what a door's call of a deprecated or warned function does.
2. **Q2, defect 2465.** A variadic whose callee reads past `...` by a run-time value reads an
   argument nobody passed: a door forwarding a format, `open` with `O_CREAT` and no mode,
   `sqlite3_mprintf`, `ioctl`. Whether a variadic may be bound directly at all.
3. **Q3, defect 2448.** A header found through the environment, `--include` or `CPATH`, describes
   another release than the library the link takes, at exit 0, both directions. What the build
   does, and on what it rests.

## The verdicts

| seat | Q1, 2464 | Q2, 2465 | Q3, 2448 |
|---|---|---|---|
| compiler-engineer | object to the lane's route, (a), (b), *told* and the door; recommend **E** (`-Weverything` ignored, a list raised) | approve **S**; object to **D**, keep C's contract for unmarked callees | approve **P4**; object to P2 and P3; after building P1 and its Linux tie, **approve P1 with the tie and its guard** |
| ffi-pragmatist | object to the lane's route; veto (a0) and (c); recommend the region **fail closed**, measured as (d) | approve **D** and **S** | object to the base; approve **P1** and P2 per declaring header, and **P4**; object to P3 |
| spec-warden | approve (b); object to keep, the lane's three words, (a) and (c) | approve **D and S** at +0; object to W0 landing unpaid | approve **P4**; a refusal (P1 or P3) at +0 only as a refusal; object to P2 |
| historian (advisory) | object to the lane's route (`-Wattribute-warning` is not advice) and to (a) and (c) | approve the door-only rule | approve pairing by construction or provenance |

## What each seat measured, in short

- **compiler-engineer** (`245-reports/compiler-engineer.md`): the lane's route closes 2464's
  reproducer and leaves 14 of 19 raw shapes raw, seven of them undefined behaviour at exit 0; (a)
  breaks defect 248's pinned `#warning`; (c) loses every kept error; E refuses the seven and moves 0
  of 3,161 headers; the found-beside false note, now defect 2469; *var* (D) refuses a correct
  `open(p, O_RDONLY)`; P4 built, and following the base's own note links readline 8.3's header to
  libedit.
- **ffi-pragmatist** (`245-reports/ffi-pragmatist.md`): curl 8.23.0's width checker, the one live
  `warning` attribute in 139 kegs; `sqlite3_config` and `sqlite3_mprintf` dying of SIGSEGV, `open`
  creating mode `0450`, all at exit 0 for direct bindings; 0 tracked programs refused by D; under
  `--library` alone readline's own record reads `0` for `42`, ASan a heap-buffer-overflow; P1 and P2
  agree on 11 builds.
- **spec-warden** (`245-reports/spec-warden.md`): every route +0 tokens, 8167 legacy and 8314
  cl100k at the base; W0, *"A function its header declares with `...` is never bound: bind instead
  a function of a header of your own that calls it"*, +27 vendored, a lower bound; the found-beside
  false citation, now defect 2470.
- **historian** (`245-reports/historian.md`): 58 rows across the three questions; the rows this
  synthesis rests on are re-read below.
- **critic, second pass** (`245-reports/critic-reports.md`): E fails open on a default error of
  plain clang and seven words raw at the base; an enumerated list leaves six raw, the
  ffi-pragmatist's own included; (d) refuses everything else; raising `-Wattribute-warning` gives a
  false order note through a syntax-only trial; 2448's fault reproduces; the author's profile sets
  `LIBRARY_PATH=:/opt/homebrew/lib`, whose empty element puts `.` first.
- **build, Q1 and Q2** (`245-reports/build-q1q2.md`): (d) with S and D in one compiler; the critic's
  17 shapes, 14 refused at `-O0` and 13 at `-O2`, every one exit 1 and none exit 2; curl's checker
  refused at both levels, its false note gone; both programs of defect 2469 told the true note;
  defect 158's 14 cases passed; `surface` 452 and 0; the census of 496 tracked roots binding a
  header, 492 built by the base and 491 by the route, the one that moves defect 2036's `printf(m)`.
- **build, Q3** (`245-reports/build-q3.md`): P1 judged per supplied name, the names a door's body
  reaches included, `LIBRARY_PATH` read from the link job; every mismatch of the sitting refused
  (readline under `--library`, `LIBRARY_PATH` and `--sanitize`, lzma's two unpaired builds, a stray
  `libcurl` in `.`), every paired build passing; the 8 examples printing the same bytes, `run` 42
  and 0, `unsupported` 20 and 0; P4 moving the three goldens that pin its old words.

## The resolution adopted, provisionally

**R1, Q1 (defect 2464): a header's own code fails closed.** In the region a group's header is
compiled in, every warning clang gives is an error, except an allow-list of advice: the two
deprecation words quieted, `#pragma message` and `#warning` told and never refused (defect 248).
As built: each unit's line carries `-Werror` and `-Wno-error=#warnings`; `-Wimplicit-int` and
`-Wuser-defined-warnings`, which the region's `-Wall` ignore takes under, are errors again after it;
the trials that write an order note compile under the unit's words, and a code-generating trial
(`-c -femit-all-decls`) runs where the syntax pass compiles the written order, so no note says a
header compiles in an order the unit refuses; defect 158's carrier line keeps its case testing what
it pinned. **The level**: a unit compiled above `-O0` that includes a group's header is compiled
once more at `-O0` with its own words, no `-femit-all-decls`, its object discarded
(`cli/region_level.hero`), and a refusal there is the unit's, so a program has one verdict at
every level: the `attr_warning` shape refused under `build` and `run` at `-O0` and `-O2`, the 496
tracked roots binding a header the same at both levels, an unused `static inline` calling a
warned function built at both. It costs one more compile of such a unit on a cold build above
`-O0`, +24 % to +95 % of that unit's own compile in instructions retired over three units, and
nothing on a warm build or at `-O0`; the unit's key names that compile's words. The refusal at
`-O2` owes a note saying the `-O0` compile is what refused it.

**R2, Q2 (defect 2465): a variadic is bound only through a door, and a door's format is a
literal.** S: `-Wformat-security` is an error in the region with `-Wformat`. D: a function its
header declares variadic, bound with nothing past its `...`, is refused at the binding,
`ffi_call_shape`. The notes cite C11 6.5.2.2p7 and 7.16.1.1p2 for what C does past a prototype's
`...`, and spec § 13 only for what § 13 holds, which repairs defect 2470's five false citations
(lines 220, 230, 240, 250 and 260 of `emit/ffi_past_prototype.hero`). Defect 2036's correct
`printf(m)` case, the one tracked program S refuses, is pinned twice: refused in `unsupported/` as
a known cost, and built in `run/` as `printf("%s", m)`, which prints its own `hi`. W0 is not landed:
it has no payment; its note under W0 is written in the build report, for the author to choose.

**R3, Q3 (defect 2448): a library and the header that declares its names come from folders the
build names, or neither.** P1 as built: for every name a library the program names supplies, the
names a door's body reaches included, the header clang read for its declaration and the library
file the link took come both from folders the build names, or both from neither: for a header the
folders panel 237's rule counts (the program's own, `--include`, a package's `-I`, the
environment's `CPATH`, `C_INCLUDE_PATH` and `CCC_OVERRIDE_OPTIONS`), for a library `--library`, a
package's `-L` and `LIBRARY_PATH` read from clang's own link job, an empty element counting as the
working folder. Otherwise the build is
refused, exit 1, `ffi_unpaired_release`, one refusal per library naming both halves and who named
each. P4: `machine_locked_path`'s note names both halves. A name only the program's own folder
declares is not judged. **The tie, measured on Linux arm64**: P1 alone refused a paired zlib build four ways on Debian,
a folder clang adds on its own being named by `--library` or `LIBRARY_PATH`, so a folder clang
adds itself counts as the platform's whichever way it is also named, and only where no other
folder clang adds holds a file of that name with other bytes: the tie as first worded let a
planted second release build and run at exit 0. With the guard, 7 of 7 correct builds pass on
Linux arm64, a stray copy and the planted release are refused, and the 28 rows of this Mac keep
their verdicts. The class of 2448 stays `blocking`, a measured fault.

**R4, defect 2469** is repaired by R1's `-Wimplicit-int` and its trials; **defect 2470** by R2's
notes, line 240 included.

**R5, the landing**, in batch 31's lane `b31-245`, from the two build seats' diffs: the four
modules past 300 lines split along the seams the build reports name (`header_region`,
`header_order`, `ffi_past_prototype`, `units`), the new modules kept under it; a golden case per
shape (a refusal of each kind, the loop, a code-generating refusal, D's `open` and
`sqlite3_mprintf`, P1's mismatches with libraries the case carries); the three goldens P4 moves,
written from a run; the gate whole, every golden tree judging a widened refusal, `cache` alone.
design.md §4.19 and Part 7 row 10 and `.claude/rules/c-boundary.md` amended in the landing's own
commit.

## Disagreements, not smoothed

- **E against (d).** The compiler-engineer recommended E with a written condition, a default
  error of plain clang that E silences; the critic met it (`-Watomic-access`, seven words raw at the
  base and silent under E). R1 takes (d). **The conservative alternative** is the ffi-pragmatist's
  enumerated prototype, which leaves six words raw and needs a test for when clang's list moves.
- **D against C's contract.** The compiler-engineer measured a correct `open(p, O_RDONLY)` refused
  by D and argued the door rebuilds the fault; the critic ran the door the note describes and it
  made the file `-rw-r--r--`. R2 takes D. **The conservative alternative** is S alone with C's
  contract stated in design.md Part 7 row 10, which keeps a direct `open` creating `-r--r-x---` and
  `sqlite3_mprintf` reading garbage at exit 0.
- **P1 against P2.** P2, pairing by prefix, agrees with P1 on every build of this Mac and rests on
  where packagers put files, which Nix's split outputs refute (the historian's row 3.9, unrun).
  **The conservative alternative to R3** is P4 with P1 in the fault's direction only, a library
  named while its names' header is not, which keeps lzma's forward pair at exit 0.

## Prior art, per resolution

R2's door-only rule was reached first by Go's cgo and by Swift, and its format error by Debian; R3's
pairing was reached first by OpenSSH's build and by zlib's own check. Every page was re-read on
2026-11-11 by the coordinator's `curl`, each quoted sentence found verbatim. The lines are the
decision issue's `## Prior art`.

## Left open, as questions

- **What a build cannot promise.** Two halves each named by unrelated words build at exit 0
  (the keg's curl header by `--include` with a stray library), and the loader can pair another
  release at run time (`DYLD_LIBRARY_PATH`): R3 judges what the build names and stops at the link.
- **Panel 237's stand-in rule** refuses a named folder holding a header the platform ships; whether
  it should yield when the same build names that library's folder too is asked by the
  compiler-engineer and answered by no seat.
- **A unit's cache key** names `VERSION` and not the compiler's sources, so a compiler that changes
  only what it refuses shares the warm objects of the one before it: defect 2472, found by the
  ffi-pragmatist's build; R1 keys its own second compile.
- **W0**, the specification's sentence for D, waits on a payment and on the author.
- **Off this Mac**: Linux x86-64, Windows and glibc's headers under the region's `-Werror` are
  unrun; the CI's four legs after the landing are the platforms, a red one a defect.

## Predictions to score

- **ffi-pragmatist (build)**: at R5's gate the census of tracked roots binding a header moves the
  one program named above and no other, at `-O0` and at `-O2` alike; checkable at the gate.
- **compiler-engineer (build)**: when R3 lands, the CI's Linux legs print `ffi_unpaired_release` in
  no `run`, `corpus` or `compat` case, and `supplied_names.hero` stays at or under 300 code lines;
  scored by the CI after the landing's push.
- **spec-warden**: W0, if the author lands it, reads +27 vendored as a lower bound; scored by
  `heroes measure` at that landing.

## Process notes

- The seats were four by the author's answer; the critic read the five briefs first, and they were
  rewritten on its 25 items before any seat started.
- Two build seats followed the critic's second pass, by `/panel` 3c. Each was asked one more
  measurement after its report: the ffi-pragmatist the level ruling's single-verdict answer, the
  compiler-engineer the Linux tie, which its own report named an inference.
- Lapses, each recorded in its report and undone: the critic and the compiler-engineer each wrote a
  file under `/tmp` and removed it; the ffi-pragmatist's unquoted heredoc ran backticked words,
  writing nothing.
- Batch 30's gate B, its aggregated pass and a Linux arm64 leg ran on this Mac while the seats
  worked, and another session's Rust builds too: no duration in any report is a measurement of
  speed, which each says.

## Author's verdict

Pending. The sitting's decision issue, `Panel 245:`, is filed open:
https://github.com/heroes-lang/heroes/issues/2475. **What a yes
settles**: R1 to R5 as adopted. **What it does not**: W0 in the specification, which waits on its
payment; the conservative alternatives above, which stay the author's to choose.
