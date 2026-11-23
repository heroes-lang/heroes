# Panel 257, the ffi-pragmatist: each clause of w26, w27 and the arms against Darwin, the clauses found false, and the repaired clause, run

Written 2026-11-23 from the cells of `ffi-pragmatist.md` (this folder), Darwin arm64 only (Apple clang 21.0.0, macOS 27.2.1). **Every cell is three runs**
(`timeout 20`, both streams through `head -c 100000`, the exit from `PIPESTATUS`), plain (`-O0`), `-O2` and `--sanitize` where the row says so; **the programs and
their scripts are in `programs/ffi-pragmatist/`**. The columns are the builds: **today** (the frozen runtime), **C4r** (panel 255's Darwin route over the frozen
runtime), **landed** (`523d35537`, the lane's route C), and **A** = the landed runtime with `HERO_JMP_FORCE_UNCALIBRATED 3` (`rt/landed-A`, accessor reads route 0), **what a platform whose
calibration does not verify runs, simulated on this Mac, not Darwin x86-64**. `R` = runs (exit 0, the program's own lines), `S`/`SW`/`U` = exit 134 and the first line of stderr is that
kind (section 6 of the report), `T` = true on Darwin (cell, program), `F` = false (the falsifying program), `-` = untested.

## 1. The programs that decide, one table (from `logs/*.log`, rendered by `scripts/mktable3.sh` and `md.py`)

| program | what C does | today | C4r | landed | A (route A forced) |
|---|---|---|---|---|---|
| `p5` | the handler returns | R | R | R | R |
| `p1` | `siglongjmp` out, static buffer | R | R | R | R |
| `p2` | `siglongjmp` out, heap buffer | R | R | R | **U** |
| `p9` | `siglongjmp` to the first of 40 live saves | R | R | R | R |
| `p3` | `siglongjmp` through a whole copy | **U** | R | R | **U** |
| `p10` | the handler copies an outside save and `siglongjmp`s to the copy | **U** | R | R | **U** |
| `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib` | `siglongjmp` out to a buffer in the heap, TLS, a library loaded after start | R | R | R | **U** (all six) |
| `b_0`, `b_1` | the handler leaves by `setcontext` to a `getcontext` context taken before the signal | R | R | R | **U** |
| `sc1` (new, `depth/dsweep.h`) | the handler leaves by a ONE-WAY `setcontext` onto a `makecontext` stack whose `uc_link` is the runner's context | **U** | **S** | **SW** | - |
| `p4` | a jump no function of the C library makes (assembly on arm64) | S | S | S | S |
| `p8` | the handler jumps within itself, then calls | S | S | S | S |
| `p7`, `g1_0`, `g1_1`, `g2`, `g2_1` | the handler `swapcontext`s (or swaps to a coroutine that jumps) away, never resumed | R | S | SW | `p7` SW; `g1_0`, `g2` U; `g1_1`, `g2_1` not run on A |
| `g3` | the handler swaps to a coroutine that returns through `uc_link` | **U** | S | SW | U |
| `fp3` | plain `siglongjmp`, target in memory once handed to `makecontext` | R | **S** (`-O2`: R) | **S** (`-O2`: R) | R |
| `as_0` | handler `siglongjmp`s INTO a coroutine | R | S | S | R |
| `sched` | SIGALRM handler `swapcontext`s to the next green thread (switched back to later) | U | S | SW | - |
| `bctx_signal` | SIGALRM handler resumes a Boost.Context fiber (asm switch), which switches back | S | S | S | - |
| `watchdog-a` / `-b` / `-c` | handler `siglongjmp` / `swapcontext` away never back / sets a flag | R / R / R | R / **S** / R | R / **SW** / R | R / - / R |

## 2. Each clause, marked

**w26** (`spec-w7.txt`): *"A handler of C's ends by its return or by a `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of it; left any other way (by an inline jump, a `swapcontext` away from it or a jump onto another stack given to `makecontext`) it is still taken to be running, and a callback called later on its thread from deeper in the stack than the handler was may abort, saying the handler was not seen to end."**

| atom | verdict on Darwin | the cell |
|---|---|---|
| ends by return: callbacks run | **T** | `p5`: R on all four columns; the depth sweep (report 4b): R at 11 asked depths (landed: 9) x 3 modes, today, C4r, landed |
| ends by `longjmp`, `siglongjmp`, `_longjmp` out of it: callbacks run, any memory | **T on C4r and landed, F on today and on route A**; **F on C4r and landed for `fp3`** | `p1`, `p2`, `p3`, `p9`, `p10`, `gcopy`, `x_*`, `jo_*` R on C4r and landed; **today** `p3`, `p10`, `gcopy` U; **route A** `p2`, `p3`, `p10`, `gcopy`, `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib` all U (3 of 3 in `-O0` and `--sanitize`); **`fp3` S at `-O0` and `--sanitize` on C4r and landed, 3 of 3, at every depth** (the target's stack pointer lies 196,560 bytes into a range once handed to `makecontext`: `depth/fp3_layout.c`) |
| ends by `setcontext` out of it: callbacks run | **T for `b_0`, `b_1` on route C only; F on route A (U, 3 of 3 in `-O0` and `--sanitize`); F for `sc1`** | `b_0`, `b_1` R x3 modes on today, C4r, landed; **route A U**; **`sc1` refused on all three builds at every depth** (today U, C4r S, landed SW; 4 depths x 3 modes x 3 runs; under `--sanitize` the 262144 depth overflows my coroutine's own 256 KiB stack and is ASan's report, not a refusal): a one-way `setcontext` out of a handler onto a `makecontext` stack is *a `setcontext` out of it* and the runtime holds the handler |
| left by an inline jump: still taken to be running | **T** | `p4` S at 11 asked depths (landed: 9) x 3 modes, today, C4r, landed |
| left by a `swapcontext` away from it: still running | **T on C4r and landed; today does not refuse (the clause says *may*)** | `p7`, `g1_0`, `g1_1`, `g2`, `g3`, `nn`, `sched`, `watchdog-b`: S (C4r), SW (landed), R (today) |
| left by a jump onto another stack given to `makecontext`: still running | **T on route C, F on route A** | `as_0` S on C4r and landed, **R on route A** |
| a callback called later on its thread from deeper in the stack than the handler was may abort | **true as *may*, misleading as the condition** | Darwin refuses at EVERY depth, **from 1,264 bytes ABOVE the handler's frame (the raise site's own level) to 256 KiB below it**: `p4`, `p7`, `g11`, `g2`, `g3`, `as0`, `p8` S/SW at 11 asked depths (landed: 9) x 3 modes x 3 runs on C4r and landed (report 4b: 2,460 runs on today and C4r, 810 on landed, 0 timeouts) |
| *saying the handler was not seen to end* | **T for S, partial for SW, F for U and K** | report section 6 |

**w27** (`spec-w12.txt`): w26 without the context and `makecontext` items. **Atoms 1, 2 (with the same falsifiers), 3 (with `sc1`), 4 and the depth clause as w26's.** It is **silent on every context switch**, so `p7`, `g1_*`, `g2`, `g3`,
`sched`, `watchdog-b`, `as_0`, `sc1`, `bctx_signal` abort on C4r and landed with no clause that says so: **incomplete, not false**.

**f1**: *"A handler of C's ends, for the program, where this runtime sees it return or sees `longjmp`, `siglongjmp` or `_longjmp` take the thread out of it; a handler left any other way (a jump no function of the C library makes, a switch of context never switched back) or jumped within is held running, and a callback called while one is held running aborts, saying the handler was not seen to end."*

| atom | verdict | the cell |
|---|---|---|
| return, or `longjmp`/`siglongjmp`/`_longjmp` *seen* to take the thread out, ends it | **T by construction; not a rule a reader can apply** | `fp3` is refused at `-O0` although the program's `siglongjmp` took the thread out: the runtime *did not see* it do so; the clause is true of whatever the runtime concludes |
| a jump no function of the C library makes: held running | **T** | `p4` S, every depth |
| **a switch of context *never* switched back: held running** | **F as written** | `sched` (every green thread IS switched back to; the program ends with the three right sums), `bctx_signal` (the worker fiber switches back, the handler returns): both refused on today (U / S), C4r (S / S) and landed (SW / S); `watchdog-b` is the *never* case and is refused: **the refusal is of a callback called while the handler is suspended, not of a switch that never returns** |
| jumped within: held running | **T** | `p8` S, every depth |
| a callback called while one is held running aborts, saying the handler was not seen to end | **T for S**, partial for SW | section 6 |

**f1b** (no context clause): every atom of f1 except the *never switched back* one; **T**, and silent on the G forms (R on today, refused on C4r and landed).

**f2**: *"A callback called after C left a handler by `longjmp`, `siglongjmp` or `_longjmp` runs, wherever the buffer lives; one called while a handler is held running aborts ..."*

| atom | verdict | the cell |
|---|---|---|
| runs after a `longjmp`-family exit, **wherever the buffer lives** | **F on route A and on today; F for `fp3`** | route A: `p2`, `x_heap`, `x_tls`, `x_dlopen`, `jo_*` U; today: `p3`, `p10`, `gcopy` U; C4r and landed `fp3` S at `-O0`/`--sanitize` |
| *never switched back* | **F** | as f1: `sched`, `bctx_signal` |

**f3**: *"A callback called while a handler of C's has not been seen to end, by return or by `longjmp`, `siglongjmp` or `_longjmp` out of it, aborts, saying so."* **T of every refusal I measured** (`p4`, `p6`, `p8`, the G forms, `sched`, `sc1`, `fp3`, `as_0`, 2494): none of them is a handler seen to end by those. It **names no cause and promises nothing about what runs**; its predictive power for a program author is **nil by design** (the reader cannot know whether the runtime "saw" the end).

**v1 arms** (`arms/v1/`, kept as the critic read them; my cells):

| arm and atom | verdict | the cell |
|---|---|---|
| e1/e1b *callbacks run on, whatever memory the buffer is in* | **T on route C; F on route A** | rows above |
| e1/e1b *an inline `__builtin_longjmp`* | **not writable on arm64** (*error: __builtin_longjmp is not supported for the current target*, report section 5) | `inline2/bl.c` |
| e1/e1b *a C++ exception* | **unreachable on Darwin**: no program leaves a handler by an exception and reaches a callback (libc++abi terminates, `terminating due to uncaught exception of type Fault`, 3 of 3 on all three builds) | `cxxguard` |
| e1/e1b *a switch of context never switched back* | **F as written** | `sched`, `bctx_signal` |
| e1/e1b *a jump to a buffer the handler refilled* | **F**: a handler that refills and leaves RUNS (the critic's p10: R on C4r, landed) | `p10` |
| e1/e1b *though C is correct* | **F for every program here whose handler is asynchronous** (C11 §7.14.1.1, recalled, leaves the handler's calls undefined except for a `raise`d signal; report section 4) | `watchdog-a`, `probe`, `bmon` do not `raise` |
| e2 *callbacks run only where it returned or ... to a buffer a save of that family filled; any other exit aborts the next callback* | **F** | `p3`, `p10` (a copy is not a buffer a save filled; they run), **`b_0`, `b_1` (`setcontext` out: run)**, `gcopy` |
| e3 *where C leaves a handler other than by returning, longjmp, siglongjmp or _longjmp, the next callback aborts* | **F** | **`b_0`, `b_1`** (a `setcontext` out is none of the four; the callback runs on today, C4r and landed) |

## 3. The clauses found false, and the repair

Four things the arms and w26/w27 say that Darwin falsifies, each with the program:

1. **`setcontext` out of a handler ends it** (w26, w27): **`sc1`** (a one-way `setcontext` onto a `makecontext` stack) is refused; `b_0`/`b_1` (a `setcontext` to a `getcontext` context) run. The distinction is the *target*, as the landed words say (*"or `setcontext` into a context `makecontext` made"*).
2. **a switch of context *never* switched back** (f1, f2, e1): **`sched`**, **`bctx_signal`** switch back and are refused.
3. **from deeper in the stack than the handler was** (w26, w27): Darwin refuses from the raise site's own level (-1,264 bytes) as it does 256 KiB down: the condition is Linux's (the engineer's), not Darwin's.
4. **a `longjmp`-family exit ends it, whatever memory** (w26, w27, f2, e1): `fp3` (`-O0`, `--sanitize`), route A (heap, TLS, late-loaded library, copies), today (copies).

**The repaired clause** (mine; the shortest text I can make true of Darwin on today's C4r-and-landed route C, with the two places where the runtime's route and the compiler's frame layout decide):

> A handler of C's is over where it returns, or where `longjmp`, `siglongjmp`, `_longjmp` or a `setcontext` to a context `getcontext` took takes the thread out of it, unless the jump lands on another stack that was, now or once, given to `makecontext`; left any other way (by a jump no function of the C library makes, such as one written in assembly, or by a `swapcontext` away from it), by such a landing, or jumped within, it is held to be running, and a callback called later on its thread may abort, saying the handler was not seen to end or to switch back.

**What it changes against w26**: (a) `setcontext` is qualified by its target (`sc1` is a falsifier of w26's unqualified word); (b) *"unless the jump lands on another stack that was, now or once, given to `makecontext`"* is an exception to the first half, not a second list, because `fp3`'s `siglongjmp` is *both* a listed end and a landing on such a stack, and a reader who meets both must know which wins (the runtime's: held); (c) the depth condition is dropped (it is false of Darwin as a necessary condition and invites the inference *a shallow callback is safe*); (d) *"a jump no function of the C library makes"* replaces *"an inline jump"* (the golden's own words; `__builtin_longjmp` does not compile on arm64); (e) *"or jumped within"* (the critic's B9, `p8`); (f) the closing words match both S and SW.
**What it does not say, and a sentence true on both platforms still owes**: *"wherever the buffer lives"* holds **only where the runtime verified the C library's buffer layout at start** (route C), which it did on this Mac and which the accessor `hero_signal_jump_route()` reports; on a platform where it did not, a jump to a heap buffer, thread-local storage or a library loaded after start is refused too (route A, rows above: 10 programs). The repaired clause says nothing of it because it is silent about memory, and **silence is true** (`may abort`).

**Run: each atom against the programs of section 1**, today, C4r, landed, route A (the table above is the run). The repaired clause is **true of every cell of section 1** on C4r and landed (an atom marked *may* is satisfied by a run), and **false of none**; on today it is true by *may* of the G forms that run; on route A it is true by *may* of the jumps it lists as ending (`p2`, `p3`, ..., `b_0`, `b_1` refused) **only because it does not promise that a callback runs**: the **certain** half of the clause (*"is over where it returns ..."*) is what the cells falsify on route A, and the sentence must either scope it (*"where this runtime can read the C library's jump buffer"*) or state the exception.
