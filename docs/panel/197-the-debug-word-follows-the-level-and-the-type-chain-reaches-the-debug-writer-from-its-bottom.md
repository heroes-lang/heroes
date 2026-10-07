# Panel 197: the debug word follows the level, and the type chain reaches the debug writer from its bottom

Convened 2026-10-07 by the coordinator for defects 322 and 219, two
improvements of batch 14 whose cards name a sitting (322: *the flag list
touches every program's debug information, a question for a sitting of its
own*), under the author's instruction of that day to take the whole queue of
improvements. **Soundness lane** (`.claude/skills/panel/SKILL.md` § Two
lanes): the compiler-engineer and the ffi-pragmatist, the completeness critic
before the seats and after them; no surface, no diagnostic and no spec token
moves (the spec's grep for debugging terms is empty, the critic's first pass,
fact 2). The tree frozen at **`dad2da47`**, worktree `lane-panel-197`. Briefs
written from 12:12, repaired from 12:34 on the critic's first pass (19
repairs, every one applied); the seats from 12:35, stopped by API session
limits three times and resumed at 13:41, 14:48 and about 16:45; the
ffi-pragmatist done at about 14:16 and its Windows item at 14:22 to 14:24
once the box answered again; the compiler-engineer done by 16:44; the critic's
second pass 16:48 to 17:19; this synthesis from 17:21, every time read from
`date`. Briefs in `197-briefs/`, reports in `197-reports/` (each copied by the
coordinator from the seat's reply, a subagent's Write of a report file being
refused, and so marked in each).

## The question, verbatim from the brief

`selfhost/cli/flags.hero`'s `flags()` passes clang sixteen words, the second
`-g`. Defect 322: at `-O2` the 400-return shape's clang footprint drops by
more than two fifths under `-gline-tables-only`. Defect 219: clang's debug
information dies on a type chain between 3,000 and 5,000 nested variants on
this Mac. **Which debug information does a build ask clang for, at which
level, and how does a type chain past panel 184's floor of 2,000 compile under
it?** The routes, a list to be widened: (A) `-g` stays and clang's stack is
raised; (B) `-gline-tables-only` for every build; (C) `-g` unoptimised and
line tables at `-O2`; (D) line tables by default, `-g` under a flag; and, from
the critic's first pass, (E) keep `-g` and hand LLVM's DWARF writer the chain
from its bottom, (F) name the debug kind, (G) a per-unit word, (H) retry on
death, (I) the probes at `-g0`, (J) `-g0`, (K) `-gno-column-info`, (L) an
environment variable.

## The verdict table

| | compiler-engineer | ffi-pragmatist | critic, second pass |
|---|---|---|---|
| **(E) the cast touch** | **adopt** (built, one emitter line; 5,000 and 10,000 build under `-g` on this Mac and Linux) | allow alongside (C) | verified on Ubuntu clang 18.1.3 and Apple clang 21 (bottom first in `retainedTypes`); the ffi seat's own (E) was another mechanism |
| **(C) `-g` at `-O0`, line tables at `-O2`** | **adopt** (built, +6 code lines without comments, +26 by `code_lines`) | **adopt** | the lane holds, `-O2`'s help line unchanged |
| **(B) line tables everywhere** | **object** (the `-O0` debugger's variables, M-typed-inspection's working half) | **object** (a header's C locals at `-O0`) | — |
| **(J) `-g0`** | refused (breakpoints do not resolve) | **veto** (ASan loses its location; Linux names a false file) | — |
| **(A) raise clang's stack** | refused (three different numbers, panel 107; cannot reach Windows) | neutral | — |
| **(K) no column** | its own item | neutral (Linux half unrun) | its own item; UBSan's false column is the front end's |
| **(F), (G), (H), (I)** | (F) moves nothing; (G) a per-platform premise; (H) a failed compile first; (I) no gain | (I) neutral | (I) not adopted for want of a gain |
| **(D), (L)** | not adoptable in this lane (a surface) | not judged | — |
| **section** | §1.1, §1.12, §3.1 `:653-654`, Part 2 `:630-632` | §1.11, §4.19, §3.1 `:653-654` | — |

## What the sitting measured

- **Today's C is panel 190's A-star C** (29,417 lines and 2,403 releases at
  400 returns), so the card's −42.75% describes a file nothing emits: today
  `-g` needs 2,041,923,528 to 2,046,150,600 bytes of clang's footprint at
  `-O2` and line tables 844,104,736 to 881,263,672, **−57%**; at 800,
  7,790,025,112 to 7,924,799,896 against 3,261,549,568 to 3,376,008,216. Line
  tables cost what `-g0` costs at `-O2`: all of `-g`'s excess is variable and
  type DWARF. Both words grow 3.7 to 4.0 times per doubling of N. On Linux
  arm64 the 400 shape: −56.86% by max RSS, −59.60% by the cgroup's peak.
- **The compiler's own 471 units**, replayed alone: line tables at `-O0` cost
  3.9% fewer instructions and 37% smaller objects; at `-O2`, 16% and 40%.
- **lldb at `-O0`**: line stepping identical under `-g` and line tables;
  under line tables `frame variable` says *no variable information is
  available*; under `-g0` a breakpoint never resolves. **At `-O2` under `-g`**
  the locals are already undeclared and only parameters show (one program,
  the compiler-engineer's second probe), and the ffi-pragmatist saw `-g` read
  a value falsely. So what (C) gives up at `-O2` is parameters' values.
- **Defect 219 is not a flag question.** LLVM's DWARF writer builds the
  retained types before any function and meets the chain's top first, through
  the casts of the element descriptor (`retainedTypes = {R4999 *, const R4999 *,
  uint64_t}`); the backend dies, on this Mac at `-c -g` (exit 139 in-process)
  and on Linux. Writing `typeorder.hero`'s touch as a cast, `(void)((T *)at +
  1);`, makes the writer meet the chain from its bottom (42 retained types
  opening `R0*, R1*, R2*`, on three clangs), and v5000, v10000 and o5000 build
  under `-g` on this Mac and on Linux; Ubuntu clang 18.1.3, the CI's Linux
  clang, dies at v10000 stock and builds it under (E) (in a container, arm64).
  The critic's shapes beside (E): a top-type constant, a module boundary,
  records by value 15,000 deep all build under (E); the extern-records shape at
  15,000 dies under every word, not a debug-info death, and (E) leaves header
  records alone by construction (`typeorder.hero:114`, `:145`).
- **What no route reaches**: the options shape costs clang super-quadratically
  in its front end (×7.59 per doubling, debug info 1.0% of it), so o10000 is
  about an hour of clang on this Mac, and **Windows' clang dies at o10000 in
  its front end under every word**, `-Wstack-exhausted` on a struct copy.
- **Sanitizers**: ASan's and UBSan's locations identical under `-g` and line
  tables on this Mac and Linux, through the compiler's own path; under `-g0`
  ASan loses file and line everywhere and Linux names a false file. **Windows**
  (clang 23.1.1, the compiler's words, `-O0`): line tables keep every function,
  every line row (the extra rows under `-g` repeats) and `llvm-symbolizer`'s
  file and line, and drop every local; the `.pdb` is still written; lldb.exe
  cannot start on the box (*python3.dll*), so no debugger ran there.
- **The stack guard** names its function under every word, `-g0` included:
  `flags.hero:174`'s *because of `-g`* is false as a cause.

## Disagreements, stated plainly

- **(B) or (C).** Defect 322's own route is (B). Both seats object: (B)
  removes the variables at `-O0`, where `heroes build` and `heroes test` build
  and authors debug, and M-typed-inspection measured that half of it works
  today under `-g`; (C) takes the same −57% where the cost is (`heroes run`
  builds at `-O2`, and every `run` golden's `-O2` leg) and gives up only
  parameters' values, which `-O2` already makes unreliable. No seat argued for
  (B) once (C) was built.
- **(I).** The compiler-engineer refuses it, the ffi-pragmatist calls it
  neutral; the probe instructions measured within the spread: not adopted for
  want of a gain, not for a harm.
- **(E)'s numbers.** The ffi-pragmatist measured a file-scope declaration
  over extern records, not the adopted cast; its numbers are not cited for
  (E), and its condition (*Heroes types only*) holds by construction.
- **(K).** Neither seat would land it here; the critic measured that UBSan's
  false column comes from the front end and survives (K); its own item.

## The resolution — ratified by the author (below)

The most robust and complete route at every disagreement (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, (E): the type chain reaches the debug writer from its bottom.** The
   touch `typeorder.hero`'s `in_order` emits for a chain past 32 deep is
   written as a cast, so the retained types the DWARF writer builds first open
   at the chain's bottom; Heroes types only, never a header's record (as
   today, `:114`, `:145`). A **mechanism test** goes red if it is reverted:
   `-g -emit-llvm -S` of a unit more than 32 deep, its first retained type the
   chain's bottom, on the platform's own clang (under a second; the 32- and
   100-deep goldens build without (E) on every clang measured, so nothing else
   would see a revert).
2. **R2, (C): the debug word follows the level.** `-g` leaves `flags()`;
   `-g` at `-O0`, `-gline-tables-only` at `-O2`, in each unit's words, the
   runtime object's key (`build_words`), the identity probe and the link line
   (the Windows driver writes the `.pdb` only when the link line carries a
   `-g`-family word). `-O2`'s help line stays *compile at -O2* (the critic's
   condition 1). A **location test** goes red if the word is ever `-g0`: one
   ASan `run` case whose needle names its `.hero` file and line, under
   `--sanitize` at `-O2` (an ASan case, since UBSan keeps its location under
   `-g0`); and one check that an `-O0` unit carries a local's variable DWARF,
   by `llvm-dwarfdump` where it is installed and skipped by name where it is
   not (M-typed-inspection's step 1, its smallest form, pulled forward).
3. **R3, refused**: (B) for the `-O0` variables; (J) on the ffi-pragmatist's
   veto (a lost and a false report location); (A), three stack numbers, the
   shape panel 107 refused, and no reach on Windows; (F) moves nothing; (G) on
   a per-platform depth premise the deaths show false; (H), a failed compile
   first and debug information that depends on the machine; (I) for want of a
   gain; (D) and (L) are surfaces this lane cannot adopt.
4. **R4, the documents that move**, in their own commit citing this sitting
   (the critic's condition 2): design.md Part 2 `:630-632` and Part 8 `:3682`
   (variables in lldb at an `-O0` build; line tables at `-O2`);
   `M-typed-inspection.md:20` and `:55` (its suite builds at `-O0`; the
   falsifier reads *`-O0`'s `-g` deleted*); `M-vscode-extension.md:16-29`
   (the debug launch builds at `-O0`); a dated correction appended to
   `tests/golden/run/fixedbugs-170-variants-and-options-100-deep-build.hero`,
   whose header says the debug information dies at 5,000; one sentence in
   `.claude/rules/generated-c.md` § Flags; `flags.hero:174`'s comment
   (the guard names its function whatever the word).
5. **R5, the landing is rebuilt on its own trunk.** Batch 14's lanes cli and
   emit edit `toolchain.hero`, `compiling.hero`, `link.hero` and
   `typeorder.hero` while this sat (the critic read `toolchain.hero` at 313
   uncommitted in lane cli), so every line count above is `dad2da47`'s: the
   landing is built after those lanes merge, re-counted against the module
   ceiling (`toolchain.hero` was at exactly 300), its seed regenerated and its
   fixpoint by `cmp`, the `selfhost/emit/**` row's suites and `cache` alone
   run, the two emissions re-blessed.
6. **R6, platforms.** The CI's four legs (Linux x86-64, Linux arm64, macOS,
   Windows) run R1's and R2's tests; on the Windows box, a unit at `-O2` under
   line tables (the `.pdb`'s line rows) and the 400-return shape's memory; an
   lldb run in the Docker image, which has `lldb-22`. Each a single case.
7. **R7, the cards.** Defect 219 closes on rows, never on *fixed*: Mac, v up
   to 10,000 and o5000 build, o10000 about an hour of front end; Linux, both
   shapes to 10,000; Windows, v10000 and o5000 build (stock too), o10000 dies
   under every word. Defect 322 closes for its constant factor, and **its
   growth, ×3.85 per doubling under both words, is filed as its own
   improvement**, so it is not recorded nowhere. Filed beside, as
   improvements: the Windows front-end death at o10000 (the box's phase split
   owed); the options shape's super-quadratic front-end cost; (K); and `step`
   into a Heroes function landing on the generated C (design.md `:682`'s
   attribution, which an author still sees as C).
8. **R8, defect 335** owes this route nothing; any lldb test the landing adds
   must not bless 335's wrong line (the critic's item 10).

**The conservative alternative, the author's to choose instead**: R1 alone,
(E) for defect 219, and `-g` left on every build, defect 322 staying open at
its measured −57%; the debugger unchanged everywhere, clang's `-O2` memory
unchanged.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the landing of R1 and R2, `emission` reads 969 passed and 2 failed before the re-bless (the two files with the touch, line counts unchanged), every other gate suite 0 failed with no re-bless, and on this Mac `heroes run` of the 800-return shape peaks at ≤ 3.5 GB of clang's footprint against 7.79–7.92 GB | the landing |
| ffi-pragmatist | on the box, `llvm-pdbutil` lists `S_LOCAL px` under `-g` and none under line tables, the same line rows under both, and line tables still produce a `.pdb` | **scored at 14:24**: (1) held; (2) held for the lines, not word for word (113 against 100 rows, the difference repeats only); (3) held |

## Author's verdict

**RATIFIED, 2026-10-07**, R1 to R8 as written above, the author answering
through the question widget after 17:21 by the clock read before the synthesis
was written, choosing *ratify* over the conservative alternative (R1 alone) and
over *I want to read it first*, on the coordinator's summary of the route;
recorded as a reading (CLAUDE.md § 4). The ratification issue is
`issues/2026-10/07/2026-10-07-1721-panel-197-ratify-amend-or-overturn-r1-to-r8-the-debug-word-follows.md`.
The author may overturn it (CLAUDE.md § 4).
