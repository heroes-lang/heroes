# Panel 255, the completeness critic's reading of the round-two brief, before the seats start

Written 2026-11-22 by the completeness critic (no seat, no verdict on the route). Read:
`255-briefs/round2.md` (99 lines, `wc -l`) whole; every file under `255-briefs/round2/` by `find`, `wc -l`,
`md5 -r`, `grep` and `sed`; `255-reports/critic-reports.md` findings 1 to 13 and its "What the synthesis
must settle" (lines 443 to 488); the seats' round-one reports by `grep` only. Everything I ran ran in the
brief's own folder (read only), in the tree of `lane-p255` (read only, `git log -1`: `a75179641`), in my
copy `.claude/worktrees/p255-scratch/critic/p255/r2b/` (`TMPDIR` inside it), or in one container
`crit255-r2brief-arm64` of `heroes-linux-arm64:latest` run with `--rm --network none` (`docker ps -a
--filter name=crit255` prints none after). Nothing was built.

## Findings

### 1. The paths, the file lists and the counts re-run: they hold, with four imprecisions

- `find 255-briefs/round2 -type f | xargs wc -l`: `ce-C3/` holds `sigguard.c`, `siginstall.c`,
  `sigjump.c`; `fp-C3/` holds `rebind.c`, `sigguard.c`, `entries.c`, `jumps.c`; `kC5/` holds `rebind.c`,
  `sigguard.c`; `shapes/critic/`, `shapes/ce/`, `shapes/fp/` hold every file the brief names. As listed.
- `grep -n MUST | wc -l`: `xr.h` 6, `xf.h` 1, `extra.h` 15. As stated. `grep -c MUST` reads 0 for every
  header the brief lists (13 files counting the duplicates in `fp/`), and also for `pngdemo.h` and the
  three 2522 headers, which the brief does not list (harmless).
- `md5 -r`: `fp-C3/rebind.c` `c494aba8ad64daebebaf8be68838ebe1` (the md5 critic-reports gives for FP's C3);
  `ce-C3/sigjump.c` `235884bb69d8...` (the prefix of finding 5); `extra.h` `f4f7ece3...` (finding 5);
  `critic/residue.h` = `fp/res/residue.h` = `39e24325...` (finding 4); `critic/cojump.h` =
  `fp/cojump/cojump.h`, `critic/nest.h` = `fp/nest/nest.h`. The three 2522 headers and `gcb`, `gcopy`,
  `grefill` are byte-identical to the tree's cases (`cmp` against `tests/golden/run/`, all silent).
- Imprecision a: **`fp/` holds two different `residue.h`.** `fp/residue.h` is `94d8b3ca...`, 75 lines, the
  handler calling the program directly; `fp/res/residue.h` is the 83-line one with `rs_down(12, ...)`
  (`diff`: lines 49 to 54 and 67). `fp/t_partial.hero` and `fp/res/t_partial.hero` are byte-identical
  (`cmp`), so one name builds two programs by folder, and only the `res/` one reaches past the thunk's
  walk of eight. The table's `t_partial`, `t_unseen` and "the `res` programs" must name `res/`'s.
- Imprecision b: `grep -c 'critic probe kC5'` reads `rebind.c` 5, `sigguard.c` 1: five edits and the
  `suspended` field, as critic-reports says; round2.md line 21's "five edits ... (`rebind.c`,
  `sigguard.c`)" reads as five across both.
- Imprecision c: `shapes/critic/` also holds twelve `ctx-m*.hero` (against `xc.h`; `diff` with `ce/`'s
  twin differs only in the `extern` line), `t_partial`, `t_unseen`, `t_unseen0` and `run.sh`, `run2.sh`,
  unnamed by line 22.
- Imprecision d: the `ij*` programs are not in `round2/` (`find -iname '*ij*'` prints nothing). They are
  in `docs/panel/251-reports/evidence.tar.xz` (`tar -tJf`: `251-compiler-engineer/q1/ship/ij/ijn.c`,
  `ijo.c`, `ijstale.c`, `ijv.c`), pointed at by `00-shared.md` lines 42 to 46, which is its § 2, outside
  the § 6 to § 8 round2.md tells the seats to read. Extracted into my copy: `ijn.c`'s first line is
  *"Control: the same interface, the handler calls the program back directly, no jump"*, so its MUST BE
  REFUSED is right.

### 2. The line round2.md cites, `sigjump.c:62`, is the tree's file, not the round's input

`sed -n 55,70p` of both: in the tree's `runtime/parts/sigjump.c` line 62 reads *"other save or jump
between; `makecontext`'s new stack (variadic, so not"*; in `round2/ce-C3/sigjump.c`, the file the engineer
is told to start from, line 62 reads *"Under A only: a buffer refilled ..."*, and `grep -n -i variadic`
finds the word in no round2 file (only `runtime/parts/sigjump.c:62` and `runtime/parts/rebind.c:495`). The
fact holds: `ce-C3/sigjump.c` defines `getcontext` (513), `setcontext` (546), `swapcontext` (554) and no
`makecontext`. The citation should read `runtime/parts/sigjump.c:62`. Also, rule 3's "(critic 3 and
4 ...)": finding 3 is the calibration's failure, but finding 4 is T's veto. What the parenthesis says
(*"both seats ranked A above the told refusal"*) is finding 3 and must-settle **item** 4.

### 3. The expectation table, row by row: every row matches its source but `gcopy`, which reverses an author's ruling

Checked against the MUST lines (`xr.h` 90 to 92, 124, 148, 179, 197; `xf.h` 6; `extra.h` lines 51 to 275,
mapped to `x_*` by each `.hero`'s call), finding 5's and 11's "MUST" cells, FP's report rows 343 to 351
(`jo_*`, `fake`, `ctx/`) and CE's 346 to 350 (`ij*`, `x_*`). All agree, `ctx-m1`/`m3` refused and `m2`
run included, except one row:

- **`gcopy` is listed MUST RUN.** `gcopy.hero` is byte-identical to the tree's
  `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so.hero`.
  Its `.expected` pins the refusal (`!panic: ... jumped to a buffer this runtime holds no record of ...`),
  and its comment says *"pinned as a known cost by the author's ruling B of 2026-11-20 ... the refusal
  stands"*. Critic-reports gives it no MUST anywhere (findings 3, 5, 11 report outcomes only), and
  must-settle item 8 calls its running *"ruling B's change, the author's to read"*. Making it a MUST
  turns the reversal of an author's ruling into a pass criterion of the sitting.
- `grep -l 'holds no record of' tests/golden/run/*.expected` finds exactly two goldens: that one and
  `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` (= `grefill`, `cmp`
  identical). Under the decode `grefill` is refused in S words (finding 5). So C4's case sets will show
  both goldens moving, one in verdict and one in words. The brief does not say so, nor that rewriting a
  `.expected` is not a seat's (CLAUDE.md § Hard stops).

### 4. Rule 8 cannot hold in the "C4 forced to fail" column, by measurements already on file

The report table asks for the union under *C4 forced to fail*. Rule 8 says each verdict is the shape's
stated MUST, and rule 3 makes the forced column A. A refuses, measured and written: `e` (its own header,
`xr.h:197`: *"MUST RUN; A refuses it"*), `x_heap`, `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`,
`x_refill_out` (CE's report lines 349, 460 and 461), `x_fake` under `--sanitize` (CE 350), `jo_heap`,
`jo_tls`, `jo_lib` (FP 343 to 345, 391), `fake` under `ASAN_OPTIONS=detect_stack_use_after_return=1` (FP
351) and `gcopy` (finding 5, `ceC3fail` 134 U). That is thirteen MUST RUN rows the forced column cannot
meet, while `xr.h:179` says `d_*` is *"the control a failed calibration must still follow"*. Without a
column of its own the forced run is either a certain rule-8 failure or read silently as exempt. Two
further gaps on the A path:
- **Darwin's A has no code among the inputs.** `kC5/rebind.c` keeps no record (line 120, *"no record is
  kept"*; `grep -c -i fingerprint` 2, both in the header comment at 53 to 55). FP's round-one A is
  `F/p255/A/rebind.c` (FP's report line 229); CE's A and its forced build are `CE/p255/routes/A/` and
  `Cfail/` (CE 146, 296). None is copied into `round2/`, which the brief calls the record. Only
  `ce-C3/sigjump.c:315` carries a forcing define (`HERO_JMP_FORCE_UNCALIBRATED`); `fp-C3` and `kC5` have
  none.
- **Darwin x86-64 is A by construction.** `kC5/rebind.c:376` `#if defined(__aarch64__)`, `:468`
  `static void hero_rebind_calibrate(void) {}`. So the thirteen rows above split there from Darwin arm64
  for good. The CI's Darwin leg is `macos-26` (`ci.yml:1434`); Darwin x86-64 is unrunnable here
  (`00-shared.md` § 7). Rule 8's "both platforms" does not say whether Darwin x86-64 counts.

### 5. Rule 7's premise about `uc_stack`, measured on glibc 2.41 arm64: the C library does not read it at a swap

Rule 7 offers Linux *"the target context's own `uc_stack` read at the swap (it lives: the C library reads
it next)"*. Command: in the container, `llvm-objdump -d --disassemble-symbols=getcontext,swapcontext,setcontext,makecontext`
of `/lib/aarch64-linux-gnu/libc.so.6` (`ldd`: `GLIBC 2.41-12+deb13u3`). `uc_stack` is at `+0x10` to
`+0x28`. Results: `getcontext` and `swapcontext`'s save half store at `0xb8`, `0x148` to `0x1c0`,
`0x1d0`... and nothing in `0x10` to `0x28`; `setcontext` loads from `0x148` to `0x1b8`, `0xb8` to
`0xe8`, `0x1d0`... and nothing in `0x10` to `0x28`; `swapcontext` resumes by `48e98: b 0x3ca40
<setcontext>`; only `makecontext` reads it (`ldr x0, [x0, #0x20]`, `ldp x5, x3, [x20, #0x8]`). So at a
swap the field holds whatever the program last wrote, and for a target filled by `getcontext` or
`swapcontext` the C library never writes it. The premise as worded is false at the swap. The x86-64
glibc and Darwin readings are unrun. Second measurement, same container: `llvm-readelf -r libc.so.6 |
grep -E 'setcontext|swapcontext|getcontext'` prints nothing, so no call libc makes to a context function
can reach a runtime's definition. The `uc_link` resume at a coroutine's return is therefore a
`setcontext` no wrapper sees (finding 7).

### 6. The must-settle list against C4's rules

| item | answered? |
|---|---|
| 1 shape A, what a swap proves, build both, union | **yes**: rules 5 and 8, both seats build; every shape the item names is in the table |
| 2 one design | **yes**: one rule set; rule 7 leaves the known-stack mechanism per platform (finding 7) |
| 3 the context class: file it `blocking`; does 2679 wait | **no**. `grep -n -i -E '2522\|blocking\|filing\|land'` of round2.md finds only line 24 (the headers). No filing, no destination (2522 or a defect of its own), no saying that 2679 now waits for the context design, and no seat asked to keep route C's 2679-only column, so "land C first" stays measurable on round-one data alone |
| 4 the fallback, and the runtime saying it | **partly**: rule 3 answers the fallback, the accessor and the define. The forced column has no expectations (finding 4), and nothing says whether rules 4 to 7 bind the A path. Finding 3's `a_1_0` ran under FP's failed calibration through the thunk's own-frame prune |
| 5 the new known costs | **yes**: rules 6 and 7, and the report's known-costs line |
| 6 no case switches a context; the goldens a landing owes | **no**. round2.md's one "golden" (line 52) is the accessor's; finding 8's list of cases owed on both platforms is absent; the prediction is not required to be one a CI leg can see |
| 7 the calibration's strength | **yes**: rule 2 (it names no save half of `swapcontext`, whose context's stack-pointer field rule 1 reads) |
| 8 what stands, ruling B's change the author's | **contradicted** for `gcopy` (finding 3) |

**Rules a build could satisfy while still failing a MUST, or one the brief would hold it to:**
- Rule 4 does not fix the classification order. A build that asks known stacks before the own stack
  satisfies rules 4 and 7, and on Darwin refuses `f` (`kC4`, finding 11, measured). Rule 8 catches that
  only where the table learns at `makecontext`. Under rule 7's Linux "read at the swap", `f` never swaps
  (`xf.h`: *"returns without ever swapping to it"*), so its range never enters the table and `f` passes
  under either order: the premise rule 7 asks the seat to name goes untested on Linux.
- Rule 5's text holds two of `kC5`'s three parts. `kC5` also clamps the suspension when a handler's own
  trampoline pops (`kC5/rebind.c:240`, `if (own->suspended > at) own->suspended = at`). A build of the text
  alone keeps a suspension after its handler ended (a handler resumed at a `getcontext` of its own rather
  than at the swap). No union shape exercises it.
- *"No prune drops a suspended entry"*: `kC5`'s trampoline end is `own->depth = at` (line 238), not a
  prune. It truncates every entry above `at`, suspended ones included, so a build can satisfy the
  wording and still drop a suspended handler (finding 7, shape N).
- The trampoline and the thunk each prune by their own frame (`kC5/rebind.c` lines 224 and 252,
  `hero_rebind_prune(own, me|frame, false)`, *"`w <= at`"* on the same stack). No rule of C4 states this
  prune, yet it is what runs the A family when no SEEN swap suspends the handler (finding 7).

### 7. Shapes the union lacks that C4's own rules make decisive (by reading `kC5`; none built or run)

- **A′, shape A spelled `getcontext` + `setcontext` (rule 6 makes it decisive; the dangerous
  direction).** The handler runs `getcontext(&hc)`, then `setcontext(co)` into the coroutine, and the
  coroutine's `uc_link` is `&hc` (or it ends with `setcontext(&hc)`). how 1: the coroutine calls the
  program. how 2: the handler, resumed, calls it. Two orientations: the coroutine's stack an own-stack
  array with the handler on the own stack, and a heap stack with the handler on the alternate stack.
  MUST BE REFUSED, by A's own rule (`xr.h:90` to 92). Under C4, rule 6 makes that `setcontext` a jump.
  Own-stack orientation: with the own stack asked first, the target's stack pointer lies on the "same"
  stack above the handler's entry, so `hero_rebind_prune` drops it (`w <= at`). Alternate-stack
  orientation: rule 6's second clause drops it (`by_longjmp && there == ALT && here != ALT`). The thunk
  then finds depth 0 and the program runs inside the signal. The way back through `uc_link` is a
  `setcontext` inside libc that no wrapper sees (finding 5). A `setcontext` away from a handler does not
  end the handler's frames, which survive on their own stack and are legally resumed; this is the
  premise rule 6 borrows from `siglongjmp`. `b_0` and `b_1`, the union's only `setcontext` shapes, are
  escapes that are never resumed, so the union cannot see this.
- **A by an unseen switch** (critic-reports, "Unrun here", FP's Boost.Context and libco case). The
  same as shape A with a hand-written switch. No swap is seen, so rule 5 suspends nothing, and the
  thunk's own-frame prune drops the own-stack handler's entry: it runs in `a_*_0`'s orientation.
- **G, "swap away and never resume"**: the program round2.md lines 71 to 72 ask the seats to build, given
  neither a form nor a MUST. It has three forms: the handler escapes by `swapcontext` to a pre-signal
  context (`b` spelled with `swapcontext`); the coroutine `siglongjmp`s to a pre-signal buffer; the
  coroutine returns through `uc_link` to a pre-signal context (unseen). Under rule 5 each leaves its
  suspension standing for good, and every later callback on the thread is refused. Each is a correct
  program by the reasoning that makes `b_*` MUST RUN.
- **F′ and F″, the stale table under the order rule 7 picks.** F′ is `f` with one swap into its context
  and back before the helper returns, the range a learn-at-swap table holds. F″ is a coroutine's
  `mmap`ed stack unmapped and the same range mapped again as the alternate stack, then `d_1`. With the
  own stack asked first, `f` is vacuous for the table; these are not.
- **N, nested handlers suspended and resumed out of order.** H1 swaps to K1, a second signal's H2 on K1
  swaps to K2, and K2 resumes H1, which returns. The trampoline's `depth = at` drops H2's suspended
  entry, and K2's later call of the program runs inside H2's signal. MUST BE REFUSED.
- **U, a swap target whose `uc_stack` is stale** (a reused or uninitialised `ucontext_t`, legal since the
  C library never reads the field there, finding 5). It is decisive only for rule 7's Linux route.

## Unrun here, said

Every shape of finding 7; the x86-64 glibc and Darwin readings of `uc_stack`; whether libsystem's
`uc_link` resume is a direct call (by reading only); any harness, suite or timing.

## What the coordinator must repair before the seats start

1. **`gcopy` out of MUST RUN.** It reverses ruling B and the tree's pinned golden. Report both verdicts as
   the author's question, and tell the seats that `gcopy` (verdict) and `grefill` (words) are the two
   goldens C4 moves and that neither `.expected` is theirs to rewrite.
2. **Give "C4 forced to fail" its own expectations.** Thirteen MUST RUN rows are measured refused under
   A (`e`, `x_heap`, `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`, `x_refill_out`, `x_fake` sanitized,
   `jo_heap`, `jo_tls`, `jo_lib`, `fake` with the ASan option, `gcopy`). Name what A must still follow
   (`d_*`, `gcb`); say whether rules 4 to 7 bind the A path; copy FP's `F/p255/A/` and CE's `routes/A/`,
   `Cfail/` into `round2/`; say what Darwin x86-64 (A by construction) is held to.
3. **Add A′ (`getcontext` + `setcontext` + `uc_link`, both orientations, MUST BE REFUSED) to the union,
   and make the trampoline's and thunk's own-frame prune a stated rule.** As written, rule 6 runs it inside
   the signal by reading, and no union shape can see it.
4. **Give G its three forms and a stated expectation** (or say the verdict is the author's), and add A
   by an unseen switch.
5. **Must-settle item 3**: say where the context findings go (filed `blocking`, 2522 or their own defect)
   and that 2679 now waits for them, or keep route C's 2679-only column in the seats' table.
6. **Must-settle item 6**: name the `run` goldens a landing owes on both platforms (finding 8's list), and
   require the falsifiable prediction to be one a CI leg can see.
7. **Rules 4, 5 and 7**: fix the classification order; strike "(it lives: the C library reads it next)",
   measured false at the swap on glibc 2.41 arm64; add F′ and F″ (and U if Linux reads `uc_stack`); write
   `kC5`'s third part (the clamp at a handler's end) into rule 5, with shape N for the truncation.
8. **Small**: `sigjump.c:62` → `runtime/parts/sigjump.c:62`; "critic 3 and 4" → finding 3 and item 4;
   "five edits" → five and the field; where `ij*` live (`251-reports/evidence.tar.xz`, `00-shared.md`
   § 2); `t_partial`/`t_unseen` are `res/`'s, since `fp/residue.h` is the older header; `fake` is
   decisive on Darwin only under `ASAN_OPTIONS=detect_stack_use_after_return=1`; list `critic/`'s
   `ctx-m*` and `t_*`.
