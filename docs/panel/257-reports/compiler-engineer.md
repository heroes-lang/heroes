# Panel 257, compiler-engineer (Linux arm64): report

Written as I go, 2026-11-23, first command 11:10 by this Mac's clock (`date` read `Mon Nov 23 11:10:47 CET 2026`).
Copy: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/compiler-engineer/` at
`828567a30a4018e74605378c3572249b2bed6d85` (`git log -1 --format=%H`, `git status --short` printed nothing).
Briefs read whole: `257-briefs/compiler-engineer.md`, `00-shared.md`, `257-reports/critic-briefs.md` (all, 448 lines).
The agent file read is the copy's `.claude/agents/compiler-engineer.md` (veto on the ceiling; here exercised on soundness only).
Load at start: `uptime` 6.46 16.46 43.51 (indicative; durations below are indicative and in no verdict).
Evidence of panel 255 extracted into `ce-work/evidence/` of my copy: `find ce-work/evidence -type f | wc -l` = 4510.

(sections are appended below as each question is run)

## 0. The two Linux builds, built inside the container

Container `p257-ce-1` (`docker run -d --name p257-ce-1 -v <copy>:/src:ro -v <copy>/ce-work/out:/out heroes-linux-arm64:latest sleep infinity`),
image ID `338a1044ac90`; inside it `uname -m` = `aarch64`, `ldd --version` = `Debian GLIBC 2.41-12+deb13u3`, `clang --version` =
`Debian clang version 22.1.8 (1~deb13u4)`; `g++` is `/usr/lib/llvm-22/bin/clang` (`readlink -f $(which g++)`), `gcc` is not present.
The script is `programs/compiler-engineer/scripts/build.sh` (a model of panel 255's `r2/leg2.sh`): one tree per variant under `/w/<variant>`,
the frozen tree (tar of my copy without `.git`, `ce-work`, `site`, `docs/panel`), then for `C2679` the evidence's
`routes/C2679/sigjump.c` over `runtime/parts/`, stage 1 = `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, stage 2 =
`./heroes build selfhost/main.hero -o heroes-next`.

| variant | what `build.log` says (`ce-work/out/<variant>/build.log`) |
|---|---|
| today | stage 1 ok 52 s; stage 2 ok 299 s; `heroes-next --version` = `heroes 0.6.0`, `sources 56db985736468b40` (the critic's stage 2 of the same tree read `56db985736468b40`: the same tree) |
| C2679 | `runtime/parts/sigjump.c` replaced, md5 `ba8487af034ea2b7dd80413bab0f2884` (the brief's), 671 lines (`wc -l`); stage 1 ok 51 s; stage 2 ok 296 s; `sources 094c31fd2ce84689` |

**The adopted Linux build built** (stage 1 and stage 2, no error: `grep -c FAILED ce-work/out/C2679/build.log` = 0).
Durations are indicative (two stage 2 builds ran side by side on an 8-CPU container).
Every cell below is built by **today's stage 2** (`/w/today/heroes-next`) with `HEROES_RUNTIME=/w/<variant>/runtime` named, so the
only thing that differs between a *today* cell and a *C2679* cell is the runtime directory; the gate (`run fixedbugs-2522`) uses each tree's own stage 2.
`cells.sh` (the scripts folder): `-O0` and `-O2`, `plain` and `--sanitize`, 3 runs a cell, each binary under `timeout 20`, **both streams
bounded** (`{ timeout 20 ./b 2>&1 1>&3 | head -c 100000 > e.txt; echo ${PIPESTATUS[0]} > c.txt; } 3>&1 | head -c 100000 > o.txt`, the exit read
from `PIPESTATUS`), stderr kept apart in `e.txt` so *the first line of stderr* is exact.

## 1. First: did the adopted Linux build build, and what did `run fixedbugs-2522` read on it

Yes it built (§ 0). The gate, `scripts/gate.sh`: `./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2522` in each tree
with that tree's own stage 2 (files `ce-work/out/gate-<variant>-fixedbugs-2522.log`):

| tree | the harness's line | the failures it names |
|---|---|---|
| today (frozen `runtime/`) | `harness: 25 passed, 0 failed` | none |
| **C2679 (the adopted Linux build)** | **`harness: 23 passed, 2 failed`** | exactly `run/fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused at -O0` (it prints the S words where the golden expects the U words) and `run/fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so at -O0` (it runs, printing `called 8`, where the golden expects an abort) |

That is the brief's *23 passed, 2 failed* and the two goldens R2 moves: the route builds and moves what R2 says it moves. The twelve
`a-callback-after-*` goldens are not in the failure list on either tree: **C1 holds on Linux, today and adopted, for all twelve** (`grep -c '^FAIL' ` = 2 on C2679, 0 today).
I did **not** run the compiler's own test `byte for byte (defect 2533)` (the brief says it is red at the frozen tree and is not mine; I ran no
`heroes test selfhost/main.hero` at all: it is not a question of this seat).

## 2. The ten programs, Linux arm64 (Debian clang 22.1.8, glibc 2.41), both runtimes, `-O0` and `-O2`, plain and `--sanitize`, 3 runs a cell

Script `scripts/cells.sh`, programs `ce-work/programs/` (the lane's `257-briefs/blind/programs/`, copied unchanged: `md5` equal, see `programs/compiler-engineer/MANIFEST`).
Files: `ce-work/out/cells-today-p.log`, `cells-C2679-p.log`. **Every one of the 80 cells (10 programs x 2 runtimes x 2 levels x 2 modes) had 3 of 3 runs
with one outcome** (40 lines per runtime, each `x3`). The outcome did not move with the level or with `--sanitize`, so the table has one column per runtime.

| program | what it does | Linux today | **Linux C2679 (adopted)** | the critic's key (`critic-briefs.md` table) | first line of stderr (S or U), copied |
|---|---|---|---|---|---|
| p1 | `siglongjmp`, static buffer | runs | runs | RUNS / RUNS | |
| p2 | `siglongjmp`, heap buffer | runs | runs | RUNS / RUNS | |
| p3 | whole copy of the saved buffer | **U** | **runs** | ABORTS U / RUNS | today: `panic: p3.answer was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`...` |
| p4 | inline jump (assembly on arm64), deep callback | S | S | S / S | `panic: p4.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left...` |
| p5 | handler returns | runs | runs | RUNS / RUNS | |
| p6 | handler calls the function | S | S | S / S | same S line, `p6.answer` |
| p7 | handler `swapcontext`s away, never back (G) | runs | **runs** | RUNS / RUNS | (no refusal) |
| p8 | handler jumps within itself, then calls | S | S | S / S | same S line, `p8.answer` |
| p9 | jump to the first of 40 live saves | **U** | **runs** | ABORTS U / RUNS | today: the U line, `p9.answer` (`saved before more save points than it keeps`) |
| p10 | the handler refills a buffer from a save the main line made, and leaves by it | **U** | **runs** | ABORTS U / RUNS | today: the U line, `p10.answer` |

**All 20 cells (10 programs x today/adopted) equal the critic's key; none differs.** (The critic's key is Linux today and Linux C2679, `-O0`, plain, 3 each; mine adds `-O2` and `--sanitize`, and they did not move.)

## 3. C5, the C++ exception (brief item 3): it can be made, and the program that makes it is correct only on a stated platform fact

Files `programs/compiler-engineer/c5/` (`c5.cpp`, `c5.h`, `c5.hero`, `plain_main.cpp`), script `scripts/c5.sh <variant>`, logs `c5-today.log`, `c5-C2679.log`.
`clang++ --version` = `Debian clang version 22.1.8`; its `g++` is clang, no GCC (`readlink -f $(which g++)` = `/usr/lib/llvm-22/bin/clang`; `which gcc` prints nothing).

The program: a C++ handler `static void c5_handler(int) { throw 7; }` installed by `sigaction`; `c5_run` has `try { raise(SIGUSR1); } catch (int) {}`, so the exception leaves
the handler, the runtime's wrapper (when the Heroes runtime is there), the kernel's signal frame and `raise`, and is caught **outside** the handler; then it calls the program's
callback, shallow (`cb(7)` from `c5_run`'s own frame) or, with `PAD=65536`, 64 KiB deeper (`alloca`). Built as `clang++ -shared -fPIC -O0 c5.cpp -o libc5.so`; bound by
`extern "c5.h" link "c5"` and `heroes build c5.hero --library lib-<v>`; run with `LD_LIBRARY_PATH`. Three libraries: **direct** (`raise(SIGUSR1)` called directly), **ptr** (`-DVIA_PTR`: the call goes
through `static int (*volatile c5_raise)(int) = raise;`), **ptr-nce** (ptr and `-fnon-call-exceptions`).

| library | control: the same C++ as a plain C++ program, no Heroes runtime (`plain_main.cpp`) | Heroes program, today and C2679, `-O0`/`-O2`, plain and `--sanitize`, shallow | the same, `PAD=65536` |
|---|---|---|---|
| direct | **exit 134, `terminate called after throwing an instance of 'int'`**, 3 of 3, no `caught` line | the same `terminate` line, 3 of 3, in every build (`-O0`/`-O2` x plain/`--sanitize`), on both runtimes | the same |
| ptr | exit 0, `c5: caught 7 outside the handler`, `answer ran for 7`, `result 70`, 3 of 3 (shallow and `PAD=65536`) | **exit 0, `caught`, `answer ran for 7`, `result 70`**, 3 of 3 in each of the 4 builds, on both runtimes | **exit 134, S**: `panic: c5.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left...`, 3 of 3 in each of the 4 builds, on both runtimes |
| ptr-nce | the same as ptr | the same as ptr | the same as ptr |

What this says, each with its measurement:
1. **The direct form is not a correct program, with or without Heroes**: a plain C++ control terminates (`exit 134`), so the exception never reaches the `catch`. Cause, inferred from the header and the pointer form's success (not from the unwind tables): `echo '#include <csignal>' | clang++ -E` prints `extern int raise (int __sig) noexcept (true);` (glibc's `__THROW` is `noexcept (true)` in C++ mode), so no call-site entry covers that call. I did not run the direct form with `-fnon-call-exceptions` (the nce library is built with `-DVIA_PTR`), so *whether `-fnon-call-exceptions` alone repairs the direct form* is unrun.
2. **A throw out of a handler can be made to work, on this platform**: through a pointer to a function not declared `noexcept` the same program is caught outside the handler (control: 3 of 3). **What makes it correct C++ is not a rule I can name**: I ran no standard's text; what is measured is one platform's unwinder (libgcc/libstdc++ on glibc 2.41, aarch64) plus a call the compiler must treat as potentially throwing. `-fnon-call-exceptions` is not needed for a synchronous `raise` (ptr and ptr-nce read alike) and I did not test an asynchronous signal (a timer) against it. So *"a C++ exception"* in the S words and in the arms' cause list **is testable, with one program, and only on a platform fact**.
3. **With the Heroes runtime the exception is caught outside the handler and the wrapper's entry is left standing** (the exception unwinds through `hero_sig_wrapper` with no cleanup; `parts/siginstall.c`). The next callback then **runs if it is called from a frame within about 5 KiB of the raise (shallow) and aborts in the S words if it is deeper (`PAD=65536`)**: § 4 below (the depth sweep) is the same effect for the inline jump. So **C5 as the table states it, *"a C++ exception through the handler: aborts (S)"*, is true only for the deep callback**; the S words themselves describe the deep case.
4. The adopted build changes nothing for C5 (`sigjump.c` is not reached by an exception: its own comment, `routes/C2679/sigjump.c` lines 62 to 64, says so, and the cells agree).

## 4. What *"a callback called while ..."* means (brief item 4, and the first issue's *"the next callback"*)

Programs `programs/compiler-engineer/mine/` (`p4n p4a p4b p4s p4d p4x p4c1 p4c2 b1 b2 b3 b3c`, each `.h` + `.hero`; they are p4's own code, `diff` against `p4.h` is in each file's one-line change), script `scripts/cells.sh`
and `scripts/pad.sh`, logs `cells-<v>-mine*.log`, `pad-<v>.log`. Every cell below is **3 of 3**, on both Linux runtimes (today and C2679), and the outcome never differed between the two runtimes,
`-O0` and `-O2`, plain and `--sanitize` unless said. **The two runtimes agree on every program of this section**, which is the point: the sentence is true or false of the *mechanism*,
and the adopted build does not change the mechanism (`hero_sig_wrapper`, `hero_sig_prune`, `hero_sig_prune` at `runtime/parts/siginstall.c:150`, `hero_sig_wrapper` at `:174`, `hero_sig_inside` at `:212`, and `hero_callback_entered` at `runtime/parts/sigguard.c:463`).

**(a) After an inline jump, a program that never calls another callback.** `p4n`: p4's inline jump out of the handler, then `return 70` with no callback: **exit 0, `result 70`** (3 of 3 in 4 builds, both runtimes). `p4a`
adds a Heroes `atexit` function (spec line 427's own example of a callback) and lets the program end by returning from `main`: **exit 0, `registered: 0`, `result 70`, `the atexit function ran`**: the atexit function, a callback called after
the handler was left by a jump this runtime did not see, **runs**. (`p4b`, same, ending by `deep_exit()`, which calls `exit(0)` from a frame 64 KiB down: **exit 134, S: `panic: p4b.at_end was called while a signal handler of C's that this runtime saw begin had not been seen to end...`**.
A first version of `p4a.h` had no compiler barrier on its `alloca` and `p4b` ran at `-O2` (the optimiser narrowed the allocation); I added `__asm__ volatile("" ::"r"(pad) : "memory")` as `prelude.h`'s `call` has, and re-ran the four builds of both: 134 S in all four. Both logs are kept, `cells-<v>-mine3.log` is the second.)

**The depth sweep, the finding.** `p4d` is p4 with the callback called `PAD` bytes (environment variable, `alloca`) below `run_case`'s frame (`pad.sh`, `-O0` and `-O2`, both runtimes, 3 of 3 each):

| PAD (bytes) | 0 | 1024 | 2048 | 3072 | 4096 | 4608 | **5120** | 6144 | 8192 | 16384 | 65536 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| outcome, all four columns alike | runs | runs | runs | runs | runs | runs | **134 S** | 134 S | 134 S | 134 S | 134 S |

(`grep -c . pad-today.log pad-C2679.log` = 22 lines each, every line `x3`; `exit=0 runs` for PAD up to 4608 and `exit=134 S` from 5120.) **After an inline jump, the next callback aborts only if it is called from a frame deeper than the dead handler's wrapper**
(the thunk's frame is compared with the wrapper's: `hero_sig_inside`, `siginstall.c:212-221`, keeps an entry only where `w > frame`; `hero_sig_prune`, `:150-160`, drops the entries at or below the frame). The threshold here is the size of arm64 Linux's signal frame plus `raise` (between 4,608 and 5,120 bytes), a platform number. **`p4s`** (p4 with the callback called directly from `run_case`, no padding): **runs, exit 0, `answer ran for 7`, `result 70`** in 4 builds on both runtimes.
So **"a callback called while one is held running aborts" is false of a program that left a handler by a jump the runtime did not see and then calls the callback from where it raised the signal**, which is the natural place.

**A later handler clears it.** `p4x`: p4's inline jump out of the handler, then a **second** signal (`SIGUSR2`) is raised at `run_case`'s depth and its handler returns, then p4's deep callback: **runs, exit 0, `result 70`** (3 of 3, 4 builds, both runtimes), where p4 itself, without the second signal, aborts in S. The entry is dropped by the second wrapper's `hero_sig_prune(own, me)`.
"Held running" is therefore **not a state the handler is in until something ends it**: it is a record of a stack address, and a later signal's wrapper that runs at that depth or shallower drops it.

**(b) A callback called on another thread while the first thread's handler is held running.** Entries are per thread (`HeroSignalStack` through `pthread_getspecific`, `parts/sigguard.c`).
- `b1`: the main thread's handler is **truly running** (it waits on a flag), and a thread the program started (`hero_thread_spawn`) calls the callback through a C function: **runs** (`answer ran for 7 on a thread the program started`, `handler left 0`, `worker returned 70`, exit 0, 3 of 3, 4 builds, both runtimes).
- `b2`: the main thread's handler is left by the inline jump, then a program-started thread calls the callback: **runs on that thread** (`answer ran for 7 on a thread`), then the main thread's deep callback **aborts, S** (`panic: b2.answer_main was called while a signal handler of C's ... had not been seen to end`), 3 of 3.
- `b3`: the same handler is running and a thread **C made** (`pthread_create`) calls the callback: **exit 134, `panic: b3.answer ran on a thread this program did not start: C called it back from a thread of its own, where this runtime's values are not safe. To run work on another thread, start it with hero_thread_spawn...`**, 3 of 3. **`b3c`, the control with no signal and no handler anywhere, prints the same words** (exit 134): that abort is the thread rule (defect 2492), not this rule.
So *"a callback called while a handler is held running aborts"* is **false of another thread** (b1, b2), and the callback that does abort on a C thread aborts in other words, for another reason.

**(c) A callback called from a second handler after the first was left by a jump.** `p4c1`: the first handler is left by `siglongjmp` (a clean leave), then a second signal's handler calls the callback: **134 S** (`panic: p4c1.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end`), 3 of 3 in 4 builds, both runtimes. `p4c2`: the first handler is left by the inline jump, then a second signal's handler calls the callback: **134 S**, the same. Both abort **because the second handler is running** (line 427's *"nor called by one"*), the first leave having nothing to do with it.

**What the three say about *"the next callback aborts"*.** It is true of the next callback **called by a handler** (p6, p8, p4c1, p4c2, C12), and of one called from deeper in the same thread's stack than a handler left by a jump this runtime does not see; **it is false of the next callback called from where the signal was raised (p4s, the C++ exception shallow, an `atexit` function at the natural end), of one after a later handler cleared the record (p4x), and of one on another thread the program started (b1, b2).**

## 5. The table of § 6, C1 to C12, on Linux arm64 (Debian clang 22.1.8, glibc 2.41)

Cell = `runs` / S / U / K / D / T / `other (exit)`, **today | adopted (C2679)**; every cell 3 of 3 runs, `-O0`, plain and `--sanitize`, unless a cell says otherwise (`-O2` was run for the p-programs, mine and the `x_`/`g_`/`jo_` shapes: it did not move any cell). Where one run differs from its siblings it is named. The command for every cell is
`cells.sh <variant> <srcdir> <log> <name>...` (`scripts/cells.sh`), the logs are `cells-<v>-{p,mine,ce,ce2,fp2,g21,c12,refill,p1n}.log` (copied to `programs/compiler-engineer/logs/`). **First line of stderr, S**:
`panic: <module>.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running, ...` (one line of 828 bytes, then the shell's `Aborted`).
**U**: `panic: <module>.answer was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps), so whether the jump left the handler is not known: ...`.
**T**: `panic: <module>.answer ran on a thread this program did not start: C called it back from a thread of its own, where this runtime's values are not safe. To run work on another thread, start it with `hero_thread_spawn` from `hero_os.h`...`. Every S, U and T line ends with the runtime's `HERO_SIGNAL_REMEDY`; in the p4 run it reads `... only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped; ...`, **with no mailbox clause** (the critic's B2, seen on the text a Linux binary printed).

| id | the claim | program(s) | today | **adopted C2679** |
|---|---|---|---|---|
| **C1** | handler left by `longjmp`, `siglongjmp`, `_longjmp` to a stack/static buffer: the callback runs | p1 | runs | runs |
| | | the twelve `a-callback-after-*` goldens (`./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2522`, § 1) | all 12 pass | all 12 pass |
| | | p1n (two handlers nested, the inner `siglongjmp`s out of both) | runs | runs |
| **C2** | the same to a heap, TLS, mapped-page, late-loaded-library buffer, or one of 40 live saves | p2 (heap) | runs | runs |
| | | **p9 (40 live saves, jump to the first)** | **U** (`saved before more save points than it keeps`, `HERO_SIG_JUMPS 32`) | runs |
| | | `plain_heap`, `plain_mmap` | runs | runs |
| | | `heap` | plain runs; **`--sanitize`: exit 1, `ERROR: AddressSanitizer: heap-use-after-free ... #0 hero_jmp_fingerprint runtime/parts/sigjump.c:138:53`** | runs |
| | | `mmap` | **plain: exit 139 (SIGSEGV, nothing on stderr); `--sanitize`: exit 1, `ERROR: AddressSanitizer: SEGV on unknown address ... #0 hero_jmp_fingerprint sigjump.c:138:53`** | runs |
| | | `handler_heap`, `handler_mmap` | `handler_heap` plain runs, sanitize ASan heap-use-after-free; **`handler_mmap` plain 139, sanitize ASan SEGV** | both run, plain and sanitize |
| | | `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib` | all six run | all six run |
| | | `pngdemo` (libpng) | **unrun in this image**: § 9b | see § 9b |
| **C3** | whole copy of a saved buffer, and a buffer the **handler** refills from a save the main line made, then leaves by: runs (R2) | p3 | **U** | runs |
| | | **p10** | **U** | runs |
| | | `gcopy` (the golden's shape) | **U** | runs (`called 8`) |
| **C4** | an inline jump: aborts (S) | p4 (assembly on arm64; the 64 KiB `inline_deep` callback) | S | S |
| | | the golden `...a-correct-inline-jump-out-of-a-handler-is-refused-in-words-never-false` | passes | passes |
| | | **p4s, the same jump and a callback called from `run_case`'s own frame** | **runs** | **runs** |
| **C5** | a C++ exception through the handler: aborts (S) | § 3 | shallow: runs; deep: S | the same |
| **C6** | a jump that lands within the handler, then a callback: aborts (S) | p8 | S | S |
| | | the two `...jumps-within-itself...` goldens, `grefill` | goldens pass; `grefill` **U** | goldens pass; `grefill` **S** |
| | | the `...refills-by-memcpy...` golden | passes (U words) | **fails the golden: it prints the S words, not the U words it expects, and still aborts** (one of the two R2 moves, § 1) |
| **C7** | a context switched away from inside a handler and never back (G): aborts (S) | **p7** | runs | **runs** |
| | | `g_1_0`, `g_1_1` (form 1, the handler `swapcontext`s to the runner's context) | run | run |
| | | `g_2_0`, `g_2_1` (form 2, to a coroutine that `siglongjmp`s out), `g1_0`, `g1_1`, `g2`, `g2_1` (critic's) | run (the `--sanitize` runs print `WARNING: ASan is ignoring requested __asan_handle_no_return` on stderr, exit 0) | run (the same warning) |
| | | **`g_3_0`, `g3`** (form 3, to a coroutine that returns through `uc_link`) | run | **S** |
| | | `g_3_1` | runs | runs |
| **C8** | a handler left by a jump into a coroutine | `fp3`, `as_0`, `cs_1` | run | **run** (`fp3 --sanitize` exits 1 after its whole output: LeakSanitizer on the shape's own `calloc`, `ft_make r2new.h:308`, 4,560 bytes) |
| **C9** | the words | § 7 | | |
| **C10** | read across tables | rows that split **by platform** against Darwin are the critic's; **rows that split by build on this platform**: p3, p9, p10, `gcopy`, `heap`/`mmap`/`handler_*` (today fails, adopted holds), p7/G (adopted runs: § 6) | | |
| **C11** | thread, second handler, never called again | § 4 | | |
| **C12** | `exit()` in a handler reaching an `atexit` function | § 8 | | |

## 6. C7, G (brief item 6): which forms the adopted Linux build refuses, which it runs

The six G forms of panel 255's `xg_run(form, onstack, cb)` (`compiler-engineer/p255/r2/shapes/xn.h:251`, read): **form 1** the handler `swapcontext`s to the runner's own context taken before the signal; **form 2** to a coroutine on a heap stack that `siglongjmp`s to the runner's buffer; **form 3** to a coroutine that returns through `uc_link` to the runner's context; `_0` on the thread's stack, `_1` with the handler on an alternate stack. The runner then calls the program from a frame 64 KiB deeper.
Plus p7 and the ffi-pragmatist's `g1_0 g1_1 g2 g3` and the critic's `g2_1` (`ce-work/shapes-critic-full`, the critic's `fsh/r2new.h`). 3 of 3 each, `-O0` and `-O2`, plain and `--sanitize` (`cells-<v>-ce.log`, `-fp2.log`, `-g21.log`):

| form | today | **adopted Linux build** |
|---|---|---|
| p7, `g_1_0`, `g_1_1`, `g1_0`, `g1_1` (form 1) | runs | **runs** |
| `g_2_0`, `g_2_1`, `g2`, `g2_1` (form 2) | runs | **runs** (`--sanitize`: exit 0 and `WARNING: ASan is ignoring requested __asan_handle_no_return` on stderr, both runtimes) |
| **`g_3_0`, `g3`** (form 3) | runs | **S**, exit 134 (`panic: g30.answer was called while a signal handler of C's that this runtime saw begin had not been seen to end: ...`) |
| `g_3_1` (form 3, handler on the alternate stack) | runs | **runs** |

**The adopted Linux build refuses 2 of the 12 G programs I ran (p7, `g_1_0 g_1_1 g_2_0 g_2_1 g_3_0 g_3_1`, `g1_0 g1_1 g2 g2_1 g3`: form 3 on the thread's stack, `g_3_0` and `g3`) and runs the other 10**, which agrees with panel 255 R4's *"on Linux, C 2679 alone refuses `g_3_0`"* and the engineer's own § 3.4. (`g_3_0` is refused because the swap is decoded to the coroutine's heap stack, which proves nothing left, and the coroutine's return through `uc_link` is glibc's own `setcontext`, unseen: panel 255's account, § 3.4; I did not re-derive it.)
**Which clause of an arm is false on Linux until defect 2702 lands** (defect 2702, `gh issue view 2702`, OPEN, *"A Heroes function runs inside a signal handler that switches contexts..."*): the **context clause** of f1 and f2, *"a switch of context never switched back"*, in the list of ways a handler is *left* and *held running*: on the adopted build **p7 runs** (3 of 3, 4 builds), and so do 9 of the 11 other G programs. f1b drops it, f3 never names a context. **But f1b and f3 are false on Linux in the same place under a literal reading**: f1b's *"left any other way (a jump no function of the C library makes)"* and f3's *"has not been seen to end, by return or by `longjmp`, `siglongjmp` or `_longjmp`"* both include a handler left by `swapcontext`, which is left *any other way* and is not one of the three, and p7 runs. The repair is not to name the context: it is to say, for as long as 2702 is open, that *the runtime follows a context switch like a jump, by the stack pointer it installs* (what `swapcontext`/`setcontext` do in `routes/C2679/sigjump.c`), or to scope the sentence by platform.

## 5b. C6, the refill (brief item 5): is any correct program that LEFT the handler refused under the adopted build?

**None was found.** What I built and ran (3 of 3 per cell; `-O0` and `-O2`, plain and `--sanitize`; `cells-<v>-refill.log`, `-ce2.log`, `-p.log`, `-fp2.log`; programs in `programs/compiler-engineer/mine/` and the panel-255 shapes named):

| program: what refills the buffer, and how the handler leaves | today | **adopted C2679** |
|---|---|---|
| **p10** (the critic's): the handler `memcpy`s a save the main line made into a second buffer and `siglongjmp`s to the copy | U | **runs** |
| **p10h**: the same with the copy **in the heap** (`malloc`) | U | **runs** |
| **p10a**: the same with the handler on an **alternate stack** (`SA_ONSTACK`, 64 KiB) | **runs** | runs |
| **p10c**: the handler `memcpy`s a `ucontext_t` the main line saved with `getcontext` and `setcontext()`s to the copy | U | **runs** |
| **p10d**: 40 live frames each saving a buffer; the handler copies the **first** save to a second buffer and leaves by the copy | U | **runs** |
| p3 (a whole copy made by the main line before the signal), `gcopy` | U, U | runs, runs |
| `x_refill_out` (a buffer saved shallow, refilled by `memcpy` **outside** any handler, then jumped to), `x_realloc` | U, U (`x_realloc --sanitize`: ASan heap-use-after-free) | runs, runs |
| *the programs whose jump lands inside the handler:* **p8**; `grefill` (the golden's shape, the handler copies **its own** save into a stale buffer and jumps to it); `x_partial` (a partial copy of the handler's own save into a static buffer, jumped to within the handler), `x_unseen`, `x_unseen0` (the handler refills the buffer by glibc's own `__sigsetjmp` reached by `dlvsym`, a save the runtime never sees, jumps to it within the handler and calls the callback from the still-running handler; `extra.h:225-271`, read) | S; U; U, U, runs (`x_unseen0` today) | **S; S; S, S, S** |

So **on the adopted build every program that leaves the handler through a refilled or copied buffer ran (p10, p10h, p10a, p10c, p10d, p3, gcopy, `x_refill_out`, `x_realloc`: 9 programs), and every one refused is a program in which the callback is called by the handler or the jump lands in it (p8, `grefill`, `x_partial`, `x_unseen`, `x_unseen0`)**. The class the runtime acts on is **where the jump lands**, as the critic's B22 said, and the arms' *"a jump to a buffer the handler refilled aborts"* (e1/e1b of the first issue) is false of p10, p10h, p10c, p10d.
**What I tried and did not find**: a refill that leaves and is refused; the programs I could not make (so a question, not a finding): a refill by a second save point in the *handler* followed by a jump to the *main line's* older buffer (it lands outside by construction and is p1), and a refill by another thread (not meaningful for a per-thread record). **What would change the answer**: any program in which the target's stack pointer, as `swapcontext`/`longjmp` will install it, lies below the handler's wrapper frame while the handler is in fact over; the decode makes that impossible on one stack by construction and I found none on two.
**Where the adopted build does refuse a correct refill-and-leave program**: when the decode is not verified (route A): § 9a, p3, p10, p10h, p10d, `gcopy`, p9 refuse in the U words.

## 4b. One more shape of § 4: the order of two callbacks

`p4sd`: p4's inline jump out of the handler, then the callback called **shallow** (from `run_case`'s own frame), then **deep** (p4's 256 KiB frame): **both run** (`answer ran for 7`, `answer ran for 7`, `result 140`, exit 0, 3 of 3, 4 builds today and 4 adopted). p4, which makes only the deep call, aborts in S. So the shallow call **drops** the standing entry (`hero_sig_inside` calls `hero_sig_prune(own, frame)` before it looks, `siginstall.c:212-216`) and the later deep call finds nothing. **Whether a callback aborts after a jump the runtime did not see depends on what was called before it, on the same thread, from where**: the sentence's *"a callback called while one is held running aborts"* gives a reader no way to know which callback that is.

## 7. The words (brief item 7, C9)

Every refusal I ran, the first line of stderr (the runtime prints one `panic:` line of about 830 bytes; the shell adds `Aborted`):

| kind | printed by | programs | today | adopted C2679 |
|---|---|---|---|---|
| **S** | `HERO_SIGNAL_STANDING` (`runtime/parts/sigguard.c`, `hero_signal_refuse`) | p4, p6, p8, p4b, p4c1, p4c2, b2 (main thread), c5 deep, C12, p7-family form 3 | `panic: <fn> was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running, ...` | **the same line**: the first stderr line of p4 `-O0` plain, `./bin-p4-O0-plain 2>&1 >/dev/null | head -1 | md5sum`, reads `509bd3c5652729f9bc038a791c7d8c24` under today's runtime **and** under C2679's |
| **U** | `HERO_SIGNAL_UNFOLLOWED` | p3, p9, p10, p10h, p10c, p10d, `gcopy`, `grefill`, `x_refill_out`, `x_partial`, `x_unseen` (today) | `panic: <fn> was called after C, inside a signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps), so whether the jump left the handler is not known: a handler not seen to end is held to be running, ...` | **none of the adopted build's 361 outcome lines prints U** (`grep -hv '^DONE' cells-C2679-*.log | wc -l` = 361; `grep -h '| U |' cells-C2679-*.log | wc -l` = 0), decode verified (`decoding: 3`); **the same runtime forced to route A (`decoding: 2`, § 9a) prints it for p3, p9, p10, p10h, p10d, `gcopy`, `grefill`, `x_heap`, `x_tls`, `x_dlopen`** |
| **T** | `hero_thread_refuse` (`parts/thread.c`) | b3, b3c | `panic: b3.answer ran on a thread this program did not start: C called it back from a thread of its own, where this runtime's values are not safe. To run work on another thread, start it with `hero_thread_spawn` from `hero_os.h` ...` | the same |
| **D** | `HERO_SIGNAL_DELIVERED` | the 2494 goldens `...given-to-signal...`, `...the-programs-own-sigaction-installs...` | § 8 (gate `fixedbugs-2494`) | § 8 |
| **K** | `HERO_SIGNAL_CALLED` | **no program of mine printed it**, and `grep -l 'C called it while handling a signal' tests/golden/run/*.expected` prints **nothing**: no golden pins K's words | unrun by me | unrun by me |

**Does *"saying the handler was not seen to end"* describe it?** For **S: yes, literally** (`had not been seen to end` is in the line). For **U: only its last clause** (*a handler not seen to end is held to be running*): its first words say the jump's *buffer* has no record, so *"the handler was not seen to end"* is what the line concludes, not what it opens with. **For T and D: no.** T is the thread rule (defect 2492) and D is *"ran as a signal handler"*; neither says a handler was not seen to end. K is unrun. The arms' clause is therefore true of S and, loosely, of U, and the sentence should not promise it of *every* refusal of a callback.
**R2's promise and the U words**: R2 says the U words go (*"the true 'had not been seen to end'"*). On the adopted build with the decode verified none of my cells printed U; **where the decode is not verified the adopted build prints U for the copy programs (p3, p10, p10h, p10d, `gcopy`) and for p9**, because `sigguard.c` is unchanged by the Linux route (the route replaces `sigjump.c` alone) and its `HERO_SIGNAL_UNFOLLOWED` reading is what route A feeds. That is **a finding against R2's promise, conditional on route A**; whether any CI platform reaches route A is § 9.
**The S words' parenthetical**: *"an inline `__builtin_longjmp`"* names a spelling that **does not compile on this platform** (`clang -c bl.c`: `error: __builtin_setjmp is not supported for the current target`, Debian clang 22.1.8 aarch64; `scratch-bl/bl.c`), *"a C++ exception"* is § 3's one program (needs a non-`noexcept` call), and *"a jump inside a system library"* I did not run. None of the three names **a switch of context**, which is what p7-family G is, nor the **depth condition** of § 4.

## 9a. The adopted build where the decode does not verify (route A), forced: the cost of *"whatever memory"*

Not a platform I have: a **forced** copy. `/w/C2679A/runtime` is C2679's runtime with **one line added** to `parts/sigjump.c`, `#define HERO_JMP_FORCE_UNCALIBRATED 1` (the line panel 255 used for its `C4fail` column; `diff` against C2679's `sigjump.c` is two lines; md5 `9bfa2aacb1f232fdd34afbaa8b3ed2e4`). The accessor (`acc`, `compiler-engineer/p255/r2/shapes/acc.hero`: `hero_signal_jump_decoding()`, *1 the setjmp family by the key, 2 contexts by their field, 3 both, 0 A*) reads **`decoding: 3` on C2679 and `decoding: 2` on C2679A**, 3 of 3 each, plain and `--sanitize` (`ce-work/out/acc-C2679.log`, `acc-C2679A.log`). So the forced build has the setjmp-family decode **off** (route A for jumps) and the context decode on. `-O0`, 3 of 3 (`cells-C2679A-{p,mine,fp,ce}.log`):

| program | C2679 (decoding 3) | **C2679A (decoding 2)** |
|---|---|---|
| p1 (static), p2 (heap, saved and jumped to at once), p5, p7 | runs | runs |
| **p3, p10, p10h, p10d** (copies and refills) | runs | **U** |
| **p9** (40 live saves, jump to the first) | runs | **U** |
| **`gcopy`**, **`grefill`** | runs, S | **U, U** |
| **`x_heap`, `x_tls`, `x_dlopen`** (a heap, a TLS, a late-loaded buffer; a second save or jump in between) | run | **U** |
| `heap`, `mmap`, `handler_heap`, `handler_mmap`, `plain_heap`, `plain_mmap`, `jo_heap`, `jo_tls`, `jo_lib` | run | run |
| p10a (altstack), p10c (`ucontext_t` copy, decoded by its field) | run | run |
| p4, p6, p8 | S | S |
| p4s, p4x (§ 4) | run | run |
| `g_1_0`, `g1_0`, `fp3`, `as_0` | run | run |
| `g_3_0`, `g3` | S | S |

So **where the decode does not verify, the adopted build refuses the copied-buffer program, the 40-save program and the heap/TLS/late-loaded programs that the sentence's *"wherever the buffer lives"* promises to run, in the U words**: *"whatever memory"* is a clause that **holds only where the accessor reads 3** (or the setjmp-family bit of it), and R1's own fallback says so (panel 255 R1, *"glibc's four other architectures, a future C library, Darwin x86-64"*). A sentence that must be true on every platform the runtime runs on **needs the condition** or a scope; one that must be true on Linux arm64 and x86-64 with glibc 2.41/2.43 needs the CI's own reading (§ 9).

---
**The report resumed at 14:35 (`date`) after the API's five-hour limit stopped the first run in the middle of § 9.** Sections 0 to 7, 5b, 4b and 9a above are
the first run's, unchanged. What follows is the resumed run's (the first run had left its logs for §§ 8, 9 and 9b in `ce-work/out/` unwritten; I re-read them
by their script and re-ran what the resumed run's findings rest on, and say below which). The load of the Mac read **230 (`uptime`, 15:04)** while the
resumed run worked, and a second container of another lane (`l2702-dbg`) held 1,109 % of a CPU (`docker stats --no-stream`): every duration is discarded.

## 8. C12 (brief item 10): `exit()` inside a handler reaching an `atexit` function

Program: the 2494 golden `fixedbugs-2494-a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` (`.h`, `.hero`, `.expected` copied to
`programs/compiler-engineer/mine-c12/`): a C handler that calls `exit(0)`, a Heroes function handed to `atexit`, a `raise`. The handler never leaves: `exit` runs the
`atexit` function inside it, which is line 427's *"nor called by one"*.
- **Cells** (`scripts/cells.sh <v> /src/ce-work/mine-c12 ...`; `out/cells-today-c12.log`, `cells-C2679-c12.log`): **today and C2679 alike**, `-O0` and `-O2`, plain and
  `--sanitize`, 3 of 3 each, **exit 134, stdout `registered: 0|installed: 0`, kind S**: `panic: fixedbugs2494aheroesatexitfunctionachandlerreachesthroughexitisrefused.at_end was called
  while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be running, ...`.
- **The first stderr line is byte-identical under the two runtimes**: `./<bin> 2>&1 >/dev/null | head -1 | md5sum` in the container reads `028f347456d4f3fb99c1fee109f1e0f4` for the binary built against today's runtime and for the one built against C2679's.
- **The gate**: `scripts/gate.sh <v> fixedbugs-2494` (the harness, the tree's own stage 2): today `harness: 15 passed, 0 failed`, C2679 `harness: 15 passed, 0 failed`
  (`out/gate-today-fixedbugs-2494.log`, `out/gate-C2679-fixedbugs-2494.log`). The golden's `.expected` carries the runtime's tail **without** the mailbox clause (the critic's B2), as the frozen tree's runtime prints it.
- **Ubuntu 26.04, glibc 2.43** (§ 9): the same binary, 3 of 3, exit 134, S, both runtimes, plain and `--sanitize`.
- **Do the words fit the case?** The handler never left, so *"either C called it from inside that handler"* is the true alternative, and the S line's own first half says so; a reader told only
  *"saying the handler was not seen to end"* is told something true (the handler had not ended: the process was ending inside it). **Any arm that defines the refused class as *"left some way"* does not
  cover C12 by its letter** (f1, f1b, f2 define *held running* by *left any other way or jumped within*; a handler that never left is neither): the class has to read *"has not ended"* (f3's words) or add *"or still running"*.

## 9. The CI's Linux library (brief item 9): the adopted build run on Ubuntu 26.04, glibc 2.43, and the accessor's reading

**What I ran and what I did not.** The literal `compiler-engineer/p255/r2/ubuntu-bins.sh` with `VARIANT=C2679` was **not run as written**: it rebuilds a stage 1 and a stage 2 (about 6 minutes uncontended, hours at this load)
and builds only the evidence's shapes; the ten programs and my own are not among them. Its method is panel 255's and I kept it: **the binaries are built in the Debian container** (clang 22.1.8, glibc 2.41), **never run
there for this section**, and **run in `ubuntu:26.04`** (container `p257-ce-2`, `docker run -d --name p257-ce-2 -v <copy>/ce-work/out/bins:/bins:ro -v <copy>/ce-work/scripts:/scripts:ro ubuntu:26.04 sleep infinity`), so the C library the
binary loads is Ubuntu's: `uname -m` `aarch64`, `ldd --version` first line `ldd (Ubuntu GLIBC 2.43-2ubuntu2.4) 2.43`. The builder is `scripts/bins.sh <variant>` (`out/bins-C2679.out`: *built 108 binaries for C2679*; `bins-today.out`: *106 for today*),
the runner `scripts/ubuntu-run.sh <variant>` (3 runs, `timeout 20`, both streams bounded), its output `out/ubuntu-C2679.log` and `out/ubuntu-today.log`, the comparison `scripts/compare.py` and `scripts/table.py`.
**One difference from `ubuntu-bins.sh`, checked**: it builds with the variant's own stage 2; mine builds with today's stage 2 and `HEROES_RUNTIME=/w/<variant>/runtime` (as every cell of this report). The check: `p3 p4 p7 p9 p10`
built by **C2679's own stage 2** (`/w/C2679/heroes-next --version` = `sources 094c31fd2ce84689`, no `HEROES_RUNTIME`) and run in Ubuntu, 3 of 3 each: p3, p7, p9, p10 **run**, p4 **S** (`out/bins/C2679own/`): the same as the cells built the other way.

**The accessor**: `acc` (`compiler-engineer/p255/r2/shapes/acc.hero`: `hero_signal_jump_decoding()`, 1 the setjmp family by the key, 2 contexts by their field, 3 both, 0 route A for both) run in Ubuntu 26.04
reads **`decoding: 3`, 3 of 3 plain and 3 of 3 `--sanitize`** (`grep acc out/ubuntu-C2679.log`), as in Debian 2.41 (§ 9a). **So every Ubuntu row below ran on a build whose decode verified against glibc 2.43's layout**,
and *"whatever memory"* holds on it by route C; the forced route A is § 9a (`decoding: 2`).

**Result, Ubuntu 2.43 against Debian 2.41, the same binaries**: `scripts/sweep.sh <bindir> 20` run in each container, 20 runs per binary (`-O0`, plain and `--sanitize`), 106 binaries for C2679 and 104 for today
(`out/sweep-debian.log`, `out/sweep-ubuntu.log`): **`diff` of the two files differs on 4 lines: the two headers and `cs_1-O0-plain`** (C2679: Debian 20 of 20 run; Ubuntu **2 run, 18 abort S**. today: Debian 20 of 20 run; Ubuntu **1 runs, 19 abort S**).
Every other binary, in every cell, did the same on the two libraries (210 binary-by-runtime lines compared, 20 runs each). The Ubuntu table beside the Debian one (3 runs, `scripts/table.py`; `-O0`; plain / `--sanitize` where they differ):

| program | Debian 2.41 today | Debian C2679 | Ubuntu 2.43 today | **Ubuntu 2.43 C2679 (`decoding: 3`)** |
|---|---|---|---|---|
| p1, p2, p5, p1n, p10a, `jo_heap`, `jo_tls`, `jo_lib`, `plain_heap`, `plain_mmap`, `x_heap`, `x_tls`, `x_dlopen` | runs | runs | runs | **runs** |
| p3, p10, p10c, p10d, p10h, `gcopy` | U | runs | U | **runs** |
| p9 (40 live saves) | U | runs | U | **runs** |
| `heap`, `mmap`, `handler_heap`, `handler_mmap` | runs; plain `mmap` and `handler_mmap` exit 139, `--sanitize` ASan | runs | the same as Debian | **runs** |
| p4, p4b, p4c1, p6, p8, b2 (main thread), `g3`, `g_3_0`, `grefill`, C12 | S for p4, p4b, p4c1, p6, p8, b2, C12; `g3` and `g_3_0` **run** today; `grefill` **U** today | S for all nine (`grefill` **S**: R2's move from U) | the same as Debian | **S** |
| p4s, p4x, p4a, p7, `g_1_*`, `g_2_*`, `g_3_1`, `g1_*`, `g2`, `g2_1`, `as_0`, `fp3`, `b1` | runs | runs (`--sanitize` of `g_2_*`, `g2`, `g2_1`: ASan's `__asan_handle_no_return` warning on stderr, exit 0; `fp3 --sanitize`: LeakSanitizer exit 1 after its whole output, under C2679 only) | runs | **runs** (the same two notes) |
| b3 (a thread C made) | T | T | T | **T** |
| **`cs_1`** (the ffi-pragmatist's: an alternate-stack handler leaves by `setcontext` into a coroutine whose stack is `malloc`'d) | runs | runs | **S (19 of 20)** | **S (18 of 20)** |
| **`c_1`** (the same shape left by `siglongjmp`) | runs | runs | **S, 3 of 3 plain**; runs `--sanitize` | **S, 3 of 3 plain**; runs `--sanitize` |

The two last rows are § 10's finding. **Clauses that hold only where the accessor reads 3** (route C) are the whole `longjmp` family's *"whatever memory"*: on Ubuntu 2.43 they hold (`decoding: 3`), and nothing in this table needed route A.
Linux x86-64 (`ubuntu-26.04`) has no image here: **unrun**, § 11.

## 9b. libpng (`pngdemo`, C2's row), run where the library exists

The image `heroes-linux-arm64:latest` has no libpng (`pkg-config --modversion libpng` prints *not found*, as the shared brief says). `heroes-linux-arm64:gtk4-ui` (container `p257-ce-3`) is **the same Debian 13, glibc 2.41, clang 22.1.8** and has **libpng 1.6.48**
(not the 1.6.59 of the Mac): `scripts/png.sh`, `out/png.log`, `png-src/` (`png.hero`, `pngdemo.hero`, `good.png`, `trunc.png`), each program built plain and `--sanitize` at `-O0` and `-O2` (`pngdemo` `-O0` only), 3 runs a cell, today's stage 2 as the compiler:
| runtime | `png` (a good image, a truncated one) and `pngdemo` plain | `--sanitize` |
|---|---|---|
| today | runs: `0 68 3072 -1 3072`, `good 64048 truncated -3 good again 64048 truncated again -3` | **exit 1, ASan `heap-use-after-free ... #0 hero_jmp_fingerprint runtime/parts/sigjump.c:138:53`** (3 of 3, `png` at both levels and `pngdemo`) |
| C2679 | runs, same lines | **runs** (3 of 3 each) |
| C2679 forced to route A (`HERO_JMP_FORCE_UNCALIBRATED`, § 9a) | runs | runs |
Debian's `libpng16.so.16` imports **`__longjmp_chk`** and `_setjmp` (`nm -D /usr/lib/aarch64-linux-gnu/libpng16.so.16 | grep -i jmp`, in `p257-ce-3`): the library's `longjmp` is the **fortified spelling**. The runtime hooks it (`runtime/parts/sigjump.c:261`, `siglookup.c:141` at the frozen tree), and my `p1f` (a handler that calls `__longjmp_chk` itself, § 10) runs. *The spec's list `longjmp`, `siglongjmp`, `_longjmp` does not name `__longjmp_chk`*: a reader of a distribution's library sees the name in `nm`.

## 4c. The depth boundary, read, not inferred (resumed run): *"from deeper in the stack than the handler was"*

`p4e` (programs/compiler-engineer/mine/): p4's inline jump out of the handler, then the callback `PAD` bytes below `run_case`'s frame; the handler stores `__builtin_frame_address(0)` and the program prints
**D = the dead handler's frame address minus the address the callback's frame starts at (D > 0: the callback is deeper than the handler was)**. `scripts/padE.sh <variant>` (Debian container, `-O0` and `-O2`, PAD 4608 to 5120 step 32 and 5200, 5300; 3 runs each, every run of a PAD alike):
| level | last D that runs | first D that aborts S | both runtimes alike (`out/padE-today.log`, `out/padE-C2679.log`) |
|---|---|---|---|
| `-O0` | **-128** (PAD 4704) | **-96** (PAD 4736) | yes |
| `-O2` | **-48** (PAD 4832) | **-16** (PAD 4864) | yes |
So the abort starts **about 100 bytes (`-O0`) or 30 bytes (`-O2`) before the callback is as deep as the handler's own frame**: the entry the runtime keeps is the **wrapper's** frame, which lies above the handler's by the handler's frame. *"From deeper than the handler was"* is exact to within one handler frame (a callback up to that far above the handler's frame already aborts); under *"may abort"* the sliver does not make the sentence false, and an arm that says *"aborts"* without the depth (f1, f1b, f2, f3, e1 to e3) is false for **p4s** (shallow: runs), **p4sd**, **p4x** (a later handler clears the entry) and for the callback an `atexit` function is (p4a: runs).

## 10. NEW: two clauses that are false on Linux today and adopted, on both C libraries: a handler left for a stack that is not the thread's own

The runtime's test *"is a wrapper still running above this callback's frame?"* is, in the frozen tree, `hero_sig_inside` and `hero_sig_prune` (`runtime/parts/siginstall.c:150-160`, `:212-221`; the landed tree's `siginstall.c` is unchanged by `git diff --stat 828567a30 523d35537`): an entry is dropped when the callback's frame is on the same stack and the entry is not above it,
or when the entry is on an alternate stack and the callback's frame on **the thread's own stack**. The thread's own stack is known (`own->lo, own->hi`); **a stack that is not the thread's own, a coroutine's (`makecontext`) or any stack a program gives itself, is not known, and two of them are
compared by raw address as if they were one stack** (`there == here` both false, then `w <= at`). So when the handler runs on one such stack (the alternate stack) and the program leaves it **for a correct reason**, and the callback is later called on another such stack, the answer is **which of the two mappings lies lower in memory**.

**Programs** (all in `programs/compiler-engineer/`): `cs2` (mine: an alternate-stack handler, a coroutine on its own `mmap`'d stack made with `makecontext`; the coroutine saves with `sigsetjmp` (HOW=jmp) or `getcontext` (HOW=ctx), the handler leaves by `siglongjmp` / `setcontext`, the coroutine calls the program twelve frames down; **ORDER=0** maps the coroutine's stack first, the alternate stack second; **ORDER=1** the reverse, as a program that installs
`sigaltstack` at start-up and makes coroutines later does; it prints which stack is higher); panel 255's `cs_1` (`setcontext`) and `c_1` (`siglongjmp`), whose coroutine stack is `malloc`'d; and `tal` (mine: the same alternate-stack handler on a thread started with `hero_thread_spawn`, the alternate stack mapped before or after the thread).
Scripts `scripts/cs2.sh <bindir> <variant>`, `scripts/tal.sh`, `scripts/ubuntu-run2.sh`; the same binaries run in the Debian container (`/out/bins`, glibc 2.41) and in `ubuntu:26.04` (`/bins`, glibc 2.43), 3 runs a cell:
| program | the coroutine's (thread's) stack is | Debian 2.41 today / C2679 | Ubuntu 2.43 today / C2679 |
|---|---|---|---|
| **`cs2` ORDER=0**, HOW=jmp and HOW=ctx, plain and `--sanitize` | ABOVE the alternate stack | **runs** 3 of 3, all four cells, both runtimes | **runs** 3 of 3, all four cells, both runtimes |
| **`cs2` ORDER=1, HOW=jmp (`siglongjmp`)**, plain | **BELOW** | **S** 3 of 3, exit 134, both runtimes | **S** 3 of 3, both runtimes |
| **`cs2` ORDER=1, HOW=ctx (`setcontext`)**, plain | **BELOW** | **S** 3 of 3, both runtimes | **S** 3 of 3, both runtimes |
| `cs2` ORDER=1, either HOW, `--sanitize` | BELOW | **S** 3 of 3 each (first stderr line ASan's `__asan_handle_no_return` warning, the panic the next) | **S** 3 of 3 each |
| `cs_1` (`setcontext`, coroutine stack `malloc`'d 256 KiB) | Debian: ABOVE (an `mmap`); **Ubuntu: BELOW** (`cs1a` prints `0xaaaad76012c0`, the program's heap, against the alternate stack at `0xffff8a01f000`) | runs 20 of 20, both runtimes (`out/sweep-debian.log`) | **S 19 of 20 (today), 18 of 20 (C2679)** (`out/sweep-ubuntu.log`) |
| `c_1` (`siglongjmp`, same shape) | as `cs_1` | runs 3 of 3, both runtimes | **S 3 of 3 plain, both runtimes**; runs `--sanitize` (ASan's allocator places it above) |
| **`tal`**, an alternate-stack handler on a **thread the program started**, ORDER=0 and 1, plain and `--sanitize` | ABOVE or BELOW (printed) | **runs** 3 of 3 in all 4 cells, both runtimes | **runs** 3 of 3 in all 4 cells (C2679) |
The adopted build **changes nothing here**: today and C2679 print the same cells on both libraries. **Why Ubuntu differs from Debian for `cs_1` and `c_1`, measured**: `cs1a` (the same program with both stacks' addresses printed) shows Debian's `malloc(256 KiB)` an `mmap` above the alternate stack (`0xffffa3421010` against `0xffffa33e1000`, six runs) and Ubuntu's the program's `brk` heap
(`0xaaaad76012c0` against `0xffff8a01f000`, six runs): **a difference of the C library's allocator, never of the runtime**, which the runtime's address compare turns into a refusal. (That glibc 2.43 places it there is read from the addresses; why, I did not look into.)

**What this makes false.** (1) **w26 and w27's *"A handler of C's ends by ... a `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of it"*** (the clause says a callback then runs): **false on Linux for `cs2` ORDER=1**, today and adopted, on both C libraries, for **both** HOW; and on the CI's glibc 2.43 for panel 255's own `c_1` and `cs_1`. (2) **f2's *"A callback called after C left a handler by `longjmp`, `siglongjmp` or `_longjmp` runs, wherever the buffer lives"** and **e1 and e1b's *"may leave a handler by ... and the program's callbacks run on, whatever memory the buffer is in"***: false for the same programs (the buffer lives on a coroutine's stack). (3) The class *"a handler not seen to end"* is therefore not a fact about the handler that C's code decides: for these programs it is the order of two mappings.
**What it does not make false**: the thread's own stack (p1 to p10, the main line; `tal`: the alternate stack on a started thread, either order), a handler and a callback on the same coroutine stack (`cj`, `c_0`, `d_0`, `d_1`, `e`, `ns_*`: run on both libraries, `scripts/ubuntu-run2.sh`), and `longjmp`, `_longjmp` and `__longjmp_chk` themselves: `p1l` (`setjmp`/`longjmp`), `p1u` (`_setjmp`/`_longjmp`), `p1f` (`sigsetjmp`, the handler calls glibc's `__longjmp_chk`, the fortified spelling libpng imports) and `p7s` (`getcontext` in the main line, `setcontext` in the handler, no copy, no swap) **run, 3 of 3, plain and `--sanitize`, today and C2679, on Debian 2.41 and on Ubuntu 2.43**.
**The repair** is in `ce-clauses.md` (R-1) and is run there. **What the repair would be for the runtime** (not mine to decide): compare two entries only when the runtime knows both stacks, or treat two unknown stacks as *unsure*; that is the lane's, and I did not try it.

## 7b. Corrections to § 7 (resumed run)

- **K was run.** § 7 says *"no program of mine printed K"*. **`as_1`** (the ffi-pragmatist's `nr_asecond(save: 1)`: a handler left by `siglongjmp`, re-entered by `setcontext` from a coroutine, which then calls the program) prints K: `panic: as1.answer was called from inside a signal handler: C called it while handling a signal, where neither this runtime's memory nor the C library's is safe to use. No function of the program is a signal handler: ...`, **3 of 3 plain and 3 of 3 `--sanitize` on Debian 2.41, today, C2679 and landed** (`programs/compiler-engineer/logs/xo-deb-today.log`, `xo-deb-C2679.log`, `deb-landed.log`), and **on Ubuntu 2.43 the same binary prints S in plain and K in `--sanitize`** (`ubuntu-C2679.log`, `ubuntu-landed.log`). The refusal is right (the program IS called by a handler); **which words it prints depends on the C library and on `--sanitize`**, and K has no *"not seen to end"* in it. `grep -l 'C called it while handling a signal' tests/golden/run/*.expected` still prints nothing: no golden pins K.
- **U**: *"none of the adopted build's 361 outcome lines prints U"* was over the first run's logs; over every adopted log of the resumed run too (`grep -h '| U |' cells-C2679-*.log ubuntu-C2679.log deb-landed.log ubuntu-landed.log | wc -l` = **0** of 802 outcome lines (`grep -hv '^DONE\|^aarch64\|^finished'` over the same files)), on both libraries and on the landed runtime. **R2's promise holds on every build I could reach that verifies the decode** (accessor 3); the forced route A (§ 9a) is the one place U survives.

## 11. Unrun, and said so (brief item 11)

- **Windows**: no machine, no image. The frozen and the landed `runtime/parts/sigguard.c` both say, in the `_WIN32` branch, *"Windows: no probe and no walk (panel 247's R10) ... unrun"* and the landed accessor's golden says *"Windows has no route to ask"*: **every clause that says a callback aborts is, by the runtime's own comment, not what the Windows leg does**; I ran nothing there.
- **Linux x86-64** (`ubuntu-26.04`): no image here and I built none. What rests on it: the **`__builtin_setjmp` / `__builtin_longjmp` branch** of the inline jump (`p4.h`'s `#if defined(__x86_64__)`, the golden `fixedbugs-2522-an-inline-jump-out-of-a-handler.h`), which compiles only there (A12); the **calibration's x86-64 verification** of the setjmp key (carried from panel 255's account in the landed golden's header, *not* re-run); the **depth threshold** (4,608 to 5,120 bytes here, which I take to be arm64's signal frame plus `raise`: an inference). The cross-stack finding (§ 10) is an address comparison without a machine-specific part: I **expect** it on x86-64, an inference.
- The CI's **`macos-26`** leg and **Darwin** are the ffi-pragmatist's.
- **`ubuntu-bins.sh` as written** (§ 9): not run; its method was, built by `bins.sh`.
- **libpng 1.6.59** (the Mac's): the image has 1.6.48 (§ 9b).
- A **C++ exception from an asynchronous signal** and **`-fnon-call-exceptions` on the direct form** (§ 3); the shape `jo_co` (does not compile with this clang: `-Walloc-size`).
- A **real route A** (a platform whose accessor reads below 3): none reachable; the forced build (§ 9a) is a stand-in.

## Against the landed runtime (523d35537)

Copy `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/compiler-engineer-landed/` (`git log -1 --format=%H` = `523d35537b68e155841ddc68266fc5148c379b54`, `git status --short` empty before I built; `git diff --stat 828567a30 HEAD -- seed` printed nothing, so the seed is the frozen tree's).
Container `p257-ce-4` (`docker run -d --name p257-ce-4 -v <copy>:/srcL:ro -v <ce-work>:/cew:ro -v <ce-work>/out:/out heroes-linux-arm64:latest sleep infinity`), `scripts/build-landed.sh`.

### L1. FINDING (a blocker, first): the landed tree builds no Heroes program in this Linux image

- **Stage 1 built** (clang on the seed: `stage 1 ok`, `out/landed/build.log`). **Stage 2 FAILED**: `internal error: the platform layer did not compile / compiling the platform layer's hero_ui_print_gtk failed: runtime/platform/hero_ui_print_gtk.c:631:28: error: variable 'chosen' may be uninitialized when used here [-Werror,-Wconditional-uninitialized] ... error: no platform layer` (`logs/build-landed-stage2-FAILED-in-latest-image.log`).
- **It is not a problem of stage 2 only: `heroes build p1.hero` (a program with no UI) fails the same way** with the landed tree's **stage 1** (`/w/landed/heroes`) and with the **frozen tree's stage 2**, `HEROES_RUNTIME=/w/landed/runtime`; and in the other image, `heroes-linux-arm64:gtk4-ui` (the same Debian 13, clang 22.1.8, with GTK 4.18.6), frozen stage 2 and a copy of the landed runtime. So **on Debian 13 aarch64 with clang 22.1.8 the landed tree builds nothing**.
- The file is the UI lane's (`git log --oneline -3 -- runtime/platform/hero_ui_print_gtk.c`: step 105p6 and 105p7), nothing of the signal mechanism. `-Werror=conditional-uninitialized` is the project's own flag (`selfhost/cli/flags.hero:143`). **`rebel-round` at `b9129d41b` has the same line 609** (`git show rebel-round:runtime/platform/hero_ui_print_gtk.c | sed -n 609p`). I do not know why the UI lane's own Linux runs compiled it (a different clang or flags: unrun by me); whether the CI's `ubuntu-26.04` clang (the workflow's comment names 21.1.8, `ci.yml:355`) refuses it is **unrun**.
- **What I did so the landing check could be run**: a copy of the landed runtime with **one initializer**, `hero_ui_print_settings chosen = {0};` at line 609 (`logs/landed-one-initializer.diff`; `diff -r /srcL/runtime /w/landed/runtime` in the container prints that one hunk and nothing else). With it stage 2 built (`stage2 exit=0`, `heroes 0.6.0`, **`sources 25dceefe1c908fb1`**, which includes my patched file; indicative 1,381 s at a load above 100). **Every landed cell below is the patched copy**, whose signal files are byte-identical to the tree's.

### L2. The landed Linux mechanism, by reading (counts by `wc -l`)

`runtime/parts/sigjump.c` 159 lines (it was 283 at the frozen tree), `sigjump_decode.c` 193, `sigjump_probe.c` 163, `sigjump_record.c` 165, `sigjump_save.c` 113 (Linux and every POSIX but Darwin: **793 lines**), `siginstall.c` 362 (**unchanged by the landing**), `sigguard.c` 583 (523 at the frozen tree); Darwin's `rebind.c` 133 and `rebind_{decode,entries,image,install,jump,record}.c` 320, 302, 217, 116, 229, 234 (**1,551 lines**). `HERO_SIGNAL_SWITCHED`'s only setter is `rebind_entries.c:297` (Darwin): **on Linux the new refusal of a context switch is not reachable** (`grep -n HERO_SIG_SWITCHED runtime/parts/*.c`), so **defect 2702 is still what makes Linux meet R4** and the existing words (S) are what Linux prints for `g_3_0`.

### L3. The cells beside panel 255's routes (`-O0`, plain and `--sanitize`, 3 runs a cell; `scripts/landed-bins.sh`, `ubuntu-run.sh`; the Debian cells of `deb-landed.log`, the Ubuntu cells of `ubuntu-landed.log`; the full table, 81 rows, is `logs/table-landed-vs-C2679.md`)

**81 programs, built by the frozen stage 2 with `HEROES_RUNTIME` = the landed runtime and run in both containers: 79 rows read the same as C2679 on the same library, 2 differ and neither is a difference of the runtime** (`b3`: Debian C2679's cell was classified `other` by the first run's script, its words are the T words in both; `cs_1`: the Ubuntu flake of § 10, 18 of 20 under C2679 and plain S, sanitize runs, under the landed runtime). The decisive rows (**`cs2` here is its default ORDER=0; § 10 has ORDER=1**):
| program | Debian 2.41 C2679 | **Debian 2.41 landed** | Ubuntu 2.43 C2679 | **Ubuntu 2.43 landed** |
|---|---|---|---|---|
| `p1` | runs | **runs** | runs | **runs** |
| `p2` | runs | **runs** | runs | **runs** |
| `p3` | runs | **runs** | runs | **runs** |
| `p4` | S | **S** | S | **S** |
| `p5` | runs | **runs** | runs | **runs** |
| `p6` | S | **S** | S | **S** |
| `p7` | runs | **runs** | runs | **runs** |
| `p8` | S | **S** | S | **S** |
| `p9` | runs | **runs** | runs | **runs** |
| `p10` | runs | **runs** | runs | **runs** |
| `gcopy` | runs | **runs** | runs | **runs** |
| `p1l` | - | **runs** | - | **runs** |
| `p1u` | - | **runs** | - | **runs** |
| `p1f` | - | **runs** | - | **runs** |
| `p7s` | - | **runs** | - | **runs** |
| `g_3_0` | S | **S** | S | **S** |
| `g3` | S | **S** | S | **S** |
| `g_1_0` | runs | **runs** | runs | **runs** |
| `g_2_0` | runs | **runs** | runs | **runs** |
| `g_3_1` | runs | **runs** | runs | **runs** |
| `b_0` | - | **runs** | - | **runs** |
| `b_1` | - | **runs** | - | **runs** |
| `cs_1` | runs | **runs** | S (2/3) + runs (1/3) / runs | **S / runs** |
| `c_1` | - | **runs** | - | **S / runs** |
| `cs2` | - | **runs** | - | **runs** |
| `as_1` | - | **K** | - | **S / K** |
| `grefill` | S | **S** | S | **S** |
| `p4s` | runs | **runs** | runs | **runs** |
| `p4x` | runs | **runs** | runs | **runs** |
| `p4sd` | runs | **runs** | - | **runs** |
| `b1` | runs | **runs** | runs | **runs** |
| `b2` | S | **S** | S | **S** |
| `b3` | other(exit 134) | **T** | T | **T** |
| `C12 golden (2494 atexit)` | S | **S** | S | **S** |
| `route accessor golden` | - | **runs** | - | **runs** |

**Also identical to C2679, run with the landed tree's own stage 2** (`/w/landed/heroes-next`, no variant): the ten programs `p1` to `p10` at **`-O0` and `-O2`, plain and `--sanitize`, 3 of 3 each** (`cells-landed-p.log`: `diff` of its 40 lines against `cells-C2679-p.log`'s, variant names stripped, is empty); **the depth sweep `p4d`** (PAD 0 to 65536, 22 cells: `diff` against `pad-C2679.log` and `pad-today.log` empty: runs to 4,608 bytes, aborts from 5,120); **`p4e`'s boundary** (`padE-landed.log` equals `padE-C2679.log`: runs at D = -128 (`-O0`) and -48 (`-O2`), aborts from -96 and -16); **C5** (`c5-landed.log` equals `c5-C2679.log` but for its last line: direct form `terminate`, pointer form caught, 64 KiB deeper S); **`cs2`** under the four ORDER/HOW pairs and **`tal`** under both orders (`cs2-debian-landed.log`, `cs2-ubuntu-landed.log`, `tal-*-landed.log`): ORDER=1 aborts S for both HOW, ORDER=0 and `tal` run, on both libraries.
**The accessor** (`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`, `hero_signal_jump_route()`): prints **`route 3`**, 3 of 3, plain and `--sanitize`, **on Debian 2.41 and on Ubuntu 2.43** (`deb-landed.log`, `ubuntu-landed.log`), the golden's own expected text. Route 3 is *"both"* (setjmp family by the key, contexts by their field), the same as C2679's `decoding: 3` on both libraries.
**C12**: the 2494 golden: S, exit 134, first stderr line `028f3474...` (the same bytes as today's and C2679's, `md5sum` of `./<bin> 2>&1 >/dev/null | head -1`: three runs in the Debian container and one in the Ubuntu one all read `028f347456d4f3fb99c1fee109f1e0f4`); its cells in `deb-landed.log` and `ubuntu-landed.log` (plain and `--sanitize`, 3 of 3, exit 134, S) are in the table above.
**libpng**: `png.sh` with `VS=landed` in the `gtk4-ui` image (libpng 1.6.48): see below the gates.

### L4. The landed tree's own gates, and libpng

`scripts/gate.sh landed <filter>` (the harness's own `run`, the landed stage 2, `out/gate-landed-*.log`):
| gate | the harness's line |
|---|---|
| `fixedbugs-2522` | **`harness: 25 passed, 0 failed`** (the adopted C2679 tree, with the frozen goldens, read 23 and 2: the lane has moved the two goldens R2 moves, and the landed runtime passes them) |
| `fixedbugs-2679` | `harness: 9 passed, 0 failed`, **one SKIP**: `fixedbugs-2679-libpng-decodes-a-truncated-image-then-a-good-one` (*this machine lacks what the case binds ... the package `libpng`*): judged where libpng is, below |
| `fixedbugs-2494` | `harness: 15 passed, 0 failed` (today and C2679 read the same, § 8) |
`png.sh` with `VS=landed` in the `gtk4-ui` image (libpng 1.6.48; `out/png-landed.log`), frozen stage 2 and the patched landed runtime: `png` at `-O0` and `-O2`, plain and `--sanitize`, `0 68 3072 -1 3072`, `pngdemo` `-O0` plain and `--sanitize`, `good 64048 truncated -3 good again 64048 truncated again -3`: **3 of 3 each, exit 0, 6 cells**. (Today's runtime fails the same `--sanitize` cells in ASan: § 9b.)

### L5. Each clause of w26 and w27 against the landed runtime, on Linux arm64 (the full table with every cell is `ce-clauses.md` §§ 1 and 2)

Every cell of L3 equals C2679's, so **the verdict of each clause is the same for the landed runtime as for the adopted build**:
| clause | w26 | w27 | landed, Linux | the cell |
|---|---|---|---|---|
| *ends by its return* | yes | yes | **true** | p5 |
| *by a `longjmp`, `siglongjmp`, `_longjmp` out of it* (the callback then runs) | yes | yes | **true on the thread's own stack, false across two stacks the program made** | p1, p1l, p1u, p1f, p2, p3, p9, p10 run; **`cs2` ORDER=1: S**, both libraries |
| *by a `setcontext` out of it* | yes | yes | **true on the thread's own stack, false across two made stacks** | p7s, p10c run; `cs2` ORDER=1 HOW=ctx S; `cs_1`, `c_1` S on glibc 2.43 |
| *left by an inline jump it is still taken to be running* | yes | yes | **true** (at depth) | p4 S; p4s runs, which *"may abort"* allows |
| *a `swapcontext` away from it ... still taken to be running* | yes | **no** | **false until 2702** | p7, `g_1_*`, `g_2_*` run; `g_3_0`, `g3` S |
| *a jump onto another stack given to `makecontext` ... still running* | yes | **no** | **false as a rule** (true for the two form-3 programs) | `g_3_1`, `g_2_*`, `b_0`, `b_1`, `fp3`, `as_0` run |
| *a callback called later on its thread from deeper than the handler was may abort* | yes | yes | **true** (to within one handler frame) | p4d, p4e, b1, b2 |
| *saying the handler was not seen to end* | yes | yes | **true of S, false of K** (`as_1`: *"called from inside a signal handler"*) | p4, as_1 |
**w27 holds on Linux wherever w26 holds and is silent where w26 is false (G), so it is the safer of the two; both are false in the second row (and third) across two made stacks.** The repair is R-1 and R-3 of `ce-clauses.md`, run there.

## Verdict (the structure of `.claude/agents/compiler-engineer.md`)

- `verdict`: **object** (no veto). w26 and w27 each carry a clause a running program falsifies on Linux; the repaired w27 (R-1 and R-3 of `ce-clauses.md`) I would approve. The sentence adds no core construct and breaches no ceiling, so the veto is not exercised.
- `section`: design.md **§ 1.12** (*"a Heroes program must not segfault and must not corrupt memory"*: the refusals the sentence describes exist for it, and a description of them a program contradicts is not the boundary being *"complete and defended"*) and **§ 4.17** (the words an error prints must be true: K and U and S differ by library and by flag); for the ceiling § 1.1 and § 1.7. design.md has **no section on the spec's sentence about a handler**; the text it extends is spec lines 427 and 428 (§ 13), and the rule of its shape is `.claude/rules/spec-shape.md` (not design.md).
- `implementation_cost`: **the sentence: 0 lines of compiler** (spec prose, erased before the checker, the lowering and the backend). What it binds: `selfhost/emit/callback_thunk.hero` (375 lines; its test at line 354 holds the spec's tail to `HERO_SIGNAL_REMEDY`), `runtime/parts/sigguard.c` (583 lines, the constant), 22 `run` goldens. **The mechanism it describes**, at `523d35537`: `sigjump.c` 159 + `sigjump_decode.c` 193 + `sigjump_probe.c` 163 + `sigjump_record.c` 165 + `sigjump_save.c` 113 (Linux, 793) + `siginstall.c` 362 + `sigguard.c` 583 + Darwin's `rebind*.c` 1,551 = **3,289 lines of runtime C** (`wc -l` of those 14 files), under Pascal-P4's ~4,000: it fits one person, and the sentence adds none of it. **Repairing § 10 in the runtime** (compare two entries only when both stacks are known, `siginstall.c:150-160` and `:212-221`, a 362-line file identical in the frozen and the landed tree): about 10 to 15 lines **estimated, not written**.
- `needed_for_self_hosting`: **no** (Principle 0): the compiler does not call a C handler; it is robustness (§ 1.12) and the thesis's own claim.
- `argument` (≤120 words): The sentence is true where the runtime sees a jump on the thread's own stack and false where it does not: a handler that leaves by `siglongjmp` or `setcontext`, correctly, between two stacks the program made (`cs2` ORDER=1; panel 255's `c_1` and `cs_1` on the CI's glibc 2.43) aborts its next callback, today, adopted and landed alike, because `hero_sig_prune` compares raw addresses of stacks it does not know. So *"ends by ... out of it"* is false in a corner a distribution's allocator moves. Say *"may abort"* with the condition, scope the context clauses to 2702, add K to the words, and say Windows has none.
- `prediction`: if w26 or w27 lands unrepaired, the first `run` golden written for the case, `cs2` ORDER=1 (`programs/compiler-engineer/mine/cs2.h`), **reads S** while the sentence says the callback runs, at the sentence's landing (the lane, after the author's ratification), unless `hero_sig_prune`/`hero_sig_inside` (`runtime/parts/siginstall.c:150-160`, `:212-221`) change first; **checkable at that landing**: `ORDER=1 HOW=jmp ./bin-cs2` exits 134 under any runtime whose `siginstall.c` is the frozen file (`diff` of the frozen and the landed file is empty).
- `condition`: I approve w27 repaired if (a) `cs2` ORDER=1 and `c_1` on glibc 2.43 run under the lane's runtime, **or** the sentence carries R-1's scope; (b) the words clause names K or promises S only; (c) the context clauses carry *"until 2702"* or are left to w27; (d) a Windows scope is stated; and (e) the landed tree builds in a Linux image the CI uses.

## One line per numbered question of the brief (the answer; the one measurement that would change it)

1. **The table, C1 to C9, Linux, today and adopted** (§ 5, § 9): C1 holds (12 goldens, p1, p1l, p1u, p1f, p7s); C2 holds adopted and landed (today U, exit 139, ASan); C3 holds adopted and landed; C4 S **at depth only** (p4s runs); C5 runs shallow, S deep; C6 S; C7 splits (p7 runs, `g_3_0`, `g3` S; Darwin refuses); C8 runs; C9 S true, K, T, D other, U route A only; and **a new row false on both: `cs2` ORDER=1 / `c_1` on glibc 2.43 (§ 10)**. *Changes if*: `siginstall.c:150-160` stops comparing unknown stacks, and `cs2` ORDER=1 runs.
2. **p1 to p10**: all 20 cells (today, adopted) equal the critic's key; the landed runtime's equal the adopted build's (`diff` empty over 40 cells at `-O0`/`-O2`, plain/`--sanitize`). *Changes if*: a cell reads otherwise on a quieter machine (none did in 3 runs; `cs_1` is the only flaky binary in 106, of the C library).
3. **C5**: the direct form is not correct C++ (`terminate`, no Heroes in the control); a pointer to a non-`noexcept` call catches outside the handler; only the callback 64 KiB deeper aborts (S). *Changes if*: a clean statement of the rule that makes a throw out of a handler correct, or an asynchronous-signal run.
4. **"a callback called while ..."** (§ 4, § 4c): (a) never called again: exit 0 (and an `atexit` function runs at the natural end); (b) another thread: runs (b1, b2); (c) a second handler: S, because *that* handler is running. *The next callback aborts* is true of one called by a handler or from deeper, on that thread. *Changes if*: a started thread's call aborts.
5. **C6, the refill**: **none found refused** (p10, `p10h`, `p10a`, `p10c`, `p10d`, p3, `gcopy`, `x_refill_out`, `x_realloc`: all run, adopted and landed); the refused ones land inside the handler. *Changes if*: a program whose installed stack pointer is below the handler's wrapper while the handler is over.
6. **C7, G**: adopted and landed refuse **2 of the 12 G programs** (`g_3_0`, `g3`) and run 10; **the clause false on Linux until 2702 is the context clause** (f1, f2, w26's `swapcontext`). *Changes if*: 2702's lane lands and p7 reads S on Linux.
7. **The words**: S: *"had not been seen to end"*, true; U: 0 of 802 lines on any verified decode, only on the forced route A; **K** appears (`as_1`) without it; T, D are other rules. *Changes if*: a real platform reads `decoding` below 3.
8. **Each arm's clauses**: `ce-clauses.md`: **false**: the cross-stack clause (w26, w27, f2, e1), the context clause (w26, f1, f2), the unqualified *"aborts"* (f1, f1b, f2, f3, e1 to e3), *"refilled"* (e1), the K words; **true**: the rest. *Changes if*: the runtime's compare is fixed or the sentence takes R-1.
9. **The CI's library**: `decoding: 3` on Ubuntu 26.04 glibc 2.43, `route 3` for the landed runtime; 105 of 106 binaries read the same as on glibc 2.41, the exception `cs_1` (18 of 20 abort): the allocator puts a 256 KiB `malloc` in the `brk` heap. *Changes if*: the CI's x86-64 leg reads other.
10. **C12**: S words, byte-identical today and adopted and landed (`028f3474...`), gate 15 and 0 on all three. *Changes if*: the golden's words move at the landing.
11. **Unrun**: Windows; Linux x86-64; `macos-26`; the literal `ubuntu-bins.sh`; libpng 1.6.59; asynchronous C++ throws (§ 11). *Changes if*: any of them is run.

**Containers**: `p257-ce-1` to `p257-ce-4` removed at the end (`docker ps -a | grep p257` empty, below).

**Cleanup, 2026-11-23 (`date` read 15:5x)**: `docker rm -f p257-ce-1 p257-ce-2 p257-ce-3 p257-ce-4` printed the four names; `docker ps -a | grep p257 | wc -l` printed **0**. `pgrep -f 'build/'` on the Mac listed three processes, none of them mine (two `zsh -c` wrappers and a `build/<key>/main` of another session's harness: `ps -o args -p` read them); every program of this report ran inside a container, now removed. **One slip, corrected**: the resumed run wrote six scratch files into the Mac's `/tmp` (`xo-deb-C2679.txt`, `xo-ub-C2679.txt`, `xo-deb-today.txt`, `xo-ub-today.txt`, `table2.md`, `decisive.md`), outside the repository's root, before it noticed; their contents are in `programs/compiler-engineer/logs/` and the six files are **removed** (`ls` of them prints *no such file*). Nothing else was written outside the root (the scratch copies are under `.claude/worktrees/p257-scratch/`, the containers' files in the containers); nothing was committed, pushed or staged; no credential was printed and no paid run was made.
Files of this seat: this report; `ce-clauses.md` (each clause of the arms, true / false / untested, and the repaired clauses R-1 to R-8 with their runs); `programs/compiler-engineer/` (`MANIFEST`, `run.sh`, the programs, `scripts/`, `logs/`).
