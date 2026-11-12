# Panel 246, the completeness critic's first pass, over the briefs

Written 2026-11-12 from 00:58, before any seat starts, by the completeness critic, under
`docs/panel/246-briefs/critic-briefs.md`. Not a judge: no verdict on any route. Every fact below
carries the command run from `.claude/worktrees/p246` (or from my folder
`.claude/worktrees/p246-scratch/critic/`) and what it printed. Written as I go; the numbered list at
the end is the report.

## Setup

- `git rev-parse HEAD origin/main` printed `b8c8240d43ae9fc3cb336816d70f42cf5cf66628` twice;
  `git status --short` printed only `?? docs/panel/246-briefs/`.
- Every `*.md` brief equals its `*.before-critic.md` copy (`diff -q`, no output for each of the six).
- `.claude/worktrees/p246-scratch/critic/` is ignored (`git -C <root> check-ignore -v` printed
  `.gitignore:34:.claude/worktrees/`).
- `uptime` at 00:58: load averages 4.96 5.27 6.58. No duration in this report is a measurement.

## Log, as I went

### 01:00, the compiler at the base

- The tree is `git archive b8c8240d4 | tar -x` into `p246-scratch/critic/tree` (no worktree
  metadata written). Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0.
  Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0 (`real 186.27`, `user
  126.51`, `sys 25.09`: a waiting run, not a duration). `./heroes-next --version`: `heroes 0.6.0`,
  `sources 017930ae6061c8b4`, as 00-shared.md says.

### 01:01 to 01:05, framing facts that held (each run, each printed what the brief says)

- The spec: `wc -l` 492, `wc -c` 28431, `shasum -a 256` `fa765470...93141c0e` and the same for
  `git show b8c8240d4:spec/heroes-spec.md`; `grep -n -i thread` one line, 377; *There are no mutable
  globals* at 135; `grep -n -i ships spec/heroes-spec.md` prints nothing (exit 1). My stage 2's
  `measure spec/heroes-spec.md` from my tree's root: claude-legacy 8236, cl100k_base 8386, maximum
  8386, `real 10973`, *claude-opus-5, 2026-11-11*, *the binding number*, headroom 5411, 11033 judged.
- Panel 235: `grep -n fifth` hits line 723, which lies inside **R12** (712 to 728), not R19 (788):
  the shared brief's correction holds. R19 at 788 reads as quoted.
- `.claude/rules/cli-surface.md:54-57` and `:76-78` read as quoted. `ls tests/golden/fetch | wc -l`
  47, `| grep -c 2434` 5. `grep -n -i kill tests/harness/suite_fetch.hero` prints nothing (exit 1).
  The lock search over `runtime selfhost tests/harness` finds 0 files.
- Panel 232 `:272-274` holds the promise as quoted; design.md `Amended 2026-10-27` at 3748,
  `Global mutable state` at 3572, `### 1.6` at 253, the ceiling at 255, `payment` at 282, 301, 324,
  328 and 330, `stays as written` at 4079, `isolated per-thread heaps` at 4089.
- Line counts (`wc -l`): staging.hero 286, add.hero 458, fetch_all.hero 260, unplaced.hero 112,
  process.hero 475, hero_os.h 488, hero_compiler.h 80, fs.c 415, replace.c 1268, run.c 1962,
  spawn.c 444, thread.c 99, check/ffi.hero 412, emit/ffi_narrowed.hero 499: all as the briefs say.
  add.hero's `begun` 90, `ended` 94, `undone` 97, `publish.rewrite` 116; fetch_all.hero's `begun`
  53, `ended` 61 and 96, the comment 94-95; process.hero's group 40-68 (`hero_fs_exists` to
  `hero_run_serial` 54-62), `pid` 275, `serial` 282; run.c's serial 1425-1428
  (`static _Atomic int64_t hero_run_serial_next = 0;` ... `return ++hero_run_serial_next;`).
- hero_os.h: the `hero_fs_*` declarations run 193 to 300 and the comment block opens at 172 (the
  shared brief says 193-300, the two seat briefs 172-300: both true, of different things);
  `hero_os_pid` 333, `hero_run_serial` 415, `hero_thread_spawn` 435, `_sized` 441, `_join` 443,
  `_limit` 447.
- The blind seat's arms: rebuilt by `p246-scratch/critic/arms.py` from 00-shared.md's sentences
  alone, x, y, z, p and q hash as the brief says (`fa765470cf65ac41`, `5943625fca746d85`,
  `9feb9c4cd1af64b9`, `e7f845cc476f8d0e`, `5d739d1f1c7e57af`). **Arm r does not, by the rule as
  written**: S3 put directly after `refused)` hashes `aaa2c30fb6a54a12`; put after `refused) · `
  (space, middle dot, space, the built-ins table's separator) it hashes `7874df3e1b62d9ee`, the
  brief's. Item 13 below.
- The spec-warden's table: every row reproduced by my stage 2's `measure` on those arms (x 8386,
  y 8459, z 8401, p 8456, q 8451, r 8444, T1+S1 8529, T1+S3 8517, the maxima), each printing *No
  ceiling judges ... a count and not a verdict*.
- t1's expected line, `python3 -I -c 'print(sum(i*i for i in range(800000)))'`:
  `170666346666800000`, under 2^63. t2's counts `[4, 2, 6, 2]`, sum 14.
- `claude --version` `2.1.311 (Claude Code)`; `claude --help` lists `--bare`, `--restricted`,
  `--safe-mode`, `--strict-mcp-config`, `--max-budget-usd`, `--tools`, `--allowedTools`,
  `--disallowedTools`, `--model`, `--output-format`. Panel 244's cost line and panel 239's `1.5671`
  read as quoted.

### 01:04 to 01:10, flagged item 1: PID reuse, reproduced on this Mac

A private repository in my folder (`p246-scratch/critic/pid/repo`, `git init`, so the `file://`
index sits inside the program's own repository as panel 235's R9 C4 asks): an index of two
packages, `small` (one file) and `large` (3,000 files, to widen the window), `app/heroes.toml`
naming both at their sums. `heroes-next fetch` placed both, exit 0.

1. **The second comment of defect 2453, reproduced at the base.** The author's edit appended to
   `small`'s placed `a.hero` and to one file of `large`; `heroes-next fetch &`, a loop polling for
   `heroes.packages/.fetching-*/replaced-0`, then `kill -9`: `wait` 137, and the tree left
   `.fetching-5898-1/` holding `git-root.txt`, `git-said.txt`, an empty `tree/` and
   `replaced-0/a.hero`, the only file under `heroes.packages` that holds the edit
   (`grep -rl 'the author edited this' heroes.packages`). The ledger's folder is serial **1**.
2. **Control**: a later `fetch` with another PID, exit 0, `kept small`, `placed large`; the dead
   folder and the edit stand.
3. **PID reuse, simulated without waiting for the kernel**: the killed state restored, then
   `sh -c 'mv heroes.packages/.fetching-5898-1 heroes.packages/.fetching-$$-1 && exec heroes-next
   fetch'`, so the later run holds exactly the PID the dead folder's name carries (`exec` keeps
   the shell's PID; renaming the folder to it is what the kernel recycling 5898 would do).
   Printed `kept small`, `placed large`, exit 0, nothing about the folder; afterwards
   `heroes.packages` holds no `.fetching-*` and **no file holds the edit**.
4. **The same through `heroes check`**: the killed state restored, `small`'s placed tree removed,
   `main.hero` holding `use <small>/a`; the shell's PID given to the dead folder, then `exec
   heroes-next check main.hero`: `error[package_unread]`, exit 1, and the dead folder and the edit
   are gone. With `use small/a` (misspelled, `error[package_unmarked]`) the same: gone. The control
   `check` with another PID leaves both.

So the inference of 00-shared.md § Premises holds, measured: a later run handed a dead run's PID
removes that run's ledger folder and `replaced-N` silently at exit 0, **and the later run may be a
`heroes check`, `build`, `run` or `test`, not only `add` or `fetch`** (`packages.hero:121` calls
`unplaced.refusal` from `reach`, which every verb resolving a `use <name>/...` runs). Route A's
recovery *at the start of `add` and `fetch`* does not reach that path. How soon a PID comes back on
this Mac: PIDs 5898 (01:05) to 14913 (01:10, `sh -c 'echo $$'`), a second reading below.

### 01:12 to 01:16, flagged item 2: a killed `heroes check` on the `unplaced.hero` path

The same program, `heroes.packages/` removed, `small`'s source a `file://` index, `main.hero`
holding `use <small>/a`; `git` slowed by a wrapper first on `PATH`
(`p246-scratch/critic/slowgit/git`: it logs its pid and group, sleeps 6 s, then runs `/usr/bin/git`);
`heroes-next check main.hero &`, then `kill -9` once the wrapper had logged.

- **What it leaves**: `heroes.packages/.fetching-17297-1/` holding `git-root.txt`, `git-said.txt`
  and an empty `tree/`, **and `heroes.packages/` itself, which the program did not have before**
  (`find heroes.packages` after the kill; `ls -a` before it showed only `heroes.toml` and
  `main.hero`). It places nothing and moves nothing aside: nothing for a recovery to put back,
  but a folder and a `heroes.packages/` the author never made. `check` is the verb an editor runs
  most and kills most; `build`, `run` and `test` reach the same path (`packages.hero:121`).
- **Its git child does not outlive it**: before the kill `ps -g 17301` showed the wrapper (17301,
  its own group, parent 17297) and its `sleep 6` (17305); after it, `ps -p 17301` and `ps -p 17305`
  print only the header and `pgrep -f .../slowgit/git` nothing. The runtime already ends the children
  of a killed `heroes`: `runtime/parts/run.c:506-557` (defect 425: the four ending signals forwarded
  while a child is waited on; defect 437: Linux's `PR_SET_PDEATHSIG`; the Windows job object) and
  `:734-783` (defect 465 and panel 200's R2: on Darwin a SENTINEL, `/bin/sh` started by
  `posix_spawn` with `POSIX_SPAWN_CLOEXEC_DEFAULT` since defect 575, ends every child still named
  when the runner's pipe reads its end). So 00-shared.md A3's *a child (`git`, `curl`) that
  inherits the lock and outlives its killed parent* is not the shape this runtime has: what is left
  to ask is a child between its fork and its exec, the sentinel's own reaction time, and a program's
  C forking without exec (the sentinel's own written premise). No brief cites these lines.
- **What it means for route A as briefed**: a folder `check` makes carries no lock and no ledger
  today. A recovery that takes *every `.fetching-*` folder whose lock it can take* must decide what
  a folder with no lock file is: taken, it removes a LIVE `check`'s folder while its git writes
  into it; left, a killed `check`'s folder stays for ever. And the on-disk ledger must carry the
  in-memory `found` (`staging.hero:117-119`), or recovery cannot know whether the empty
  `heroes.packages/` is the author's or the dead run's. Unrun as code: route A does not exist yet.

### 01:17 to 01:24, flagged item 3: feature 1069's `stopflag.h`, panel 234's R6, panel 114's R2

- **What R6 refuses.** `grep -n 'ships platform C' docs/panel/234-*.md`: line 421, inside R6 (400
  to 429): *"its veto of panel 114 R3 binds the shipped header of P3, so **no package ships
  platform C of its own**"*. Feature 2295's body says the same of the layer. The sentence names a
  **package**; nothing in R6 speaks of a program.
- **What leaves it open for a program.** Panel 114's **R2**, ratified 2026-09-06 *"in full"*
  (line 281): *"A program may name a header it ships beside itself, and the spec says so at a net
  cost of +1 token"*, with R6 *"a header of `static inline` functions needs no `compile` at all"*.
  **R2's spec sentence is not in the spec and never was**: `grep -n -i
  'conditional\|beside its own\|header it\|own header\|static inline' spec/heroes-spec.md` prints
  nothing; `git log --oneline -S '<s>' -- spec/heroes-spec.md` prints nothing for *beside its own
  file*, *no conditional compilation* and *ships beside*. R3 made R2's landing wait on three
  instruments; whether they were built, and why the sentence stayed out, I did not trace (a
  question). The ffi-pragmatist's brief notes the absence; no brief says the ruling is ratified and
  unlanded.
- **The route, rebuilt at the base** (`p246-scratch/critic/stop1/`): 1069's three-line header
  (`static volatile sig_atomic_t stop_requested`, `static inline` `stop_request()` and
  `stop_asked()`), a Heroes handler bound through `signal.h`'s `signal` (it must read its `sig`, or
  `error[unused_binding]`), the loop asking between steps, `kill(getpid(), SIGINT)` at the third:
  built exit 0, printed `drained after 3 requests`, exit 0. As 1069's re-check said.
- **The shape beside it, run** (`p246-scratch/critic/stop2/`): the handler in a second module
  `handler.hero`, each module binding `stopflag.h`. Built exit 0, printed **`drained after 1000
  requests`**, exit 0: the handler set its own unit's flag, the loop read the other's, and only my
  cap of 1000 ended the loop. Without the cap such a program no longer ends on Ctrl-C at all, and
  the compiler says nothing: a silent wrong program, a robustness question (§ Precedence rank 3),
  not a token question. The ffi-pragmatist's brief names *one per C unit* as a price to weigh; none
  says it was run.
- **Whether the briefs present it as a route**: the compiler-engineer's B2 lists it as *a
  package's own header* among the homes of the door; the ffi-pragmatist's B2 asks it rebuilt and
  priced against R6; the spec-warden's question 3 offers *a package's card* or *`hero_os.h`'s
  comments*, not a program's own header; the historian's and the blind seat's briefs do not name it,
  and no blind arm carries panel 114 R2's ratified sentence. So it is presented as a **package**
  route, which R6 already refuses, and never as the **program** route R2 left open.

### 01:24 to 01:32, what crosses into a server's thread: the socket is a handle, not an `i64`

- **Panel 234's R3, on the base and ratified 2026-11-06** (`sed -n 312,330p docs/panel/234-the*.md`,
  item 3): *"a descriptor (a POSIX `int`, a winsock `SOCKET`) is wrapped in C as a struct behind a
  pointer so it carries the marks"*. Lane `core-platform`, uncommitted in its worktree (`git -C
  .claude/worktrees/core-platform status --short`: 29 entries, `runtime/hero_net.h`,
  `runtime/platform/hero_net.c` among them), writes that: `hero_net_socket *hero_net_accept(...)`,
  *"a group declares it as a handle, `record Socket tag hero_net_socket`"*, *"A socket is a handle
  the layer issues and checks, never an address it reads through ... never another socket that
  took its number"*.
- **Run at the base** (`p246-scratch/critic/handle/`): a header declaring `hero_net_socket` and a
  Heroes body `(function(s: Socket) -> i64)` with `record Socket tag hero_net_socket`, handed to
  `hero_thread_spawn` with `arg: Socket`: `check` exit 0, `build` exit 1,
  `error[ffi_callback_type]` *"clang read the header, and it says `long long (*)(long long)`"*.
- So 00-shared.md B3's *"the descriptor crossing as the `i64`"*, the ffi-pragmatist's B3 (*whether
  the `i64` crossing holds it on every platform*), feature 1073's *"One connection per thread needs
  only the descriptor, which is an `i64`"* and feature 893's re-check of 2026-10-25 (*"A descriptor
  is a number, so it crosses as the `i64` the body takes"*) all rest on a premise R3 overturned for
  any server written over the core packages: under T1's rung **an accepted connection cannot reach
  a worker thread at all**, unless the layer gives a handle a number and back (which R3's
  *never another socket that took its number* exists to refuse), the rung admits a handle (a
  move, with its marks, across the boundary), or the step is one thread (`M-core-packages.md:35`,
  *"writable, one thread"*; the lane's own case is
  `the-platform-layer-echoes-over-loopback-in-one-thread`). No brief asks it.
- The lane also rewrites spec line 377 in place (`git -C .claude/worktrees/core-platform diff --
  spec/heroes-spec.md`: the F3 sentence, word for word as the spec-warden quotes it), so the blind
  arms' anchor `` `hero_os.h`. `` stops existing when the lane lands, and it moves the spec's pins
  (`selfhost/measure/pinned.hero`, `docs/measurements/010-spec-budget-ledger.md` modified there).

### 01:30, how soon a PID comes back on this Mac

`date +%s; sh -c 'echo $$'` at 01:07 printed 1794442039 and 14913; at 01:11, 1794442323 and 26541:
11,628 PIDs in 284 s under this evening's load. `grep -rn 'define PID_MAX'` over the SDK's
`usr/include` prints nothing, so the cycle's length is not read here: how soon a given PID recurs
is a question whose two numbers are this rate and that length (macOS's is commonly given as 99,999,
unverified).

### 01:32 to 01:40, Part A's platforms and neighbours

- **The tree already rules on this name the other way.** `selfhost/cli/stub.hero:172-182`,
  `heroes new`'s staging folder `.heroes-new-<pid>-<serial>`: *"A staged name that stands already,
  a run of this process number killed before it ended, is refused and never removed: it is not this
  run's."* `staging.hero:48` removes the same shape of name. No brief cites it; it is a route
  nobody listed for the PID-reuse shape (below), and a precedent any route must answer.
- **Routes B and C as briefed do not stop the removal measured above**: *report the leftovers and
  remove nothing* is false of the code while `opened` removes a folder of its own name first; C's
  pin pins a tree that a later run of the same PID deletes. Whatever the sitting adopts, line 48
  is in it.
- **The Linux leg as briefed cannot run a `file://` kill case.** `LINUX-MACHINE.md:126-138`'s body,
  which both seat briefs quote, copies with `--exclude=.git`; my stage 2 `fetch` from a program in
  no repository (`GIT_CEILING_DIRECTORIES` set above it): `error[source_refused]: \`file://../idx\`
  is read only from inside the program's own repository, and the program's folder is in none`,
  exit 1. `docker run --rm heroes-linux-arm64:latest sh -c 'command -v git; command -v curl'`
  printed `/usr/bin/git` (2.47.3) and **no curl**. `docker images`: `heroes-linux-arm64:latest
  338a1044ac90`, as briefed.
- **Windows compiles on this Mac.** `find /opt/homebrew -maxdepth 6 -name windows.h` and `which
  x86_64-w64-mingw32-gcc` find nothing, as briefed, but `which zig` finds
  `/Users/joseph/.zvm/bin/zig` (0.15.2), and `zig cc -target x86_64-windows-gnu -Wall -Wextra`
  compiled and linked `p246-scratch/critic/win/lock.c` (`CreateFileW`, `LockFileEx(LOCKFILE_EXCLUSIVE_LOCK
  | LOCKFILE_FAIL_IMMEDIATELY)`, `SetConsoleCtrlHandler` with CTRL_C, CTRL_BREAK and CTRL_CLOSE,
  `InterlockedExchange`) at exit 0, no warning, `file`: `PE32+ executable (console) x86-64`; its
  caches pointed into my folder (`ZIG_GLOBAL_CACHE_DIR`), `find ~/.cache/zig -newer lock.c` 0. Not
  run: `which wine wine64` finds neither. Running it is the Windows box's (`.claude/rules/platforms.md`
  § The Windows box: *"Paid by the hour, usually off, and only the author can start it"*), or the
  CI's Windows leg after a push; no brief names the box or its cost.
- **Panel 235's R7 speaks of a lock too**: *"no user cache folder is read and no lock between two
  programs is needed (the compiler-engineer, § 7)"* (`sed -n 635,652p`). A1 quotes R12 and R19 and
  not R7.
- **The runtime suite's Rule 3** (`tests/harness/suite_runtime.hero:144-196`, the allow-list from
  222): every object a runtime part keeps between calls is `_Thread_local`, `_Atomic`, or a line of
  `SHARED_BY_DECISION` with its reason. A held lock descriptor (Part A) and a stop flag (Part B)
  are both such objects; the precedent for a flag a handler sets is `runtime/parts/panic.c
  hero_runtime_spoke`, `volatile sig_atomic_t`, allow-listed at line 229. No brief names the suite
  or the precedent.

### 01:40 to 01:52, Part B's records and probes

- **The coordinator's probes, rebuilt with my compiler** (`p246-scratch/critic/probes/`, copies of
  `probe-cross/{s,a,g,h}.hero`): `s` check exit 0, build `error[ffi_callback_type]`; `a`
  `error[ffi_type]` exit 1; `g` `error[expected_declaration]` exit 1; `h` built exit 0 and printed
  `caught 2`, exit 0. `probe-const/main.c:130` holds `HERO_ARRAY_STATIC(hero_constant_h_main_WORDS_4,
  ...)` returned by `h_main_WORDS()`; `heroes_runtime.h:196`: *"refcount < 0 means static, never
  freed"*. All as briefed.
- **Panel 113's R5 was ratified, and the runtime ships less.** `grep -n 'the scalars and'
  docs/panel/113-*.md`, line 276, in R5 (258 onward): *"the body
  signature admits only what is measured safe to cross. Today that is the scalars and **`str`**"*;
  line 410: *"RATIFIED 2026-09-06 by the author, in full"*. Journal 034 (`grep -n -i '\bstr\b'
  docs/records/journal/034-isolated-threads.md`) records *"Only an `i64` crosses a thread
  boundary"* (line 79) and gives no reason why `str` and the other scalars were left out; I found
  no ruling that narrowed R5 (searched: journal 034, panel 113, design.md Part 7 item 13). And
  panel 111's *"9 reported `heap-use-after-free`"* for a shared `str` (lines 129-131), which
  feature 1073's body cites as the reason not to widen, was measured **before** the atomic count:
  design.md:4082, *"The refcount half is closed at M-isolated-threads step 3"*.
- **`hero_os.h`'s own comment is false today**: `sed -n 430-434p runtime/hero_os.h`, *"WHAT MAY
  CROSS IS DECIDED BY THE CHECKER ... a `[T]` or a `{K: V}` in that signature is
  `error[ffi_type]`"*; a `str` body and a handle body pass the checker and are refused by clang
  (above).
- **`exit` is not the leak gate**: `runtime/parts/os.c:1005-1010`, `hero_exit`: *"Not the leak
  gate, for `hero_os.h`'s reason"*; the emitted `main` calls `hero_runtime_check_leaks()` before
  `return 0` (`selfhost/emit/c_main.hero:83`, `:102`). So the leak check task 1072 wants *"after
  the long run"* runs only for a server that returns from `main`, which is what S1 to S3 promise
  and what 00-shared.md's measured handler (`exit(code: 0)` in `h.hero`) skips. No brief ties B2
  to 1072's instrument.
- **Panel 111's four classes** (`sed -n 125-170p docs/panel/111-*.md`): a shared `str`'s count,
  `==` on nested arrays through `hero_eq_queue`, copy-on-write's test-then-mutate, and the stack
  guard on a worker. Feature 925's body names what panel 113 did **not** try: *"`sort` through a
  function pointer, the `eq` and `hash` descriptor walks, and the drop-list drain under
  contention"*. B3's enumeration in the compiler-engineer's brief lists neither.
- **The mandate**: `gh issue list --label kind:decision --search 2453` prints nothing; the open
  decisions are 2475 alone. The author's `2a 3a` of about 00:30 and the 10 USD ceiling are not
  readable from the tree or GitHub; they stand in the coordinator's conversation alone.
- **Kinds**: `gh issue view 1072 --json labels`: `kind:task`, not a feature as 00-shared.md and
  the critic brief call it; 893, 925, 1069, 1073 and 2295 are `kind:feature`; all seven on
  `M-core-packages`; 2453 `class:improvement`, *"no wrong value, message or build"*.
- **Children start by `execvp`** (`run.c:1340`), the sentinel by `posix_spawn` (`:900`): the
  ffi-pragmatist's sentence holds: a lock descriptor without close-on-exec rides into every `git`
  and `curl` for its whole life, and one with it only between the fork and the exec.

## The list: each item the fact, the command, what it printed, and the repair to the brief

**Part A**

1. **PID reuse removes a dead run's `replaced-N`, measured (flagged item 1).** `kill -9` of a
   `heroes-next fetch` once `heroes.packages/.fetching-*/replaced-0` existed, then
   `sh -c 'mv heroes.packages/.fetching-5898-1 heroes.packages/.fetching-$$-1 && exec heroes-next
   fetch'` (exec keeps the shell's PID, so the later run holds the PID the dead folder's name
   carries). Printed `kept small 1.0.0 ...`, `placed large 1.0.0 ...`, exit 0, and `grep -rl 'the
   author edited this' heroes.packages` found nothing; the control, the same later `fetch` under
   another PID, left `.fetching-5898-1/replaced-0/a.hero`. **Repair**: 00-shared.md § Premises,
   first bullet: replace *"An inference from reading, unrun"* with this measurement and its recipe,
   so the compiler-engineer's step 5 starts from it; ask the sitting the class question it raises
   (2453 is `improvement`, *"no wrong value, message or build"*; this is the only copy of an
   author's edit removed silently at exit 0), and whether it is a defect to file apart.
2. **The later run may be any verb that resolves a package, not only `add` or `fetch`.** The same
   plant, then `exec heroes-next check main.hero` (`use <small>/a`, its tree removed): printed
   `error[package_unread]`, exit 1, and the edit was gone; with `use small/a`,
   `error[package_unmarked]`, gone too; the control under another PID kept it. The path is
   `selfhost/module/packages.hero:121` → `unplaced.refusal` → `staging.begun` → `opened`.
   **Repair**: Part A's proposal item 3 (*"A later `add` or `fetch` first recovers"*) and A3: name
   every writer of `.fetching-*`, `add`, `fetch` and, through `unplaced.hero`, `check`, `build`,
   `run`, `test` and `mutate`, and ask whether recovery, the lock, or neither is owed on that path.
3. **A killed `check` leaves a folder and a `heroes.packages/` the author never had (flagged item
   2).** `git` slowed by a wrapper on `PATH`, `heroes-next check main.hero &`, `kill -9`: `find
   heroes.packages` printed `.fetching-17297-1/` with `git-root.txt`, `git-said.txt` and an empty
   `tree/`, where `ls -a` before showed `heroes.toml` and `main.hero` alone. **Repair**: § Premises,
   second bullet, replaced by this; ask route A (i) what recovery does with a folder that has no
   lock file (taken, it removes a LIVE `check`'s folder while git writes into it; left, a killed
   `check`'s stays for ever), and (ii) whether the on-disk ledger carries `found`
   (`staging.hero:117-119`), without which recovery cannot tell the author's empty
   `heroes.packages/` from the dead run's.
4. **A child does not outlive a killed `heroes`; the runtime already ends it.** The wrapper (pid
   17301, its own group) and its `sleep 6` (17305) were listed by `ps -g 17301` before the kill and
   absent after it (`ps -p` printed only the header; `pgrep -f .../slowgit/git` nothing).
   `runtime/parts/run.c:506-557` and `:734-783`: defects 425, 437 (`PR_SET_PDEATHSIG`), 465 and
   575 (Darwin's SENTINEL, `posix_spawn` with `POSIX_SPAWN_CLOEXEC_DEFAULT`), panel 200's R2, the
   Windows job object. **Repair**: A3's *"a child (`git`, `curl`) that inherits the lock and
   outlives its killed parent"* rewritten from these lines: the residual shapes are a child between
   its `fork` and its `execvp` (`run.c:1340`) sharing the lock's open file description, the
   sentinel's own delay, and a program's C that forks without exec (the sentinel's written premise).
5. **The tree already treats the same name the opposite way, and B and C as briefed keep the
   removal.** `selfhost/cli/stub.hero:174-182`: *"A staged name that stands already, a run of this
   process number killed before it ended, is refused and never removed: it is not this run's."*
   `staging.hero:48` removes it. **Repair**: add a route nobody listed, D: `opened` never removes
   a standing `.fetching-<pid>-<n>` (refuse it, or take the next serial), `heroes new`'s rule,
   priced beside A, B and C; and say in § Part A that B (*"remove nothing"*) and C (a pin) are false
   of the code while line 48 stands, so each must carry D or say why not.
6. **Panel 235 speaks of a lock in R7 too.** `sed -n 635,652p`: *"no user cache folder is read and
   no lock between two programs is needed (the compiler-engineer, § 7)"*. **Repair**: A1 quotes R7
   beside R12 and R19, so the seats rule on a lock private to one run against both sentences.
7. **A door in `hero_os.h` is itself a surface, which is the lane's own condition.**
   `.claude/skills/panel/SKILL.md:18-34`: the soundness lane is for *"no surface, no diagnostic and
   no spec token"*, and *"A question that looks internal but has a sentence in the spec behind it is
   a full panel"*; spec line 377 names `hero_os.h`, panel 232 (`:272-274`) promises its functions,
   decision 2228 lets every program bind it. `hero_compiler.h:7-17` says why the compiler's own
   doors live apart (*"no program's unit pays for them"*): `heroes_guard_open.h` has 241
   `push_macro("hero_` lines and none for `hero_compiler.h`'s `hero_env_put`. **Repair**: A2 asks
   whether answering `hero_os.h` takes Part A out of the soundness lane, and what such a door owes
   (`heroes_guard_open.h`, `heroes_guard_close.h`, `selfhost/emit/guarded_names.hero`, panel 232's
   promise).
8. **The Linux leg as briefed cannot run a `file://` kill case.** My `fetch` from a program in no
   repository (`GIT_CEILING_DIRECTORIES` above it) printed `error[source_refused]: ... is read only
   from inside the program's own repository, and the program's folder is in none`, exit 1; the
   briefed body copies with `--exclude=.git` (`LINUX-MACHINE.md:134`; its line 153 says when to keep `.git`, not for this); `docker run --rm
   heroes-linux-arm64:latest sh -c 'command -v git; command -v curl'` printed `/usr/bin/git` and no
   curl. **Repair**: the compiler-engineer's step 8 and the ffi-pragmatist's step 5: each case
   `git init`s its own repository (as `p246-scratch/critic/pid/repo` does) or the copy keeps
   `.git`, and no case needs `curl`.
9. **The Windows arms compile on this Mac.** `which zig`: `/Users/joseph/.zvm/bin/zig` (0.15.2);
   `zig cc -target x86_64-windows-gnu -Wall -Wextra -o lock.exe lock.c` (`CreateFileW`,
   `LockFileEx`, `SetConsoleCtrlHandler`) exit 0, no warning, `file`: `PE32+ executable (console)
   x86-64`; caches pointed in with `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR`, `find
   ~/.cache/zig -newer lock.c` 0; `which wine wine64` neither. **Repair**: both seat briefs replace
   *"written and not compiled here"* with this route for compiling (the caches inside the seat's
   folder, never `~/.cache`); running stays on the Windows box, which `.claude/rules/platforms.md`
   says is *"Paid by the hour, usually off, and only the author can start it"*, a cost outside the
   10 USD and the author's to allow, or on the CI's leg after a push: say which.
10. **The runtime suite judges both new objects.** `tests/harness/suite_runtime.hero:144-196`,
    Rule 3: every object a runtime part keeps is `_Thread_local`, `_Atomic` or a
    `SHARED_BY_DECISION` line; `:229` allow-lists `runtime/parts/panic.c hero_runtime_spoke`, a
    `volatile sig_atomic_t` a handler path sets. **Repair**: the compiler-engineer's A step 2 and
    B2 and the ffi-pragmatist's B2 name the `runtime` suite as a judge of the held lock descriptor
    and of the stop flag, and `hero_runtime_spoke` as the precedent.

**Part B**

11. **A connection is a handle, not an `i64`, by a ratified ruling.** Panel 234's R3 item 3
    (`sed -n 312,330p`): *"a descriptor (a POSIX `int`, a winsock `SOCKET`) is wrapped in C as a
    struct behind a pointer so it carries the marks"*; lane `core-platform`'s uncommitted
    `runtime/hero_net.h` hands out `hero_net_socket *`, *"never another socket that took its
    number"*. My run: `record Socket tag hero_net_socket`, a body `(function(s: Socket) -> i64)`
    handed to `hero_thread_spawn`: `check` exit 0, `build` `error[ffi_callback_type]`, exit 1.
    **Repair**: 00-shared.md B3 (*"the descriptor crossing as the `i64`"*) and the
    ffi-pragmatist's B3 rewritten on R3: ask whether T1's rung can serve one connection per thread
    at all (feature 893), and price the three ways out, a numbered handle (which R3's reason
    refuses), a rung that moves a handle with its marks, or one thread (`M-core-packages.md:35`).
12. **The wider rung was ratified, and the narrowing has no ruling I could find.** Panel 113's R5,
    line 276 (*"the scalars and `str`"*), ratified in full at line 410; journal 034:79 records
    `i64` alone with no reason; panel 111's *"9 reported `heap-use-after-free`"* (129-131), which
    feature 1073 cites, predates design.md:4082's *"The refcount half is closed at
    M-isolated-threads step 3"*; `hero_os.h:430-434` says *"WHAT MAY CROSS IS DECIDED BY THE
    CHECKER"*, false for `str` and handles. **Repair**: B1 gives the seats both measurements in
    their order, asks where `i64`-only was decided (a ruling, or a gap between R5 and the code),
    whether R5's *scalars* includes `f64` and `bool`, and names the header comment as false today.
13. **Arm r does not hash as briefed by the rule as written.** `p246-scratch/critic/arms.py`: S3
    directly after `refused)` gives `aaa2c30fb6a54a12`; after `refused) · ` it gives
    `7874df3e1b62d9ee`, the brief's (x, y, z, p, q all match). **Repair**: the blind seat's brief
    says *"replaced with `one written as a number is refused) · ` followed by S3's text"*.
14. **The blind seat's caps are looser than every precedent's.** 239's command `--max-budget-usd
    0.45` (`239-briefs/blind/README.md:58`), 244's `0.36` with *"Bound: 0.36 USD a session, 2.88
    USD for the eight"* (`244-briefs/blind/README.md:52`); their sessions cost 0.1158 to 0.1887 and
    0.1209 to 0.1347. This brief caps at 3 and leans on a runner guard and on an unrun question
    (whether the CLI passes its cap). **Repair**: cap each session at 0.45, so twenty are bounded
    by their caps alone at 9.00 USD inside the 10, the guard kept as a second bound; or say why 3.
15. **The program's own header is a route the briefs give only as a package's, and its trap is
    measured.** Panel 114's R2 (ratified 2026-09-06): *"A program may name a header it ships beside
    itself, and the spec says so"*; `git log -S` over the spec for *beside its own file*, *no
    conditional compilation* and *ships beside* prints nothing. Panel 234's R6 refuses only a
    **package** shipping platform C (line 421). 1069's `stopflag.h` rebuilt: one module, `drained
    after 3 requests`, exit 0; handler and loop in two modules, **`drained after 1000 requests`**
    (my cap), exit 0, Ctrl-C swallowed. **Repair**: present it to every seat as the program's route
    (open by R2, closed to packages by R6) with the two-module measurement as a robustness fact
    against it; ask whether R2's unlanded sentence is owed on its own; and consider a t3 arm with
    R2's sentence (+1 net by 114's own price).
16. **Only a stop that returns from `main` lets task 1072's leak check run.** `os.c:1005-1010`,
    `hero_exit`: *"Not the leak gate"*; `c_main.hero:83` and `:102` call
    `hero_runtime_check_leaks()` before `return 0`. **Repair**: B2 ties the stop to 1072: a seat
    runs a stopped server-shaped loop and reads the leak line, since a stop that ends through `exit`
    (as `h.hero` does) skips the very check the long run is for.
17. **Routes nobody listed for the stop.** (a) SIGINT and SIGTERM blocked by `pthread_sigmask` and
    `sigpending` asked between steps: no handler, no Heroes code in one, no `SA_RESTART` question;
    (b) the stop as a descriptor (a self-pipe's read end, kqueue's `EVFILT_SIGNAL`, Linux's
    `signalfd`) so one `poll` wakes a blocked `accept`, which the ffi-pragmatist's brief names only
    as a fallback, never as a spelling; (c) the stop inside the platform layer's blocking calls,
    since R3 makes the layer own `accept`. All unrun. **Repair**: add them to B2's candidates in
    00-shared.md and the two compiling briefs, and to the historian's list (Python's
    `signal.set_wakeup_fd`, libuv's `uv_signal_t`).
18. **Which thread runs a POSIX handler is asked only of Windows.** A process-directed signal goes
    to any thread not blocking it, so a Heroes handler (the program's route) may run on a thread a C
    library made, where `thread.c`'s guard stops it (panel 114's R4: *"panic: main.worker ran on a
    thread this program did not start"*, exit 134). And `run.c:518` forwards four signals (TERM,
    INT, HUP, QUIT) where S1 to S3 take two. Both unrun. **Repair**: B2 asks for a run with a C-made
    thread alive when SIGINT arrives, and why the stop takes two of the four.
19. **B3's enumeration lacks the shapes already known to be untried.** Feature 925: *"`sort`
    through a function pointer, the `eq` and `hash` descriptor walks, and the drop-list drain under
    contention"*; panel 111's four classes (125-170). **Repair**: add both lists to the
    compiler-engineer's and the ffi-pragmatist's B3.

**The framing**

20. **The mandate is not on record where I can read it.** `gh issue list --label kind:decision
    --search 2453` printed nothing; the open decisions are 2475 alone. **Repair**: the synthesis
    and its decision issue quote the author's `2a 3a` and the 10 USD ceiling in the author's words.
21. **Feature 1072 is a task.** `gh issue view 1072 --json labels`: `kind:task`. **Repair**: say
    *task 1072* in 00-shared.md.
22. **Lane `core-platform` has no commit of its own and a dirty tree.** `git -C
    .claude/worktrees/core-platform status --short`: 29 entries, among them `spec/heroes-spec.md`
    (line 377 rewritten with F3 word for word), `selfhost/measure/pinned.hero`, the spec's ledger,
    `runtime/hero_net.h` and `runtime/platform/`. **Repair**: 00-shared.md says so; the spec-warden's
    question 2 prices against the lane's actual line, which removes the blind arms' anchor and
    moves the pinned `real` when it lands.

What held, run: the base and its `sources` line; every line count and line number in the seat
briefs except the two readings of `hero_os.h`'s block (172 and 193, both true); the spec's size,
hash, line 377 and line 135; the vendored price table, all eight rows; arms x to q; t1's and t2's
answers; the CLI's version and flags; the four probes; the fifth-class line being R12's; and 2453's
two comments, the second reproduced.
