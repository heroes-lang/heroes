# Panel 216: a header constant C leaves undefined is read by running the program's own unit under a trap, and a statement expression is asserted in its accessor

**Convened 2026-10-16 from 21:45, synthesised 2026-10-17 from 01:30**, on defect 2075
(https://github.com/heroes-lang/heroes/issues/2075, `systemic`), under the author's goal given as
they left the session that evening: close the open milestones and the blocking defects. **Lane:
soundness**: compiler-engineer, ffi-pragmatist, and a completeness critic, each twice. No route
adopted adds surface, a diagnostic class or a spec token, so the sitting was not re-convened full
(the brief's rule). **Resolution: provisional, author ratification pending.** Briefs:
`docs/panel/216-briefs/` (the shared brief corrected at 22:05 after the critic's first pass, which
it records). Reports: `docs/panel/216-reports/` (`compiler-engineer.md` and `ffi-pragmatist.md`,
each with a dated second pass; `critic-briefs.md`, both passes). Every number below is a seat's,
with its command in that seat's report, or the coordinator's, with its command named here.

## The proposal, verbatim (the shared brief's question)

> A group's integer `constant` whose header value C leaves undefined, and which clang folds with no
> warning at all, builds at exit 0 and prints whatever the machine held. Two shapes are known:
> `INT_MIN / -1`, and a floating value converted to an integer out of its range, `(int)(1e20)`.
> What does the compiler do with them, given that the one C11 check the coordinator found that sees
> them also refuses values C defines?

Routes A to E were the coordinator's; F to J the critic's first pass; K the ffi-pragmatist's; F2
the compiler-engineer's repair of F; Ux the critic's second pass; Ux-whole the compiler-engineer's
repair of Ux; variant S and the trap mode the ffi-pragmatist's repair of Ux-whole.

## What was measured

- **The defect, re-measured**: `K_DIV` (`INT_MIN / -1`) printed `1835574252 1794335708 1840260076`
  over three runs of `heroes build`'s `-O0` on this Mac, and `0 0 0` at `-O2`; the shared brief's
  first table had read `heroes run` as `-O0`, which compiles at `-O2`, corrected at 22:05.
- **The class is wider than two shapes** (critic, pass 1): fifteen more undefined neighbours build
  silent (`INT_MIN % -1`, NaN and infinity cast, an undefined value under `* 0`, in a ternary or a
  comparison), and the same defect reaches `f64` and record constants. A header value's own
  `_Pragma` disarms today's check (ffi-pragmatist, filed as defect 2104). A statement expression
  stopped the build at exit 2 (filed as defect 2101).
- **B, C, D vetoed** by the ffi-pragmatist over the real headers: B refuses a `run` golden and eight
  more values C defines; C refuses 42,107 of 42,107 real macros on clang 18.1.3, the floor and two CI
  legs; D refuses 26 to 38 real macros a C binding builds today.
- **F, the undefined value read at build time as a sanitizer trap** (critic's route, measured by the
  ffi-pragmatist): 0 of 127,090 gate-passing real macros refused on three clangs. **Then found
  unsound twice**: compiled at `-O1` with the sanitizer, the probe judges another header than the
  build compiles (compiler-engineer, `feat.h` branching on `__OPTIMIZE__`); repaired as F2, which
  preprocesses under the build's words first, F2 is right at `-O2` and wrong at the `-O0` default
  (critic, pass 2): silent misses for `__LINE__`, `sizeof(__FILE__)` and a `__builtin_constant_p`
  of a const function, false refusals for `__COUNTER__` and the mirrored `__builtin_constant_p`
  shape, floats still varying, and a canary that sees the sanitizer switched off and not the
  optimiser's folding switched off (a false message at exit 1 on `K_FDIV`). The compiler-engineer
  withdrew F2 on its own stated condition: it refuses `A_COUNTER_MISS`, a value C defines at every
  level.
- **Ux, run the program's own unit's accessors under the sanitizer** (critic, pass 2, by hand): 22
  flagged and 26 kept of 48 shapes, the same at `-O0` and `-O2`, every undefined integer the trunk
  builds and three floats, every defined value kept. **Compiled with the sanitizer in one step it
  repeats F's drift** (compiler-engineer): `FEAT_UB` kept while the program prints `1803133900`.
  **Preprocessed first under the program's own words, Ux-whole**: 44 flagged and 52 kept over 96
  cells, `diff`-identical to the critic's, `FEAT_UB` caught at both levels; about 0.3 s real cold
  on this Mac, 0.07 s on Linux arm64, nothing warm (the verdict kept by its key).
- **Ux-whole over the real headers, the whole population** (ffi-pragmatist, pass 2): Apple clang
  21.0.0, 41,114 bound real macros, **0 flagged** at `-O0` and at `-O2`; Debian clang 22.1.8,
  43,827, 0 and 0; Ubuntu clang 18.1.3, 42,086, 0 and 0. The control flags 17 of 17 undefined
  values and 0 of 19 defined ones; the corpus, 96, 100 and 100 accessors, 0 flagged on every leg.
- **The trap mode** (ffi-pragmatist, pass 2): the same verdicts as the report mode on the 96 cells,
  the attack cells and every real macro, on all three clangs, with **no sanitizer runtime**: on
  `ubuntu:24.04` with `clang` installed without recommends, the report mode links for none of 131
  libraries and the trap mode runs clean on all 131. A probe that resumes after each trap checked
  OpenSSL's 7,425 accessors in one run, 0.22 to 0.32 s; one child per accessor took 2.7 s.
- **Three costs of Ux-whole as the compiler-engineer linked it** (ffi-pragmatist, pass 2): linked
  as the recipe links, the probe failed for 42 of 84 unit-levels on this Mac and 44 of 88 on each
  Linux; linked as the program is, it must load the program's libraries at build time, so a correct
  binding whose library is not loadable on the build machine is refused (`TK_K`) or an undefined
  one passes in silence (`TK_BAD`); and the report mode at `-O2` on clang 22 does not stop after
  reporting, went on into the program's own `main` and wrote a file during the build. **Repaired,
  variant S**: the unit's own text with `hero_extern_refs` no longer `used`, dead-stripped, linked
  with the runtime alone: linked and ran for every corpus unit (84 of 84, 88 of 88, 88 of 88) with
  no library, `TK_K` kept and `TK_BAD` flagged on this Mac and Debian.
- **Ux runs header code during `heroes build`** (compiler-engineer): a header constructor wrote one
  line under the probe and none under the trunk's build. The specification and the rules are
  silent (searched); design.md §4.19's pkg-config repair treated a linker the file named, run during
  the build, as a defect, the nearest ruling, which is not this one (ffi-pragmatist).
- **α2 and the reader, for defect 2101** (compiler-engineer, built): a constant of the program's own
  headers asserted inside its accessor, where C reads a statement expression; the critic's pass 2
  found seven rows still at exit 2, one cause, a requirement clang prints over several lines read
  as one line, and the compiler-engineer built that reader. **Landed by the coordinator in lane
  `lane-land215` the same night**: of the fifteen shapes of the seat's `st.h`, the nine at exit 2
  build two (`({ 3; })` prints 3, `({ 3; 4; })` 4) and refuse seven as `ffi_not_constant` at exit 1,
  the other six unchanged; `ffi_not_constant`'s note, *`X` is an object or a macro that expands to
  one — reading it twice could give two answers*, false of `({ int x = 3; x; })`, was made true;
  `emission` moved 35 blessed files, the seat's 34 and defect 2074's case added since.
- **Outside every route** (critic and compiler-engineer): a value clang folds in an initialiser,
  `enum { E = 1 << 31 }`, a `static const`, a brace-list record, prints `-2147483648`, stable,
  under every compiler built here.
- **Unrun**: the Windows box (off throughout); the trap mode's object cross-compiled for Windows
  references no `__ubsan` symbol, an inference rather than a run (ffi-pragmatist).

## Verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **adopt Ux-whole, preprocessed first**; object Ux one-step (soundness), Ux-split (0.74 to 0.77 s per 10^9 reads at `-O2`), F2 (recorded conservative); **adopt α2 and the reader**; **veto J** (§1.1, §1.11) | §1.1, §1.7, Part 5 | about F2's 239 code lines plus 40, unbuilt; 0.3 s cold on this Mac, 0.07 s Linux arm64, nothing warm; α2 and the reader +20 code lines | Ux-whole gives the 96 verdicts and the critic's attack cells unchanged, `FEAT_UB` flagged at `-O0` and `-O2`, moves 0 `emission` files, costs at most 0.5 s real cold on `K_DIV` | preprocess first; the run bounded in time and bytes; a two-sided canary; the Windows link measured before landing; build-time execution ruled by the author. Turns to object if Windows needs a change to the user's machine or the author rules `build` must never execute the program's code |
| ffi-pragmatist | pass 1: **approve F**, vetoes B, C, D; pass 2: **object Ux-whole as specified**, **veto** any probe that must link or load the program's libraries and any form that requires the sanitizer runtime; **approve the repaired form** | §1.11, §1.12, §4.19 | the resuming trap probe, 0.22 to 0.32 s for 7,425 accessors | built as (a) to (e), 0 flags over the 41,114, 43,827 and 42,086 bound macros and the corpus at both levels on all three clangs, control 17 and 0; `TK_K` builds, `TK_BAD` refused with its library absent at load; on `ubuntu:24.04` without `libclang-rt-18-dev` every program that builds today still builds | (a) the trap mode decides, each accessor isolated by resuming after a trap; (b) variant S; (c) a finished line and the two-sided canary in every probe, a probe that does not finish refusing the build with words naming the probe's death and the header, never *C leaves the value undefined*; (d) stdin `/dev/null`, output to a file, time and byte bounds; (e) the report mode only with `-fno-sanitize-recover` and `UBSAN_OPTIONS=log_path`; (f) the Windows box runs the resuming trap probe before landing |
| completeness critic | pass 1: the brief's two false lines corrected, routes F to J, the class widened, 2101 filed; pass 2: F2 unsound at `-O0`, the canary one-sided, α2's seven rows, **Ux** | — | — | — | F2 would need floats asked, the accessor at the unit's own `#line`, the level seen, the initialiser-folded class named outside; α2 needs the multi-line reader before 2101 closes (built) |

## Disagreements, stated plainly

- **How the probe links.** The compiler-engineer's recipe links the program's objects and libraries;
  the ffi-pragmatist vetoes that, measured on half the corpus failing to link and a library
  absent at load turning a correct binding into a refusal. **Measurement decides it**: variant S
  links and runs every corpus unit with no library, and the compiler-engineer's own soundness
  conditions (preprocess first, bounds, canary) are untouched by it. Variant S is adopted.
- **Report mode or trap mode.** The report mode names the check that fired, which a message could
  quote; the trap mode needs no runtime and cannot run past a report into `main`. **The trap mode
  decides**, measured equal on every cell and macro; the report mode stays available only under
  condition (e), and nothing in the resolution needs it.
- **What design.md does not rule.** Whether `heroes build` may run the program's header code at all.
  The compiler-engineer makes it the author's to rule; the ffi-pragmatist names the nearest ruling
  (§4.19's pkg-config repair, a linker run during the build, treated as a defect) and finds it not
  this one. The synthesis does not decide it: it is the question below.

## Resolution (provisional: author ratification pending)

**R1, the route for a value C leaves undefined: Ux-trap-S.** Per module unit that binds an integer,
`f64` or record constant of the program's own headers: the unit's own text, preprocessed with `-E`
under the program's own words (its include lines, defines and level); compiled at the program's own
level with the four checks the compiler-engineer chose, in trap mode
(`-fsanitize-trap`), its `hero_extern_refs` no longer `used` and dead-stripped, linked with the
runtime alone (variant S); its accessors called by one probe that resumes after each trap, so each
is judged alone, with stdin `/dev/null`, its output to a file, and `hero_run_limit` and
`hero_run_limit_writes` bounding it; every probe carries the two-sided canary (an undefined accessor
that must trap, a defined one that must not) and ends with a finished line, and a probe that does
not finish refuses the build with words naming the probe's death and the header, never *C leaves the
value undefined*. A trapped accessor is told `ffi_constant_type` on the constant's line, the
header's line quoted, as defect 2072's region tells today. The verdict is kept by its key, so a warm
build pays nothing. Today's region (`emit/value_check.hero`) stays until R1 lands and supersedes it.

**R2, defect 2101: α2 and the reader of a requirement over several lines, now.** A constant of the
program's own headers whose accessor returns it is asserted inside that accessor; a requirement
clang prints over several lines is read as one; `ffi_not_constant`'s note is true of a statement
expression. Landed in lane `lane-land215` (`emit/extern_assert.hero`, `emit/constant_accessor.hero`,
`emit/ffi_constant_value.hero`'s `assertion_lines`, `emit/ffi_declared.hero`).

**R3, when R1 lands: after two conditions no seat can meet tonight.** (i) The Windows box runs the
resuming trap probe and links its unit without a change to the user's machine (both seats' condition);
(ii) the author rules whether `heroes build` may run the program's header code, which R1 does by
construction (the compiler-engineer's condition). R1 is built in a lane of its own, gated on this
Mac, Linux arm64 and the Ubuntu clang 18.1.3 leg, and merged when (i) and (ii) are met. Until then
defects 2075 (`systemic`) and 2104 (`blocking`) stay open, so **M-buildable-structs is not tagged**
(CLAUDE.md § Verification: no milestone over an open blocking or systemic defect).

**R4, the class outside every route is filed as its own defect**, defect 2109
(https://github.com/heroes-lang/heroes/issues/2109), `systemic`, a ruling no rule reaches: a value
clang folds in an initialiser, which no route here executes, reproduced by the coordinator with the
trunk's compiler: `enum { E_SHIFT = 1 << 31 }` builds and prints `-2147483648` on three runs, a
`static const` holding `INT_MIN / -1` builds and prints `1`, while `#define D_SHIFT (1 << 31)` is
refused today.

**R5, the routes refused**, each with its measured reason: A (the defect), B, C, D (vetoed over the
real headers), E (a run-time trap changes what a program does and blames a C library's double free,
no constant named, at `-O2`), F and F2 (unsound at `-O0`, F2 withdrawn on its seat's own condition),
G (closes the float half only and changes every header's inline code), H (its filter refuses no real
macro, so it saves nothing, and a pragma passes it on clang 18), I (accepts the undefined value), J
(vetoed, §1.1 and §1.11), K alone (refuses `(int)(intptr_t)(void *)16`, a value C defines, and keeps
`1 << 31` under a pragma), Ux one-step (unsound, `FEAT_UB`), Ux-split (the run-time cost of an
accessor in another unit), Ux-whole linked with the program's libraries (vetoed, §1.12).

### The question to the author

**May `heroes build` run the program's header code to judge its constants?** R1 compiles the
module's own unit and calls its accessors; a header's constructor runs with them (measured: it wrote
a line). Nothing in the specification or the rules forbids it, and nothing allows it. **The
recommendation is yes, bounded as R1 bounds it**, because it is the one route measured to tell every
undefined integer and float shape the trunk builds, at the level the program is built at, while
refusing none of about 42,000 real macros on three clangs; every route that does not run the code
was measured wrong in one direction or the other. A no makes the conservative resolution below the
adopted one.

### What the conservative resolution would have been, so the author can choose it

F2 with the critic's four conditions (floats asked, the probe's accessor at the unit's own `#line`,
the level dependence seen, the initialiser-folded class named outside), as the compiler-engineer
records it: it executes nothing, and it still misses the `-O0` shapes the critic found and refuses
`__COUNTER__`'s. Or, the ffi-pragmatist's conservative route, K alone (`diagnose_if` in the unit,
the note naming the value), with its measured hole, `1 << 31` under a pragma, and its one refusal of
a defined pointer shape.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | built, Ux-whole gives the 96 verdicts of `ux48/pre48.txt` and the critic's attack cells unchanged, `FEAT_UB` flagged at `-O0` and `-O2`, 0 `emission` files moved, at most 0.5 s real cold on `K_DIV` | the seat's scripts against the built compiler; `emission`; `/usr/bin/time -p`, three runs | R1's landing |
| ffi-pragmatist | built as (a) to (e), 0 flags over the bound real macros and the corpus at both levels on three clangs, control 17 and 0; `TK_K` builds, `TK_BAD` refused; on `ubuntu:24.04` without `libclang-rt-18-dev` every program that builds today builds | the seat's `216-ffi-m2/` scripts against the built compiler | R1's landing |
| coordinator | R2 moves no `run`, `unsupported` or `warnings` verdict but its own cases, and builds both `st.h` shapes clang folds | the lane's gate | R2's landing, measured 2026-10-17 in lane `lane-land215`: `run` 531/0, `unsupported` 329/0, `warnings` 610/0, `emit` 13/0 |

## Process notes

- **The brief carried two false lines**, both corrected under their text at 22:05 from the critic's
  first pass: the Mac's `-O0` column (it was `heroes run`, which is `-O2`), and `--sanitize`
  *traps* (it prints and runs on, exit 0).
- **Each route that looked settled was overturned by the next pass's measurement**: F by the
  compiler-engineer, F2 by the critic, Ux by the compiler-engineer, Ux-whole's linking by the
  ffi-pragmatist. The resolution is the last route standing after every seat attacked it with a
  run, which is why it carries every seat's conditions.
- **Found beside the sitting and filed or to file**: defects 2101 and 2104 (filed 2026-10-16); the
  initialiser-folded class (R4, defect 2109); and defect 2108
  (https://github.com/heroes-lang/heroes/issues/2108, `blocking`): `heroes run --sanitize` on
  Ubuntu's clang 18.1.3 installed without recommends exits 2 with *internal error: the runtime did
  not compile*, `'sanitizer/common_interface_defs.h' file not found`, found by the ffi-pragmatist's
  pass 2 and reproduced by the coordinator in a clean `ubuntu:24.04` container.
- **No historian sat**, so this file and its ratification carry no `## Prior art`
  (`.claude/rules/records.md` § And the record says whose idea it was).

## Author's verdict

Pending: https://github.com/heroes-lang/heroes/issues/2110 asked for it, naming the sitting as
`panel 216`, with its one question and the recommendation.

**RATIFIED, 2026-10-17**, R1 to R5 as written above, the author answering in conversation on
returning to the session that night, on the coordinator's summary, which put five decisions each
with its measurement and a recommendation, the first this sitting's: meant as *I accept all your
recommendations on the things I have to decide*. So **the question to the author is answered yes**:
`heroes build` may run a program's header code to judge its constants, bounded as R1 bounds it, and
the conservative resolution is not taken. Recorded as a reading (CLAUDE.md § 4). The author may
overturn it.

**What still stands between R1 and its landing**: the Windows box running the resuming trap probe
and linking its unit without a change to the user's machine (R3's condition (i), both seats'),
which only the author can start; condition (ii) is this answer. Defects 2075 and 2104 stay open
until R1 lands.

**Condition (i), measured 2026-10-17 on the Windows box** (clang 23.1.1, `x86_64-pc-windows-msvc`,
the compiler built there from the seed at `71c762929`), the author having started it on being
asked: the compiler-engineer's `ux2.sh` recipe in trap mode, each of the 48 shapes' own unit
preprocessed under the program's words, compiled with the four checks and `-fsanitize-trap`,
linked with the runtime alone and nothing installed: **44 trapped and 52 kept over the 96 cells,
`diff`-identical cell by cell to this Mac's `pre48.txt`**; a trap ends the probe as *Illegal
instruction*, exit 132 under the box's bash. And the resuming probe: one program binding `K_DIV`,
`K_COMMA`, `K_FLT` and `K_FDIV`, its probe writing each accessor's index before calling it and
started again after the one that trapped, read `K_DIV` and `K_FLT` trapped and the two others kept
in 3 runs, at `-O0` and `-O2`. So both of R3's conditions stand met; R1 is built in its lane and
judged by the CI's four legs after its push. The scripts are the coordinator's, `ux_trap.sh` and
`resume.sh`, copied from the compiler-engineer's `ux2.sh` and the ffi-pragmatist's `resume.py`.

**And a correction of the sentence above, measured the same hour on the box.** Its 48 shapes'
units call no C library, so they did not ask what variant S does where a unit does: a unit
defining `uses_the_library`, which calls `library_function_nobody_links`, beside two accessors,
compiled with the checks in trap mode and `-ffunction-sections -fdata-sections`, and linked with a
probe that calls only the accessors: with `-Wl,/OPT:REF` (link.exe) the link exits 1 on the
unresolved symbol, and with `-fuse-ld=lld -Wl,/OPT:REF` it exits 1 too, *undefined symbol:
library_function_nobody_links*, at `-O0` and `-O2`, so dead-stripping does not drop a reference
there as ld64's `-dead_strip` and GNU ld's `--gc-sections` do on the corpus (the ffi-pragmatist's
84 of 84 and 88 of 88). With `-Wl,/FORCE:UNRESOLVED` it links, exit 0, and the probe prints the
defined accessor's `7` and traps on the undefined one, exit 132, at both levels. No library is
linked or loaded either way, which is the ffi-pragmatist's veto line, and an accessor calls none,
since `__builtin_constant_p` refuses a constant that is a call (`ffi_not_constant`). So R1's probe
links with the platform's own word for leaving the program's libraries out: `-dead_strip` on
Darwin, `--gc-sections` on Linux, `/FORCE:UNRESOLVED` on Windows.
