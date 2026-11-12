# Panel 246, compiler-engineer's report

Seat: compiler-engineer (the ceiling: design.md §1.1, §1.7, Part 5). Written as I go, from 04:54 on
2026-11-12. My folder: `.claude/worktrees/p246-scratch/246-compiler-engineer/`, the base
`ef50a7478` extracted by `git -C .claude/worktrees/b33-round archive ef50a7478 | tar -x` into
`tree/`, `TMPDIR` set to `../tmp` for every build and tool. Every number below names the command
that produced it. `uptime` read load averages from 6.47 (04:54) to 69.30 (04:57) and 57.21 (05:06),
so **no duration here is a measurement** (CL-025); where one is written, `real` stands beside `user`
plus `sys`.

## Setup log

- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 21.28` (`user 13.25`).
  Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, `real 192.27` (`user 129.50`,
  `sys 26.24`), a waiting run. `./heroes-next --version`: `heroes 0.6.0`, **`sources
  796d1133542b1909`**, the base's. `heroes-next doctor` names this tree's own `runtime/`.
- The brief's fixture: the coordinator's index and catalogue (`p246-scratch/a-pid/repo/idx`,
  `app/heroes.toml`, `app/main.hero`, 3,007 files, `find fx-src -type f | wc -l`) **copied** into
  my folder as `fx-src/`; every case makes its own repository around itself (`git init -q`), so a
  case never runs inside the coordinator's folder.
- **A hook note, not a finding of the language**: writing a `.hero` file in my tree copy, the
  layer-0 hook judged it with the TRUNK's `./heroes` (built 2026-11-03) and said so, because my copy
  has no `.git` and `.claude/hooks/trees.py` finds a tree by `seed/heroes.c` beside a `.git` entry
  (`.claude/rules/verification.md` § A suite is the last judge). I formatted with my own stage 2
  (`heroes-next fmt --in-place`) and never rebuilt or ran the trunk's binary.

## Where the brief is wrong, said first

- **B3's numbered handle** (00-shared.md: *"a numbered handle (the layer gives a socket a number and
  back, which R3's never another socket that took its number exists to refuse)"*): the layer's
  handle is already a number with a generation (`hero_net_find`, `runtime/platform/hero_net.c:281-295`,
  reads `(index + 1) | life << 32` and refuses a stale life), so a number keeps R3's promise. Built
  and run below: four connections served one per thread under T1's `i64` rung, and a number held past
  its close told `not open`.
- **Route A as the issue's comments wrote it** (recovery at the start of `add` and `fetch`) and as a
  first build would place it (inside `staging.begun`) are both wrong in a way the brief's A2 only
  asked about: the first leaves every killed `check`'s folder; the second makes the build tell a
  false `package_unread` (measured, finding A-3). Recovery belongs once before discovery asks a
  placed tree.
- **The stop's sentence** (S1 to S3: *"a loop that asks between its steps finishes what it holds
  and returns from `main`"*) is false of the runtime in four measured situations (B2), not a
  question of spelling.
- Minor: the `fetch` form counts 48 checks over the folder's 47 `.case` files.
- **Not the brief's, but read here first**: the compiler's own tests at the base very likely hold one
  red test (`defect 2355`'s, `-dumpmachine` in `cli/doctor.hero`), an inference from route A's run
  over byte-identical inputs, unrun on the base.

**How to read this report**: sections were appended as runs ended. Part A: its build, its phases
and A1 to A4, the three-compiler table, the platforms, what moved, and its verdict at the very end.
Part B: B1, B2 and B3 first; then, after Part A's A4, B2's route (a) and (b), the stop candidates'
cost table, S3 and the grading compilers, and B3's numbered handle; then what I would adopt and
Part B's verdict, before Part A's verdict.

## Part A: defect 2453, the soundness lane

### What I built (every route below is built and run, none adopted unbuilt)

| tree | what | built | `--version` second line |
|---|---|---|---|
| `tree/` | the base, control | seed then stage 2 | `sources 796d1133542b1909` |
| `tree-a/` | route A with D, door in `runtime/hero_compiler.h` | stage 2 by `tree/heroes-next`, `heroes-next` | `sources 12428e4c41d3bf00` |
| `tree-a/` | the same plus recovery before discovery (below, finding A-3) | `heroes-a2`, stage 2 by `tree/heroes-next` | `sources 7d9014d844216c9c` |
| `tree-d/` | route D alone, `staging.hero` +12 -2 | stage 2 by `tree/heroes-next`, `heroes-next` | `sources 201bf3d413214e1f` |

**Route A as built**, compiler-private (`.claude/skills/panel/SKILL.md`'s soundness lane: no surface,
no diagnostic, no spec token):

- `runtime/parts/lock.c`, new, 101 lines (`wc -l`), 52 of them neither blank nor comment: one door
  `int64_t hero_lock_take(const char *path, int64_t make)` and `void hero_lock_release(int64_t)`.
  POSIX: `open(O_RDWR|O_CLOEXEC[|O_CREAT|O_EXCL])`, `flock(LOCK_EX|LOCK_NB)`, then the name asked
  again (`fstat` of the descriptor against `stat` of the path, `st_dev`/`st_ino`), since a run that
  opened the file and waited while its holder removed it would otherwise hold a lock no name reaches.
  `flock`, never `fcntl` record locks: those are the process's and drop when ANY descriptor of the
  file closes. Windows: `CreateFileW(..., FILE_SHARE_READ|WRITE|DELETE, CREATE_NEW|OPEN_EXISTING)`,
  not inheritable, `LockFileEx(LOCKFILE_EXCLUSIVE_LOCK|LOCKFILE_FAIL_IMMEDIATELY)`.
- `runtime/hero_compiler.h` +10 (80 to 90), `runtime/runtime.c` +4 (297 to 301, one `#include` and
  its comment).
- **The descriptor is handed back to the caller, kept in `Staging` and `Ledger` as an `i64`, and
  nothing is kept in the runtime between calls**, so `tests/harness/suite_runtime.hero`'s Rule 3
  has no object to judge: no `_Thread_local`, no `_Atomic`, no allow-list line. (Its run on
  `tree-a` is below.) `hero_compiler.h` is outside the guard's 241 names, so no guard header and
  no `guarded_names.hero` line moves.
- `selfhost/fetch/leftover.hero`, new, 175 lines, 96 neither blank nor comment: `held`, `let_go`,
  the ledger's two line shapes, `recovered(program)` (every `.fetching-*` folder under
  `heroes.packages/` whose lock it can take, undone from its ledger and removed; a folder whose
  lock is held, **or which has no lock file**, left as it stands), the replay `put_back`, and a
  directory listing over `hero_dir_scan`.
- `selfhost/fetch/staging.hero` +62 -7 (286 to 341 by `wc -l`): `opened` is **route D** (a
  standing name is never removed, the next serial taken, `heroes new`'s rule) and takes the lock;
  `dropped` lets the lock go before removing (Windows removes no open file); `begun` runs
  `leftover.recovered` first, so `found` reads the folder as the author left it, then writes the
  ledger's first line `found 0|1`; `placed` writes the tree's row (`<key>\t<aside>`, both relative,
  since the next run may open the program by another path) **before the first rename**, through
  `write_file`, which is whole or not at all (defect 438); `ended` removes the ledger first, then
  the lock, then the folder, so a run killed while it removes its folder leaves nothing a later run
  would undo.
- **Recovery is `undone` replayed from disk, never a reading of the catalogue.** Every row, last
  first: where its aside still waits, the tree at the key (if any) is moved into the dead folder and
  the aside put back; where it has no aside, the tree at the key is taken away. Each of the six
  states a kill can leave a row in (killed before, between and after either rename, with and without
  an aside, or inside `undone` itself) replays to the tree the run found, and a second replay changes
  nothing. **That sentence is a reading of the replay**; the phases below run (a), (b), (c), (d) and
  two replays at once (g), not every one of the six.

**Where it costs, in the brief's own unit (`wc -l` at the base), the route A tree against the base**:
`selfhost/` +242 lines (`staging.hero` +62 -7, `leftover.hero` +175, `packages.hero` +5 for finding
A-3), `runtime/` +115 (`lock.c` +101, `hero_compiler.h` +10, `runtime.c` +4). No lexer, parser,
checker, descriptor, ownership or emitter line. Owed and not written: a compiler test for
`leftover.recovered` (a planted dead folder, unlocked), and the `fetch` form's cases (below, A4).

## Part B: the server step (measured so far; the verdict block closes the part)

### B1, what crosses into a thread

**The rule as the build holds it, run** (`b1/`, my base `heroes-next`, one program per shape binding
`hero_thread_spawn` with that body; `check` then `build`, each exit and first code read):

| body and `arg` | `check` | `build` | answer |
|---|---|---|---|
| `i64` | 0 | 0 | prints `42` |
| `str`, `bool`, `f64`, `i32`, `ptr`, `-> ()` result, `-> i32` result, a group record (`record Pt tag pt`), a handle (`record Socket tag hero_net_socket`) | 0 each | **1 each**, `error[ffi_callback_type]` | *"clang read the header, and it says `long long (*)(long long)`"*, at the binding's own line, spec § 13's note under it |
| `[i64]`, `{str: i64}`, a plain record `Pair` | 1, `error[ffi_type]` | not reached | |
| `cstr` (`s.cstr()` as `arg`) | 1, `error[lend_kept]` | not reached | |

So: **`hero_os.h:430-434` is false today**, as the brief says, and more widely than it says: the
checker's `crosses_the_boundary` (`selfhost/check/ffi.hero:79-94`) is the FFI's general rule (what
may reach C at all), never the thread's; what refuses a `str`, a scalar other than `i64`, a group
record or a handle is the header's C prototype, read by clang at `build`. **Every such refusal is
already on the author's line with a code, exit 1** (`ffi_callback_type`, panel 111 R4's class), so
design.md §4.17's promise holds at `build`; what `check` misses is the timing, not the line. A
checker rule that knew `hero_thread_spawn`'s prototype would duplicate clang's verdict by naming one
header's function in `selfhost/check/` (my estimate, unbuilt: 15 to 25 lines and a case; **no new
class**, `ffi_callback_type` reused). I would not build it: the repair owed is the comment's four
lines in `hero_os.h`, saying the header's prototype decides and `build` tells it.

**Where `i64`-only was decided**, from history read through git only: `git -C
.claude/worktrees/b33-round log -S hero_thread_spawn -- runtime/hero_os.h runtime/runtime.c
runtime/parts/spawn.c` names two commits; the first, `ea4a6fda7` (2026-09-06 12:22, *M-isolated-threads
step 6*), wrote `int64_t (*)(int64_t)`. Its body argues the HANDLE's type (*"WHY THE HANDLE IS AN
`int64_t`"*) and says *"AND THE ISOLATION IS THE TYPE RULE"*, and has no sentence about the BODY's
type or about narrowing panel 113's R5 (*"the scalars and `str`"*, ratified 2026-09-06, `grep -n 'the
scalars and' docs/panel/113-*.md`: 276). Searched for a ruling and found none: that commit's body,
journal 034 (`:79` states the outcome, gives no reason), panel 113, panel 114 (`grep -n -i '\bstr\b'`
hits only line 178, about concatenation), `docs/roadmap/milestones/M-isolated-threads.md` (`grep -n
-i 'i64\|\bstr\b\|cross'`: line 19, panel 111's shared-`str` veto, measured **before** the atomic
count). **So it reads as a gap between a ratified ruling and the step that implemented it, not a
ruling**; that is a reading of four records, said as such.

**R5's rung, built and measured again** (`sstr/`: a runtime copy with one more door,
`hero_thread_spawn_str(int64_t (*)(HeroStr), HeroStr)`, 23 lines in `parts/spawn_str.c` over the
`i64` door: the `str` counted once more for the thread, handed to the body borrowed as every
callback parameter is, let go on the worker; `b1s/share.hero`): one heap `str` (built at run time,
40 bytes) shared by 32 threads, each pushing it into an array 8,000 times (256,000 count operations
on one count), concatenating, slicing and comparing it.
- plain: `512000 40`, exit 0, three runs of three (the right sum; exit 0 is the leak gate passing);
- `--sanitize` (ASan and UBSan): the same, exit 0, five of five, no report;
- ThreadSanitizer, built by hand from `--emit-c` (`clang -std=gnu11 -fno-strict-aliasing
  -fsigned-char -fno-optimize-sibling-calls -g -O1 -fsanitize=thread -I ../sstr/runtime share.c
  ../sstr/runtime/runtime.c -lm`): 0 warnings, exit 0, three of three;
- **the instrument proved live the same way**: `control.hero`, a C counter (`racy.h`) bumped
  unlocked by two threads through the same door, built the same way: `WARNING: ThreadSanitizer:
  data race`, `racy_bump racy.h:2`, one warning.

So panel 111's *"9 reported `heap-use-after-free`"* does not reproduce on the atomic count for this
shape (a `str` is never mutated in place, so no copy-on-write site is reached). Not run: `f64` and
`bool` bodies (they hold no count; a door per type is the cost, below).

### B2, the stop

**Built**: `s1/` and `s2/`, each a copy of my base `heroes-next` beside a copy of the base's
`runtime/` with one part, `parts/stop.c` (S1: 46 lines; S2: 48; `wc -l`), and the declarations in
`hero_os.h` (S1 two functions, S2 one), the flag and the armed bit `static _Atomic int`, lock-free on
every target, so a handler may write it (C11 7.14.1.1p5), any thread may read it, and Rule 3 admits
it with no allow-list line (the `volatile sig_atomic_t` of `hero_runtime_spoke` is not needed and is
not a thread-visibility guarantee). POSIX arm `sigaction` with `sa_flags` 0 for SIGINT and SIGTERM;
Windows arm `SetConsoleCtrlHandler` for CTRL_C, CTRL_BREAK and CTRL_CLOSE. Both runtimes compile on
this Mac and with `zig cc -target x86_64-windows-gnu`, no diagnostic (the Windows arm compiled, not
run). The copied binary's `--version` second line is the base's, `sources 796d1133542b1909`: the
compiler is the base's and the runtime beside it is the arm's (`heroes doctor` from `b2/` names
`s1/runtime`), which is what the blind seat's grading needs for S1 and S2; `tree-s3/` for S3 is in
its own row below.

Each probe a binary under `timeout`, its output into a file, sent the signal by `b2/sendrun.sh`
(`pgrep -P` of the `timeout`, so the signal goes to the program itself), `pgrep` read empty after:

| probe | signal | output | exit |
|---|---|---|---|
| `loop1` S1: a step that allocates, `usleep(10000)`, the stop asked between steps; **one block leaked first through `leak.h`** | INT, TERM | `drained after 60 steps` / `79` | **134**, `panic: 1 heap blocks still live at exit` |
| `loop2` S2, the same | INT, TERM | `drained after 66` / `78` | **134**, the same panic |
| `exit1`, base: a Heroes handler through `signal` that calls `exit(code: 0)`, the same leak | INT, TERM | `caught 2` / `caught 15` | **0** |
| `clean1` S1 with no leak | INT, TERM | `drained after 64` / `74` | 0 |
| `slowdrain` S1: a drain of 300 steps after the stop | INT | `stop seen after 64 steps; draining`, `drained` | 0 |
| `slowdrain` S1, **a second INT 0.5 s after the first** | INT, INT | `stop seen ...; draining`, `drained` | 0 |

So **task 1072's leak gate runs for a stop that returns from `main`, and does not for one that ends
through `exit`** (`runtime/parts/os.c:1005-1006`; `selfhost/emit/c_main.hero:83`, `:102`): the same
leaked block is a panic in S1 and S2 and silence in the coordinator's `h.hero` shape. And a **second
Ctrl-C does nothing** under S1 as drafted: a drain that hangs can then be ended only by SIGKILL.

**Under `heroes run`** (S1's `slowdrain.hero`, `timeout 60 ../s1/heroes run`, the signal sent to the
`heroes` process once its program was running, the Darwin sentinel told apart by its `sh -c` line):
SIGTERM reached the program (`stop seen after 18 steps; draining`), and `heroes` then ended it at its
grace, `HERO_RUN_GRACE_MS` 2000 (`runtime/parts/run.c:561`): `drained` never printed, `heroes` exit
**143** after 3 s. **So S1 to S3's promise, "finishes what it holds and returns from `main`", is
false under `heroes run` for any drain past two seconds, and the leak gate does not run there
either.** The SIGINT reading of the same run is void: a background job's shell starts its children
with SIGINT ignored, and `run.c` leaves an ignored signal ignored (its own comment, `:519-525`).

**A server blocked in `accept`** (`accept1.hero`: S1, a loopback listener on port 0, no client, the
stop asked between connections), `timeout 30` then my SIGKILL:
- S1, INT: no output, still alive at `ELAPSED 04:25` by `ps`; S1, TERM: still alive at `00:34`,
  **past `timeout 30`'s own SIGTERM, which the stop turned into a flag**; both ended only by `kill -9`,
  exit 137. The layer asks `accept` again after `EINTR` (`runtime/platform/hero_net.c:509-512`), so
  a loop that asks between connections is never reached while it waits, and **a stop armed makes the
  program deaf to the polite end a supervisor sends** (measured with `timeout`; that `docker stop` and
  systemd send the same SIGTERM first is their documentation's, unrun here).
- **Route (c), built** (`s1c/`: the same runtime, `hero_net.c`'s loop `&& hero_stop_asked() == 0`,
  2 lines and a declaration): INT and TERM each print `accept returned nothing`, `stopped after 1
  accepts`, exit 0; with a `hero_thread_spawn` worker alive beside the blocked `accept`
  (`accept2.hero`), 3 of 3 runs `accept returned nothing`, `worker saw the stop after 67/74/79
  steps`, exit 0; the same program on S1 without (c): no output, exit 137. Darwin only: whether
  Linux delivers the signal to the thread blocked in `accept` is unrun; and whether Windows' `accept`
  returns at all when a console control event runs the handler on a thread of its own is a question
  (unrun), which, if it does not, (c) on Windows answers only by the handler closing or waking the
  listening socket.

**Which thread runs a handler** (`cthread.h`: a thread a C library makes for itself, `pthread_create`
of a `pause()` loop, and optionally the main thread blocking SIGINT and SIGTERM as a library may ask):
- the program's own header (`stopflag.h`) with a **Heroes** handler, C's thread alive, main
  blocking: INT and TERM each `panic: owncthread.on_end ran on a thread this program did not start`,
  exit **134** (`runtime/parts/thread.c`'s guard, panel 111 R9);
- the same with main not blocking: drained, exit 0, 5 of 5 on this Mac (Linux unrun);
- **S1 under the same conditions as the first**: `drained after 61` / `80`, exit 0. A handler the
  runtime writes in C asks no thread who it is, so S1 to S3 do not meet the guard at all.

**Four signals or two** (`child1.hero`: S1 armed, then `hero_run_go` of `/bin/sleep 5`, the signal
sent one second in):
- TERM and INT: the child was **not** ended: it ran its five seconds (`child answered 0 status 0`),
  wall 5 s, and the program read `stop asked 1` after. `run.c` catches the four only where the
  process left them at their default (`:518-525`, `hero_run_catch_endings` at `:667-690`), and an
  armed stop has taken INT and TERM, so **arming the stop removes those two from defect 425's
  forwarding**: a supervisor's SIGTERM no longer reaches a `git` or a `clang` the program waits on.
- HUP, still at its default: forwarded, the program died of it, exit 129 in 1 s, as before.
- (Both INT and TERM runs then printed `panic: a launch's argument list still holds 2 word(s) at
  exit`: my probe's missing `hero_run_reset()`, the leak gate doing its job, not the route.)

**The program's own header, feature 1069's route** (rebuilt: `stopflag.h` is the coordinator's
`a-probes/stop1/stopflag.h` copied): its two-module trap stands as the critic and the coordinator
measured it (I did not re-run stop2; one unit drained in my `own_cthread_free` runs). A rule the
compiler could hold: refuse a group header that defines a file-scope `static` object which is not
`const` when two modules bind it, read from clang's `-ast-dump=json` the build already asks for
layouts (`VarDecl`, `storageClass: static`, not const-qualified). Unbuilt; my estimate 40 to 70
lines in `selfhost/cli/` and **a new diagnostic class**, so outside any soundness lane, and it
would refuse every header-only C library that keeps a `static` cache, which is the common shape. The
cheaper honest repair is the opposite: the stop in the runtime, where one object exists for the
program whatever its module count.

### B3, one connection per thread, and copy-on-write

**A connection cannot reach a worker under T1's rung**: the handle body is refused at `build` (B1's
table, `p_handle`, `ffi_callback_type`), as the coordinator measured. The three ways out, priced:

- **One thread** (`docs/roadmap/milestones/M-core-packages.md:35`, *"writable, one thread"*): 0
  lines; the platform layer's own case `the-platform-layer-echoes-over-loopback-in-one-thread` already
  runs it. Costs a server its concurrency, not its correctness.
- **A numbered handle** (the layer hands a socket out as a number and takes it back; **this bullet is
  my first estimate, superseded by the build further below, which found the layer's handle already
  carries a generation and keeps R3's promise**): the layer's
  table already exists (`hero_net_take` and `hero_net_slot` in `runtime/platform/hero_net.c`); a
  `hero_net_number(s) -> i64` and `hero_net_of(n) -> Socket` pair is my estimate of 20 to 30 lines
  there, unbuilt. **What it costs is R3's own promise**, `hero_net.h:33-36`, *"never another socket
  that took its number"*: a number held across a close names whatever the slot holds next, which is
  the stale-descriptor class R3 wraps descriptors to refuse. A generation counter packed into the
  number (slot and generation) would keep the promise for a number, at the price of a second
  checked form beside the handle. Unbuilt.
- **A rung that moves a handle with its marks**: `hero_thread_spawn` would have to take a group
  record behind a pointer, so the checker's `acquires`/`consumes` marks (`selfhost/check/ffi.hero`,
  `crosses_the_boundary`'s `.named` arm says yes to a group record already) must learn that the
  spawning thread gives the handle up at the call (a `consumes` on `arg`), and `ffi_narrowed.hero`'s
  `ffi_callback_type` path must accept a body whose parameter is that record. **That is a change to
  what the checker and the ownership pass do**, a typed spawn door per handle type or a generic one;
  my estimate 80 to 150 lines across `check/` and `emit/`, unbuilt and not priced by a build.

**Where a counted value reaches two threads under the rung, enumerated from the tree** (`grep` of
`runtime/parts/*.c` for a static `HeroStr`/`HeroArray`/`HeroMap` object: none; `hero_args_at`
builds a fresh `str` per call, `runtime/parts/os.c:533-538`; the drop lists are `_Thread_local`,
`runtime/parts/drop.c:59-61`; a Heroes callback C calls from its own thread panics, measured in B2):
a `constant` and a string literal, both static (count below zero, never mutated in place, copied
before any write); a value C hands back, fresh per call; a `ptr` a program's own C turns into an
`i64`, which is the program's C and outside every guarantee of the language; and, under R5's rung,
the `str` itself, which has **no in-place mutation site** (the four `refcount == 1` sites are
`cow.c:44`, `:88`, `:93`, arrays, and `map-write.c:128`, maps). So no shape I can write in Heroes
reaches a copy-on-write test-then-mutate from two threads under either rung; under a handle or an
array rung it would.

**Run** (`b3/constshare.hero`, my base `heroes-next`, the `i64` door, 8 workers, 3,000 steps each):
each worker compares a nested `constant GRID: [[i64]]` with `==` (the `eq` walk), copies a row of it
and writes the copy (copy-on-write on a static), `sort`s a constant `[str]` and a constant `[i64]`
(the runtime's comparator walk), and hashes the words into a map of its own (the `hash` walk); the
drop lists drain under contention as each step's copies die. Plain `336000`, exit 0 (the right sum);
`--sanitize` 3 of 3 the same, no report; ThreadSanitizer by hand from `--emit-c`, as in B1, **0
warnings, 3 of 3**, the instrument proved live by B1's control. Panel 111's four classes: the shared
`str`'s count (B1's run, clean), `==` on nested arrays (clean, above), copy-on-write's test-then-mutate
(unreachable by any shape I could write, above), the stack guard on a worker (claimed in
`hero_spawn_enter`, `runtime/parts/spawn.c:225-234`, the claim at `:226`; not re-run here).

### Part A, the phases (`parta.sh` in my folder; every case a fresh copy of `fx-src/` in its own `git init -q` repository; kills are `kill -9` of the compiler's own process)

**Control at the base, reproduced in my folder with my compiler** (`runs-base.partial.log`; that run
was stopped by me mid-way because I edited the script under it, and is re-run whole below):

- (d) a `fetch` killed once `replaced-0` stood: `wait` 137, left `.fetching-59925-1/` whose
  `replaced-0/a.hero` is the only file holding `small`'s edit.
- 1a, a later `fetch` under another number: `kept small`, `placed large`; the dead folder and the
  edit stand.
- **1b, a later `fetch` handed the dead run's number** (the folders renamed to the shell's `$$`,
  then `exec`): `kept small`, `placed large`, and afterwards **0 `.fetching-*` folders and 0 files
  holding `small`'s edit**.
- **1c, a later `check` handed the dead number**, `small`'s placed tree moved away:
  `error[package_unread]`, the dead folder gone, `small`'s edit gone (only `large`'s stands).
- 1d, the same `check` under another number: the dead folder and the edit stand.

So the coordinator's and the critic's measurement holds on my compiler: a later run of any verb that
reaches `staging.opened` with the dead number removes the only copy of the author's edit, silently.

**Route A (`tree-a/heroes-next`, recovery inside `staging.begun`), `runs-a.log`:**

| phase | what the kill left | the later run | afterwards |
|---|---|---|---|
| (d) `fetch` killed, `replaced-0` standing | `.fetching-85315-1/`, `replaced-0/a.hero` the only copy of the edit | 1a `fetch`, other number: `placed small`, `placed large`, exit 0 | 0 folders; the edit put back, then replaced by `fetch` and dropped at its end, as `fetch` does to any edited tree: 0 files hold it |
| (d) | the same | 1b `fetch`, **the dead number**: the same lines, exit 0 | the same as 1a: the dead number changes nothing |
| (d) | the same, `small`'s tree moved away | 1c `check`, the dead number: `error[package_unread]`, exit 1 | 0 folders; **`small@1.0.0-.../a.hero` holds the edit again**, in place |
| (d) | the same | 1d `check`, other number: the same message, exit 1 | the same as 1c |
| (a) `fetch` killed while git slept | `.fetching-5316-1/` | `fetch`: `placed small`, `placed large`, exit 0 | 0 folders |
| (a) `add small` killed while git slept | `.fetching-15480-1/` | `add small`: `added small 1.0.0`, `wrote heroes.toml`, exit 0 | 0 folders |
| (e) `check` killed while git slept, no `heroes.packages/` before | `.fetching-15772-1/` and a `heroes.packages/` the author never made | `check`: `package_unread`, exit 1 | **0 folders, and `heroes.packages/` gone** (the ledger's `found 0`), as the program was |
| (b) planted: `small`'s edited tree in the dead folder's `replaced-0`, its key empty, a staging folder holding the new tree, each with an unlocked `lock` | | `check`: `package_unread`, exit 1 | 0 folders; the edit back at `small`'s key |
| (b) | the same | `fetch`: `placed small`, `placed large`, exit 0 | 0 folders; the edit put back then replaced, as `fetch` does |
| (c) `add large` killed with `large` placed, the catalogue not yet written (`catalogue moved: no`) | `.fetching-37416-1/`, `large@...` placed and named by no catalogue | `check` (`use <small>`, no entry): `unknown_entry`, exit 1, reaches no package | the folder and `large` stand (nothing asked a package) |
| (c) | the same | `fetch`: exit 0 | 0 folders, `large` taken away, **`heroes.packages/` gone**: the program as the author left it |

The base keeps every one of those dead folders for ever under another number, and under the dead
number removes them with whatever they hold.

**Finding A-3, measured above in 1c, 1d and (b): recovery inside `staging.begun` makes the build's
first message false.** `module/packages.hero` decides a tree is not placed (`problem_of`), then
`unplaced.refusal` calls `staging.begun`, which recovers the tree; the build then prints
`package_unread ... not placed` about a tree that stands by the time the author reads it. A false
message is `blocking` by `.claude/rules/verification.md` § Bounded discovery. **Repaired in
`heroes-a2`**: `leftover.recovered(b.opened)` runs once in `packages.reach`'s `if !b.asked`, before
any placed tree is asked (+5 lines, `packages.hero` 301 to 306); its phases are below.

(f) under route A: a `fetch` held alive in a slowed `git` while a second `fetch` ran to its end
(`placed small`, `placed large`, exit 0): **the live run's `.fetching-74477-1` stood untouched**
after the second (its lock held), and the live run went on to open `.fetching-74477-3`.

**The lock and the child, measured** (`runs/lsof/`, `heroes-a2 check` held in a slowed `git`,
`lsof -p` of each process): `heroes` holds `.fetching-80988-1/lock` on descriptor 3; its `git`
child and that child's own child hold only `git-root.txt` and `git-said.txt` there, **not the
lock**. So with `O_CLOEXEC` a child holds the lock only between its `fork` and its `execvp`
(`runtime/parts/run.c:1340`), and Darwin's sentinel, started by `posix_spawn` with
`POSIX_SPAWN_CLOEXEC_DEFAULT` (`:885`, `:900`), never does. That run's end: `package_unread`
(true: `small` is not placed), and `heroes.packages/` removed again.

### A1, a lock private to one run, against panel 235's R7, R12 and R19

- **R12's "a lock" is a lock FILE in Cargo's sense**: the sentence lists it beside *"a workspace or
  a configuration beside the catalogue"*, and the compiler-engineer report R12 rests on asks
  *"Whether a lock file is needed. Under MVS the selected versions are a function of the catalogues
  alone"* (`docs/panel/235-reports/compiler-engineer.md:107-116`). Route A's `lock` is no per-project
  file and holds nothing: it is made by one run inside that run's own folder, under the placement
  folder no `use` can spell, read by no build, removed by the run that made it or by the next. Its
  content is empty and its only question is the operating system's *"is your holder alive"*. By
  `.claude/rules/cli-surface.md:54-57`'s own test, *what the tool reads*, the tool reads nothing
  from it.
- **The ledger is read by a later run**, which is the nearest thing to state carried between runs.
  It is the tool's own scratch, as `build/`'s private files are (`cli/publish.hero`), never written
  by a person, and R12's falsifier holds of it: deleting `ledger` or `lock` never changes which
  module compiles, only whether a dead run's leftovers are undone (a folder with a lock and no
  ledger is removed without a replay, which is what `ended` intends; a person deleting a ledger
  under a dead run's `replaced-N` would lose that tree, the one residue I found by reading).
- **R7's "no lock between two builds is needed beyond `build/`'s own"** (`:381` of the same report)
  rested on process-number names being enough; defect 2453's measurement is that they are not,
  after a kill. The route keeps R7's letter (two programs never share a tree, no user folder is
  read) and corrects its premise for two runs over one program. **That correction is the
  sitting's to state, and I would state it**; I do not read it as a fifth input class.
- **R19**: route A prints nothing new. No message, code or exit moved in any phase above, and the
  placement folder's name is unchanged. Whether the 47 `fetch` goldens agree is run below.

### A2, every writer of `.fetching-*`

Enumerated from the tree (`grep -rn 'staging\.\(opened\|begun\|placed\|ended\|undone\|dropped\)'`):
`add.hero:90`, `fetch_all.hero:53`, `unplaced.hero:46` (every verb resolving a `use <name>/...`:
`check`, `build`, `run`, `test`, `mutate`), and three openers of a package's own staging folder,
`git_source.hero:48`, `index_source.hero:126`, `add_wanted.hero:87`. Route A puts the lock in
`opened`, which all six reach, so **no folder of a route-A compiler is ever lockless**; the D rule
is in `opened` too, so no writer removes a standing name. The `check` path owes recovery, and
**owes it before discovery asks a placed tree** (finding A-3, measured); `found` is on disk, so the
killed `check`'s `heroes.packages/` was removed and the author's own empty one would be kept
(`found 1`, the base's own test *"A `heroes.packages/` the run found, empty, is the author's:
kept"*, passing in `tree-a`'s tests). A folder with no lock file (a compiler older than the route,
or a run killed between `mkdir` and the lock's `open`, microseconds apart) is left standing: a run
that cannot be proved dead is never undone.

### A3, the shapes beside it

- **A file system**: `flock` on a FAT volume made and mounted inside my folder (`hdiutil create -fs
  MS-DOS`, `-mountpoint fat/mnt`): taken, and a second open's `flock` refused (`Resource
  temporarily unavailable`), as on APFS. A network file system: unrun, no share here. Where `flock`
  is refused outright (`ENOTSUP`, `ENOLCK`), the route as built refuses the verb (`cannot_place`,
  *"the lock ... cannot be made"*); the robust fallback is to unlink the file and run lockless, so
  the folder falls back to route D's rule. Owed, unbuilt.
- **An owner alive in another session**: (f) above. **PID reuse**: 1b and 1c above, the dead number
  changes nothing under route A. **A kill between the two renames**: (b), planted, undone.
  **A killed `fetch` over an edited tree**: (d), the edit put back.
- **Two recovering runs at once**: (g) below. Both take the lock before reading; the loser of the
  race sees the lock held (or, after the winner removed the folder, no lock file) and leaves; a
  run holding a lock on a file the name no longer reaches is answered *held* by the `st_ino` check.
- **A child**: measured above. **A program's C that forks without exec**: the door is the
  compiler's own and no program binds it, so no program's C ever holds this lock.

### A4, the cases

The `fetch` form has no kill step (`grep -n -i kill tests/harness/suite_fetch.hero`: nothing, the
brief). **A golden can hold every phase above as a planted folder**, as (b) is planted here: a
`.fetching-<n>-1/` holding an empty, unlocked `lock`, a `ledger` (`found 1` and one row) and a
`replaced-0/` tree, then the verb, then the tree compared. That needs the form to plant files
before the run, which is the harness's to grow (unbuilt, a question for `suite_fetch.hero`'s owner,
not for this lane). And recovery prints nothing, so no golden's text moves (A1).

**Route (a), built** (`s1a/`: S1's two names, `hero_stop_catch` blocking SIGINT and SIGTERM with
`pthread_sigmask`, `hero_stop_asked` reading `sigpending`, 29 lines, POSIX only):
- `clean1`, TERM: `drained after 77 steps`, exit 0. INT: never seen, `timeout`'s own SIGTERM at 15 s
  drained it (`drained after 1094 steps`, exit 124): the probe runs as a background job, which a
  shell starts with SIGINT ignored, and an ignored signal is discarded rather than held. S1's
  `sigaction` overrides an inherited ignore; (a) keeps it; `run.c` keeps it too (`:519-525`).
- **`child2`** (the stop armed, then a child `/bin/sh -c 'kill -TERM $$; sleep 1; echo survived'`):
  under (a) the child printed **`survived`**, exit 0: it started with TERM held, because `run.c`
  gives a child back *"the mask as it was before the launch blocked them"*
  (`hero_run_child_signals`, `:694-705`), and that mask now holds the stop's two. Under S1 the same
  child died of its own TERM (`child answered 143`). So route (a) starts every `git`, `clang` and
  program a server runs deaf to Ctrl-C and to TERM, unless `run.c` learns to clear the stop's two.
- `accept1` under (a), TERM: no output, killed, exit 137: a blocked signal interrupts nothing.

**Route (b)**, the stop as a descriptor polled beside the listener: **not built**. Its premise is a
`poll` over the listener's descriptor, and R3 hides the descriptor inside `hero_net_socket`
(`runtime/hero_net.h:45-46`), so a program cannot poll it; (b) is a layer door
(`hero_net_accept_or_stop`, a self-pipe or `EVFILT_SIGNAL`/`signalfd` beside the socket) or it is
(c). I would build it only if (c) fails on Linux or Windows.

**What each stop costs, and where it lands** (`diff` against the base, `wc -l`):

| candidate | `selfhost/` | `runtime/` | checker, emitter, ownership | a new reserved name | moves |
|---|---|---|---|---|---|
| S1 (`hero_stop_catch`, `hero_stop_asked`) | `emit/guarded_names.hero` 1 line rewritten | `parts/stop.c` 46, `hero_os.h` +7, `runtime.c` +1, each guard header +2 | 0 | no | every emission's `hero_program_sources` line (B2 above: 2 `diff` lines per program) |
| S2 (`hero_stop_asked` alone, armed by its first call) | the same, 1 name | `parts/stop.c` 48, `hero_os.h` +5, `runtime.c` +1, each guard header +1 | 0 | no | the same |
| S3 (`stop_asked() -> bool`, a built-in) | `inventory.hero` +6 -2 (appended at the end, the count 41 to 42: the order is identity, `inventory.hero:20-28`), `library_source.hero` +6 (the binding and a two-line body, `exit`'s precedent at `:270-276`), `guarded_names.hero` 1 line | S2's runtime | **0**: a `.heroes`-tier built-in is library source compiled as Heroes | **yes**: every built-in is reserved everywhere (`inventory.hero:6-9`) | the same, and the spec's built-ins table, which the `spec` suite holds to `inventory.hero` |
| route (c), the stop inside the layer's `accept` | 0 | `platform/hero_net.c` +2 (Darwin, measured); Windows unbuilt | 0 | no | the layer's key |

**None of them is a core construct** (design.md §1.7, Part 5's seven): S1 and S2 are two doors of
`hero_os.h`, S3 a library function over the same door. My veto has no ground here. S3 is the one that
reserves a word, so a program of today that names a function `stop_asked` is refused tomorrow
(`b2/s3clash.hero`, run below if the S3 compiler builds in time).

**S3, built** (`tree-s3/`, stage 2 by my base compiler, `heroes-next --version`: `sources
06b60f830cca15c0`; `check selfhost/main.hero` exit 0 first): `b2/s3loop.hero`, the stop asked as
`!stop_asked()` with no binding written: `drained after 69` (INT) and `87` (TERM), exit 0.
`b2/s3clash.hero`, a program of today that declares its own `function stop_asked() -> bool`: the S3
compiler refuses it, `error[builtin_name_taken]`, exit 1; the base accepts it, exit 0. And **S3 moves
every program's C, not one line of it**: `p_i64.hero` emitted by the S3 compiler against the base,
`diff` 13 lines (a `_Static_assert(HERO_RET_INT(hero_stop_asked()), ...)` for the library's new
binding, and every `#line` after it shifted), where S1's runtime moved 2. So S3's re-blessing is
every line after the library's bindings in up to 861 emitted files, and the `lldb` sessions that pin
`#line`s are a question (unrun). Not run on the S3 tree: the compiler's own tests and the `spec`
suite (the built-ins table needs S3's spec row, which I did not write).

**The blind seat's grading compilers** (its task t3, one per arm): S1
`.claude/worktrees/p246-scratch/246-compiler-engineer/s1/heroes` and S2 `.../s2/heroes`, each my base
binary (`sources 796d1133542b1909`) with the arm's `runtime/` beside it, found there by `heroes
doctor` (*"look in ./runtime and beside the compiler"*), so a grading run must not stand in a folder
holding a `runtime/` of its own; S3 `.../tree-s3/heroes-next`, `sources 06b60f830cca15c0`, its
runtime S2's arm.

### B3, the numbered handle, built and run (supersedes the estimate above)

**The layer's handle is already a number with a generation.** `hero_net_find`
(`runtime/platform/hero_net.c:281-295`) reads a `hero_net_socket *` as `(index + 1) | life << 32`
and refuses any value whose `life` the slot no longer holds; the pointer is never dereferenced. So
the numbered handle keeps R3's *"never another socket that took its number"* by construction.
Built (`netnum/`, a runtime copy): `int64_t hero_net_number(hero_net_socket *)` and
`hero_net_socket *hero_net_of(int64_t)`, 8 lines in `hero_net.c` and 6 in `hero_net.h`.
`b3n/perthread.hero`: a loopback listener, four connections, each accepted on the main thread and
handed to its own worker through `hero_thread_spawn` as that number; the worker takes it back with
`hero_net_of`, echoes five bytes and closes. Then the first worker's number, held past its close
with a fresh socket opened meanwhile, is sent on.
- First build: **`panic: a C handle was given back to hero_net_number, and the mark that began its
  life names hero_net_close`**, exit 134, three of three: the handle marks follow a handle into a
  number. With the binding written `-> Socket acquires hero_net_close | hero_net_number` (spec § 13's
  set form, `spec/heroes-spec.md:461`): `worker sent 5` four times, `echoed 20 bytes over 4
  connections`, **`a number held past its close: send -1, not open true`**, exit 0, three of three;
  under `--sanitize` the same, two of two.
So **one connection per thread is servable under T1's rung today with two layer doors and no change
to the checker, the emitter or the rung**, and the marks make the hand-over explicit
(`hero_net_number` consumes, `hero_net_of` acquires). What it does not give: a socket two threads
use at once (the layer's own `busy` count would serialise it, unrun), and Windows (unrun; the
handle's encoding is the same C on every arm).

### Part A, the three compilers on the same phases (`parta2.sh`, each run with a slowed `git` of its own; `runs-base2.log`, `runs-d.log`, `runs-a2.log`)

Base `tree/heroes-next` (`796d1133542b1909`), route D alone `tree-d/heroes-next` (`201bf3d413214e1f`,
built by my base compiler: `staging.hero` +12 -2, `real 290.33`, `user 155.51`, a waiting run), route
A2 `tree-a/heroes-a2` (`7d9014d844216c9c`). "Edit" is `small`'s; every exit is the compiler's own.

| phase, then the later run | base | route D alone | route A2 |
|---|---|---|---|
| (d) then `fetch`, another number | dead folder and edit stand, hidden | the same | 0 folders; the edit put back, then replaced by `fetch` (`placed small`) and dropped |
| (d) then **`fetch`, the dead number** | **0 folders, 0 files hold the edit** | the dead folder stands (renamed to the number), edit kept in it | the same as another number |
| (d), `small` moved away, then **`check`, the dead number** | **edit gone** with the folder | folder and edit stand; `package_unread ... not placed`, true | 0 folders; **edit back in place; the message now `the tree placed at ... is sha256-33c8..., and the catalogue's sum for it is sha256-1909...: a file of it was changed ... heroes fetch places it again`**, true |
| (a) `fetch`, `add`, `check` killed in `git`, then the same verb | each dead folder stands for ever; the killed `check`'s `heroes.packages/` too | the same | 0 folders each; after the killed `check`, **`heroes.packages/` gone**, as the program was |
| (b) planted between the renames, then `check` | `not placed`, folders and hidden edit stand | the same | 0 folders, edit in place, the sum message |
| (b), then `fetch` | `placed small`; folders and the edit, hidden, stand | the same | 0 folders; edit put back then replaced |
| (c) `add large` killed after `large` placed, catalogue unwritten, then `check`, then `fetch` | folder and `large` (named by no catalogue) stand through both | the same | **the first `check` undid it**: 0 folders, `large` taken away, `heroes.packages/` gone |

**So route D alone closes exactly what the defect measured, the silent loss under a reused number,
at 12 lines, and nothing more**: every killed run's folder, a `heroes.packages/` the author never
made and a tree no catalogue names stand for ever, and an edit moved aside stays where no build
looks. **B and C as briefed carry D** (each is false of the code while `staging.hero:48` removes a
standing name, as the shared brief says): C is D plus a golden pinning the leftovers; B is D plus a
line telling them, which is a new message, R19's fixed interface, outside this lane.

**The runtime suite on a tree holding all three new parts** (`tree-rt/`: route A's `lock.c`, S1's
`stop.c`, the `str` door's `spawn_str.c`, their `hero_os.h` declarations and the guard lines the
three `hero_os.h` names owe; `./heroes-rt run tests/harness/main.hero -- ./heroes-rt runtime`):
**`runtime: 10 passed, 0 failed`**, two floors told as outgrown and counted, never failed (48
runtime source files against 37, 111 objects that survive between calls against 83). Rule 1 held
because the `str` door's box goes through `hero_alloc` (my first draft's `malloc` would have been
its stray); Rule 3 admits the stop's two `static _Atomic int` with no allow-list line, and finds no
object in `lock.c`. `real 2252.90` against `user 60.68`: a waiting run.

### Platforms

- **Linux arm64: unrun until the coordinator runs it.** `linux-a.sh` in my folder is
  `docs/platforms/linux/LINUX-MACHINE.md` § How to run something's body (a read-only mount copied
  inside, no Dockerfile): it copies `tree`, `tree-d`, `tree-a`, `fx-src` and `parta2.sh` without
  `build/`, `.git` or my Mac binaries, builds each compiler from its own seed in two stages, and runs
  the phases for the base, route D and route A2 (`heroes-a2`). Every case makes its own repository
  (`git init -q`, `parta2.sh`'s `mkfx`) and none needs `curl`. The command, from my folder:
  `docker run --rm -v "$PWD":/src:ro heroes-linux-arm64:latest bash /src/linux-a.sh 2>&1 | head -c
  1048576 > linux-a.log`. Not run by me: the brief gives the container to the coordinator.
- **Windows: compiled, not run.** `zig cc -target x86_64-windows-gnu -std=gnu11 -Wall -Wextra`
  (0.15.2, both caches in my folder, `find ~/.cache/zig -newer` 0) over `tree-rt/runtime/runtime.c`,
  which holds route A's door, S1's stop and the `str` door: exit 0, `nm` shows `hero_lock_take`,
  `hero_stop_asked`, `hero_thread_spawn_str` defined; the one warning is the base's own
  (`replace.c:89`, an unknown pragma). The same for `s1/`, `s2/` and `tree-a/` alone. Its run is the
  CI's Windows leg after a push.

### Part B, what I would adopt, from the runs above

- **B1**: correct `hero_os.h:430-434` (the header's prototype decides, `build` tells it as
  `ffi_callback_type` on the author's line); write the rung the sitting rules, and **do not write
  T1's or T2's "nothing else crosses" as if R5 had been narrowed**: no record says it was, and R5's
  `str` rung ran clean (0 ThreadSanitizer warnings, 0 ASan reports) for 23 lines of door. If the
  sitting keeps `i64` alone, the spec sentence should say *today* and cite the gap, so 1.x widens it
  by addition (panel 232).
- **B2**: **S1**, explicitly armed, **with route (c) in the layer's blocking calls** and three
  rules written beside it, each measured above as false of the draft: the stop leaves a signal it
  inherited ignored, as `run.c` does; it says what happens to a child waited on (today: not
  forwarded), or forwards the stop's two itself; and a second Ctrl-C ends the program. Not S2: a
  query that changes how the process meets a signal is a side effect a reader cannot see at the
  call. Not S3: it reserves a word (`s3clash.hero` refused) and moves every program's C (13 `diff`
  lines in the smallest), for no capability S1 lacks; the blind seat's t3 decides between spellings
  on its measurement, not I. Not (a): it starts every child deaf to TERM (`survived`). Not the
  program's own header: it dies on a C thread (exit 134) and splits across modules.
- **B3**: one connection per thread **is already servable under T1** with the layer's numbered
  handle (14 lines, run: 4 connections, a stale number told `not open`); `M-core-packages.md:35`'s
  *one thread* needs no change at all. Neither needs a handle rung, which is the only way out that
  would touch the checker and the ownership pass.

### Part B, verdict (compiler-engineer)

- `verdict`: **object** (no veto: no candidate adds a core construct).
- `section`: design.md §1.12 (a program that cannot be stopped by the signal every supervisor sends,
  and a child left running, are robustness, not ergonomics), with CLAUDE.md § 12's *spec beats
  compiler*: S1 to S3's sentence would be false of the runtime the day it lands. §1.7 and Part 5 for
  the veto I do not use.
- `implementation_cost`: S1 `runtime/parts/stop.c` 46 + `hero_os.h` +7 + `runtime.c` +1 + each
  guard header +2, `selfhost/emit/guarded_names.hero` 1 line; S2 one name fewer (48, +5, +1, +1);
  S3 S2's runtime plus `selfhost/inventory.hero` +6 -2 and `selfhost/library_source.hero` +6, a
  reserved word, and every emitted program moved (13 lines in the smallest, against 1 for S1); route
  (c) `runtime/platform/hero_net.c` +2 on POSIX, Windows unbuilt; T1/T2 0 code lines, the header's
  comment 4; R5's `str` door 23 + 6; the numbered handle 14. Lexer, parser, checker, descriptors,
  ownership, emitter: 0 for every candidate.
- `needed_for_self_hosting`: no.
- `argument`: Every candidate is a door or a library line, so no veto. But the promise all three
  spellings write, *"finishes what it holds and returns from `main`"*, is false of the runtime as
  measured: blocked in the layer's `accept` the program never returns and turns `timeout`'s SIGTERM
  into a flag (alive past 30 s, only `kill -9` ended it); under `heroes run` the drain is killed at
  2000 ms; arming takes INT and TERM out of defect 425's forwarding, so a waited child runs on; a
  second Ctrl-C does nothing. Route (c) mends the first in 2 lines. And T1's *"nothing else
  crosses"* freezes a gap, not a ruling: R5's `str` rung ran clean.
- `prediction`: if S1, S2 or S3 lands without route (c), a server built over the platform layer,
  blocked in `hero_net_accept` with no client and sent SIGTERM under `timeout -k 3 15`, ends at exit
  137 with no drain line on Darwin and on Linux arm64 at M-core-packages' server step; with (c) it
  prints its drain line and exits 0 on both.
- `condition`: a Linux arm64 run in which (c) does not wake a thread blocked in `accept` (the signal
  taken by another thread), which would send me to route (b); or a spec sentence for the stop that
  names `accept`, `heroes run`'s grace and a child waited on, which would turn the objection into an
  approval of S1. The blind seat's t3 may move my preference among S1 to S3, not the objection.

(f) on all three: the second `fetch` (exit 0) left the live run's first folder standing, and the
live run ended at exit 0 with no folder left; under the base and D only because the two numbers
differed. (g) on route A2, two `check`s started at once over one planted dead folder, ten trials:
both exit 1 each time (the true sum message above), **0 folders left in 10 of 10, and the edit in
place exactly once in 10 of 10** (`grep -c 'folders: ;' runs-a2.log`: 10). The first route A
binary's (g) read the same, 10 of 10.

### Part A, what moves under the route, run

- **The `fetch` golden form on route A2** (`tree-a/`, made its own repository with `git init -q` so
  the hook judges it as the tree it is; `./heroes-a2 run tests/harness/main.hero -- ./heroes-a2
  fetch`): **`fetch: 48 passed, 0 failed`**, over the folder's 47 `.case` files (`ls | wc -l`; the
  form counts one check more than the folder holds files, which I did not trace). No golden's text
  moved: R19's interface holds.
- **The compiler's own tests on route A** (the first binary, `heroes-next`, before the five lines of
  A-3): **2108 tests, 1 failed**, `"every word a module hands clang is on the line the edits are asked
  of (defect 2355)"`, whose failing word is `selfhost/cli/doctor.hero: -dumpmachine`. That test reads
  only `selfhost/cli` and `selfhost/emit`, byte-identical between the base and route A (`diff -rq`,
  no output), so the red is the base's own; **that it fails on the base is an inference from
  identical inputs, unrun on the base** (the coordinator's gate on `b33-round` would see it). The
  defect 2434 tests and every `fetch`/`staging` test passed. Not re-run on `heroes-a2`.
- **The emissions**: a program emitted by the base and by route A differs in exactly the
  `hero_program_sources` line (B2's measurement), so the landing re-blesses that one line in up to
  861 files; route D alone moves none.
- **Rule 3**: the runtime suite on `tree-rt`, above, `10 passed, 0 failed`.

### Part A, verdict (compiler-engineer)

- `verdict`: **approve** route A with route D inside it, **as built in `tree-a/heroes-a2`** (door
  in `runtime/hero_compiler.h`, recovery once before discovery asks a placed tree). Route A as
  defect 2453's comments wrote it, recovery at the start of `add` and `fetch`, leaves every killed
  `check`'s folder for ever; recovery inside `staging.begun` tells a false `package_unread`
  (measured). Neither is what I approve.
- `section`: design.md §1.12 (the silent loss of the author's only copy is robustness, CLAUDE.md §
  Precedence rank 3), held under §1.1's ceiling and §1.7's core-or-sugar test, which it passes.
- `implementation_cost`: `selfhost/` +242 -7 (`fetch/staging.hero` +62 -7, 286 to 341;
  `fetch/leftover.hero` new, 175, 96 code lines; `module/packages.hero` +5); `runtime/` +115
  (`parts/lock.c` 101, 52 code lines; `hero_compiler.h` +10; `runtime.c` +4). Lexer, parser,
  checker, descriptors, ownership, emitter: 0; no guard header, no message, no spec token. One line
  re-blessed in up to 861 emitted files. Owed: a compiler test for `leftover.recovered`, the `fetch`
  form's planted-folder cases, the lockless fallback where `flock` is refused. Against 217,480 lines
  of `selfhost/` `.hero`, 0.11 %. Route D alone: `staging.hero` +12 -2.
- `needed_for_self_hosting`: no.
- `argument`: The loss is real on my compiler: a later `fetch` or `check` handed a dead run's number
  deletes the only copy of the author's edit at exit 0. Route D alone stops exactly that, in 12
  lines, and leaves every killed run's folder, a `heroes.packages/` the author never made and a tree
  no catalogue names for ever, measured. Route A with D left 0 folders in every phase, kept a live
  run's folder, survived two recoverers 10 of 10, kept its lock out of its `git` child (`lsof`), and moved no
  `fetch` golden. Its one flaw was where the brief put recovery: it told a false message, repaired
  by running it before discovery. No construct, no message, no token: the soundness lane holds.
- `prediction`: landed in `heroes-a2`'s shape, `tests/golden/fetch/` passes with no case text
  changed and the compiler's own tests gain at least one case naming 2453, with `git diff --numstat
  ef50a7478..<landing> -- selfhost runtime` between +330 and +600 lines net at M-core-packages' tag;
  and `linux-a.sh`'s Linux arm64 run reads 0 `.fetching-*` folders after every later run under
  `heroes-a2`, and the edit lost under the base in its 1b and 1c.
- `condition`: a Linux arm64 or Windows run in which a phase under `heroes-a2` leaves a dead folder,
  loses the edit or removes a live run's folder; a supported file system on which `flock` is refused
  outright (then the lockless fallback must land with the route, or I object); or the panel ruling
  that the ledger is a fifth input class under R12, which would leave route D alone as the lane's
  answer and send A to a full sitting.
