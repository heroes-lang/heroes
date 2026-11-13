<!-- The completeness critic's second pass, its final message, written into this file by the coordinator
on 2026-11-13 (the harness refuses a subagent a report file of its own). -->

# Panel 247: the completeness critic's second pass, over the reports

I read all of these whole: the five seat reports (`historian.md`, `spec-warden.md`, `compiler-engineer.md`, `ffi-pragmatist.md`, and `llm-ergonomist.md`, all 5,268 lines), my first pass (`critic-briefs.md`), and the briefs they answer (`00-shared.md` and the five seat briefs, current versions). I give no verdict.

**Where I worked.** Everything I wrote is under `.claude/worktrees/p247-scratch/critic2/` (`TMPDIR` in `critic2/tmp`). I used only copied binaries:
- The "today" grading compiler, `shasum -a 256` `c8a7586f07ffccac`, which matches the registration.
- The E grading compiler, `c06b1956714cebef`, with `HERO_SIGNAL_DEFER 1` at `thread.c:135`.
- The ffi-pragmatist's route-C compiler, `7eadb93fbd6812f0`, beside copies of its `tree-c`, `tree-c2` and `tree-c2w0` runtimes.

Every process I started was stopped (`pgrep -fl p247-scratch/critic2`: none). Load was 6.11 at 11:20 (`uptime`), so no duration below is a measurement; costs are instructions retired. No paid run, no API call, no GitHub write. My reads of GitHub were `gh issue list`/`view` only.

## Tension 1: refuse or defer, and what the blind data pays

**The arm-x baseline is mostly the FFI's failures, not handler choices.** I tabulated `grade/*/result.json` (`critic2/work/reasons.txt`):
- **Arm x:** 7 wrong of 8. Three are `internal error` at exit 2 (defect 2511), two `empty_record`, one `ffi_constant_type` (`POLLIN` as `i16`), one `line_end_before_continuation`. Only one x reader wrote a Heroes handler (t1-x-1, `handlers.tsv`).
- **Arm m:** 8 wrong of 8. Four exit 2, three `empty_record`, one `lent_shape`.

**All seven exit-2 answers die where the compiler expands Darwin's macro.** Darwin defines `#define sigemptyset(set) (*(set) = 0, 0)` (`signal.h:128`). The emitted call passes `(void *)&cell` into it, and so does the return probe `HERO_RET_I32(sigemptyset(0))`. Yet the compiler's own pointee probe in the same unit already writes `(sigemptyset)(...)`, which calls the function (`clang -fsyntax-only` over each emitted `main.c`). The two `u32` answers die this way too, not only the `ptr` ones.

**The counterfactual, measured.** I added a shim `signal.h` (`#include_next <signal.h>`, then `#undef` of the set macros) through `heroes build --include`, without changing any answer's text. I graded with the registered rubric (grade.py's `t1_run` and `t2_run`, copied) and the registered drivers. **All 7 were right** (`cf-results.txt`):
- t1-x-4, t1-m-1 and t1-m-2: both signals, exit 0, ended 0.51 to 0.77 s after the signal.
- t2-x-2: M from 511,760 to 632,233.
- t2-x-4: M from 400,939 to 624,910.
- t2-m-2: M from 7,456 to 7,774.
- t2-m-4: M from 855,438 to 1,080,142.
- Every t2 answer: 3 storms, all exit 0.

So with 2511 repaired as a call of the function, x reads 4 of 8 and m 4 of 8. Under the registered rule (at least +3) the verdicts become:

| arm | as graded | with 2511 repaired as a call of the function |
|---|---|---|
| r | +7, enters | +4, enters |
| e | +3, enters | 0, does not enter |
| s | +7, enters | +4, enters (still provisional) |
| m | -1, does not enter | 0, does not enter |

**Arm e's entry sits on the threshold, and on defect 2511.**

**The spec's own example is refused by its compiler, and it cost arm e two answers.** I bound `atexit(f: (function() -> ()))` against `stdlib.h` exactly as spec § 13 writes it. The build exited 1 with *"`error[ffi_return_type]`: `atexit` returns `int` in `stdlib.h`, and this declaration takes nothing back"*. Every tracked case binds it `-> i32` (four golden files, `grep 'function atexit'`).

t1-e-2 and t1-e-3 omitted `signal`'s result, citing that example, and were refused. With `-> ptr` and `_ =` supplied, both are **right under E** (`ex-results.txt`). So arm e is 4 to 6 depending on this defect. Three issue searches found nothing naming it: `atexit ffi_return_type`, `atexit example spec refused`, `atexit int result`.

**Arm r's +7 measures the remedy clause, not route C.** Arm r is 8 of 8 under today, C and E alike (`tally.txt`). All eight are C headers whose handler only stores a `volatile sig_atomic_t` or does a lock-free atomic add, and all eight keep that object `static` in the header. That is `swp6`'s shape exactly. The refusal clause alone (hr) was never an arm.

**Arm s was graded under F, whose timing hs does not promise.** Ten SIGINT runs each under E of the two t1-s answers that sleep once: t1-s-3 began a step after the signal 4 of 10 times, t1-s-4 3 of 10 (`stopped after 4 steps`). The rubric grades both outcomes right. One more inference from reading the answers: 9 of arm s's 12 right answers under E, and t1-e-1, carry state through C's environment across two threads (`setenv` in the deferred handler, `getenv` in `main`). Whether that is safe is a question; I read no C library page on it today.

**The spec-warden's registered predictions, scored from the data (nobody scored them):**
- **SW1 is falsified.** It needed x's Heroes-handler answers to outnumber r's by at least 2. They are 1 against 0 (`handlers.tsv`). SW1 was hm6's payment.
- **SW2 holds.** r 8 against x 1, under C.
- **SW3 is falsified as registered.** It allowed e at most x + 1; e is x + 3. It holds under the 2511 counterfactual (4 against 4).
- **SW4 holds under the repaired route C** (`critic2/work/sw4`, `swp6` built by the repair's compiler and runtime). `nm` shows two `stop_seen`. One SIGINT at 0.5 s and a storm of 630,012 SIGINTs both ended `stopped after 200 rounds`, exit 0.
- **SW5 is unrun.**

SW1 to SW3 were written at 05:05 against the 12-session design, with a margin of 2. Nobody rescaled them to 3 of 8.

**What each route does to programs that work today**, from the seats' runs and mine:
- **C:** refuses `quiet`, `t3x1`, the 2496 case and D's own case. The three 2257 cases pass. A program's own C handler is untouched: `chandler`, the 2257 C case, and my `sigset(SIG_HOLD)` program (`pending after the hold: 1`, exit 0).
- **F:** runs `quiet`, `t3x1` and `storm` to exit 0, but `t3x1` was a tick late in 1 of 3 runs. The 2496 case passes while its words are false (0 of 200 on the worker, compiler-engineer).
- **The ffi-pragmatist's repair breaks a correct program's own C** (next section).

## Tension 2: hrr's remedy

- **As worded it is sound only by what readers already knew.** The text never says what the handler may do. The ffi-pragmatist's plain-C `snprintf` handlers died 4 of 4 on Darwin, and `swp6` survives every route (above).
- **G does not replace it for t1.** G's `hero_signal_watch` answers -1 for SIGINT and SIGTERM (`sigdoor.c:41-43`). So G serves t2's counting, across modules (the seat's `gtwo`), and not t1's stop.
- **The stop door that does serve t1 exists and is unnameable.** `hero_net_catch_stop` plus `hero_net_stop_asked` (`hero_net.h:130-170`) is a door spec § 13 may not name (panel 246's veto). **0 of 40 readers used `hero_net` or any `hero_signal` door** (`grep -l` over `grade/*/src`). The spec names `hero_net.h` once, as where sockets come from.
- **What that leaves the sitting:** a G sentence is unmeasured, so the registered rule cannot admit one. A remedy naming both doors cannot even be put to readers without the author amending panel 246's veto.
- **The spec-warden and the ffi-pragmatist disagree on where G lives.** The spec-warden's `hg` sends readers to "its platform layer's" headers. G as built declares its door in `hero_os.h:444-451`.

## Tension 3: the ffi-pragmatist's repair, re-run

**It holds as reported on its own cases** (route-C compiler `7eadb93fbd6812f0` with its `tree-c2` runtime):
- `fownlocale` 3 storms and `fliblocale` 3 storms: 6 of 6 exit -6 with the words. Six crash reports, every one SIGABRT through `hero_signal_refuse`'s `raise` (`k2-storms.txt`, `ips.py`).
- `--sanitize` on `fcase`, `fown`, `flib`, `fexit` and `ffork`: 5 of 5 exit 134 with the words.
- `qsortcost` at `-O2` (`qsortcost.txt`):

| build | instructions retired | against D |
|---|---|---|
| D | 1,511,650,304 to 1,513,094,059 | |
| repair, walk 8 | 6,114,601,653 to 6,114,842,590 | 4.0x |
| repair, walk 0 | 3,002,361,887 to 3,003,730,500 | 2.0x |

The compiler-engineer's registered prediction (2) caps the landed form at 2,032,000,000. The repair without the walk exceeds that by 1.48x, and the cheaper form ("one thread-local record read once") is unbuilt by anyone.

**It covers 3 of glibc's 9 installers, only by defining them.** My probe, `inst/installers.c`, defines `sigaction` the way the repair does and installs through each name found with `dlsym`:
- **Darwin:** libc's `signal`, `sigset`, `bsd_signal`, `sigvec` and `__sigaction` never reach the executable's own `sigaction`.
- **Linux arm64** (glibc 2.41, image `338a1044ac90`): the nine exported names are the seat's nine. None of libc's reaches it either. At `-std=gnu11` with no feature macro, `ssignal` and `__sysv_signal` are declared and callable.

**Its probe makes the exact test blind outside that list. This is new and measured.**
- `hero_signal_learn` installs its probes with `sigaction` (`thread.c:186` and `194`). In the repaired runtime that name is the runtime's own wrapper, so the address learned is inside `hero_signal_around`, not `_sigtramp`'s.
- A Heroes function handed straight to Darwin's `bsd_signal` (`direct.hero`): route C as built refuses it (exit 134); the repair runs it inside the signal (`caught 30 through bsd_signal, inside the signal`, exit 0).
- The same with a `localeconv` handler under storm (`dloc-storms.txt`): **C 3 of 3 refused (SIGABRT); repair 3 of 3 `exit -9`, nothing on stderr, 3 SIGKILL reports of the locale lock**, `h_dlocale_on_signal` called from `_sigtramp`.
- The seat's library installing through `bsd_signal` or `sigvec`:
  - Without frame pointers: ran inside the signal; 6 of 6 storms `exit -9`, 6 SIGKILL lock reports.
  - With frame pointers (`mov x29, sp`): still ran inside the signal at walk 8 and at walk 0.
- So the seat's sentence *"a dylib handing the thunk itself to the delivery is still the exact test's"* fails under its own repair by this mechanism. The walk does not rescue it, so "the walk optional" loses nothing the walk could see here.

**Its `sigset` breaks a correct program.** The repair defines `sigset` as `signal(sig, h)`. A program's own C calling `sigset(SIGUSR1, SIG_HOLD)` (`SIG_HOLD` is `(void (*)(int))5`, `sys/signal.h:133`):
- Under D and under route C as built: `pending after the hold: 1`, exit 0.
- Under the repair: exit 134, a false message, *"panic: a null pointer was read through, at offset 0x5, called from hold.main"*, and 1 SIGABRT report.

**Its glibc `qsort` prediction (2) is unrun.** `linux-247c.log`, the repair's Linux run, holds no `qsort` row; the rows exist only in `linux-247.log`'s plain-C walk.

## Tension 4: the blind seat's findings

- **`sigemptyset` (2511) is its own defect**, but how it is repaired decides arm e's verdict (above). The compiler already writes the function form in its pointee probe.
- **`empty_record` is its own adjacent question.** `record sigset_t partial` with no fields is refused, and the message's fix ("name the type for a HANDLE") does not fit a value C writes in place. Darwin's `sigset_t` is a scalar; glibc's needs its private `__val`. A portable route exists, a `ptr` to a `malloc`'d block, and it ran right on Darwin once 2511 was modelled. On Linux it is unrun.
- **The lend-tracking false message is 2494's class.** Both reports die on the reader thread the answer spawned: `hero_lend_audit < hero_lend_push < hero_lend_take_at < h_main_count_bytes < ... hero_spawn_trampoline`, and `h_main_count_signals` in daaef5. That is the handler's `write(@buf)` lend nesting into that thread's lend tracking, which D does not bracket. C refuses it at the thunk's first line. It belongs in D's residue list and in the landing's coverage.
- **C's doubled refusal is route C's own landing item (adjacent).** In run 3 of ba6229 under C (pid 74286), threads 0 and 1 (the spawned reader) are both inside `hero_signal_refuse`, which unblocks SIGABRT alone (`thread.c:241-244`). It needs a once-per-process latch.

## Tension 5: R4's reach

- **No approved route contradicts R4 in design.** The repaired route C does in measured programs: `bsd_signal`, `sigvec`, a framed library, and the `SIG_HOLD` damage. Route C as built does in `fown` and `flib-nofp`.
- **C refuses a program § 13's callback sentence allows**, so the spec owes a sentence (CLAUDE.md § 12). The sentence the spec-warden approves (hm6) fails the rule and SW1. The sentence that passes (hrr) is the one the spec-warden objects to, with SW4 holding.
- **hrr's clause reads "no code of the program runs inside a signal handler, so write the handler in C"**, and a shipped C header is the program's code. R4 says Heroes code. That is a reading only; 8 of 8 readers were not misled.

## Questions the sitting did not ask

- **What working path does today's spec give a reader on each platform?** Seven of eight x readers already avoided a Heroes handler; the path they chose dies on Darwin by 2511.
- **How is 2511 repaired: a call of the function, or a refusal?**
- **A two-module task** (SW4 on readers) was never registered.
- **A G-naming arm** was never registered either; budget question in item 9 of the list.
- **Linux grading of the forty answers** is unrun: the `ptr` and record answers predicted Linux.
- **The repair's interposition of the C library's names** against `.claude/rules/c-boundary.md` and defect 2489's rule: no seat measured it.

## The list

1. **Arm x's baseline is mostly unrelated refusals.**
   - Command: my `reasons.txt` script over `grade/*/result.json`.
   - Printed: x refusals: 3 at exit 2, 2 `empty_record`, 1 `ffi_constant_type`, 1 `line_end_before_continuation`. m: 4 at exit 2, 3 `empty_record`, 1 `lent_shape`. e: 2 `ffi_return_type`, 2 `empty_record`. Heroes handlers in arm x: 1.
   - The synthesis owes: reading the rule as "readers had no working path", not "readers wrote Heroes handlers".
2. **With 2511 modelled as a call of the function, x and m read 4 of 8 each, and e no longer enters.**
   - Command: shim `signal.h` via `heroes build --include`, then `cfgrade.py` with the registered rubric.
   - Printed: 7 of 7 right (`cf-results.txt`).
   - The synthesis owes: stating that e's entry (+3) rests on 2511, and getting 2511's ruling before relying on it.
3. **The spec's `atexit` example is refused.**
   - Command: build of `atexit(f: (function() -> ()))` against `stdlib.h`.
   - Printed: exit 1, `error[ffi_return_type]`. With the result supplied, t1-e-2 and t1-e-3 are right under E (`ex-results.txt`).
   - The synthesis owes: a defect filed (spec against compiler), and arm e read as 4 to 6.
4. **Arm r is route-independent, and every r handler is the `swp6` shape.**
   - Command: `tally.txt`; `awk` over `grade/*/src/*.h`.
   - Printed: r 8 of 8 under today, C and E. Eight handlers, each a `sig_atomic_t` store or an atomic add, each object `static`.
   - The synthesis owes: crediting hrr's remedy clause, not route C, and stating that the blind run never tested two modules.
5. **SW1 and SW3 are falsified as registered; SW2 holds; SW4 holds under the repaired C; SW5 is unrun.**
   - Command: `handlers.tsv` and `tally.txt`; `sw4` runs with `one.py` and `storm.py`.
   - Printed: x 1 against r 0; e 4 against x 1; `stopped after 200 rounds` twice; `nm` shows 2 `stop_seen`.
   - The synthesis owes: re-reading the spec-warden's hm6 approval (unpaid by SW1), its he veto (its SW3 ground falls, the "says what a fault does" condition stands), and its hrr objection (stands on SW4).
6. **Under F, a deferred handler races the loop.**
   - Command: 10 SIGINT runs each of t1-s-3 and t1-s-4 under E.
   - Printed: `stopped after 4 steps` 4 of 10 and 3 of 10.
   - The synthesis owes: marking arm s's +7 as graded under a route that does not keep hs's promise, and asking whether cross-thread `setenv`/`getenv` is safe.
7. **The repair holds on its own cases, at a cost over the compiler-engineer's bound.**
   - Command: `k2` storms, `ks` sanitized runs, `qsortcost` under `/usr/bin/time -l`.
   - Printed: 6 of 6 refused (SIGABRT); 5 of 5 exit 134; 1.51G, 6.11G and 3.00G instructions.
   - The synthesis owes: resolving the conflict with prediction (2)'s 2,032,000,000 bound by building the read-once form and counting it, or accepting the cost under rank 3 in writing.
8. **The repair's probe blinds the exact test, and its `sigset` breaks `SIG_HOLD`.**
   - Command: `direct.hero` and `dlocale.hero` under C as built and under the repair; `kv-*` libraries; `hold.hero`.
   - Printed: C refused 3 of 3; repair 3 of 3 SIGKILL in silence; `bsd_signal` and `sigvec` libraries ran inside the signal at walk 8 and walk 0; `hold-r` exit 134 with a false message.
   - The synthesis owes: not adopting the repair as built. Its own veto ground applies. The landing owes probes installed through the real `sigaction`, every installer of each C library (3 of 9 on glibc, 3 of Darwin's 5), and `sigset`'s full meaning.
9. **G declines the stop, sits in `hero_os.h`, and was found by no reader; the stop door was found by none either.**
   - Command: `grep` of `sigdoor.c` and `hero_os.h`; `grep -l hero_net` over the 40 answers.
   - Printed: -1 for SIGINT and SIGTERM; `hero_os.h:449`; 0 of 40.
   - The synthesis owes:
     - saying that no remedy sentence beyond hrr was measured, and that the best remedy cannot be measured without amending panel 246's veto;
     - correcting `hg`'s header;
     - weighing an arm g plus a two-module arm. Size: about 0.24 USD a session (9.5693 / 40 from `run.json`), so 8 to 16 sessions is about 1.9 to 3.8 USD of the 15.43 left.
10. **The lend audit's false message is 2494's class; the doubled refusal is route C's own.**
    - Command: the crash reports' threads, read with `python3 -I`.
    - Printed: `hero_lend_push` on the spawned reader thread (two reports); two threads inside `hero_signal_refuse` (pid 74286).
    - The synthesis owes: adding the lend tracking to D's residue list and the landing's coverage, and a refusal latch in route C's landing (adjacent).
11. **2511 and `empty_record` are their own defects.**
    - Command: the emitted units through `clang -fsyntax-only`.
    - Printed: the `(void *)` call and the `sigemptyset(0)` probe fail; the compiler's own pointee probe already writes `(sigemptyset)(...)`.
    - The synthesis owes: the hint on 2511's issue, and a question on `empty_record`'s "handle" fix.
12. **Unrun, posed as questions:**
    - Linux grading of the forty answers.
    - The repair's probe regression in Linux (its mechanism is the same by reading).
    - glibc's `qsort` under the repair (prediction (2)).
    - S built.
    - SW5's real price.
    - The blinding: t1-e-2's context named the `p247-scratch` path, and that answer was refused anyway.
