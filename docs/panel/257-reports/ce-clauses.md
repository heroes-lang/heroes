# Panel 257, compiler-engineer: each clause of w26, w27, f1, f1b, f2, f3 (and the first issue's e1 to e3) against Linux arm64

Written 2026-11-23 by the resumed run of the compiler-engineer seat. Every verdict names its cell and its program; the programs are in
`programs/compiler-engineer/` (`blind/` the lane's p1 to p10, `mine/` mine, `panel255-shapes-*/` panel 255's shapes I ran, `mine-c12/`, `mine-acc/`),
the scripts that built and ran them in `programs/compiler-engineer/scripts/` and `run.sh`, the logs in `programs/compiler-engineer/logs/`. Builds: **today**
(the frozen tree, `828567a30`), **C2679** (panel 255's adopted Linux build over `runtime/parts/`), **landed** (`523d35537`, with one initializer in an unrelated UI
file so that it compiles here: the report's *Against the landed runtime*). C libraries: **Debian glibc 2.41** (container image `heroes-linux-arm64:latest`)
and **Ubuntu 26.04, glibc 2.43** (the CI's). Every cell is 3 of 3 runs unless it says otherwise; `-O0`, `-O2`, plain and `--sanitize` where the report says.
**Marks**: T true on Linux (the cell), **F** false (the program that falsifies it), **U** untested (why).

## 1. The atoms the arms are made of

| # | the claim | programs, cells | mark |
|---|---|---|---|
| A1 | a handler that returns: a callback called after it runs | `p5`: runs, today, C2679 and landed, `-O0` and `-O2`, plain and `--sanitize`, Debian and Ubuntu | **T** |
| A2a | a handler left by **`longjmp`** (the `setjmp` pair): the callback runs | `p1l`: runs, today, C2679, landed, Debian and Ubuntu, plain and `--sanitize` | **T** |
| A2b | by **`_longjmp`** | `p1u`: the same cells, runs | **T** |
| A2c | by **`siglongjmp`**, wherever the buffer lives (stack, static, heap, TLS, mapped page, late-loaded library, 40 live saves, a copy, a buffer the handler refills and leaves by) | p1, p2, p3, p9, p10, `p10h` `p10a` `p10c` `p10d`, `heap`, `mmap`, `handler_heap`, `handler_mmap`, `plain_heap`, `plain_mmap`, `x_heap`, `x_tls`, `x_dlopen`, `jo_*`, `gcopy`, `pngdemo`: run on **C2679 and landed**, both libraries (accessor 3). **Today** runs p1, p2 and refuses p3, p9, p10, `p10c`, `p10d`, `p10h`, `gcopy` in the U words and faults on `mmap`, `handler_mmap` (exit 139) and under `--sanitize` on `heap`, `handler_heap`, `pngdemo` (ASan, `hero_jmp_fingerprint`). Where the decode does **not** verify (C2679A, a forced route A): p3, p9, p10, `p10d`, `p10h`, `gcopy`, `x_heap`, `x_tls`, `x_dlopen` abort in the U words (report § 9a) | **T** on the adopted and landed builds with `decoding: 3` / `route 3`; **F** where the accessor reads lower (forced only; no real platform measured) |
| A2d | by glibc's **`__longjmp_chk`** (the spelling `longjmp` takes in a fortified library; libpng imports it; **no arm names it**) | `p1f`: runs, all builds, both libraries; `pngdemo`: runs on C2679 | **T** (and the arms' lists omit it) |
| A2e | **A2a to A2c on a stack that is not the thread's own** (a handler on an alternate stack leaving for a coroutine's stack, the callback called on that coroutine's stack) | `cs2` ORDER=1: **S**, `siglongjmp` and `setcontext` both, 3 of 3, plain and `--sanitize`, today, C2679, landed, Debian and Ubuntu. `cs2` ORDER=0: runs. `c_1` and `cs_1`: **S on Ubuntu 2.43** (plain; `c_1` 3 of 3, `cs_1` 18 of 20), run on Debian 2.41 | **F** (`cs2` ORDER=1; `c_1` on glibc 2.43) |
| A3 | a handler left by **`setcontext`** (to a context the main line saved): the callback runs | `p7s` (`getcontext` in the main line, `setcontext` in the handler): runs, all builds, both libraries; `p10c` (a copy of the context): C2679 and landed run, today U; **`cs2` ORDER=1 HOW=ctx, `cs_1` on glibc 2.43: S** | **T** for the main line's stack; **F** across two made stacks (A2e) |
| A4 | a handler left by an **inline jump** (assembly on arm64) is *held running* and the next callback aborts | `p4` (callback 256 KiB deep): S, every build, both libraries. **`p4s`** (callback from where the signal was raised), **`p4sd`** (shallow then deep: both run), **`p4x`** (a second signal's handler at the raise depth, then the deep callback), **`p4a`** (an `atexit` function at the natural end): **run**, every build. `p4d`/`p4e`: abort when the callback is deeper than the handler's **wrapper** frame (about 100 bytes at `-O0`, 30 at `-O2` before the handler's own frame), run above it; identical on today, C2679 and landed | **F** for *"the next callback aborts"*; **T** for *"a callback called later on its thread from deeper in the stack than the handler was may abort"* (to within one handler frame) |
| A5 | the abort is **per thread** | `b1` (the main thread's handler truly running, a thread the program started calls the callback): runs; `b2` (handler left by the inline jump, a started thread calls: runs, the main thread's deep call: S); `b3`/`b3c` (a thread C made: the thread rule's T words, with no signal at all) | **T** for *"on its thread"*; **F** for *"a callback called while one is held running aborts"* with no thread named |
| A6 | a handler left by **`swapcontext`** away and never back is *still taken to be running* | `p7`, `g_1_0`, `g_1_1`, `g1_0`, `g1_1`, `g_2_0`, `g_2_1`, `g2`, `g2_1`: **run** (callback 64 KiB deeper than the handler's frame, so a held entry would abort it) on today, C2679 and landed. **`g_3_0`, `g3`: S** on C2679 and landed (today they run); `g_3_1`: runs | **F** (10 of 12 programs run); **T** for form 3 on the thread's own stack. *The clause waits for defect 2702* |
| A7 | **a jump onto another stack given to `makecontext`** is *still taken to be running* | `g_3_0`, `g3` S (C2679, landed); `g_3_1`, `g_2_0`, `g_2_1`, `g2`, `g2_1`, `cs_0`, `c_0`, `cj`, `d_0`, `d_1`, `e`, `ns_0`, `ns_1`, `fp3`, `as_0`: **run** on Debian (C2679 and landed; `b_0`, `b_1` run on landed only, which is all I built). The sentence does not say which jump is meant; of the coroutine programs I ran, only `g_3_0` and `g3` are refused as a *jump onto the coroutine's stack*, and `cs2` ORDER=1, `c_1`, `cs_1` are refused for the reason of A2e | **F** as a rule; **T** for the two |
| A8 | a jump that lands **within** the handler: *held running*, the callback aborts | `p8`, `grefill` (R2: S, today U), `x_partial`, `x_unseen`, `x_unseen0`: S, C2679 and landed; the `...refills-by-memcpy...` golden prints S (R2's move) | **T** |
| A9 | the abort says **the handler was not seen to end** | S: yes, in the line. **K**: `as_1` (a handler re-entered by `setcontext` after a `siglongjmp` left it, which then calls the program) prints *"was called from inside a signal handler: C called it while handling a signal"*, no *not seen to end*, on Debian (today, C2679, landed, plain and `--sanitize`, 3 of 3) and on Ubuntu `--sanitize`; the same binary on Ubuntu plain prints S. **T**: `b3`. **U**: route A only (forced). **D**: the 2494 `given-to-signal` goldens, which pass | **T** for S; **F** if the sentence promises it of every refusal |
| A10 | `exit()` inside a handler reaching an `atexit` function: refused | the 2494 golden: S, 3 of 3 in `-O0`/`-O2` x plain/`--sanitize`, today, C2679 and landed, first stderr line byte-identical (`028f3474...`); gate `fixedbugs-2494`: 15 passed, 0 failed on today and C2679 | **T**; the handler never left, so *"left some way"* does not describe it |
| A11 | a callback that is **C++**: *"a C++ exception through the handler"* aborts | `c5` (clang++, `throw` in the handler): the **direct** form is not correct C++ even with no Heroes (`terminate called`, 3 of 3 plain control); the **pointer** form is caught outside: shallow call runs, 64 KiB deeper S, all builds, identical on today, C2679, landed | **F** for the shallow call; **T** for the deep one |
| A12 | the **inline `__builtin_longjmp`** is the example of a jump no C library function makes | `clang -c` on `__builtin_setjmp` here: *"not supported for the current target"* (aarch64): it does not compile on this platform | **F** as an example on arm64; U on x86-64 (no image) |

## 2. The arms, clause by clause

**w26** (the warden's adopted text): *A handler of C's ends by its return or by a `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of it; left any other way (by an inline jump, a `swapcontext` away from it or a jump onto another stack given to `makecontext`) it is still taken to be running, and a callback called later on its thread from deeper in the stack than the handler was may abort, saying the handler was not seen to end.*
| clause | verdict on Linux | cell and program |
|---|---|---|
| ends by its return | **T** | A1: p5 |
| ends by `longjmp`, `siglongjmp`, `_longjmp` out of it (a callback then runs) | **T** on the thread's own stack; **F** across two made stacks | A2a to A2c: p1, p1l, p1u, p2, p3, p9, p10; A2e: **`cs2` ORDER=1** S, `c_1` and `cs_1` S on glibc 2.43 |
| ends by `setcontext` out of it | **T** (main line); **F** (made stacks) | A3: p7s, p10c; `cs2` ORDER=1 HOW=ctx |
| left by an inline jump: still taken to be running | **T** (at depth) | A4: p4 |
| left by `swapcontext` away: still taken to be running | **F** on Linux until 2702 | A6: p7, `g_1_*`, `g_2_*` run; `g_3_0`, `g3` S |
| left by a jump onto another stack given to `makecontext`: still running | **F** as a rule (2 of about 20) | A7 |
| a callback called later on its thread, from deeper than the handler was, **may** abort | **T** | A4, A5: p4d/p4e, b1, b2 |
| saying the handler was not seen to end | **T** for S, not for K | A9 |
**w27** (the fallback): w26 without the `swapcontext` and `makecontext` clauses. It is **T** in every clause that is T above and **F** in the two clauses that are F for A2e/A3. It **holds on Linux where w26 does not** only in being silent on G, so that p7 running (Linux) and p7 refused (Darwin) both fit it. It is **false** in the same place as w26: *"ends by ... `longjmp` ... or `setcontext` out of it"* is contradicted by `cs2` ORDER=1, today, adopted and landed, on both libraries.

**f1** (*ends ... where this runtime sees it return or sees `longjmp`, `siglongjmp` or `_longjmp` take the thread out of it; a handler left any other way (a jump no function of the C library makes, a switch of context never switched back) or jumped within is held running, and a callback called while one is held running aborts, saying the handler was not seen to end*):
| clause | mark | cell |
|---|---|---|
| ends where this runtime *sees* a return or a `longjmp`... | **T**, and empty: it is true of `cs2` ORDER=1 because the runtime *did not see* the `siglongjmp`, a reader cannot tell which programs those are | A2e |
| left any other way is held running: *a jump no function of the C library makes* | **T** (p4) at depth | A4 |
| *a switch of context never switched back* is held running | **F** on Linux until 2702: p7, **p7s** (a `setcontext` that is never switched back and runs on **every** build, Darwin aside), `g_1_*`, `g_2_*` run | A6, A3 |
| *jumped within* is held running | **T** | A8: p8, grefill |
| *a callback called while one is held running aborts* | **F**: p4s, p4sd, p4x, p4a run; b1 (another thread) runs | A4, A5 |
| *saying the handler was not seen to end* | **T** for S, **F** for K | A9 |
**f1b** (f1 without the context clause): **F** in *"a handler left any other way ... is held running"* the same way, since `swapcontext` away (p7) is *another way* and runs on Linux; otherwise as f1.
**f2** (*a callback called after C left a handler by `longjmp`, `siglongjmp` or `_longjmp` runs, wherever the buffer lives; one called while a handler is held running aborts, ...*): *runs, wherever the buffer lives*: **T** for every memory of A2c on the adopted and landed builds (accessor 3), **F** on a stack the program made (`cs2` ORDER=1: the buffer lives on a coroutine's stack) and **F** with route A (forced); *one called while a handler is held running aborts*: **F** as in f1; the context clause **F** as in f1.
**f3** (*a callback called while a handler of C's has not been seen to end, by return or by `longjmp`, `siglongjmp` or `_longjmp` out of it, aborts, saying so*): read as written, *not seen to end by return or by those three* includes every handler left by `setcontext` (p7s), by `swapcontext` (p7) and by an inline jump called from the raise site (p4s), and the callback **runs**: **F** for **p7s**, **p7**, **p4s**, **p4x**, **b1**; **T** for p4, p6, p8, grefill, A10. *Saying so* is **T** for S and K reads *"called while handling a signal"*.

**The first issue's arms (the critic measured them on the adopted builds; re-measured here)**:
- **e1, e1b**: *"a jump to a buffer the handler refilled"* held running: **F** (p10, `p10h`, `p10c`, `p10d` leave the handler and run, C2679 and landed). *"whatever memory the buffer is in"*: **T** (A2c) and **F** (A2e). *"an inline `__builtin_longjmp`"*: **F** as a spelling on this platform (A12). *"a C++ exception"*: **T** deep, **F** shallow (A11). *"the next callback aborts"*: **F** (A4).
- **e2** (*callbacks run after a handler only where it returned or C left it by `longjmp`... to a buffer a save of that family filled; any other exit aborts*): **F** (p3, p10 run: the buffer was filled by `memcpy`); `p7s` runs.
- **e3** (*where C leaves a handler other than by returning, `longjmp`, `siglongjmp` or `_longjmp`, the next callback aborts*): **F** for `p7s` (`setcontext`), `p4s`.

## 3. The repaired clauses, and the repaired claim run

Each repair says only what the cells above show; where a clause says *"may"*, the run shows both outcomes and the condition that separates them, because a *"may"* with no
condition cannot be falsified and tells a reader nothing. Scripts and logs in `programs/compiler-engineer/`.

**R-1 (A2e, A3: the false clause of w26, w27, f2, e1)**. Replace *"ends by ... out of it"* by
*"... ends by its return or by a `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of it onto the thread's own stack; a handler left for a stack the program made (a coroutine's,
one given to `makecontext`) while the callback is called on another such stack may still be taken to be running ..."*.
**Run**: (a) every jump that lands on the thread's own stack ran: p1, p1l, p1u, p1f, p2, p3, p5, p7s, p9, p10, `p10a`, `p10c`, `p10d`, `p10h`, `tal` (an alternate-stack handler on a thread the program started, the alternate stack mapped before or after the thread's: runs both ways, today, C2679, landed, both libraries); (b) the two stacks the repair names: `cs2` ORDER=0 runs, ORDER=1 aborts S, `siglongjmp` and `setcontext`, both libraries, three builds; `c_1`, `cs_1`: Debian runs, Ubuntu S. **What I tried and did not find**: a jump onto the thread's own stack that aborts; a handler and a callback on the same made stack that aborts (`cj`, `c_0`, `d_0`, `d_1`, `e`, `ns_0`, `ns_1`: run on both libraries).
**What the repair cannot say**: *which* of two made stacks is the lower is the program's mapping order (`cs2`) or the C library's allocator (`cs_1`), so a reader of the repaired sentence cannot predict the cell. That is the runtime's, filed here for the lane: `siginstall.c`, `hero_sig_prune` (lines 150 to 160 of the frozen file, unchanged in the landed tree) compares two addresses on stacks it does not know. Until it changes, the sentence has to carry the *"may"*.

**R-2 (A6, A7: the context clauses of w26, f1, f2)**. Replace *"a `swapcontext` away from it or a jump onto another stack given to `makecontext`"* by
*"(until defect 2702 lands) a `swapcontext` onto a stack of the program's own making that does not return to the thread's stack by a jump this runtime sees"*, or drop it from the Linux reading and keep **w27**.
**Run**: `p7`, `g_1_0`, `g_1_1`, `g1_0`, `g1_1` (to the runner's own context), `g_2_0`, `g_2_1`, `g2`, `g2_1` (through a coroutine that `siglongjmp`s to it) all **run**; `g_3_0`, `g3` (the coroutine returns through `uc_link`, a `setcontext` the runtime does not see) **S**; `g_3_1` (the handler on an alternate stack) runs. Identical on C2679 and landed. **What 2702 changes**: Darwin's C4r already refuses p7; on Linux this repair is false the day 2702 lands, so it carries *"until 2702"*.

**R-3 (A9: the words)**. Replace *"saying the handler was not seen to end"* by *"saying that a signal handler of C's was not seen to end, or that C called it while handling a signal"*.
**Run**: `p4` prints S (*"had not been seen to end"*); `as_1` prints K (*"C called it while handling a signal"*), Debian 3 of 3 on today, C2679 and landed; the same `as_1` binary on Ubuntu plain prints S. The two thread-rule and delivery words (T, D) belong to other rules and stay out.

**R-4 (A4, A5: the unqualified *"aborts"* of f1, f1b, f2, f3, e1 to e3)**. Replace *"a callback called while one is held running aborts"* by *"a callback called later on its thread, from deeper in the stack than the handler was, may abort"* (w26's own words).
**Run**: `p4d` (PAD 0 to 65536, 3 builds, 3 of 3 each: runs to 4,608 bytes, aborts from 5,120), `p4e` (the boundary: aborts when the callback is within about 100 bytes of the handler's frame or below it at `-O0`, 30 at `-O2`; identical on today, C2679, landed), `p4s` (shallow: runs), `p4sd`, `p4x` (a later handler clears it), `b1`/`b2` (another thread runs). **Residue**: *"deeper than the handler was"* is exact to within one handler frame, which a sentence may ignore.

**R-5 (A10: the class)**. Where an arm defines the refused class as *"left some way"*, add *"or still running"*: the handler of C12 never leaves. **Run**: the 2494 golden, S, 12 of 12 cells (4 builds x 3 runs) on today and C2679; gate 15 passed, 0 failed on each.

**R-6 (A2c, e1, e1b)**. Drop *"a jump to a buffer the handler refilled"*: p10, `p10h`, `p10c`, `p10d` run. Name *"a jump that lands within the handler"* (p8, `grefill`, `x_partial`, `x_unseen`: S).

**R-7 (A2d)**. Where the sentence names `longjmp`, say *"(`longjmp`, `_longjmp`, `siglongjmp`, in whichever spelling the library gives them)"*, so that glibc's `__longjmp_chk` (libpng's, `nm -D`) is read as one of them. **Run**: `p1f`, `pngdemo`.

**R-8 (A12, A11)**. Do not name `__builtin_longjmp` as the example of a jump no library function makes: it does not compile on arm64 clang. Name *"a jump written in the program's own assembly (or `__builtin_longjmp` on x86-64)"*, or no example. A C++ exception through a handler is **correct C++ only through a call the compiler may treat as throwing** and then only the deep callback aborts (`c5`, report § 3).

## 4. Untested, and why

- **Linux x86-64** (`ubuntu-26.04`): no image here. The inline jump's `__builtin_longjmp` branch (A12), the depth threshold (4,608 to 5,120 bytes here, which I take to be arm64's signal frame plus `raise`: an inference), and the calibration's x86-64 verification (carried from panel 255, read in `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump.hero`'s header) rest on it. The mechanism of A2e is an address comparison with no machine-specific part, so I expect it there, **an inference**.
- **Windows**: no probe in the runtime (`runtime/parts/sigguard.c`, the `_WIN32` branch, *"no probe and no walk"*, unchanged at `523d35537`): every clause that says a callback aborts is false there by the runtime's own comment; unrun.
- A C++ exception from an asynchronous signal (a timer) and `-fnon-call-exceptions` on the direct form: unrun.
- `jo_co`: the ffi-pragmatist's shape does not compile with this clang (`-Walloc-size` on a `calloc(1, 1024)` for a `ucontext_t`): not built.
- A real platform with route A (`decoding` below 3): none is reachable here; the forced build (C2679A) is the only reading.
