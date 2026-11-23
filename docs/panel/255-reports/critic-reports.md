# Panel 255, the completeness critic's second reading: the reports, before the synthesis

Written 2026-11-22 as I went, by the completeness critic (no seat, no verdict on the route). Read: the
briefs again as repaired (`00-shared.md`, `2679-question.md`, both seat briefs, `critic.md`), my first
reading (`critic-briefs.md`), then `compiler-engineer.md` (621 lines, `wc -l`; written CE below) and
`ffi-pragmatist.md` (593 lines; FP below), both finished. Everything I ran, I ran in my copy,
`.claude/worktrees/p255-scratch/critic/` (written `K/`), a detached worktree at `a75179641`, its stage 2
compiler built this afternoon (`./heroes --version`: `heroes 0.6.0`, `sources 61416c0c33ecc5a9`, the
tree's key), `TMPDIR=K/p255/tmp`. Each seat's route files and shapes I re-ran were COPIED into
`K/p255/r2/ce/` and `K/p255/r2/fp/` (md5 checked against the seat's folder where a line says so); nothing
was run, built or written in a seat's folder. My shapes are `K/p255/r2/sh/` (`xr.h`, the CE's third stack
ported to Darwin as `xc.h`), my runner `K/p255/r2/sh/run.sh` (each shape built plain and `--sanitize`
against a runtime named by `HEROES_RUNTIME`, run 3 times under `timeout 20`, one line per distinct
outcome), my logs `K/p255/r2/logs/` (Darwin) and `K/p255/r2/out-linux/` (Linux arm64, container
`crit255-r2-arm64` of `heroes-linux-arm64:latest`, stage 1 and stage 2 built inside it).

**The runtimes I ran**, each a copy of the tree's `runtime/` with a route's files replaced:
Darwin `K/p255/r2/rt/{today,fpC3,fpT,fpC3fail}` (FP's `C/rebind.c` and `C/sigguard.c`, md5
`c494aba8ad64daebebaf8be68838ebe1` for `rebind.c` in both places, which FP's report says is C3; `fpC3fail`
the same with its last calibration line replaced by `atomic_store(&hero_rebind_cal_bad, 1)`, line 463);
Linux `today`, CE's `C`, `C3` (its three files), `T`, and `ceC3fail` (CE's own `Cfail/sigjump.c` with C3's
`siginstall.c` and `sigguard.c`); and two probes of mine on Darwin, `kC4` and `kC5` (finding 11). Containers
`crit255-r2-arm64`, `crit255-r2b-arm64`, `crit255-r2c-arm64`, each removed after its run (`docker ps -a
--filter name=crit255` prints none).

## Findings

### 1. A coroutine whose stack is an array on the thread's OWN stack: the ffi-pragmatist's C3 runs the program inside the signal where today refuses it (Darwin, measured)

Both C3s were judged on a third stack that is a separate mapping (CE's `x_ctx`, `mmap`; FP's `ctx2` and
`cojump`, `malloc`). The shape beside it (CL-061): the coroutine's stack an array in a LIVE frame of
the thread's own stack, as a program that keeps its coroutine stacks as locals writes it. `xr.h` shape
A (`xr_coarray`): a 64 KiB `char stack[]` local to the runner, `getcontext` + `makecontext` on it, then
`raise`; the handler (on the thread's own stack for `a_*_0`, on an `mmap`ed alternate stack with
`SA_ONSTACK` for `a_*_1`) `swapcontext`s into the coroutine. how 1 (`a_1_*`): the coroutine calls the
program through twelve frames while the handler is suspended in `swapcontext`, FP's `ctx2` how 2 rule,
MUST be refused. how 2 (`a_2_*`): the coroutine swaps back and the handler, still running, calls the
program, FP's `ctx2` how 1 rule, MUST be refused. Each run prints `where: the coroutine's stack is ON
the thread's own stack` (by `pthread_get_stackaddr_np`; it said ON in every run, plain and `--sanitize`).
Command: `K/p255/r2/sh/run.sh K/heroes K/p255/r2/rt/<route> K/p255/r2/w-<route> a_1_0 a_2_0 a_1_1 a_2_1`,
logs `K/p255/r2/logs/mac-<route>.log`; every cell 3 of 3 runs, plain / `--sanitize`:

| Darwin arm64, this Mac | today | **FP's C3** | FP's C3, calibration forced to fail |
|---|---|---|---|
| `a_1_0` handler on own stack, the coroutine calls | 134 U / 134 U | **0, `answer ran for 1` / 0, `answer ran for 1`** | **0 / 0, runs** |
| `a_2_0` handler on own stack, the handler itself calls after the swap back | 134 U / 134 U | **0, `answer ran for 2` / 0, `answer ran for 2`** | 134 U / 134 U |
| `a_1_1` handler on the alternate stack, the coroutine calls | 134 U / 134 U | 134 S / 134 S | 134 U / 134 U |
| `a_2_1` handler on the alternate stack, the handler calls | 134 U / 134 U | 134 S / 134 S | 134 U / 134 U |

(U: *"jumped to a buffer this runtime holds no record of as it now stands"*; S: *"a signal handler of
C's that this runtime saw begin had not been seen to end"*.) **Under FP's C3 a Heroes function runs
inside the signal on `a_1_0` and `a_2_0`, plain and `--sanitize`, where today's Darwin refuses both**:
the dangerous direction, and in `a_2_0` the caller is the still-running handler itself. By reading
FP's `rebind.c` (C3), the cause: `hero_rebind_stack_of` (lines 176 to 190) asks the thread's own stack
FIRST, so a `makecontext` stack carved out of it is classified OWN, never KNOWN, though
`hero_rebind_known_stack` recorded it; the swap's prune then finds the handler's entry deeper than the
coroutine's stack pointer on the "same" stack and drops it (line 205, `there == here && w <= at`, at a
swap as at a jump), and `hero_rebind_inside` prunes again by the thunk's own frame (line 253). FP's
report says C3 "moves no program toward running inside a signal" and that a hand-switched stack "is
never seen and stays of unknown extent: its handler's entries stand, a told refusal": both are false
for a stack inside the thread's own bounds, which is classified OWN however it was switched to (the
second by reading; the first measured above). The Linux half of this finding is finding 5.

### 2. Correct jumps out that today's Darwin follows and the ffi-pragmatist's C3 refuses: a `setcontext` escape and `cojump` on the alternate stack (Darwin, measured)

FP's report says C3 follows "every correct jump out" (`jo_*`, `nest`, `cojump`, `fake`, the twelve
`a-callback-after-*`). Two shapes beside its own, each a correct program that leaves the handler before
the program is called, MUST RUN. `xr.h` shape B (`xr_escape`): the runner's context by `getcontext`,
the handler resumes it by `setcontext` (a jump out, as `siglongjmp` is), the runner then calls the
program 64 KiB deeper (`xr_deep`, CE's depth shape); `b_0` handler on its own stack, `b_1` on the
alternate stack. Shape C (`xr_cojump`): FP's `cojump.h` rewritten into `xr.h` with an `onstack`
argument; `c_0` is FP's shape (handler on the coroutine's heap stack), `c_1` the same handler on an
alternate stack, jumping OUT to the coroutine's buffer. Same command and logs as finding 1, 3 of 3
runs each, plain / `--sanitize`:

| Darwin arm64 | today | **FP's C3** |
|---|---|---|
| `b_0` `setcontext` out, handler on own stack | 0, `answer ran for 3` / 0 | 0 / 0, runs |
| `b_1` `setcontext` out, handler on the alternate stack | 0, `answer ran for 3` / 0 | **134 S / 134 S, refused** |
| `c_0` FP's `cojump` | 0, `answer ran for 9` / 0 | 0 / 0, runs (FP's measurement, reproduced) |
| `c_1` `cojump`, handler on the alternate stack | 0, `answer ran for 9` / 0 | **134 S / 134 S, refused** |
| `d_0`, `d_1` a static buffer, `siglongjmp` out, own / alternate stack (control) | 0 / 0 | 0 / 0, runs |
| `e` a heap buffer, a save between, `siglongjmp` out (CE's `x_heap`) | 0 / 0 | 0 / 0, runs |

**Two correct programs today's Darwin runs are refused under FP's C3**, in S words, which say the
handler was not seen to end (true of neither: both left it). By reading, the causes are C3's own rules:
`b_1`, a context jump never drops an entry on the alternate stack for a target on the thread's own
(`by_longjmp` false, line 206), so a handler left by `setcontext` from the alternate stack stands for
ever; `c_1`, an entry on the alternate stack is dropped only for a target on the thread's OWN stack, and
the coroutine's buffer is on a known stack. Neither is in FP's table of costs ("its own known costs: a
handler on a stack switched by hand-written assembly ... ; Darwin x86-64 gets no decode"). They are new
known costs of FP's C3 against today, owed to the author as ruling B's was, or repairs. CE's C3 on the
same programs is finding 5.

### 3. The two C3s fail differently when their calibration does not verify, and neither says it failed

CE's C3 falls to **A** where its key does not verify (CE's `Cfail`: 2522 25/0, measured by CE). FP's
C3 removed the record and the save points' rewrite on Darwin, so its fallback is "every jump inside a
handler keeps every entry, marked unsure" (`rebind.c` C3, lines 476 to 479). FP's own verdict on A
says the opposite ordering: *"as that floor it is better than C's told refusal of every jump (critic
5)"*, and FP's report never runs its C with the calibration failed. I did: `K/p255/r2/rt/fpC3fail`
(FP's C3, its line 463 replaced so `hero_rebind_decodes` stays 0), same runner, logs
`K/p255/r2/logs/mac-fpC3fail.log`, `mac2-fpC3fail.log`, 3 of 3 runs, plain / `--sanitize`:

| Darwin arm64, FP's C3 with the calibration failed | outcome | today |
|---|---|---|
| `gcb` = `fixedbugs-2522-a-callback-after-a-siglongjmp-out-of-a-handler-on-an-alternate-stack-deep-first-runs`, built from the tree with its header | **134 U / 134 U** (its `.expected` is `installed 0`, `escaped 1`, `deep 8`, `shallow 6`, exit 0) | 0 / 0, the expected lines |
| `d_0`, `d_1` a static buffer jumped out of, own / alternate stack | **134 U / 134 U** each | 0 / 0 |
| `e` a heap buffer | 134 U / 134 U | 0 / 0 |
| `gcopy` the copied-buffer case | 134 U / 134 U | 134 U / 134 U |
| `c_0` FP's `cojump` | **0 plain / 134 U sanitized** | 0 / 0 |
| `a_1_0` (finding 1) | **0 / 0: runs inside the signal** | 134 U / 134 U |

So where FP's decode does not verify (Darwin x86-64 by construction, `rebind.c` C3 lines 465 to 467,
`hero_rebind_calibrate` empty under `#else`; a future libsystem), **the twelve depth cases and every
static-buffer jump out of a handler are refused**, in U words that are false for a static buffer
(*"copied with `memcpy`, refilled ..., or saved before more save points than it keeps"*), where CE's
fallback follows them. And it is not the uniform told refusal FP describes: the thunk still prunes by
its own frame (line 253), so `a_1_0` runs inside the signal and `c_0`'s verdict depends on
`--sanitize` (frame sizes). On the same platform event (a calibration that does not verify) the two
C3s give opposite verdicts on the canonical correct program.

**Neither calibration announces its outcome.** `grep -n hero_rebind_decodes` of FP's C3 finds lines
373, 463, 476 (declared, set, read by the jump); `grep -n hero_jmp_decoding` of CE's C3 finds 134, 320,
453, 473; no print, no accessor, no `heroes doctor` row, no runtime-suite test. FP's Q1 item 5 asks for
*"the `heroes doctor`-style check or a test of the runtime suite"* and did not build it; CE names *"a
test that forces it (`Cfail`'s define)"* as owed. Both seats' claim that the calibration verified is
read from outcomes (CE: the refill case's S words and `x_unseen` refused; FP: `jo_*`, `nest`,
`cojump` running), which is a sound inference, and I re-measured it the same way with a control that
only the decode passes: on Darwin `d_0` runs under FP's C3 and is refused under `fpC3fail` (above). The
claim *"failing loud"* is, in both seats' builds, a refusal of particular programs, never a statement
that the premise died: a landing owes the instrument (`.claude/rules/module-shape.md`, the runtime
premise's test that fires when it dies).

### 4. T's vetoing shapes, re-run from copies: both reproduce on Darwin

FP's `res` (`residue.h` md5 `39e24325e72a382c9eb74f3f87f74f6d`, the same in FP's folder and my copy),
`t_partial` and `t_unseen`, built with FP's T (`K/p255/r2/rt/fpT`, its `rebind.c` copied), today and
FP's C3; `K/p255/r2/sh2/run.sh`, `K/p255/r2/logs/mac2-<route>.log`, 3 of 3 runs each:

| Darwin arm64 | today | FP's T | FP's C3 |
|---|---|---|---|
| `t_partial` (MUST be refused) | 134 U / 134 U | **0, `answer ran for 30`, `seen 300` / the same** | 134 S / 134 S |
| `t_unseen` (MUST be refused) | 134 U / 134 U | **0, `answer ran for 30` / the same** | 134 S / 134 S |

FP's veto evidence stands as reported. The Linux half (CE's `x_unseen` under CE's T) is finding 5.

### 5. The same shapes on Linux arm64 under CE's routes: shape A runs inside the signal under CE's C3 in three orientations of four, and CE's C3 refuses FP's `cojump`

Container `crit255-r2-arm64` (`heroes-linux-arm64:latest`: `aarch64`, `ldd (Debian GLIBC
2.41-12+deb13u3) 2.41`, `Debian clang version 22.1.8`), my copy mounted read-only, stage 1 then stage 2
inside it (`stage 2 ok: heroes 0.6.0 sources 61416c0c33ecc5a9`, the tree's key), then `K/p255/r2/linux.sh`:
each route a `HEROES_RUNTIME` folder (`today`; CE's `C`; CE's `C3`, its three files, `sigjump.c` md5
prefix `235884bb69d8`; CE's `T`; `ceC3fail`), the same runner, 3 of 3 runs each, plain / `--sanitize`
(every cell below agreed across its 3 runs and both modes unless it says otherwise). FP's `ctx2.h`
copied with FP's own Linux fix (`calloc(1, sizeof(ucontext_t) ...)`, as FP's `linux-ctx2.sh` applies
it), `cojump.h` (`cj`) and `nest.h` (`ns_0`, `ns_1`) unchanged; CE's `extra.h` (md5
`f4f7ece3a365181fad6c920e700942cc`, CE's folder and mine) for `x_*`. Logs `K/p255/r2/out-linux/shapes-<route>.log`.

| Linux arm64 | today | CE's C | **CE's C3** | CE's T | CE's C3, calibration failed (A) |
|---|---|---|---|---|---|
| `a_1_0` own-stack handler, coroutine on an own-stack array calls (MUST refuse) | **runs** | **runs** | **runs, `answer ran for 1`** | runs | |
| `a_2_0` the handler calls after the swap back (MUST refuse) | **runs** | **runs** | **runs, `answer ran for 2`** | runs | |
| `a_1_1` alternate-stack handler, the coroutine calls (MUST refuse) | **runs** | **runs** | **runs, `answer ran for 1`** | runs | |
| `a_2_1` alternate-stack handler, the handler calls (MUST refuse) | 134 K | 134 K | 134 K (the walk) | 134 K | |
| `b_0`, `b_1` `setcontext` out (MUST run) | run | run | run | run | `b_1` runs |
| `c_0` and FP's own `cj` (MUST run) | run | run | **134 S, refused** | run | |
| `c_1` `cojump`, handler on the alternate stack (MUST run) | runs | runs | **134 S, refused** | runs | |
| `d_0`, `d_1`, `x_static` static buffer out (MUST run) | run | run | run | run | **run** |
| `e`, `x_heap` heap buffer out (MUST run) | run | run | run | run | 134 U (A's cost) |
| FP's `ctx2` how 1 (`c2_1`) / how 2 (`c2_2`) (MUST refuse) | **runs / runs** | 134 S / **runs** | 134 S / 134 S | runs / runs | |
| FP's `nest`, own and alternate stack (MUST run) | run | run | run | run | |
| CE's `x_unseen` (MUST refuse) | 134 U | 134 S | 134 S | **runs, `answer ran for 5`** | |
| CE's `x_unseen0` (MUST refuse) | **runs** | 134 S | 134 S | **runs** | |
| CE's `x_partial` (MUST refuse) | 134 U | 134 S | 134 S | 134 S (arm64: the token rides in `__jmpbuf`) | |
| `gcopy` the copied-buffer case | 134 U | **runs, `called 8`** | **runs, `called 8`** | runs | 134 U |
| `grefill` the refill case (MUST refuse) | 134 U | 134 S | 134 S | 134 S | 134 U |

(K: *"called from inside a signal handler: C called it while handling a signal"*, the walk.) What this
settles and what it moves:
- **Shape A is a class C3 was meant to close and does not, on either platform.** CE's C3 runs the
  program inside the signal in three orientations of four (`a_1_0`, `a_2_0`, `a_1_1`, plain and
  `--sanitize`, `where: ... ON the thread's own stack` in every run), exactly as today and CE's C do;
  only the walk catches `a_2_1`. CE's report says C3 *"closes the residue today's header names for
  `makecontext`'s stack, for every orientation I could build"*: the orientations built were all
  separate mappings. By reading CE's C3 `siginstall.c`, the causes: `hero_sig_stack_of` answers OWN for
  any address inside the thread's bounds (no known-stack class on Linux; `makecontext` is not wrapped,
  `sigjump.c:62` and 63), the prune drops an entry on another stack whenever the target is on the own
  stack, at a swap as at a jump (`left = ... || (here == HERO_SIG_ON_OWN && there != HERO_SIG_ON_OWN)`,
  `a_1_1`), and drops a same-stack entry deeper than the target (`a_1_0`, `a_2_0`).
- **CE's C3 refuses FP's `cojump` (`c_0`, `cj`)**, a correct program today, CE's C and CE's T run, FP's
  C3 runs on Darwin. CE named this cost *"by reading and unrun"* (*"a signal delivered while a coroutine
  runs on a third stack WITHOUT `SA_ONSTACK` ..."*): now measured. CE's C3 table says *"no measured cost
  on the other shapes"*; it had not run FP's.
- **Today's `ctx2` finding re-run with the tree's stage 2**: FP's Linux run used stage 1 from the seed
  (`sources 05186f2b38e77a97`, FP's report), not the tree's compiler; mine, stage 2 at the tree's key,
  reads the same, both `ctx2` programs running inside the signal, plain and `--sanitize`. FP's reading
  of C (*"would fix how 1 by the decode ... and keep how 2 unless it takes C2's thunk rule"*) is
  confirmed: CE's C refuses `c2_1` and runs `c2_2`; C3 refuses both.
- **T's veto reproduces on Linux**: `x_unseen` runs under CE's T (`answer ran for 5`, plain and
  `--sanitize`), refused under today, C and C3. `x_partial` is refused under T on arm64, as CE reported;
  its x86-64 run (T runs it) I did not re-run.
- **The copied-buffer and refill cases land the same under both C3s**: `gcopy` runs (`called 8`) on
  Linux under CE's C and C3 and on Darwin under FP's C3 and T (`K/p255/r2/logs/mac2-fpC3.log`,
  `mac2-fpT.log`, plain and `--sanitize`); `grefill` is refused under every route on both platforms, in S
  words under C, C3 and T, U words under today and a failed calibration. No contradiction between the
  seats there.
- **CE's calibration verified in my container**, by a control only the decode passes: `e` and `x_heap`
  run under CE's C3 and are refused under `ceC3fail` (A).

### 6. The two C3s side by side: three programs get opposite verdicts, and one split is toward Heroes code inside a signal

Both seats approve "C3", built apart, and say so as if it were one route. From findings 1, 2, 3 and 5,
every shape run under both (my runs, 3 of 3, plain and `--sanitize`, both modes agreeing):

| shape | Linux today | **Linux CE's C3** | Darwin today | **Darwin FP's C3** | the two C3s |
|---|---|---|---|---|---|
| `a_1_0`, `a_2_0` coroutine on an own-stack array, own-stack handler (MUST refuse) | runs | **runs** | refused U | **runs** | both unsound |
| `a_1_1` the same, alternate-stack handler (MUST refuse) | runs | **runs** | refused U | refused S | **split: Linux runs inside the signal** |
| `a_2_1` (MUST refuse) | refused K | refused K | refused U | refused S | agree |
| `b_1` `setcontext` out of an alternate-stack handler (MUST run) | runs | runs | runs | **refused S** | **split** |
| `c_0` FP's `cojump` (MUST run) | runs | **refused S** | runs | runs | **split** |
| `c_1` `cojump` on the alternate stack (MUST run) | runs | refused S | runs | refused S | agree, both refuse what today runs |
| CE's third stack m1 / m2 / m3, four orientations (m1, m3 MUST refuse; m2 MUST run) | m1 **runs in 3 of 4** / runs / refused U (finding 9) | refused S / runs / refused S (finding 9) | **refused U all four** / runs / refused U | refused S / runs / refused S | agree |
| a static, a heap buffer out (`d_*`, `e`); `gcopy` | run (`gcopy` refused U) | run | run (`gcopy` refused U) | run | agree |
| `x_unseen0` (Linux) / `t_unseen0` (Darwin, FP's `res` how 2 with no save between) (MUST refuse) | runs | refused S | **runs** | refused S | agree |
| a failed calibration: a static buffer out (`d_0`, `d_1`, Darwin also `gcb`) | | runs (A) | | **refused U** | **split** |

(The Darwin `ctx-m*` rows are mine, CE's `x_ctx` ported unchanged but for its feature macros, `xc.h`;
`t_unseen0` is FP's `residue.h` with its `setjmp(later)` skipped for `how 3`, `K/p255/r2/sh2/residue0.h`,
logs `mac3-<route>.log`.) Four consequences for the synthesis:
- The two seats' answers to `00-shared.md` § 6's first question now meet a run. CE's rule, *"a verdict
  may differ by platform only between runs and a told refusal, never toward a crash or Heroes code
  inside a signal"*, is broken by `a_1_1` under the two C3s as built; FP's rule, *"no"*, is broken by
  `a_1_1`, `b_1`, `c_0` and the failed calibration. The recommended composition is not one route until
  its two halves agree on these programs.
- **The two seats' third-stack designs are each refuted by the other's shape**: CE's C3 refuses FP's
  `cojump` (a known-stack table would follow it, as FP's does); FP's C3 refuses `b_1` and `c_1`, which
  CE's prune follows or today follows. Neither seat ran the other's shapes on its own platform (each
  brief forbade reading the other's folder), so neither table shows this.
- **Today's `x_unseen0` residue is on Darwin too**: `t_unseen0` runs inside the signal under today's
  `rebind.c` (exit 0, `answer ran for 30`, `seen 300`, plain and `--sanitize`, 3 of 3), refused under
  FP's C3. And **today's Darwin refuses CE's m1 in all four orientations**, so the third-stack finding
  is Linux's alone under today's runtime, while shape A is Linux's under today (three of four) and
  becomes Darwin's under FP's C3 (two of four).
- Nothing here contradicts either seat's measurements: every row the seats ran, I re-ran and read the
  same (`c2_*`, `cj`, `ns_*`, `x_unseen`, `x_unseen0`, `x_partial`, `t_partial`, `t_unseen`, the copy
  and refill cases, CE's m1 to m3 on Linux in finding 9). The contradictions are between the
  two DESIGNS, on shapes each seat did not build.

### 7. Today's runtime's new soundness findings are defect 2522's class, not 2679's, and no issue or ruling holds them

What the seats measured in today's runtime (FP: `ctx2` on Linux; CE: `x_unseen0`, the third stack's
m1), and what I add (shape A on today's Linux in three orientations of four, `t_unseen0` on today's
Darwin), have one thing in common: **under `--sanitize` every one of them exits 0 with no
AddressSanitizer line** (findings 5 and 6, the runner prints the first ASan line and printed none). No
memory the program gave back is read; the failure is a Heroes function running inside a signal
handler's extent, design.md §1.12 by the sitting's own veto list (`00-shared.md` § 6). Defect 2679 is
the runtime reading given-back memory (its title, the brief's § 3 table: every 2679 row is a SEGV or an
ASan report). So they are **not 2679's class**: no route for 2679 is obliged to close them, and closing
2679 does not close them, which both seats say in their own words (CE: *"it is not 2679's: no route
closes or opens it"*; FP: *"a soundness defect of its own, blocking by its class"*).

Whose they are: `runtime/parts/sigjump.c:59` to 66 names two of them as *"defect 2522's rows"* (*"a buffer
refilled ... by a save this file never saw ... with no other save or jump between"*, which is
`x_unseen0`; *"`makecontext`'s new stack"*, which is the third stack). Defect 2522 is open
(`gh issue view 2522`: `OPEN`, `kind:defect`, `class:blocking`, `area:runtime`). Its rows as lane-signals
read them on 2026-11-20 and the author's rulings of that day (its last four comments, read whole into
`K/p255/tmp/i2522c.md`, 6,241 bytes) are rows 1 to 5: the copied buffer, the inline `__builtin_setjmp`
pair, installers through `dlsym`, the `macos-26` run, the census. `grep -n -i -E
'makecontext|third|getcontext|swapcontext|never saw|unseen|context'` of those comments finds no row for a
context or for an unseen refill with no save between, and ruling B (*"the refusal stands, as a known cost"*)
rules on refusals, never on a program that runs inside the signal. `gh issue list --state all --search`
of `makecontext`, `swapcontext`, `coroutine`, `"context swap"` and `"third stack"` finds no defect about
them (`makecontext`, `swapcontext` and `"third stack"` matched nothing; `coroutine` matched 370, 159,
896, 2674, 125 and `"context swap"` 1940, 959, 1917, 1393, 1415, none about signals). A search is the
searcher's vocabulary: these words found nothing, which is not a proof that nothing exists. So they stand as words in
a code comment and two seats' reports: **unfiled, unruled, `blocking` by their class** (a wrong
program accepted, `.claude/rules/verification.md` § Bounded discovery), and the synthesis must say where
they go: 2522's open item (its rows grown by the measured shapes) or a defect of their own, and
whether the 2679 landing waits for them. A Darwin note the seats could not give: under today's runtime
the third stack's m1 is refused on Darwin in all four orientations (finding 6), so the class is
platform-shaped today too.

### 8. No case in the tree holds a context switch, so both predictions and every CI leg are blind to findings 1, 2, 5 and 6

`git grep -l -E 'swapcontext|makecontext|setcontext' -- tests/` in my copy prints nothing: no golden
case, of any form, switches a context. Both seats' predictions are about `fixedbugs-2522`'s count line
on the CI's legs (CE: `ubuntu-26.04-arm` and `ubuntu-26.04`; FP: `macos-26`), and both would come true
with C3 landed as built while shape A runs the program inside the signal on both platforms and
`cojump`, `b_1`, `c_1` are refused where today follows them: the CI cannot falsify any of it. CE lists
*"the third stack's twelve"* among the `run` goldens a landing owes; FP lists no golden for `ctx2`,
`cojump`, `nest` or its `jo_*` programs. And the harness keeps one `.expected` per case (my first
reading, finding 9), so `a_1_1`, `b_1` and `c_0`, whose verdicts the two C3s split (finding 6), cannot
be pinned at all until the two designs agree or the harness grows a per-platform expectation. The
synthesis should name the cases a landing owes on BOTH platforms: CE's twelve third-stack programs,
FP's `ctx2`, `cojump`, `nest`, `jo_heap`, `jo_tls`, and this reading's `a_*`, `b_*`, `c_*`,
`x_unseen0`/`t_unseen0`.

### 9. CE's third-stack table re-run on Linux from my copy: it reproduces cell for cell

Container `crit255-r2b-arm64`, the same image and build (`stage 2 ok: heroes 0.6.0 sources
61416c0c33ecc5a9`), `K/p255/r2/linux2.sh`, CE's twelve `ctx-*.hero` against `xc.h` (its `x_ctx` and
`x_deep` copied unchanged), today and CE's C3, 3 of 3 runs, plain / `--sanitize`, logs
`K/p255/r2/out-linux2/ctx-<route>.log`: today m1 **runs in three orientations** (`o0 s0`, `o0 s1`, `o1
s0`: `answer ran for 7`) and is refused in `o1 s1` (S); m2 runs in all four; m3 refused in all four (U).
CE's C3: m1 refused in all four (S), m2 runs, m3 refused (S). CE's table stands. What it does not
reach is finding 5's shape A, the same class on the thread's own stack.

### 10. The two calibrations verify different things, each missing what the other has (by reading, the code cited)

- **CE's** (`K/p255/r2/ce/C3/sigjump.c`, `hero_jmp_try` 264 to 293, `hero_jmp_try_far` 295 to 301,
  `hero_jmp_calibrate` 303 to 321): a probe that fills every callee-saved register with a pattern, so
  the measured `-O2` trap (a register holding the stack pointer, key 0, CE's Q2) cannot pass; it
  verifies the stack pointer's word, a second mangled word and the unmangled patterns, for `setjmp`,
  `_setjmp`, `__sigsetjmp` and `getcontext`, at two frames **`0x9230` bytes apart on one stack**. No
  second stack, and not the 1 MiB distance its own Q2 probe used (CE argues the patterns make distance
  irrelevant, which is a reading, sound as far as I can see).
- **FP's** (`K/p255/r2/rt/fpC3/parts/rebind.c` 386 to 464): the stack pointer's word alone (word 12, and
  `+0x148` of a context), at one frame, 16 frames of 64 KiB deeper and on a 256 KiB `mmap`ed stack. No
  register patterns: a libsystem that moved the stack pointer to another word would be caught only if
  the new word 12 does not happen to track the stack pointer, which CE measured a register can do at
  `-O2`. Unmeasured on Darwin; a question, not a defect.
- The landing would take the stronger of each: patterns AND two stacks far apart, on both platforms.

### 11. A route nobody built: what a context swap inside a handler proves. Two probes of mine on Darwin show the repair is in reach and which premise each pays

Not a route proposal and no verdict: two edits of FP's C3 in my copy, run to show what would have to be
true for shape A to close without moving the rest. Both keep FP's decode; both add two rules that
findings 2 and 6 point at: a `setcontext` is a jump (it never returns, as `siglongjmp` does not;
FP's C3 passes `by_longjmp` false for it), and a jump that leaves the alternate stack for ANY other
stack drops the alternate stack's entries (FP's C3 drops them only for a target on the thread's own).
They differ in how shape A is closed:

- **`kC4`, known stacks before the own stack** (`K/p255/r2/rt/kC4/parts/rebind.c`, 4 edits marked
  `critic probe kC4`): `hero_rebind_stack_of` asks the table of `makecontext` stacks before the
  thread's own bounds.
- **`kC5`, a swap suspends** (`K/p255/r2/rt/kC5/parts/`, 5 edits marked `critic probe kC5`, a field
  `suspended` in `sigguard.c`'s record): a `swapcontext` made while a handler stands marks every
  standing entry suspended until that `swapcontext` returns in its own frame (the wrapper is no longer a
  tail call, as CE's Linux wrapper already is not), and no prune drops a suspended entry. The own stack
  stays first, as FP has it.

Shape F (`K/p255/r2/sh2/xf.h`, `f.hero`): a helper makes a context on a 256 KiB local array and
returns without swapping to it, then the plainest correct jump out of a handler, a static buffer saved
ABOVE that dead range, the signal raised 32 KiB below it so the handler's frame lies INSIDE it, the
program called 64 KiB deeper: MUST RUN. Every row 3 of 3 runs, plain / `--sanitize` agreeing, logs
`K/p255/r2/logs/mac-kC4.log`, `mac2-kC4.log`, `mac-kC5.log`, `mac2-kC5.log`, `mac4-f.log`:

| Darwin arm64 | today | FP's C3 | `kC4` known first | `kC5` swap suspends |
|---|---|---|---|---|
| `a_1_0`, `a_2_0`, `a_1_1`, `a_2_1` (MUST refuse) | refused | **runs, runs**, refused, refused | refused all four (S) | refused all four (S) |
| `b_1` `setcontext` out of the alternate stack (MUST run) | runs | refused | runs | runs |
| `c_0`, `cj` FP's `cojump` (MUST run) | run | run | run | run |
| `c_1` `cojump` on the alternate stack (MUST run) | runs | refused | runs | runs |
| `d_0`, `d_1`, `e`, `gcb` (MUST run); `nest` both stacks (mine under `kC4`, `kC5`; FP's for today and C3) | run | run | run | run |
| CE's m1 / m2 / m3, four orientations | refused / runs / refused | the same | the same | the same |
| `t_partial`, `t_unseen`, `t_unseen0`, `grefill` (MUST refuse) | `t_unseen0` runs, the rest refused | refused | refused | refused |
| `gcopy` | refused | runs | runs | runs |
| **`f` a stale `makecontext` range (MUST run)** | runs | runs | **refused (S)** | runs |

What this says, and what it does not:
- `kC4` closes shape A by trusting the table, and **pays with `f`**: a `makecontext` range carved from
  the thread's own stack is never forgotten when its frame returns (nothing tells the runtime), so a
  later correct handler whose frame falls inside it is judged on "another stack" and its jump out is
  refused. Its premise: every range in the table is still a coroutine's stack, which no runtime can see
  end.
- `kC5` closes shape A by the rule *a handler a swap suspended has not ended*, and keeps `f`, `b_1`,
  `c_1` and every row above. Its premises, unmeasured: that a suspended handler is resumed in the frame
  that suspended it (a coroutine that `longjmp`s away and never resumes the handler leaves its entries
  standing, every later callback refused, told; by construction, unrun); and it is Darwin-only as built
  (Linux has no `suspended` field, though CE's `swapcontext` is already not a tail call).
- Neither probe was run through the harness, on Linux, or against the `runtime` suite's census. They
  show that the two seats' C3s are not the only designs for the third stack, and that the most robust
  one is not yet built: the sitting should not adopt either seat's C3 as *the* third-stack repair on
  this evidence.

### 12. Claims asserted in the reports that the runs above change, and the questions the sitting did not ask

Claims, each with the run that moves it:
- CE, on § 6's second question: *"C refuses, of everything I ran on the CI's two Linux architectures, no
  correct program a reader expects to run beyond what today already refuses"*. True of C as CE ran it;
  **not of C3**, which CE approves: C3 refuses `c_0`/`cj` and `c_1` (finding 5), correct programs that
  leave the handler before calling. By CE's own standard C3 owes the reader a sentence, or a repair.
  The same holds for FP's C3 and `b_1`, `c_1` (finding 2), where FP answered *"no new sentence for any
  program in this report but one"* (`cojump`, which C3 then fixed).
- CE: C3 closes the third stack *"for every orientation I could build, with no measured cost on the
  other shapes"*; FP: C3 *"moves no program toward running inside a signal"*. Shape A (findings 1, 5)
  runs inside the signal under both. CE's C3 cost *"by reading and unrun"* is now measured (`cojump`
  refused).
- FP: a stack switched by hand-written assembly *"is never seen and stays of unknown extent: its
  handler's entries stand, a told refusal"*. For a stack carved from the thread's own bounds it is
  classified OWN (`rebind.c` C3 line 177 asks own bounds first), by reading; shape A measures the
  `makecontext` form of it.
- FP's verdict: A is *"better than C's told refusal of every jump (critic 5)"* as a floor; FP's built C
  has the told refusal as its floor (finding 3).
- FP's `ctx2` run on Linux used the seed's stage 1 compiler (`sources 05186f2b38e77a97`); re-run with the
  tree's stage 2 it reads the same (finding 5). No verdict moves; the provenance is now the tree's.

Questions the sitting should have asked, and did not:
- **What does a context switch inside a handler prove?** Both briefs ask about *a third stack*; the
  question underneath is whether `swapcontext` from a handler is a jump out (both C3s prune at it by
  address) or a suspension (FP's C3 half-says so for the alternate stack only). Findings 1, 2, 5 and 11
  are all this one question, answered differently by each design and correctly by neither.
- **Is the third stack part of 2679's resolution?** Both seats folded C3 into their approval of the
  2679 route. Finding 7 measures that the third stack, `ctx2` and `x_unseen0` are defect 2522's class;
  route C closes 2679's eight shapes and libpng without C3 (CE's C row, 16 of 16 clean; FP's C2 row;
  libpng by both seats, which I did not re-run; the eight shapes re-run by me under both C3s, finding 13). The
  sitting can adopt C for 2679 and send the context question to 2522 with its own shapes, or hold 2679
  until the context design is measured: which one is a choice the synthesis owes, said.
- **One design or two?** The briefs gave one platform to each seat with no reading of the other's
  folder, the right rule for independence, and the reason no seat ran the other's shapes. The cost is
  finding 6: three programs split. Before adoption, each platform's design should be run on the union
  of both seats' shapes and this reading's.

### 13. Defect 2679's eight shapes under both C3s, re-run from copies: closed on both platforms

The repro of the brief (`255-briefs/repro/shapes.h` and its eight `.hero`, copied into `K/p255/r2/sh3/`),
my runner, 3 of 3 runs, plain / `--sanitize`. Darwin (`K/p255/r2/logs/mac6-<route>.log`): today reads
the coordinator's table (`heap`, `handler_heap`, `plain_heap` 134 with a heap-use-after-free under
`--sanitize`; `mmap`, `handler_mmap`, `plain_mmap` **139** plain and 134 sanitized; `context` and
`stack` 0); **FP's C3: all sixteen builds exit 0, no ASan line**. Linux arm64 (container
`crit255-r2c-arm64`, stage 2 at the tree's key, `K/p255/r2/linux3b.sh`, `K/p255/r2/out-linux3b/r8-<route>.log`):
today reads the coordinator's table (`mmap`, `handler_mmap` **139** plain; `heap`, `mmap`, `context`,
`handler_heap`, `handler_mmap` exit 1 with an ASan report under `--sanitize`; `stack`, `plain_heap`,
`plain_mmap` 0); **CE's C3: all sixteen builds exit 0, no ASan line**.
So route C's decode, as each seat built it, closes 2679's shapes on the platform it was built for; the
items above are about what C3 adds beside 2679, not about 2679.

## Unrun here, said

- No harness run of any route (the case sets are the seats'); no `runtime` suite run; libpng under any
  route (both seats ran it); x86-64 anything (CE's emulated legs stand as CE's); Darwin x86-64 (no
  Rosetta, `00-shared.md` § 7).
- `kC4` and `kC5` on Linux, through the harness, or under the census; `kC5`'s premise that a suspended
  handler resumes in the frame that suspended it.
- A coroutine switched by hand-written assembly on a stack carved from the thread's own (FP's
  Boost.Context and libco case): by reading only (finding 12).
- FP's `jo_tls`, `jo_lib`, `fake` and `ctx2` on Darwin, and CE's `x_tls`, `x_dlopen`, `x_prot`,
  `x_realloc`, `x_refill_out`, `x_fake`, `x_dlclose` on Linux: the seats' measurements, not re-run.
- Every timing: none taken.

## What the synthesis must settle

Most important first. Each item names the finding that measures it.

1. **Shape A runs a Heroes function inside a signal handler under BOTH seats' C3s** (findings 1, 5, 6):
   a coroutine whose stack is an array on the thread's own stack, the handler swapping into it. Linux,
   CE's C3: three orientations of four, as today; Darwin, FP's C3: two of four, where today's Darwin
   refuses all four. Neither C3 can be adopted as *the* repair of the third stack on this evidence. The
   question underneath, which neither brief asked, is **what a context switch inside a handler proves**
   (a jump out, or a suspension); the synthesis should state the rule, and require the design that
   carries it to be built and run on BOTH platforms against the union of the seats' shapes and this
   reading's (`a_*`, `b_*`, `c_*`, `f`, `ctx2`, `cojump`, `nest`, CE's m1 to m3, `x_unseen0`). Finding
   11's two probes show the rule is reachable on Darwin and what each premise costs (`kC4` known
   stacks first: closes A, refuses `f`; `kC5` a swap suspends: closes A, keeps `f`, unrun on Linux,
   in the harness and in the census).
2. **The two C3s are two designs, and three programs get opposite verdicts** (finding 6): `a_1_1`
   (Linux runs it inside the signal, Darwin refuses), `b_1` and `c_0`/`cj` (one runs, the other
   refuses), and a failed calibration (Linux follows a static buffer out, Darwin refuses it). CE's
   rule for § 6 (*"only between runs and a told refusal"*) is broken by `a_1_1`; FP's (*"no"*) by all
   of them. Settle one design for both platforms, or put the split to the author with the platform
   question, which stays theirs.
3. **Is the context class part of 2679's resolution?** (findings 7, 12) Every context finding is
   defect 2522's class (a Heroes function inside a signal, no memory given back read: clean under
   `--sanitize`), named as 2522's rows in `sigjump.c:59` to 66, held by no issue, no row of 2522's
   2026-11-20 reading and no ruling. Whatever the route for 2679, the synthesis owes their filing as
   `blocking` (today's `ctx2` and shape A on Linux, `x_unseen0` on BOTH platforms, m1 on Linux), and
   says whether 2679's landing (route C's decode, which closes 2679's eight shapes and libpng on both
   platforms by the seats' runs and mine, finding 13) waits for the context design or lands first.
4. **The calibration's failure** (finding 3): CE's C3 falls to A; FP's to keep-every-entry, which
   refuses the twelve depth cases and every static buffer out in words false for them, and is not even
   uniform (`a_1_0` runs, `c_0` splits on `--sanitize`). FP's own verdict ranks A above that floor.
   Settle one fallback for both; and require the runtime to SAY whether its decode is on (neither
   build does: no print, no accessor, no test), with the forced-failure case on both platforms.
5. **New known costs against today** (findings 2, 5, 12): FP's C3 refuses `b_1` (a `setcontext`
   escape) and `c_1` on Darwin; CE's C3 refuses FP's `cojump` and `c_1` on Linux. Each is a correct
   program today runs. Either repaired (finding 11 shows both `b_1` and `c_1` follow under one more
   rule) or told to the author as ruling B was, with the spec sentence § 6 asks about.
6. **No case in the tree switches a context** (finding 8): both predictions and every CI leg are blind to
   items 1 to 5. The landing owes those programs as `run` goldens on both platforms, and the harness
   cannot pin a split verdict (one `.expected` per case).
7. **The calibration's strength** (finding 10): CE's patterns and FP's second stack, each missing in the
   other's; a landing takes both.
8. **What stands, re-run from copies** (findings 4, 5, 9): T's veto (`x_unseen` on Linux, `t_partial`
   and `t_unseen` on Darwin, each running the program inside the signal under T); CE's third-stack table
   on Linux; under every C the copied-buffer case runs (`called 8`) and the refill case stays refused in
   S words, on both platforms, the same way: ruling B's change, the author's to read.
