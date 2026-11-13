# 247: a Heroes function a signal reaches is refused at its thunk's first line, the allocator's guard stays beneath it, and the spec names the runtime's doors that count a signal and stop a server

**Status** a full sitting, convened 2026-11-13 by the coordinator of M-core-packages under CLAUDE.md
§ 4 (a sitting asked for once per milestone; this milestone's run from 234), on defect 2494, which
lane b34-signals of batch 34 stopped on: *"every complete repair refuses a program that works today,
so it needs a sitting's ruling"* (`247-briefs/00-shared.md`, its first paragraph). It rules **how**
panel 246's R4, ratified 2026-11-12 (*"no Heroes code runs in a handler"*), is enforced or honoured,
never whether (`00-shared.md` § What the sitting rules). The one paid run is the blind seat's, under
the author's yes of that morning, *può spendere fino a 25* (*it may spend up to 25*): **21.0131 USD**
in all, summed from the 96 `run.json` files under `p247-scratch/s/` (the runners' `DONE` lines and the agent's sum). The
resolution is `provisional — author ratification pending` (§ Author's verdict).
**Seats** the full panel: compiler-engineer, ffi-pragmatist, spec-warden, historian (advisory),
llm-ergonomist (the blind seat, forty sessions and three follow-ups of twenty-four, twenty-four and eight). The completeness
critic read the briefs before any seat (23 items, `grep -c -E '^[0-9]+\. '` over its report; the
briefs repaired on them, the first versions kept as `247-briefs/*.before-critic.md`) and the reports
after (12 items).
**Base** `ae5e2cedddee93e0d53715a5b5ae6d7833c569df` (tree `3e22d534...`), the head of batch 34's
round when the briefs were written, its stage 2 printing `sources 7cc70d136b590d46`; with D
(`932876f95`, batch 34) applied, `sources 560c95206c908459`. Each seat built its own in
`.claude/worktrees/p247-scratch/247-<seat>/`. D and the rest of batch 34 reached `main` at
`09685b1bc` while the sitting sat.
**Briefs** `docs/panel/247-briefs/`, **reports** `docs/panel/247-reports/`, **evidence**
`docs/panel/247-reports/evidence.tar.xz` (its note beside it).

## The proposal

Verbatim in `247-briefs/00-shared.md`, § The routes (lines 311 to 391) and § The questions (393 to
428); in short:

- **The defect**: a function of the program handed to C as a signal handler, which spec § 13 allows
  today, runs Heroes code inside the signal, where it reaches C that is not safe there: the runtime's
  allocator (closed by D), `snprintf` in every integer formatted (`hero_int_to_str`), stdio, `exit`,
  the handle, lease and lend tracking. On Darwin libplatform kills the process with SIGKILL and no
  word (`os_unfair_lock`'s recursive-lock check); on Linux the same run goes on (question 7).
- **D**, landed in batch 34: the allocator's re-entrance guard, the floor every route builds on.
- **A**, a refusal at build by the callee's name (`signal`, `sigset`, `bsd_signal`). **C**, a
  refusal at run time when the system's delivery calls a Heroes function (protoC: a SIGURG probe
  learns the delivery's return address, a walk of six frame records looks for it). **E**, the
  Heroes function run later on a thread of the runtime's. **S**, run later at a safe point of the
  program's own thread. **F**, C's detection with E's or S's delivery. **G**, a door of the
  runtime's that counts a signal other than SIGINT and SIGTERM. **H**, a refusal by the handler's
  body. **I**, the handler made safe in place.
- **The spec's drafts** (`247-briefs/spec-warden.md`, its table): **hr** *"A function given to
  `signal` stops the program the first time a signal calls it: no code of the program runs inside a
  signal handler."* (+29 vendored); **hrr**, hr continued *"..., so write the handler in C, in a
  header the program ships."* (+43); **ha** route A's (+36); **he** route E's (+25); **hs** route S's
  (+35); and the spec-warden's own, **hm6**, the callback sentence's *"never a result;"* become
  *"never a result or a signal handler, nor called by one;"* (+9).

## The verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve** C in the thunk form (exact test, bounded walk, refusal async-signal-safe) **with D**; **veto** D alone, A alone, E by name, H, I; **object** to F, S, and A's declaration form beside C; **approve** G as a complement | design.md §1.12, §1.1, §1.7 and Part 5, §1.3 against F; CLAUDE.md § Precedence rank 3 over rank 5 | as built: `callback_thunk.hero` +28; `parts/thread.c` route C's part 200 lines; `heroes_runtime.h` +5; four guard headers +2 each; `guarded_names.hero` 4 names; no checker change, no IR change, no core construct, no diagnostic class | (1) `clocale` and `wraplocale` refused 20 of 20 each, 40 SIGABRT reports and no SIGKILL; (2) `qsortcost` at `-O2` at most 2,032,000,000 instructions with the walk; (3) at most +250 lines under `runtime/` and +40 under `selfhost/emit/` | (a) a POSIX leg where the exact test misses a direct delivery; (b) the author choosing to honour R4 rather than enforce it: F; (c) a walk at or under (2) and not blind under `--sanitize` |
| ffi-pragmatist | **approve** C in the thunk form **with its repair** (a thread-local depth kept by the runtime's own definitions of `sigaction`, `signal` and `sigset`; `SS_ONSTACK`; the walk optional), D the backstop, G the door; **veto** C as built with the walk the only answer to F's case, D alone, A alone, A's declaration form, E by name, H, I; **object** to F and S | §1.11, §4.19, §1.12; CLAUDE.md § Precedence rank 3 over rank 5 | the repair 2.0x the callback cost on `qsortcost` (walk 0), the walk 3.7x; G `parts/sigdoor.c` 71 lines and three declarations in `hero_os.h` | (1) `fliblocale` and `fownlocale` refused 20 of 20 on Darwin and Linux arm64, and without the depth and `SS_ONSTACK` `-9` in at least 18 of 20 on Darwin; (2) a comparator handed to glibc's `qsort` from a C handler refused at its first call; (3) glibc's stdio re-entered corrupts its `FILE` in at least 9 of 10 | (a) `siglongjmp` out of a handler; (b) an installer the interposition cannot define; (c) the Windows box; (d) the author honouring R4: the same answers are F's selector |
| spec-warden | **object** to hr, hrr, ha (provisional); **veto** he and hs (not provisional, Principle 0); **approve** hm6 (provisional, with a refusing route) | design.md §1.6, §1.2, §1.0; CLAUDE.md § 2 and § 12; `.claude/rules/spec-shape.md` | hr +29, hrr +43, ha +36, he +25, hs +35, hm6 +9, hm +4 (vendored maximum, a lower bound); 5216 free after the FFI floor | SW1 to SW5 (§ Predictions to score) | hrr: SW4 falsified, or the remedy worded to one module and the trap told by the compiler, and panel 114's R3 gate met |
| historian | **approve (advisory)** a deferring route (S or E, with F's selection) with D the backstop and a door for the program's own C handler; **object (advisory)** to A as R4's only enforcement, and to D alone, H and I | precedent | | | a runtime that kept running handlers in the signal behind a body check; a deferring runtime that went back; a refusal by name that sees a wrapper's `sigaction` |
| llm-ergonomist | an experiment, graded by its registered rules (§ The blind seat) | design.md Part 11 | 21.0131 USD | the decision rules, registered before any session | a `context: yes` voids a session: none in 96 |

## What each seat measured, in short

- **compiler-engineer** (`247-reports/compiler-engineer.md`): route C in the thunk form, built
  (`heroes-c2`, `sources 1f83fdf86f0d2855`): **22 refusals of 22** said why, every crash report the
  refusal's own `raise`; every installer seen with no list, the program's own `sigaction` (`wrap`)
  included; a Heroes call pays nothing (`guarded.hero` unmoved). Under D alone `clocale` (one C call,
  `localeconv`, no Heroes value built) and `wraplocale` died **10 of 10 by SIGKILL, nothing on
  stderr**; under A alone `wraplocale` passes `check` and dies 5 of 5. F built (`heroes-f`): runs
  `quiet`, `t3x1` and the storm to exit 0, `t3x1` a tick late in 1 of 3, the 2496 case passing while
  its words are false (0 of 200 on the worker). S's cheapest polls +4.8% on the compiler's own check.
- **ffi-pragmatist** (`247-reports/ffi-pragmatist.md`): the exact test is exact, 2.3 million
  deliveries on two kernels, one return address each, never a false match. The walk misses a library
  built `-fomit-frame-pointer` (`flib-nofp`) and a handler on an alternate stack the program made
  (`fown`): under a storm `c-fownlocale` and `c-fliblocale` died **6 of 6 by SIGKILL in silence** on
  Darwin, and ran inside the signal on Linux arm64. `sigaltstack(NULL, &now)`'s `SS_ONSTACK` answers
  every alternate-stack row exactly (§ 5). Interposing `sigaction`, `signal` and `sigset` saw the
  static library without frame pointers and missed a dylib (Darwin's two-level namespace). G built
  and run: `gcount` and `gtwo` (another module takes the count) exit 0 under two storms on Darwin
  and Linux arm64, counted 606,994 to 1,532,787.
- **spec-warden** (`247-reports/spec-warden.md`): every draft priced (vendored, its own script); a
  reader's plausible shapes run under D (`swp1` to `swp7`); **`swp6`, one header with a `static`
  flag bound by two modules, ignored SIGINT 2 of 2 at exit 0** under every route: a C handler in a
  header the program ships fails in silence across modules. § 13's `atexit` example and the
  four tracked programs that give `signal` a function (§ 9).
- **historian** (`247-reports/historian.md`): of the runtimes read that say where a program's handler
  runs, none runs it inside the signal by default; Perl moved its handlers out at 5.8.0 for 2494's
  class; Python, Ruby, Tcl, Lua and OCaml defer to a safe point; Go, Java, .NET, GNAT and Apple's
  Dispatch use a thread they own; Go's maintainers answered 2494's exact shape *"this will never
  work"*, a byte down a pipe instead; ThreadSanitizer selects handlers by interposing the
  registration calls, a shape the brief did not list.
- **The critic's second pass** (`247-reports/critic-reports.md`): arm x's baseline is mostly the
  FFI's refusals, not readers writing Heroes handlers; with defect 2511 modelled as a call of the
  function, x and m read 4 of 8 and arm e no longer enters; the spec's `atexit` example is refused
  by its compiler (defect 2513); arm r's credit is hrr's remedy, and all eight r handlers are the
  `swp6` shape; the ffi-pragmatist's interposition, re-run, **blinds the exact test** (its probe
  installs through the runtime's own `sigaction`), lets a library installing through `bsd_signal` or
  `sigvec` run inside the signal, and **breaks `sigset(SIG_HOLD)`** (exit 134, a false message); G
  declines SIGINT and SIGTERM and no reader of forty found any door.

## The blind seat, graded by its registered rules

**The forty** (`247-reports/llm-ergonomist.md`, the coordinator's assembly of its runner's grading;
registered in `247-briefs/llm-ergonomist.md` before any session): five arms, x the base, r **hrr**,
e **he**, m **hm6**, s **hs**, two tasks (t1, stop a loop cleanly on Ctrl-C; t2, count SIGUSR1 under
a storm while `main` works), four sessions each; graded under today (the base with D), C and E
(route F, E's delivery with C's detection; no S built, so s is provisional). `right` of 8:

| arm | today | C | E | the registered rule (+3 over x, under the arm's own route) |
|---|---|---|---|---|
| x | 1 | 1 | 1 | |
| r | 8 | 8 | 8 | +7, enters |
| e | 1 | 0 | 4 | +3, enters |
| m | 0 | 0 | 0 | -1, does not enter |
| s | 4 | 0 | 8 | +7, enters (provisional) |

**Read with the critic's items 1 to 4**: seven of x's eight wrong answers are refusals of the FFI
(three `internal error` at exit 2, defect 2511's `sigemptyset` macro; two `empty_record`; one
`ffi_constant_type`; one parse error), and one x reader wrote a Heroes handler; with 2511 modelled as
a call of the function, x and m read 4 of 8 each, r +4 still enters, **e does not enter**, s +4. So
the rule measured *"readers had no working path"* more than *"readers wrote Heroes handlers"*. Arm
r is route-independent (8, 8, 8): its credit is hrr's remedy clause, not route C. The two-module
shape was never a task of the forty.

**The follow-up, twenty-four sessions** (registered 2026-11-13 at about 11:30 before any of its
sessions, `247-briefs/llm-ergonomist.md` § Follow-up): arms **r** (hrr) and **g** (**hg3**, below),
both with § 13's `atexit` example corrected to `-> i32`; tasks t1, t2 and **t3, two modules** (a
stop asked in one module and read in another); four sessions per cell; graded under today with D and
G. The rule: **g replaces r if g gives at least three more `right` than r over its twelve
sessions**; otherwise r stands; t3 reported on its own.

> **hg3**: No function of the program is a signal handler: after `hero_net_catch_stop() -> i64`
> (`hero_net.h`), Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1 and a waiting
> call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any
> other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call.

| run | arms | rule | `right` | verdict |
|---|---|---|---|---|
| the forty | x, r, e, m, s | an arm enters at x + 3 or more, under its own route | above | r enters; m does not |
| first follow-up | r (hrr), g (hg3) | g replaces r at r + 3 or more of 12 | r 10, g 12 | **r stands** (+2); r's two losses a race in the driver; 19 of 20 r answers the `swp6` shape; t3 never formed the trap |
| second follow-up | m2 (hm6), g (hg3) | hg3 replaces hm6 at m2 + 3 or more of 12, 2511 modelled | m2 2, g 12 | **hg3 replaces hm6** (+10); ten of m2's answers refused `empty_record` (defect 2514) |
| third follow-up | m2, g, on t4, a server | the stop clause measured on a server-shaped task at m2 + 3 or more of 4 | m2 0, g 4 (of 4) | **the stop clause is measured** (+4); two m2 answers refused `empty_record`, one gave no answer, one caught no signal |

Every g reader (twelve, twelve and four) used a runtime door, and none wrote a
Heroes handler or a C header; every m2 reader blocked the signals and asked `sigpending`; 19 of 20 r
readers kept their flag `static` in a header. Spend in all 21.0131 USD of the author's 25.

**Context**: 95 of 96 answered `no` to whether any project rule, contract or memory reached them, and one, t4-m2-3, gave no answer, having hit its budget; none voided.

## The resolution adopted

`provisional — author ratification pending`. Robust over conservative wherever the two differ, the
conservative form recorded in § Conservative alternatives.

**R1. Route C, in its exact form, enforces panel 246's R4.** At the first line of every thunk
(`selfhost/emit/callback_thunk.hero`, the one door from C into Heroes code, so a Heroes call and a
program handing C no function pay nothing), three questions, the cheapest first:

1. **the exact test**: the thunk's own return address against the delivery's, learned at start for a
   plain and an `SA_SIGINFO` handler. It sees every installer with no list: `signal`, `sigset`,
   `bsd_signal`, `sigvec`, the program's own `sigaction` (`wrap`), the alternate stack,
   `SA_SIGINFO`;
2. **where the thunk's frame lies outside the thread's own stack, the kernel's answer**:
   `sigaltstack(NULL, &now)` and its `SS_ONSTACK`, for a handler on an alternate stack the program
   made (`fown`, `fownlocale`), in place of the walk's alternate-stack bound (the ffi-pragmatist's
   § 5, which the critic's re-run left standing);
3. **the walk**: at most eight frame records within the thread's own stack bounds, those bounds read
   once per thread from `pthread_get_stackaddr_np` or `pthread_getattr_np` (so `--sanitize`, which
   left the compiler-engineer's bounds empty, does not blind it), for a Heroes function a C handler
   calls (`fcase`, `flib-fp`, `fexit`, `ffork`).

**The probes that learn the delivery's address install through the C library's own `sigaction`**,
never through a definition of the runtime's (the critic's item 8). **The refusal is
async-signal-safe end to end** (spec § 6: *"An abort ends the program at once, saying why"*): its
words written by `write(2)` from fixed parts with the function's name copied in, no `snprintf`, no
stdio, no `abort()` inside the handler; SIGABRT's default restored and raised, `_exit` after; and
**latched once per process**, so a second thread arriving (two threads inside `hero_signal_refuse`,
pid 74286, the critic's item 10) waits for the first's end and the words are written once.

**Its cost is accepted under CLAUDE.md § Precedence, rank 3 over rank 5, in writing** (the critic's
item 7): the walk lands in its read-once form, prediction (2)'s bound (2,032,000,000 instructions on
`qsortcost` at `-O2`) is scored at the landing, and a miss is reported, never bought back by dropping
the walk.

**R2. D stays, as the backstop for what R1 cannot see**, at its landed cost (`check` +0.9%, warm
build +1.1%, batch 34): frames with no record past the walk (`flib-nofp`), a depth past eight frames
(glibc's `qsort`, 61,280 of 121,205 comparator calls missed by the walk in the ffi-pragmatist's Linux
run), and Windows. **At the landing D's guard reaches the lend tracking too** (`hero_lend_push`,
`hero_lend_take_at`), which a handler's `write(@buf)` re-enters today with a false message (the
critic's item 10: the blind answers `daaef5` and `ba6229`, both dead in `hero_lend_audit` on the
reader thread); if it does not build, the landing names it in D's residue in its commit.

**R3. What R1 and R2 leave is a defect of its own, `blocking`, filed in the hour 2494 closes**: a
Heroes function a C handler calls through frames the walk cannot read (a library built without frame
records, `flib-nofp`), which allocates nothing and so passes D: `c-fliblocale` dies by SIGKILL in
silence on Darwin and runs inside the signal on Linux arm64 (the ffi-pragmatist's § 4). Its cases
are those two. Never a known cost: it blocks M-core-packages' tag like any `blocking` defect.

**R4. The C library's installers are not interposed as built** (the ffi-pragmatist's repair), on the
critic's measurements: its probe installs through the runtime's own `sigaction`, so the exact test
learns the wrong address (`direct.hero` and `dlocale.hero`: route C as built refused 3 of 3, the
repair 3 of 3 SIGKILL in silence); a library installing through `bsd_signal` or `sigvec` ran inside
the signal at walk 8 and walk 0; its `sigset` is `signal(sig, h)`, so `sigset(SIGUSR1, SIG_HOLD)`
ended at exit 134 with a false message; it defines 3 of glibc's 9 installers and 3 of Darwin's 5; and
a Darwin dylib binds `sigaction` to libSystem whatever the executable defines. **It is R3's
candidate**, which R3's lane builds and measures first, against: the probes through the C library's
own `sigaction`; every installer each C library exports defined with its whole meaning (`SIG_HOLD`,
`SIG_IGN`, `SIG_DFL`, the previous handler answered); `sigaction(sig, NULL, &old)` answering the
program's own handler; `siglongjmp` out of a handler (the ffi-pragmatist's condition (a)); and R3's
two cases refused 20 of 20. **The runtime defining names of the C library is an architectural act no
seat measured against panel 239's rule** (`.claude/rules/c-boundary.md`, `ffi_defined_elsewhere`),
so it lands, if it does, only with a soundness sitting's ruling and that rule amended in the same
commit (CLAUDE.md § 4: architecture is a trigger).

**R5. Spec § 13 states R4 whole and names the doors: hm6 and hg3, each as measured.** hm6 merged
into the callback sentence, *"A callback is a **parameter**, never a result or a signal handler, nor
called by one; ..."* (+9 vendored), and hg3 appended after the corrected example (+120):

> No function of the program is a signal handler: after `hero_net_catch_stop() -> i64`
> (`hero_net.h`), Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1 and a waiting
> call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any
> other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call.

**Chosen by the blind seat's registered rules, in order** (§ The blind seat): of the forty, r (hrr)
entered against x and m (hm6) did not; the first follow-up left r standing against g (+2); but
**hrr cannot land as worded**: its remedy, *"in a header the program ships"*, is panel 114's R2
sentence, which 114's R3 holds (*"R2 does not land without them"*, ratified 2026-09-06), and 19 of
its 20 readers wrote the `swp6` shape; so the second follow-up measured g against the smallest
complete statement the spec owes (CLAUDE.md § 12), hm6: **g 12 against m2 2, +10, hg3 replaces
hm6**. **Both land, and why**: hg3's first clause says no function of the program **is** a handler,
and R1 also refuses one a C handler **calls** (`fcase`, `fexit`, `ffork`, by the walk), which only
hm6's *"nor called by one"* states; so hm6 stays in the callback sentence as the rule's whole, and
hg3 follows it as the measured remedy (+128 vendored over the corrected base, 8493 to 8621, the
base's stage 2's `measure` of `p247-scratch/price/spec-m2g3.md`, `163219f2fa69fe82`; +131 over the
base's 8490; real unmeasured, the landing's `--refresh` the one paid call). Each
text lands exactly as its arm read it; the two together were never one arm, said here.
**What the gap rests on**: ten of m2's twelve answers were refused `empty_record` at `record
sigset_t partial`, defect 2514's shape, a compiler defect and not hm6's words; with 2514 repaired the
gap is a question. **Its stop clause and panel 246's resolution**: panel 246's resolution keeps a stop door out of the spec
until *"a sitting with a thesis effect measured on a server-shaped task and its W4 falsified on
Darwin and Linux"* (its § What a veto compels). The third follow-up measured the first half on a
server blocked in `accept` (t4): **g 4 of 4, m2 0 of 4, +4**, so the clause is measured on a
server-shaped task. The second half is held for the platform layer's stop by the tracked case
`tests/golden/run/the-platform-layer-stops-a-server-on-a-request-to-end.hero`, W4's own shape (a
server blocked in `accept` on a worker, ended by SIGTERM, its `accept` answering stopped and `main`
returning): green on Darwin at batch 34's gate and on the CI's Darwin leg, and on Linux x86-64 on the CI's leg of `09685b1bc` (run 43392996643, job 131777144333: `run: 780 passed, 0 failed`, its five `SKIP run/` lines none of the platform layer's, read from the job's log), the Linux arm64 leg still running when this was written. So R5's stop
clause meets the condition panel 246's veto wrote for its own return; the author's ratification is
what records that veto discharged.

**R6. § 13's `atexit` example is corrected** (defect 2513): `` `atexit(f: (function() -> ()))` ``
becomes `` `atexit(f: (function() -> ())) -> i32` ``. The spec is wrong here, not the compiler: § 13,
two sentences above the example's paragraph, *"checks every result type ... against that header,
and a result may be wider than C's"*, and `stdlib.h` returns `int`, so the example as written is
refused `error[ffi_return_type]` (the critic's item 3), and it cost arm e two answers. +3 vendored
(`spec-x2.md`, 8493 against 8490).

**R7. G, the runtime's door that counts a signal, lands in `hero_os.h`** as the ffi-pragmatist
built it (`247-reports/ffi-pragmatist.md` § 9): `runtime/parts/sigdoor.c` (71 lines) and three
declarations, `hero_signal_watch(sig) -> i64` (the runtime's own C handler: one process-wide
`_Atomic` counter per signal and one byte down a non-blocking pipe, both async-signal-safe),
`hero_signal_take(sig) -> i64` (the deliveries since the last take) and `hero_signal_fd() -> i32`
(readable once one came, for `poll`). It answers -1 for a fault's signal, SIGKILL, SIGSTOP, SIGINT
and SIGTERM: **it is not a stop door**, which panel 246's resolution keeps out of `hero_os.h`; the
stop stays the platform layer's. Its Windows arm answers -1 (UCRT raises no signal G could count
there but the stop's). **Principle 0's ticket is a measured argument the three seats that judged it
accept** (the compiler-engineer and the ffi-pragmatist, *approve as a complement*; the historian,
*deferral's complement*), and the blind seat measured readers finding it once named (t2, 4 of 4 g
answers in each follow-up, every one through `hero_signal_watch` and `hero_signal_take`): G is the one route built that counts a signal across modules (`gtwo`,
exit 0 under two storms on Darwin and in Linux arm64), where the remedy readers otherwise write, a
`static` counter in a header the program ships, is the shape that fails in silence when a second
module binds it (`swp6`, 2 of 2; the coordinator's `t3trap`, SIGKILLed still printing) and the shape
19 of 20 r answers wrote; and with it R1's refusal and D's words have a remedy for every signal a
program may catch that does not lean on panel 114's gated road (R9).

**R8. What R1 changes, told and not undone** (CLAUDE.md § 2, no form kept for an older program
before 1.0.0): `quiet`, `t3x1`, the 2496 case and D's own case are refused at run time, at the first
signal; the landing rewrites the 2496 case and D's case so their handlers are C's or a door's, and
their words follow; the two 2257 cases build and run as today (their handlers never run) against a
spec that now forbids them, so the landing rewrites them to the spec's word too; a program's own C
handler (`chandler`, the 2257 C case), and `sigset(SIG_HOLD)`, are untouched.

**R9. D's words and R1's refusal carry one remedy**: never a Heroes function as a handler; to stop
on Ctrl-C or SIGTERM, `hero_net_catch_stop` from `hero_net.h` (panel 246's R4); to count any other signal, `hero_signal_watch` and `hero_signal_take` from
`hero_os.h` (R7). No remedy names a C header: that road is panel 114's R2, held by its R3, and its
plainest use is the `swp6` trap. The words name each door with the header it comes from, as R5's
sentence does.

**R10. Windows**: R1's probe and walk are POSIX's (`#if !defined(_WIN32)`); on Windows a Ctrl-C
handler runs on a thread Windows creates, where 2492's thread guard already refuses a Heroes
function, and a fault's handler on the faulting thread is D's. The ffi-pragmatist's Windows arm
(`F/win/winprobe.c`, `thread_win.c`) is compiled by `zig cc` for x86-64 and Aarch64 and **unrun**:
the landing runs it on the box when the box is back, else the CI's Windows leg judges after the push
(`.claude/rules/verification.md` § The optimistic chain).

**Refused**: A alone and E by name (vetoed by the compiler-engineer and the ffi-pragmatist: their
selection is a name list, and `wraplocale` dies 5 of 5 in silence through its hole); A's declaration
form beside C (objected: it refuses `chandler`, a C handler R4 allows); H and I (vetoed; each leaves
Heroes code inside the signal, which changes R4, the author's call, and nobody asked for it); F and S
(objected by both soundness seats, below); the sentences he and hs (vetoed by the spec-warden).

## What a veto compels

- **The ffi-pragmatist's veto of C with the walk as the only answer to F's case** stands on
  `fownlocale` and `fliblocale` dead in silence. R1's `SS_ONSTACK` closes `fown`'s shape exactly;
  `fliblocale`'s is R3, an open `blocking` defect with the interposition, repaired, as its candidate
  (R4). The resolution does not adopt what the veto refuses: it adopts the walk **beside** two
  exact answers and D, and keeps the veto's remaining ground open as a defect that blocks the tag.
- **The compiler-engineer's vetoes** (D alone, A alone, E by name, H, I) and **the ffi-pragmatist's**
  (the same, and A's declaration form): none is adopted.
- **The spec-warden's vetoes of he and hs**: neither sentence enters; with them E and S, which each
  would need, are not adopted.
- **Panel 246's veto of a stop door in the spec** (its § What a veto compels: *"no stop sentence in
  the specification and no stop door in `hero_os.h` or among the built-ins"*): R5's stop clause is admitted by that veto's own return condition (R5: a thesis
  effect on a server-shaped task, g 4 of 4 against m2 0 of 4, and W4 falsified on Darwin and Linux
  by the tracked case, and on Linux x86-64 on the CI's leg of `09685b1bc` (run 43392996643, job 131777144333: `run: 780 passed, 0 failed`, its five `SKIP run/` lines none of the platform layer's, read from the job's log), the Linux arm64 leg still running when this was written). The veto's other half, *"no stop door in `hero_os.h` or among the
  built-ins"*, stands untouched: G answers -1 for SIGINT and SIGTERM (R7).

## Disagreements, not smoothed

- **Refuse or defer.** The historian approves a deferring route on the precedents' weight (none of
  the runtimes read runs a handler in the signal; Perl moved out at 5.8.0); the two soundness seats
  object to F and S because they change what runs today (`t3x1` a tick late, the 2496 case's words
  false, every handler on a thread of the runtime's or at a poll), because whether `on_signal` runs
  now or later depends on who calls it at run time rather than on the line (design.md §1.3), and
  because S's polls cost +4.8% on the compiler's own check. The blind data does not settle it: arm s
  reached 8 of 8 under F, a route that does not keep hs's promise (the critic's item 6: a deferred
  handler raced the loop, 4 of 10 and 3 of 10 runs one step late, both graded right), and 9 of its
  12 right answers carried state across threads through `setenv` and `getenv`, a question no seat
  ran. **Refusal is adopted because it enforces R4 with the semantics programs have today and moves
  no handler; deferral is recorded, built (F) and priced, as the author's alternative** (below).
- **The remedy's home.** The spec-warden puts the remedy in the refusal's message at zero spec
  tokens and objects to hrr's *"a header the program ships"* (panel 114's R2 sentence never entered
  the spec, held by its R3; `swp6`'s trap); the blind seat measured hrr's remedy moving readers +7
  (+4 with 2511 modelled) on single-module tasks. The second follow-up settled it by measurement: the doors named in the spec (hg3)
against the rule alone (hm6), +10; and R9 keeps the remedy in the refusal's message and D's words as
well, at zero spec tokens, where the spec-warden wanted it. The spec-warden's objection to hrr is
upheld on its own ground (panel 114's R3), and hm6, which it approved, lands with hg3.
- **The walk's price.** The compiler-engineer bounds it (prediction 2) and the ffi-pragmatist calls it
  optional beside its depth; the critic measured the repair without the walk at 1.48 times that
  bound and the read-once form unbuilt. R1 keeps the walk under rank 3 in writing.

## Conservative alternatives, recorded

- **C's exact test alone, with D** (`HERO_SIGNAL_WALK 0`; the compiler-engineer's own conservative
  form): `qsortcost` +27.9% at `-O2`, nothing else moving, F's case left wholly to D. R1 keeps the
  walk and adds `SS_ONSTACK`.
- **The spec-warden's hm** (+4), the conservative twin of hm6 (+9).
- **F, if the author chooses to honour R4 rather than enforce it** (the compiler-engineer's condition
  (b), the ffi-pragmatist's (d), the historian's approval): built by the compiler-engineer
  (`heroes-f`, `sources 6fe8841f73943f3c`), its costs in § Disagreements; it would need a spec
  sentence of its own and Principle 0's ticket for a new meaning (he's veto ground).

## Prior art, per resolution

The historian's candidate lines (`247-reports/historian.md` § Candidate prior-art lines) for the
routes this resolution adopts, each page re-read by the coordinator's session on 2026-11-13 and
written from the page's own words; the routes not adopted (S, E, F, A, H, the interposition) carry
none.

- R1 and R7: `- **Reached first by Go**: [issue 35814](https://redirect.github.com/golang/go/issues/35814): a signal handler can only call async-signal-safe functions and Go code will never be async-signal-safe; the maintainer recommends opening a pipe, reading it on the Go side, and writing a byte to it in the signal handler, the write call being async-signal-safe.`
- R1: `- **Reached first by libunwind**: [unw_is_signal_frame](https://raw.githubusercontent.com/libunwind/libunwind/master/doc/unw_is_signal_frame.tex): a frame is recognised as a signal frame, a signal trampoline normally created by the kernel when delivering a signal, and the call answers UNW_ENOINFO when it cannot determine whether it is one.`
- R2: `- **Reached first by Go**: [runtime/malloc.go at go1.23.0](https://raw.githubusercontent.com/golang/go/go1.23.0/src/runtime/malloc.go): mallocgc throws "malloc deadlock" when its thread is already allocating and "malloc during signal" when it runs on the thread's signal goroutine.`
- R7: `- **Reached first by Apple**: [Dispatch Sources](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/GCDWorkQueues/GCDWorkQueues.html): a signal dispatch source runs its handler asynchronously on a dispatch queue with no restriction on the functions it calls, reports how many signals were delivered since its handler last ran, and cannot monitor SIGILL, SIGBUS or SIGSEGV.`

## Defects the sitting found

- **2511**, `sigemptyset` a macro on Darwin (`#define sigemptyset(set) (*(set) = 0, 0)`): the
  emitted call and the return probe expand it and die at exit 2, while the compiler's own pointee
  probe already writes `(sigemptyset)(...)`. Filed by the coordinator; the critic's ruling question,
  *a call of the function or a refusal*, is answered by the compiler's own pointee probe's form, a
  call of the function, its repair batch 35's.
- **2513**, § 13's `atexit` example refused by its compiler: R6.
- **2514**, `record sigset_t partial` with no fields refused `empty_record`, its fix (*"name the type
  for a HANDLE"*) not fitting a value C writes in place (the critic's item 11): reproduced by the
  coordinator, it is a cycle of three refusals, one of whose notes is false of a scalar typedef
  (*"a program never holds one by value"*), while the form that works, `@set: u32`, built and ran
  and goes unnamed; seven of the forty answers were refused `empty_record` (two in x, three in m,
  two in e). Filed `blocking` (a false message); its repair batch 35's, lane b35-ffi with 2511.
- **The lend tracking re-entered from a handler** with a false message: R2.
- **R1's doubled refusal**: R1's latch.

## What the landing owes beside the resolutions

- R1, R2, R7 and R8 to R10 in lane b35-signals of batch 35, gated by its cases; R5 and R6, the
  spec's, by the coordinator in the same round, with `spec`, `special`, `grammar`, `fixes` and
  `unseen` (the map for `spec/**`) and the site's build; the cases: the compiler-engineer's
  `clocale` and `wraplocale` and the ffi-pragmatist's `fownlocale` (prediction (1) each, 20 of 20),
  `fexit`, `ffork`, `wrap`, `infocase`, `chandler`, the `SIG_HOLD` program, the spec-warden's `swp6`
  (SW4); every pinned program that hands C a function re-emitted and its emission blessed by name.
- The refusal's part in a `runtime/parts/` file of its own (`thread.c` reached 405 lines as built).
- `heroes measure spec/heroes-spec.md --refresh` on the landed spec (SW5), the one paid call.
- R3 filed, and 2494 closed with the landing's hash.

## Left open, as questions

- Linux grading of the 96 answers (the `ptr` and record answers predict Linux; unrun).
- glibc's `qsort` under any interposition (the ffi-pragmatist's prediction (2)): unrun.
- Whether cross-thread `setenv` and `getenv` in a deferred handler are safe (no C library page read).
- S built: no seat built it.
- Windows, every arm: compiled, never run.

## Predictions to score

- compiler-engineer (1), (2), (3), at the landing (above).
- ffi-pragmatist (1) at the landing; (2) and (3) in Linux arm64, unrun.
- spec-warden **SW1 falsified** (x's Heroes-handler answers 1, r's 0), **SW2 holds** (r 8 against x 1
  under C), **SW3 falsified as registered** (e at x + 3; it holds with 2511 modelled, 4 against 4),
  scored by the critic from `handlers.tsv` and `tally.txt`, all three registered at 05:05 against the
  twelve-session design with a margin of 2 and never rescaled; **SW4 holds under the repaired C** and
  is scored again at the landing; **SW5** at the landing's `--refresh`.
- The follow-up's rule: the first, **r stands** (+2); the second, **hg3 replaces hm6** (+10); the third,
**the stop clause counts as measured on a server-shaped task** (+4).

## Process notes

- The blind seat's first design (twelve sessions, three arms) was widened to forty before any
  session, on the author's yes to 25 USD; three follow-ups were registered after it, each before
  any of its sessions and each on a question the one before left (the critic's item 4 on r's credit;
  hrr crossing panel 114's R3; panel 246's server-shaped condition); every text before each change
  is kept beside the brief (`.before-extension.md`, `.before-followup.md`,
  `.before-second-followup.md`, `.before-third-followup.md`). 96 sessions, 21.0131 USD.
- One registered sentence was false: the third follow-up's *"nothing in the runtime sets line
  buffering"*, a `grep` cut by `head -8` before `runtime/parts/os.c:119-123`'s `_IOLBF`; corrected
  under it with its date, the coordinator's pipe run and the agent's own; no grade depended on it.
- The `claude` CLI passes `--max-budget-usd` by one request: t4-m2-3 ended at 0.4960 USD on a 0.40
  cap, the question the briefs had left open; the runners' sum bounds held the seat inside 25.
- The harness refused the blind seat's runner and the critic a report file of their own; the
  coordinator wrote both into `247-reports/` from their final messages, as panel 246 did.
- The Mac slept on battery from about 06:35 to 09:17 and intermittently; the API answered 403 to the
  subagents around 10:45 and 11:18; seats resumed from their own report files.
- The historian's OCaml fetch was saved by the fetching tool outside the repository's root
  (`historian.md` § One thing written outside); it stands where the tool put it, for the author.

## Author's verdict

Pending: the author's reading of R1 to R10, asked by decision 2517
(https://github.com/heroes-lang/heroes/issues/2517), open since 2026-11-13. A yes settles the ten
resolutions as adopted, panel 246's veto of a stop door in the spec as discharged by its own return
condition (R5), and R3's residue filed `blocking`; it does not settle R4's interposition, which
waits for a soundness sitting, the conservative alternatives, which stay the author's to choose, or
the predictions, on their own clocks.

## Added 2026-11-13 at about 16:15, after the sitting's commit

The Linux arm64 leg of run 43392996643, still running when R5 was written, ended green: job
131777144556, `run: 780 passed, 0 failed`, no `run/the-platform-layer` case among its skips (its log,
read through GitHub's API). W4's shape is falsified on Darwin and on both Linux legs; the run's one
red leg, Windows x86-64, is defects 2509 and 2510, repaired in batch 35's round.
