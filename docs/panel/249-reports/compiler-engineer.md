# Panel 249, compiler-engineer's report

Written as the work goes, from 2026-11-14 23:28 CET (`date`). Folder
`.claude/worktrees/p249-scratch/249-compiler-engineer/`, tree `tree/` in it, made from `722ae76bf` by `git
archive` as 00-shared.md § The base says, with a `.git` FILE reading `gitdir: /nonexistent-249-compiler-engineer`.
Every binary `ce-...`, every container `ce-249-<n>`. Stopped by the API's rate limit at about 00:40 and resumed at 01:47; every container's log was read before going on.

## 0. Where the briefs are wrong (said first)

- **"a Go archive's fault reaches Go's handler through the runtime's guard at the base already"** (00-shared.md §
  Question 1, the runtime's own use of signals; my brief § Question 1): **false for a null dereference**, run (§ 4.2):
  the guard's handler claims every fault below `HERO_NULL_WINDOW` as its own null read and ends the program
  (`runtime/parts/stack.c:961-972`), passing on only a fault it cannot classify (`:974`). A Go archive's nil dereference,
  which Go 1.27.3 turns into a panic its `recover` catches, ends a Heroes program at the base, 3 of 3, exit 134, with
  words blaming a `nullptr` handed to C.
- **"which two cases move"** (`compiler-engineer.md` brief, § B' item 3): under B' **five** of the fifteen 2494
  programs move on Darwin, at `-O0` and `-O2` alike (§ 2.4): the three whose Heroes function the kernel itself
  called (`given-to-signal`, `the-programs-own-sigaction-installs`, `with-siginfo-...`) move from the delivered words
  to R1's *"was called from inside a signal handler"*, and the allocator's and the lend tracking's cases move from D's
  words to R1's, so D keeps no falsifier among the goldens.

## 1. The base in my folder

- This Mac: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0 (23:36:28), `./heroes build
  selfhost/main.hero -o heroes-next` under `timeout -s KILL 1800` exit 0 (23:40:22), `heroes-next --version`
  printed `heroes 0.6.0` and **`sources 2ab7ca0a644692a1`**: on the base (`seed.log`, `stage2.log` in my folder).
- The inputs copied into `inputs/`, every hash and line count matching `briefs/inputs-manifest.txt` (`shasum -a 256`).
- The base's `runtime/` alone extracted to `base-rt/runtime` (`git archive 722ae76bf runtime`), used as the control
  through `HEROES_RUNTIME`, the compiler's own channel (`selfhost/cli/doctor.hero:7`); a control binary holds no
  `hero_sig_wrapper` and a B' binary holds one (`nm ... | grep -c`: 0 and 1).
- The emulated x86-64 image (`ce-249-1`, started 23:41 CET): `perf: none | valgrind: none | /usr/bin/time: ls: cannot
  access`, so **no instruction counter exists there**; `/proc/self/status` names no shadow-stack line and
  `/proc/cpuinfo` no `shstk`, `user_shstk` or `ibt` flag (both read by `grep`, printed empty).

## 2. Candidate B' (panel 248's route I), built

### 2.1 What I built, lines per file

**An order I changed, said**: the brief asks B' built in Linux arm64 first; my one container slot (00-shared.md, *"One
container of yours at a time"*) was held by the emulated base `ce-249-1` from 23:41 to 00:19 and then by the emulated
word `ce-249-3`, so I built and ran B' on Darwin first and wrote every Linux arm64 probe into one container, `ce-249-2`
(`linux/bp.sh`), run after them.

- `runtime/parts/siginstall.c`, **new, 311 lines** (`wc -l`; `shasum` `bf6ed0abe5a25f72`): the table of the
  program's handler and flags per signal (two atomic arrays), the C library's own `sigaction` found by
  `dlsym(RTLD_NEXT)` in a constructor and lazily (so a library's constructor that installs before the runtime's
  finds it), `sigaction` defined (a function installed as this runtime's wrapper with the program's own flags,
  `SA_SIGINFO` added, and the program's own mask; a query and an install's `old` answer the table's handler and the
  program's own `SA_SIGINFO` bit), `signal`, `siginterrupt` (with glibc's `_sigintr` mirrored) on both legs, and on
  glibc `bsd_signal`, `ssignal`, `__sigaction`, `__sysv_signal`, `sysv_signal`, `sigset`, `sigignore`, each as glibc
  2.41's source writes it; the wrapper (re-making `SA_RESETHAND` in the table, the kernel's own `siginfo_t` and
  context handed on); and the depth: each claimed thread's record holds 8 entries, the wrapper pushes its own frame's
  address, calls, pops by restoring its index; an entry is pruned where it lies on the same stack below the frame
  asking, or on an alternate stack while the frame asking is on the thread's own; the thunk refuses where an entry on
  its own stack lies above its frame.
- `runtime/parts/sigguard.c` 364 to 377 (+17 -4 by `diff`): the record's two fields, two forward declarations, the
  probes through `hero_sig_real` (the C library's own, as 248's I-onstack kept them), and one line in
  `hero_callback_entered`, `if (hero_sig_inside(own, (uintptr_t)frame)) hero_signal_refuse(what, 0);`.
- `runtime/runtime.c` 315 to 321: the part included after `sigguard.c`.
- **Compiler lines: 0** (`selfhost/` untouched). Darwin's arm is `sigaction`, `signal` and `siginterrupt` defined in
  the executable, enough for R3's static cases; Darwin's other installers (`bsd_signal`, `sigset`, `__sigaction`,
  `sigvec` and the four `_nobind`/`__platform_` names 248 listed) are **not built**.
- The runtime compiles with `flags()`'s 19 words at `-O0` and `-O2` with no diagnostic on this Mac (`clang @flags.txt
  -c -I runtime runtime/runtime.c`, exit 0 at both).

### 2.2 Darwin arm64, R3's two cases under B'

The library built as 00-shared.md says (`clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. lib/sigcb.c`, `ar rcs`),
`heroes-next build <p>.hero --library lib-nofp -o ce-bp-<p>`, both exit 0.

- `ce-bp-flib`, 20 runs under `timeout -s KILL 20`: **20 of 20 exit 134**, stdout `0|` (the install's line, the
  Heroes function never reached), stderr R1's words, *"panic: flib.answer was called from inside a signal handler:
  C called it while handling a signal ..."* with `HERO_SIGNAL_REMEDY` (the words 2533 calls false). At the base the
  coordinator read 3 of 3 exit 0, `answer ran for 30` (00-shared.md § Question 1).
- `ce-bp-fliblocale`, `python3 -I storm.py ./ce-bp-fliblocale SIGUSR1 4 0.3` 20 times from 23:45:19: **20 of 20
  armed, 20 of 20 `exit -6`** (SIGABRT, the stop), after 14,595 to 19,329 signals, each stderr R1's words. `ips.py`:
  `('ce-bp-fliblocale', 'SIGABRT', 'abort') 20`, **no `lock` report**; the 20 reports' pids are the storms' 20 (`comm
  -3` of the two sorted lists printed 0 lines). `ce-bp-flib` left 9 `abort` reports for its 20 runs, none other.
  `pgrep -fl ce-bp-` after: nothing.
- **So on Darwin, for a static library bound to the executable's definition, B' gives R3 what it asks: a refusal
  before the Heroes function's first C call, with R1's words.**

### 2.3 The depth question, run (Darwin arm64): B' refuses a correct program

`depth/depth.h` and `depth/tmpl.hero` in my folder: a C handler that leaves by a jump (`how` 0 `siglongjmp` from
`sigsetjmp(env, 1)`, 1 `longjmp`, 2 `_longjmp`, installed with or without `SA_ONSTACK`), `dp_escape` raising it once;
then a correct callback with **no signal running**, made once from 32 frames of 512 bytes below the escape's level
(`dp_deep`) and once directly (`dp_shallow`), in both orders. Twelve programs, each built against the B' runtime and
against the base runtime, 24 binaries, each run under `timeout -s KILL 20`:

| jump | stack | order | base | B' |
|---|---|---|---|---|
| `siglongjmp` | thread's own | deep first | exit 0, `deep 8 shallow 6` | **exit 134, R1's words, `deep` never printed** |
| `longjmp` | thread's own | deep first | exit 0 | **exit 134, R1's words** |
| `_longjmp` | thread's own | deep first | exit 0 | **exit 134, R1's words** |
| each of the three | thread's own | shallow first | exit 0 | exit 0, `shallow 6 deep 8` (the shallow call pruned the entry) |
| each of the three | alternate (`SA_ONSTACK`) | either | exit 0 | exit 0 (an alternate-stack entry is pruned by a frame on the thread's own stack) |

**The question 00-shared.md posed is answered no on the thread's own stack**: a depth entry holding the wrapper's
stack pointer cannot tell a handler still running from one a jump left once the program later calls back from deeper
than the abandoned wrapper, whichever of the three jumps left it. That is a correct program killed with a false
message, the `blocking` class (`.claude/rules/verification.md` § Bounded discovery: *"a correct program refused"*),
and the same hazard panel 248 measured of route T, a stale address found below a buffer nobody wrote
(`248-reports/ffi-pragmatist.md:191-196`). On the alternate stack the question is answered yes, exactly, by the
kernel's own geometry.

### 2.3b B'': the same depth confirmed by the frame records, built and run (Darwin arm64)

Because 2.3 is my own objection of 248 met by a run, I built the variant that answers it, `bp2-runtime/` in my folder
(`siginstall.c` 339 lines, +28 on B'): before refusing on an entry on the thread's own stack above the thunk's frame,
the thunk climbs the frame records from its own frame; **meeting the wrapper's own record exactly** means the wrapper
still runs (a frameless handler leaves the frame register holding the wrapper's record, so its callee's record names
it), **climbing past it without meeting it** means a jump left it (pruned, no refusal), and **a chain that ends first**
stays a refusal. An entry on an alternate stack is still answered by geometry alone.

- The twelve depth programs against B'': **12 of 12 exit 0**, every jump kind, both stacks, both orders (`ce-bp2-dp-*`).
- R3's `flib` against B'': **20 of 20 exit 134 with R1's words**.

**Where B'' can miss, by clang's own register allocation** (`tmp/regs.c`, `tmp/kh3.c`, a handler keeping many values live
across its callback, `clang --target=<t> -O2 -fomit-frame-pointer -S`): **Apple's arm64 target never allocates `x29`**
(only `stp`/`ldp` of it, no other use), **`aarch64-linux-gnu` allocates it as an ordinary register** (`eor x29, x2, x14`;
in the handler `ldp x29, x21, [x1, #40]`, a field of `info` in `x29` across `blr x8`), and **`x86_64-linux-gnu`
allocates `%rbp`** (`movq %rdi, %rbp; callq *%rbp`; in the handler `%rbp` a product of `ctx` fields across `callq`).
So on Darwin the confirmation is exact; **on Linux a library built `-fomit-frame-pointer` can call back with any value
in the frame register**: a value no record can be (a code address, a small integer) ends the chain and is refused
(correct when the wrapper runs, a false refusal after a jump), and **a value that is an address in the signal frame
above the wrapper** (the `ctx` or `info` a crash reporter's handler keeps live) reads as *climbed past* and is **missed**.
That shape is `linux/ship/rows/kmiss.h` (the frame register set to `ctx` around the call, in assembly so it is exact).

**Run on x86-64 in the emulated image** (`ce-249-3`, `amd64/word.log`'s last section; R3's library built
`-fomit-frame-pointer` there; each program once under `timeout -s KILL 30`):

| program | base | B' | B'' |
|---|---|---|---|
| R3's `flib` | exit 0, `answer ran for 10` | **exit 134**, R1's words | **exit 134**, R1's words |
| `kmiss` (frame register holding `ctx` at the call) | exit 0, `answer ran for 10` | **exit 134**, R1's words | **exit 0, `answer ran for 10`: MISSED** |
| depth, deep first, `siglongjmp` / `longjmp` / `_longjmp` | exit 0 / 0 / 0 | **exit 134 / 134 / 134 (false refusals)** | exit 0 / 0 / 0 |

**So on Linux x86-64 B' refuses three correct programs and B'' misses one handler, each run**: B'' trades B''s false
refusal for a silent miss wherever a frameless library's handler holds an address in the signal frame in its frame
register, which clang may give it (the allocation above). Neither is free on Linux; on Darwin B'' is exact.

### 2.3c Darwin arm64, the dylib residue of B' and B'', run

R3's library built as a dylib (`clang -std=gnu11 -O2 -fomit-frame-pointer -dynamiclib ... -install_name
@rpath/libsigcb.dylib`), `flib.hero --library lib-dylib`, against the base, B' and B'', each run three times with
`DYLD_LIBRARY_PATH=lib-dylib` under `timeout -s KILL 20`: **9 of 9 exit 0, `answer ran for 30`**, the Heroes function
inside the signal under every runtime. A two-level dylib's `sigaction` binds to libSystem's and never reaches a
definition in the executable (248's finding, `248-reports/ffi-pragmatist.md:130-136`, now run against B' itself). **So
Darwin's arm of B' as built (definitions in the executable) closes a static archive's handler and no dylib's**; every
Homebrew library this Mac holds is a dylib (`/opt/homebrew/lib/*.dylib`, the census's libuv, SDL3, OpenSSL, ncurses).
Closing the dylib needs what reaches every image, 248's I-dylib or the ffi-pragmatist's D-tramp, whose price is a dylib
beside every program (§ 5): the escape's.

### 2.3d Darwin arm64, the census's points (4), (5) and (7) under B', run

`ptsd/` in my folder (`linux/ship/rows/pts.h` less its raw system call), five programs, base and B', each under
`timeout -s KILL 30`:

| point | base | B' |
|---|---|---|
| (7) a SIGALRM handler by `signal` interrupts a `read` a thread's write ends after 2 s | `read returned 1`, after about 2000 ms (restarted) | the same |
| (7) the same after `siginterrupt(SIGALRM, 1)` | `read returned -1, errno 4 (Interrupted system call)`, about 1000 ms | the same |
| (5) `sigaction(SIGUSR2, NULL, &old)` after `signal` | the program's handler: 1, `SA_SIGINFO` shown 0, `SA_RESTART` 1 | the same |
| (4) the handler's own `backtrace` | **8 frames**: `pt_trace < _sigtramp < pthread_kill < raise < pt_run < h_pts4_main` | **9 frames**: `pt_trace < hero_sig_wrapper + 352 < _sigtramp < pthread_kill < raise < pt_run` |
| `errno` as the interrupted code reads it after a handler that sets 99 | 99 | 99 |

So on Darwin B' keeps the program's flags, its query and `errno` as the base does, and adds **one frame, named
`hero_sig_wrapper`, between a handler and the delivery** (point (4)): a crash reporter that skips a fixed number of
frames, or that looks for `_sigtramp` one frame up, reads one frame off.

### 2.3e A handler that jumps within itself, then calls back (Darwin arm64), and what it says of the jump-dropping routes

`injump/lib/ij.c` in my folder: a library built apart `-O2 -fomit-frame-pointer` whose SIGUSR1 handler does
`sigsetjmp` and calls a helper that `siglongjmp`s back into it (an error path inside a handler), **then calls the
program back while the handler still runs**; `ij.hero` is R3's `flib` bound to it. Three runs each under `timeout -s
KILL 20`: **base 3 of 3 exit 0, `answer ran for 30`** (inside the signal, missed); **B' 3 of 3 and B'' 3 of 3 exit 134
with R1's words**: no jump drops their depth, so the wrapper's entry stands and (under B'') the chain meets it.

**What it says of the routes that drop the depth at every jump**: D-tramp and D-rebind *"each dropping the depth to
zero first"* at a rewritten `siglongjmp`, `longjmp`, `_longjmp`, `setcontext`, `swapcontext` (`ffi-pragmatist.md` §§
3.2, 3.7), and the ffi-pragmatist's B' model the same at the C library's exits (its § 4.1, *"Dropping the depth at the
jumps trades that false refusal for a miss when a jump stays inside a handler; neither is free"*). **By those routes'
own rule this program's callback runs inside the signal** (the jump inside the handler zeroes the depth before the
callback): **a prediction from their stated design, unrun by me**, since their builds live in the ffi-pragmatist's
folder, which my rules keep me out of. The program is in `injump/` for the synthesis to have run against D-rebind.

### 2.4 Darwin arm64, the fifteen 2494 programs under B'

The fourteen `fixedbugs-2494-*.hero` and the folder case `...-a-signal-one-module-watches-another-module-takes.case`,
each built against the B' runtime at `-O0` and `-O2` and run in its own folder under `timeout -s KILL 30`, stdout then
stderr as `!` lines compared with `.expected` by `cmp` (`judge.sh` in my folder; not the `run` form, which I did not run
here: a harness build is a heavy command and the emulated container ran beside it).

- **10 read their `.expected` at both levels**: the `calls`, `atexit`, `fork`, `thread-c-made`, both alternate-stack
  cases, `sigset`, the door's two, `heroes-gives-signal`.
- **5 move, at both levels, every one still exit 134**: `given-to-signal`, `programs-own-sigaction-installs` and
  `with-siginfo-programs-own-sigaction-installs` print *"... was called from inside a signal handler: C called it
  ..."* where they expected *"... ran as a signal handler: the system called it ..."*: the kernel now calls the
  wrapper, so the thunk's return address is the wrapper's and the exact test is passed, and the depth refuses.
  `allocator` and `lend` print R1's words where they expected D's (*"a signal handler used this runtime's allocator
  while the code it interrupted was inside it ..."*): their handler is the program's own C reaching the function
  through more frames than the walk reads (the case's own comment, its lines 13-17), which the depth does not count,
  so the thunk refuses before the allocator is entered. **D's guard stays beneath for what B' does not reach and
  keeps no falsifier among the goldens**; its two cases would need a handler B' does not wrap (installed by a raw
  system call or before... ) to keep one. Keeping the delivered words for the first three would need the wrapper's
  two call sites learned as deliveries (a third probe through the definitions): unbuilt.

- **Under B''** (`g2494b2/verdicts.txt`): the same, 20 of 30 read their `.expected` and the same five move at both
  levels, each still exit 134.

### 2.7 Linux arm64 (`ce-249-2`), R3's cases, the shared library, the storms

`git archive 722ae76bf | timeout -s KILL 5400 docker run -i --rm --name ce-249-2 -v <folder>/linux:/out
heroes-linux-arm64:latest bash /out/bp.sh` from 01:48 CET (`linux/bp.sh`, `linux/bp-arm64.log`). Inside: *Debian GLIBC
2.41-12+deb13u3*, *Debian clang version 22.1.8 (1~deb13u4)*. Base seed exit 0, **stage 2 exit 0 after 150 s**, `sources
2ab7ca0a644692a1`. **Both B' runtimes compile against glibc's headers** (`clang @flags.txt -fstack-clash-protection -O2
-fsyntax-only`, exit 0 each), the glibc arm's eight installers included.

- `flib`, the library built apart `-O2 -fomit-frame-pointer`: **base 3 of 3 exit 0, `answer ran for 10`** (inside the
  signal); **B' 20 of 20 exit 134 with R1's words; B'' 20 of 20 the same**.
- **The shared-library form** (`libsigcb.so`, `-fPIC -shared -fomit-frame-pointer`, `LD_LIBRARY_PATH`): base 3 of 3
  inside; **B' 3 of 3 and B'' 3 of 3 refused**: on ELF a shared library's `sigaction` reaches the executable's exported
  definition (`-rdynamic`, `link_flags.hero`), which Darwin's two-level namespace does not give (§ 2.3c).
- **Storms** (`storm.py ... SIGUSR1 4 0.3`): base 3 of 3 armed, `exit 0`, *"built 20000000 words"* after 2,060,481 to
  2,169,692 signals (the program ran on with Heroes code inside the signal, R4 broken, as 00-shared.md measured); **B'
  20 of 20 armed, `exit -6`, R1's words; B'' 5 of 5 the same**. No crash reports on Linux: exits counted.

### 2.8 Linux arm64 (`ce-249-2`), the depth, the 2494 programs, `--sanitize`, the rows

- **The depth question**: the 36 depth programs (12 shapes, three runtimes) read exactly as on Darwin: **B' refuses the
  three deep-first shapes on the thread's own stack, one per jump kind, exit 134; B'' none; the base none**; every
  alternate-stack and shallow-first shape exits 0 under all three.
- **The fifteen 2494 programs**, `-O0` and `-O2`: **base 30 of 30 read their `.expected`; B' 20 of 30; B'' 20 of 30**,
  the ten others under each the same five cases as on Darwin (§ 2.4), at both levels.
- **`--sanitize`**: B''s `flib` refused 3 of 3 at `-O2` and 3 of 3 at `-O0`; ASan's own `heap-buffer-overflow` report
  kept against the base and against B'.
- **The rows** (`linux/ship/rows/`; each program once under `timeout -s KILL 30`):

| row | base | B' | B'' |
|---|---|---|---|
| each of `sigaction`, `signal`, `bsd_signal`, `sigset`, `sysv_signal`, `__sysv_signal`, `ssignal`, `__sigaction`, the handler in the program's own header | 8 of 8 refused (the walk) | 8 of 8 refused | 8 of 8 refused |
| the same eight, a static archive built `-O2 -fomit-frame-pointer` | **8 of 8 MISSED** (`answer ran for 10`) | **8 of 8 refused** | **8 of 8 refused** |
| the same eight, a shared library built so | **8 of 8 MISSED** | **8 of 8 refused** | **8 of 8 refused** |
| `kmiss`, the frame register holding `ctx` at the call | MISSED | **refused** | **MISSED** |
| a library loaded by `dlopen` installs and calls back | MISSED | **refused** | **refused** |
| a shared library installing from its constructor before `main` | MISSED | **refused** | **refused** |
| (7) restart under `SA_RESTART`; `EINTR` after `siginterrupt` | 2000 ms, the byte; -1 `EINTR` at 1000 ms | the same | (not run) |
| (5) `sigaction(sig, NULL, &old)`; a raw `rt_sigaction` | the handler, `SA_RESTART`; **the raw query answers the handler** | the handler, `SA_RESTART`; **the raw query answers the wrapper** | (not run) |
| (4) a handler's `backtrace` | 10 frames, `__kernel_rt_sigreturn` second | **11 frames, the wrapper second** | (not run) |
| `errno` after a handler that sets 99 | 99 | 99 | (not run) |

So on Linux arm64 **B' and B'' reach every glibc installer from every place a handler can live, a `dlopen`ed library
and a constructor's install before `main` included**, where the base misses every frameless library row; B' refuses
`kmiss` and the jump-out programs alike (one right, three wrong); **B'' refuses no correct program and misses `kmiss`**;
and a raw `rt_sigaction` query sees the wrapper, the one place the program's own query differs.

### 2.9 Linux arm64, bdwgc and Go under B' (`ce-249-2`, `ce-249-4`)

- **bdwgc** (Debian's `libgc-dev` 1:8.2.8-1 by `apt-get`; 248's list check inside a Heroes program, `gc/gcprobe.h`, the
  runtime's own claim giving `main` its alternate stack; five runs each under `timeout -s KILL 60`): **base 5 of 5 *"the
  list intact"* after 325 to 355 collections; B' 5 of 5 intact after 340 to 368**; SIGPWR's handler answers a function,
  `SA_ONSTACK` 0, under both. The wrapper's extra frame on the collector's own stack moves nothing it scans.
- **A Go 1.24.4 `c-archive`** (`golang-go` 2:1.24~2 by `apt-get`, `CC=clang`, which the image needs for cgo, the
  ffi-pragmatist's § 0 item 4; my first container lacked it and its Go rows did not build, so `ce-249-4` ran them alone,
  02:09 to 02:12, `docker exit 0`): **base 3 of 3 and B' 3 of 3: `preempted 1`, then the recovered nil panic killed by the
  runtime's null arm**, exit 134, as on Darwin (§ 4.2). Go's preemption survives B' on both POSIX legs; its nil-to-panic
  survives neither the base nor any candidate.

### 2.10 The suite B' owes, on this Mac

- **`runtime`** (`runtime/` moved): `tree/` rebuilt over the B' runtime (seed exit 0, stage 2 exit 0, `sources
  22bff5603a3c4469`, 02:27:19 to 02:29:33), then `run tests/harness/main.hero -- ./heroes-next runtime`: **`runtime: 10
  passed, 0 failed, 1 floor outgrown`**: `FLOOR runtime/threads`, *"objects that survive between calls: 137 against a
  floor of 109"*. C's run of the same suite told no floor (§ 3.6), so the new count is B''s own state (its handler
  table, its flags, its interrupt mask, its found `sigaction`): **a landing owes that suite's thread audit of each and the
  floor's raise in its own commit**. B'' (the same statics) not run separately.
- The compiler's own tests: `selfhost/` unmoved by B'; not run.

### 2.5 Darwin arm64, the cost

`qsortcost` (panel 247's, `inputs/ev247/247-compiler-engineer/cases/`), `heroes-next build qsortcost.hero -O2`, against
the base runtime (`ce-qsortcost-base`) and the B' runtime (`ce-qsortcost-bp`), each run three times directly under
`/usr/bin/time -l` (it ends by construction; under `timeout` the counter reads `timeout`'s own 16 million, which I
discarded): base **4,077,277,637 / 4,076,453,087 / 4,075,633,463**, B' **4,160,018,199 / 4,159,560,228 /
4,159,672,628** instructions retired. A counting twin of the header (`qsortcount.h`, a counter in the shim) prints
**20,953,348** callbacks. So B' adds about 83.4 million instructions, **about 4.0 a callback** where no handler runs
(the record's slot is already read for the thread question; `hero_sig_inside` reads `depth` and returns).

### 2.6 Darwin arm64, under `--sanitize`

- `ce-bp-flib-asan-O2` and `-O0` (`--sanitize`, B' runtime): `nm` reads `T _sigaction` and `t _hero_sig_wrapper` in
  the executable, `otool -L` reads `@rpath/libclang_rt.asan_osx_dynamic.dylib`; **3 of 3 exit 134 with R1's words at
  each level**: the runtime's strong definition stands in front of ASan's interposed one for a static library.
- **ASan's own report is kept**: a header's `malloc(8)` written at `[8]`, `--sanitize`, prints `ERROR: AddressSanitizer:
  heap-buffer-overflow` and exits 134 against the base runtime and against B' alike.

## 3. Candidate C (frame records in every unit)

### 3.1 The three cases in the emulated x86-64 image, at the base (`ce-249-1`)

`git archive 722ae76bf | timeout -s KILL 5400 docker run -i --rm --name ce-249-1 -e TAG=base -v <folder>/amd64:/out
heroes-linux:latest bash /out/run.sh` from 23:41 CET; `amd64/run.sh` and `amd64/base.log` in my folder. Inside: *Debian
GLIBC 2.41-12+deb13u3*, *Debian clang version 22.1.8 (++20260613092233...)*. Seed exit 0 after 21 s; **stage 2 exit 0
after 962 s** counted from the container's start (emulated wall at load 40 to 230, not a measurement), `sources
2ab7ca0a644692a1`. Each case built with that stage 2 and run directly at each level (`heroes-next build <case> -o <b>
<level>`, the binary under `timeout -s KILL 30`):

| case | `-O0` | `-O2` |
|---|---|---|
| `a-heroes-function-a-c-handler-calls-is-refused` | **green**: exit 134, `installed: 0`, R1's words, the `.expected` | **red**: exit 0, `the answer ran for signal 10`, `raised: 0`, `seen: 20` (the CI's line 4326, the same words) |
| `a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` | **red**: exit 0, `the atexit function ran: 42` | **red**: the same |
| `a-heroes-function-a-c-handler-reaches-through-fork-is-refused` | **red**: exit 0, `prepare ran: 42`, `the parent goes on`, `raised: 0`, `after the signal` | **red**: the same |

**The critic's model holds, run**: the `calls` case reads the CI's shape (green at `-O0`, red at `-O2`); **the `atexit`
and `fork` cases read red at `-O0` as well**, where the CI's log reads them green at `-O0` (`briefs/ci/job-132377462166.log`:
`grep -c -E 'FAIL run/.* at -O0'` 0, three `at -O2`). So for those two the emulated image measures Debian's x86-64 glibc,
whose `__run_exit_handlers` and fork handlers keep no frame record the walk can climb, and not the CI's Ubuntu leg:
**on a Debian x86-64 machine the tree's own `atexit` and `fork` cases run the Heroes function inside the signal at
every level today**, 2522's class reached through the C library's own frames, at the base. Why the two libcs differ
is on the historian's pages, not my run: Ubuntu builds its packages, its glibc included, with frame pointers since 24.04,
and Debian's glibc maintainer refuses (`249-reports/historian.md`, its verdict table's row C).

**The CI at this base, read when it ended** (run 43640139573, its Linux x86-64 job 132585763737, `gh api
.../jobs/132585763737 -q .status,.conclusion`: `completed | failure`, `head_sha` `722ae76bf...`; the log by `gh api
--allow-escape-sequences .../jobs/132585763737/logs`, 474,328 bytes, `ci-132585763737.log` in my folder): line 4092
**`run: 812 passed, 3 failed`**; lines 4295, 4306, 4317, **the same three cases, each `at -O2`**, none at `-O0`; line
1303 *"Ubuntu clang version 21.1.8 (6ubuntu1)"*. So **yes, the three read red at `722ae76bf`** on the CI's leg, in the
CI's shape, where Debian's libc in the emulated image reads two of them red at `-O0` too.

**ASan runs in the emulated image**: a plain program built `--sanitize` printed `plain 42`, exit 0; a header's `malloc(8)`
written at `[8]` printed `ERROR: AddressSanitizer: heap-buffer-overflow ... WRITE of size 1`, the frames
`over.h:3:71 < h_over_main over.hero:5 < main`, exit 1. So a red under `--sanitize` there would be the program's, not
the instrument's absence.

### 3.2 The word, built

- **Where it goes, and why there**: asked of the machine, as `frame_words()` asks its word, not in `flags()` for every
  machine. The runtime answers where it walks (`hero_frame_records_walked`, defined beside `HERO_SIGNAL_WALK` in
  `sigguard.c`, 1 under its POSIX arm and 0 under its Windows arm, so the walk and the word share one `#if`), and
  `frame_words()` adds `-fno-omit-frame-pointer` where it answers 1. On Darwin and Linux arm64 the word changes no
  instruction (Darwin measured below; Linux arm64 in `ce-249-2`); on Windows x86-64 it changes code and buys nothing,
  since there is no walk there (panel 247's R10): a handler calling a callback, cross-compiled by this Mac's Apple clang
  21 for `x86_64-pc-windows-msvc` and `x86_64-w64-windows-gnu` at `-O2`, reads `subq $40, %rsp; ... callq` without the
  word and `pushq %rbp; subq $32, %rsp; leaq 32(%rsp), %rbp; ... callq` with it (`tmp/wh.c`; the box's own clang is
  unread, the box being off, so the Windows leg's object is owed); so the only machine where it acts is Linux x86-64, the one the walk
  needs it on. `flags()` for every machine would be one line shorter and slower on Windows for nothing; CLAUDE.md §
  Precedence's *"where robustness is not at stake take the one fastest at run time"* decides it.
- **Lines**: `selfhost/cli/flags.hero` 584 to 602 (one `extern` line, `frame_words()` from 5 code lines to 7, its
  test from one word to two, and a 9-line comment), `runtime/hero_compiler.h` 109 to 116 (one declaration and its
  comment), `runtime/parts/sigguard.c` 364 to 371 (two one-line definitions and a comment). `heroes-next fmt
  selfhost/cli/flags.hero --in-place` left it unchanged (canonical); `heroes-next check selfhost/main.hero` exit 0.
- **Stage 2 on this Mac** (`tree-c/`, the seed then `heroes build selfhost/main.hero` under `timeout -s KILL 1800`,
  both exit 0): `--version` **`sources 13171722209da1f5`**, off the base by those three files.
- **What moves in the emitted C: nothing but the key**. `fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused`
  emitted with `--emit-c` by the base's stage 2 (against `base-rt/runtime`) and by the word's, each from inside its own
  tree with the same relative path: `cmp` names line 338 alone, `hero_program_sources` (`657db0614c958db3` against
  `3261ab4b3aa0efaa`); with that line taken out of each (`grep -v 'hero_program_sources(void) {'`), **`cmp` printed
  nothing**. The word is clang's, never the emitter's.

### 3.3 Darwin arm64

- **On this Mac the word changes not one instruction**: `qsortcost`'s emitted C (`--emit-c`, 322 lines) and
  `runtime/runtime.c`, each compiled `clang @flags.txt -O2 -gline-tables-only` with and without
  `-fno-omit-frame-pointer`: the two objects of each are **byte-identical** (`cmp` printed nothing), `objdump -d` 364
  and 25,570 lines, 0 differing. Apple's arm64 ABI keeps frame records already; the word buys Darwin nothing and costs
  it nothing but the key that moves.
- **`qsortcost` counted, built `-O2` by the base's stage 2 and by the word's**, three runs each under `/usr/bin/time -l`:
  base 4,080,259,415 / 4,078,006,111 / 4,077,540,412, word 4,076,977,818 / 4,076,262,829 / 4,077,032,068 instructions
  retired: the same within the runs' own spread, as byte-identical code must be.
- **A library that keeps frame records but calls back deeper than `HERO_SIGNAL_WALK`** (`deep/lib/deep.c`, built `-O2
  -fno-omit-frame-pointer -fno-optimize-sibling-calls`, its handler reaching the callback through twelve frames of its
  own; `deep.hero` is R3's `flib` bound to it), three runs each under `timeout -s KILL 20`: **base 3 of 3 exit 0 and
  word 3 of 3 exit 0, `answer ran for 30`** (inside the signal); **B' 3 of 3 and B'' 3 of 3 exit 134 with R1's words**.
  The word gives the walk records, never more steps; the depth counts no frames.
- **The `calls` case's handler at `-O2` here** (the brief: *"on this Mac that disassembly is unread: read it"*):
  `objdump -d` of `called_handler` in the base's `-O2` build and in the word's: **`stp x29, x30, [sp, #-0x10]!` /
  `mov x29, sp`** first in both, then the same eight instructions, `ret`. Darwin keeps the record unasked.
- **The fifteen 2494 programs under the word** (the word's stage 2, `-O0` and `-O2`, `judge.sh`): **30 of 30 read their
  `.expected`**.
- **R3's two cases under the word**: `ce-c-flib` **3 of 3 exit 0, `answer ran for 30`** (the Heroes function inside the
  signal, as at the base); `ce-c-fliblocale` three storms, **3 of 3 armed, 3 of 3 `exit -9`**, `ips.py`: `('ce-c-fliblocale',
  'SIGKILL', 'lock')`, the three reports' pids the storms' three (55946, 55970, 55998). **The word reaches none of a
  library built apart**, run.

### 3.4 The word in the emulated x86-64 image (`ce-249-3`)

`tree-c` archived with `COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs` (no `build/`, no binaries), `timeout -s
KILL 5400 docker run -i --rm --name ce-249-3 -e TAG=word ... heroes-linux:latest bash /out/run-word.sh`, from 00:23 to
01:08:23, **`docker exit 0`**, no container left (`amd64/run-word.sh`, `amd64/word.log`). Seed exit 0 after 20 s; **stage
2 exit 0 after 915 s** (emulated, not a measurement); `sources 13171722209da1f5`, the same key as this Mac's word build.

| case | base, `-O0` / `-O2` (§ 3.1) | **the word, `-O0` / `-O2`** |
|---|---|---|
| `calls` | green / **red** | green / **green**: exit 134, R1's words, the `.expected` |
| `atexit` | red / red | **red / red** (`the atexit function ran: 42`) |
| `fork` | red / red | **red / red** (`prepare ran: 42`) |

- **The `run` form under the word**: `calls` **1 passed, 0 failed**; `atexit` FAIL at `-O0`; `fork` FAIL at `-O0`; all
  fourteen **13 passed, 2 failed** (the base read 12 and 3).
- **The `calls` handler at `-O2` under the word**: `push %rbp; mov %rsp,%rbp; movslq %edi,%rdi; call *...`, the record
  the base's `-O2` lacked (00-shared.md's `push %rax`).
- **ASan**: the plain program `plain 42` exit 0, the overflow reported, as at the base.
- **So on x86-64 the word closes the tree's `calls` case and leaves Debian's `exit` and `fork` frames, run**: no word of
  this compiler reaches them.

**The price on x86-64, by static text** (no counter in that image: `perf`, `valgrind`, `/usr/bin/time` all absent, § 1):
`qsortcost`'s emitted C and `runtime/runtime.c`, compiled by the image's clang with `flags()`'s words,
`-fstack-clash-protection` and each level's debug word, with and without the word; `size`'s text column:

| object | `-O0` without / with | `-O2` without / with |
|---|---|---|
| `qsortcost`'s unit | 3,089 / 3,089, `cmp` same | **1,829 / 1,944** (+6.3 %), `objdump -d` 374 / 393 lines |
| the runtime's object | 146,060 / 146,060, `cmp` same | **131,379 / 132,282** (+0.69 %), 24,648 / 25,560 lines |
| the linked `qsortcost` | 181,616 / 181,616 | **163,980 / 164,996** (+0.62 %); both print `1` |
| the compiler's own C, `seed/heroes.c` | 21,025,019 / 21,025,019, `cmp` same | 14,357,891 / **13,274,958** (the word's 7.5 % SMALLER; measured once each, 169 s and 161 s of emulated compile; why, unread) |

**The compiler as this tree builds it costs nothing**: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` and
`heroes build selfhost/main.hero` both compile at `-O0` (CLAUDE.md § Commands; `compile.hero:76`'s default), where
x86-64's clang keeps every record already and the objects are byte-identical. The price falls on programs built at `-O2`
(`heroes run`, `build -O2`): under one per cent of text for the runtime and a program, more for a unit of small
functions. **Instructions on x86-64 are unread**: no counter exists in the image, and the CI's runner is not mine to
time. Fedora and Ubuntu's own measurements of the same flag are the historian's (*"between 1-2% in most cases"*), not
mine.

### 3.4b The word in Linux arm64 (`ce-249-2` § 7)

The word's tree (the base and `c-overlay/`, the same three files), seed and stage 2 exit 0, **`sources 13171722209da1f5`**
(the key this Mac's and the emulated image's word builds print).

- **No instruction moves**: `qsortcost`'s emitted C and `runtime/runtime.c` compiled `-O2` with and without
  `-fno-omit-frame-pointer` by the image's clang: `objdump -d` 370 and 23,953 lines each, **0 differing, `cmp` same**.
- **The fifteen 2494 programs under the word**, `-O0` and `-O2`: **30 of 30 read their `.expected`**.
- **The `run` form narrowed**: **`callback` 14 passed, `signal` 10 passed, `handler` 12 passed, 0 failed**, under the word
  and at the base alike.
- **R3's two cases under the word**: `flib` 3 of 3 exit 0, `answer ran for 10`; `fliblocale` 3 storms armed, `exit 0`,
  *"built 20000000 words"* after 1,913,261 to 1,942,367 signals: inside the signal, as at the base.

### 3.6 The suites C owes, on this Mac

- **The compiler's own tests** (`selfhost/` moved): `tree-c/heroes-next test selfhost/main.hero` under `timeout -s KILL
  1800`, 02:12:32 to 02:16:58: **2,160 tests, 8 failed**; *"the frame words are the machine's to ask, and never the
  list's (defect 2253, panel 249)"* `ok`. **The control**, the base's compiler and runtime (`tree/heroes-next` with
  `HEROES_RUNTIME=base-rt/runtime`), 02:20 to 02:24:48: **2,160 tests, 8 failed, the same eight** (`diff` of the two sorted
  `FAIL` lists printed nothing): package-manager tests that fetch git sources and tags (*"a git entry is fetched from its
  blobs ..."*, *"a name that is not UTF-8 is refused ..."* and six more), which fail in my copy at the base too, whose
  `.git` file points nowhere (00-shared.md § The base). **So the word moves no test.**
- **`cache` alone** (`flags.hero`'s row, every unit's and the runtime's key moves): `tree-c/heroes-next run
  tests/harness/main.hero -- ./heroes-next cache`, 02:25:34 to 02:26:46: **`cache: 9 passed, 0 failed`**.
- **`runtime`** (`runtime/` moved): **`runtime: 10 passed, 0 failed`**, 02:26:57 to 02:27:08.
- **`run` whole: not run**, and why: it builds each of 795 cases at four levels, the CI's Darwin shard of it alone took
  1 h 22 min at this base (`gh run view 43640139573`), and my folder's time went to the four containers; the cases the
  word can move (the fifteen 2494 programs at two levels on three images, the `callback`, `signal` and `handler`
  groups in Linux arm64) were run instead. **Owed, before a landing.**

### 3.5 What C leaves, and whether C alone closes 2522

- **A library built apart**: R3's two cases under the word run the Heroes function inside the signal on Darwin, 3 of 3
  and 3 storms of 3 SIGKILL (§ 3.3); on Linux arm64 3 of 3 and three storms running on (§ 3.4b). The word reaches only what this compiler compiles.
- **The C library's own frames between a handler and the callback**: on Debian's x86-64 glibc the `atexit` and `fork`
  cases are red at `-O0` already (§ 3.1), where every frame of the program keeps its record, so no word of this
  compiler reaches them: under the word both stay red at `-O0` and `-O2` in the emulated image, run (§ 3.4).
- **A library that keeps records but calls back deeper than eight frames**: inside the signal under the word, 3 of 3
  (§ 3.3).
- **Windows**: no walk to give records to (panel 247's R10).
- **So C alone does not close 2522 in R3's terms**: it closes, at most, the tree's own `calls` case on x86-64 (and,
  on the CI's Ubuntu libc, its `atexit` and `fork` cases, which read green there at `-O0`; the `calls` case on Debian's
  image is closed by it, run, § 3.4); R3's two cases, the C library's own frames on a libc like Debian's, a deep
  library and Windows are its residue. What C does close is the CI leg red on `main` today (§ 3.1), the tree's own
  cases on the CI's own architecture.

## 4. Question 1, the runtime's half of the census

### 4.1 What the member tells a program that binds a census library, base and B' (Darwin arm64)

Each built with my stage 2, against `base-rt/runtime` (`HEROES_RUNTIME`) and against the B' runtime, `q1/` in my folder:

| program | base | B' |
|---|---|---|
| libuv (the tree's `fixedbugs-413-libuv-keeps-every-handles-address.hero`, `pkgconfig "libuv"`, Homebrew 1.53.0) | builds, runs `0 0 0 true true true` | the same |
| a Go 1.27.3 `c-archive` (`go/answer`, `goanswer(x)`, the Go environment of 00-shared.md before every `go`) | builds, `go answers 42` | the same |
| a library defining `sigaction`, forwarding by `RTLD_NEXT` (`defsig`) | **exit 1** `ffi_defined_elsewhere`: *"defines `sigaction`, of the C library, which the program's own runtime calls ..."* | **exit 1** `ffi_defined_elsewhere`: *"defines `sigaction`, a name of the program's own runtime, and a program holds one `sigaction`"* |
| a library defining `siginterrupt` (`defint`), a name the base runtime neither calls nor defines | builds, `defint 9` | **exit 1** `ffi_defined_elsewhere`: *"defines `siginterrupt`, a name of the program's own runtime, and a program holds one `siginterrupt`"* |

**So B' widens panel 239's member with no compiler line**, as 248 measured of I-onstack
(`248-reports/compiler-engineer.md:162-177`): every installer B' defines and the base runtime does not call (on glibc
`bsd_signal`, `ssignal`, `__sigaction`, `__sysv_signal`, `sysv_signal`, `sigset`, `sigignore`, `siginterrupt`; on Darwin
`siginterrupt` as built) refuses a library that defines it. **That is the escape's widened diagnostic, in those words.**

**And B' reaches a Go archive's own installs on Darwin**: in `ce-q1-go-bp` `nm -m` reads `external _sigaction` defined at
`0x10000149c` and no `(undefined) external _sigaction (from libSystem)`, which `ce-q1-go-base` holds; Go's
`runtime.sigaction_trampoline` therefore calls the runtime's definition, from Go's constructor, before `main`.

### 4.2 Go's preemption and its nil panics, base and B' (Darwin arm64): found beside the question

`go/census/main.go` (Go 1.27.3 `c-archive`): `go_preempt` (one P, a goroutine spinning with no call in its loop, another
goroutine that can run only if the runtime preempts the spinner by SIGURG and an edited context; 1 where it ran within
5 s) and `go_nilpanic` (a nil `*int64` read under a `recover`; 1 where recover caught it). `q1/gocensus.hero` prints
`go_preempt`, `go_nilpanic`, `go_preempt`; three runs each under `timeout -s KILL 60`:

- **base: 3 of 3 exit 134**, `preempted 1`, then **`panic: a null pointer was read through, at offset 0x0, called from
  main.go_nilpanic`** and `HERO_NULL_READ_TAIL` (*"Two things put a null where C read it: a handle or `ptr` holding
  `nullptr` handed to a C function ..."*). **B': the same, 3 of 3.**
- So **Go's preemption survives the base and B'** (the context edit reaches the kernel through the wrapper), and **Go's
  nil-to-panic does not survive the base**: the runtime's guard (installed from `hero_args_set`, after Go's constructor
  installed its own, which it saves) claims the null fault before passing anything on (`stack.c:961-972`), and its
  message names a cause that is not this program's and calls Go's symbol a Heroes name. It is the `blocking` shape (*"a
  correct program refused"*, *"a false message"*), the runtime's own handler changing what a library's handler sees
  (the census's points (4) to (6)), at the base and under every candidate of this sitting, none of which touches
  `stack.c`'s null arm. Searched before calling it unfiled: `gh issue list --state all --search` for `Go archive nil`,
  `"null pointer was read through" Go`, `c-archive recover`, `stack guard chain Go`, `SIGSEGV library handler null
  window`, `implicit null check`, `Go panic recover fault`: no issue names it; a question, not proof of absence. **Not
  this sitting's question; for the coordinator to file.** Reproducer: `go/census/` and `q1/gocensus.hero` in my folder.

## 5. Candidate A and D-tramp, from the runtime's side (read, and run where marked)

**The runtime's own 27 install lines** (`grep -n -E '(^|[^_a-z])(sigaction|signal)\(' runtime/parts/*.c
runtime/platform/*.c`, comments dropped, re-run: the brief's list, line for line) **and 18 mask lines** (the same for
`pthread_sigmask|sigprocmask`: `spawn.c` 274, 344, 377, 561, 568; `run.c` 1111, 1379, 1389, 2264, 2268, 2283, 2318;
`sigguard.c` 125, 144, 207, 216; `hero_net.c` 1120, 1122). Every one names the installer or the mask call by name, so a
definition of the runtime's (A's, B''s) reaches each: those in `runtime.c`'s one unit bind inside it, and `hero_net.c`'s
member binds at the link to the executable's export (§ 4.1's `nm -m` shows the same binding for a Go archive).

- **Under A** (a marker added to the mask of every handler installed with a function; the mask calls let the marker go
  from blocked to unblocked and never back; the thunk asks whether it is blocked): the runtime's own handlers (the lease
  handlers `os.c:471`, the forwarding `run.c:1094`, the door `sigdoor.c:104`, the guard `stack.c:1054-1055`, the net
  stop `hero_net.c:1295`) are installed with the marker in their masks, harmless since none calls a thunk. The 18 mask
  calls lose the marker from every block: **R2's poller thread** (`hero_net.c:1115-1122`, `sigfillset` then
  `SIG_SETMASK` around `pthread_create`) **is born with every signal blocked but the marker**, and so is every thread
  `spawn.c:274` starts with its `held` set; harmless while nobody SENDS the marker, which is A's own premise, and a
  correct program that blocks the marker and waits for it (248's critic's item 3, `swait.c`) loses it, as with SIGURG.
  `sigguard.c:125` (the stop blocks all) and `panic.c:54` (`signal(SIGABRT, SIG_DFL)`) sit inside the refusal and panic
  paths: under A the first leaves the marker unblocked on a thread that is ending, and the second installs no function,
  so A's definition passes it through untouched. **None of these is a soundness hazard of the runtime's own; each is a
  line a landing must read once.** (Read, unrun: A is the ffi-pragmatist's to build.)
- **The probe's SIGURG once the marker is another signal**: the probes borrow SIGURG for two raises at the first crossing
  (`sigguard.c:175-216`), and a Go archive's constructor installs SIGURG for its preemption (00-shared.md, Go 1.27.3
  `runtime/signal_unix.go:178-182`; the ffi-pragmatist's trace, its § 1.2). The ffi-pragmatist measured preemption
  working before and after the probe (*"preempt true"* both times) and the probe giving Go's handler back with the
  kernel's own mask; I measured the same, `preempted 1` before and after a call, base and B' (§ 4.2). What the borrowing
  still costs: for the instant of the two raises **every** SIGURG delivered to the process, a Go preemption aimed at
  another thread included, runs the probe instead of Go's handler (lost, and Go re-sends), and a program's own SIGURG
  handler is displaced the same way. **Under A on Linux the probe should borrow the marker instead**, a signal the
  runtime then owns and no library's handler holds, which removes the window; what it costs is lldb's default for a
  real-time signal (`process handle` stops on it where SIGURG passes: panel 247's compiler-engineer chose SIGURG for that,
  `sigguard.c:189-191`'s comment), unrun here. On Darwin, where A's marker would be a classic signal, the same reasoning
  holds for whichever signal the census finds free.
- **D-tramp** (Darwin, an interposer of libsystem_kernel's `__sigaction` handing the kernel a trampoline of the
  runtime's): from the runtime's side its price is the I-dylib's (248's R1 objection): **a dylib beside every program
  that hands C a function**, which `heroes build` does not make today (`link_flags.hero` gives no `LC_RPATH`; 248's
  critic's item 8), so `selfhost/cli/link_flags.hero` and the build's output both move, a change a binding's author
  must learn (**the escape, in those words**), plus a trampoline that must reproduce `_sigtramp`'s sixth argument and its
  `__in_sigtramp` store, which no header declares (00-shared.md § D). Unbuilt by me; the ffi-pragmatist builds it.
- **The shadow stack** (00-shared.md § D, the critic's item 18): the emulated x86-64 image's `/proc/self/status` holds
  no shadow-stack line and its `/proc/cpuinfo` no `shstk`, `user_shstk` or `ibt` flag (§ 1); **the Linux arm64 image's
  `/proc/self/status` holds no shadow-stack line either, and its `/proc/cpuinfo` names `bti paca pacg` and no `gcs`**
  (kernel *7.0.14-linuxkit*, `ce-249-2`'s first lines): neither of this Mac's Linux legs offers a shadow stack. Linux enables x86-64's user shadow stack only for a process whose every object carries the
  `SHSTK` GNU property, which clang writes only under `-fcf-protection=return` or `full`, and no word of `flags()` asks
  it (`sed -n 133,154p selfhost/cli/flags.hero`, re-read): **so no Heroes program runs with a shadow stack on any leg
  this tree builds today**, an inference from the flags and the kernel's documented rule, unrun on a CET machine (none
  here). It stays a question, not a route.

## 6. Question 3, from the compiler's side

### 6.1 D-imports: what reading a bound library's imports at build would take

- **What the member reads today**: who SUPPLIES each name the program and the runtime call (`selfhost/cli/supplied.hero`,
  357 lines; the link's own answer, `--trace-symbol` on GNU ld and lld, Apple's `-map`, a Windows linker's `/MAP`,
  `supplied_probe.hero:232-251`), and **one reader of undefined names**: `supplied_kept.hero:221-229`, `alone_needs`,
  an object linked `clang -nostdlib` alone and every name the link calls undefined read back, used for **the runtime's
  object and the platform layer's members only** (`:174-186`, `runtime_needs`; `cli/layer.hero`). **No step reads a
  bound library's imports** (searched: `grep -n -E 'undefined|nm |-u"|--trace|-map|/MAP'` over the fourteen
  `supplied*.hero`, `library_needs.hero`, `link_report.hero`; `library_needs.hero` reads the undefined names a failed
  link REPORTS, which a library that links reports none of).
- **What D-imports would take, per linker**: on GNU ld and lld, `alone_needs` applied to each bound library file, an
  archive under `--whole-archive` (`supplied_probe.hero:190-194` already writes those words) and a shared library through
  GNU ld's default `--no-allow-shlib-undefined` (`library_needs.hero:250` shows its words for a `.so`), then the census's
  name list matched against the answer; **on Apple's ld64 a dylib's imports are bound two-level and a link reports none
  of them** (an inference from ld64's two-level binding, which 248 measured for `sigaction`; unrun as a link), so Darwin
  needs a reader of the dylib's own import table (`nm -u` or `dyld_info -imports`, a tool the member does not call
  today); on Windows, an import library's names. So **a new reader on Darwin, the existing one widened on the ELF
  linkers, a name list of 42 installers, mask calls, waits and jumps to keep current, and a new diagnostic**: a
  compiler message a program did not get yesterday, **the escape, in those words**. My estimate of its size, unbuilt:
  150 to 300 lines across `supplied_kept.hero`, a new reader module and a words module, each with its tests.
- **What it would refuse or tell in the tree**: the programs that bind a library whose imports the seed reads as holding
  an installer (libuv, libcrypto, SDL2, SDL3, ncurses, curl on Linux, bdwgc) **and** hand C a function. Counted:
  `git ls-files '*.hero' | xargs grep -l -E '\b(link|pkgconfig) "(uv|libuv|crypto|libcrypto|ssl|libssl|SDL3|sdl3|SDL2|sdl2|ncurses|ncursesw|curl|libcurl|gc|bdw-gc)"'`
  prints **10 files** (`examples/curl/main.hero`, `examples/sdl/main.hero`, `docs/panel/176-briefs/xfer_ssl.hero`, the
  compiler test `selfhost/emit/ffi_linked.hero`, `tests/golden/fixedbugs/ffi-int-narrowing-below-32.hero`, four `run`
  cases), and of those `xargs grep -l -E ': \(function\('` (a function-typed parameter, the proxy for handing C a
  function) prints **1**, `tests/golden/run/fixedbugs-413-libuv-keeps-every-handles-address.hero`; 234 tracked `.hero`
  files declare a function-typed parameter at all. **So in the tree D-imports would reach one program, and that program
  is correct**: libuv's handler writes a pipe and its loop calls the program's callback outside the signal (the
  ffi-pragmatist's census row, its § 1.2). Every one D-imports would refuse in the tree would be a false refusal.
- **Its holes, unchanged by building it** (00-shared.md § D, carried, and the ffi-pragmatist's trace): a library reached
  by `dlopen` (sdl2-compat installing SDL3's handlers, which no reading of SDL2's imports shows), an installer reached
  by `dlsym`, a raw system call, and the program's own units. **247's veto reaches it** in my reading: a selection by a
  name list with a hole a correct-looking program dies through in silence (`docs/panel/247-*.md:293-294`), here of a
  library's imports instead of a callee's name, with the same `dlopen` hole measured.

### 6.2 The words, and the order of 2533's repair and this sitting's

- **What a refusal at run time could say that R1's does not**: every candidate I built refuses with words the runtime
  already has. B' and B'' call `hero_signal_refuse(what, 0)`, R1's *"was called from inside a signal handler: C called
  it while handling a signal ..."*, and C refuses nothing new (it lets the existing walk see). The one thing a refusal
  could add is **who installed the handler** (B''s table holds the handler's address, so `dladdr` could name its image,
  `libsigcb.dylib` or `ce-bp-flib`), outside a signal-safe path only: `dladdr` is not async-signal-safe, and the stop is
  (`sigguard.c:59-62`, *"async-signal-safe from its first call to its last"*). So no new words are needed by any route built, and a library's name in the words would need a
  name gathered before the signal (at install, in the table): unbuilt, a question.
- **What spec line 404 and `HERO_SIGNAL_REMEDY` would then say**: unchanged by this sitting's routes; **2533 alone moves
  them** (the macro's *"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1"* against line 404's
  *"Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make ..."*, `sed -n 403,404p spec/heroes-spec.md`, re-read).
  Confirmed at the base: three users (`sigguard.c:306`, `alloc.c:555`, `lend.c:194`), **ten** golden files pinning the
  words (`grep -rl 'hero_net_stop_asked() -> i64\` answer 1' tests/golden | wc -l`: 10), 2533 `OPEN` (`gh issue view
  2533`).
- **The order: 2533's repair first, this sitting's landing after**, for three measured reasons. (1) Every refusal the
  runtime makes today prints words false since `cd693eec1`, and every route of this sitting refuses through the same
  macro, so a landing before 2533 would add refusals carrying a false sentence. (2) The landings touch different text:
  2533 changes the remedy (the tail of every refusal, ten goldens); B' or B'' changes the first clause of five 2494
  goldens on Darwin (§ 2.4) and C changes no golden at all; landing 2533 first, the five move once more in their
  first clause only, against an already-true tail, and a reviewer reads one change per commit. (3) 2533's repair
  sits in batch 37's lane b37-runtime (00-shared.md), and this sitting's landing needs the author's ratification and,
  for B', B'' and D-imports, the escape's full panel first: 2533 does not wait on either. **No new words are proposed
  here, so nothing of this sitting is the escape's on account of words**; the escape binds B' and B'' through the
  widened `ffi_defined_elsewhere` (§ 4.1) and D-imports as a new diagnostic (§ 6.1).

## 7. Where my builds agree with the ffi-pragmatist's, and D-rebind

Read after my own measurements stood (`249-reports/ffi-pragmatist.md`, finished 00:33).

- **They agree wherever they overlap**: B' as I built it refuses a correct program after a jump (its § 4.1 cites my §
  2.3; I measured it on all three POSIX images, §§ 2.3, 2.3b, 2.8); no definition in the executable reaches a Darwin
  two-level dylib (my 9 of 9, § 2.3c), the residue its D-tramp and D-rebind exist to close; a wrapper or trampoline that
  hands on the kernel's own context keeps Go's preemption (my B' on both POSIX legs, its D-tramp and D-rebind); bdwgc
  stays intact under a wrapper (my 5 of 5 inside a Heroes program, its 5 of 5 in C); the runtime's null arm kills a Go
  archive's recovered nil panic at the base and under every candidate (both of us, both legs); and D-imports would refuse
  only correct programs among the census and the tree (its traces, my count of 1).
- **D-rebind** (its § 3.7: the executable rewrites, in every loaded image outside the shared cache, the import slots for
  `sigaction`, `signal` and the jumps, and hands the kernel D-tramp's trampoline): **not built by me** (its build is in its
  folder, which my rules keep me out of). From the compiler's and the runtime's side its price is low: about 120 lines of
  runtime C plus the trampoline, **0 compiler lines**, no file beside the program, no C library name defined (so panel
  239's member does not widen, by its reading), its holes `dlsym`, the shared cache and a raw `__sigaction`, told.
- **One row it lacks, which my build measured**: a handler that jumps **within itself** and then calls back (§ 2.3e):
  base missed, **B' and B'' refused 3 of 3**. D-rebind drops the depth to zero at every rewritten jump, so by its own rule
  that program's callback runs inside the signal: **a prediction from its stated design, unrun**. The repair is measured
  on Darwin already: confirm a depth entry by the frame records instead of zeroing it at a jump (B'', § 2.3b), exact there
  because Apple's arm64 target never allocates `x29` (§ 2.3b) and clean on all twelve jump-out shapes. **D-rebind's reach
  with B''-style confirmation is the Darwin arm I would approve**; unbuilt as a pair.

## 8. Verdicts

Every verdict below rests on the runs above. My definition holds a veto for a core construct or a breached ceiling; the
brief gives this lane a veto on soundness. I cast none: no candidate I built makes a program corrupt memory or die in
silence that the base does not; each one's misses are misses the base has too.

### Question 1, the runtime's half of the census

- `verdict`: **object** to the premise that the runtime's own handlers are neutral to a bound library's
- `section`: design.md §1.12 (*"A Heroes program must not segfault and must not corrupt memory"*, and *"What would make
  this wrong"*: an abort that tells the reader nothing true); §1.11. design.md names no signal rule.
- `implementation_cost`: 0 lines asked; the cause is `runtime/parts/stack.c:961-972` (1,213 lines), the null arm before
  `hero_stack_pass_on` (`:673-687`, called at `:974`).
- `needed_for_self_hosting`: no
- `argument`: Run on both POSIX legs (Go 1.27.3 on Darwin, Go 1.24.4 on Linux arm64), at the base and under B': a Go
  archive's recovered nil dereference ends the Heroes program, exit 134, blaming a `nullptr` handed to C (§§ 4.2, 2.9);
  the ffi-pragmatist found it independently. Go's preemption survives everything. The member answers census libraries as
  248 measured; B' widens it to every installer it defines (`siginterrupt` refused, § 4.1), the escape's widened
  diagnostic. Not this sitting's question; a defect for the coordinator to file.
- `prediction`: `q1/gocensus.hero` exits 134 at its `go_nilpanic` on Darwin arm64 and Linux arm64 at every commit until
  `stack.c`'s null arm passes on a fault whose pc lies outside the program's own images; checkable at M-core-packages'
  close.
- `condition`: a run where Go's `recover` returns with the runtime's guard installed.

### Question 2, candidate C (the word)

- `verdict`: **approve**, as x86-64's complement for the units this compiler builds, **not** as a closing of 2522
- `section`: design.md §1.1 (a word, no construct: the ceiling untouched) and §1.12 (the runtime's walk needs the records
  it reads)
- `implementation_cost`: built: `selfhost/cli/flags.hero` 584 to 602 (1 `extern` line, `frame_words()` 5 to 7 code lines,
  its test, a comment), `runtime/hero_compiler.h` 109 to 116, `runtime/parts/sigguard.c` 364 to 371. No checker, lowering
  or emitter change: emitted C identical but `hero_program_sources` (§ 3.2). Compiler tests: § 3.6.
- `needed_for_self_hosting`: no
- `argument`: The CI's Linux x86-64 leg is red at this base on the tree's three cases at `-O2` (812 passed, 3 failed). In
  the emulated x86-64 image the word turns the `calls` case green at `-O2` (13 of 15 against 12). It changes no object
  byte on Darwin or Linux arm64, and none at `-O0` anywhere, the compiler included; at `-O2` it adds 0.6 to 0.7 % of text
  to the runtime and a program. It reaches nothing built apart, no frame deeper than eight, and not Debian's `exit` and
  `fork` frames, which stay red at every level.
- `prediction`: with C landed, the CI's Linux x86-64 job reads **`run: 815 passed, 0 failed`** on its first run after the
  landing (the three green at `-O2` on Ubuntu's frame-pointered glibc), Darwin and Linux arm64 read the counts they read
  before, and the emulated Debian x86-64 image still reads the `atexit` and `fork` cases FAIL at `-O0`.
- `condition`: a CI x86-64 run under the word where any of the three stays red, or a leg where the word moves an emitted
  byte.

### Question 2, candidates B' and B'' (a wrapper with a per-thread depth)

- `verdict`: **object** to B' as built (false refusals on three POSIX images); **object** to B'' as a Linux route (a
  silent miss on both Linux legs); **approve B''-style confirmation as Darwin's depth**, paired with a reach into dylibs
  (§ 7)
- `section`: design.md §1.12 (*"What would make this wrong"*: an abort of a correct program, R1's words naming a cause that
  is not its own); §1.1 (a table mirroring glibc's eight installers and their meanings)
- `implementation_cost`: built: `runtime/parts/siginstall.c` new, **311 lines (B') or 339 (B'')**; `sigguard.c` +17 -4;
  `runtime.c` +6; **0 compiler lines**; widens panel 239's `ffi_defined_elsewhere` (the escape); moves five 2494 goldens
  on both POSIX legs (§§ 2.4, 2.8); about 4 instructions a callback on `qsortcost` (§ 2.5).
- `needed_for_self_hosting`: no
- `argument`: Both reach every glibc installer from a program's header, a static archive, a shared library, a `dlopen`ed
  library and a constructor before `main` (Linux arm64, 26 rows), refuse R3's cases 20 of 20 and storms on both legs, a
  library twelve frames deep and a handler jumping within itself. **B' refuses three correct programs after a jump out,
  on Darwin, Linux arm64 and emulated x86-64**; curl and OpenSSL take that path (historian). **B'' refuses none, and
  misses a handler whose frame register holds a signal-frame address**, which Linux's clang may allocate and Apple's
  never. Neither reaches a Darwin dylib (9 of 9).
- `prediction`: B'' landed as built: `linux/ship/rows/kmiss.hero` reads exit 0 (missed) on the CI's Linux x86-64 and
  arm64 legs and exit 134 on Darwin, and the twelve depth programs exit 0 on every POSIX leg.
- `condition`: for B', a jump-out probe it passes; for B'' on Linux, a run where it refuses `kmiss` without refusing a
  depth program.

### Question 2, candidates A, D-tramp and D-rebind (built by the ffi-pragmatist; priced from the runtime's side)

- `verdict`: **object** to A (its marker is a signal a correct C program may block and wait for, which then dies in
  silence: the ffi-pragmatist's `swait_rt.c`, exit 192, its veto, not reproduced by me); **object** to D-tramp (a dylib
  beside every program, the escape); **object** to D-rebind as specified, **pending the row it lacks** (§ 7), and
  **approve its reach** paired with B''-style confirmation
- `section`: design.md §1.1; CLAUDE.md § 10 (one command: D-tramp's build would emit two files)
- `implementation_cost`: unbuilt by me. A's definitions meet the runtime's 18 mask lines and R2's poller (§ 5); D-tramp
  moves `selfhost/cli/link_flags.hero` (215) and the build's output; D-rebind about 120 runtime lines and the trampoline,
  0 compiler lines (its report).
- `needed_for_self_hosting`: no
- `argument`: (§§ 5, 7.) From the compiler's side D-rebind is the cheapest Darwin reach measured: no file, no name, no
  diagnostic, no compiler line. Its zeroing of the depth at every jump trades the false refusal for a miss that my
  `injump` program should fall into; B''-style confirmation avoids both on Darwin, measured.
- `prediction`: `injump/ij.hero` against D-rebind as specified exits 0, `answer ran for 30` (missed); against D-rebind with
  B''-style confirmation, exit 134; checkable by the synthesis on this Mac before ratification.
- `condition`: D-rebind refusing `injump` 3 of 3 as it stands.

### Question 3, what closes 2522

- `verdict`: **object** to any claim that a candidate built closes 2522 under R3; **object** to D-imports (the
  ffi-pragmatist vetoes it on a built `dlopen` program; my objection rests on its cost and the tree's one correct
  program); **closing 2522 needs the author's change to R3 (route K)**, in those words
- `section`: R3 is panel 247's (*"Never a known cost"*, `docs/panel/247-*.md:191-195`); design.md does not cover a residue
  of the C boundary told rather than closed. §1.12 for the false refusals; §1.1 for D-imports' reader.
- `implementation_cost`: D-imports unbuilt, estimated 150 to 300 lines in `selfhost/cli/` (a new reader of a dylib's
  imports on Darwin, `supplied_kept.hero:221-229`'s lone link widened on ELF, a words module), a new diagnostic.
- `needed_for_self_hosting`: no
- `argument`: Every route built leaves 2522's class somewhere, run: C leaves libraries built apart, deep frames and
  Debian's libc; B' refuses correct programs; B'' misses on Linux; Darwin's executable definitions miss every dylib;
  D-rebind leaves `dlsym` and, by its rule, a jump within a handler; a raw system call passes every route. The best set
  measured (C on x86-64, B'' on Linux, D-rebind with B''-style confirmation on Darwin) still leaves residues R3 forbids.
  No route adds words: refusals are R1's, so **2533's repair lands first** (§ 6.2).
- `prediction`: no route proposed to this sitting refuses both `kmiss` and the twelve depth programs correctly on Linux
  x86-64, by M-core-packages' close.
- `condition`: a Linux route measured to refuse `kmiss`, `injump`, R3's cases and a `dlopen`ed library with no false
  refusal on the twelve depth programs.

Written by the compiler-engineer, 2026-11-14 23:28 to 2026-11-15, resumed 01:47 after a stop. Every container of mine
ended (`ce-249-1` 00:19 and `ce-249-3` 01:08 in the emulated image, `ce-249-2` 02:07 and `ce-249-4` 02:12 in Linux arm64,
each `docker exit 0`); no file written outside my folder and this report.
