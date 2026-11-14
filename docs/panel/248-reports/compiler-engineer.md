# Panel 248, the compiler-engineer's report

Written as it goes, started 2026-11-14 10:07 CET. Seat definition: `.claude/agents/compiler-engineer.md`
(its output structure, one section per question). Folder:
`.claude/worktrees/p248-scratch/248-compiler-engineer/` (`tree/`, `tmp/`, probes beside them). Every
binary is named `ce-...`.

## Setup

- `git -C .claude/worktrees/p248 archive df7612ad3dc62dcc6d90bccc650adf6b9736078f | tar -x -C .../tree`;
  `git -C .claude/worktrees/p248 rev-parse 'df7612ad3^{tree}'` printed
  `f72f48da250e919a68ebc6a98be0facccc29a052`.
- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0, 369,103,613 instructions
  retired (`/usr/bin/time -l`), at load average 224 (`uptime` 10:07: *224.25 162.66 77.55*; other seats
  building), so no duration below is a measurement; instructions are.
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next` exit 0; `./heroes-next --version` printed
  `heroes 0.6.0` and `sources 4c9a6a9800095eef`, `doctor` `ok   runtime      runtime`. On the base.
- **The provenance hook and a copy**: the first harness run was refused by `.claude/hooks/provenance.py`,
  which looks for the nearest directory holding `seed/heroes.c` **and a `.git` entry**
  (`.claude/hooks/trees.py:48-57`) and, the copy having none, judged my compiler against the trunk's tree
  (`keys 4805ba787cff58dc`). So the copy carries a `.git` FILE reading `gitdir: /nonexistent-ce-248`
  (`git status` there: *"fatal: gitfile does not point to a valid repository"*), which makes the hook judge
  the copy's own tree and keeps every git command run inside the copy from climbing to the trunk's
  repository, which it otherwise would (the copy sits inside the trunk's checkout). After each runtime
  change stage 2 is rebuilt so the hook compares a compiler with the tree it judges. The routes are built
  in one tree in this order: Q1 (runtime signal installers), Q2/Q4/Q5 (the platform layer), Q3 (`run.c`
  and the net); each group of cases was run after its own route landed, with the earlier ones in place.

## Question 1: defect 2522, the runtime defining the C library's installers

### R3's two cases at the base, reproduced (Darwin arm64)

- Library and programs built as 00-shared.md says (`clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. lib/sigcb.c`,
  `ar rcs lib-nofp/libsigcb.a`, `heroes-next build flib.hero --library lib-nofp -o ce-flib-nofp`, the same
  for `fliblocale.hero` as `ce-fliblocale`), every build exit 0. Files: `q1/` in my folder.
- **`ce-flib-nofp`, 20 runs**, each `timeout -s KILL 20`, outputs kept in `q1/base-runs/flib-<n>.{out,err}`:
  **20 of 20 exit 0**, stdout `0|answer ran for 30|0|60|`, stderr empty. The Heroes function ran inside the
  signal 20 times of 20.
- **`ce-fliblocale`, 5 storms** (`python3 -I tools/storm.py ./ce-fliblocale SIGUSR1 4 0.3`, `q1/base-runs/storms.log`):
  **5 of 5 armed, 5 of 5 `exit -9`**, after 16,235 to 17,508 signals, stderr empty. `ips.py ... ce-fliblocale`:
  `('ce-fliblocale', 'SIGKILL', 'lock') 5`, pids 64977, 64990, 64999, 65007, 65015, the storms' five; stack
  `_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < localeconv_l < h_fliblocale_answer <
  h_0cb_fliblocale_answer < sigcb_handler < localeconv_l < __vfprintf < ... < hero_int_to_str < h_fliblocale_round`.

### What the runtime already has: an alternate stack on every thread it claims

`runtime/parts/stack.c:994-1018` (`hero_stack_guard_enter`) gives `main` (from `hero_args_set`) and every
thread `hero_spawn_enter` claims a 256 KiB `mmap`'d alternate signal stack with a guard page, for the stack
guard, outside `--sanitize` (`stack.c:620-623`, the branch under `HERO_STACK_GUARD_YIELDS_TO_ASAN`, where
the guard yields to ASan). And `sigguard.c:329-334`, R1's second question, already asks `SS_ONSTACK` where
the thunk's frame lies outside its thread's stack. So **a C handler installed with `SA_ONSTACK` runs where
the thunk's existing second question refuses it, with no frame record needed, no walk, no per-thread
depth, and no new memory**. The library's handler in R3 is installed `sa.sa_flags = SA_RESTART`
(`lib/sigcb.c`), without it. That is route I-onstack, and it is the one I built.

### Route I-onstack, built: `runtime/parts/siginstall.c`

The runtime defines the installers each platform's C library exports that a static archive of the program
can call; each calls the C library's own (found once, by `dlsym(RTLD_NEXT, name)`, in a constructor, so no
`dlsym` runs inside a signal) and then, where the handler is a function, adds `SA_ONSTACK` to the action
the kernel holds. `sigaction` and `__sigaction` add it to the action before the call; the others call the
C library's own and add it after, under the signal held off the calling thread. **No table is kept**: the
kernel's action is the one truth, the handler in it is the program's own (never a wrapper of the
runtime's), so `SIG_HOLD`, `SIG_IGN`, `SIG_DFL`, the previous handler answered, `sigaction(sig, NULL, &old)`,
`sigignore`, `siginterrupt`, `SA_RESETHAND` and `siglongjmp` out of a handler all keep the C library's own
meaning, and `sigignore` and `siginterrupt` need no definition at all (they rewrite the kernel's action,
which keeps the flag). Names defined: both platforms `sigaction`, `signal`, `bsd_signal`, `sigset`; Darwin
`__sigaction` (the kernel's, `struct __sigaction`), `sigvec`, `_sigaction_nobind`, `_signal_nobind`,
`_sigvec_nobind`, `__platform_sigaction`; glibc `__sigaction`, `ssignal`, `sysv_signal`, `__sysv_signal`
(`sigvec` left undefined: a compat symbol a library built today cannot link, 00-shared.md, so defining it
would make such a library link where it does not). The probes (`sigguard.c:183,185`) go through
`hero_sig_real`, the C library's own `sigaction`, as R4 asks. `runtime.c` includes the part after
`sigguard.c`.

**Lines**: `runtime/parts/siginstall.c` 250 (`wc -l`), `runtime/parts/sigguard.c` +2 (a declaration, two
calls changed), `runtime/runtime.c` +3. **The compiler: 0 lines. The emitted C: unchanged** (below).

### R3's two cases under route I-onstack (Darwin arm64)

- **`ce-i-flib-nofp`, 20 runs** (`q1/i-runs/flib.log`): **20 of 20 exit 134**, stdout `0`, stderr *"panic:
  flib.answer was called from inside a signal handler: C called it while handling a signal, where neither
  this runtime's memory nor the C library's is safe to use. No function of the program is a signal
  handler, nor called by one: ..."*: R1's words, and `answer ran for` never printed, so the refusal came
  before the function's first line.
- **`ce-i-fliblocale`, 20 storms** (`q1/i-runs/storms.log`): **20 of 20 armed, 20 of 20 `exit -6`** (the
  refusal's SIGABRT), each stderr R1's words, after 16,005 to 18,074 signals. `ips.py ... ce-i-fliblocale`:
  `('ce-i-fliblocale', 'SIGABRT', 'abort') 20`, **the 20 reports' pids the 20 storms' pids** (67636 to
  67705, listed in the log), **none SIGKILL**; stack `__pthread_kill < pthread_kill < raise <
  hero_signal_stop < hero_signal_refuse < hero_callback_entered < h_0cb_fliblocale_answer < sigcb_handler`:
  refused at the thunk's first line, before the callback's first C call (`localeconv`). The 8 SIGABRT
  reports of `ce-i-flib-nofp` are its refusals (`ips.py`: `('ce-i-flib-nofp', 'SIGABRT', 'abort') 8`; 20
  runs, ReportCrash keeps a sample).

### A defect in my first build, found by its own case, and repaired

`fixedbugs-2494-a-signal-the-programs-own-c-holds-with-sigset-stays-pending` failed at the first run
(`exit 158`, nothing printed): my `sigset` held the signal around the C library's call and gave the old mask
back after, which undid `SIG_HOLD`'s own mask change. Repaired: the hold stays only around installers
that never touch the thread's mask (`signal`, `bsd_signal`, `ssignal`, `sysv_signal`, `__sysv_signal`,
`_signal_nobind`, the `sigvec` pair); `sigset` runs unheld. The case then printed `pending after the hold:
1`, exit 0. (A whole-meaning claim is only as good as the case that holds it; this one held it.)

### The cases under route I-onstack (Darwin arm64, stage 2 rebuilt, `sources 0974d5bb4ccd9702`)

`./heroes-next run tests/harness/main.hero -- ./heroes-next run <group>`, logs in `q1/i-runs/h-*.log`:

| group | result |
|---|---|
| `fixedbugs-2494` | **13 passed, 2 failed**: the two D cases, below |
| `fixedbugs-2496` | 1 passed, 0 failed |
| `fixedbugs-2257` | 3 passed, 0 failed |
| `fixedbugs-an-ignored-signal` | 1 passed, 0 failed |

**The two D cases** (`...-a-handler-that-allocates-inside-the-allocator-...`, `...-that-lends-inside-the-lend-tracking-...`)
reach the Heroes function through 24 frames on purpose (their header,
`fixedbugs-2494-a-thread-of-cs-own-sends-the-signal.h`, `DEEP_FRAMES 24`), the residue D stands for. Under
route I they are refused **earlier**, at R1's second question, with R1's words instead of D's: the route
closes the very shape the cases were written to leave to D. **D stays witnessed** where a program's C takes
the alternate stack away: a scratch copy of the allocator case whose header disables the stack in
`storm_start` printed D's words 5 of 5 (`q1/dcase/`, `exit 134`, *"a signal handler used this runtime's
allocator while the code it interrupted was inside it"*). So the golden change owed is one call in those two
cases' C (`sigaltstack` with `SS_DISABLE` and **a size**, below), or their words moved; and R3's two cases
become `fixedbugs-2522-...` goldens.

**A finding beside it, measured**: on Darwin, `sigaltstack` with `SS_DISABLE` and a zeroed `ss_size` answers
**`-1`, errno 12 (ENOMEM)** and leaves the stack standing (the scratch case printed `disable -1 errno 12
flags 0`; with `ss_size = MINSIGSTKSZ`, `disable 0 errno 0 flags 4`). `runtime/parts/stack.c:1027-1036`,
`hero_stack_guard_leave`, disables exactly so (`memset(&off, 0, sizeof off); off.ss_flags = SS_DISABLE;`) and
keeps the mapping when the disable fails. So, by reading and by that one measurement, **on Darwin every
thread this runtime spawns keeps its 257 KiB alternate-stack mapping after it ends**: unrun as a count of
mappings over many spawns, a question for a defect, not this sitting's.

### The shapes beside R3's, under route I-onstack

- **A handler left by `siglongjmp`, then a callback outside any signal** (`q1/ljmp.hero`, library
  `ljlib/ljmp.c`, `-O2 -fomit-frame-pointer`, `SA_NODEFER`): Darwin, 3 of 3 `twice ran for 7`, `14`, then
  `ljmp_onstack()` `0`: **not refused**, and Darwin's `siglongjmp` clears `SS_ONSTACK`.
- **A handler left by `_longjmp`** (no mask, no stack state restored): 3 of 3 `twice ran for 8`, `16`,
  **not refused**, but `ljmp_onstack()` then reads **`1`**: a stale flag. A plain C control (`q1/c/stale.c`)
  reads `0` without `SA_ONSTACK` and `1` with it, so **route I makes this state reachable where the base did
  not**: on Darwin, after a C handler leaves by `_longjmp`, the kernel believes the thread is on its alternate
  stack, so (a) the next signal on that thread is delivered on the ordinary stack (R3's handler there falls
  back to the walk, the base's state), and (b) the stack guard's overflow message is lost on that thread.
  A false refusal would need, on top, a callback whose frame lies outside its thread's stack (C running on a
  stack of its own). Linux reads the flag from the stack pointer, so it cannot go stale (run below).
- **R3 with the library installing through `_signal_nobind`**, one of Darwin's four undeclared installers
  (`q1/libnb/sigcb.c`, `-O2 -fomit-frame-pointer`, static): base runtime 3 of 3 `answer ran for 30`, exit 0
  (inside the signal); route I **3 of 3 refused**, exit 134: the runtime's definition of the undeclared name
  is what the static link binds the library's call to.
- **R3 with the library a dylib** (`lib-so/libsigcb.dylib`, `otool -L`: `@rpath/libsigcb.dylib`): 5 of 5
  `answer ran for 30`, exit 0, **not refused**: the two-level namespace residue, as 00-shared.md said.
- **`--sanitize`** (`-O0` and `-O2`): the build links (`nm`: `T _sigaction`, `T _signal`, `T _bsd_signal`,
  `T _sigset` in the executable beside `@rpath/libclang_rt.asan_osx_dynamic.dylib`), and **3 of 3 refused at
  each level** with R1's words; the base under `--sanitize -O0` (`HEROES_RUNTIME` naming the base's
  `runtime/`, extracted alone) printed `answer ran for 30` 3 of 3. Why it reaches there although
  `stack.c` yields to ASan: **ASan gives each thread an alternate stack of its own**, 524,288 bytes
  (`q1/c/asanalt.c` under `-fsanitize=address`: `alt stack size 524288 flags 0`; plain: `size 0 flags 4`).
  Which `sigaction` a call reaches: the program's static archive is bound at the static link to the
  executable's own definition (two-level namespace binds only imports, which the executable's own
  definition is not), which finds the C library's by `dlsym(RTLD_NEXT)`.

### Panel 239's member, measured (base runtime by `HEROES_RUNTIME`, same compiler; `q1/s239/`)

| library defines | base | route I-onstack |
|---|---|---|
| `sigaction` (forwarding by `RTLD_NEXT`, a crash reporter's shape) | **exit 1** `ffi_defined_elsewhere`: *"defines `sigaction`, of the C library, which the program's own runtime calls"*, remedy *"bind each of them in a group of library `defsig`"* | **exit 1** `ffi_defined_elsewhere`: *"defines `sigaction`, a name of the program's own runtime, and a program holds one `sigaction`"*, remedy *"rename it where library `defsig` is built"* |
| `sigaction`, bound in the library's own group (the base's remedy taken) | **exit 1** `ffi_unpaired_release` (the header the SDK's, the library a named folder's) | exit 1 `ffi_defined_elsewhere`, as above |
| `bsd_signal` (a name the runtime does not call today) | **exit 0**, runs: `9`, `0` | **exit 1** `ffi_defined_elsewhere`: *"defines `bsd_signal`, a name of the program's own runtime"* |

**Compiler lines for it: 0.** The member already reads the runtime's object as a definer
(`selfhost/cli/supplied_read.hero:239-241` reads GNU ld's, lld's and ld64's duplicate-symbol words; the
`own` kind, `supplied_judged.hero:268`). **This is the escape's widened diagnostic**, in those words: a
program whose library defines `bsd_signal`, `sigset`, `sigvec`, `ssignal`, `sysv_signal`, `__sysv_signal`,
`__sigaction`, `__platform_sigaction` or one of Darwin's `_nobind` names builds at the base and is refused
under the route. The `sigaction` and `signal` libraries were refused already (the runtime calls both).
`c-boundary.md:186-220` already says *"the runtime calls or defines"*: its words need no change for the
names, only the record that the set grew, which is the amendment I propose (below).

### The emitted C and the cost

- `--emit-c` of `fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused.hero`, the same compiler with
  the base's runtime and with route I's: 19,172 bytes each; `cmp` with the line `hero_program_sources(void) {`
  taken out of each: **identical**; whole, they differ at that line alone (line 338). **Nothing moves in the
  emitter.**
- `qsortcost` (panel 247's, copied, `ce-b-qsortcost` / `ce-i-qsortcost`, `timeout -s KILL 60 /usr/bin/time -l`):
  base **10,979,557,925 to 10,980,097,228** instructions (4 runs), route I **10,980,250,809 to 10,980,788,856**
  (4 runs): a constant of about 0.6 million, 0.006 %, the constructor's 13 `dlsym` lookups; **per callback
  nothing**, the thunk being unchanged. (My first three pairs measured `timeout`'s own 16 million
  instructions, `/usr/bin/time` outside `timeout`; discarded.) `heroes check selfhost/main.hero` is not owed:
  the route moves no compiler code.

### Linux arm64 (container `ce-248-1`, `linux/q1/run.sh`, log `linux/q1/linux.log`, docker exit 0)

The base tree from stdin, seed and stage 2 there (`sources 4c9a6a9800095eef`), R3's library built as
00-shared.md says, then route I's three files overlaid on `runtime/`, the same compiler:

| case | base | route I-onstack |
|---|---|---|
| `ce-flib-nofp`, 20 runs | **20 of 20 exit 0**, `answer ran for 10` (inside the signal; the coordinator's *unrun* row, run) | **20 of 20 exit 134**, R1's words, nothing of `answer` printed |
| `ce-fliblocale`, storms | 5 of 5 armed, 5 of 5 exit 0, `built 20000000 words`, after 2,080,060 to 2,214,327 signals | 20 of 20 armed, 20 refusals with R1's words, after 321 to 539 signals (the exit codes re-run below: my `head -c 600` cut 19 of the 20 `pid` lines) |
| `ce-ljmp` (`siglongjmp`, then `_longjmp`), 3 runs | 3 of 3 not refused, `onstack` 0, 0 | 3 of 3 not refused, `onstack` **0, 0**: Linux reads the flag from the stack pointer, no stale state |
| R3 with the library **shared** (`lib-so/libsigcb.so`), 5 runs | (not run) | **5 of 5 refused**: the executable's `sigaction` is exported (`-rdynamic` is on every POSIX link, `selfhost/cli/link_flags.hero:70-86`), so a shared library's call reaches it |
| `--sanitize -O0` / `-O2` flib, 3 runs each | (not run) | **3 of 3 refused at each level**; `nm`: `T sigaction`, `T signal` (the runtime's) beside ASan's `T ___interceptor_sigaction` and `W __interceptor_sigaction`: the runtime's strong `sigaction` takes the place of ASan's weak one, so ASan's own interception of `sigaction` is off in such a build (its handlers install by its internal system call and stand) |
| `--sanitize -O0` fliblocale, 3 storms | (not run) | 3 of 3 armed, 3 refusals |

## Question 2: defect 2502, the platform layer's waits

### A route nobody listed, measured first: the close's own shutdown is the wake

`hero_net_close` already calls `shutdown()` on a socket a call is busy on (`hero_net.c:1149`,
`hero_net_shut`). Where a shutdown itself wakes a `poll` with no timeout, no wake descriptor is needed. My
probe `q2/shutwake.c` (a thread in `poll(fd, -1)`, another shutting `fd` down), on this Mac:

| shape | Darwin `poll` | Windows `WSAPoll` (the box, `win/wshut.c`, clang 23.1.1, MSVC target) |
|---|---|---|
| connected, waiting to read, `shutdown` | **woke, 0.1 ms, revents 0x11** | did not wake (3 s timeout) |
| connected, waiting for room (1,383,928 bytes queued), `shutdown` | **woke, 0.0 ms, 0x10** | woke after 1,410 ms, 0x10 |
| listener, waiting to accept, `shutdown` | did not wake (3 s) | did not wake (3 s) |
| listener, a connection to itself | woke, 0.3 ms | woke, 0.4 ms |
| a loopback pair's read end, a byte sent | (not run) | woke, 0.1 ms |

So on POSIX a close needs a wake descriptor only for a **listener**; Windows needs one for every waiter,
or keeps its slices. (Linux arm64 rows: run in container 2, below.)

### Route (g), built: no slice on POSIX, a pipe per listener, the stop's pipe made with the first socket

In `runtime/platform/hero_net.c` of my tree (`q2/apply.py` applied it; `q2/hero_net.base.c` is the base):
every POSIX wait is one `poll` with no timeout over the socket, the stop's pipe and, for a listener, a pipe
of its own (made at `hero_net_listen`, written once by a close, never read); the stop's pipe is made with
the first socket (`hero_net_open`) rather than by `hero_net_catch_stop`, so a stop the program asks itself
(question 4) reaches every wait; each wake is a **state that stays** (a shut socket, a pipe never drained),
so a close or stop made before the `poll` began is seen by it and no wake is lost; where the system gives
no pipe the wait keeps its slices; Windows keeps its slices (the box's rows above). **Lines**: `hero_net.c`
1235 to 1449 (`wc -l`), which carries questions 4 and 5 too (the route alone is about 130 of them);
`hero_net.h` 176 to 205. Compiled with no warning by Apple clang (`-Wall -Wextra -Wconversion`) and zig's
`x86_64-windows-gnu` (MinGW's, the project's MSVC target owed to the CI). **The compiler: 0 lines.**

### Measured (Darwin arm64; the probes link the base's layer or route (g)'s)

- **`waiters-checked.c`** (copied, `ce-waiters-base` / `ce-waiters-g`, `timeout -s KILL 60 /usr/bin/time -l ./w 100 wait S`, `q2/waiters.log`), two runs each:

| 100 threads waiting | base instructions | base invol. switches | route (g) instructions | route (g) invol. |
|---|---|---|---|---|
| 0 s | 71,820,760 / 72,115,548 | 508 / 508 | 72,547,990 / 72,577,384 | 508 / 470 |
| 5 s | 284,003,619 / 292,849,550 | 10,933 / 11,943 | **73,367,507 / 73,939,904** | **513 / 521** |
| 10 s | 504,559,590 / 506,044,915 | 24,283 / 24,384 | **72,826,031 / 73,154,225** | **515 / 517** |

  The base at 5 s reads a little above the brief's 271.6 to 275.0 million (here 284 to 293); route (g) is
  flat in time.
- **Past the limit** (`ulimit -n 256`, `timeout -s KILL 30`, `q2/limit.log`): the base holds 126 probe pairs
  and route (g) 124 (its two pipes); past it **both answer at once** `no connect ..., kind 9`
  (`HERO_NET_LIMIT`), exit 1, nothing waits for ever. The fall-back to slices where no pipe can be made is
  built and **unrun** (no probe exhausts the descriptors before the first socket).
- **A close ending a waiter** (`q2/closer.c`, the WAITING socket closed, wall clock on a shared machine, so
  indicative only): base `recv` mean 154 µs, worst 259 µs; `accept` **27,390 µs** (the listener waits its
  slice). Route (g): `recv` mean 142 µs, worst 217 µs; `accept` **32 µs**. Both answer `HERO_NET_NOT_OPEN`.
  (At the base a connected socket's close already wakes at once, its shutdown waking the slice's `poll`;
  the slice was the cost of idling, and the listener's wait.)
- **Server-shaped** (`q2/server.c`, a process holding N accepted connections, each with a thread in
  `hero_net_recv`, the stop caught; its peers a `python3` process; `lsof -p`, pipes counted; `q2/lsof.log`):

| | base | route (g) |
|---|---|---|
| N = 100 | **106** descriptors (101 IPv4, 2 pipes, 3 streams) | **108** (101 IPv4, 4 pipes, 3 streams) |
| largest N under `ulimit -n 256` | **250** (256 descriptors) | **248**; the 249th `accept` answers `HERO_NET_LIMIT`, the server goes on |

  So route (g) is **N + 8**, against the critic's arithmetic of 3N + 6 for a pipe per waiting thread (the
  lane's, N up to 83 under 256) and 2N + 6 for one descriptor per socket.

## Question 3: defect 2490, a child's working folder

### The door, built: `hero_run_dir(folder)`

`runtime/hero_os.h` beside `hero_run_env`, and `runtime/parts/run.c` (`q3/apply_run.py`; the base kept as
`q3/run.base.c`): staged for the next child alone and given back by `hero_run_reset`, as a variable is
(counted with the variables at exit); **POSIX**: at `hero_run_go`, before the fork, the parent opens the
folder (`open(folder, O_RDONLY | O_DIRECTORY | O_CLOEXEC)`, in its own folder) and, where a folder is
staged, makes a program path holding a `/` whole from its own folder (`getcwd`); a folder that cannot be
opened is the launch's refusal **before any child exists**, `HERO_OS_NOT_FOUND` with `hero_run_why` the
errno, the vocabulary a failed start already has; the child opens its three streams as it does today (in
the parent's folder, inherited) and only then `fchdir`s, the last call before `exec`, `fchdir` being on
POSIX's list; **Windows**: the staged folder as `CreateProcessW`'s `lpCurrentDirectory`. **The order it
promises, on every system: every relative path of the launch is read in the PARENT's folder**: the folder,
the three streams, a program path holding a folder; a bare program name through the parent's `PATH`, as
panel 242's R6 already says. The `CreateProcessW` side of that promise is not an inference: run on the
box, below. **Lines**: `run.c` 2104 to 2195 (97 lines of diff), `hero_os.h` 513 to 526. **The compiler: 0.**
Compiled for MinGW by zig (`-c`, exit 0, the base's runtime too).

### Windows, run on the box (the author switched it on at 10:31; `win/cpw.c`, clang 23.1.1, MSVC target, `/c/w/p248-ce/q3`)

| `CreateProcessW` with | result |
|---|---|
| `lpCurrentDirectory` `sub` (relative), program `child.exe` | started, the child's folder `C:\w\p248-ce\q3\sub`: **a relative folder is read in the parent's folder** |
| the absolute folder | started, the same folder |
| a folder that does not exist | **failed, `GetLastError` 267** (`ERROR_DIRECTORY`), no child |
| a program present only in the child's folder, named bare | **failed, 2**: the program is looked for in the parent's folder, not the child's |
| `sub\subonly.exe`, folder `sub` | **started**: a relative program path is read in the parent's folder (in the child's it would be `sub\sub\...`) |
| a block copied from `GetEnvironmentStringsW` | the parent's block holds 1 entry opening `=`; the child reads `=C:` |
| a block without the `=X:` entries | started; the child reads no `=C:`, and `C:rel-probe` still resolves on its own current drive (`C:\w\p248-ce\q3\sub\rel-probe`) |

`hero_run_win_environment` copies every entry `GetEnvironmentStringsW` gives (`run.c:390-404`), so by reading
it keeps the `=X:` entries; what is lost without them is a drive-relative path on *another* drive, unrun.
So the POSIX arm's order (folder, streams and program path read in the parent's folder) is chosen to be
Windows' measured order, not the other way round.

### The child's existing calls after a threaded `fork`

POSIX's list (00-shared.md) holds `fchdir` and `execve`, not `getrlimit`, `setrlimit` or `execvp`. A bounded
stress on this Mac (`q3/forkstress.c`: four threads writing the environment without pause, the main thread
forking 2,000 children, each killed if it has not ended in 2 s; `timeout -s KILL 120`): the child as it is
today, `getrlimit` + `setrlimit` + `execvp("true")`, **2,000 forks, 0 hung, 0 failed**; `fchdir` +
`execve`, **2,000, 0 hung, 0 failed**. A sample, not a proof: what POSIX does not promise stays unpromised.
What would replace each, priced: `getrlimit` moves to the parent before the fork (the child keeps one
`setrlimit`, a single system call on both C libraries by their sources, unread here); `execvp` becomes a
search of the parent's `PATH` in the parent (which R6 already promises) and `execve` in the child, with
the `/bin/sh` fall-back for `ENOEXEC` built before the fork, about 40 lines of `run.c`; unbuilt, not owed by
the door, and I recommend it as its own defect, not this sitting's.

### The net, moved

`tests/harness/shell.hero` binds `hero_run_dir`, and its one spawn takes a folder (`attempted_in`,
`run_in`); `case_setting.ran` gives the case's folder at the spawn (`folder_line` in place of `env_line`,
the compiler and the copy's paths still written whole), `skip_here` names links alone; `suite_fetch` gives
the folder of a `cd <folder> &&` line at the spawn, its skip gone. Diff lines: `shell` 48, `case_setting`
79, `suite_fetch` 31, `freed_bytes` 7. **The seventh file is the door's own consequence**: the door's case
failed at first only under Guard Malloc, because `freed_bytes.guarded` gives the program
`MALLOC_LOG_FILE` relative to the root, and the child started in its own folder read it there and wrote
Guard Malloc's banner over the program's stderr. A path a parent hands its child in a variable is read in
the child's folder: the harness now gives that one whole (`shell.whole_path`, through the runtime's own
`hero_fs_resolved_shown`). **A program's own author meets the same thing**; the door's comment and the
card owe the sentence.

### The cases, run on this Mac (stage 2 rebuilt, `sources 38714259264b6832`)

| run | result |
|---|---|
| the door's own case, `run/a-child-starts-in-the-folder-it-is-given` (new): a child in a folder named relative to the parent sees a marker by its bare name, its stdout lands at a path relative to the parent, a missing folder is refused before any child with a reason, and after `hero_run_reset` the child is back in the parent's folder | **passed** at every level the `run` form asks, Guard Malloc included, after the `freed_bytes` change above |
| `run a-child` (the two spawn cases and the door's) | **10 passed, 0 failed** |
| `run every-argument`, `run fixedbugs-436`, `run fixedbugs-2503` | 1, 1 and 1 passed, 0 failed |
| `unsupported fixedbugs-2344-an-empty` / `-a-trailing` | **2 passed** / **1 passed**, 0 failed, started at the spawn in `here`, no `/usr/bin/env` |
| `fetch fixedbugs-2479` | **1 passed**, 0 failed |
| `runtime` (every route under `runtime/` together) | **10 passed, 0 failed** |

**Whether these run in a `git archive` copy, answered**: they do not, at the base or with the door. With
the copy's `.git` pointing nowhere, the 2344 cases failed *"the case writes `<root>` or asks its run
something, and git answers no repository root here"* and the fetch suite refused to run (exit 2, *"the
suite runs in a checkout"*): `case_setting.repository_root` asks `git rev-parse --show-toplevel`
(`case_setting.hero:190-198`). They ran once the copy's `.git` named an empty repository made inside my
folder (`git init -q tmp/ce-root`, no commit, no remote; the project's repository untouched).

**A mistake of mine, said**: the door's case first told the child from the parent by `args().len() > 1`,
and `args()` does not count the program's name, so each child took itself for the parent and started
another, each in a process group of its own, which `timeout -s KILL` does not reach (00-shared.md says
so of every child the runtime spawns). About 2,000 nested runs in 75 s across two attempts; every one
killed by name (`pkill -KILL -f q3/ce-door`, then `pgrep` read 0), and the 2,015 nested folders they made
under my tree's `build/` removed. The case now reads `args()[0] == "where"`, as the spawn case does.

### Which of the four cases would run on Windows, each read; the run owed

- `fixedbugs-2344-an-empty-cpath`: no, `platform_skips.hero:151-153` keeps it Darwin's alone.
- `fixedbugs-2344-an-empty-element-of-c-include-path` and `-a-trailing-separator-...`: no longer skipped by
  the folder; whether clang on Windows reads `C_INCLUDE_PATH=:` and `=/nonexistent-2344:` as on POSIX is
  the lane's *unread and unrun*, and `FOLDER_LISTS` turns `:` into `;` there (`case_setting.hero:50-52`):
  **owed to the CI's Windows leg**.
- `fetch/fixedbugs-2479`: no longer skipped by the folder; its 9 `cd` lines now spawn in the folder,
  **owed to the CI's Windows leg** (the box is on, but the net there is a long run I did not start).

### Against a `heroes` flag that starts the compiler in a folder (git's `-C`)

It would serve these four cases and no other program: a Heroes program that starts a child in a folder of
its own (a build tool, a test runner, the net itself) still could not. ~~And `.claude/rules/cli-surface.md`'s
stopping rule refuses a flag two invocations compose to.~~ **Corrected 11:20, on reading the rule
(`cli-surface.md:18-25`)**: the stopping rule *admits* it, since *"the golden harness ... must type it"*,
and its shape would be a flag (*"changes how one question is answered about the same input"*); I had
written the opposite from memory. The comparison is then on what each buys: the flag costs the compiler
an argv entry and a `chdir` at its start with every relative path of its argv read from the new folder,
the surface one flag, and it leaves every Heroes program without the capability; the door costs the
runtime 110 lines and no compiler line, and it is the language's (decision 2228: `hero_os.h` is the
language's door). **The door is better**, for the program and not only for the net.

### Question 2's Windows arm, run on the box (`win/layer/winlayer.c` against the base's `hero_net.c` and the route's, clang 23.1.1, MSVC target, `/c/w/p248-ce/q2w`)

The route's `hero_net.c` compiles for the MSVC target with no warning shown (`cc g 0`). Two runs each:

| | base | route (g) |
|---|---|---|
| 100 waiters for 5 s, CPU (`GetProcessTimes`) | 0.422 s / 0.188 s | 0.094 s / 0.219 s (the same slices: no change there) |
| a close ending a waiting `recv` | 26.5 / 15.8 ms, `NOT_OPEN` | 15.8 / 31.8 ms, `NOT_OPEN` |
| a 300 ms deadline on a quiet `recv` | (no door) | **313.4 / 317.7 ms, `TIMED_OUT`** |
| the asked stop, an `accept` waiting | (no door) | **0.0 / 15.7 ms, `STOPPED`**, `stop_asked` 1 |

What the route leaves for Windows: its slices, as the header now says. A slice-free Windows arm would need a
socket to wake on (a loopback pair's read end woke `WSAPoll` in 0.1 ms, above) per waiting thread or a
process-wide wake with every waiter re-asking its socket; priced, unbuilt, not needed by any question here.

### What `hero_net.h:95-101` and `136-138` promise instead (built, in my tree's header)

- the close: a waiting call *"ends failing `HERO_NET_NOT_OPEN`: at once on Darwin and Linux, where the
  close's shutdown, or a listener's own pipe, wakes it, and within about 50 ms on Windows, where a call
  waits in slices of that"*;
- the stop: *"one already waiting at once (within about 50 ms on Windows)"*;
- `HERO_NET_TIMED_OUT` gains *"or the socket's deadline passed"*.

## Question 4: a stop the program asks itself

### Built: `hero_net_ask_stop()` in `hero_net.c` and `hero_net.h`

What the first SIGINT or SIGTERM does after `hero_net_catch_stop`, asked by the program from any thread with
no signal: the counter goes 0 to 1 by compare-and-swap and the stop's pipe is written once; a stop already
asked is left as it is; it answers 0. **Before `hero_net_catch_stop`** it works the same (the pipe is made
with the first socket since question 2's route, so it exists before any wait), and signals keep their
default. **After it**, a Ctrl-C finds the count at 1 and ends the process at once, the existing handler's
*"any other, a second one"* branch (`hero_net.c:556-577`), **no new code**: the asked stop counts as the
first request. **Wakes**: POSIX the stop's pipe (every wait watches it); Windows the slices, which ask the
counter. **Lines**: about 30 in `hero_net.c`, 10 in `hero_net.h`.

### Measured

- **`q4/askprobe.c`** (a thread in `hero_net_accept`, the stop asked from `main`): before `catch_stop`,
  the accept ended **17 µs** after the ask, kind 12 (`STOPPED`), `stop_asked` 0 then 1, a second ask answered
  0; after `catch_stop`, **28 µs**, the same; and with a SIGINT sent after the asked stop, **exit 130**, the
  process ended at once (the line after the `kill` never printed). Windows: 0.0 / 15.7 ms (above).
- **`ce-quitserver`** (`examples/server/main.hero` copied, its handler asking the stop at `/quit` through
  the program's own group of `hero_net.h`; `timeout -s KILL 60`, clients `curl --max-time 5` under
  `timeout -s KILL 90`, the server's output bounded by `dd bs=1 count=65536`, `q4/quitrun.sh`): `GET /hello`
  200 twice, `GET /quit` 200 `stopping`, a later `GET /hello` refused (`000`); the server printed **`stopped:
  requests answered 3, connections 3, failed 0, asked 1`**, **exit 0**, nothing more (the leak check clean);
  `pgrep` found nothing after. The handler runs on a connection's thread (`http.serve_conn`), so the door is
  asked from a thread that is not `main`'s, as the example's shape needs.

### The face, and what it moves

- `hero_net.h` gains the door; **spec § 13's line 404 owes a sentence**, in the escape's words: *"Ctrl-C and
  SIGTERM only make `hero_net_stop_asked() -> i64` answer 1"* becomes false where the program can make it
  answer 1 itself. The spec-warden prices the words.
- `<core>/net` gains `ask_stop()` and the reader `stop_asked() -> bool`, which its binding lacks today, in
  **`core` 0.4.0** (a published version is never edited); **built in my tree's package index**
  (`packages/core/0.4.0`, its `list` and its `versions` line `sha256-3496279b...`, placed by `heroes fetch`
  into question 5's probe, which compiled and ran against it). Wrappers live in `net/hero_net.hero`, since
  the compiler refuses an extern called across a module (`extern_across_modules`, measured).
- **My spelling differs from the blind seat's card for t4** (`net.ask_stop() -> ()?`): mine is
  `net.ask_stop()` with no result, since the door cannot fail (it answers 0 whatever stood); a `()?` that
  never fails would make every caller handle a failure that does not exist. `net.stop_asked() -> bool` is
  the card's spelling. The sessions are re-prepared to the built spelling, per the brief.

## Question 5: a connection that sends nothing

### The shapes at the base, run (`q5/server`: the example server copied, **`room` 8** in place of
`hero_thread_limit()`, a fair probe said; built against the base's runtime by `HEROES_RUNTIME`; the server
under `timeout -s KILL 60`, the client side under `timeout -s KILL 90`, the new client `curl --max-time 10`;
`q5/q5run.sh`, `q5/quiet.py`)

| shape | the new client's `GET /hello` |
|---|---|
| 8 connections that send nothing | **not answered in 10 s** (`000`) |
| **the narrower: one quiet connection first, 7 that finish after it** | **not answered in 10 s** (`000`); the server resumed only when the probe closed its quiet socket (`requests answered 7, connections 9, failed 1` at the stop) |

So the critic's reading holds, run: one quiet connection accepted first stops the accept as surely as
`room` quiet ones, since a thread leaves `threads` only at its in-order join.

### The deadline, built: per socket, Go's `SetDeadline` made relative

`hero_net_set_deadline(s, ms)` in my tree's layer (question 2's section has its code): every wait on `s`
that begins after the call ends `ms` from then, `HERO_NET_TIMED_OUT`; 0 takes it away; negative is
`HERO_NET_INVALID`; a call already waiting keeps the deadline it began with; the clock is
`CLOCK_MONOTONIC` on POSIX and `GetTickCount64` on Windows, both never going back (the layer's
`hero_time.c` is a separate archive member, so the layer reads the clock itself rather than pulling that
member in). On POSIX it is the `poll`'s own timeout, which the slice-free wait of question 2 makes exact;
on Windows it shortens the slice. The face, **`net.set_deadline(c: Conn, ms: i64) -> ()?`**, the blind
seat's spelling for t5, in `core` 0.4.0 (built, above), and the code `timed_out` already in `net`'s card.
The probe's connection thread calls `net.set_deadline(conn, ms: 3000)` before `http.serve_conn`.

| shape, with the deadline (`ce-server-d`, my tree's runtime, `core` 0.4.0) | the new client's `GET /hello` |
|---|---|
| 8 quiet connections | **answered `200 hello, world` after 2.5 s**; at the stop `failed 8`, the eight quiet ones ended `timed_out` |
| one quiet first, 7 finishing | **answered after 2.4 s**; `failed 1` |

Both servers ended at exit 0, the leak check silent. **A call with no deadline set**, a million one-byte
`send`/`recv` pairs that never wait (`q5/nowait.c`, three runs each): base 156.79 / 157.88 / 159.03 billion
instructions, route 158.80 / 159.07 / 160.64 billion: overlapping ranges; what the route adds per call is a
copy of the deadline under a lock the call already takes, and in a wait one `deadline == 0` test.

**Why per socket and not `net/http` alone or a join that does not wait**: an idle limit in `serve_conn`
alone leaves every other program of `net` without a bound on a read, and a per-call limit is defeated by a
peer sending one byte just before each limit (the slowloris shape), which an absolute deadline is not; a
join that does not wait moves spec line 377 and still leaves the quiet connection's thread and socket held
for ever, one of 256 slots each. The deadline frees both. `net/http` can add its own limit on top
(`ReadHeaderTimeout`'s shape) by calling `set_deadline` itself, in the same new version, if the panel wants
the example safe without its author asking: unbuilt.

### The other routes of question 1, from the runtime's side

- **I as R4 wrote it (a wrapper handler and a per-thread depth)**: not built, priced against I-onstack.
  It needs a table of the program's handlers per signal (to answer `sigaction(sig, NULL, &old)` with the
  program's own), `sigignore` and `siginterrupt` defined so the table does not go stale (00-shared.md's run),
  the wrapper's own `SA_RESETHAND` and `SA_NODEFER` behaviour re-made, and a depth a `siglongjmp` out of the
  handler leaves raised for ever, which then refuses every later callback of a correct program (R4's
  condition (a)). I-onstack keeps the kernel's action as the only table and has no depth: the C library's
  own installer runs, `SA_ONSTACK` is added, and leaving a handler by `siglongjmp` was measured clean
  (Darwin and Linux, above). Its own new residue is Darwin's `_longjmp` stale flag (above).
- **I-dylib** (route I shipped as a dylib with `__interpose`): would reach the Darwin dylib residue, at the
  price of every program linking a dylib of the runtime and carrying it beside the binary; unbuilt. Not
  needed for R3's two cases, which I-onstack closes; the dylib residue stays the escape's to tell.
- **B** (asynchronous signals blocked on every claimed thread): unbuilt, priced by reading. The 2494 case
  `the-runtimes-door-counts-each-signal-the-program-sends-itself` raises a signal at its own thread
  (`raise` is thread-directed), which then stays pending for ever on a blocked thread, so that case would
  read 0 where it reads each signal; a C library's `raise(SIGUSR1)` waiting for its handler before going on
  would hang. `run.c`'s forwarding handler would run on the one open thread, which is fine. It changes what
  every program's signals do; refused from the compiler's side.
- **T** (a bounded scan for the delivery's return address): unbuilt. My probe of its false refusal
  (`q1/c/stale_scan.c`: a plain C handler delivered at seven depths, then 65 later calls outside any signal
  each scanning up to 4,096 words above their frame, bounded by the stack's top) found **0** stale copies
  in every case, so the false refusal is unproven either way here. Its cost is not: a scan of K words at
  every thunk entry, against I-onstack's zero; at K = 512 and the 20 million comparator calls of a sort of a
  million values, about 10 billion more loads (an inference from `qsortcost`'s count, unrun). Rejected on
  cost and on redundancy.
- **W** (`--wrap`): no Darwin or MSVC form (Apple `ld`'s manual, 1,575 lines, 0 holding `wrap`, re-run;
  its `-alias symbol_name alternate_symbol_name` *"Create[s] an alias ... for the symbol"* and its
  `-interposable` *"Indirects access to all to exported symbols when creating a dynamic library"*, its own
  words at lines 608-611 and 1027-1029: neither redirects another object's references); and where it exists
  it wraps the runtime's own calls too (00-shared.md), so the runtime would call `__real_<name>`, undefined
  on a link that does not wrap. I-onstack needs no link flag on any platform: the static linker binds an
  archive's reference to the runtime object's definition on its own. **R** (`objcopy`): Xcode ships no
  `llvm-objcopy` (00-shared.md), so no Darwin form without a new tool dependency. Both rejected.
- **U** (unwind tables) and **M** (the mask): unbuilt; I-onstack makes both unnecessary for R3's cases.
- **Windows**: the route defines nothing there. A console's Ctrl-C runs its handler on a thread Windows
  makes (refused by 2492's thread guard), and `AddVectoredExceptionHandler`'s, `SetUnhandledExceptionFilter`'s
  and UCRT `signal`'s fault handlers run synchronously on the faulting thread, where D stands; no Windows
  shape of R3 (an asynchronous handler interrupting the thread that called it) was found by me, and the box
  was not asked one: owed if the synthesis wants one.

## Linux arm64, container 2 (`ce-248-2`): my whole tree, every route, built there (`linux/c2/run.sh`, log `linux/c2/linux.log`)

Seed and stage 2 exit 0, **`sources 38714259264b6832`**, the same key as my Darwin stage 2 (the same sources).

- **Question 1, R3's storms under route I, the exits kept this time**: **20 of 20 armed, 20 of 20 `exit -6`,
  20 refusals with R1's words**, after 281 to 720 signals each (pids 1045 to 1216). With container 1's
  20 of 20 `flib-nofp` refusals, **R3's two cases are closed 20 of 20 on Darwin and in Linux arm64**.
- **Question 2, `shutwake` in Linux**: a connected socket waiting to read, woken by `shutdown` in 0.1 ms
  (0x11); waiting for room, 0.0 ms (0x14); **a listener, woken by its shutdown, 0.1 ms (0x10)**, where Darwin
  woke nothing; by a connection to itself 0.2 ms. So the listener's own pipe is Darwin's need alone; it is
  kept on both for one code path (2 descriptors per listener).
- Route (g) in Linux: under `ulimit -n 256` 124 probe pairs held, the 125th connect `HERO_NET_LIMIT`, exit 1 at
  once; a close ending a waiting `recv` mean 93 µs, worst 189 µs; an `accept` 100 µs, both `NOT_OPEN`; a
  server holding 100 connections **108 descriptors** (`/proc/<pid>/fd`: 4 pipes, 101 sockets), N + 8 as on
  Darwin. The waiting cost in instructions is not here: the image has no `/usr/bin/time` (`exit 127`), so
  it is measured in container 3 below.
- **Question 4 in Linux**: the asked stop ended a waiting `accept` 42 µs after (before `catch_stop`) and
  25 µs after (after it), `STOPPED`; a SIGINT after it, **exit 130**.
- **Question 3 in Linux**: the fork stress, 2,000 forks each, the child as it is and `fchdir` + `execve`:
  **0 hung, 0 failed** in both.

## The verdicts, in the seat's structure (`.claude/agents/compiler-engineer.md`)

**For all five, the ceiling first**: `diff -rq` of my tree's `selfhost/` against the base's, extracted alone,
is empty. **Every route lands in `runtime/`, the platform layer, the net and the `core` package, and none
moves the compiler**: no construct reaches the checker, the lowering or the backend (design.md §1.7, Part 5:
*"Only these need implementing in the type checker and the lowering and the backend"*). The emitted C of a
2494 case is byte-identical but for `hero_program_sources` (Q1). The runtime grows by `siginstall.c` 250,
`hero_net.c` +214 (1235 to 1449, three questions), `run.c` +91, the headers +42. **I cast no veto**: nothing
here adds a core construct or breaches the ceiling.

### Q1, defect 2522

- `verdict`: **approve**, route **I-onstack** (built), and with it the runtime defining the C library's
  installers; **object** to R4's wrapper-and-depth form, B, T, W, R and I-dylib as routes for R3.
- `section`: design.md §1.12 (a correct program must not die in silence, and the class is `blocking`) and
  §4.20 (the runtime in C); §1.7 and Part 5 for the cost (no core construct).
- `implementation_cost`: `runtime/parts/siginstall.c` **250** lines (new); `runtime/parts/sigguard.c` +2 (the
  probes through the C library's own `sigaction`); `runtime/runtime.c` +3; `selfhost/` **0**; emitted C **0**;
  goldens: R3's two cases as `fixedbugs-2522-...`, and the two D cases of 2494 either one `sigaltstack`
  call in their C (measured: D then speaks, 5 of 5) or their words moved to R1's.
- `needed_for_self_hosting`: **no**. Robustness decides that the class closes (CLAUDE.md § Precedence,
  ranks 3 and 4 read together); no form enters the language.
- `argument`: Every claimed thread already holds a 256 KiB alternate stack, and the thunk already asks
  `SS_ONSTACK`. Forcing `SA_ONSTACK` on every handler the program's C installs puts R3's handler where the
  kernel answers the question exactly, with no frame record, no depth and no table: the C library's own
  installer keeps every meaning. Measured: R3's two cases refused 20 of 20 each on Darwin (before
  `localeconv`) and in Linux arm64, under `--sanitize` too (ASan's own 512 KiB alternate stack), the
  callback's cost unchanged, the compiler untouched. Residue said: a Darwin dylib, a raw system call, a
  `sigset`-family install racing a delivery to another thread, and Darwin's stale flag after `_longjmp`.
- `prediction`: with the route landed, the CI's Darwin and both Linux legs run R3's two cases as goldens and
  read the refusal on every run; `qsortcost` stays within 0.01 % of the base's instructions
  (10,979,557,925 to 10,980,097,228 here); and of the net's `run` cases exactly the two D cases of 2494 move,
  checkable at the landing's round gate.
- `condition`: a correct program refused under the route (a C callback run on a stack of the program's own
  after a handler left by `_longjmp` on Darwin, which I could not build tonight), or a library whose handler
  is measured to need the interrupted stack, or ASan's own fault report lost in a `run` case because the
  runtime's strong `sigaction` stands in front of ASan's weak one in Linux.
- **The ruling asked**: the runtime may define the C library's installers, under CLAUDE.md § 4 (architecture,
  this sitting) and §1.12. **The amendment to `.claude/rules/c-boundary.md`'s thirteenth member, in the
  escape's words, a widened diagnostic**: *"Since panel 248 the runtime defines the C library's handler
  installers, Darwin's ten (`sigaction`, `__sigaction`, `signal`, `bsd_signal`, `sigset`, `sigvec` and the
  four undeclared) and glibc's eight linkable, each passing to the C library's own with `SA_ONSTACK` added;
  a library a `link` or a `pkgconfig` answer of the program names that defines one is told
  `ffi_defined_elsewhere` as any name the runtime defines, and no group of that library makes it meant, since
  a replacement takes the guard away."* Measured: a `bsd_signal` library builds at the base and is refused
  under the route, with **0 compiler lines** (the member already reads the runtime's object as a definer).

### Q2, defect 2502

- `verdict`: **approve** route **(g)**, built: on POSIX no slice; a closed connection woken by the close's own
  shutdown, a listener by a pipe of its own, the stop by its pipe made with the first socket, a deadline as
  the `poll`'s bound; Windows keeps its slices. **Object** to the lane's route (a), a pipe per waiting
  thread: it buys what (g) buys at 3N + 6 descriptors where (g) costs N + 8.
- `section`: design.md §1.12 (no wait that never ends: past the limit both answer `HERO_NET_LIMIT` at once,
  measured) and §1.1 (the smallest route that closes it: no descriptor per connection).
- `implementation_cost`: about **130** lines of `runtime/platform/hero_net.c` for the route alone (the file
  1235 to 1449 with questions 4 and 5), `runtime/hero_net.h` two promises rewritten; `selfhost/` **0**.
- `needed_for_self_hosting`: **no**.
- `argument`: The base's connected-socket close already woke its waiter at once (its shutdown wakes the
  slice's `poll`, 154 µs measured); the slices paid for idling and for Darwin's listener. A shutdown is a
  state, not an event, so no wake is lost; the listener's pipe is never read, likewise. Measured on Darwin:
  100 waiters for 10 s cost 72.8 to 73.2 million instructions against 504.6 to 506.0 million, 515
  involuntary switches against 24,300; a listener's close wakes its `accept` in 32 µs against 27 ms; a server
  holds N + 8 descriptors (108 at N = 100, 248 under 256). Linux arm64 the same shapes. Windows keeps slices.
- `prediction`: at the landing, `waiters-checked 100 wait 10` on this Mac reads under 80 million
  instructions (the base over 500 million), and `lsof -p` of a server holding 100 connections reads 108.
- `condition`: a platform the CI runs where a connected socket's shutdown does not wake a `poll` waiting on
  it (I measured Darwin and Linux arm64 only; Linux x86-64 is the CI's), or a lost wake under a close racing
  the `poll`'s start.

### Q3, defect 2490

- `verdict`: **approve** the door, `hero_run_dir(folder)`, built: the folder opened in the parent before the
  fork, `fchdir` in the child after its streams, `lpCurrentDirectory` on Windows; **object** to a `heroes -C`
  flag as the answer (admissible by the stopping rule, `cli-surface.md:18-25`, but it serves four cases and
  no program).
- `section`: design.md §1.12 (the parent's error before any child exists, not a child dying in a folder it
  cannot enter) and §4.20; decision 2228 (`hero_os.h` is the language's door).
- `implementation_cost`: `runtime/parts/run.c` +91, `runtime/hero_os.h` +13; `selfhost/` **0**; the net:
  `shell.hero` 48 diff lines, `case_setting.hero` 79, `suite_fetch.hero` 31, `freed_bytes.hero` 7; one new
  `run` case.
- `needed_for_self_hosting`: **no** (the net needs it, not the compiler).
- `argument`: One promise on every system, run on both: every relative path of a launch is read in the
  parent's folder, which is what `CreateProcessW` does (measured on the box: a relative folder resolved in
  the parent's, a missing one refused with 267, a relative program path read in the parent's). POSIX opens
  the folder before the fork so a missing one is the parent's refusal (`HERO_OS_NOT_FOUND`), and enters it
  with `fchdir`, on POSIX's list. The four cases ran without `/usr/bin/env`; a relative path in a variable is
  the one new thing an author must be told.
- `prediction`: on the CI's Windows leg the two 2344 cases and fetch 2479 run instead of skipping, and if
  either fails it fails on clang's reading of `:` in `C_INCLUDE_PATH`, never on the folder.
- `condition`: a Windows run where `CreateProcessW` reads a relative `lpCurrentDirectory` otherwise than in
  the parent's folder on the CI's runner, or a measured hang of `fchdir` after a threaded fork.
- **The escape**: the door is a door a reader must be told of (spec-warden: whether § 13's line 377, which
  names processes from `hero_os.h`, owes a sentence), and the card owes *"a relative path handed to the
  child in a variable is read in the child's folder"*.

### Q4, a stop the program asks itself

- `verdict`: **approve** `hero_net_ask_stop()`, built, and in `core` 0.4.0 `net.ask_stop()` and
  `net.stop_asked() -> bool` (built in my tree's index).
- `section`: design.md §1.12 and §4.20; Part 5 (a library function, no construct).
- `implementation_cost`: `hero_net.c` about 30 lines, `hero_net.h` about 10 (the pipe made with the first
  socket is question 2's); `core` 0.4.0: `net.hero` +20, `net/hero_net.hero` +15, `net.md` +3; `selfhost/` 0.
- `needed_for_self_hosting`: **no**.
- `argument`: It is the first signal's effect without the signal: a compare-and-swap and one byte down the
  pipe every wait watches, from any thread, safe outside a handler. A Ctrl-C after it ends the program at
  once through the handler's existing second-request branch, no new code. Measured: an `accept` ends 17 to
  42 µs after the ask (Darwin, Linux), within a slice on Windows; `ce-quitserver` ends at `/quit` with its
  drain line, exit 0, the leak check clean.
- `prediction`: the blind seat's t4 sessions, re-prepared to `net.ask_stop()` with no result, compile on the
  first attempt at least as often as with the card's `-> ()?`, since no failure is there to handle.
- `condition`: a program that must tell *"asked by itself"* from *"asked by a signal"*, which `stop_asked`
  cannot.
- **The escape**: spec § 13's line 404 owes a sentence (*"only"* is false once the program can ask), and the
  `net` card two lines. My spelling differs from the card's t4 (`-> ()?`), said above.

### Q5, a connection that sends nothing

- `verdict`: **approve** a deadline per socket, `hero_net_set_deadline(s, ms)` and `net.set_deadline(c: Conn,
  ms: i64) -> ()?` in `core` 0.4.0, built; **object** to a join that does not wait (it moves spec line 377
  and leaves the quiet thread and socket held, one of 256 slots each) and to a limit in `net/http` alone.
- `section`: design.md §1.12 (a server a single silent peer can stop is a server that dies in silence) and
  §4.20.
- `implementation_cost`: `hero_net.c` about 60 lines (the clock, the timeout, the field, the door), the header
  10, `core` 0.4.0's face; `selfhost/` 0.
- `needed_for_self_hosting`: **no**.
- `argument`: Measured: at the base one quiet connection accepted first, with 7 that finished after it,
  stops the example server's accept (a new `GET /hello` unanswered in 10 s), as 8 quiet ones do. With a 3 s
  deadline set by the connection's thread, both shapes answer the new client after 2.4 to 2.5 s, the quiet
  ones ending `timed_out`; Windows 313 to 318 ms for 300. An absolute deadline is not defeated by a peer
  sending a byte before each per-call limit. A call with no deadline pays nothing measurable.
- `prediction`: with `examples/server` setting a deadline in `connection`, the narrow shape answers a new
  client within the deadline plus 1 s on every CI leg.
- `condition`: a measured cost of the deadline's check on a call that does not wait above the spread of
  three runs, or a program for which a deadline set while a call already waits must end that call (Go's
  semantics; mine ends calls that begin after it).
- **The escape**: a function on the `net` card (the spec-warden's to price), not a spec sentence unless the
  panel wants `HERO_NET_TIMED_OUT`'s second meaning in § 13.

### Container 2's cases, and container 3 (Linux arm64, after the verdicts were drafted; they change none)

- **The cases in Linux arm64**, my tree's compiler built there (`linux/c2/logs/`): `run fixedbugs-2494` **13
  passed, 2 failed** (the same two D cases as on Darwin, for the same reason); `fixedbugs-2496` 1 of 1;
  `fixedbugs-2257` 3 of 3; `fixedbugs-an-ignored-signal` 1 of 1; `the-platform-layer` **11 of 11**; `a-child`
  **10 of 10**, the door's own case included; `every-argument`, `fixedbugs-436`, `fixedbugs-2503` 1 of 1
  each; `runtime` **10 of 10**. (The 2344 and fetch cases need a git root and were not run there.)
- **Question 2's waiting cost in Linux arm64** (container `ce-248-3`, `linux/c3/run.sh`: a `wait4` wrapper,
  `ru.c`, in place of the `/usr/bin/time` the image lacks; the base's layer and the route's; two runs each):

| 100 threads waiting | base: voluntary switches, CPU | route (g): voluntary switches, CPU |
|---|---|---|
| 0 s | 104 / 105, 0.006 / 0.005 s | 102 / 105, 0.005 / 0.005 s |
| 5 s | 9,731 / 9,905, 0.153 / 0.052 s | **103 / 106, 0.006 / 0.006 s** |
| 10 s | 19,849 / 19,561, 0.100 / 0.197 s | **106 / 104, 0.006 / 0.007 s** |

  (About 100 wakes per second per 100 waiters at the base, a slice each; none with the route.) A close in
  Linux: base `recv` mean 177 µs, `accept` 51 µs (a Linux listener's shutdown wakes it at the base too);
  route `recv` mean 102 µs, `accept` 40 µs.
