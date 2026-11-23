# Panel 257, the ffi-pragmatist: the sentence against real C, on Darwin

Written 2026-11-23 as I go, from 11:10 by this Mac's clock (`date`), in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/ffi-pragmatist/` (`F/` below), a detached
worktree at `828567a30` (`git log -1 --format=%H` printed it, `git status --short` printed nothing). Machine: this Mac,
Darwin arm64, Apple clang 21.0.0 (`clang --version`), macOS 27.2.1. Load at the start: `uptime` read 7.21 17.46 44.66
(shared, so every duration here is indicative only). No paid run, no credential printed, nothing outward, no file outside
the repository root. A fact without its command says *unrun* or *recalled*.

## Status (updated as sections land)

- 11:10 seed compiler built in `F/` (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 13.3 s, `heroes --version`
  printed `heroes 0.6.0 / sources 05186f2b38e77a97`, the critic's seed key too).
- 11:10 evidence extracted into `F/ev/` (`find F/ev -type f | wc -l` = 4510, the archive's own count).
- `F/p257/rt/today` = `cp -R runtime`; `F/p257/rt/c4r` = the same with the two files of the archive's `r2/C4r/` over
  `parts/` (`md5 -q`: `rebind.c` 2730f1617d4d1ff35d9e5074f209c2da, `sigguard.c` 3c603aff7998aa786c4d01970b7fe2cb, both equal to the
  table of the shared brief's section 4; today's: `4d39ad4af1c9ef0720f711ea0e4180e4`, `f035b38c36e16875959e6f76bbbd09b9`).

## Method (as run)

- Compiler: **my own stage 2**, `F/heroes-next` (`./heroes build selfhost/main.hero -o heroes-next` from the seed,
  `stage2.log`: `real 285.47`, `heroes-next --version` printed `sources 56db985736468b40`, the critic's stage 2 key too).
  **The adopted Darwin C4r build built**: `F/heroes-c4r` is stage 2 built by the same seed with
  `HEROES_RUNTIME=F/p257/rt/c4r` (`stage2-c4r.log`: `wrote heroes-c4r`, `real 279.46`); its programs and the net's
  goldens are built with the same variable exported (`F/p257/gate.sh`). Today's: `HEROES_RUNTIME=F/p257/rt/today`
  (a `cp -R runtime`, `diff -rq runtime p257/rt/today` printed nothing).
- A cell: build by `F/p257/prog/run.sh` (the folder's own copy is in `257-reports/programs/ffi-pragmatist/`), `-O0` (**plain
  is `-O0`: `./heroes --help` says `-O0 ... (the default here)` under `build`; `heroes run` defaults to `-O2`, so the cells
  below say `o0`, `o2` and `san` = `--sanitize` at `-O0`**), then 3 runs from the folder under `timeout 20`, **both streams**
  through `2>&1 | head -c 100000 > o.txt`, the exit from `PIPESTATUS`, one log line per distinct outcome (`count exit | stdout | the
  first `panic:` or sanitizer line, kind S/U/K/D`). Library cells link a dylib built by `libs.sh` (`clang -O2 -fPIC -dynamiclib`).
- The machine was shared and loaded: `uptime` read 55 to 64 for the load average at 11:25 (indicative: no duration is a result).

## 0. Did the adopted Darwin build build, and what did `run fixedbugs-2522` read

**Yes, it built, and it read 23 passed, 2 failed, exactly the two goldens R2 moves.** Commands: `F/p257/gate.sh c4r fixedbugs-2522`
(`HEROES_RUNTIME=F/p257/rt/c4r ./heroes-c4r run tests/harness/main.hero -- ./heroes-c4r run fixedbugs-2522`, `timeout 2400`,
`head -c 400000`; log `F/p257/logs/gate-c4r-fixedbugs-2522.log`):

- `run (only fixedbugs-2522): 23 passed, 2 failed`, `harness: 23 passed, 2 failed`, `EXIT 1`;
- the two: `fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` (still refused, its words now S, the
  `.expected` pins U) and `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`
  (runs: the actual output has `called 8` after `escaped 1`, exit 0).
- **Today**, same script with `today` (`heroes-next`, `rt/today`): `run (only fixedbugs-2522): 25 passed, 0 failed`, `EXIT 0`
  (`logs/gate-today-fixedbugs-2522.log`). 25 and 0 on the C4r build would have been a build that did not move them; it did.
- I did **not** run the compiler's own tests (`byte for byte (defect 2533)` is red at the frozen tree by the brief's section 1;
  the compiler's own tests are not this seat's question): *unrun*.
- The 2494 set and the recovery golden, the two that bear on item 10 and on (b): section 10 below.

## 1. The table of section 6, C1 to C9 (and C12), Darwin, today and C4r

Every cell is **3 runs** (`timeout 20`, `2>&1 | head -c 100000`, exit from `PIPESTATUS`), **plain (`o0`) and `--sanitize` (`san`)**;
`run` = exit 0 and the program's own lines; `134 S`/`134 U`/`134 K` = exit 134 and the first line of stderr is the refusal of that
kind (S *"had not been seen to end"*, U *"holds no record of"*, K *"C called it while handling a signal"*); `134 ASan:uaf` = an
AddressSanitizer report (a heap-use-after-free); `139` = the process died of SIGSEGV; `134+ran` = the program printed its answer
and the process then aborted (a Heroes function ran inside the signal); `134 other` = another first line (named below). Where the
three runs of a mode differ the cell says so; none did except `watchdog_h2` (section 3). A cell with `(x6)` was run in two
batches of 3 (the same outcome). Sources: `F/p257/logs/{today,c4r}-{shapes,shapes-A,shapes-G,prog,prog2,prog-R}.log`, one line
per distinct outcome; rendered by `F/p257/mktable.sh` (`md.py`). The programs are in
`257-reports/programs/ffi-pragmatist/`: `p1` to `p10` and the panel-255 shapes under `shapes/`, the real libraries in the folder
itself and in `watchdog-a`, `-b`, `-c`.

| id / program | Darwin **today** (first outcome of 3 runs a mode; o0 / o2 / san where they differ) | Darwin **C4r** |
|---|---|---|
| **C1** | *longjmp, siglongjmp, _longjmp to a stack or static buffer: the callback runs* | | |
| `p1` | run | run |
| `a-callback-after-a-longjmp-out-of-a-handler-on-its-own-stack-deep-first-runs` | run (o0/san) | run (o0/san) |
| `a-callback-after-a-siglongjmp-out-of-a-handler-on-an-alternate-stack-shallow-first-runs` | run (o0/san) | run (o0/san) |
| `a-callback-after-an-underscore-longjmp-out-of-a-handler-on-its-own-stack-shallow-first-runs` | run (o0/san) | run (o0/san) |
| **C2** | *the same, buffer in heap, TLS, a mapped page, a late-loaded library, or 40 live saves: runs* | | |
| `p2` | run | run |
| `p9` | run | run |
| `heap` | o0: run ; san: 134 ASan:uaf | run (o0/san) |
| `mmap` | o0: 139 ; san: 134 ASan:SEGV | run (o0/san) |
| `handler_heap` | o0: run ; san: 134 ASan:uaf | run (o0/san) |
| `handler_mmap` | o0: 139 ; san: 134 ASan:SEGV | run (o0/san) |
| `plain_heap` | o0: run ; san: 134 ASan:uaf | run (o0/san) |
| `plain_mmap` | o0: 139 ; san: 134 ASan:SEGV | run (o0/san) |
| `x_heap` | run (o0/san) | run (o0/san) |
| `x_tls` | run (o0/san) | run (o0/san) |
| `x_dlopen` | run (o0/san) | run (o0/san) |
| `jo_heap` | run (o0/san) | run (o0/san) |
| `jo_tls` | run (o0/san) | run (o0/san) |
| `jo_lib` | run (o0/san) | run (o0/san) |
| `pngcb` | o0: run (x6) ; o2: run (x6) ; san: 134 ASan:uaf (x6) | run (x6) |
| `pngdemo` | o0: run ; o2: run ; san: 134 ASan:uaf | run |
| **C3** | *a whole copy of a saved buffer, and a buffer the handler refills from a save the main line made, then leaves by: runs (R2)* | | |
| `p3` | 134 U | run |
| `p10` | 134 U | run |
| `gcopy` | 134 U (o0/san) | run (o0/san) |
| `a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so` | 134 U (o0/san) | run (o0/san) |
| **C4** | *an inline jump: aborts (S)* | | |
| `p4` | 134 S | 134 S |
| `a-correct-inline-jump-out-of-a-handler-is-refused-in-words-never-false` | 134 S (o0/san) | 134 S (o0/san) |
| **C5** | *a C++ exception through the handler: aborts (S)* | | |
| `cxxguard` | 134 other | 134 other |
| **C6** | *a jump that lands within the handler, then a callback: aborts (S)* | | |
| `p8` | 134 S | 134 S |
| `rr_0` | 134+ran (o0/san) | 134 U (o0/san) |
| `rr_1` | 134+ran (o0/san) | 134 U (o0/san) |
| `rr_2` | 134+ran (o0/san) | run (o0/san) |
| `a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` | 134 U (o0/san) | 134 S (o0/san) |
| `a-handler-that-jumps-within-itself-by-longjmp-then-calls-deep-is-refused` | 134 S (o0/san) | 134 S (o0/san) |
| `a-handler-that-jumps-within-itself-by-siglongjmp-then-calls-deep-is-refused` | 134 S (o0/san) | 134 S (o0/san) |
| **C7** | *a context switched away from inside a handler and never switched back (G): aborts (S)* | | |
| `p7` | run | 134 S |
| `g1_0` | run (o0/san) | 134 S (o0/san) |
| `g1_1` | run (o0/san) | 134 S (o0/san) |
| `g2` | run (o0/san) | 134 S (o0/san) |
| `g2_1` | run (o0/san) | 134 S (o0/san) |
| `g3` | 134 U (o0/san) | 134 S (o0/san) |
| `as_1` | 134 K (o0/san) | 134 U (o0/san) |
| `nn` | run (o0/san) | 134 S (o0/san) |
| `sched` | 134 U | 134 S |
| `watchdog_b` | run | 134 S |
| **C8** | *a handler left by a jump into a coroutine / the stale-range shape* | | |
| `fp3` | run (o0/san) | 134 S (o0/san) |
| `as_0` | run (o0/san) | 134 S (o0/san) |
| **C12** | *exit() inside a handler reaching an atexit function* | | |
| `fixedbugs-2494-a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` | 134 S (o0/san) | 134 S (o0/san) |

**Which row of mine `rr_*`, `nn` and the A-second forms are.** *(a)* `rr_0`, `rr_1`, `rr_2` (the handler saves, jumps out to the
runner, the runner at once jumps back into the handler's save): C6's neighbours, a jump that **lands within the handler**, but a
handler that had already been left, so the jump is **undefined in C** (section 4), not a correct program: today runs the program
inside the signal then aborts (`134+ran`, 3 of 3 both modes) and **C4r refuses `rr_0`, `rr_1` in U words**; `rr_2` (the jump back
made from a coroutine on another range) **runs under C4r, exit 0** (`answer ran for 82`, 3 of 3 both modes): the residue panel 255
named, **a Heroes function runs inside a resumed handler on C4r**; it is the one row of this table where C4r runs a program
inside a signal, and no arm's clause reaches it. *(b)* `nn` (two handlers, three contexts): **C7's** family (a context switched
inside a handler): today runs it inside the signal (`answer ran for 31`), C4r refuses it S. *(c)* the A-second forms: `as_0`
(handler on its own stack, a jump into an array coroutine) is **C8**; `as_1` (the coroutine resumes the handler's saved context)
is C7's resumption of a handler left: today K, **C4r U**.

## 2. The ten programs, `p1` to `p10`, today and C4r, `-O0` / `-O2` / `--sanitize`

Built by my stage 2 against each runtime, **3 runs a cell, all three modes** (`plain` is `-O0`, the same binary: `heroes build`'s
default), `F/p257/logs/{today,c4r}-shapes.log`. The programs are the critic's own bytes (`shasum -a 256` of
`257-briefs/blind/programs/p*.h` and `p1.hero`, first 12 hex: p1.h 9cadabccde1b, p2.h 174ff046ee2f, p3.h d64041ce5ee8, p4.h
cb459e82fb0d, p5.h f1a08b31e9d1, p6.h 60001822e492, p7.h 4b72842a94b5, p8.h 79c5a0f346f1, p9.h e86de7a0fb53, p10.h
39fc285ad0cf, prelude.h 95d7d7d6618a; `cmp -s` of each of the 21 files of that folder with my copy in `F/p257/shapes/`: 21 files, 0 differing).

| program | what C does | today | C4r | the critic's cell, re-measured |
|---|---|---|---|---|
| `p1` | siglongjmp out, static buffer | run | run | same |
| `p2` | siglongjmp out, heap buffer | run | run | same |
| `p3` | siglongjmp through a whole copy of the saved buffer | 134 U | run | same |
| `p4` | a jump no library function makes (asm on arm64) | 134 S | 134 S | same |
| `p5` | the handler returns | run | run | same |
| `p6` | the handler calls the program function itself | 134 S | 134 S | same |
| `p7` | the handler swapcontexts to before the signal, never resumed (G) | run | 134 S | same |
| `p8` | the handler jumps within itself, then calls | 134 S | 134 S | same |
| `p9` | siglongjmp to the first of 40 live saves | run | run | same |
| `p10` | the handler memcpys an outside save and siglongjmps to the copy | 134 U | run | same |

**All ten cells equal the critic's** (`critic-briefs.md`, "The registered key": Mac today and Darwin C4r columns, 3 of 3 each) and
the key's *adopted* column for p1 to p7, and **no mode moves a verdict** (`-O0`, `-O2`, `--sanitize`: 3 of 3 in each, 180 runs
for the ten programs: `10 programs x 3 modes x 2 builds x 3 runs`).

**p7 (G), re-measured, not copied: today RUNS on this Mac and C4r refuses it.** Today: exit 0 and `answer ran for 7` / `result 70`,
3 of 3 in `-O0`, `-O2` and `--sanitize`. C4r: exit 134 and the first line of stderr is the S refusal, 3 of 3 in each mode
(`logs/c4r-shapes.log`, `p7 o0/o2/san 3 exit=134 | | S`). **Confirmed.** Beside it, the same shape as a real library
(`watchdog-b`, section 3c) and as the author's own forms (`g1_0`, `g1_1`, `g2`, `g2_1`: today run, C4r S; `g3`: today U, C4r S).

## 3. Real C: the bindings the sentence implies, written, compiled against their real headers, built and run

Everything below is in `257-reports/programs/ffi-pragmatist/` (headers, `.c`/`.cpp`, `.hero`, scripts; no binary). Each Heroes
program binds a C library through an `extern "<header>" link|pkgconfig "<lib>"` group whose signatures **clang checks against
the header at build**; each library also has a **plain-C driver** (`<lib>_main.c`, no Heroes) that is the control for what C
itself does. Libraries are compiled by `libs.sh` (`clang -O2 -fPIC -dynamiclib -Wall -Wextra`; one `unused function` warning in
`watchdog_h.c` was removed with `__attribute__((unused))` before the Heroes runs, and the last `libs.sh` printed none). The plain-C
drivers, 3 runs each under `timeout 20`: `watchdog_a`, `_b`, `_c`, `probe`, `sched` at `-O0` and at `-O2`, `lazy`, `watchdog_h`
at `-O2`, every one **exit 0 with the output named below, 3 of 3**; `cxxguard` (3c) exit 134, 3 of 3.
**The header guard is intact on every one of them**: a wrong result type (`watchdog_a_bad1.hero`, `-> i32` for the header's
`int64_t`) is `error[ffi_return_type]: ... returns int64_t in watchdog_a.h, not i32 - clang read the header`, and a wrong callback
parameter (`watchdog_a_bad2.hero`, `function(i32)`) is `error[ffi_callback_type] ... clang read the header, and it says long long
(*)(long long)`: both build errors, with C4r's runtime selected (`F/p257/prog/`, `heroes-next build`, no run).

### 3a. libpng 1.6.59's error path (`png_error` is a longjmp that is not in a handler: the control)

`pngdemo.h` is panel 255's own 25-line shim (verified against `png.h` by `pkgconfig "libpng"`; `pkg-config --modversion libpng` =
1.6.59; `good.png` 2299 bytes, `trunc.png` 200 bytes, `ls -la`), copied from the archive; `pngcb.h` is mine: the same shim with
`png_set_error_fn`, so **a Heroes function is called inside libpng's error path** (`pc_error_fn` calls the program's callback,
returns, and libpng's `png_default_error` then `longjmp`s to the `setjmp` of the binding's frame, inside the heap `png_struct`).
Decodes `good.png`, `trunc.png`, `good.png` again, `trunc.png` again.

| cell | today | C4r |
|---|---|---|
| `pngdemo` plain / `-O2` | exit 0, `good 180180` `truncated -3` `good again 180180` `truncated again -3` (3 of 3) | the same (3 of 3) |
| `pngdemo` `--sanitize` | **exit 134, AddressSanitizer: heap-use-after-free** (3 of 3): `hero_rebind_fingerprint rebind.c:334` <- `hero_rebind_settle` <- `hero_rebind_note` <- `hero_rebind_setjmp rebind.c:445` <- `png_create_png_struct` <- `pd_decode pngdemo.h:11` (the second decode), the block freed by `png_destroy_png_struct` at `pngdemo.h:25` (the first) | exit 0, no report (3 of 3) |
| `pngcb` plain / `-O2` | exit 0, `error callback ran for 1` after each truncated decode (3 of 3, run in two batches: x6) | the same |
| `pngcb` `--sanitize` | the same heap-use-after-free (x6) | exit 0, the callback ran (x6) |

What a reader who followed the sentence would expect: **it runs** (no signal handler is involved; the sentence is about jumps out of
handlers, and the control says so). **On C4r that is what happens, in all three builds.** On **today's** runtime it runs only by
luck: the runtime itself reads memory libpng freed (`hero_rebind_setjmp`'s fingerprint, defect 2679), which `--sanitize` shows and
the plain build hides: a design.md section 1.12 failure of the runtime, with no handler and no jump out of one. So the sentence
is true of libpng on C4r and says nothing about the bug C4r removes; it needs no clause for libpng. (A `longjmp` that abandons a
frame of a Heroes function, which libpng does when a Heroes read function reports an error with `png_error`, is a different
problem: 3e below.)

### 3b. A SIGSEGV/SIGBUS recovery handler that siglongjmps out (`probe`), and one that returns (`lazy`)

The golden `fixedbugs-a-c-handler-that-recovers-leaves-no-line` (read: `tests/golden/run/`) recovers from its own `abort` and trap
through a constructor-installed handler and calls nothing of the program afterwards; **it passes on both builds** (the set, section 10).
`probe.c` is the library a debugger, a fuzzer or a crash reporter links: `probe_scan` maps six pages, makes pages 1 and 5
`PROT_NONE` and unmaps page 3, reads one byte of each under a `sigaction(SA_SIGINFO)` handler for SIGSEGV and SIGBUS that
`siglongjmp`s back to a `sigsetjmp(env, 1)` in the loop, and after each recovery **calls the program's `on_fault(page)`**
(a Heroes function). Hardware faults, not `raise`. Plain C: `fault at page 1 signal 10`, `page 3 signal 11`, `page 5 signal 10`
(Darwin delivers SIGBUS for a `PROT_NONE` page and SIGSEGV for an unmapped one), `result 90`, 3 of 3 at `-O0` and at `-O2`.
`probe_lazy` (`lazy.hero`): a SIGSEGV/SIGBUS handler that `mprotect`s the page readable and **returns** (a lazy loader, a write
barrier); then calls the callback.

| cell | today | C4r |
|---|---|---|
| `probe` (`-O0`, `-O2`, `--sanitize`) | exit 0, the three fault lines, `result 90` (3 of 3 each) | the same (3 of 3 each) |
| `lazy` (`-O0`, `-O2`, `--sanitize`) | exit 0, `page 1 loaded` `result 10` (3 of 3 each) | the same |

**The sentence is true of both**: a handler left by `siglongjmp` after a hardware fault, and one that returns, both run on today and
on C4r, plain, `-O2` and sanitized, with the callback called after. What the sentence would not tell a reader is what C and POSIX
say of them (section 4): the first is not on this Mac's list of functions a handler may call (`siglongjmp` absent), the second is
what C11 calls undefined for a computational exception (recalled), and both work.

### 3c. The watchdog: three real libraries driven by a timer signal, `watchdog-a`, `-b`, `-c` (the key of t2, which had none)

Each folder of `257-reports/programs/ffi-pragmatist/watchdog-{a,b,c}/` holds the library (`watchdog_X.c` and `.h`, built as a
`libwatchdog_X.dylib`), a plain-C driver, the Heroes program and a `run.sh` (build, then 3 runs of each binary). The library
`run_with_deadline(cb)` **arms `setitimer(ITIMER_REAL)` for 50 ms** (the signal is SIGALRM, delivered by the kernel while the
thread runs a computation of 2^40 steps that never ends by itself; `raise` is not used), the handler is the design's, and after it
the library calls `cb(7)` and returns its answer; `watchdog_interrupted()` is 1 only if the computation was left by the timer with
progress made. The handlers are the t2 task's three, word for word: **A** `siglongjmp(env, 1)` (a static `sigjmp_buf` saved by
`run_with_deadline`), **B** `swapcontext(&handler_ctx, &scheduler)` (a context saved by `getcontext` before the timer started,
never resumed), **C** `deadline_hit = 1` (the computation polls it). The Heroes program passes `answer` as `cb`.
**Plain C, no Heroes (the control): A, B and C each print `answer ran for 7` / `result 70` / `interrupted 1`, exit 0, 3 of 3.**

| Heroes program | today (`-O0`, `-O2`, `--sanitize`, 3 runs each) | C4r (the same) |
|---|---|---|
| `watchdog-a` (siglongjmp) | exit 0: `answer ran for 7`, `result 70`, `interrupted 1` | the same |
| `watchdog-b` (swapcontext away, never back) | **exit 0**: the same three lines | **exit 134, S**: `panic: watchdogb.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running ...` |
| `watchdog-c` (a flag) | exit 0: the same three lines | the same |

(`F/p257/logs/{today,c4r}-prog2.log`; the folder script `watchdog-b/run.sh`, run from a copy against `rt/c4r`, printed the same
S line 3 of 3 in each mode.) **So the key of t2 (A runs, B aborts, C runs) is what C4r does on Darwin, 3 of 3 in all three modes, and it
is not what today's runtime does: today B runs.** What a reader who followed arms f1 or f3 would expect: A runs (a `siglongjmp`
out), C runs (the handler returned), **B aborts** (a switch of context out of a handler never resumed): all three predictions hold.
What the abort *says*, for the author of the library's C: *"C left the handler in a way this runtime does not follow"* is true
of B, but the parenthesis of causes beside it names an inline `__builtin_longjmp`, a C++ exception and a jump inside a system
library, **not a switch of context** (section 6).
**These three also show that a handler installed by a dylib's own `sigaction` call is entered through the runtime's trampoline
under C4r** (the library is a `.dylib` whose call goes through libSystem's stub, `rebind.c`'s header says it rewrites those slots
before `main`): B could not be refused otherwise.

### 3d. A preemptive green-thread scheduler (`sched`): SIGALRM, the handler swapcontexts to the next thread

`sched.c` (a library a user-level threading package ships): `sched_spawn(entry, arg)` makes a context on a 256 KiB `malloc`ed stack,
`sched_run()` arms a **5 ms periodic `ITIMER_REAL`** and runs three green threads, each of them the program's `sum(n)` (a Heroes loop of
100, 150 and 200 million additions), and **the SIGALRM handler `swapcontext`s to the next thread that is not done**; a finished
thread `setcontext`s to the next. Plain C: `preempted true`, the three sums (`5000000050000000`, `11250000075000000`,
`20000000100000000`), exit 0, 3 of 3 at `-O0` and `-O2`. Heroes (`sched.hero`):

| today | C4r |
|---|---|
| **exit 134, U** (3 of 3 each of `-O0`, `-O2`, `--sanitize`): `panic: sched.sum was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with memcpy, refilled where the runtime does not look, or saved before more save points than it keeps) ...` | **exit 134, S** (3 of 3 each): `panic: sched.sum was called while a signal handler of C's that this runtime saw begin had not been seen to end ...` |

**It is refused on both builds, and nothing of it ran** (no line printed before the abort): the second thread's first `sum` is entered
by the first tick's handler, a context switch made *inside* a handler. A reader of f1's clause *"a switch of context never switched
back"* would **not** predict this: every thread of this scheduler is switched back to (a handler's context is resumed by the next
tick; the program ends with all three sums right in C); the refusal is about a callback **entered while another handler stands
suspended**. Today's words (U) are false of it (no buffer, no copy, no save point count). **A real library class that today refuses
in false words and C4r refuses in S words that do not name it.**

### 3e. A jump that abandons the program's own frames (`watchdog_h1`, `watchdog_h2`, `abandon`): a finding beside the sentence

Found while writing 3c, as the shape *beside* it (CLAUDE.md CL-061): the natural timeout wrapper does not interrupt the library's
own computation, it interrupts **the program's function** (`run_with_deadline(work, after)`: arm a 50 ms timer, call `work(0)`,
and when the SIGALRM handler `siglongjmp`s out, call `after(7)`). Design A again, so **the sentence's clause *"the callbacks run
after a longjmp, siglongjmp or _longjmp"* is the one that applies**; the jump now crosses the Heroes frames of `work`.

| program | today | C4r |
|---|---|---|
| `watchdog_h1`: `work` is a Heroes loop of integer arithmetic | exit 0, `answer ran for 7`, `result 70` (3 of 3 in `-O0`, `-O2`, `--sanitize`) | the same |
| `watchdog_h2`: `work` builds a `str` each turn (`f"{i}"`) | `answer ran for 7`, `result 70`, then **exit 134, `panic: 1 heap blocks still live at exit (a missing decref) - this is a compiler bug`** (3 of 3 in each mode; the block count was 1 or 2 from run to run) | the same (3 of 3 each; 1 or 2) |
| `abandon`: **no signal at all**: a C function `longjmp`s (`jmp_buf` static) over the frame of a Heroes callback that holds a `str` | `result 7`, then **exit 134, `2 heap blocks still live at exit ... this is a compiler bug`** (3 of 3 each mode) | the same (3 of 3 each) |

(`F/p257/logs/{today,c4r}-prog-R.log`, `{today,c4r}-prog.log`; plain C for `watchdog_h`: a C `work` that loops `malloc`/`free`
is abandoned by the same handler, `after ran for 7`, exit 0, 3 of 3.) **The callback does run, as the sentence says, and the program
then ends by aborting at its end with a message that blames the compiler**, on today's runtime and on C4r alike, with a handler
(`watchdog_h2`) and without one (`abandon`): a frame that a C jump abandons never releases the heap values it held, and the exit-time
check (`runtime/parts/alloc.c:357`, `hero_live_blocks != 0`) says *"a missing decref ... this is a compiler bug"*. I searched the spec
and the design (`grep -n "still live at exit" docs/design.md spec/heroes-spec.md` printed nothing) and the issues
(`gh issue list --search "longjmp heap blocks still live" --state all` and `"jumps over frames of the program leak"`: both `[]`):
**no document says what happens to a Heroes frame a C jump abandons, and the abort's words are false of it** ("never false" is
the golden's own standard, `fixedbugs-2522-a-correct-inline-jump-out-of-a-handler-is-refused-in-words-never-false`). The leak is
a count the runtime itself keeps and reports, not a memory error (`grep -c AddressSanitizer` over the last `--sanitize` output of `watchdog_h2` and of
`abandon`, on each build: 0 in all four files), but it is a **C program that ends in an abort with a false cause**; a libpng binding whose read
function reports an I/O error with `png_error` would be this shape (**unrun**: I did not write that binding). The sitting's sentence is
about the callback after a jump; this is the callback *under* one.

### 3f. C5, a C++ exception thrown out of a handler: **no correct program exists on Darwin arm64** (`cxxguard`)

`cxxguard.cpp` (`clang++ -O2 -fPIC -dynamiclib -fexceptions -fnon-call-exceptions`, a C header over `extern "C"`): `cxx_guard(where, cb)`
installs a SIGSEGV/SIGBUS handler (`SA_SIGINFO`) that does `throw Fault()`, stores through the pointer it is given (a bad one: address 16 in the
plain-C driver, a null pointer in the Heroes program; a trapping instruction either way), catches `Fault` in its own frame and calls `cb(7)`. **Plain C driver, no Heroes: exit 134,
`libc++abi: terminating due to uncaught exception of type Fault: fault`, 3 of 3**: the unwinder does not cross the signal frame
(`_sigtramp`) into the frame that faulted, so the `catch` is never reached and no callback is ever called. Under Heroes it is the same
on both builds, `-O0`, `-O2`, `--sanitize`, 3 of 3: the line above, then the runtime's own
`panic: the process is dying of SIGABRT, raised by this process, under cxxguard.main, and this runtime did not raise it. A C library
ends a program this way when it catches misuse ...` (exit 134). **So the S words' "a C++ exception" is not what a Darwin program meets:
the program dies in libc++abi before the runtime sees any callback**, and the shared brief's "a statement of the words, never
measured" is now measured on the one platform it can be: there is *no* clang++ program here that leaves a handler by an exception
and reaches a Heroes callback. `g++` is clang on this Mac (`g++ --version`: Apple clang 21.0.0), so this is one front end. Whether
GCC/glibc on Linux unwinds through a signal frame under `-fnon-call-exceptions` is **recalled (it does), unrun**, and is the
engineer's.

### 3g. Third-party C libraries bound beside them (what the sentence must not break)

Each is a small `extern` group over the real header (`pkgconfig`/`link`), a Heroes callback in the library's path, **3 runs in each
of `-O0`, `-O2`, `--sanitize` on each build, every one exit 0** (`F/p257/logs/{today,c4r}-prog-R.log` and `-prog-X.log`):

| library (version) | what is bound | the signal or jump in it | today | C4r |
|---|---|---|---|---|
| **libuv** 1.53.0 (`uvsig`) | `uv_signal_start` + `raise(SIGUSR1)` + `uv_run`; the callback is a Heroes function | libuv's own C handler writes a byte to a pipe and **returns**; the loop calls the callback | `signal callback ran for 30`, `result 0` | the same |
| **SQLite** 3.54.0 (`sqlint`) | `sqlite3_progress_handler` with a Heroes callback; a 30 ms SIGALRM handler calls `sqlite3_interrupt` | the signal arrives **while Heroes code (the progress callback) runs**; the C handler returns | `interrupted 1` | the same |
| **OpenSSL** libcrypto 4.1.0 (`ossl`) | `EVP_Digest` SHA-256 of "abc", callback with the first byte | libcrypto imports `sigaction` and `siglongjmp` (`nm -gu`, section 8) | `first byte 186`, `result 186` | the same |
| **GNU readline** 8.3 (`rl`) | callback mode, one line from a file, after a raised SIGWINCH | readline installs its own handlers (`rl_catch_signals`); the handler returns | `line of 14 bytes`, `result 0` | the same |
| **libpng** 1.6.59 | 3a | `png_error` longjmp, no handler | 3a | 3a |

None of these four is refused by anything on either build, which is what the sentence predicts: **a handler of the library's that
returns, or a jump the library makes outside a handler, is not what the sentence is about.** (`readline` needs `PKG_CONFIG_PATH`
for its keg-only `.pc`; `go.sh` sets it.)

## 4. "Though C is correct": what C and POSIX say of each program the arms say is refused

**What I read and what I recalled.** There is no copy of ISO C or of POSIX on this machine: `find / -maxdepth 6 \( -iname 'n1570*'
-o -iname 'c11*.pdf' -o -iname '*posix*.html' \)` printed nothing, and `find ... -iname 'siglongjmp*' -o -iname 'longjmp*'` found only
the SDK's copies of the BSD man pages below. **So every clause of C11 or POSIX in this section is *recalled* (marked R) and none is
quoted; the clauses marked READ are this Mac's own man pages (`man -P cat`, macOS 27.2)**:

- READ `sigaction(2)`: the list of functions that *"are either reentrant or not interruptible by signals and are async-signal safe"*
  holds `_exit()`, `alarm()`, `kill()`, `raise()`, `sigaction()`, `signal()`, `sigprocmask()`, `write()` and the rest of POSIX.1-1990's
  base interfaces; `grep -c -E 'longjmp|swapcontext|setcontext|getcontext|malloc|printf|[^_]exit\(\)'` over those lines (198 to 236 of
  the page) printed **0**; and *"All functions not in the above lists are considered to be unsafe with respect to signals. That is
  to say, the behaviour of such functions when called from a signal handler is undefined."* Its STANDARDS: *"expected to conform to
  IEEE Std 1003.1-1990"*.
- READ `setjmp(3)`: *"The longjmp() routines may not be called after the routine which called the setjmp() routines returns"*; an
  env *"that has already returned"* calls `longjmperror(3)` (*"longjmp botch"*) and aborts; `siglongjmp` and `sigsetjmp` *"conform to
  IEEE Std 1003.1-1988"*, `setjmp` and `longjmp` to *"ISO/IEC 9899:1990"*.
- READ `makecontext(3)`: `swapcontext` *"saves the current thread context in *oucp and makes *ucp the currently active context"*;
  nothing about signals. READ `getcontext(3)`: `setcontext` takes a context *"initialized by ... being passed as an argument to a
  signal handler"*, and then *"execution continues at the point the thread was interrupted by the signal"*: **this platform documents a
  handler leaving by a context**, and does not put `setcontext` or `swapcontext` on the list above.

| program (arms' word) | C11 (R) | POSIX (R) / this Mac (READ) | status |
|---|---|---|---|
| a handler that `longjmp`/`siglongjmp`/`_longjmp`s out (p1, p2, p3, p9, p10, `probe`, `watchdog-a`, the 12 goldens) | §7.14.1.1p5 (R): for a signal not made by `abort`/`raise`, a handler may call only `abort`, `_Exit`, `quick_exit`, `signal`; `longjmp` is none: **undefined** unless the signal is `raise`d; §7.13.2.1p2 (R): defined while the `setjmp` function has not returned | `siglongjmp` is on no list this Mac gives (READ, 0 matches); POSIX gives `sigsetjmp`/`siglongjmp` for leaving handlers and says what it saves (R) | **defined only where the handler runs as the result of `raise`, `abort` or a `kill` to self** (both standards give that exemption, R); `watchdog-a` (a timer) and `probe` (a hardware fault) are **undefined by both (R) and by this Mac's list (READ)**, and every one of these programs works |
| the handler calls the program's function (p6, K/D, the atexit golden) | same: undefined for an asynchronous signal, free for a `raise`d one (R) | only the listed functions (READ); a Heroes function allocates and prints, so **not safe**: the allocator golden `...is-stopped-by-name` measures it (*"a signal handler used this runtime's allocator while the code it interrupted was inside it, which would corrupt memory"*) | **not correct C in general**; line 427's own rule |
| `exit()` in a handler reaching an `atexit` function (C12) | `exit` is not among the three (R): undefined for an asynchronous signal; the golden uses `raise` (`leaving_raise`), so **defined** (R) | `_exit()` is on the list, `exit()` is not (READ) | correct for a `raise`d signal by C11 (R); the callback is *called by the handler* all the same |
| a jump no library function makes: asm on arm64, `__builtin_longjmp` on x86-64 (p4) | not C: an extension | not POSIX | **outside both standards**: correct only by the compiler's own documentation (R, unread) |
| `swapcontext`/`setcontext` out of a handler, never back (p7, `watchdog-b`, `g1_*`, `g2*`, `g3`, `nn`) | C11 has no contexts | POSIX.1-2001 had them, POSIX.1-2008 removed them (R); not on the list (READ); **documented by this Mac** (READ, `getcontext(3)`) | **unspecified by the standards, documented by the platform** |
| contexts switched back later (`sched`) | the same | the same | the same |
| a handler that jumps within itself (p8: `sigsetjmp` and `siglongjmp` both in the handler) | §7.13.2.1p2 (R): defined, the function has not returned | the same | **the jump is correct C**; what is refused is the call that follows, made by a handler that **never left**: line 427's rule, not a jump |
| a jump back into a handler already left (`rr_0`, `rr_1`) | §7.13.2.1p2 (R): **undefined** | *"may not be called after the routine which called the setjmp() routines returns"* (READ) | **undefined**, and the Mac's page says so |
| a `siglongjmp` into a buffer saved on a `makecontext` stack (`as_0`) | silent about stacks (R) | not read | **not known** (unread) |
| a plain `siglongjmp` after a dead `makecontext` stack (`fp3`) | as the first row | as the first row | **as correct as C1 is**: refused by C4r anyway |
| a C++ exception out of a handler (`cxxguard`) | C++: undefined (R) | | **no program exists on Darwin**: libc++abi terminates it (3f) |
| a copied or refilled `jmp_buf` (p3, p10, `gcopy`) | silent: `jmp_buf` is an array type (R) | | **unspecified**, works on both platforms' libraries |

**So "though C is correct" is not what the standards say of one program in this list**: for the first row it holds only where the signal
is `raise`d (the arms' own programs `raise`; `watchdog-a` and `probe` do not, and are undefined by C11 and POSIX (R) and by this Mac's
list (READ)); for the inline jump and the contexts the standards say nothing or less. **The v1 arms' *"though C is correct"* (e1, e1b)
must not be written, and the f arms do not write it; what the sentence can say that is true on both platforms is that the refusal is
the runtime's inability to *follow* the handler, not a verdict on the C.** The wording that follows (section 5).

**The clause the arms should say instead** (mine; tested in `257-reports/ffi-clauses.md`):

> A callback called while a handler of C's is still running aborts, saying the handler was not seen to end. A handler is over where
> it returned or where `longjmp`, `siglongjmp` or `_longjmp` took the thread out of it; it is still running where it called the
> callback itself, jumped within itself, or was left by a switch of context (`swapcontext`, `setcontext`), a jump into a coroutine's
> stack or a jump written in assembly, whatever the platform's C library makes of those.

It keeps *held running* (the class), names `swapcontext` where the arms name a never-switched-back context, **drops `__builtin_longjmp`
(not on arm64, section 5) and *"a C++ exception"* (unreachable on Darwin, 3f), and says nothing of C's correctness.** Its known
counterexamples on C4r are `fp3` (a plain `siglongjmp` refused) and the undefined `rr_*`/`as_1`.

---

# RESUMED at 14:35 (the API's five-hour limit stopped the first run after section 4; sections 0 to 4 above stand, and where a later
# measurement falsifies one of them the correction is written beneath, dated, never in place)

Resumed from this file, `257-briefs/resume.md` and the two warden text files only (`spec-w7.txt`, w26, and `spec-w12.txt`, w27; I did
not read the other seats' reports). Machine at the resume: `uptime` read 40.50 20.44 24.01 at 14:34, **247.81 183.85 132.01 at 14:59** (other
lanes; every duration is indicative, and the sweeps below have **no timeout in them**: `grep -c 'exit=124'` over every log printed 0).
**Two corrections to my own report of the first run, found at the resume** (each by its command):

1. *Sections 1 to 3 say the programs are "in `257-reports/programs/ffi-pragmatist/`: `p1` to `p10` and the panel-255 shapes under
   `shapes/`, the real libraries in the folder itself".* `ls` of that folder at 14:35 printed `watchdog-a watchdog-b watchdog-c`
   **and nothing else**: the copy had not been made when the limit stopped the run. It is made now (`programs/ffi-pragmatist/scripts/copy-out.sh`, run last: sources, scripts and
   text logs only, no binary and no dylib).
2. *Section 3g says libcrypto's imports are in "section 8" and section 8 did not exist.* The search is section 8 below; the first run's
   `search/` folder (1,607 paths, `.a` and `.dylib`, duplicates by symlink) is superseded by `search2/` (422 unique `.dylib`s).

## 4b. The Darwin depth sweep (resume.md's first item; the warden's condition): is the refusal a stack-depth test on Darwin?

**Answer: no, on Darwin the refusal does not look at the callback's depth, at any depth I could make, on today's runtime and on C4r.** The
warden's reading (the abort after an unseen exit is a stack-depth test on Linux, so *"aborts"* became *"may abort"*) is Linux's; the
Darwin code says so before it is measured (`rebind.c` of C4r, `hero_rebind_inside`: *"any entry standing refuses, on whatever stack and
however pinned ... No prune by the thunk's own frame (rule 5)"*; the trampoline's push and the thunk never compare a frame), and the
measurement agrees.

**The program.** `p257/depth/dsweep.h` (one header, ten forms picked by the environment variable `FORM`; the callback is called by the
program `DEPTH` bytes deeper than a reference frame; the header prints on stderr, before the call, how many bytes below the reference the
frame it calls from really lies, and the log quotes **that** number). The reference is the handler's own frame where the handler runs on
the thread's stack, the raising function's frame for the alternate-stack form `g11`, and the coroutine's own frame for `as0` (a coroutine
stack in a live frame lies ABOVE the handler, so "deeper than the handler" cannot be asked there; its axis is how far into the
coroutine's stack). **Plain C, no Heroes (the control, `dsweep_main.c`, `-O0` and `-O2`, `FORM` each of the ten, `DEPTH` 0 and 4096): all
ten forms print `answer ran for 7`, `result 70`**, so every form is correct C that a clang on this Mac runs. Forms:

| form | what C does | the shape it is |
|---|---|---|
| `p1` | the handler `siglongjmp`s out, a static buffer | a jump out (a control: must run) |
| `p5` | the handler returns | a return (a control: must run) |
| `p8` | the handler jumps within itself (`sigsetjmp` and `siglongjmp` both in it) and calls the callback itself | C6 (refused) |
| `p4` | a jump no function of the C library makes (assembly on arm64) | C4 |
| `p7` | `swapcontext` to a context taken before the signal, never resumed | C7, G form 1 (`g1_0`) |
| `g11` | the same, the handler on the alternate stack | `g1_1` |
| `g2` | the handler swaps to a coroutine that `siglongjmp`s to a buffer the runner saved before the signal | G form 2 |
| `g3` | the handler swaps to a coroutine that returns through `uc_link` to a context taken before the signal | G form 3 |
| `fp3` | a plain `siglongjmp` out of a handler whose frame lies below a dead `makecontext` range that holds the target | C8 |
| `as0` | the handler `siglongjmp`s INTO a coroutine (a stack array of a live frame) that calls and returns | C8 |

**How deep the handler lies, measured** (`p257/depth/frame.c`, plain C): the handler's frame lies **1312 bytes below the raising frame**
on this Mac (`./frame` x3 and `-O2`: 1312, 1312, 1312, 1312). The depths asked are `-1024 0 1024 4096 6144 16384 65536 262144` (the
brief's 0, 1024, 4096, 6144, 16384 and three more) and, as an extension, `-2048 -4096 -8192`, which the header clamps at the shallowest
frame the program can call from after the landing: **measured `-1264`, that is the callback called from 1,264 bytes ABOVE the handler's
frame, from about where the raising function's own frame lay before the jump** (the handler's frame lies 1,312 bytes below the raising function's frame in plain C and, with this runtime's trampoline between (`depth/frame2.hero`, built and run: 1,296 / 1,328 today, 1,344 / 1,328 on C4r, 1,344 / 1,328 landed, `-O0` / `-O2`), 1,328 to 1,344; so a callback frame 1,264 above the handler's lies 64 to 80 bytes below the raising function's own frame, the closest a callback can be made to the raise site; `logs/c4r-depth-shallow.log`: `p4 D=-8192 measured=-1264`).
So the sweep runs from **the raise site's own level down to 256 KiB below the handler**, which is the whole range the brief's depths
(0 to 16384 bytes deeper) sit in the middle of.

**The cells.** `p257/depth/go.sh <today|c4r> o0,o2,san` (stage 2 `heroes-next` with `HEROES_RUNTIME=p257/rt/<name>`), `timeout 20`, both
streams through `head -c 100000`, the exit from `PIPESTATUS`, **3 runs a cell, each cell's three runs identical in verdict**; logs
`depth/logs/{today,c4r}-depth.first.log` (720 runs each), `-san2.log` (240 each, below), `-shallow.log` (270 each); 2,460 runs in all
(`grep -E ' x[0-9] ' | sed ... | paste -sd+ | bc`). `-O0`, `-O2` and `--sanitize` are columns of one cell, since **no mode moved a refusal except `fp3` at `-O2`** (below).

| form | Darwin **today**, every depth `-8192 ... 262144`, o0 / o2 / san | Darwin **C4r**, every depth, o0 / o2 / san |
|---|---|---|
| `p1` (control) | runs at all 11 asked depths, all 3 modes | runs at all 11 asked depths, all 3 modes |
| `p5` (control) | runs at all 11 asked depths, all 3 modes | runs at all 11 asked depths, all 3 modes |
| `p4` inline jump | **S at all 11 depths**, all 3 modes | **S at all 11 depths**, all 3 modes |
| `p8` jump within itself | **S at all 11**, all 3 modes | **S at all 11**, all 3 modes |
| `p7`, `g11`, `g2` | **run at all 11**, all 3 modes (today does not refuse them) | **S at all 11**, all 3 modes |
| `g3` | **U at all 11**, all 3 modes (false words: no buffer, no copy) | **S at all 11**, all 3 modes |
| `as0` | run at all 11, all 3 modes | **S at all 11**, all 3 modes |
| `fp3` | run at all 11, all 3 modes | **S at all 11 at `-O0` and `--sanitize`; RUNS at all 11 at `-O2`** |

**So: every refusal that holds, holds at every depth on Darwin, today and C4r, and the one place a refusal moves it is the optimization
level (`fp3`), not the depth.** What this says of the sentence:

- *"a callback called later on its thread from deeper in the stack than the handler was may abort"* (w26 and w27) is **true on Darwin
  and not the Darwin rule**: it says a callback from shallower is safer; on Darwin the callback from the raise site's own level aborts
  (`p4 D=-8192`, measured -1264, S, 3 of 3 in each mode) exactly as one 256 KiB deeper does. The *"may"* is what makes it true; a reader
  who reads the depth clause as the condition would write, for Linux, a callback from a shallow frame and **meet the abort on this Mac**:
  a program that RUNS on one platform and ABORTS on the other for the same C, from a rule that is *"depth"* on one and *"an entry stands"* on the other.
- **An instrument artifact I found and removed, so nobody reads it as a refusal**: the first `--sanitize` sweep printed, for C4r `g2` and `g3`
  at `D=0` only, `AddressSanitizer: CHECK failed: asan_thread.cpp:369` (exit 134, no `panic:` line, three of three). It was my probe's
  own pad store landing on shadow memory the abandoned handler frames left poisoned (ASan prints `ASan is ignoring requested
  __asan_handle_no_return ... False positive error reports may follow` for any `siglongjmp` made off the thread's default stack). `dp_call` is
  now `no_sanitize("address")` and the whole `--sanitize` column re-run (`-san2.log`): S at every depth, no CHECK failure, today runs.
  Both logs are kept (`.first.log` holds the artifact).
- **`fp3` at `-O2` runs on C4r and is refused at `-O0` and `--sanitize`** (`c4r fp3 o2 D=-1024 ... D=262144: exit=0 answer-ran=yes`, 3 of 3 at each of
  11 depths). The refusal of this shape is the runtime's stale-range table meeting the program's frame layout, which the optimization
  level moves: a refusal that depends on `-O` is the nearest thing to a depth test Darwin has, and it is not one (section 7).

## 6. The words (item 6): the first line of stderr of every refusal on Darwin, today, C4r and the landed runtime, and whether *"saying the handler was not seen to end"* describes it

The refusal is **one line** of stderr (it ends with the spec's tail). The four kinds, their first words as the runtime prints them (copied
from `w-*-shapes/last-*-o0.txt`, `grep -m1 '^panic'`), and the programs that print each on this Mac (kind letters of section 1; **SW** is the
landed runtime's new fifth kind, `HERO_SIGNAL_SWITCHED`, `sigguard.c:440` of the landed tree):

| kind | first words (verbatim) | prints it on Darwin | does the phrase *"the handler was not seen to end"* describe it? |
|---|---|---|---|
| **S** | `panic: p4.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running ...` | today: `p4`, `p6`, `p8`, the inline golden, the 2494 atexit golden, the two jumps-within goldens; **C4r and landed: those, and also** `fp3` (`-O0`, `--sanitize`) and `as_0`; **C4r also** `cs_1` and the `a_*` shapes (today U; not run on landed) and every G form (`p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `g3`), `nn`, `sched`, `watchdog-b`, `sc1` (landed: SW) | **yes, word for word** (*"had not been seen to end"*), and **true** of `p4` (the first example, spelled in assembly, is what the program does), `p6`, `p8`, 2494; **false of every program whose refusal is not a jump** (C4r's G forms, `sched`, `watchdog-b`, `fp3`, `as_0`): its two alternatives, *"called it from inside that handler"* and *"left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library)"*, **name none of the program's own act**; the reader of `watchdog-b` looks for an inline jump in a library that has a `swapcontext` and a `siglongjmp` |
| **SW** (landed only) | `panic: p7.answer was called while a signal handler of C's that this runtime saw begin had switched to another context (`swapcontext`, or `setcontext` into a context `makecontext` made) and had not been seen to switch back or to end: either C called it on that other context, or C abandoned the handler by a switch this runtime cannot tell from one that comes back. A handler not seen to end is held to be running ...` | landed: `p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `g3`, `nn`, `sched`, `watchdog-b` (all 3 modes, 3 of 3) | **partly**: *"had not been seen to switch back or to end"* is not the arms' phrase, and the *"A handler not seen to end is held to be running"* that follows is; **true** of every program it prints for, including `sched`, whose first callback IS called on the other context. One finding inside it: **it names `setcontext` into a context `makecontext` made, and `b_0`, `b_1` (`setcontext` to a `getcontext` context) run**; the words say exactly what the runtime refuses and the spec's w26 lists `setcontext` as an END, so the pair is consistent only if a reader keeps *"into a context `makecontext` made"* in mind |
| **U** | `panic: rr0.answer was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps), so whether the jump left the handler is not known: a handler not seen to end is held to be running ...` | today: `p3`, `p10`, `gcopy`, the copied-buffer golden, the refill golden, `g3`, `sched`, the `a_*` shapes; **C4r and landed: `rr_0`, `rr_1`, `as_1` only** | **no**: its head is *"holds no record of"*, and the phrase appears as the consequence at its end. **A finding against R2's promise**: R2 moved the copy and the refill out of U, and U still prints on C4r and on the landed runtime for **three programs, none of which is a copy, a refill or a full table**: `rr_0`, `rr_1` (a jump back into a handler already left) and `as_1` (a coroutine resumes a handler's saved context); the parenthesis of causes is **false of all three** (they are undefined in C, section 4, but the words say they were *copied or refilled*) |
| **K** | `panic: as1.answer was called from inside a signal handler: C called it while handling a signal, where neither this runtime's memory nor the C library's is safe to use.` | today: `as_1`, `cs_1` (C4r: U and S) | **no**, and **true**: the callback really is called inside the handler's extent |
| **D** | `... ran as a signal handler: the system called it inside a signal ...` | no program of this section prints it | not applicable |

**Does *"saying the handler was not seen to end"* describe the refusal? For the S kind and for the landed SW kind, as the sentence's tail, yes; as the
whole of what a reader needs, only for a jump the reader can recognise from the three examples.** What the words do not tell, and a
program author needs: *which act* of the program's C the runtime could not follow. Proposed first words (mine, for the lane that
owns `sigguard.c`, tested nowhere: a suggestion, not a measurement): for `fp3` and `as_0`, *"... C jumped, while the handler ran, to a stack
this runtime knows as a context's (one `makecontext` was given), so whether the jump left the handler is not known"*; for `rr_*` and
`as_1`, *"... C jumped back into a handler it had already left"*. The existing parenthesis, *"(an inline `__builtin_longjmp`, a C++
exception, a jump inside a system library)"*, has **one example that does not compile on arm64** (section 5 of this report, below) and
**one that no program can reach on Darwin** (section 3f's `cxxguard`).

**C4r's S words for the G forms are the lane's reason for SW, and the measurement agrees with it**: `g1_0` today runs, C4r prints S
(*"either C called it from inside that handler, or ... an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library"*), landed prints SW. The
landed SW words are true of all nine programs that print them in the P, A and prog logs (and of `sc1`, the tenth, in the depth logs).

## 5. The inline jump's spelling (item 5): `__builtin_longjmp` on arm64, on this Mac

`p257/inline2/bl.c`: a SIGUSR1 handler that calls `__builtin_longjmp(buf, 1)` to a `__builtin_setjmp(buf)` in `main` (the spelling the arms
and the S words name). `clang --version`: Apple clang 21.0.0 (clang-2100.4.1.1), `uname -m` arm64.

- `clang -O0 -o bl bl.c` **does not compile**: `bl.c:5:37: error: __builtin_longjmp is not supported for the current target` and
  `bl.c:8:9: error: __builtin_setjmp is not supported for the current target`, `2 errors generated`. `-target arm64-apple-macos
  -fsyntax-only` prints the same two errors.
- `-target x86_64-apple-macos -fsyntax-only` prints nothing and the object links (`file bl_x86`: *Mach-O 64-bit executable x86_64*); it **cannot run
  here**: `arch -x86_64 ./bl_x86` prints `Bad CPU type in executable` (no Rosetta on this Mac: `pkgutil --pkg-info
  com.apple.pkg.RosettaUpdateAuto` found no receipt). So the x86-64 spelling is compiled and **unrun**. The two Linux triples ask for glibc's
  headers (`'signal.h' file not found`): unrun here (the golden's own header says Linux arm64 refuses it too: `sed -n 1,16p
  tests/golden/run/fixedbugs-2522-an-inline-jump-out-of-a-handler.h`, read in the first run).
- **What a reader who reads *"an inline `__builtin_longjmp`"* in the spec writes on arm64**: a program that does not build. The class the arms
  name has no member on the architecture this Mac and the `ubuntu-26.04-arm` leg share, in the spelling the arms give; the golden writes the same
  jump in assembly (`p4.h`, `stp x19..x30`, `ldp`, `mov sp, x9`, `ret`, 70 lines, `shapes/p4.h`), and a real program that makes such a jump on arm64 does
  it the same way (hand-written assembly or a library's own switch: recalled, unread: Boost.Context's `jump_fcontext` and libco are
  written that way; **CORRECTED at 15:00: Boost.Context IS on this Mac** (`ls /opt/homebrew/opt/boost/lib | grep -i -E 'context|coroutine|fiber'` printed `libboost_context`, `libboost_coroutine`, `libboost_fiber`, each `.a` and `.dylib`; I had written here that neither library was, from a zsh `ls` whose one glob matched nothing and aborted the whole command; it is bound in section 8 as `bctx_plain` and `bctx_signal`; libco: the `*libco*` glob matched nothing, and that is all I ran for it).
- **The class's wording that is true on both** (the golden's own, and mine): *"a jump no function of the C library makes"* (the golden:
  *"a jump no function of the C library makes, so this runtime sees no event for it"*), which is true of the x86-64 `__builtin_longjmp`, of the arm64
  assembly and of any hand-written switch, and **names no spelling that fails to compile somewhere**. Mine, if the sentence wants one example: *"a
  jump written in assembly, or by the compiler's own `__builtin_longjmp` where it has one"*. The warden's *"by an inline jump"* (w26, w27) is the shortest
  true form and **needs the word *inline* defined for a reader who has never met it**: in this report's words, *inline* means *no call of a C
  library function*, which is what the golden's phrase says.

## 7. C8 (item 7): `fp3` and `as_0`, which real C program is that class, and are the existing words true of it

Section 1 has both: **today runs both** (`fp3`, `as_0`: exit 0, `answer ran for ...`, 3 of 3 at `-O0` and `--sanitize`), **C4r refuses both in S words**, and the
landed runtime prints S for both (landed table, section 12). The depth sweep adds: `fp3` is refused at every depth at `-O0` and `--sanitize` and **runs at every
depth at `-O2`** on C4r (4b), so the refusal is not depth and not stable across builds of the same C.

**`fp3` in one line**: *a program that once ran a coroutine on a stack it took from a frame of its own thread's stack, and later, in the frames that now
occupy that memory, recovers a fault by a plain `siglongjmp` out of a handler.* (`r2new.h` `ft_make` hands `makecontext` a 256 KiB local array of
a function that then returns; `ft_inside`'s frame, whose stack pointer a `sigsetjmp` saves, lies in the same memory; a handler 320 KiB below it
`siglongjmp`s back; the program is called.) A real program of that class is one that mixes a `makecontext` task started from a helper with a
`sigsetjmp`-based fault recovery (Boost.Test's execution monitor or a C exception macro) on the thread that ran the task: rare, and **the
program is correct C by every reading of this report's section 4** (a `siglongjmp` out of a handler to a live `sigsetjmp`).
**The reason it is refused, measured** (`p257/depth/fp3_layout.c`, plain C, the two addresses printed; the runtime's rule is rebind.c's 4 and 7:
*"A jump's target lies on a coroutine's stack where a range `makecontext` handed out holds it ... nothing tells this file one has ended"*): at `-O0`,
`ft_inside`'s stack pointer, the one word the decode reads, lies **196,560 bytes into the 262,144-byte range** (`clang -O0 ...; ./fp3_layout-O0`: *"lies INSIDE the range makecontext was
given (sp - lo = 196560, hi - sp = 65584)"*); at `-O2` it lies **at `hi`** (`sp - lo = 262144, hi - sp = 0`: *OUTSIDE*). **One byte of frame layout is the difference between the
run and the abort.** No spec sentence can say that; the nearest true clause is *"a jump to a place on a stack that was once given to `makecontext`"*.

**`as_0` in one line**: *a signal-driven scheduler whose handler hands the thread to a task by `siglongjmp` into that task's saved point, the task's stack a local
array, and the task runs the callback and returns to the scheduler through its context's `uc_link`* (the setjmp/longjmp-plus-`SIGALRM` preemption of
user-level threads, the idiom before `ucontext`). The handler is **left for good** (a `siglongjmp`), the callback is called on the coroutine's own stack, far
from any frame of the signal's, (`sigsetjmp(as_kb, 0)` does not save the mask, so SIGUSR1 stays blocked after the jump; whether that matters to Heroes code is unmeasured): **correct C, refused, a known cost** (the 2522 ruling B's own
phrase in `sigguard.c`: *"the refusal stands, a known cost of a correct program"*).

**Are the existing S words true of them? No, and they point away from the cause.** `fp3` and `as_0` print (C4r, landed, `-O0` and `--sanitize`, 3 of 3 each):
*"... was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this
runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library)"*. (1) *"C called it from inside that handler"*: **false of both**: no frame of the handler lies
above the callback (`as_0`'s runs on the coroutine's stack; `fp3`'s after the jump); (2) *"C left the handler in a way this runtime does not follow"*: **literally true**
(the runtime did follow the jump, `hero_signal_jump_route()` answers 3 here, and could not judge it: *"No two addresses on two different ranges are ever compared"*, rebind.c rule 4), **and its
three examples are not the program's**: a reader of `fp3` is told to look for an inline jump, a C++ exception or a jump inside a system library, and finds a `siglongjmp` of the
C library. The reader's conclusion, *"my jump is one this runtime does not follow"*, is true, and it is not the thing the words name.

**Can the class be said in a spec sentence?** Yes, in one clause, *"a jump onto a stack given, now or once, to `makecontext`"* (w26 says *"a jump onto another stack given to
`makecontext`"*, which covers `as_0` and **not `fp3`**: `fp3`'s target is on the thread's own stack, in memory the runtime still lists). **Leaving it out costs**: a correct program refused at `-O0` and run at
`-O2` with words that point at three causes it does not have, and no workaround a reader can derive from the sentence (a heap buffer, or a frame layout). The cost is small in frequency
(it needs a `makecontext` task started from a returned frame *and* a recovery handler on the same thread) and large per case (nothing in the message or the spec leads to it), which is why the
warden's w26 should keep *"given to `makecontext`"* and add *"or once"*, or say *"may abort"* for it, as the arms already do for the depth.

## 8. Bindability (item 8, design.md §1.12, §4.19): is there a C library a Heroes program binds today that STOPS working under C4r because of a refusal the sentence names?

**Answer: none that I found, among 422 shared libraries searched, eight third-party libraries bound (ten bindings) and the repository's own packages searched; the only programs that go from RUN to REFUSED
between today and C4r (and the landed runtime) are the context-switching designs I wrote, `watchdog-b` and the G forms; the two real libraries whose design is refused (`sched`-style green threads, Boost.Context resumed from a handler) were already refused today.**

**What I searched** (`p257/search2/search.sh`, `nm -u` of every `.dylib`; `counts.txt`: *libs 422 both 13 installer-only 20 jump-only 11 context 2*): every `*.dylib` under
`/opt/homebrew/opt/*/lib` (maxdepth 2, symlinks resolved, duplicates by real path removed: **422**). A library *installs a handler* if it imports `_sigaction`, `_signal`, `_sigset`, `_bsd_signal` or
`_sigvec`; it *jumps* if it imports `_longjmp`, `__longjmp`, `_siglongjmp`, `_setcontext`, `_swapcontext` or `_makecontext`. **13 import both** (`both.txt`): Boost.Test's `libboost_unit_test_framework` and
`libboost_prg_exec_monitor`, `libexpect`, Ghostscript's `libgs`, `libgpcl6`, `libgpdl`, `libgxps`, `libLLVM`, `libmpg123`, OpenSSL's `libcrypto` 3 and 4, GNU readline, `libruby`. **11 jump only** (libpng, cairo and
its script interpreter, freetype, harfbuzz-raster, jpeg-turbo, leptonica, libtiff, libvpx, poppler, gettext's `libgettextsrc`: error paths that `longjmp` with no handler). **20 install only** (libuv, glib and gio, ncurses,
SDL3, nspr, Tcl and Tk, `libnode`, dbus, abseil's failure-signal handler, libssh, gpgme, libassuan, libslirp, npth, ...). **2 import the context functions**: both are OpenSSL's `libcrypto` (its ASYNC fibers),
and **no library of the 422 imports a context function and installs a handler**. (The first run's `search/` folder counted `.a` archives and symlinks, 1,607 paths: superseded, not deleted.) **`libboost_context.dylib`,
`libboost_coroutine.dylib` and `libboost_fiber.dylib` are on this Mac and import none of the listed names** (their switch is assembly); **I wrote in section 5 that no such library is here, and
that was wrong: `ls /opt/homebrew/opt/boost/lib | grep -i -E 'context|coroutine|fiber'` printed `libboost_context`, `libboost_coroutine`, `libboost_fiber`, each `.a` and `.dylib`.** The repository's own bindings:
`grep -rlE 'swapcontext|makecontext|setcontext|longjmp|setjmp|sigaltstack' packages examples examples-rebel runtime/platform runtime/vendor runtime/*.h` over the landed tree printed **one file, `packages/rebel/0.1.0/files/big.ledger`,
a ledger and no code**; `sigaction|signal(` printed `process.hero`, `process/hero_proc.hero` and the runtime's own `hero_term.c`, `hero_net.c`, `hero_proc.c` (handlers that set a flag and return).

**What I bound**, each an `extern` group over the library's real header (clang checks it), a Heroes callback in the library's path, **3 runs in each of `-O0`, `-O2`, `--sanitize` on each of today, C4r and the landed runtime**,
a plain-C driver as control where it is mine (`p257/prog/`, logs `logs/{today,c4r,landed}-prog*.log`):

| library (version) | what the Heroes program does | the signal or jump in the library | today | C4r | landed |
|---|---|---|---|---|---|
| **libpng** 1.6.59 | decode a good image, a truncated one, a good one; an error callback | `png_error` `longjmp`s, no handler | runs (`--sanitize`: heap-use-after-free in the runtime, defect 2679) | runs | runs |
| **libuv** 1.53.0 | `uv_signal_start`, `raise(SIGUSR1)`, `uv_run`, callback | its handler writes a byte and returns | runs | runs | runs |
| **SQLite** 3.54.0 | `sqlite3_progress_handler` callback, a SIGALRM handler calls `sqlite3_interrupt` | the signal lands while Heroes code runs; handler returns | runs | runs | runs |
| **OpenSSL** libcrypto 4.1.0 | `EVP_Digest` SHA-256 with a callback | CPU probe `sigaction` + `siglongjmp` (imports) | runs | runs | runs |
| **OpenSSL ASYNC fibers** (`asyncjob`, new) | `ASYNC_start_job` runs a job on a `makecontext` fiber; **the job calls a Heroes callback on the fiber's stack**, pauses, resumes, calls it again | `swapcontext`, `makecontext`, no handler: the one real library that imports the context functions | runs, `result 230` | runs, `result 230` | runs, `result 230` |
| **GNU readline** 8.3 | callback mode, one line | its own handlers (SIGINT, SIGWINCH) return | runs | runs | runs |
| **Boost.Test execution monitor** 1.92 (`bmon`, new) | `execute()` runs a function that raises SIGFPE; the monitor's handler `siglongjmp`s out, `execute` throws, the shim catches and **calls the Heroes `report`** | a real library's signal handler leaving by `siglongjmp`, then a callback | **runs** (`-O0`, `-O2`: `report ran for 210`, `result 2100`) | runs | runs |
| (same, SIGSEGV / SIGBUS / SIGTRAP / SIGABRT, and any fault under `--sanitize`) | | Boost **declines to install over a handler already set** (`execution_monitor.ipp` lines 713-716 read: `if( m_new_action.sa_sigaction \|\| m_new_action.sa_handler ) { m_installed = false; return; }`); the runtime's own SEGV/BUS handler (`stack.c`), ASan's, hold them | the runtime's panel (*"a null pointer was read through, at offset 0x10, called from bmon.main"*), exit 134 | the same | the same |
| **Ghostscript** 10.08 libgs (`gsrun`, new) | `gsapi_init_with_args`, `gsapi_run_string`, a stdout hook **forwards each chunk to the Heroes callback from inside the interpreter** | imports `signal`, `setjmp`, `longjmp` | runs, `result 23` | runs | runs |
| **Boost.Context** 1.92 fibers (`bctx_plain`, new) | a fiber calls the Heroes callback, switches back, is resumed, calls it again | assembly `jump_fcontext`, no handler | runs, `result 30` | runs | runs |
| **Boost.Context from a handler** (`bctx_signal`, new) | a SIGALRM handler (`setitimer`, 20 ms) resumes a worker fiber, which calls the Heroes callback and switches back; the handler returns | **a switch made in a handler by a library's own assembly**, switched back | **S, exit 134** | **S** | **S** |
| the three watchdogs, `probe`, `lazy`, `sched` | section 3 | section 3 | A, C run; **B runs**; `sched` U | A, C run; **B S**; `sched` S | A, C run; **B SW**; `sched` SW |

**What this says.** (1) *No shipped library of the eight stops working under C4r or the landed runtime*: every row that runs today runs on both, except `watchdog-b`, which is a design of mine. (2) The one real
library whose pattern the sentence refuses, **Boost.Context resumed from a signal handler**, is **refused today as well** (S, 3 of 3 in each mode on today, C4r and landed): the sentence documents a refusal that already stood; it does
not create one. Its S words (*"either C called it from inside that handler ..."*) are true of it: the fiber runs inside the handler's extent. **A reader of f1's *"a switch of context never switched back"* would
not predict this refusal**, since the fiber switches back and the handler returns: the refusal is about a callback called **while the handler is suspended**, not about a switch never returned from (`sched` is the same finding, section 3d).
(3) **The sentence is silent on a class it will not be asked about on this Mac and that is not empty elsewhere**: a library that leaves a handler by a **hand-written assembly switch** (Boost.Context, libco, a green-thread
package's own). `w26`'s *"an inline jump"* covers it **only if the reader reads *inline* as *no call of a C library function***, which neither the spec nor the golden's header states for someone who has not seen the golden.
(4) A finding beside the sentence, **identical on all three runtimes, not touched by R1 to R4**: **Boost.Test's execution monitor cannot convert SIGSEGV, SIGBUS, SIGTRAP or SIGABRT into an exception inside a Heroes program**, because the runtime's own
handlers hold those signals and Boost's constructor leaves an installed handler alone; it works for SIGFPE (measured, `-O0` and `-O2`, 3 of 3 on each runtime) and, unmeasured, for any other signal the runtime and the sanitizer do not hold. A Heroes program gets the runtime's null-read panel where a
C++ program gets an exception it can catch: §1.12-safe (no segfault) and a binding limit that a sentence on *jumps out of handlers* does not and cannot reach. **Unrun** (named, not bound): `libexpect`, `libgpcl6`, `libgpdl`, `libgxps`, `libLLVM` (its `CrashRecoveryContext` is a signal-to-`longjmp` library), `libmpg123`,
`libruby`, OpenSSL 3's `libcrypto` (the same code as 4), the 11 jump-only libraries other than libpng, and every library not under Homebrew's `opt/*/lib`.

## 10. C12 (item 10): `exit()` inside a handler reaching an `atexit` function, the 2494 golden's words today, under C4r and on the landed runtime

The golden `fixedbugs-2494-a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` (read in full: `tests/golden/run/`, `.hero`, `.h`, `.expected`): the program
registers a Heroes function with `atexit` (the spec's own example of a callback, line 427), installs a C handler for SIGUSR1 whose body is `exit(0)`, raises SIGUSR1, and the Heroes `at_end`
runs **inside the signal**, called by `exit`. Its `.expected` first line (today and C4r, byte for byte equal; `logs/gate-*-fixedbugs-2494.log` read *15 passed, 0 failed* on each):
*"panic: fixedbugs2494aheroesatexitfunctionachandlerreachesthroughexitisrefused.at_end was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that
handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running ..."* The shape
(`p257/shapes/fixedbugs-2494-...`, built and run by me, o0 and san, 3 runs each): **today S, C4r S, exit 134, 3 of 3 each** (section 1's C12 row).

- **Are the words true?** Yes, and this is the one S refusal the words fit exactly: the handler never left, and **C called the function from inside that handler** (through `exit`). It is the S kind's own case, and line 427's
  *"nor called by one"*: the callback is *called by* a signal handler.
- **Does the sentence's arm text describe it?** w26/w27/f1/f2/f3 each speak of a handler *left* (by return, jump or switch) and of a callback *called later*. **None of them describes `exit` in a handler**: the
  handler is not left, the callback is called *during* it. Line 427 already carries it (*"nor called by one"*) and the arms' *"a callback called while one is held running aborts"* (f1, f2) covers it **only if *held
  running* is read as including *running*, the one word f1 uses for both**; f3's *"called while a handler of C's has not been seen to end, by return or by `longjmp`... out of it"* covers it (the handler has not ended by any of them).
  w26 and w27 do not say it, and do not need to: line 427 does.
- **What C and POSIX say** (section 4; recalled, never quoted): `exit` is not one of the functions C11 allows a handler to call for an asynchronous signal (undefined), but **the golden `raise`s the signal, so for C11 the call is defined**
  (the exemption for `raise`/`abort`), and POSIX's list has `_exit` and not `exit` (this Mac's `sigaction(2)`, READ). **The refusal is of a program that is correct C by C11 and is refused**; the words
  do not call it wrong (*"either C called it from inside that handler ..."*), which is the right register.
- **The landed runtime**: the golden's program built and run by me against it prints the same S words, 3 of 3 in `-O0` and `--sanitize` (section 12's goldens table), and **the landed tree's own gate, `run fixedbugs-2494`, read 15 passed, 0 failed**, as today's and C4r's did.

## 11. Unrun (item 11): what I did not run, and which rows rest on it

- **The CI's `macos-26`, Darwin x86-64, Windows**: no probe in the runtime (shared brief § 4); I have none of them. Every Darwin cell in this report is **this Mac only**: Darwin arm64, Apple clang 21.0.0, macOS 27.2.1 (`sw_vers -productVersion`),
  libSystem as shipped here. **The decode's calibration (route C) is run before `main` on each machine** (`hero_signal_jump_route()` answers 3 here, `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`: *route 3* on C4r and the landed runtime, 3 of 3); on a
  platform where it does not verify the route falls to A (the accessor golden's own comment: *"Darwin x86-64"*), **which I simulated on this Mac** with `HERO_JMP_FORCE_UNCALIBRATED 3` on the landed runtime
  (`p257/rt/landed-A`, accessor reads **route 0**, section 12's route-A table): that stands for *what route A decides*, **not for what Darwin x86-64 does**, which no machine here can say (`arch -x86_64` fails: *Bad CPU type in executable*, no Rosetta).
- **Rows resting on the unrun**: every row of sections 1, 2, 4b, 8 and 12 rests on this Mac alone; **the x86-64 `__builtin_longjmp` spelling** is compiled (`-target x86_64-apple-macos -fsyntax-only`) and never run; **the CI's Linux legs** are the engineer's;
  **Windows** has no handler probe, so on it *no* callback is refused for being called by a handler and the sentence is knowingly Windows-false unless scoped (the shared brief's B12).
- **Unrun by choice or by limit**: the compiler's own tests (`byte for byte (defect 2533)` is red at the frozen tree: section 0); `libexpect`, `libLLVM`, `libmpg123`, `libruby`, the other Ghostscript front ends (section 8); a Linux container (not my platform);
  **no paid run** (the llm-ergonomist's blind reading is unrun, and the resolution is provisional on it; I think a paid run would **not** change my verdict, which rests on measured C, not on how a reader paraphrases).

## 9. Each arm's clauses (item 9): where they are, and the repaired clause

**The full table is `ffi-clauses.md`** (this folder: every atom of w26, w27, f1, f1b, f2, f3 and the v1 arms e1, e1b, e2, e3, each marked true on Darwin with its cell, false with its falsifying program, or untested, and the repaired clause with the run).
The result in four lines:

1. **w26 and w27's *"`setcontext` out of it"* is false for one program**: `sc1` (`depth/dsweep.h`: a one-way `setcontext` onto a `makecontext` stack) is refused on today (U), C4r (S) and the landed runtime (SW), at every depth, 3 of 3 in `-O0`, `-O2`, `--sanitize` (the 262144 depth overflows my own coroutine stack under ASan and is not a refusal); `b_0` and `b_1` (a `setcontext` to a `getcontext` context) run on all three. The landed words already say it (*"or `setcontext` into a context `makecontext` made"*).
2. **f1, f2 and e1's *"a switch of context never switched back"* is false for two real programs**: `sched` (SIGALRM, `swapcontext` to the next green thread, every thread switched back to, the three sums right in plain C) and `bctx_signal` (Boost.Context 1.92: a handler resumes a fiber that switches back); both are refused on all three builds. The refusal is of *a callback called while the handler is suspended*; `watchdog-b` is the only *never switched back* case.
3. **w26 and w27's *"from deeper in the stack than the handler was"* is not Darwin's rule**: refused at every depth from the raise site's own level (1,264 bytes above the handler's frame) to 256 KiB below it (report 4b).
4. **A `longjmp`-family exit ends the handler "whatever memory" only where the runtime verified the C library's buffer layout at start**: on this Mac the accessor reads 3 and all of it holds; under `HERO_JMP_FORCE_UNCALIBRATED 3` (route A, what an unverified platform runs) **twelve correct programs** that run on route C are refused in U words (`p2`, `p3`, `p10`, `gcopy`, `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib`, and **`b_0`, `b_1`, the `setcontext` out of a handler that w26 and w27 list as an END**; and `rr_2`, which is not correct C); and `fp3` is refused at `-O0` on route C (section 7).

**The repaired clause** (`ffi-clauses.md` § 3; run against section 1 there): *"A handler of C's is over where it returns, or where `longjmp`, `siglongjmp`, `_longjmp` or a `setcontext` to a context `getcontext` took takes the thread out of it, unless the jump lands on another stack that was, now or once, given to `makecontext`; left any other way (by a jump no function of the C library makes, such as one written in assembly, or by a `swapcontext` away from it), by such a landing, or jumped within, it is held to be running, and a callback called later on its thread may abort, saying the handler was not seen to end or to switch back."*
It supersedes the clause at the end of section 4 (**corrected here: that clause listed `setcontext` among the switches that hold a handler running, and `b_0`, `b_1` run on all three builds; it also named *"a jump written in assembly"* without the platform note of section 5**).

## 3h. (resumed, beside 3e) libpng's own error path abandoning a Heroes frame: the real-library version of `abandon`, on all three runtimes

Section 3e found, with a C function of mine (`abandon`) and a watchdog, that a C `longjmp` over the frame of a Heroes callback that holds a `str` ends the program, after the callbacks have run, with
`panic: N heap blocks still live at exit (a missing decref) - this is a compiler bug`, and wrote *"a libpng binding whose read function reports an I/O error with `png_error` would be this shape (unrun: I did not write that binding)"*.
**I wrote it** (`p257/prog/pngread.h`, `pngread.hero`, `pngread_main.c`): libpng 1.6.59 reads `good.png` through `png_set_read_fn`; the shim's C read function calls the program's Heroes callback with a call count; **the callback, on its 3rd call, calls
`pr_fail()` (= `png_error`, libpng's own `longjmp`) while its `str` `word` is live**, so the jump abandons the callback's frame, its thunk and the shim's frame. The `setjmp` is the binding's C. **Plain C, no Heroes (the control):
the same with a C callback: `failing -3`, `fine 180180`, exit 0.** Heroes, `pngread`, today, C4r and landed, `-O0`, `-O2`, `--sanitize`, 3 runs each (**27 of 27 identical**):

```
libpng error: the program's read function failed
failing -3
fine 180180
panic: 2 heap blocks still live at exit (a missing decref) - this is a compiler bug        (exit 134)
```

**The callbacks ran, the jump did what C says, both decodes returned the right numbers, and the process then aborted at exit with a message that blames the compiler, on every runtime** (`logs/{today,c4r,landed}-prog-pngread.log`). Nothing is corrupted (`grep -c AddressSanitizer` over the last `--sanitize` output of each runtime, `w-{today,c4r,landed}-prog/last-pngread-san.txt`, printed 0, 0, 0), so it is not a §1.12 memory failure; it is
**a correct C program whose C is not "refused" and not "run": it ends in a false abort**. It is **not a jump out of a signal handler**, and it is the libpng error path the brief names (item 3a) as the control: **the control is not clean**. It is the one place where the sentence's implied promise (*after a `longjmp`, callbacks run*) is true and a user
who followed it still meets an abort. **No document says what happens to a Heroes frame a C jump abandons**: `grep -n "still live at exit" docs/design.md spec/heroes-spec.md` printed nothing (section 3e), and I filed nothing (no GitHub write). **A sentence on jumps out of handlers should either say it in one clause (*"a jump
that abandons a function of the program leaves what its frame held unreleased, and the program then ends at exit saying a block is missing a decref"*) or leave it to the issue it belongs to; leaving it out costs a libpng binding's error path.**

## 12. Against the landed runtime (523d35537): the decisive rows beside the panel-255-routes cells

**The build.** `ffi-pragmatist-landed/` (a detached worktree at `523d35537b68e155841ddc68266fc5148c379b54`, `git status --short` empty): `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (the seed is the frozen tree's file, `git diff --stat 828567a30 rebel-round -- seed` printed nothing), `heroes --version` printed
`heroes 0.6.0 / sources 05186f2b38e77a97`, the key of the frozen seed. Programs are built **by that seed compiler** with `HEROES_RUNTIME=p257/rt/landed` (a `cp -R` of the landed `runtime/`: `diff -rq` printed nothing), while the today and C4r columns are built by the frozen tree's **stage 2**
(sections 0 and method): *a compiler other than the cells' own*, which no cell of the table below shows to matter (every `p1` to `p10` verdict equals C4r's). `p257/golanded.sh`, `landed-all.sh`; **plain (`-O0`), `-O2` where the P set had it, and `--sanitize`, 3 runs a cell**, `logs/landed-*.log`.
**The accessor**: `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump` built and run by me against the landed runtime: **`route 3`**, 3 of 3 in `-O0` and `--sanitize` (C4r: `route 3`, 3 of 3; the lane's golden `.expected` says `route 3`); with `HERO_JMP_FORCE_UNCALIBRATED 3` prepended to `runtime.c` (`rt/landed-A`, `make-rt.sh`): **`route 0`**, 3 of 3 each.

| program (first column: what it is) | Darwin **today** | Darwin **C4r** | Darwin **landed** |
|---|---|---|---|
| `p1`, `p2`, `p9`, `p5` | run | run | run |
| `p3`, `p10`, `gcopy` (a copy of a save; a buffer the handler refills and leaves by) | **U** | run | run |
| `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib` | run | run | run |
| `heap`, `handler_heap`, `plain_heap` / `mmap`, `handler_mmap`, `plain_mmap` | run (`--sanitize`: ASan heap-use-after-free) / **`-O0` exit 139, `--sanitize` ASan SEGV** | run | run |
| `pngdemo`, `pngcb` (libpng) | run (`--sanitize`: heap-use-after-free) | run | run |
| `p4` inline jump | S | S | S |
| `p6`, `p8` | S | S | S |
| `p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `nn`, `watchdog-b` | run | **S** | **SW** |
| `g3`, `sched` | **U** | **S** | **SW** |
| `sc1` (one-way `setcontext` onto a `makecontext` stack) | **U** | **S** | **SW** |
| `b_0`, `b_1` (`setcontext` to a `getcontext` context) | run | run | run |
| `fp3` | run | **S** (`-O2`: run) | **S** (`-O2`: run) |
| `as_0` | run | S | S |
| `as_1` | K | U | U |
| `rr_0`, `rr_1` | 134 + ran | U | U |
| `rr_2` | 134 + ran | run | run |
| `cxxguard` (C5) | dies in libc++abi, then the runtime's SIGABRT panel | the same | the same |
| `watchdog-a`, `-c`, `probe`, `lazy`, `uvsig`, `sqlint`, `ossl`, `rl`, `asyncjob`, `gsrun`, `bctx_plain`, `bmon` (SIGFPE) | run | run | run |
| `bctx_signal` | S | S | S |
| `abandon`, `watchdog_h2`, `pngread` | run, then abort at exit (*a missing decref ... compiler bug*) | the same | the same |

**A row that differs between C4r and landed is a finding: there is none in what runs and what is refused, and one in the words**: every cell of panel 255's C4r equals the landed runtime's except that **the ten programs C4r refuses in the S words for a context switched inside a handler print the new SW words** (`p7`, `g1_0`, `g1_1`, `g2`, `g2_1`, `g3`, `nn`, `sched`, `watchdog-b`,
`sc1`: ten). The lane's route is C4r's design, split over `rebind.c` and six files, and **the depth sweep agrees**: `p257/depth` against the landed runtime, **810 runs, 0 timeouts, 3 of 3 in every cell**, equal to C4r's cells at every depth from -8192 (clamped to -1264) to 262144 in `-O0`, `-O2` and `--sanitize`, with SW where C4r has S (`p7`, `g11`, `g2`, `g3`) and **`fp3` running at `-O2` at every depth** (S at `-O0`, `--sanitize`).
**Route A forced** (`golandedA.sh`, 3 of 3, `-O0` and `--sanitize`), the cells that move from route C: **`p2`, `p3`, `p10`, `gcopy`, `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib`, `b_0`, `b_1` run on route C and print U on route A; `rr_2` U; `g1_0`, `g2`, `g3` print U instead of SW; `fp3` and `as_0` RUN**; `p1`, `p4`, `p5`, `p7` (SW), `p8`, `p9`, `rr_0`, `rr_1`, libpng, `probe`, `lazy`, `watchdog-a`, `-c` do not move.

**The stale constant.** `grep -c mailbox` printed **1** for spec line 428 (`sed -n 428p spec/heroes-spec.md`) and **0** for `runtime/parts/sigguard.c` of the landed tree: **the landed `HERO_SIGNAL_REMEDY` still ends *"... and a waiting call of that layer answer stopped; `hero_signal_watch ..."`**, without *"or a mailbox's `receive` that finds it empty"*, in every refusal above (copied from the first line of `p7`'s output); the compiler's test *byte for byte (defect 2533)* therefore stays red on the landed tree (not run by me).

**Each clause of w26 and w27 against the landed runtime on this Mac**: *true* — return ends (`p5`), a `longjmp`-family exit ends it (`p1`, `p2`, `p3`, `p9`, `p10`, `gcopy`, the heap, TLS and late-library rows; **false for `fp3` at `-O0`**), an inline jump holds it (`p4`), a `swapcontext` away holds it (SW), a jump onto another stack given to `makecontext` holds it (`as_0`, `sc1`), *"may abort"* (every refusal is at every depth); *false* — **`setcontext` out of it ends it is false for `sc1`** and true for `b_0`, `b_1`; **the depth condition is not the rule**; **the words** (*"saying the handler was not seen to end"*) are the S words only (section 6). **Under route A** (the platform the accessor golden names as *Darwin x86-64*): *"a `longjmp`-family exit ends it, any memory"* and *"`setcontext` out of it ends it"* are false of twelve programs (section 9, point 4) and *"a jump onto another stack given to `makecontext` holds it"* is false of `as_0`.

---

# 13. The verdict (the structure of `.claude/agents/ffi-pragmatist.md`), and one line per question of the brief

- **verdict**: **object** (no veto). Nothing here breaks the C ABI, no `extern` signature stops being checked against its header (`watchdog_a_bad1.hero`, `_bad2.hero` still fail to build: *clang read the header*), and **no shipped library of eight stops working under C4r or the landed runtime**; what is refused was refused, or was run by luck, before. What I object to is the sentence's *promises*, four of which a program I ran falsifies, and the words *"though C is correct"*.
- **section**: design.md §1.12 (*Robustness, and a boundary that is complete and defended*, line 642 of the frozen copy, by `grep -n '^### 1.12'`), §1.11 (*the founding constraint*, line 484) and §4.19 (*FFI*, line 2654): the sentence must be true of what C does at the boundary, and the documents say nothing of a jump that abandons a Heroes frame (`grep -n "still live at exit"` over design.md and the spec printed nothing).
- **experiment**: the depth sweep (`depth/dsweep.h`, ten forms, plain C control first, 2,460 runs on today and C4r + 810 on landed, 0 timeouts); the table of section 1 on today, C4r and landed (3 runs, `-O0`/`-O2`/`--sanitize`); eight real C libraries bound against their real headers and run (libpng 1.6.59 twice, libuv 1.53.0, SQLite 3.54.0, OpenSSL libcrypto 4.1.0 twice, GNU readline 8.3, Boost.Test 1.92's execution monitor, Ghostscript 10.08, Boost.Context 1.92 twice), three watchdog libraries driven by `setitimer`, a SIGSEGV/SIGBUS recovery library, a preemptive green-thread scheduler; a search of 422 `.dylib`s; the landed runtime with route A forced. **clang accepted every header; every wrong signature I wrote was a build error.**
- **argument** (the strongest reason the sentence is wrong, 103 words by `wc -w`): *"though C is correct"* is not what C11 and POSIX say of an asynchronous handler's jumps (undefined unless the signal is `raise`d; recalled), so no arm may write it. Four clauses are false of Darwin by a program: **`setcontext` out ends it** (`sc1` refused, `b_0` runs), **a switch *never* switched back** (`sched` and a Boost.Context handler switch back and abort), **from deeper than the handler** (refused from the raise site's own level), **whatever memory** (route A refuses twelve programs; `fp3` aborts at `-O0` and runs at `-O2`). And libpng's own `png_error` over a Heroes frame ends in *"a missing decref ... compiler bug"*.
- **prediction**: **the `sqlite3_progress_handler` binding of §4.19's SQLite fence needs no shim under the landed runtime** (`sqlint`: a SIGALRM handler calls `sqlite3_interrupt` while Heroes code runs, `interrupted 1`, 3 of 3 in `-O0`, `-O2`, `--sanitize`, on today, C4r and landed); **libpng's decode and its error path need no clause** from the sentence (3 runs each, `good 180180`, `truncated -3`); and **a Boost.Context fiber resumed from a SIGALRM handler aborts with the S words on all three builds**, so no wording of the sentence changes what that binding can do. A falsifier of the first two: any run of `sqlint` or `pngdemo` that is not exit 0.
- **condition**: the verdict becomes **approve** if the sentence (a) drops *"though C is correct"* and the depth condition, (b) says `setcontext` by its target and *"now or once"* for `makecontext` stacks, (c) names a jump no C library function makes without `__builtin_longjmp`, and (d) scopes *"whatever memory"* to platforms where the accessor reads 3. It becomes a **veto** if a shipped library that runs under today's runtime aborts under the landed one because of a refusal the sentence names (none found among 422), or if `macos-26`'s calibration does not verify (route A on the CI's Darwin leg: then `p2`-shaped bindings with a heap `jmp_buf` in a handler abort there in U words, which would make a bindable class unbindable on a CI platform).

## One line per question of the brief: the answer, and the one measurement that would change it

0. **Did the adopted Darwin C4r build build, and what did `run fixedbugs-2522` read?** Yes, `wrote heroes-c4r`; **23 passed, 2 failed, exactly the two goldens R2 moves** (today 25 and 0; **the landed tree, with its own moved `.expected` files, 25 and 0**). *Changes if:* 25 and 0 on the C4r build over the frozen goldens (a build that did not move them).
1. **C1 to C9 on Darwin, today and C4r, first line of stderr** (section 1, words in section 6): C1, C2 run on both except today's `--sanitize` heap-use-after-free and `-O0` exit 139 on the mmap shapes; C3 today U, C4r run; C4 S both; C5 no program exists (libc++abi terminates); C6 S both; C7 today runs, C4r S, **landed SW**; C8 today runs, C4r S (`fp3` at `-O2`: runs); C9 the words say it only for S. *Changes if:* the same table on `macos-26` or Darwin x86-64 (unrun).
2. **p1 to p10**: all ten equal the critic's cells, 180 runs, no mode moves a verdict; **p7 confirmed**: today RUNS (`answer ran for 7`, `result 70`, 3 of 3 in each mode), C4r refuses it (S), landed refuses it (SW). *Changes if:* a different compiler build moving p7's today cell.
3. **Real C**: (a) libpng runs on all three builds (its `--sanitize` heap-use-after-free is today's, fixed by route C) **but its own error path over a Heroes frame ends in a false abort at exit** (`pngread`, 27 of 27); (b) the SIGSEGV/SIGBUS recovery library (`probe`) and a returning handler (`lazy`) run on all three, the golden passes; (c) the three watchdogs: A and C run everywhere, **B runs today and is refused by C4r (S) and landed (SW)**; (d) the green-thread scheduler `sched`: U today (false words), S C4r, SW landed, **nothing of it ran**. *Changes if:* the same libraries on Linux (the engineer's).
4. **"Though C is correct"**: not what the standards say for the first row (undefined for a timer signal or a hardware fault; defined only where the signal is `raise`d), the inline jump (not C), contexts (not in C; POSIX.1-2008 removed them, recalled; documented on this Mac); **write instead** the repaired clause of `ffi-clauses.md`. *Changes if:* I read C11 §7.14.1.1 and POSIX's list from their text (every clause is *recalled* except this Mac's man pages).
5. **The inline jump**: `clang` on arm64 prints `error: __builtin_longjmp is not supported for the current target` (and the same for `__builtin_setjmp`); the class's wording true on both is the golden's *"a jump no function of the C library makes"*. *Changes if:* a clang that supports it on arm64.
6. **The words**: S names the handler not seen to end, true for `p4`, `p6`, `p8`, 2494 and **false (by its two alternatives) for every context, `fp3` and `as_0`**; SW (landed) is true of the ten it prints for and partly says the arms' phrase; **U still prints on C4r and landed for `rr_0`, `rr_1`, `as_1` and its parenthesis is false of them: a finding against R2's promise.** *Changes if:* the lane rewords U.
7. **C8, `fp3` and `as_0`**: *a `siglongjmp` out of a handler to a stack once given to `makecontext`* and *a signal-driven scheduler that jumps into a task*; **the existing S words are false of both** (they name an inline jump, a C++ exception or a system-library jump); it **can** be said in one clause (*"onto another stack given, now or once, to `makecontext`"*), and leaving it out costs a correct program refused at `-O0` and run at `-O2`, one byte of frame layout apart. *Changes if:* a real library fitting `fp3`.
8. **Bindability**: **none stops working** (422 `.dylib`s searched, 13 import both a handler installer and a jump, 2 import context functions and neither installs a handler; eight libraries bound; the repo's own packages hold no context or jump code); the real library whose design is refused, Boost.Context from a handler, was refused today. *Changes if:* any library that runs today and aborts under the landed runtime.
9. **Each arm's clause**: `ffi-clauses.md`; **false on Darwin**: `setcontext` out ends it (`sc1`), *never switched back* (`sched`, `bctx_signal`), *from deeper than the handler* as the condition (every depth), *whatever memory* (route A, today for copies, `fp3`), *a jump to a buffer the handler refilled* (`p10` runs), *though C is correct*, `e3` (`b_0`, `b_1` run); repaired clause written and run there. *Changes if:* Linux cells moving any of them.
10. **C12**: the 2494 golden's S words are the one S refusal the words fit (the handler never left, C called it from inside), identical today, C4r (15 passed, 0 failed each) and landed; the sentence does not need to carry it, line 427 does (*"nor called by one"*). *Changes if:* a rerun of the 2494 set reading other than 15 and 0 (the landed gate read 15 and 0, section 12).
11. **Unrun**: `macos-26`, Darwin x86-64, Windows; every row rests on this Mac alone; **route A was simulated on arm64** (`HERO_JMP_FORCE_UNCALIBRATED 3`, accessor 0), which is not Darwin x86-64. *Changes if:* the CI's Darwin legs reading `hero_signal_jump_route()`.

**The goldens' own programs, built and run by me from the frozen sources against the landed runtime** (`logs/landed-shapes-G.log`, 25 programs: the 2494 atexit golden and the 24 `.hero` programs of the 2522 set my copy holds, `-O0` and `--sanitize`, 3 runs each; `mktable3.sh`): **every row equals C4r's** (twelve `a-callback-after-*` run; the inline jump S; the jumps-within pair S; the three `sigaction`-by-another-name goldens S; the two raw-system-call goldens stopped by name; the 2494 atexit golden S; **the refill golden S and the copied-buffer golden RUNS**, the two R2 moves), and today's two U rows (refill, copied buffer) are the rows that moved. The landed tree's *own* `.expected` files for them are the lane's gate, run in section 12's last paragraph if it finished.

## 14. Where everything is (`programs/ffi-pragmatist/`, sources, scripts and text logs only: no binary, no dylib, no build folder; 396 files by `find | wc -l` at 15:49)

- **the folder itself**: the real-library programs (`libs.sh` builds the shared libraries and the plain-C drivers: `watchdog_a` to `_c`, `watchdog_h`, `probe`, `sched`, `cxxguard`, `bmon`, `bctx`; `run.sh <label> <modes> <name>...` builds each `<name>.hero` and runs it 3 times): `pngdemo`, `pngcb`, `pngread` (libpng), `uvsig` (libuv), `sqlint` (SQLite), `ossl` and `asyncjob` (OpenSSL), `rl` (readline), `bmon` (Boost.Test), `gsrun` (Ghostscript), `bctx_plain`, `bctx_signal` (Boost.Context), `probe`, `lazy`, `sched`, `abandon`, `cxxguard`, `watchdog_a` to `_c`, `watchdog_h1`, `_h2`, and the two wrong-signature builds `watchdog_a_bad1.hero`, `_bad2.hero`;
  `watchdog-a`, `-b`, `-c` (the first run's own folders, each with a `run.sh`).
- **`shapes/`**: `p1` to `p10` and the preludes (the critic's bytes, 21 files `cmp`-equal), the panel-255 shapes (`r2new.h`, `xr.h`, ...), the 2494 and 2522 golden programs, `run.sh`.
- **`depth/`**: `dsweep.h` (ten forms and `sc1`), `dsweep.hero`, `dsweep_main.c` (plain C), `sweep.sh`, `go.sh`, `summ.py`, `frame.c`, `frame2.h`/`frame2.hero`, `fp3_layout.c`, and its logs.
- **`inline2/`**: `bl.c` (`__builtin_longjmp`). **`bmon-dbg/`**: the Boost handler-disposition probe. **`search2/`**: `search.sh` and the lists. **`scripts/`**: the wrappers that set the compiler and the runtime (`go.sh`, `golanded.sh`, `golandedA.sh`, `gate.sh`, `gate-landed.sh`), the table renderers (`mktable3.sh`, `md.py`), `make-rt.sh` (how `rt/landed-A` is made), `copy-out.sh`. **`logs/`**: every log the report quotes.
- **To run them on another platform** the critic sets `HEROES` (a stage 2 or seed compiler) and `HEROES_RUNTIME` (a copy of `runtime/`, with the two C4r files over `parts/` for the Darwin route, from the evidence archive's `ffi-pragmatist/p255/r2/C4r/`) and calls `run.sh` or `depth/sweep.sh` as their headers say; **the Darwin-only pieces** are `p4.h`'s arm64 assembly (an x86-64 branch is there), `r2new.h`'s `au_switch` (arm64 assembly), the `rebind`-specific shapes, and the Boost and Homebrew paths in `libs.sh`.

**The landed tree's own goldens, by its own gate** (`p257/gate-landed.sh`, the net's own form, `./heroes run tests/harness/main.hero -- ./heroes run <set>` in `ffi-pragmatist-landed/` through the seed compiler built there, `timeout 3000`, `head -c 400000`; `logs/gate-landed-*.log`): **`fixedbugs-2522`: 25 passed, 0 failed; `fixedbugs-2494`: 15 passed, 0 failed (C12's golden among them); `fixedbugs-2679`: 10 passed, 0 failed; `fixedbugs-a-c-handler-that-recovers-leaves-no-line`: 1 passed, 0 failed** (3b's recovery golden), each `EXIT 0`.
**The frozen tree's C4r read 23 and 2 on the same set and the landed tree reads 25 and 0, because the landed tree carries the two `.expected` files R2 moves** (the refill golden in S words, the copied-buffer golden renamed `...through-a-copied-buffer-runs`); the two numbers are one fact. `fixedbugs-2702` selects nothing in the landed tree (`ls tests/golden/run | grep -c '^fixedbugs-2702'` printed 0, and no golden there is named for 2702, which resume.md says is not built; the 2679 set is 12 case names over 23 files, 10 of them goldens and 2 helper headers); my sequential gate, which was to reach that name last, was stopped by me after the three smaller sets had finished in a second process, so **that name never ran**. The compiler's own tests and the net's own tests: **unrun**.
