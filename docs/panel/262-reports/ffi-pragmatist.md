# panel 262, ffi-pragmatist: report (started 2026-11-26 21:46)

Written as it goes; a section marked *in progress* is not finished. The machine read load 37 to 105 at the start
(`uptime`, 21:47 to 21:53), so **no duration here is a measurement**.

## 0. Ground

- **My copy**: `.claude/worktrees/262-ffi-pragmatist`, detached at `87003cb42946dc0f99dbab36582693c4afb3777a`
  (`git log -1`). Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `heroes --version`:
  **`sources 1e1eb23a1927d35b`** (as `00-shared.md` says). Stage 2: `./heroes build selfhost/main.hero -o heroes-next`,
  **`sources 6ce3f9e116d22e3a`** (§ 2). Log: `programs/ffi-pragmatist/build.log`.
- **`main` moved after the base** (`git log --oneline 87003cb42..main -- runtime spec`, 21:47): step 203f (panel
  261's spec text), **step 199e** (`01706218e`, defect 2853) and **step 199f** (`8d8263f15`, *"the signal guard's walk
  of frame records ends at the top of the coroutine's stack it runs on, so a fibre's callback is not refused for a
  delivery left above it"*: the lane's G, defect 2852). `main` is **`0ce5b0cf336e906e69313cf8b8484d347010f882`**
  (21:54:29); `git diff --stat 87003cb42 main -- runtime`: `rebind_entries.c` +9, `rebind_jump.c` +27 −1,
  `sigentries.c` +8, `sigguard.c` +21 −2, `sigjump_save.c` +6. **Second column**: `git archive main runtime` into
  `main-runtime/` of my copy, built as `HEROES_RUNTIME=main-runtime/runtime`.
- **Programs**: `docs/panel/262-reports/programs/ffi-pragmatist/` (below). Every binding header was compiled by clang
  against the real headers before any build (`clang -std=c11 -Wall -Wextra -fsyntax-only -x c <h>`; aj.h with
  `-I/opt/homebrew/opt/openssl@3/include` and `@4`, uvq.h with `pkg-config --cflags libuv`): **every one accepted**,
  the warnings unused-function (static inline in a header) and, on Darwin, `getcontext`/`makecontext`/`swapcontext`
  *deprecated* (`sw.h`, `lj.h`, `gt.h`). Every `.hero` passed `heroes check` (the seed) and `heroes fmt --in-place`.

## 1. The programs (one binary each; the row is the environment its C reads)

| program | the library or shape | its C |
|---|---|---|
| `bx` | Boost.Context 1.92 `fcontext` (this Mac only) | `bx.h`: the library's assembly switch, two coroutines, `malloc` stacks or carved (`P262_CARVE=1`) |
| `aj` | OpenSSL `ASYNC` jobs (`aj3`: 3.5.9, `aj4`: 4.1.0 on this Mac; the image's 3.5.7) | `aj.h`: `ASYNC_start_job`/`ASYNC_pause_job`, the library's own 32 KiB stacks (`lib`), 256 KiB of `malloc` (`heap`) or carved (`carved`) through `ASYNC_set_mem_functions` |
| `sw` | a hand-written `swapcontext` scheduler | `sw.h`: `makecontext` per coroutine, `swapcontext` every switch |
| `lj` | a hand-written `longjmp` fiber (OpenSSL's design on this Mac) | `lj.h`: made by `makecontext`, entered once by `swapcontext`, every switch `_setjmp`/`_longjmp` |
| `sst` | the signal stack trick, four variants (`P262_SST`: RR, RL, JR, JL) | `sst.h`: below, § 4 |
| `gt` | panel 257's `sched`, a green-thread scheduler preempting by `SIGALRM`, header-only | `gt.h`: the handler `swapcontext`s to the next green thread |
| `uvq` | libuv 1.53.0 (this Mac only) | `uvq.h`: `uv_signal_start` (`signal`), `uv_queue_work` with the program's work callback (`work`), with C's work and the program's after callback (`after`) |
| `pt` | callbacks on threads the C library made | `pt.h`: `inline` (control), Linux `timer` (`SIGEV_THREAD`), `mq` (`mq_notify`), `gai` (`getaddrinfo_a`); Darwin `dispatch` (`dispatch_async_f`) |
| `cpo_heap`, `cpo_host` | panel 259's stack-copying library (greenlet's and libco's design) | 259's `cpo.h`, `own.h`, unchanged |
| `bfc_host`, `bfc_heap` | panel 259's Boost.Context bindings | 259's `bfc.h`, unchanged |
| `h9` | the blind key's `h9` (a handler left by assembly on one thread, the callback on another) | the registration's `h9.h` |

The handler beside each coroutine library is `sigmode.h`'s (`P262_SIG`): **0** none of C's (the runtime's own stands);
**1** a SIGUSR1 handler of C's installed, never fired; **2** fired on this thread before the library runs, returns;
**3** fired on a thread C made (`pthread_kill`), returns; **4** fired on this thread from inside a coroutine, on its
stack, returns. Every coroutine then has C call a function of the program (`sm_call(twice, ...)`) after its switches.
Every mode is correct C.

## 2. Results

**Builds**: stage 2 at the base reads **`sources 6ce3f9e116d22e3a`** (`heroes-next --version`, 22:01), as
`00-shared.md` says; seed `real 114.07 user 22.41`, stage 2 `real 718.98 user 185.16 sys 36.93` (`/usr/bin/time -p`,
load 37 to 105: discarded as durations, the ratio). Every cell: `cells.sh <compiler> <out>` from the programs folder,
each program built plain and `--sanitize`, each row run 3 times under `timeout 60`; summaries in `mac-base.txt`
(this Mac, the base's runtime), `mac-main.txt` (`main`'s runtime), `linux-tree/P/linux-base.txt` and `linux-main.txt`
(the container, `--network none`), every output kept as `o-<prog>-<build>-<row>-<r>.txt`.

### 2.1 This Mac, the base's runtime

- **Boost.Context 1.92 `fcontext`** (`bx`): **every row runs**, 60 of 60: modes 0 to 4, heap and carved, plain and
  `--sanitize`, 3 of 3 each, `coroutines 22 25`, `result 54`, exit 0, the three outputs equal. A handler of C's that
  fires **on the fiber's own stack** and returns (mode 4) changes nothing. Panel 259's `bfc_host` and `bfc_heap`:
  run, 12 of 12 (`coroutines 30 32`).
- **The hand-written `swapcontext` scheduler** (`sw`): heap, every mode and both builds, run. **Carved, `--sanitize`:
  exit 134, 15 of 15 (modes 0 to 4), AddressSanitizer reporting `stack-buffer-overflow ... READ of size 8` in the
  runtime's own `hero_callback_asked sigguard.c:707` called from `hero_callback_entered sigguard.c:660` from the thunk**
  (`mac-base/o-sw-san-sig0carve1-1.txt`): the guard's walk of frame records reading a redzone, **with no handler of C's
  at all (mode 0)**. Defect 2852's cause (the walk climbing gone frames of a carved fibre) in **another symptom**: not
  the sentence's words but ASan's, which 2852's body (`gh issue view 2852`) does not name. Plain: runs.
- **The `longjmp` fiber** (`lj`): heap runs on both builds (ASan's *"False positive error reports may follow"* warning
  first). **Carved, `--sanitize`: exit 134 in CALLED's words, *"lj.run was called from inside a signal handler: C
  called it while handling a signal"*, mode 0 included**: defect 2852's own shape (`mj_carved`'s, written afresh).
- **The signal stack trick** (`sst`, mode 0: **no handler of C's but the library's own creation handler**), 3 of 3,
  both builds, the same each time:
  - **RR** (the handler **returns** after its save, the library **restores** the alternate stack that stood before,
    which is the runtime's): **runs**, `coroutines 22 25`, `result 54`, `alternate stack now a coroutine's: 0`,
    `called back on the thread: 6`, exit 0. With a handler of C's fired inside a coroutine (`RR-sig4`) or on a thread C
    made (`RR-sig3`): runs. **§ 9 after it** (`RR-deep`, a recursion of the program on the thread after the
    coroutines): plain `panic: stack exhausted in sst.deep`, exit 134 (the runtime's own handler, on its own alternate
    stack again); `--sanitize`: ASan's `AddressSanitizer:DEADLYSIGNAL`, exit 134 (ASan owns SIGSEGV there,
    `HERO_STACK_GUARD_YIELDS_TO_ASAN`).
  - **RL, JR, JL**: **exit 134 at the first callback on a coroutine's stack, in REENTERED's words**: *"sst.run was
    called after C jumped into stack frames the thread had already left (a handler left by a jump and jumped back
    into, or frames only a signal's delivery puts on the alternate stack): the handler is held to be running again,
    and the function to be called from inside it, ..."* + the remedy (`mac-base/o-sst-RL-1.txt`). RL by rule 6 (a
    jump landing on the alternate stack registered now, from outside it); JR and JL by step 199d's span (a jump
    landing inside the frames of a handler a jump left). The `-deep` rows abort there too, before § 9 is reached.
  - **So every coroutine of a signal-stack-trick library starts inside a handler's frames, and the runtime lets it
    run only where the handler returned AND the library gave the alternate stack back.** Which of the four a real
    library is waits on the historian's sources (§ 4).
- **The `SIGALRM` green-thread scheduler** (`gt`, panel 257's `sched`): **exit 134 in SWITCHED's words**, 6 of 6:
  *"gt.sum was called while a signal handler of C's that this runtime saw begin had switched to another context
  (`swapcontext`, or `setcontext` into a context `makecontext` ...)"*, as panel 257 found (`:134`).
- **OpenSSL `ASYNC`** (`aj3`: `OpenSSL_version_num()` 810549392 = 0x30500090, **3.5.9**; `aj4`: 1074790400 =
  0x40100000, **4.1.0**; built with `PKG_CONFIG_PATH=/opt/homebrew/opt/openssl@<v>/lib/pkgconfig`): **every row
  runs**, 84 of 84 per version: the library's own stacks, 256 KiB of `malloc`'s, carved, modes 0 to 4, plain and
  `--sanitize`, `paused 1 1, jobs 1022 1025`, exit 0.
- **The stack-copying library** (259's `cpo_heap`, `cpo_host`): run, 12 of 12 (`coroutines 30 32`).
- **The signal stack trick with C-only coroutines** (`sst-extra.sh`, `mac-base-sst.txt`: `P262_CONLY=1`, no function
  of the program ever on a coroutine's stack, then `sst_altstack()`, then C calling the program's `twice` **on the
  thread's own stack**, then `P262_DEEP` 0 or 1), 3 of 3, both builds:
  - **RR**: `coroutines 0 3`, `alternate stack now a coroutine's: 0`, `called back on the thread: 6`, exit 0; deep:
    `panic: stack exhausted in sst.deep` plain, `AddressSanitizer:DEADLYSIGNAL` under `--sanitize`, exit 134.
  - **RL and JL**: `alternate stack now a coroutine's: 1` (the library left its own; the runtime's is gone from this
    thread), **then the callback on the thread's own stack, after every coroutine has ended, exit 134 in REENTERED's
    words** (*"sst.twice was called after C jumped into stack frames the thread had already left ..."*). **JR**:
    `alternate stack now a coroutine's: 0`, and the same refusal. So § 9's abort after an L variant is **unreached**
    (the refusal comes first); what the runtime's SIGSEGV handler does on a library's stack stays **unrun**.
  - **The finding**: under RL, JR and JL the thread is held inside a handler **for the rest of its life**: every
    later callback on that thread aborts, on any stack, long after the library's coroutines are gone. A text that
    scopes the abort to *"a callback on a coroutine's stack"* or *"while a coroutine runs"* is **false** for these
    three; a text that scopes it to *"after C left a handler of C's ... on its thread"* is true.
- **libuv 1.53.0** (`uvq`): **`signal` runs** (`answer ran for 30`, `result 60`: the callback called from `uv_run` after
  libuv's own handler returned), 6 of 6; **`work` exits 134 in the thread's words**, *"uvq.answer ran on a thread this
  program did not start: C called it back from a thread of its own, where this runtime's values are not safe. To run
  work on another thread, start it with `hero_thread_spawn` ..."*, 6 of 6; **`after` runs** (C's work on the pool,
  the program's function as the after callback on the loop's thread: `answer ran for 21`, `result 42`), 6 of 6.
- **Callbacks on a thread the C library made, Darwin** (`pt`): `inline` runs; **`dispatch` (`dispatch_async_f` on a
  global queue) exits 134 in the thread's words**, 6 of 6. The program's C never calls `pthread_create`.
- **curl's synchronous resolver timeout written by hand** (`cu_alarm`, `P262_BLOCK=pipe`: `alarm(1)` around a read
  nobody answers, the SIGALRM handler of C's `siglongjmp`s out of `read`): **runs**, `answer ran for 7`, `result 141`
  (jumped, then the callback), 6 of 6. The signal lands in the C library's `read`, never in the runtime.
- **`h9`** (a handler left by assembly on one thread, the callback on another): **runs**, `left on the thread: 1`,
  `answer ran for 7`, `result 70`, 6 of 6, as the repair's key says.
- **The same `SIGALRM` scheduler with C's bodies** (`gtc`, `gtc.h`: the handler still `swapcontext`s between green
  threads every 5 ms, no function of the program on a green thread, then C calls the program's `twice` on the
  thread's own stack after `gtc_run` returns; `gtc-run.sh`, `mac-base-gtc.txt`): **runs**, `preempted true`, `thread 0
  450000015000000`, `called back after: 6`, exit 0, 6 of 6 (plain and `--sanitize`). **So "C left a handler by a
  `swapcontext` away from it, then a callback" runs where every switch came back before the callback**: a text that
  turns `now`'s *may abort* into *aborts* for that exit (d3's stricter runtime, (d8)'s abort at the exit, any *"aborts"*
  without *"never switched back"*) is **false for `gtc`**, and (d8) would refuse at its first preemption a program
  that never puts a function of the program inside a handler.
- **`heroes doctor`** at the base (`heroes-next doctor`, this Mac): clang, its floor, Xcode's tools, swiftc, the arch,
  the runtime, the archiver, the build cache: **no line names the jump route**. (d9) as the critic words it (*"which
  `hero_signal_jump_route()` and `heroes doctor` name on a machine"*) is false of `doctor` at the base.

### 2.2 This Mac, `main`'s runtime (`0ce5b0cf3`, steps 199e and 199f in), the second column

`HEROES_RUNTIME=main-runtime/runtime sh cells.sh ...` (`mac-main.txt`), the same compiler. Every cell's verdict, as
`<count> <build> <row> <exit>`, in `mac-base.cells` and `mac-main.cells` (`grep ... | sort | uniq -c`), compared by
`diff`: **the only cells that differ are the ten carved `--sanitize` rows of `sw` and `lj`** (modes 0 to 4 each),
**exit 134 at the base, exit 0 on `main`**, 30 of 30 runs: step 199f (the lane's G, defect 2852) repairs both
symptoms, CALLED's words on the `longjmp` fiber and ASan's report inside the walk on the `swapcontext` scheduler.
Every other cell reads the same; the C-only signal-stack-trick rows (added to `cells.sh` for this column) read as
`mac-base-sst.txt`'s deep rows did at the base, 12 of 12 per build. `gtc` on `main` (`mac-main-gtc.txt`): plain 3 of 3 run;
**`--sanitize` 2 of 3 run, the first run exit 134 with no output at all** (0 bytes, `Abort trap: 6`), so a rate was
measured (`gtc-rate.txt`, the same two binaries, runs 4 to 320 each, beside the other work: a rate, not a timing):
**`main` 2 of 323 exit 134, both with 0 bytes of output; the base 2 of 323 exit 134, both `AddressSanitizer: SEGV on
unknown address 0x0000000006f0 ... READ ... #0 in hero_rebind_swapcontext+0xf00`** (`mac-base-gtc/fail-168-exit134.txt`
and `fail-318-...`, the same trace but addresses). **A correct C program segfaults inside the runtime's own rebinding
of `swapcontext`** (Darwin's, `runtime/parts/rebind_jump.c`'s family), at about 1 run in 160, under `--sanitize`, with
no function of the program inside the handler; on `main` the same rate ends with no words at all. **A §1.12 matter
(*"a Heroes program must not segfault"*), not a text's**, filed by nobody (no GitHub write is mine). **Plain, this Mac**
(`gtc-prate.txt`): base 200 of 200 and `main` 200 of 200 exit 0. **Linux arm64** (a third container,
`linux-rate.sh`, `linux-tree/P/linux-gtc-rate.txt`, the base's binaries): plain 100 of 100, `--sanitize` 100 of 100
exit 0. So the failure is **Darwin's `--sanitize` alone**: 4 of 646 runs there (both runtimes), 0 of 406 plain on this Mac
(3 + 3 + 200 + 200), 0 of 212 on Linux arm64 (the cells' 12 and the rate's 200) (`rebind_jump.c:236-239` by `atos` of the
fault's pc and its return address: the loop over `own->entries` after the real `swapcontext` returns). Its cause is
**unread**; that the timer, armed before `gtc_run`'s own `swapcontext`, can land inside the runtime's wrapper and
re-enter it from the handler is an inference, not a trace. POSIX does not make `swapcontext` async-signal-safe (panel
257's readers said so, 9 of 15), so whether `gtc` is *correct C* in the strict sense is the same question `sched` was.

### 2.3 Linux arm64 (`heroes-linux-arm64:latest`, `--network none`), the base's runtime

The container (`linux-tree/linux-run.sh`, one at a time, `--rm --pull never`, named `p262-ffi-pragmatist`): Debian
glibc 2.41, Debian clang 22.1.8 (`L-env.txt`); the seed `sources 1e1eb23a1927d35b`, stage 2 **`sources
6ce3f9e116d22e3a`** (`L-build.txt`), the same keys as this Mac's. **No Boost, no `uv.h` in the image**: `bx`, `bfc_*`
and `uvq` are **unrun** on Linux (the shared brief's table; `cells.sh` skips them). `linux-base.txt`, 106 cells, **every
cell's three runs agreeing** (`uniq -c`: 106 cells of count 3; this Mac's 132 and 140 likewise):

- **Run**: the `swapcontext` scheduler and the `longjmp` fibre, heap **and carved, plain and `--sanitize`**, modes 0 to
  4 (Linux never had 2852); **OpenSSL `ASYNC` 3.5.7** (`OpenSSL_version_num()` 810549360 = 0x30500070), every stack and
  mode, both builds; the stack-copying library; the signal stack trick **RR**, `RR-sig3`, `RR-sig4`; `gtc`; curl's
  timeout by hand over **both** a pipe and **a resolver that never answers** (`P262_BLOCK=dns`: `getaddrinfo` on a name
  sent to a UDP listener on 127.0.0.1:53 that reads and never replies, `alarm(1)`, the handler `siglongjmp`s out of
  glibc's resolver: `result 141`, jumped then called back); `h9`.
- **The image's real libcurl 8.14.1** (`cu`): `curl_version_info()` says **`CURL_VERSION_ASYNCHDNS`: it resolves on a
  thread of its own** (`resolves on a thread of its own: 1`), so `CURLOPT_TIMEOUT_MS` 1500 with `CURLOPT_NOSIGNAL` 0
  times out with **no signal at all** (`curl 128`: `CURLE_OPERATION_TIMEDOUT` 28 + 100, the progress callback, a
  function of the program, having run on this thread, `answer ran for 1`), exit 0, 6 of 6. **So the critic's recalled
  shape, `alarm` and `siglongjmp` around `getaddrinfo`, is not what this libcurl does**; a libcurl built with the
  synchronous resolver is on no disk here: that cell is **unrun with the real library**, and run by hand (above).
- **Refused, as on this Mac**: `gt` (SWITCHED); the signal stack trick RL, JR, JL (REENTERED, the C-only rows too:
  every later callback on the thread); and **the three POSIX notifications on a thread glibc made, every one in the
  thread's words**, 18 of 18: `timer_create` with `SIGEV_THREAD`, `mq_notify` with `SIGEV_THREAD`, `getaddrinfo_a`
  with `SIGEV_THREAD` (*"pt.answer ran on a thread this program did not start: C called it back from a thread of its
  own ..."*). The program's C never calls `pthread_create`.
- **§ 9 after RR**: plain *"panic: stack exhausted in sst.deep"*, exit 134; `--sanitize` ASan's *"stack-overflow ... in
  h_sst_deep"*, **exit 1** (Darwin's ASan: exit 134).

### 2.4 Linux arm64, `main`'s runtime

The same container, `HEROES_RUNTIME=/w/main-runtime/runtime` (`linux-main.txt`, 21:42 to 21:52 UTC): **106 cells,
every cell's three runs agreeing, and `diff linux-base.cells linux-main.cells` empty**: on Linux arm64 steps 199e and
199f change no verdict of mine. No container is left (`docker ps -a | grep p262-ffi`: none).

### 2.5 The signal stack trick as QEMU and libcoro send it, Linux arm64

A second container (`linux-extra.sh`, `linux-base-sst-s.txt`, `linux-main-sst-s.txt`), both runtimes, both builds,
3 of 3 each: **RRs runs** (the program's coroutines, and C's then a callback on the thread), **RLs refused** in
REENTERED's words, as on this Mac. No container left.

## 3. The table: what ran, and what each text tells a binding author (this Mac; Linux in § 2.3)

Verdicts are 3 of 3 per build unless said; *R* runs, *S* refused in STANDING's words, *SW* SWITCHED's, *RE*
REENTERED's, *C* CALLED's, *T* the thread's, *A* ASan's report. A cell of a text reads **run** (the text promises a
run), **may** (the text says it may abort), **silent** (the text says nothing a reader can apply), or **FALSE** (a
promise the run breaks). `now` is the sentence as it stands, read with *handler* a signal handler and its five exits
as listed.

| shape | handler of C's | base, plain / `--sanitize` | `main` | `now` |
|---|---|---|---|---|
| Boost.Context `fcontext`, heap and carved | 0 to 4 | R / R | R / R | silent |
| OpenSSL `ASYNC` 3.5.9 and 4.1.0, three stacks | 0 to 4 | R / R | R / R | silent |
| `swapcontext` scheduler, heap | 0 to 4 | R / R | R / R | silent |
| `swapcontext` scheduler, **carved** | 0 to 4 | R / **A** (2852's cause) | R / R | silent |
| `longjmp` fiber, heap | 0 to 4 | R / R | R / R | silent |
| `longjmp` fiber, **carved** | 0 to 4 | R / **C** (2852) | R / R | silent |
| stack copying (`cpo`), heap and carved | none | R / R | R / R | silent |
| signal stack trick **RR** | the library's, returned | R / R | R / R | may (fifth exit: a jump lands within it) |
| signal stack trick **RL, JR, JL** | the library's | **RE** / **RE**, and every later callback on the thread | the same | may |
| `SIGALRM` green threads, the program's bodies (`gt`) | the scheduler's, `swapcontext` in it | SW / SW | SW / SW | may (second exit) |
| the same, C's bodies, a callback after (`gtc`) | the same | R / R (`--sanitize`: 2 of 323 SEGV inside `hero_rebind_swapcontext`, § 2.2) | R / R (`--sanitize`: 2 of 323 exit 134, no words) | may |
| libuv `uv_signal_start` | libuv's, returned | R / R | R / R | silent |
| libuv `uv_queue_work`, the program's work callback | none | **T** / **T** | T / T | **silent** (no sentence names it) |
| libuv `uv_queue_work`, C's work, the program's after callback | none | R / R | R / R | silent |
| `dispatch_async_f` | none | **T** / **T** | T / T | **silent** |
| curl's timeout by hand, `siglongjmp` out of `read` | the program's C's, left by `siglongjmp` | R / R | R / R | silent |
| `h9`, assembly out on one thread, callback on another | left by assembly, another thread | R / R | R / R | may (no *"on its thread"*) |

**`now` is false nowhere** in this table (it promises nothing), and **silent on the two aborts a binding author is
most likely to meet with no signal in sight**: the thread's (libuv's pool, libdispatch; on Linux `SIGEV_THREAD`, § 2.3)
and, at the base only, 2852's (a no-handler fibre on a carved stack under `--sanitize` on Darwin, in CALLED's words or
ASan's).

## 4. The signal stack trick: what the runtime does to it, and which variant a real library is

- **What each variant does to the runtime's alternate stack** (C-only rows, both platforms where run): **R** gives the
  runtime's back (`alternate stack now a coroutine's: 0`), so § 9's abort works after it (*"stack exhausted in
  sst.deep"*, plain; ASan's own report under `--sanitize`); **L** leaves a coroutine's `malloc`'d stack registered
  (`... 1`): the runtime's SIGSEGV handler would then run on a library's stack, which is **unrun**, because the
  refusal of the next callback comes first. **J** with R restores it (`0`) and is refused all the same.
- **What the runtime does to the callbacks**: RR runs; RL, JR and JL hold the thread inside a handler **for good**,
  every later callback refused in REENTERED's words, on a coroutine's stack or the thread's own (the C-only rows).
  The library is correct by its own design in all four; no handler of the program's own C is involved.
- **Which one GNU Pth and libcoro are**: as I recalled both, **RR**, unverified until the historian read them.
  **The historian has since read them** (`.../panel262/docs/panel/262-reports/historian.md` § 6, 22:31, every one
  `verified` by its URL): Engelschall's USENIX 2000 paper, step 5 (*"save its machine context ... and immediately
  return from it to terminate the signal handler scope"*, `mctx_called = TRUE; return;`) and step 6 (the temporary
  stack disabled, the old one restored *"only where it was active"*); GNU Pth 2.0.1's `pth_mctx.c` variant 2 (the
  same); libcoro's `coro.c` under `CORO_SJLJ` (Tarantool's copy: the handler returns, `if (~ostk.ss_flags &
  SS_DISABLE) sigaltstack (&ostk, 0);`); **and QEMU's `util/coroutine-sigaltstack.c` on master** (the handler
  returns, `if (!(oss.ss_flags & SS_DISABLE)) sigaltstack(&oss, NULL);`). **All three real libraries are RR**, and on
  a Heroes thread the old alternate stack is always active (the runtime's), so all three give it back. **So the
  variant every real library takes runs**, and the three that abort are hand-written variants no sourced library
  takes.
- **Their delivery, built too** (`sst.h`'s third letter `s`, `sst-s.sh`): the worker signal blocked, sent to the thread
  by `pthread_kill(pthread_self(), SIGUSR2)` and taken inside `sigsuspend` with only it unblocked, as the historian
  quotes QEMU and libcoro (libcoro sends by `kill(getpid(), ...)`; on one thread the same). This Mac, base
  (`mac-base-sst-s.txt`) and `main` (`mac-main-sst-s.txt`), 3 of 3 per build: **RRs runs** (the program's coroutines:
  `coroutines 22 25`; C's: `called back on the thread: 6`), **RLs refused** in REENTERED's words as RL is.
- **So a binding of Pth's, libcoro's or QEMU's coroutines runs at the base and on `main`** on this Mac (and RR on
  Linux arm64, § 2.3), every coroutine starting inside a returned handler's frames: `now`'s fifth exit (*"a jump that
  lands within it"*) is its normal path, and `now` tells its author **may abort** for a library that runs.

## 5. What a binding author owes, by shape (and whether any text says it)

| shape | what a correct binding writes to keep its callbacks running | said by `now`? by the runtime's words? |
|---|---|---|
| a coroutine library with no signal handler (Boost.Context, OpenSSL `ASYNC`, `ucontext`, `longjmp` fibres, stack copying) | **nothing** on route C at `main`; at the base, nothing but on Darwin `--sanitize` with stacks carved from a C frame (2852, repaired by 199f) | `now`: silent (blind readers read *may* over it); the words: none needed |
| a signal-stack-trick library (Pth, libcoro, QEMU) | **nothing, because all three are RR**; a variant that leaves its alternate stack or leaves its handler by a jump poisons the thread for good | `now`: *may* (fifth exit); the words: REENTERED's, true |
| a library that preempts by `SIGALRM` and `swapcontext` in the handler | **no function of the program as a green thread's body**; C's bodies, then callbacks after the scheduler ends, run (`gtc`) | `now`: *may* (second exit), true both ways; the words: SWITCHED's |
| a library that calls back on a thread it made (libuv's pool, libdispatch; `SIGEV_THREAD`, `mq_notify`, `getaddrinfo_a` on Linux, § 2.3) | **never hand C the program's function as the work callback on that thread**: C's work, the program's function as libuv's after callback (the loop's thread), or `hero_thread_spawn` | **no spec sentence**: told only at the abort, in the thread's words, which name the remedy |
| a timeout that `siglongjmp`s out of C's blocking call (curl's synchronous resolver, by hand) | **nothing**, while the alarm is armed only around C's calls; a signal landing in the runtime's allocator meets panel 258's stop (the compiler-engineer's rate) | `now`: silent; panel 258's words: no sentence |

**The thread.** The thread's refusal is the one abort here that is **deterministic** (asked at every callback's
entry, `hero_thread_guard`, with no platform branch in `runtime/parts/thread.c`: `grep -n '#if\|_WIN32'` finds none;
Windows unrun), reached by **real libraries a binding author binds without ever writing `pthread_create`** (libuv,
libdispatch; Linux's three in § 2.3), and **stated by no sentence of the spec** (`grep -n -i 'thread C\|C made\|thread of
its own'`: only `spawn`'s line, the shared brief's q8). Its cost to a binding of libuv's pool: the binding compiles,
the header verifies every signature (the `importc` property holds: nothing is wrong at the boundary's types), and the
program aborts at the first queued work, 6 of 6 here. **It cannot be a compile error** (the thread is a run-time
fact), so the spec is the only place a binding author can be told before running: **one clause, an *aborts*, not a
*may***, e.g. *"A callback C calls on a thread the program did not start (a library's pool, a timer's, a queue's)
aborts, naming it."* (my wording, **unmeasured** on any tokeniser, a question for the warden).

## 6. The candidates of `00-shared.md` § 4, judged by the C that ran

**The warden's texts** (`docs/panel/262-reports/spec-w*.txt`) **are not readable to me while I write**: none stands in
the shared folder (`ls .../panel262/docs/panel/262-reports/`, 22:31: `critic-briefs.md`, `historian.md`,
`programs/`), and the warden's copy is another seat's directory. So each verdict below is on the candidate **as § 4
describes it**, and § 7 holds the rule a warden text must meet, row by row; a text I have not read gets no verdict.

| candidate | what falsifies it, by a program here | verdict | section |
|---|---|---|---|
| **`now`** (as it stands) | nothing (it promises nothing); **silent** on the thread's abort (libuv's pool, libdispatch, Linux's three) and on 2852 at the base; *may* over Pth, libcoro and QEMU, which run | **object** | §1.11, §1.12 (a binding author cannot design against it), §4.19 |
| **(a)** two sentences, a signal handler of C's apart from a coroutine switch | its no-handler promise is **false at the base** on Darwin `--sanitize` for fibres on carved stacks (`sw` A, `lj` C, 30 of 30), **true on `main`** (30 of 30 run) and on Linux arm64 at the base; the signal-stack-trick libraries must fall in the handler half (their coroutines start inside a handler's frames) | **approve, conditioned**: lands with or after 199f, *"no handler"* means none of the program's C **or its libraries'** (never the process's: the runtime's own stands in every build), *"on its thread"*, and the thread's abort said apart | §1.12 |
| **(b)** the property (*"... not known to have ended, or from inside one"*) | **RL** (`sst`): the handler **returned** (known to have ended), C then jumped back into its frames, and every later callback on the thread aborts; a reader of (b) predicts a run | **object** unless it says a jump back into a handler's frames holds it running again; and panel 258's stop scoped | §1.12 |
| **(c)** `now` + one clause | *"on its thread"*: `h9` runs, right; *"a switch with no handler of C's running is none of these"*: same as (a)'s promise (false at the base on Darwin `--sanitize`, true on `main`), **and false for RL, JR, JL** if *running* is read as the reader reads it (the library's handler returned or was left before the switch) | **approve, conditioned** as (a), with the jump-back said | §1.12 |
| **(d1)** no list (*"runs unless ... or C left one in a way the runtime does not follow"*) | the escape clause is circular for a binding author (no program can be designed against *"a way the runtime does not follow"*); its run-promise meets panel 258's stop and route A (the compiler-engineer's, unrun by me) | **object** | §4.19 |
| **(d2)** a promise per platform (`x1` scoped) | x1 is false at the base by 2705 and the own-stack coroutine (the shared brief's q3); an Intel Mac (route A by construction) is unrun by anyone | **object** | §1.12 |
| **(d3)** a runtime change so the sentence can be simpler | half one (*"a program whose C installs no signal handler is never told these words"*): **true on `main`** for every no-handler shape here on this Mac, and at the base on Linux arm64; half two (a stricter runtime, *may* becomes *aborts*): **false for `gtc`**, a correct preemptive scheduler that runs on both runtimes (`called back after: 6`, 6 of 6 each) | half one **approve** (with the thread said apart); half two **veto** | §1.12 |
| **(d4)** a told end (`hero_os.h`) | nothing measured needs it: every real library here either runs or is the thread's; a binding cannot tell the end of a handler inside a library it does not patch | **object** (cost: a `hero_os.h` promise for ever, decision 2228) | §4.19 |
| **(d5)** the words alone | nothing false; a binding author is told nothing before the abort | **object** | §1.11 |
| **(d6)** merge with the room's sentence | neutral for bindings, if both classes keep their words | no objection | — |
| **(d7)** line 427's home | *"leaves running for good"* is exactly what RL, JR, JL do (the C-only rows: every later callback on the thread aborts); *"aborts"* is false for `gtc` unless it says *"while one is running"* (in `gtc` none is, at the callback) | **approve, conditioned** on *"while one is running on its thread"* and the jump-back covered | §1.12 |
| **(d8)** abort at the exit | **`gtc` would be refused at its first preemption**, a program that runs today on both runtimes and never puts a function of the program inside a handler; so would C-only signal-stack-trick coroutines of any variant with no callback after | **veto**: a correct C library made unbindable by a change of the runtime, with nothing of the program at risk | §1.12 (*"any C library must be bindable"*) |
| **(d9)** a told route | `heroes doctor` at the base names no route (run) | **object** until it does, and until the spec may name a command's output | §4.19 |

**ABI cost of every candidate: none**, but (d4)'s new `hero_os.h` entry (a promise for ever). No candidate changes a
value's layout, ownership or `str`'s NUL, or adds marshalling at a C call; none touches `importc`-style header
verification (every binding here was verified against its real header by the compiler; the thread's abort and every
refusal are run-time facts no header carries).

## 7. What any warden text must meet, from the C that ran (the rule I would grade it by)

A text is **vetoed** where a correct program of a real library, or the replica of one a sourced page describes, falsifies
it on a platform it claims; **objected** where it leaves silent an abort a binding author meets with no signal in
sight. Row by row (each 3 of 3 per build, the cells above):

1. **No *aborts* (without *may*) for a jump into a handler's frames after it returned**: Pth's, libcoro's and QEMU's
   design (RR, the historian's § 6, `verified`), replicated in `sst.h` with both deliveries, runs on this Mac and on Linux arm64,
   base and `main` (§§ 2.1 to 2.5). The libraries themselves are on no disk here: **unrun as
   libraries**.
2. **No *aborts* for *"a `swapcontext` away from it"* unless it adds *"never switched back"* or *"while it is still
   switched away"***: `gtc` runs on both platforms and both runtimes.
3. **No run-promise for a coroutine library with no handler at the base on Darwin `--sanitize`** (`sw`, `lj` carved,
   30 of 30 refused); on `main` it holds (30 of 30 run), on Linux arm64 it holds at the base. A text with that promise
   lands with 199f or after it.
4. **The re-entry must be in it, in some words**: RL, JR, JL hold the thread inside a handler for good, every later
   callback refused, on any stack. A text that drops `now`'s fifth exit and promises a run after a handler that
   returned or was left by a jump is **false for them** (hand-written variants; no sourced library takes them).
5. **On its thread**: `h9` runs on both platforms; a text without it tells `h9`'s author *may* for a program that runs
   (not false, misleading).
6. **The thread's abort said somewhere**, as *aborts*: libuv's work callback, `dispatch_async_f`, `timer_create`,
   `mq_notify`, `getaddrinfo_a`, 30 of 30 on the two platforms; no text holds it today.
7. **Nothing promised about a `siglongjmp` out of a handler that interrupted the runtime's own code** (panel 258's
   stop, the compiler-engineer's rate); a `siglongjmp` out of C's own blocking call runs (`cu_alarm`, pipe and DNS, both
   platforms).
8. **No `hero_os.h` entry** unless a program here needs it; none does.

## 8. Verdict

- **verdict**: **object** to the sentence as it stands; **veto** (d8) and the stricter-runtime half of (d3) (and any
  text that says *aborts* for a `swapcontext` away from a handler, or for a jump into a returned handler's frames);
  **approve, conditioned**, (a), (c), (d7) and (d3)'s first half: landed with or after step 199f, *"on its thread"*,
  the re-entry kept in some words, *"no handler"* meaning the program's C and its libraries (never the process), and
  **the thread's abort said as an *aborts***; **object** (b) unless it says the re-entry, (d1), (d2), (d4), (d5), (d9).
  **No warden text read** (none readable while I wrote); each is to be graded by § 7.
- **section**: design.md §1.12 (*"any C library must be bindable"*, the veto's ground, panel 259's reading) and §1.11;
  §4.19 for the boundary. **design.md does not cover a callback on a thread a C library made**, as far as these words find:
  `grep -n -i -E 'did not start|thread of its own|callback.{0,40}thread|thread.{0,40}callback|thread (C|the C
  library|a library) (made|makes|started|starts)'` finds one line (3642, unrelated), and `grep -n -i thread` crossed
  with *C*, *foreign*, *callback*, *own* finds line 783 (*"the thread guard"*, the prologue's layout under one
  `#line`), 3360, 3801, 4137, 4144, 5010, none a rule on such a callback: a question for the warden, not a premise.
- **cost**: none at the ABI; (d4) alone would add a `hero_os.h` entry.
- **prediction**: *At the landing, `bx` and `aj` (Boost.Context 1.92 and OpenSSL `ASYNC` 4.1.0 and 3.5.x) carved and
  heap, built `--sanitize` on Darwin arm64 with the landed runtime, run 3 of 3 with no handler of C's, and libuv's
  work callback and `timer_create`'s `SIGEV_THREAD` callback abort in the thread's words on every CI leg that builds
  them*: checked by making `bx`, `aj`, `uvq work` and `pt timer` `run` cases at the landing (Windows: `pt`'s rows answer
  -100 there, unrun).
- **condition**: a warden text graded against § 7; a `main` that moves the runtime again (ask `git log
  87003cb42..main -- runtime`); and, apart from any text, `gtc`'s SEGV inside `hero_rebind_swapcontext` under
  `--sanitize` on Darwin (§ 2.2, 4 of 646), a robustness defect for the coordinator to file.

## 9. What stays unrun, and why

- **Boost.Context and libuv on Linux arm64**: not in the image (no `boost/version.hpp`, no `uv.h`), no download allowed.
- **GNU Pth, libcoro and QEMU themselves**: on no disk here; their design is run as replicas built from the
  historian's quotations (RR, both deliveries), never the libraries.
- **libcurl's synchronous resolver** (`alarm` and `siglongjmp`): the image's 8.14.1 resolves on a thread
  (`CURL_VERSION_ASYNCHDNS`); no build without the threaded resolver is here. Its shape, by hand, runs.
- **What the runtime's SIGSEGV handler does on a library's stack** (the L variants): the callback's refusal comes first.
- **Route A** (an Intel Mac, glibc's other architectures), **Linux x86-64**, **Windows**: no machine of mine; the
  thread's refusal on Windows is read from the code (no platform branch in `thread.c`), never run.
- **The warden's texts**: none readable while I wrote; § 7 is how I would grade them.
- **The cause of `gtc`'s Darwin `--sanitize` SEGV** in `hero_rebind_swapcontext`: located by `atos`, not traced.
- **The spec's token cost** of the thread clause I suggest: not measured on any tokeniser (the warden's).
- `main` after `0ce5b0cf3`: `bdef9ba06` at 22:43, and `git log --oneline 0ce5b0cf3..main -- runtime spec` is empty, so
  the second column stands for it.
