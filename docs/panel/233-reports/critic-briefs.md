# Panel 233, the completeness critic's first pass: the briefs

Written as it went, 2026-11-01, in `.claude/worktrees/233-critic/` (a detached checkout of
`0ee14c9be`). Every fact of `00-shared.md`, `compiler-engineer.md` and `ffi-pragmatist.md` below is
marked **verified** (with the command and its output), **false** (with what is true) or
**unverifiable** (and why). No wall clock was read: clang's work is counted in instructions retired
only, `/usr/bin/time -l clang -fintegrated-cc1 ...`. Every file this pass made is under the worktree's
`critic/` (units, probes, the `/bin/sh` wrapper `critic/scratch/ir.sh`, the option lists); for part of
the pass those helpers stood in the session's scratchpad outside the repository's root, against this
sitting's rule, and were moved into `critic/scratch/` before the pass ended, the scratchpad left empty.
Two throwaway `docker run --rm --network none` containers of `heroes-linux-arm64:latest` were run
(§ 5, § 13).

## 1. Paths, hashes, line counts: verified

- **The frozen commit** `0ee14c9bec8ca005fe103e4e71fed6dec0b54f57`: verified, `git rev-parse 0ee14c9be`
  prints it, and `git worktree list` shows `233-critic`, `233-compiler-engineer` and
  `233-ffi-pragmatist` each `0ee14c9be (detached HEAD)`, `git status --porcelain` empty in each.
- **Every line count the briefs give**: verified, `wc -l` at the frozen commit: `aggregate.hero` 351,
  `construct.hero` 278, `byte_copy.hero` 120, `typeorder.hero` 409, `signature.hero` 257, `ctype.hero`
  548, `cli/flags.hero` 584; `repro/bigframe18-unit.c` 569.
- **`IN_ORDER_FROM` is 32**: verified, `selfhost/emit/typeorder.hero:146-147`; `HERO_COPY` and
  `HERO_COPY_HELD` are written only where `deepest(...) > IN_ORDER_FROM` (`typeorder.hero:115-124`).
  `grep -c HERO_COPY repro/bigframe18-unit.c` prints `0`.
- **`runtime/heroes_runtime.h:269-272`**: verified: lines 269-270 end the comment on
  `hero_slot_escape`, 271 declares `hero_str_release_at` and 272 `hero_slot_escape`. **`runtime/parts/str.c:226`** is the identity, `void
  *hero_slot_escape(void *slot) { return slot; }`: verified. The premise *clang cannot see into* it is
  the header's own (lines 267-268: *LTO or a unity build would undo it*); the build's words carry no
  `-flto` (read from `build/words/*.rsp`, below).
- **`runtime/heroes_runtime.h:408`**: verified, `int main(int argc, char **argv)
  __attribute__((__optnone__, __noinline__));`. *Since defect 2253*: verified by its comment, line 389,
  *THE SHIM IS COMPILED AS WRITTEN (defect 2253, 2026-10-29)*.
- **compiler-engineer's `:396-408`**: true lines, but the comment they end opens at **line 389**;
  396 is mid-sentence. Correction: cite `runtime/heroes_runtime.h:389-408`.
- **The reproducer**: verified by reading `repro/bigframe12.hero` and `diff bigframe12.hero
  bigframe14.hero`: `R0` two `i64`, `R_k` two `R_(k-1)`, `make_k(n)` returns `R_k(left:
  make_(k-1)(n), right: make_(k-1)(n + 1))`; `R18` is 2^18 x 16 = 4,194,304 bytes = 4 MiB
  (arithmetic). "2^k words of 16 bytes" means 2^k `R0`s; a word is 8 bytes, so "2^(k+1) words of 8
  bytes" is the plain reading.
- **`repro/bigframe18-unit.c` is the unit today's compiler writes**: verified. Stage 2 built in this
  worktree, `./heroes build bigframe18.hero -O0 -o bf18-O0`, wrote
  `critic/build/tu-6df9b5140f80492b/bigframe18.c`, and `diff -q` against the repro unit prints nothing
  (byte-identical; the unit's text does not depend on the level).

## 2. Which compiler is "today's compiler": the setup rule is incomplete

- **`sources 2764fc8525370b80` is STAGE 2, not the compiler the setup rule builds.** Measured here:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes; ./heroes --version` prints `sources
  1831f7e156474293`; `./heroes build selfhost/main.hero -o heroes-next; ./heroes-next --version`
  prints `sources 2764fc8525370b80`. The seed was last written at `cd47d9b85` (2026-10-30) and
  `git log cd47d9b85..HEAD -- selfhost runtime` lists six commits after it; `git diff --stat cd47d9b85
  HEAD -- selfhost runtime` moves `selfhost/check/label_order.hero`, `labels_in_place.hero`,
  `walk.hero` (defect 2275's checker repair and the label rule) and `runtime/parts/map.c`,
  `runtime/parts/stack.c`. No emitter module moved, and the two stages' `--emit-c` of all four
  reproducers are byte-identical (`diff s1-bigframe$k.c s2-bigframe$k.c`, exit 0 for k = 12..18).
  **Correction for `00-shared.md`'s first rule**: "build stage 2 always, even without an edit to
  `selfhost/`; the seed's compiler is `1831f7e156474293`, six commits behind, checker-only"; and for
  `compiler-engineer.md`, "the frozen one's" means the frozen tree's stage 2, so that the checker's two
  repairs are not counted against the route.
- **"(seconds)" and "(about a minute)"**: durations; unverifiable under this sitting's no-wall-clock
  rule. Both builds finished here under `timeout`.

## 3. The measured command is not the build's compile

- **The build's `-O2` unit compile carries `-gline-tables-only`; the brief's command does not.** Read
  from the response file a stage 2 `-O2` build wrote (`./heroes-next build bigframe12.hero -O2`,
  `critic/build/words/words-119b1b1d0cb62a33.rsp`): `-std=gnu11 -D_USE_MATH_DEFINES
  -D_CRT_SECURE_NO_WARNINGS -Wall` and eight `-Werror=` words, `-fno-strict-aliasing
  -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion
  -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O2 -gline-tables-only -I <runtime>
  -I .`, then (`selfhost/cli/units.hero:204-208`) `-c -MD -MT dep -MF <d> -fno-crash-diagnostics
  <unit>.c -o <o> -Xclang -object-file-name=<o>`. On Linux `frame_words()` adds
  `-fstack-clash-protection` (`cli/flags.hero:198-202`, defect 2253), which changes how a 4 MiB frame
  is laid out and is absent from every number in the brief. Whether `-gline-tables-only` moves the
  count is measured in § 4.
- **`-O2` is the level of `heroes run`**: verified, `./heroes-next help build`: `build` says `-O0
  (the default here)`, `run` says `-O2 (the default here)`.

## 4. Instructions retired, re-run here

Every reading: `/usr/bin/time -l clang <flags> -c <unit>.c -o /dev/null` from the worktree's root, the
`instructions retired` and `peak memory footprint` lines (a `/bin/sh` wrapper, so the flags split as
words; zsh does not split an unquoted variable, which cost this pass one false reading of 77 million).
Units: `critic/units/u16.c` (from stage 2's build directory) and `u18.c` (the repro unit, identical to
stage 2's). Apple clang 21.0.0 (clang-2100.3.34.2), Apple M3, 24 GiB (`sysctl hw.memsize`
25769803776), 4 + 4 cores.

| reading | instructions retired | peak footprint (bytes) |
|---|---:|---:|
| k=16, brief's flags, **no** `-fintegrated-cc1` | 75,868,908 | 2,867,584 |
| k=16, brief's flags, `-fintegrated-cc1`, alone | 62,554,170,095 | 2,776,664,440 |
| k=16, the same, alone, second run | 62,539,351,926 | 2,776,795,488 |
| k=16, the same, two at once (A) | 62,961,023,427 | 2,774,894,896 |
| k=16, the same, two at once (B) | 62,573,835,385 | 2,776,795,464 |
| k=16, **the build's own words** (`-gline-tables-only`, `-Wall`, the `-D`/`-W` words, `-I critic`) | 65,669,753,280 | 2,778,663,240 |
| k=18, brief's flags, `-fintegrated-cc1`, alone | 297,180,947,185 | 12,286,501,936 |

- **"`-fintegrated-cc1` is required ... `time -l` counts the driver alone"**: verified in kind, the
  driver alone reads 75,868,908 here (brief: 89,153,154; a driver's count is not a stable quantity,
  15% apart between two runs).
- **"63,059,557,301 at k = 16"**: reproduced to 0.8% (62,554,170,095 and 62,539,351,926 alone).
- **"306,941,806,654 at k = 18"**: reproduced to **3.2%** (297,180,947,185), a wider gap than any seen at
  k=16. **"x4.87 for a record x4"**: this pass reads x4.75 (297.18 / 62.55). Superlinear either way.
- **"instructions retired, which a parallel job does not move"**: true only to about a percent. Two
  alone runs agree to 0.02%; under one parallel job one of the pair read **+0.65%**. Correction: "moves
  by under 1% under one parallel job (measured), so a before/after difference under about 3% is not a
  finding."
- **The brief's command under-counts the build's compile by 5.0% at k=16** (62.55 against 65.67
  billion). The seats should measure with the build's words (§ 3), or say they did not.
- **MISSING from every brief: clang's memory.** k=16 peaks at **2.78 GB** and k=18 at **12.29 GB**,
  half this Mac's 24 GiB. Two seats compiling k=18 at once, plus a critic, is about 37 GB on a 24 GiB
  machine: instructions retired stay true under swapping, but a run can be killed by the system or
  take the machine down for every other session. The briefs should (a) say "one k=18 compile at a
  time on this Mac", and (b) add peak footprint to what a route is judged by: a route that brings the
  instructions down and leaves 12 GB is not robust on a smaller machine. How much memory the CI's
  legs have (`ci.yml:189`, `runs-on: ${{ matrix.os }}`, the labels `ubuntu-26.04`, `ubuntu-26.04-arm`,
  `macos-26`, `windows-latest`, `ci.yml:328`) is a question this pass did not settle: not read, not run.

## 5. "No option bounding SROA's work": the search, re-run, and what it missed

- **The words searched.** The brief's sentence rests on the word `sroa`. Re-run here: `clang -cc1 -mllvm
  -help-hidden` (2,255 lines) and `clang -mllvm -help-list-hidden -fsyntax-only -x c /dev/null` (2,239
  lines); `grep -i sroa` finds one option in each, `--sroa-skip-mem2reg`: **verified**. `grep -i -E
  'alloca|slice|scalar-repl|aggregate'` on the driver's list finds nothing. **The driver form as the
  brief spells it, `clang -mllvm -help-list-hidden`, exits 1 with `clang: error: no input files`**; it
  needs an input (`-fsyntax-only -x c /dev/null`).
- **What the word missed on this Mac**: optimizer-wide gates that DO bound the work, `--opt-bisect-limit`
  (and `--print-pass-numbers`), and `--inline-threshold=<int>` (default 225). Neither names SROA. The
  first is a debugging gate over the whole module's pass sequence; the second is the one that matters,
  below.
- **`--sroa-skip-mem2reg`, the option the brief found, was never measured.** At k=16 under `-O2 -mllvm
  -sroa-skip-mem2reg`: **106,225,492,142** instructions, peak 4,319,432,136 bytes, **70% worse**.
- **"Whether Linux's clang ... offer one is unrun"**: runnable here for the arm64 image, and now run.
  `docker images` lists `heroes-linux-arm64:latest`; `docker run --rm --network none
  heroes-linux-arm64:latest sh -c 'clang -mllvm -help-list-hidden -fsyntax-only -x c /dev/null'`
  prints `Debian clang version 22.1.8 (1~deb13u4)`, `aarch64`, 3,131 lines, and `grep -i sroa` finds
  only `--sroa-skip-mem2reg`. **It also lists `--opt-disable=<string>`, "Optimization pass(es) to
  disable (comma-separated list)", and `--opt-bisect=<string>`, which Apple clang 21.0.0 does not
  have** (`comm -23` of the two option lists). So R5's "a platform's clang has one" reads: no
  platform measured has a bound on SROA; Linux 22.1.8 can switch a pass off by name, and Apple 21.0.0
  cannot. Still unrun: the CI's Ubuntu 26.04 clang (21.1.8 on the x86-64 leg, read from
  `ci.yml:308-311`, *its runs of that day*, 2026-10-26), the CI's Windows leg (`windows-latest`,
  `ci.yml:328`, whose clang the brief does not name; 23.1.1 is the Windows BOX's) and the Windows box
  itself (23.1.1, `ci.yml:458`, `docs/design.md:2927`).
- **The version facts**: 21.1.8 for the CI's x86-64 leg is verified as a reading of `ci.yml:310` (a
  2026-10-26 log, not a run here); 22.1.8 in the arm64 image verified by the run above; 23.1.1 for
  the Windows box verified as a reading of `ci.yml:458`. **The brief omits two legs**: the CI's
  Linux arm64 leg is Ubuntu 26.04 since 2026-10-26 (`ci.yml:308`), not the Debian image, so "the
  arm64 image" and "the CI's arm64 leg" are different clangs; and the CI's Darwin leg is `macos-26` (`ci.yml:328`),
  whose Apple clang the record says is 21.0.0 by the runner-images readme (`ci.yml:219-232`, *a
  document, not a machine*), its build number unread.

## 6. A route no brief lists, measured: no inlining

The brief frames the cost as SROA's (`-ftime-report`, 96.3%). Probed here at k=16 and k=18, the
brief's flags with `-fintegrated-cc1`, instructions retired:

| reading | k=16 | k=18 |
|---|---:|---:|
| `-O2` (today) | 62,554,170,095 | 297,180,947,185 |
| `-O1` | 63,003,124,015 | (not run) |
| `-O2 -mllvm -sroa-skip-mem2reg` | 106,225,492,142 | (not run) |
| **`-O2 -fno-inline-functions`** | **690,726,229** | **1,860,071,816** |
| **`-O2`, `__attribute__((noinline))` on the 17 / 19 `make` definitions** | **691,627,405** | **1,862,050,748** |
| `-O0` | 650,804,655 | 1,969,228,996 |

(peak footprint under either no-inline form: 10.3 MB at k=16, 10.6 MB at k=18, against 2.78 GB and
12.29 GB.) The units with the attribute are `critic/units/u{16,18}-noinline.c`, made by one `sed`
adding `__attribute__((noinline))` before each `h_bigframe<k>_R<j> h_bigframe<k>_make<j>(int64_t h0_n)
{` definition and changing nothing else.

- **SROA's cost happens only after inlining.** Without inlining, `-O2` costs about what `-O0` costs,
  and 160x less than today at k=18. A route the list does not hold: **(R6) a function that builds,
  returns or holds a record past the threshold is emitted `__attribute__((noinline))`** (and/or a call
  to one is never inlined), keeping the rest of `-O2` in its body, unlike R2's `optnone`, with no
  change to any C signature or ABI (`noinline` changes no calling convention) and no heap. **Unrun**:
  the program's output under it, the emissions it moves, the shapes beside it (a large record passed
  by value and returned modified; a callback or a thunk), and whether inlining INTO such a function
  matters as well as inlining it. **The one-body shape is measured in § 7: R6 does not reach it.**
- **R2's "or at a lower level" has no per-function form in clang, and per unit `-O1` does not help.**
  `__attribute__((optimize("O1")))` gives `warning: unknown attribute 'optimize' ignored
  [-Wunknown-attributes]` on Apple clang 21 (`critic/scratch/optattr.c`), and this build treats a clang
  warning on a correct program as `blocking`. `-O1` measured the same as `-O2` (63.0 against 62.6
  billion at k=16). So R2's lower levels are `optnone` per function (or `#pragma clang optimize off`,
  the same attribute) or `-O0` for a unit of its own.
- **`-ftime-report`'s 96.3% for `SROAPass`**: a timing, so **unverifiable under this sitting's rule**.
  The instruction probes agree with the share and change what it means: SROA does the work, the
  inliner makes it.

## 7. The defect as filed, against the brief's summary of it

`gh issue view 2278 --repo heroes-lang/heroes` (read 2026-11-01): open, labels `kind:defect`,
`class:adjacent`, `area:emit`, filed 2026-10-30.

- **`adjacent`, the 128 MiB eight-way record past `timeout 600`, "builds and runs at `-O0`" (as the
  filing says it), `-O2` the level of `heroes run`**: verified as readings of the filing.
- **The filing has FOUR shapes; the brief hands the seats ONE.** The filing's *What happens*: (1)
  panel 232's `bigframe18.hero`, *clang's `-cc1` still running after 9 min 13 s when stopped*; (2) *a
  record of 128 MiB of eight-way records built in one function*, `timeout 600`; (3) *the same passed by
  value to another module's function*, `timeout 120`; (4) *the same built level by level in a module
  of its own, with one call per level or eight*, `timeout 60`. The brief names (2) and reproduces (1)
  only. Correction: the shared brief should list all four and say which the reproducer stands for.
- **The brief's reproducer does not reproduce the filing's (1).** The filing: panel 232's
  `bigframe18.hero` still in `-cc1` after 9 min 13 s. The coordinator's reconstruction (*written again
  from its compiler seat's words*, its own first line): 297,180,947,185 instructions here, finished,
  and the coordinator's own 25.75 s; and panel 232's compiler seat's own table has `bigframe18` at
  `-O2` printing `before`, `1`, exit 0 (`docs/panel/232-reports/compiler-engineer.md:347`). Either the
  two programs differ, or the filing's reading was taken
  on a machine short of memory: k=18 needs 12.29 GB (§ 4), and *no clock was started, so every figure is
  a bound or a `ps` reading* (the filing's Origin). The brief does not mention the gap; it should,
  since the reconstruction is the only program the seats get.
- **The filing's shape (2), built in one function, is NOT the reproducer's mechanism, measured.** A
  probe of the shape (`critic/onefn/onefn{4,5,6}.hero`: `E0` one `i64`, `E_k` eight `E_(k-1)`, `main`
  building `v_k = E_k(a: v_(k-1), ..., h: v_(k-1))` level by level; `heroes check` exit 0), its unit
  from a stage 2 `-O0` build, the brief's flags with `-fintegrated-cc1`:

  | `E_k` (bytes) | `-O2` | `-O2 -fno-inline-functions` | `-O0` | `-O2` peak |
  |---|---:|---:|---:|---:|
  | E4 (32 KiB) | 834,692,942 | | | 27,705,776 |
  | E5 (256 KiB) | 5,442,324,881 | | | 158,843,360 |
  | E6 (2 MiB) | 42,293,644,688 | **42,280,701,650** | 715,198,592 | 1,206,813,968 |

  At 2 MiB, one function and no call, `-O2` costs 59x `-O0`, and removing inlining changes nothing. So
  § 6's route (R6, no inlining) answers the reproducer and NOT the filing's headline shape; the
  emitted body copies every argument into a temporary before the compound literal (`t31 = h3_v3; ...
  t38 = h3_v3; t39 = (h_onefn4_E4){.f_a = t31, ...}; h4_v4 = t39;`), so one level's frame holds about
  nine copies of the level below. By ratio alone (x7.8 instructions and x7.6 peak per x8 bytes, E5 to
  E6), 128 MiB would be about 70 GB of clang's memory: **an inference, unrun**, but it would explain
  "does not finish" on a 24 GiB machine better than instructions do.
- **"builds and runs at `-O0`" is FALSE for the brief's own reproducer at k = 16 and 18 on this Mac.**
  Built with stage 2 here and run under `timeout 60`, output through `head -c`:

  | k | `-O0` run | `-O2` run |
  |---|---|---|
  | 12 | `before`, `1`, exit 0 | `before`, `1`, exit 0 |
  | 14 | `before`, `1`, exit 0 | `before`, `1`, exit 0 |
  | 16 | `before`, **`panic: stack exhausted in bigframe16.make14`, exit 134** | `before`, `1`, exit 0 |
  | 18 | **`panic: stack exhausted in bigframe18.main`, exit 134** (nothing printed) | `before`, `1`, exit 0 (after a build of about 297 billion instructions) |

  So at k = 16 and 18 the program runs ONLY at `-O2`, the level whose clang is slow. Three consequences
  the briefs miss:
  1. **compiler-engineer.md's criterion "the program's output at `-O0` and `-O2` identical before and
     after" cannot be met** at k = 16 or 18: the two levels differ today. Correction: "each level's
     output unchanged by the route, and k = 16 and 18 still running at `-O2`".
  2. **Any route that makes the `-O2` frame look like `-O0`'s can turn today's slow but correct run
     into a stack abort**: R2's `optnone` on the function holding the record (k=16's `-O0` dies in
     `make14`'s frame), R1's copies through memory the optimiser cannot remove, and possibly R6 (the
     frames of `noinline` functions are unrun). That is design.md §1.12 ground, the "other face" the
     compiler-engineer brief names in its last line, and it should be a criterion, not an aside.
  3. **Q1's premise, "a cost that stops a needed program from running at all"**: on Darwin and Linux
     (8 MiB `main` stacks, `fixedbugs-2253-held.h:1-2`) the reproducer past k = 14 runs at `-O2`
     alone; Windows' 64 MiB (the same header) is unrun. The question "is this program needed" should
     be asked with that fact in front of it.

## 8. The baseline compiler, and "the batch did not make this"

- **`afe983265`**: verified as M-compatibility-promise step 3 (2026-10-29 19:57), an ancestor of the
  frozen commit (`git merge-base --is-ancestor`), and older than every defect repair of that
  milestone: `git log --oneline m-generated-programs..afe983265` holds no `Defect` commit, and
  `afe983265..29715956f` (the batch's gate) holds 19, defect 2253's `0c2a2f96a` among them.
- **`afe983265`'s stage 3 is `sources 3e3c95dc246b73b3`**: verified. `git archive afe983265 -- seed
  runtime selfhost` into `critic/afe/`, its seed with clang (stage 1, `638149f67ad4d782`), stage 2
  (`00577d2a5957da82`), stage 3 (`3e3c95dc246b73b3`). Stage 2 and stage 3 print DIFFERENT keys there,
  so "stage 3" in the brief is load-bearing. At the frozen commit stage 2 and stage 3 both print
  `2764fc8525370b80` (measured).
- **The whole-build seconds (0.91 ... 25.75 and 0.93 ... 24.20) and `-O0`'s 0.63 s**: wall clock;
  unverifiable under this sitting's rule.
- **The conclusion, re-measured in instructions**: `afe983265`'s stage 3 writes the k=16 and k=18
  units with exactly two lines different from today's, `_Static_assert(HERO_RUNTIME_ABI == 31` against
  `32` and the `hero_program_sources` key (`diff`). Its k=16 unit under ITS OWN runtime header (no
  `optnone` on `main`, `grep -c optnone` prints 0): **63,089,236,871** instructions against today's
  62,554,170,095 and 62,539,351,926, within 1%. **Verified**: the batch, defect 2253's `main` among it,
  did not make this cost.

## 9. The prior art, read again

- **"which took `MemCpyOpt` on a thousand-deep chain from 139 to 16 s (defect 566)"**: the numbers are
  `byte_copy.hero:47-49`'s, with two corrections. They are the **Windows box's clang 23**, not this
  Mac's; and 139 and 16 s are whole compiles at 1,000 levels (*4, 27 and 139 s at 250, 500 and 1,000
  levels became 4, 4 and 16*), while `MemCpyOpt`'s share was a separate reading, *120.6 of 129 s*.
- **MISSING, and it bears on R1**: the escape is used only for COUNTED structs, and that same comment
  measured it as a LOSS on a struct holding nothing counted, which is the reproducer's case (only
  `i64`): *A struct holding nothing counted keeps the plain copy, which clang promotes to registers:
  an `i64` chain 7,000 deep took 28 s there plain and 48 s through the identity*
  (`byte_copy.hero:50-52`). R1, "`HERO_COPY_HELD`'s shape keyed on size", should be framed with its
  own module's counter-measurement in front of it.
- **MISSING, and it bears on R1 too**: `HERO_COPY` covers a source that has an address, *a place or
  a compound literal*; *a call's result has none and stays an assignment* (`byte_copy.hero:22-24`).
  The reproducer's large copies are mostly call results (`t2 = h_bigframe18_make17(t1); ... t6 = ...`),
  so R1 as the prior art defines it does not reach them without a new shape (an out-parameter).
- **"a front-end cost cubic in a chain's DEPTH" (defects 469 and 522)**: verified,
  `byte_copy.hero:9-14`.
- **"Defect 2278's cost is the WIDTH of one record, not a depth"**: a framing claim no measurement in
  the brief separates. In the reproducer, depth and bytes move together (bytes = 16 x 2^k), so it cannot
  tell them apart. The eight-way probe of § 7 is depth 6 at 2 MiB and still costs 42 billion; a family
  that holds the bytes and varies the depth (two-way against eight-way at one size) is the instrument.

## 10. The lane, and what it cannot adopt

- **Soundness lane**: verified as the skill's (`.claude/skills/panel/SKILL.md:18-23`): *Use it when the
  proposal changes no surface, no diagnostic and no spec token*, and *When in doubt take the full
  panel ... A question that looks internal but has a sentence in the spec behind it is a full panel*
  (`:29-34`). Decision 2283's (A) carries the same condition: *no surface, no diagnostic and no
  specification token move IF the repair is the emitter's*.
- **The brief leaves two of its five routes outside the lane's reach and does not say so.** R4 is *a
  refusal with a diagnostic past a size*: a diagnostic class, so a full panel (CLAUDE.md § 4). R3, records
  kept in the heap behind a reference, has a spec sentence behind it: *A record or variant holds its
  fields **by value*** (`spec/heroes-spec.md:81`), and Q1's "a refusal the language should name" is
  R4 again. Correction: the shared brief should say that if the sitting finds R3 or R4 the most robust,
  it records that and re-convenes the full panel; it cannot adopt either in this lane.
- **The milestone's one panel gate**: verified, the author's answer on 2283 (2026-11-01, `MEMBER`
  comment): *the answer standing as this milestone's one panel gate*.
- **"`blocking` at its second tag"**: consistent with the rule (`.claude/rules/verification.md`,
  Bounded discovery) and the tags: filed 2026-10-30 00:44 UTC; `m-compatibility-promise` placed
  2026-10-30 12:12 +0100 (`git for-each-ref refs/tags/m-*`); `m-latency-ceiling` would be the second,
  and `git tag -l m-latency-ceiling` prints nothing.

## 11. The compiler-engineer's brief

- **Module descriptions**: verified against each file's first lines (`aggregate.hero` *Records as C:
  construction, field reads*; `construct.hero` *A composite value coming into existence*;
  `signature.hero` *A function's C signature*; `cli/flags.hero` *What the generated C is compiled
  UNDER*).
- **"the panel skill, § 3c: a route that does not build is not adopted"**: verified,
  `SKILL.md:276`.
- **design.md §1.12** exists (`docs/design.md:590`, *Robustness, and a boundary that is complete and
  defended*); defect 2253 is a frame larger than the stack (`tests/golden/run/fixedbugs-2253-*`).
- **The emission count's method conflicts with CL-074 and skips three of the net's groups.** The brief
  asks for `heroes build <program> --emit-c` against the frozen compiler over `examples/` and
  `tests/golden/run/`. CL-074 (`docs/records/contract/case-law.md:1640`): *a claim about a gate is
  produced with the gate's own invocation*. The net's `emission` suite already walks `tests/emission`
  (728 blessed files, `git ls-files tests/emission | wc -l`), and the groups `emit`, `fixedbugs`, `ir`, `run`
  plus `examples` (`tests/harness/suite_emission.hero:64,97-100`), and the map
  (`.claude/rules/verification.md`) sends a `selfhost/emit/**` change to `emission`, `emit`,
  `determinism`, `wholes`, `descriptors` and `lldb`. Correction: "count the route's changed emissions
  with `./heroes run tests/harness/main.hero -- <your compiler> emission` and `... emit`, each red line
  one blessed file the route moves", the hand loop at most a supplement. Two more facts about the hand
  loop: `--emit-c` prints the WHOLE program as one C text (752 lines for k=18), not the unit the build
  compiles (569); and `--emit-c` runs the build's clang rounds (its `build/tu-*` objects appear), so a
  program whose headers this Mac lacks cannot be counted that way. Sizes: `tests/golden/run/` holds
  633 `.hero` files, `examples/` 55 directories.
- **"the program's output at `-O0` and `-O2` identical before and after"**: cannot hold at k = 16 or
  18 (§ 7).

## 12. The ffi-pragmatist's brief

- **"A group's `record` is the header's struct (spec § 13)"**: verified, `spec/heroes-spec.md:386`,
  § 13 FFI.
- **The carried claim**: the filing says it (its Origin: *the cases hold a group record named
  `partial` instead (one `i64` named, 128 MiB in its C struct, which compiled at `-O2` within every
  bound)*), and the header exists: `tests/golden/run/fixedbugs-2253-held.h:12-15`, `int64_t first;
  int64_t rest[(1 << 24) - 1];`, 2^24 x 8 = 128 MiB. **Measured here**: that case's unit at `-O2` costs
  **191,127,614** instructions, peak 9,568,664 bytes. Two corrections to the brief's words: the record is
  named `Held` (`record Held tag heroes_held partial`); `partial` is the modifier that names only
  some of the fields.
- **"If true, the cost follows the number of fields a record names to clang rather than its bytes"**:
  an inference, and three things change at once between the two programs, not one: (a) 2^19 named
  scalars against one named `i64` plus one ARRAY member; (b) the 2253 value is handed to C by value and
  read through a `volatile` function pointer (`fixedbugs-2253-held.h:17-20`), so its address
  escapes, which is R1's mechanism; (c) no chain of calls to inline, while the reproducer's cost
  vanishes without inlining (§ 6) and its 2^19 named fields then cost 1.86 billion. The brief's proposed
  experiment ("many named fields against one of the same size named `partial`") moves (a) and keeps
  (b) and (c) undefined. Correction: hold the construction and the escape fixed and vary only the
  field shape, inside a Heroes record built the reproducer's way and the one-function way.

## 13. Q3's instruments, and Linux's

- **Q3 asks for a threshold in bytes, scalar fields, or depth times width.** Searched here: function
  names containing `sizeof`, `size_of`, `byte`, `bytes`, `leaves`, `scalars`, `flat`, `words_of` in all of
  `selfhost/`, and `size` in `selfhost/ir/layout.hero`: **no function gives a Heroes record's byte size
  or count of scalar leaves** (a question rather than a premise, resting on those words). Depth is
  computed today in three places: `cli/deep_types.hero:65`, `emit/zeroed.hero:123`,
  `emit/typeorder.hero:209`. So a threshold in bytes or fields is new code, or a clang probe, and the
  brief should price that; "the program in the tree nearest to it on each side" has no instrument named
  either.
- **Linux has no instrument in this sitting.** `docker run --rm --network none heroes-linux-arm64:latest
  sh -c 'ls /usr/bin/time; command -v perf'`: `ls: cannot access '/usr/bin/time'`, `no perf`.
  Instructions retired cannot be read there, and the wall clock is refused, so a route's cost on Linux
  (with `-fstack-clash-protection`, which only Linux compiles under) is unmeasurable in this sitting
  as the rules stand. The brief should say so, or name the unit a Linux reading may use.

## 14. The rule for stopping a stray clang does not find the brief's own measuring command

`pgrep -fl <your worktree>` (`00-shared.md:24`) matches a command line that holds the worktree's path.
Measured here: (1) a `heroes build -O2` started in the background spawned the driver `clang
--rsp-quoting=posix @build/words/words-....rsp -c ... build/tu-.../bigframe16b.c`, relative paths
only, and `pgrep -fl <worktree>` did not list it; it listed only the `-cc1` child, whose expanded
arguments carry absolute paths. (2) A clang started as the brief's measuring command, `clang
-fintegrated-cc1 ... -I runtime -c critic/units/u16.c -o /dev/null`, has no child and relative paths:
**`pgrep -fl <worktree>` listed nothing** while `pgrep -fl clang` listed it. Correction: "`pgrep -fl
clang`, and for each pid `lsof -a -p <pid> -d cwd` to see whose it is; or run clang with absolute paths".
(This pass stopped its own probe with `pkill -f 'critic/units/u16.c'`; `pgrep clang` is empty after.)

## 15. Routes the brief does not list

What would have to be true for each to exist is said beside it; "measured" means here, today.

1. **(R6) No inlining of the functions that build, return or hold a record past the threshold**
   (`__attribute__((noinline))`), the rest of `-O2` kept. Measured on the reproducer: k=18 from
   297,180,947,185 to 1,862,050,748 instructions, 12.29 GB to 10.6 MB (§ 6). The ABI does not move:
   `critic/scratch/abi.c` at `-O2`, a 512-byte struct returned by a `noinline` function and by a plain one,
   both write their result through `x8` (arm64's indirect-result register), and the caller of the
   `noinline` one passes `mov x8, sp; bl _held`. **It does not touch the filing's one-function shape**
   (E6: 42.29 billion with inlining or without, § 7), and its frames at k = 16 and 18 are unrun against the
   8 MiB stack.
2. **(R7) Construct in place: a construction writes its fields straight into the destination, and a
   call returning a large record writes into its caller's slot** (an out-parameter between Heroes
   functions only; `extern` signatures stay C's). True if SROA's cost tracks the number of whole-value
   temporaries and copies it is handed: the emitted body copies each argument into a temporary and then
   a compound literal (`t31 = h3_v3; ... t38 = h3_v3; t39 = (h_onefn4_E4){...}; h4_v4 = t39;`), about nine
   copies of a level per level, and the same temporaries are what makes the `-O0` frame pass 8 MiB at
   k = 16. With R3 it is the second route that could answer BOTH faces, the compile cost and the
   frame (2253's); unlike R3 it keeps a record by value (`spec/heroes-spec.md:81`) and stays in the
   lane, and it gives a call result the address R1 needs (§ 9). **Unrun.**
3. **A driver flag every clang has, per unit**: `-fno-inline-functions`, or `-mllvm -inline-threshold=`
   (present on Apple clang 21, `--inline-threshold=<int> ... (default = 225)`). R5 as briefed searched for a
   flag that bounds SROA and found none; a flag that starves SROA of its input exists everywhere
   measured (k=16 and k=18, § 6). Same limit as route 1: not the one-function shape.
4. **`-mllvm -opt-disable=<passes>`**: exists on Debian clang 22.1.8 (the arm64 image), absent from Apple
   clang 21.0.0 (§ 5), so not portable today; named so that R5 is judged against the full list.
5. **A bigger stack for a program that holds a large frame** (a thread sized from the frame, or the
   link's stack size), the complement that would make routes 1, R1 and R2 safe at k = 16 and 18,
   whose `-O0`-shaped frames abort today. Not a route for the compile cost; the frame is the other
   half of the same defect (§ 7).
6. **A clang bounded by memory with a fallback** (the unit recompiled another way past a bound):
   true only if a machine-independent bound exists; it makes the binary depend on the machine and
   hides the cost instead of removing it. Named so it is refused with its reasons rather than not at all.
7. **A record past the threshold represented in C as one array of bytes with offset accessors**, the
   shape of `fixedbugs-2253-held.h` that compiled at 128 MiB in 191 million instructions. True only if
   the array shape, not the 2253 value's escape to C, is what made that unit cheap: § 12's control
   decides it. Group records are untouched (the header's struct).

**Questions the sitting should ask and the brief does not**: which of the filing's four shapes the
adopted route must cover, and the probe of each at a size this Mac can hold; clang's peak memory as a
criterion beside instructions (12.29 GB at k=18; 1.21 GB at 2 MiB in one function); whether the route
keeps k = 16 and 18 RUNNING at `-O2` (today they run only there); and the threshold's unit given that
only depth is computed by the compiler today.

## 16. Every word that claims a search, and what was searched

| brief, line | the words | searched here | verdict |
|---|---|---|---|
| `00-shared.md:36` | *Those programs were not committed* | `git log --all -S bigframe` (two commits, both panel 232 documents, no program); `git grep -i -E 'bigframe\|eight-way\|eight R_\|E_\(k-1\)'` (panel 232's three documents only) | verified for those words; the filing names none of its 128 MiB programs, so no search can be exhaustive |
| `00-shared.md:57-58` | *list no option bounding SROA's work; the one SROA option shown* | both help lists, `grep -i` for `sroa`, `alloca`, `slice`, `scalar-repl`, `aggregate`, `opt-bisect`, `opt-disable`, `inline` | true for the word `sroa`; misses `--opt-bisect-limit`, `--inline-threshold` and the driver's `-fno-inline-functions` (§ 5, § 6) |
| `00-shared.md:59-60` | *... offer one is unrun* | the arm64 image, run (§ 5) | now run for 22.1.8; CI legs and the Windows box still unrun |
| `00-shared.md:69-70` | *below 32 levels none of that is defined* | `typeorder.hero:115-124`; `grep -c HERO_COPY repro/bigframe18-unit.c` = 0 | verified |
| `00-shared.md:48-50` | *So the batch ... did not make this* (an inference, "So") | the two compilers' units (`diff`: two lines) and instructions (63.09 against 62.55 billion) | verified |
| `00-shared.md:78` | *every one unrun* | | self-report; R2's "lower level" is now partly run (`-O1` = `-O2`; no per-function level in clang) |
| `ffi-pragmatist.md:13` | *the coordinator did not run it* | the 2253 unit, run (§ 12) | verified, with the inference confounded |
| this pass's own | *no function gives a record's byte size or leaf count* | function names with `sizeof`, `size_of`, `byte(s)`, `leaves`, `scalars`, `flat`, `words_of`; `size` in `ir/layout.hero` | a question resting on those words (§ 13) |

## 17. The short list for the coordinator

False, or true only with a correction:
1. The setup builds the seed's compiler, `sources 1831f7e156474293`; "today's compiler",
   `2764fc8525370b80`, is stage 2 (§ 2).
2. "builds and runs at `-O0`": false for the brief's own reproducer at k = 16 and 18 on this Mac (stack
   exhausted, exit 134); they run only at `-O2` (§ 7). So the compiler-engineer's "output at `-O0` and
   `-O2` identical" criterion cannot be met (§ 11), and R2 and R1 risk turning a slow correct run into an
   abort.
3. The reproducer stands for one of the filing's four shapes, and does not reproduce the filing's own
   k=18 reading (9 min 13 s unfinished against about 297 billion instructions, finished); panel 232's own
   table has `bigframe18` at `-O2` running (§ 7).
4. The measured command is not the build's compile: no `-gline-tables-only` (5.0% fewer instructions at
   k=16), no `-fstack-clash-protection` on Linux (§ 3, § 4).
5. "a parallel job does not move" instructions: it moved one reading by 0.65%; the k=18 figure reproduced
   only to 3.2%, and x4.87 reads x4.75 here (§ 4).
6. `clang -mllvm -help-list-hidden` as spelled exits 1, *no input files* (§ 5).
7. "139 to 16 s" are the Windows box's clang 23 and whole compiles; the same comment records the
   escape as a LOSS on an `i64`-only chain, and `HERO_COPY` does not reach a call's result (§ 9).
8. `pgrep -fl <your worktree>` does not find the brief's own `-fintegrated-cc1` clang (§ 14).
9. `heroes_runtime.h:396-408` begins mid-comment: `389-408` (§ 1).
10. The 2253 record is `Held`, declared `partial`; and the field-count inference changes three things
    at once (§ 12).
11. R3 has `spec/heroes-spec.md:81` behind it and R4 is a diagnostic: neither can be adopted in the
    soundness lane (§ 10).

Unverifiable under this sitting's rules: every wall-clock figure (the whole builds, `-O0`'s 0.63 s, the
"seconds" and "about a minute" of the setup) and `-ftime-report`'s shares, though the instruction
probes agree with them; the CI's Ubuntu 26.04 clang and the Windows box's 23.1.1 for options; any cost
on Linux (no `/usr/bin/time`, no `perf` in the image).

Missing: clang's memory (2.78 GB at k=16, 12.29 GB at k=18, half of this Mac); one k=18 compile at a
time; the four shapes; no-inlining (measured) and construct-in-place (unrun) as routes.
