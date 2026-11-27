# panel 262, compiler-engineer: report (started 2026-11-26 21:46)

Written as it goes. Copy: `.claude/worktrees/262-compiler-engineer`, detached at `87003cb42`. `TMPDIR` is `p262/../tmp`
inside the copy. The machine read `load averages: 39.02 51.26 52.74` at 21:47 (`uptime`), so **no duration below is a
measurement**.

## 0. The ground moved before I started (asked 21:47)

`git log --oneline 87003cb42..main -- runtime spec` (main at `c2b7574bf`):

- `8d8263f15` **step 199f, defect 2852 (G)**: *"the signal guard's walk of frame records ends at the top of the
  coroutine's stack it runs on"* (`sigguard.c`, `sigentries.c`, `rebind_entries.c`);
- `01706218e` **step 199e, defect 2853 (J)**;
- `0df8f1a36` step 203f, panel 261's spec text.

`diff -rq runtime p262/rt-main/runtime` (the second column, `git archive c2b7574bf runtime`): five files differ,
`rebind_entries.c`, `rebind_jump.c`, `sigentries.c`, `sigguard.c`, `sigjump_save.c`. **So the lane's G has landed on
`main`**, and every row resting on 2852 runs on both columns below.

## 1. Compilers (both platforms read the brief's key)

- **This Mac**: seed `sources 1e1eb23a1927d35b` (`./heroes --version`), stage 2 **`sources 6ce3f9e116d22e3a`**
  (`./heroes-next --version`; `/usr/bin/time -p`: seed `real 110.85 user 22.23`, stage 2 `real 715.55 user 184.59
  sys 36.78`, load 39 to 118: not durations). `programs/compiler-engineer/build.log`.
- **Linux arm64** (`heroes-linux-arm64:latest`, one container `p262-compiler-engineer`, `--rm --pull never`, over
  `git archive 87003cb42` in `p262/linux/`): seed `1e1eb23a1927d35b`, stage 2 **`6ce3f9e116d22e3a`**
  (`L-build-seed.txt`, `L-build-stage2.txt`).
- **The second column**: `main` at `c2b7574bf`, `git archive c2b7574bf runtime` in `p262/rt-main/`, built with
  `HEROES_RUNTIME`. It differs from the base in 199e and 199f alone (§ 0).
- **Route A**: four runtime copies, each the column's `runtime/` with `runtime.c` opened by
  `#define HERO_JMP_FORCE_UNCALIBRATED 3` (`rt-{base,main}-forceA`, both platforms) or by `#define HERO_JMP_NO_DECODE 1`
  and `#define HERO_JMP_MAKE_IN_C 1` (`rt-{base,main}-sim`, 199a's simulation, Linux arm64 alone: on Darwin
  `sigjump.c` is not the route).

## 2. My programs (`programs/compiler-engineer/`; each built plain and `--sanitize`, three runs, by `ce.sh`)

| program | what C does | handler of C's? |
|---|---|---|
| `p4s`, `p4d`, `p4x` | panel 257's, byte for byte (`docs/panel/257-reports/evidence.tar.xz`): an inline assembly jump out of a handler, then the callback at the raise site's depth (`p4s`), `PAD` bytes deeper (`p4d`, PAD 0 to 65,536), or after a second handler returned (`p4x`) | yes |
| `wd` | **the watchdog**: a `SIGALRM` handler `siglongjmp`s out of a C loop calling a Heroes function that allocates (an array of 64 and an `f`-string per call), `WD_US` before the alarm; then C calls the function once more | yes |
| `fm_top`, `fm_zero` | **`mj_carved` with no `makecontext`**: the same two fibres on quarters of an array in the host's frame, the same `_setjmp`/`_longjmp` switches, each started by an assembly jump of the header's own that sets `sp` and `x29` (`x29` = the stack's top as Darwin's `makecontext` leaves it, or 0 as glibc's and `own.h`'s do) | **none** |
| `so` | **§ 9's abort on a coroutine's stack**: a callback recursing 10,000,000 deep on a `makecontext` coroutine's 256 KiB stack from `malloc`, from `mmap` with a `PROT_NONE` page below it, or (the control) on the thread's own | none |
| `uw` | **a handler left by the unwinder**: the handler steps libunwind's local cursor out through the signal's frame to the caller's frame and `unw_resume`s there (no `setjmp` family, no `setcontext`); then the callback | yes |
| critic's `same`, `other` | an inline jump out of a handler on `main` (`same`) or on a `spawn`ed thread (`other`), then the callback on `main` | yes |

## 3. What ran, first readings (written as the rows came in; § 4 tabulates them)

**The blind key at the base, both platforms**: every cell as the repair's (`p262/M/key-base.txt`,
`p262/linux/L/key-base.txt`): `h1`, `h3`, `h7`, `h9`, `h10`, `h11`, `h13`, `n1`, `n2`, `n3` run; `h2`, `h4`, `h6`,
`h12` STANDING; `h5` SWITCHED; `h8` ONSTACK; `n4` the thread's words; three of three, plain and `--sanitize`.

**Question 4: the depth test on Linux is gone at the base.** Panel 257 measured `p4s` running on Linux and `p4d`
aborting from PAD 5,120 (`docs/panel/257-*.md:56-62`). In the Linux arm64 container at `87003cb42`
(`p262/linux/L/mine-base.txt`), **`p4s` aborts in STANDING's words, and so does `p4d` at PAD 0, 1,024, 4,096, 5,120,
8,192 and 65,536, and `p4x`**: exit 134, three of three, plain and `--sanitize`, every cell. So the first of the two
measurements *may* rests on (an inline assembly exit that runs or aborts by depth) no longer holds on Linux arm64:
the assembly exit aborts at every depth run.

**§ 9's abort on a C coroutine's stack (the critic's C28): it is a segfault, nothing said** (Linux arm64, base,
`so.hero`, three of three each):

| stack the callback recurses on | plain | `--sanitize` |
|---|---|---|
| the thread's own (control) | `panic: stack exhausted in so.deep`, exit 134 | ASan's `stack-overflow` report, exit 1 |
| a `makecontext` coroutine's, from `malloc` (256 KiB) | **`Segmentation fault`, exit 139, the runtime says nothing** | ASan's `stack-overflow`, exit 1 |
| a `makecontext` coroutine's, from `mmap` with a `PROT_NONE` page below | **`Segmentation fault`, exit 139, the runtime says nothing** | ASan's `stack-overflow`, exit 1 |

design.md §1.12 (`docs/design.md:653`): *"A Heroes program must not segfault"*; spec § 9 (line 331): *"running out of
stack aborts"*. Neither holds on a C coroutine's stack in a plain build. What the recursion wrote below the `malloc`
block before the fault is **unmeasured**. `gh issue list --state all --search` for *stack exhausted coroutine*,
*makecontext stack overflow callback*, *running out of stack coroutine*: no such defect among the results (2829,
2830, 2835 and the rest are the room's and the guard's). **A defect owed**, the coordinator's to file.

**`uw` (the unwinder's exit) does not build on Linux arm64**: the image holds no `libunwind.h`
(`ffi_missing_header`, both builds), so its Linux cells are **unrun**.

**Route A on Linux arm64 at the base** (`p262/linux/L/ra-base-{forceA,sim}/`, classified by `arm.sh`; three of three
every cell, plain and `--sanitize` alike; **forced by `HERO_JMP_FORCE_UNCALIBRATED 3` and simulated by 199a's two
switches, the same arm in every one of the 18 cases**):

| case | route C (its golden) | route A |
|---|---|---|
| `2706-...-never-switched-to-runs` | runs | **HELD** |
| `2706-...-reuse-an-ended-coroutines-stack-runs` | runs | **ONSTACK** |
| `2702-a-handler-left-by-setcontext-to-its-coroutine-runs` (`cs_0`) | runs | **UNFOLLOWED** |
| `2702-a-handler-left-by-setcontext-to-the-runner-runs` (`b_0`) | runs | **UNFOLLOWED** |
| `2522-a-callback-after-a-siglongjmp-out-of-a-handler-on-its-own-stack-{deep,shallow}-first-runs` | runs | runs |
| `2848-getcontext-and-setcontext-as-a-jump-on-the-own-stack-then-a-jump-out-of-a-handler-runs` | runs | runs |
| `2707-a-siglongjmp-...-into-a-coroutine-mapped-{after,before}-it-runs` | runs | runs |
| `2705`, `2702-...-own-stack-which-calls-the-function-is-refused`, 2848's four refusals, 2707's two `setcontext` | refused | refused, the same arm |

So on route A: **a `setcontext` out of a handler to a context `getcontext` took on the thread's own stack (`b_0`)
aborts**, and **a `siglongjmp` onto memory once given to `makecontext` (2706) aborts**; a `siglongjmp` onto the
thread's own stack (2522) and into a coroutine from the alternate stack (2707) run. `d-lane-sig-family-1`'s two
correct programs abort under the forced build too, not only under the simulation.

**This Mac at the base** (`p262/M/`): the probes as the repair's (`escape_carved`, `escape_heap` run; `mj_carved`
runs plain and is refused **CALLED** under `--sanitize`, defect 2852). `p4s`, `p4x` and `p4d` at every PAD:
**STANDING, three of three, both builds**, as panel 257's Darwin. **So the assembly exit aborts at every depth run on
both platforms at the base.** `same` STANDING, `other` runs (the thread). § 9 on a coroutine's stack: the control
says `panic: stack exhausted in so.deep` (exit 134); **the `malloc` and `mmap` coroutine stacks end in `Bus error:
10`, exit 138, nothing said**, plain; under `--sanitize` the `malloc` one ends in ASan's own `CHECK failed:
sanitizer_stacktrace.cpp:78 "((stack_top)) > ((stack_bottom))"`, exit 134, three different outputs of three.

**A fourth member of the class, built: `fm_top`.** It is `mj_carved` with `makecontext` and `setcontext` taken out
(each fibre started by the header's own assembly jump with `x29` at its stack's top, as Darwin's `makecontext` leaves
it); **it installs no signal handler**. This Mac, base: plain runs (`body holds 7 fibres 30 32 (7, 30 and 32 are right)`, exit 0); **`--sanitize`:
exit 134, three of three, CALLED**: *"panic: fmtop.run was called from inside a signal handler: C called it while
handling a signal"*. `fm_zero` (the same with `x29` = 0) runs on both builds. On Linux arm64 both run, both builds.
So the walk reads stale words above a carved fibre's top whoever made the fibre; 199f (main) bounds the walk only on
a range `makecontext` handed out (`sigguard.c` on `main`, `hero_sig_coroutine_top` / `hero_rebind_coroutine_top`),
and `fm_top` has none. **Whose words those are**: the walk refuses where a frame record's return word equals the
delivery address the runtime learned; the runtime learns it by raising SIGURG on the calling thread at the first
crossing of a function into C (`sigguard.c:270-275`, `hero_signal_learn`, `:299`), so **the runtime's own probe leaves
a delivery's return address in dead words of the thread's stack** in every program that hands C a function. That is
my reading of the mechanism, tested in § 5 (d3) by moving the probe.
*Corrected 22:20, below in this section*: the probe moved to its own alternate stack, `fm_top` is still refused, and
the diagnostic finds no delivery on that stack: the words are the learning function's own spilled copies of the
address, not a delivery's frame. The sentence above stands as what I first read.

**`uw`, the unwinder's exit, this Mac**: STANDING, three of three, plain and `--sanitize` (the one call of the function is in
`uw_run` after its `raise`; a handler that had returned would have been seen to end, so the resume took place: an
inference from the program, the refusal being the only output). The words *"C left the handler in a way this
runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library)"* are true
of it; the sentence's list (*"a jump no function of the C library makes"*) is not: libunwind is in libSystem on
Darwin, so a reader of the list finds `unw_resume` in none of its five exits, and it aborts.

**Route A forced on this Mac** (`HERO_JMP_FORCE_UNCALIBRATED 3`, read by `rebind_decode.c:308`; `p262/M/ra-base-forceA/`):
**every one of the 18 cases the same arm as on Linux arm64's route A**, three of three, both builds: 2706's two
HELD and ONSTACK, `b_0` and `cs_0` UNFOLLOWED, 2522's two, 2848's two runs and 2707's two runs run. This is the
nearest thing to an Intel Mac this disk holds (route A there by construction, `rebind_decode.c:314-315`); **an Intel
Mac itself is unrun**.

**The (d3) prototype, first try, refuted my reading of the mechanism.** `p262/rt-main-probealt/` is `main`'s runtime
with `hero_signal_probe` (`sigguard.c:270`) running its SIGURG on an alternate stack of its own, zeroed after (+18
lines, `diff | grep -c '^>'`). This Mac: the three DELIVERED goldens of 2494 still refused (exit 134, three of three,
both builds: the delivery is still learned); `mj_carved` runs on both builds (it is `main` + 199f); **`fm_top
--sanitize` is still CALLED**, three of three. So the delivery address the walk finds above `fm_top`'s fibre is not
(only) the probe's; **where it comes from is unknown to me** (no other `raise` or `pthread_kill` in the guard's files,
`grep -n`), and I name no repair for it.
*Answered 22:25, below in this section* (*Where the word comes from*): the runtime's own spilled copies.

**Step 199d's span rule held against its shape beside: a full record refuses a correct program (this Mac, both
columns).** `wide.hero` (`programs/compiler-engineer/wide.h`): a coroutine (made once by `makecontext`, then
`_setjmp`/`_longjmp`, as `mj.h`) on an `mmap`'d stack runs the callback and yields; C then leaves handlers by
`siglongjmp`, one on an alternate stack of its own (a static array) and `WIDE_N` on the thread's own stack, deepest
first; then C resumes the coroutine at the save point it made **before any handler ran**, and the coroutine calls the
callback. No handler's frames are ever jumped back into. **`WIDE_N` 7: runs** (`answer ran for 2`, `result (102 is
right): 102`), six of six. **`WIDE_N` 8: exit 134, REENTERED, six of six on each column (base and `main`), plain and
`--sanitize`**: *"wide.answer was called after C jumped into stack frames the thread had already left (a handler left
by a jump and jumped back into, ...)"*, false of this program. The ninth span widens the first
(`sigentries.c:465-470`; Darwin's twin `rebind_entries.c:398`) to the hull of a span on the static alternate stack
and one on the thread's stack, and `hero_sig_left_holds` (`sigentries.c:508-513`) asks a landing of `[lo, hi)` alone,
never of the span's stack, so a landing on any stack mapped between the two is a re-entry. *"Refuses more and forgets
nothing"* is the comment's own price; the price is a correct program told a false cause (design.md §4.17; the class
`blocking` by `.claude/rules/verification.md` § Bounded discovery: *"a correct program refused"*, *"a false
message"*). Linux arm64: the same, both columns (§ 14). **A defect owed**, the coordinator's to file.

**The assembly exit is not an abort either: `p4j`** (an inline assembly jump out of a SIGUSR1 handler, then a
SIGUSR2 handler left by `siglongjmp` to a buffer `run_case` saved above the first handler's frames, then the callback
`PAD`-deep like `p4x`): **runs, six of six, both columns, this Mac**. A followed jump above a standing entry leaves it
(rule 4, `sigentries.c:29-33`). So **no exit of the five aborts on every program**: the first (assembly) is cleared
by a later followed jump above it, and *aborts* for it would be false.

**`fm_top` on `main`'s runtime (199e and 199f in): still CALLED** under `--sanitize`, three of three (`p262/M/mine-main.txt`);
plain runs. **So the fourth member stands on `main`**, after G.
By level (`main`'s runtime, this Mac, three runs each): `-O0` runs, `-O2` runs, `--sanitize` CALLED, `--sanitize -O0`
**CALLED**: a sanitized build, as 2852's was, and the `run` form's two sanitized legs would both refuse it.

**Where the word comes from, found by a diagnostic copy of the runtime** (`p262/rt-main-diag2/`: `main`'s runtime,
the probe on its own alternate stack, and `write(2)` lines in the walk, the probes and Darwin's trampoline; `p262/diag/`,
this Mac, `--sanitize`): the only deliveries in the run are the runtime's two SIGURG probes, **both on the probe's own
alternate stack** (`DIAG tramp sig 16 frame 0x1002f1850`, room `0x1002e1cc0`); the walk climbs one record from the
thunk (`frame 0x16fc66070`) to the record at the carved array's very end (`0x16fc66140`, the `x29` the fibre started
with), and **both words there, `[top]` and `[top+8]`, hold `hero_rebind_tramp+532`** (`dladdr`), the delivery address
learned. No delivery put them there. Two adjacent copies of the learned value are what `hero_signal_learn`'s locals
`plain` and `info` (`sigguard.c:323-324`) look like spilled in its frame, dead once it returns, at the depth the host's
array later covers: **the runtime's own copies of the address it looks for, read by the walk as a frame record**. That
last step is an inference from the two words and the code, not traced instruction by instruction. Every thunk's
`hero_callback_asked` loads the same values (`sigguard.c:665-668`), so dead copies of them may lie wherever a callback
has been called. **What (d3) would then need**: a walk that never reads a word no frame record put there, which no bound
can give for a fibre the runtime never saw made (199f's bound needs `makecontext`'s range); a validated candidate (the
matched record's frame shown to be the delivery's own) or no walk where no handler was seen installed (which drops the
raw-system-call handlers the walk exists for, `sigguard.c:61-66`). Neither is priced here: I have not built either.

**`main`'s column on this Mac**: the key's 34 cells the same arm as the base's (`arm.sh` over both folders, no line
differs); `p4s`, `p4x`, `p4d` (every PAD), `uw`, `same` refused as at the base; `fm_zero`, `other`, `wide` 7 and `p4j`
run; `wide` 8 REENTERED; `fm_top --sanitize` CALLED; `so` on the coroutine's stacks `Bus error`, exit 138, plain.

## 4. Question 4, exit by exit: what aborts on every build and route, what runs on some, and what decides it

From my runs (above) and the goldens' `.expected` (route C, both platforms, the repair's and mine), route A from my
forced and simulated builds. *"Decides"* is what a reader can see in the program; a cell that a reader cannot see says
so.

| exit of the sentence | aborts | runs | what decides, in the program a reader sees |
|---|---|---|---|
| 1. a jump no C library function makes | STANDING: `h2`, 2522's inline jump, `p4s`, `p4d` at PAD 0 to 65,536, `p4x`, `same`; also the unwinder's `unw_resume` (`uw`, Darwin; libunwind is in libSystem, so it is in none of the five words) | **`p4j`** (a later handler left by a followed `siglongjmp` to a buffer saved above the first); `other`/`h9` (the callback on another thread) | whether a later followed jump lands above the handler's frames on that thread's stack; which thread calls. **The Linux depth test is gone** (panel 257's `p4s` runs there no longer) |
| 2. a `swapcontext` away | SWITCHED: 2702's six swapped-away cases, `h5` | 2702 `a-handler-resumed-by-a-jump-from-its-coroutine-then-left-runs` (the coroutine jumps back into the handler, which then leaves) | whether the handler is resumed before the callback |
| 3. a jump onto a stack once given to `makecontext` | ONSTACK: 2705, `h8`, 2702's own-stack case, 2848's two jumps; **route A**: 2706's two (HELD, ONSTACK) | route C: 2706's two, 2850's and 2851's reuse cases, `h7` (the coroutine ended or never switched to); 2707's two and `h10` (the handler on the alternate stack, into a live coroutine), on route A too | whether the coroutine is live (seen in the program only by reading the library), the stack the handler ran on (`SA_ONSTACK`, visible), **the route (invisible: the architecture, the glibc, an Intel Mac)** |
| 4. a `setcontext` onto a stack once given to `makecontext` | SWITCHED: 2702's alternate-stack case, 2707's two `setcontext`, 2848's two; **route A**: `cs_0` UNFOLLOWED | route C: `cs_0` (`2702-a-handler-left-by-setcontext-to-its-coroutine-runs`, the handler on the coroutine's own stack), `h11` | the stack the handler ran on, the route |
| 5. a jump that lands within it | STANDING `h6` (the handler then calls the function); REENTERED 2855's `rr_2` | 2745's two nested-handler cases (the outer handler, re-entered, returns) | whether the handler re-entered then ends before the callback |
| (none of the five) a `setcontext` to a context `getcontext` took on the own stack | **route A**: `b_0` UNFOLLOWED | route C: `b_0` | the route |
| (none) `siglongjmp` onto the own stack | **`wide` with 8 thread-stack exits** (REENTERED, a widened record); panel 258's jump refusal where the signal landed in the allocator (the watchdog, § 6) | 2522's two, `h1`, `wide` with 7 | how many handlers the thread has left by jumps (8 spans, `HERO_SIG_LEFT`); where an asynchronous signal landed |

**So *may* stays the only true word, for every exit, on both platforms**, but **not for panel 257's reason**: the
Linux depth test is gone (§ 3), and what splits each exit now is the route (A or C), the stack the handler ran on,
whether a coroutine is live, whether a later jump is followed, and the thread. Of these a reader sees `SA_ONSTACK`
and the thread; **the route is invisible in any program**, and so is a coroutine's liveness inside a library. A text
that replaces *may* by *aborts* for any one exit is falsified by the *runs* column of its row.

## 5. Question 2, the property, held against every refusal I produced

**The property, in one sentence a reader could use**: *a callback is refused where, on the thread C calls it on, a
signal handler of C's that the runtime saw begin is held to be running (it was not seen to return, nor left by a
`longjmp`, `siglongjmp`, `_longjmp` or `setcontext` the runtime follows to frames outside it), or where the system
called it as a handler, or where one of the eight frame records above it was entered by a delivery.*

| refusal | a handler of C's began on that thread? | inside the property? |
|---|---|---|
| `h2`, `h4`, `h6`, `h12`, `p4s`, `p4d`, `p4x`, `same`, `uw`, 2522 inline | yes | yes (not seen to end) |
| `h5`, 2702 swapped-away, 2707/2848 `setcontext` | yes | yes (switched, not seen back) |
| `h8`, 2705, 2702 own-stack, route A's 2706 and `cs_0`/`b_0` | yes | yes by the runtime's judgement (not followed); on route A `b_0` is a handler the program left correctly |
| **`wide` 8** (REENTERED) | yes, nine, **every one left by a followed `siglongjmp`** | **no**: no handler is held running; the record's widened span says one is |
| **`fm_top --sanitize`** (CALLED), both columns, Darwin | **none** | **no**: no handler, no delivery on that stack; the runtime's own copies of the delivery address |
| `mj_carved --sanitize` (CALLED), base, Darwin | none | no (defect 2852; **runs on `main`**, 199f) |
| `n4` (the thread's words) | none | outside the sentence: a thread C made, whatever C did with signals |

**The runtime's own handlers**: `h13` (`hero_signal_watch`, the remedy) runs on both platforms and columns; the
stack guard's SIGSEGV handler can have run in no program here but `so`, which died by the signal with nothing said
(whether the handler ran there I did not trace). They matter to
the property only as the source of a delivery address; in `fm_top` none was delivered on the stack read.

**The class, four members by four mechanisms**: 2831 (an unseen `makecontext` through `dlsym`, closed), 2855 (rule 6 on
a carved stack, repaired at the base), 2852 (the walk on Darwin's fibre frame pointer, repaired on `main`), and
**`fm_top`** (the walk reading the runtime's own dead copies of the delivery address above a fibre no `makecontext`
made; open on `main`). A fifth, `wide`, is outside the no-handler class but is the same kind of error: a correct
program told a handler's words that are false of it. **So a sentence promising that a program with no handler of C's
is never told these words is false today on `main`**, under `--sanitize` on Darwin, by a program I ran.

## 6. The watchdog's rate (this Mac and Linux arm64)

`wd.hero` (a `SIGALRM` handler `siglongjmp`s out of a C loop calling a Heroes function that allocates; then one more
call), `wdrate.sh`, 34 runs per build and column, `WD_US` cycling over 17 delays from 300 to 25,000 µs
(`p262/M-wd/class-*.txt` and `p262/linux/L-wd/class-*.txt`, each line *delay exit class*):

| column | plain | `--sanitize` |
|---|---|---|
| this Mac, base `87003cb42` | **6 of 34 stopped at the jump**, 28 run | **9 of 34 stopped**, 25 run |
| this Mac, `main` `c2b7574bf` | **6 of 34 stopped**, 28 run | **7 of 34 stopped**, 27 run |
| Linux arm64, base | **3 of 34 stopped**, 31 run | **9 of 34 stopped**, 25 run |
| Linux arm64, `main` | **2 of 34 stopped**, 32 run | **11 of 34 stopped**, 23 run |

**28 of 136 runs (21 %) on this Mac and 25 of 136 (18 %) on Linux arm64 end at exit 134 in panel 258's words**, *"panic: a signal handler jumped out of the code it
interrupted while that code was inside this runtime's allocator, which nothing will ever finish ... so the program
stops here, before the jump"*; the other 108 and 111 print `after the jump: 75`, exit 0 (`p262/M-wd/`, `p262/linux/L-wd/`). No other outcome: no guard D, no
handler sentence's words, no crash. The stopped runs spread over 13 of the 17 delays on this Mac and 14 on Linux arm64 (`awk` over the class files), so the outcome is where the
signal landed, which no reader of the program can see. **Every text that promises a run after a `siglongjmp` out of a
handler onto the thread's own stack (x1, x2, (b)'s *"known to have ended"*, (d1)) is falsified by this program on both
platforms, both builds, both columns**, unless it is scoped to a signal that interrupted C and not the runtime, or says that
a jump out of the runtime's own code stops the program. POSIX makes this program's C undefined only where the alarm
interrupts a call that is not async-signal-safe (`sigguard.c:184-185`, panel 258's reading); a reader cannot tell
from the program when that is.

## 7. Question 3 and item 5: every run-promise against its falsifiers

**The warden's texts**: none stands in `.claude/worktrees/panel262/docs/panel/262-reports/` at 22:32 (`ls`: the critic's,
the historian's, `programs/`), and I read no other seat's directory. So **no warden text is built or run here**; each
family of § 4 is held to the programs below, and a text the warden writes is judged by the row its clauses fall in.

| clause | falsifier I ran | platform, build, route, thread | verdict on the clause |
|---|---|---|---|
| x1's and x2's *"`longjmp`, `siglongjmp`, `_longjmp` ... onto the thread's own stack ... a callback then runs"* | `wd.hero`: 28 of 136 (this Mac) and 25 of 136 (Linux arm64) stopped at the jump (panel 258) | both platforms, both builds, both columns, route C, one thread | **false** |
| the same | `fixedbugs-2705`, `2702-...-own-stack-which-calls-the-function-is-refused`: ONSTACK (a coroutine carved from the own stack is the own stack by address) | both platforms, both builds, routes C and A | **false** |
| the same | `wide.hero` with `WIDE_N` 8: REENTERED | this Mac, both builds, both columns (Linux arm64: § 14) | **false** |
| x1's *"or a `setcontext` to a context `getcontext` took ... onto the thread's own stack, and a callback then runs"* | `b_0` (`2702-a-handler-left-by-setcontext-to-the-runner-runs`): UNFOLLOWED | Linux arm64 forced and simulated route A; Darwin arm64 forced route A; both builds; both columns on Darwin, base on Linux | **false on route A** (an Intel Mac, glibc's other architectures, a moved glibc) |
| x2's *"or `setcontext` onto the thread's own stack, is over, and its callbacks run"* | `b_0`, as above | as above | **false on route A** |
| (b) *"... not known to have ended"*, if it lists what ends a handler (a return, a followed jump above it) as ending it for every build | the watchdog, 2705, `wide` 8, `b_0` on route A | as above | false wherever it promises a run |
| (a)'s second sentence, or (d3)'s simpler one: *a program whose C installs no signal handler is never told these words* | **`fm_top --sanitize`: CALLED**, *"called from inside a signal handler"* | this Mac, `--sanitize`, both columns (`main` included, after G); route C | **false on `main`** |
| (c)'s *"a switch with no handler of C's running is none of these"* | `fm_top`, as above | as above | **false** |
| (c)'s *"on its thread"* | none: `other`/`h9` run, `same` STANDING | both platforms, both builds, both columns | **true** (narrows the *may*, promises nothing) |
| any *aborts* in place of *may*, for any one exit | `p4j` (exit 1), 2702's resumed handler (exit 2), 2706/2707/`h7`/`h10` (exit 3), `cs_0`/`h11` (exit 4), 2745's nested (exit 5) | route C, both platforms (`p4j`: this Mac; Linux arm64: § 14) | **false** |

**Which real systems stand on route A** (read, not run): `sigjump.c:45-53` (glibc's architectures but aarch64 and
x86-64, a glibc whose layout moved, a forced build) and `rebind_decode.c:314-315` (**Darwin x86-64, by construction**).
The CI's legs (`ci.yml`: `ubuntu-26.04`, `ubuntu-26.04-arm`, `macos-26`, `windows-latest`) run no forced build and no
glibc architecture of route A; `macos-26` is the Darwin arm64 leg by the workflow's own comment (`ci.yml:252-259`, read). **An Intel Mac is unrun**; Darwin arm64 forced onto
route A gives the same arm as Linux's route A in all 18 cases, which is the nearest evidence and remains an inference
for an Intel Mac.

## 8. Item 6: the runtime routes, priced (lines by `wc -l` at the base; nothing below is landed or proposed as a repair in my copy beyond the two prototypes named)

**(d3), simpler sentence by a runtime change** (*"a program whose C installs no signal handler is never told these
words"*). What it needs, measured: E3 (step 199d, at the base) and G (step 199f, on `main`) are **not enough**:
`fm_top` is refused on `main`. The rest: (i) a walk (`sigguard.c:701-711`, base) that never takes a dead word for a
frame record's, for fibres no `makecontext` made; the one prototype I built (the probe on its own alternate stack, +18
lines in `sigguard.c`'s `hero_signal_probe`) **does not close it**, because the copies are the runtime's own spilled
locals (§ 3); a validated candidate or a walk gated on a seen install is unbuilt, and either changes the thunk's path,
whose cost (`cost.sh` of panel 259, instructions retired at `-O2`) **I did not run**; (ii) the thread's words said
apart (`n4`: a thread C made is refused with no handler at all, `thread.c:132-143`); (iii) 2831's class stays closed
(the four mechanisms above came in one day, so the base rate says a fifth is likely, an inference). **Lines: unknown,
because the repair is unknown.** The runtime's own handlers do not bear on it: `h13` runs and no delivery reached the
stack read in `fm_top`.

**(d3), the other direction: a runtime that always refuses after an exit it does not follow, so *may* becomes
*aborts***. The correct programs it would refuse that run today, from § 4's *runs* column: `p4j`; `h7`, `h9`, `h10`,
`h11` of the key; and of the goldens, by their names and `.expected` (`grep -L 'panic:'` over the eight families):
2702's `setcontext`-to-its-coroutine, resumed-handler and stale-range cases, 2706's two, 2707's two `siglongjmp`
cases, 2745's two nested re-entries, 2850's and 2851's reuse cases: **at least 12 run goldens and 5 programs of
mine and the key**, each pinned as running (the goldens by their `-runs` names and `.expected`; one header read whole). Which of them the runtime "does not
follow" is a reading of the rules, not run apart. A widening of refusals of correct programs: §1.12 does not ask it.

**(d8), abort at the exit the runtime sees.** Sites: rule 5's swap (`sigentries.c:34-38`; the hooks in
`sigjump_save.c`, 305 lines, and Darwin's `rebind_jump.c`, 360), rule 6's `setcontext` onto a coroutine
(`sigentries.c:39-47`), and the jump into a live coroutine (`hero_sig_prune_to`'s `unsure` 3, `sigentries.c:570-603`
on the base, and `rebind_entries.c`'s twin): three sites on each of two platforms, plus words and cases; unbuilt.
**It refuses correct programs that run today**: 2702's `a-handler-resumed-by-a-jump-from-its-coroutine-then-left-runs`
(its header: *"A handler ... swaps to a coroutine ... and the coroutine resumes the handler by `siglongjmp` ... Correct:
the handler was resumed and then left"*), `cs_0` and `h11` (a `setcontext` onto the coroutine's stack from a handler
on that stack), 2707's two and `h10` (a jump into a live coroutine from the alternate stack): **6 programs**, plus every
handler that swaps to a coroutine and back, which rule 5 exists not to refuse: at a `swapcontext` the runtime cannot
know whether it comes back (SWITCHED's own words, `sigguard.c:612-615`). So (d8) cannot say *aborts* for exit 2 without
refusing the switch that returns.

**(d9), a told route.** `heroes-next doctor` at the base prints eight lines (`clang`, `clang floor`, `xcode CLT`,
`swiftc`, `arch`, `runtime`, `archiver`, `build cache`) and **no route** (run, this Mac); `hero_signal_jump_route` is
declared in no header of `runtime/` (`grep -rln`: `sigjump.c:201`, `rebind.c`, `rebind_decode.c`, and the 2679
golden's own header). So the spec would name a fact no tool prints: it needs a `doctor` line (`selfhost/cli/doctor.hero`,
505 lines, that builds and runs a probe against the runtime, since the route is decided at run time by the
calibration) and a public declaration in `hero_os.h`, a promise for ever; a CLI surface change, its own sitting
(`.claude/rules/cli-surface.md`). Unbuilt.

**(d4), a told end**: panel 259's precedent stands against it (`docs/panel/259-*.md:200-205`); I built nothing.

## 9. Item 7: the instrument and the placement

`tests/harness/spec_refusals.hero` (222 lines) finds the sentence by `SENTENCE_OPENS` (`:39-40`, *"A callback called after
C left a handler of C's"*), ends it at the first `". "` (`:68`), asks `CLAIMS` (`:32-36`) of it and of every arm but
`EXEMPT` (`:43-44`), and its test reads the real spec and the real `sigguard.c` (`:213-222`). The compiler's 2533 test
(`selfhost/emit/callback_thunk.hero:407-429`) cuts the remedy from *"No function of the program is a signal handler"*
**to the end of its line**, so any candidate placed after the remedy on the same line breaks it; before it, nothing.
What each family owes the instrument, by reading the code (none run, for want of a text): (c) with a clause that
neither precedes the opening nor holds `". "`: nothing; (c) inserting *signal* (*"a signal handler of C's"*): the
opening moves, `SENTENCE_OPENS` first; (a) two sentences: the instrument reads one, so a list of openings and a claim
set per sentence (estimate, unbuilt); (b) with new claim words: `CLAIMS`, the six arms that carry *seen to end*
(`grep -c` in `sigguard.c`: 6) and **91 `run` goldens** (`grep -rlE 'seen to end|from inside' tests/golden/run
--include='*.expected' | wc -l`: 91; 85 and 32 for each claim alone); the `check` case *package-a-file-source-in-no-repository*
lies outside `tests/golden/run` and is no refusal. **Unrun**: the `spec` suite, the net's own tests and the compiler's
own tests with any warden text applied, since no text exists in the sitting's folder.

## 10. Defects owed (found here, filed by nobody: the coordinator's)

1. **`fm_top`**: a program with no signal handler and no `makecontext`, two fibres carved from a C frame's array
   started with `x29` at their stacks' top (as Darwin's own `makecontext` leaves it), refused **CALLED** under
   `--sanitize` on Darwin, base and `main`; the walk reads the runtime's own dead copies of the delivery address
   (`sigguard.c:323-324`, `:665-668`) above the fibre. Class by `.claude/rules/verification.md` § Bounded discovery:
   *blocking* (a correct program refused, a false message). Beside 2852 (G), which it survives.
2. **`wide` 8**: nine handlers left by followed `siglongjmp`s (one on a static alternate stack, eight on the thread's
   stack) fill the record of spans (`HERO_SIG_LEFT` 8, `sigguard.c:362`), the widened first span holds every address
   between the two stacks (`sigentries.c:465-470`, `rebind_entries.c:398-403`), and `hero_sig_left_holds`
   (`sigentries.c:508-513`) never asks the span's stack: a coroutine resumed at a save point made before any handler
   ran is refused **REENTERED**, false. Both platforms, both builds, base and `main` (§ 14). *Blocking*.
3. **`so`**: a callback recursing past a C coroutine's stack (`malloc`'d or `mmap`'d with a guard page) ends in
   `Segmentation fault`, exit 139 (Linux arm64) or `Bus error: 10`, exit 138 (this Mac), plain, **nothing said**; the
   thread's own stack says `panic: stack exhausted in so.deep`. design.md §1.12 (*"must not segfault"*) and spec § 9
   (*"running out of stack aborts"*). What it wrote below the `malloc` block is unmeasured. *Blocking*.

## 11. Verdict

- `verdict`: **veto**, on soundness, of every text that promises (i) a run after a `longjmp`, `siglongjmp`, `_longjmp`
  or `setcontext` out of a handler onto the thread's own stack (x1, x2, (b)'s *"known to have ended"* where it lists
  ends, (d1) where it promises, (d2) for an Intel Mac), (ii) that a program whose C installs no signal handler is never
  told these words ((a)'s coroutine sentence, (d3)'s simpler sentence, (c)'s *"a switch with no handler of C's running
  is none of these"*), or (iii) *aborts* for any of the five exits ((d3) strict, (d8) as worded); **object** to (d8),
  (d9) and (d4) on what they refuse or promise for ever; **approve** (c) with a clause that promises nothing positive
  (*"on its thread"*, measured true), keeping *may*.
- `section`: design.md §1.12 (`docs/design.md:646-655`: *"must not segfault and must not corrupt memory"*; panel 251's
  veto, no Heroes code inside a signal), §4.17 (`:2381`, a message carries what is needed to fix the mistake it names:
  `fm_top`'s and `wide`'s words name a cause the program does not have), §4.19 (`:2658`, the FFI). design.md says
  nothing of the guard's rules (`grep -n -i` of *seen to end*, *swapcontext*, *makecontext*: nothing, the brief's), so
  the soundness ground is §1.12's alone.
- `implementation_cost`: a text alone moves no runtime line; (c) keeping the opening and the claims moves nothing in
  `spec_refusals.hero` (222 lines); a new opening moves `SENTENCE_OPENS` (`:39-40`); new claim words move `CLAIMS`
  (`:32-36`), six arm strings of `sigguard.c` (`:598-644`) and 91 `run` goldens; (a) needs the instrument to read two
  sentences (unbuilt). (d3): unknown (the one prototype, +18 lines, does not close `fm_top`); (d8): three sites on
  each of two platforms (`sigentries.c` 673, `rebind_entries.c` 647, `sigjump_save.c` 305, `rebind_jump.c` 360) and
  at least 6 correct programs refused; (d9): `selfhost/cli/doctor.hero` (505) and a public accessor in `hero_os.h`.
  The thunk's path moves under none of the text routes; under (d3) it would, unmeasured.
- `needed_for_self_hosting`: no.
- `argument`: Every run-promise in the record is false today on a program I ran: the watchdog is stopped at its jump
  in 53 of 272 runs; a coroutine carved from the own stack is ONSTACK; nine followed exits make a correct resume
  REENTERED; on route A a `setcontext` to the runner is UNFOLLOWED. The no-handler promise is false on `main` after G:
  `fm_top` is CALLED by the runtime's own spilled copies of the delivery address. *Aborts* is false for every exit, so
  *may* stays, though panel 257's Linux depth test is gone: each exit now splits by route, the handler's stack, a
  coroutine's liveness, a later jump and the thread, and no program shows its route.
- `prediction`: at the landing's commit, on this Mac, `wd.hero` (34 runs per build, `WD_US` cycling as `wdrate.sh`)
  ends at exit 134 in panel 258's words in at least 3 of 34 runs of each build, and `fm_top --sanitize` ends CALLED
  three of three unless a repair of the walk lands first; checkable at M-rebel-library's tag.
- `condition`: a warden text whose every run-promise is scoped to what holds on every route (route C named, or the
  promise withheld), to a signal that interrupted C and not the runtime, and to a thread that has left fewer than nine
  handlers by jumps, run by the programs above to a run on both platforms; or repairs of `fm_top`, `wide` and `so`
  landed and re-run, after which (a) and (d3) may be judged again on what remains.

## 12. What stays unrun, said

- **Every warden text** (none in the sitting's folder at 22:32), so the `spec` suite, the net's own tests and the
  compiler's own tests with a text applied; `heroes test` has no filter by title (`heroes-next test --help`), so the
  2533 test alone was not run either.
- **The thunk's cost** (`cost.sh` of panel 259): no route I priced was built far enough to change the thunk.
- **A handler installed by a raw system call with a restorer of its own on Linux x86-64**: the x86-64 image is
  emulated and its stage 2 was not built at a load of 150 (`uptime`); the critic's inference stands as one.
- **`_Unwind_ForcedUnwind` itself**: I built the unwinder's resume (`unw_resume`, libunwind's local cursor), not a
  forced unwind; whether a C header's frames (no `-fexceptions`) give a forced unwind anywhere to stop is a reading I
  did not test. `uw` is unrun on Linux arm64 (no `libunwind.h` in the image).
- **An Intel Mac**, glibc's riscv64, loongarch64 and 32-bit arm, musl, FreeBSD: route A there is read from
  `sigjump.c:45-53` and `rebind_decode.c:314-315`; my route A is forced or simulated on arm64.
- **Windows**: read only. `sigguard.c:714-728` (base): no probe, no walk; `hero_callback_entered` counts and returns,
  so no callback there is told the handler sentence's words, whatever C did (the thread's guard aside).
- **What `so`'s recursion wrote below the `malloc`'d stack before the fault.**

## 13. Item 1: every cell where the platforms, the builds or the runtimes differ (all rows above, both columns)

Compared by `arm.sh` (each output classified by its arm, or RUN, or the exit), three runs per cell:

- **The two runtimes (base `87003cb42`, `main` `c2b7574bf`)**: one cell differs, **`mj_carved --sanitize` on this Mac**
  (CALLED at the base, runs on `main`: 199f). Every other cell is the same arm: the key's 34 on each platform, my
  programs, the probes, and route A's 36 cells forced and 36 simulated on Linux arm64 and 36 forced on this Mac.
- **The two platforms**: `fm_top --sanitize` (this Mac CALLED, Linux arm64 runs; both columns); `mj_carved
  --sanitize` at the base (the same split); `so` on a coroutine's stack (exit 138 `Bus error` here, exit 139
  `Segmentation fault` there; under `--sanitize` ASan's reports, here once its own `CHECK failed`); `uw` (STANDING
  here, unbuildable there). The key: none. Route A: none (18 cases, forced on both).
- **The two builds**: `fm_top` (plain runs, `--sanitize` CALLED, this Mac); `mj_carved` at the base (the same); `so`
  (plain: a signal, nothing said; `--sanitize`: ASan's report). Nothing else.
- **The two routes**: 2706's two, `b_0`, `cs_0` (§ 3), both platforms.

## 14. The second Linux arm64 container (`p262/linux/L2-{base,main}/`, `L2.txt`)

**`wide` with `WIDE_N` 8: REENTERED, six of six on each column** (plain and `--sanitize`), the same words as on this
Mac; `WIDE_N` 7 runs, six of six; **`p4j` runs, six of six on each column**. So the full record's false refusal and the
assembly exit cleared by a later followed jump hold on both platforms, both builds, both runtimes. No container of
mine is left (`docker ps -a | grep p262-compiler`: none).
