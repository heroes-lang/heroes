# Panel 246, the brief every seat reads first

Convened 2026-11-12 by the coordinator, in M-core-packages (the GitHub milestone of defect 2453, of
features 893, 925, 1069, 1073 and 2295 and of task 1072, `gh issue view <n> --repo
heroes-lang/heroes --json labels,milestone`). **The mandate**: the author's answer `1a 2a 3a 4a 5a`
of 2026-11-12 at about 00:30 to the coordinator's batch (`/decide`), given in the coordinator's
conversation and not yet on GitHub (`gh issue list --label kind:decision --search 2453` prints
nothing; the synthesis and its decision issue quote it). **2a and 3a are this sitting**: (2a) defect
2453's repair goes through the SOUNDNESS lane, compiler-engineer and ffi-pragmatist, on route A;
(3a) a FULL sitting on the server step's two questions, the blind seat the only paid run, **with a
ceiling of 10 USD for the whole sitting, the author's**. The coordinator holds both in one sitting, in
two parts, so the shared brief and the completeness critic are written once. The critic read these
briefs before any seat (`docs/panel/246-reports/critic-briefs.md`, 22 items); this text is the brief
repaired on its first pass, the `.before-critic.md` copies standing beside it.

## The base

**`ef50a7478c241f765c0b308cb55282f95d64053f`**, its tree **`084b36b7d5db6b342c281468fff120b2fe98349b`**
(`git -C .claude/worktrees/b33-round rev-parse ef50a7478 'ef50a7478^{tree}'`), the head of batch
33's round on branch `b33-round`. Since the first base, `main` at `b8c8240d4`, it holds 15 commits
(`git log --oneline b8c8240d4..ef50a7478 | wc -l`): lane `core-platform`'s feature 2295 (the platform
layer, `runtime/hero_net.h`, `runtime/hero_time.h`, `runtime/platform/`, and spec § 13's F3 sentence
in line 377), batch 33's defects 2485, 2486, 2487, 2488 and 2477, and panel 245's ratification. A
gate agent owns `.claude/worktrees/b33-round`: **read it only through git, never build, write or run
inside it.** Every seat copies the commit and builds its own compiler from its seed in two stages:

```
mkdir -p .claude/worktrees/p246-scratch/246-<seat>/tree .claude/worktrees/p246-scratch/246-<seat>/tmp
git -C .claude/worktrees/b33-round archive ef50a7478 | tar -x -C .claude/worktrees/p246-scratch/246-<seat>/tree
cd .claude/worktrees/p246-scratch/246-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 796d1133542b1909
```

Measured by the coordinator at 04:40 to 04:42 in `.claude/worktrees/p246-scratch/base-ef50/tree`,
made that way, under `/usr/bin/time -p` on a machine other sessions were loading (`uptime`: 10.70,
4.21, 2.68 at the start): the seed `real 10.71` (`user 10.33`), stage 2 `real 127.85` (`user 94.15`,
`sys 22.17`), `heroes-next --version` printing `heroes 0.6.0` and `sources 796d1133542b1909`, and
`heroes-next doctor` naming that tree's own `runtime/`. The copy has no `.git`: a case that needs a
repository makes its own (Part A). A seat whose stage 2 prints another `sources` line is not on the
base and says so first. **No duration a seat writes is a measurement while other sessions run**
(CLAUDE.md § Verification, CL-025): report `real` against `user` plus `sys` and discard a waiting run.

## Where each seat works, and why it is not the session scratchpad

The coordinator's first instruction named `<session scratchpad>/246-<seat>/`, under `/private/tmp`.
**CLAUDE.md § Hard stops says no file is written outside this repository's root** (author,
2026-10-08), and `.claude/rules/verification.md` names its one exception, the hooks' own tests; panel
234 recorded two coordinator files written to the system's temporary folder as breaches (`grep -n
'outside the repository' docs/panel/234-*.md`). Hard stops rank first (CLAUDE.md § Precedence), so
every seat's folder is **`.claude/worktrees/p246-scratch/246-<seat>/`**, ignored by git, as panels
239 and 244 kept their blind folders under `.claude/worktrees/p2NN-scratch/` (`grep -n 'inside the
repository' docs/panel/239-briefs/blind/README.md docs/panel/244-briefs/blind/README.md`).

## The rules every seat obeys

- **Your own folder, and nothing else**: `.claude/worktrees/p246-scratch/246-<seat>/`, the tree in
  `tree/`, every probe beside it, `TMPDIR` set to its `tmp/` before any build or tool, so nothing lands
  under `/tmp`. Never use the trunk's `./heroes`, a compiler of `.claude/worktrees/p246` or of the
  coordinator's `p246-scratch/base-ef50/`, or another seat's folder; never read, build or run inside
  another seat's folder or inside `.claude/worktrees/b33-round`.
- **Your report**: `docs/panel/246-reports/<seat>.md` in `.claude/worktrees/p246`, written as you go,
  in your definition's output structure, one section per part you judge. Write nothing else in
  `.claude/worktrees/p246`.
- **Every number with the command that produced it, every negative sentence run or posed as a
  question**, the words *only*, *never*, *provably* with what was searched and where (`/panel` step 2;
  CL-077). Where this brief is wrong, say so first.
- **No paid run your brief does not name**: no `claude -p`, no API call, no `heroes measure
  --refresh`, no cloud run. One you find worth running goes in your report with its size and cost.
- **Never print a credential**: `gh auth status` only, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag.
- **A program that may not end** (a server loop, a stop's probe) is built with `heroes build` and its
  BINARY run under `timeout 30`, its output to `/dev/null` or through `head -c 65536` into a file;
  after a timeout `pgrep -f <your folder>` is read and anything left stopped before a verdict
  (`.claude/rules/verification.md` § A run that may not end).
- **One heavy command at a time** (a stage 2 build, a suite); other seats and lanes share this
  machine. Whole suites only where your route changes what the checker refuses, one at a time, named
  in the report with why. A git repository a case needs is made with `git init -q` inside your folder;
  `git add -A` and its kin are refused there too by the hooks (CLAUDE.md § Hard stops).

## Part A: defect 2453, the soundness lane (compiler-engineer, ffi-pragmatist)

**Defect 2453 is `blocking` since 2026-11-12** (`gh issue view 2453 --repo heroes-lang/heroes --json
labels`: `kind:defect,class:blocking,area:fetch` when read at 04:45), reclassed by the coordinator
from `improvement` on the critic's measurement, re-run by the coordinator at the base (below): a later
run handed a dead run's process number removes that run's `replaced-N`, the one copy of the author's
edit, **silently at exit 0**. A silent loss of the author's work is robustness, design.md §1.12 and
CLAUDE.md § Precedence rank 3, and a milestone is tagged only over zero open `blocking` defects
(CLAUDE.md § Verification), so **M-core-packages' tag waits on this repair**.

**The routes, A as lanes b31-pkg and b32-pkg wrote it** (the issue's two comments):

- **A.** Each run of `heroes add` or `heroes fetch` takes an exclusive lock the operating system
  releases when the process dies (`flock` on POSIX, `LockFileEx` on Windows) on a file inside its own
  `heroes.packages/.fetching-<pid>-<n>/`, holds it until the run ends, and writes its ledger there
  before each rename; a later run recovers every `.fetching-*` folder whose lock it can take (keeps a
  placed tree the catalogue names, removes any other tree the dead run placed, puts back what it moved
  aside, removes the folder) and leaves one whose lock it cannot take. Defect 2434's *leaves the
  program's folder as it found it* (`selfhost/fetch/staging.hero:98-108`), carried to a kill.
- **B.** Report the leftovers and remove nothing. **C.** Pin them as a known cost.
- **D** (the critic's, a route nobody listed): `staging.opened` never removes a standing
  `.fetching-<pid>-<n>`; it refuses it or takes the next serial, as `heroes new` already rules for its
  own staging name (`selfhost/cli/stub.hero:175-176`: *"A staged name that stands already, a run of
  this process number killed before it ended, is refused and never removed: it is not this run's."*,
  the check at 180-184).
- **B and C are false of the code as long as `staging.hero:48` stands**: `opened` removes a folder of
  its own name before it makes it (`written(process.remove_tree(folder))?`), so nothing is left alone
  and no pin holds. Every route carries D or says why it does not need it.

**The door is compiler-private** (the coordinator's ruling on the critic's item 7). Part A sits in
the soundness lane by the author's `2a`, a lane for *"no surface, no diagnostic and no spec token"*
(`.claude/skills/panel/SKILL.md:18-24`). A door in `runtime/hero_os.h` is a surface: spec § 13 names
that header, decision 2228 lets every program bind it (`grep -n 'Amended 2026-10-27' docs/design.md`,
3748), and panel 232 promises *"the functions `hero_os.h` declares at 1.0.0"*
(`docs/panel/232-*.md:272-274`). So the lock door lives in **`runtime/hero_compiler.h`** (80 lines,
`wc -l`), *"the doors the compiler binds for itself, which no program needs"* (its line 7-8), bound
as `selfhost/module/reading.hero` and `fingerprint.hero` bind it. **A route that needs a door in
`hero_os.h` is outside this lane**: a seat that finds one names it as such, and the synthesis returns
it to the author. What a door owes either way, measured: `runtime/heroes_guard_open.h` and
`heroes_guard_close.h` save and restore 241 `hero_` names each (`grep -c 'push_macro("hero_'` and
`'pop_macro("hero_'`), held to `selfhost/emit/guarded_names.hero`'s `NAMES` (*"every word
`heroes_runtime.h` and `hero_os.h` write"*, its lines 8-17, its tests red on a word they miss); none of
`hero_compiler.h`'s names is among them (`grep -c 'hero_env_put\|hero_file_key\|hero_text_key\|hero_file_sha256'
runtime/heroes_guard_open.h`: 0). So a `hero_os.h` door owes three files and panel 232's promise; a
`hero_compiler.h` door owes none of them. Either way it owes `tests/harness/suite_runtime.hero`'s
**Rule 3** (its line 148; `SHARED_BY_DECISION` at 211): an object a runtime part keeps between calls,
a held lock descriptor among them, is `_Thread_local`, `_Atomic` or an allow-listed line with its
reason.

**Questions** (each seat's brief says what to build for each):

- **A1, a lock private to one run, against three sentences of panel 235.** R7 (`sed -n 635,652p
  docs/panel/235-*.md`, line 643): *"no user cache folder is read and no lock between two programs is
  needed (the compiler-engineer, § 7)"*. R12 (line 723): *"A fifth input class, a lock, a workspace or
  a configuration beside the catalogue, stays refused on panel 056's three conditions."* R19 (line
  788): *"the placement folder; the verbs, their flags and the catalogue each writes, byte for byte;
  the error codes and texts; and the exit codes"*, fixed as goldens. `.claude/rules/cli-surface.md:54-57`
  counts the input classes as what *the tool* reads (a `.hero` file, argv, the machine's environment,
  the catalogue) and `:76-78` names *"a lock"* as a fifth. Whether a lock file inside a folder no `use`
  can spell, made and released by one run, is that class or another thing; and whether a line
  recovery prints is R19's fixed interface (`tests/golden/fetch/` holds 47 cases, 5 of defect 2434's:
  `ls tests/golden/fetch | wc -l`, `| grep -c 2434`), a new message this lane cannot add.
- **A2, every writer of `.fetching-*`.** Not only `add` and `fetch`: `selfhost/module/packages.hero:121`
  calls `unplaced.refusal`, which calls `staging.begun` (`unplaced.hero:46`), which calls `opened`, on
  the path of every verb that resolves a `use <name>/...`: `check`, `build`, `run`, `test` and
  `mutate`. Whether recovery, the lock, D, or none is owed on that path; what recovery does with a
  folder that has no lock file (taken, it removes a LIVE `check`'s folder while its git writes into
  it; left, a killed `check`'s stays for ever); and whether the on-disk ledger carries `found`
  (`staging.hero:117-119`), without which recovery cannot tell the author's empty
  `heroes.packages/` from one a dead run made.
- **A3, the shapes beside it**: a lock on a network or FAT file system; a folder whose owner is alive
  in another session; PID reuse (measured below); two recovering runs at once; a kill between the two
  renames of `staging.placed` (`staging.hero:73-90`); a killed `fetch` that had replaced an edited tree
  with the catalogue's. **A child does not outlive a killed `heroes`**: the runtime ends it
  (`runtime/parts/run.c:506-557`, defects 425 and 437, Linux's `PR_SET_PDEATHSIG`, the Windows job
  object; `:734-783`, defects 465 and 575, Darwin's SENTINEL started by `posix_spawn` at `:900`), and
  the coordinator's run below saw the `git` child gone after the kill. What is left to ask is a child
  between its `fork` and its `execvp` (`run.c:1340`), sharing the lock's open file description, the
  sentinel's own delay, and a program's C that forks without exec.
- **A4, the cases that prove it**: `kill -9` at each phase of `add`, `fetch` and `check`, and what a
  golden can hold of it (the `fetch` form has no kill step: `grep -n -i kill
  tests/harness/suite_fetch.hero` prints nothing; a case can plant a dead run's folder instead, a
  question).
- **Platforms.** Linux arm64 by the coordinator, in `heroes-linux-arm64:latest` (image `338a1044ac90`,
  `docker images`) by `docs/platforms/linux/LINUX-MACHINE.md` § How to run something, which copies the
  tree without `.git`: **every case makes its own git repository around itself (`git init -q`)**,
  since a `file://` index is read only from inside the program's own repository (panel 235's R9 C4),
  and **no case needs `curl`**: `docker run --rm heroes-linux-arm64:latest sh -c 'command -v git; git
  --version; command -v curl'` printed `/usr/bin/git`, `git version 2.47.3` and no curl. **Windows** is
  compiled on this Mac and not run: `zig cc -target x86_64-windows-gnu` (`which zig`:
  `/Users/joseph/.zvm/bin/zig`, `zig version` 0.15.2) compiled the coordinator's
  `p246-scratch/a-win/lock.c` (`CreateFileW`, `LockFileEx`, `SetConsoleCtrlHandler`,
  `InterlockedExchange`) at exit 0, `file`: `PE32+ executable (console) x86-64, for MS Windows`, with
  `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR` inside the folder (`find ~/.cache/zig -newer lock.c
  | wc -l`: 0). The Windows box is off since about 04:30 (the coordinator), so a Windows run is the
  CI's leg after the push.

## Part B: the server step, the full panel (all five seats)

**The proposal, as spec diffs and candidate spellings.** Every price is the vendored maximum of
`heroes-next measure`, run from the base's root on a copy of the base's spec with the sentence in
place (`p246-scratch/price2/`, made by `p246-scratch/tools/drafts2.py`); the real delta needs
`--refresh`, a paid call nobody is named for.

- **B1** (feature 1073). Spec § 13 opens, at the base, *"Anything beyond this document comes from
  packages or C libraries; threads, processes and files from the language's `hero_os.h`, and sockets
  and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names
  one."* (`spec/heroes-spec.md:377`, the one line `grep -n -i thread` finds; F3 as feature 2295 landed
  it, `docs/measurements/010-spec-budget-ledger.md` row 8417: +31 vendored and +41 real, 10,973 to
  11,014). Append after it:
  - **T1** (+73): *"A thread runs a function of one `i64` and gives back one, and nothing else crosses:
    `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` starts it and
    `hero_thread_join(handle: i64) -> i64` waits for its answer."*
  - **T2** (+15): *"Into a thread and back crosses one `i64` and nothing else."*

  **Two measurements, in their order, and no ruling between them found.** Panel 113's R5, 2026-09-06
  (`grep -n 'the scalars and' docs/panel/113-*.md`: 276; ratified *"in full"* at 410): the body
  signature *"admits only what is measured safe to cross. Today that is the scalars and **`str`**"*,
  on 250,000 concurrent operations on one `HeroStr`, 0 races. Journal 034 (`sed -n 79p
  docs/records/journal/034-isolated-threads.md`): *"Only an `i64` crosses a thread boundary"*, with no
  reason given for leaving `str` and the other scalars out. The critic searched journal 034, panel 113
  and design.md Part 7 item 13 for a ruling that narrowed R5 and found none: where `i64`-only was
  decided, a ruling or a gap between R5 and the code, is a question, and so is whether R5's *scalars*
  include `f64` and `bool`. Panel 111's *"9 reported `heap-use-after-free`"* for a shared `str`
  (`docs/panel/111-*.md:129`), which feature 1073 cites against widening, predates *"The refcount
  half is closed at M-isolated-threads step 3"* (design.md:4082). **`runtime/hero_os.h:430-434` is
  false today**: *"WHAT MAY CROSS IS DECIDED BY THE CHECKER ... a `[T]` or a `{K: V}` in that signature
  is `error[ffi_type]`"*, while a `str` body and a handle body pass `check` and are refused by clang at
  `build` (below). Panel 232's promise lets a 1.x add to `hero_os.h` and never narrow it, so the rung
  the spec writes now is the one promised at 1.0.0.
- **B2** (feature 1069). A way for a server to stop cleanly, never a mutable global (design.md Part 6,
  `grep -n 'Global mutable state' docs/design.md`: 3572). The candidates, each to be built or priced
  against the others:
  - **S1** (+70, § 13): *"`hero_stop_catch()` makes Ctrl-C and a request to end (SIGINT, SIGTERM) set a
    flag instead of ending the program, and `hero_stop_asked() -> i64` answers 1 once one has come: a
    loop asks between its steps, finishes what it holds and returns from `main`."*
  - **S2** (+65, § 13): *"From the first call of `hero_stop_asked() -> i64`, Ctrl-C and a request to end
    (SIGINT, SIGTERM) no longer end the program; it answers 1 once one has come, and a loop that asks
    between its steps finishes what it holds and returns from `main`."*
  - **S3** (+58, § 11, a built-in after `exit`, the table's ` · ` before it): *"`stop_asked() -> bool`
    (false until Ctrl-C or a request to end, SIGINT or SIGTERM, arrives; from its first call such a
    request no longer ends the program, so a loop that asks between its steps finishes and returns from
    `main`)."*
  - **(a)** the ending signals blocked with `pthread_sigmask` and asked with `sigpending` between
    steps: no handler, no Heroes code in one, no `SA_RESTART` question; **(b)** the stop as a
    descriptor (a self-pipe's read end, kqueue's `EVFILT_SIGNAL`, Linux's `signalfd`) that one `poll`
    waits on beside the listening socket, so a blocked `accept` wakes; **(c)** the stop inside the
    platform layer's own blocking calls, since panel 234's R3 makes the layer own `accept`. All three
    unrun.
  - **The program's own header**, feature 1069's re-check of 2026-10-25: a three-line `stopflag.h`
    (`static volatile sig_atomic_t`, two `static inline` functions) a program ships, open to a program
    by panel 114's **R2**, ratified 2026-09-06, whose spec sentence *"The header may be one the program
    ships beside its own file, so a difference between machines is written in C, where `#ifdef`
    already exists."* (`docs/panel/114-*.md:183-184`) **never entered the spec** (`grep -n -i ships
    spec/heroes-spec.md` prints nothing; the critic's `git log -S` over the spec found none), and
    closed to a **package** by panel 234's R6 (*"no package ships platform C of its own"*, `grep -n
    'ships platform C' docs/panel/234-*.md`: 421). Priced: R2's sentence appended after line 377,
    +31. **Its trap, measured at the base** (`p246-scratch/a-probes/stop1`, `stop2`, the critic's
    sources): handler and loop in one module printed `drained after 3 requests`, exit 0; the handler in
    a second module, each binding `stopflag.h`, printed **`drained after 1000 requests`** (the probe's
    cap), exit 0: the handler set its own unit's flag and the loop read the other's, so without the cap
    Ctrl-C is swallowed and the compiler says nothing.
  - T1 with S1 together: +143; T1 with S3: +131.

  **What every stop meets, from the tree:**
  - **The layer's `accept` asks again after a signal**: `runtime/platform/hero_net.c:509-512`, `do {
    got = accept(fd, NULL, NULL); ... } while (got == HERO_NET_NO_FD && hero_net_interrupted(code));`,
    `EINTR` on POSIX and `WSAEINTR` on Windows (`:146-152`). So a server blocked in `hero_net_accept`
    never returns to a loop that asks *between connections*, `SA_RESTART` or not, until a connection
    comes (read, unrun). `hero_net_close` *"A call in flight on it on another thread ends first"*
    (`hero_net.h:93`), by `shutdown` (`hero_net.c:240-247`): whether that wakes a blocked `accept` on
    a listening socket on each platform is a question.
  - **Which thread runs a handler**, on both platforms: a POSIX process-directed signal goes to a thread
    not blocking it, which may be a thread C made, where `runtime/parts/thread.c`'s guard stops a
    Heroes handler (panel 114's R4: *"panic: main.worker ran on a thread this program did not start"*,
    exit 134); and a Windows console control handler's thread (Microsoft's page, to read). Both unrun.
  - **Four signals or two**: `runtime/parts/run.c:518-525` catches SIGTERM, SIGINT, SIGHUP and SIGQUIT
    while a child is waited on, *"where this process had left them at their default"*; S1 to S3 take
    two. Why two, and what a stop armed before a child is run does to that rule.
  - **Task 1072's leak check runs only for a stop that returns from `main`**: `hero_exit` is *"Not the
    leak gate"* (`runtime/parts/os.c:1005-1006`), and the emitted `main` calls
    `hero_runtime_check_leaks()` before `return 0` (`selfhost/emit/c_main.hero:83`, `:102`). A handler
    that calls `exit`, as the coordinator's `h.hero` does, skips the check the long run is for.
  - **Rule 3** of `tests/harness/suite_runtime.hero` judges the flag (an object a runtime part keeps);
    its precedent is `runtime/parts/panic.c:32`'s `static volatile sig_atomic_t hero_runtime_spoke`,
    allow-listed with its reason at `suite_runtime.hero:233`.
- **B3** (feature 925, with feature 893 and task 1072). **A connection is a handle, not an `i64`**:
  panel 234's R3, ratified 2026-11-06 (`sed -n 321,324p docs/panel/234-*.md`): *"a descriptor (a POSIX
  `int`, a winsock `SOCKET`) is wrapped in C as a struct behind a pointer so it carries the marks"*;
  at the base `runtime/hero_net.h:45-46` hands out `hero_net_socket *`, *"a group declares it as a
  handle, `record Socket tag hero_net_socket`"*, and promises at 33-36 *"never a read of freed memory
  and never another socket that took its number"*. So feature 1073's *"One connection per thread needs
  only the descriptor, which is an `i64`"* and feature 893's *"A descriptor is a number, so it crosses
  as the `i64`"* rest on a premise R3 overturned. Whether T1's rung can serve one connection per thread
  at all, and the three ways out, each priced: **a numbered handle** (the layer gives a socket a
  number and back, which R3's *never another socket that took its number* exists to refuse), **a rung
  that moves a handle** with its marks across the boundary, or **one thread**
  (`docs/roadmap/milestones/M-core-packages.md:35`, *"writable, one thread"*). And copy-on-write's
  `refcount == 1` test-then-mutate (`grep -n 'refcount == 1' runtime/parts/*.c`: `cow.c:44`, `:88`,
  `:93`, `map-write.c:128`): whether anything a program can make cross reaches it under the rung,
  including the shapes panel 113 did **not** try (feature 925: *"`sort` through a function pointer,
  the `eq` and `hash` descriptor walks, and the drop-list drain under contention"*) and panel 111's
  four classes (`docs/panel/111-*.md:125-170`: a shared `str`'s count, `==` on nested arrays,
  copy-on-write's test-then-mutate, the stack guard on a worker).

These spellings are the coordinator's drafts, written to be priced and tested, never a ruling; the
names `hero_stop_catch`, `hero_stop_asked` and `stop_asked` are candidates.

## What the coordinator measured at the base (each with its command)

- **The spec**: 492 lines, 28,544 bytes (`wc -l`, `wc -c`), sha256 `bcb60736643d7772...`
  (`shasum -a 256`, equal to `git -C .claude/worktrees/b33-round show ef50a7478:spec/heroes-spec.md |
  shasum -a 256`); `heroes-next measure spec/heroes-spec.md` from the base's root: claude-legacy 8267,
  cl100k_base 8417, maximum 8417, **real 11014** (*"claude-opus-5, 2026-11-12"*, the instrument's pin,
  no `STALE`), headroom 5370 against 16384, 11074 judged with the FFI floor's 60. The ceiling:
  design.md:255 and `tests/harness/suite_spec.hero:162-163`.
- **What crosses**, with the base's `heroes-next` in `p246-scratch/a-probes/`: a `hero_thread_spawn`
  bound with a body `(function(x: str) -> i64)` and `arg: str` (`s.hero`): `check` exit 0, `build` exit
  1, `error[ffi_callback_type]` (*"clang read the header, and it says `long long (*)(long long)`"*);
  with `(function(x: [i64]) -> i64)` (`a.hero`): `check` exit 1, `error[ffi_type]`; with
  `(function(s: Socket) -> i64)` and `record Socket tag hero_net_socket` from the base's own
  `hero_net.h` (`handle.hero`): `check` exit 0, `build` exit 1, the same `ffi_callback_type`. The
  checker's rule is `selfhost/check/ffi.hero:79-94` (`.str` and `.ptr` answer yes).
- **A constant on two threads**: `constant WORDS: [str]` read, copied and written by two spawned bodies
  (`const.hero`) built at exit 0 and printed `30`; its `--emit-c` holds one
  `HERO_ARRAY_STATIC(hero_constant_h_const_WORDS...)`, a static whose count is below zero
  (`runtime/heroes_runtime.h:196-197`). No sanitizer run.
- **A mutable global and a handler**: `stopped: bool @= false` at the top of a module (`g.hero`) is
  `error[expected_declaration]`, exit 1; a Heroes function given to libc's `signal` for SIGINT that
  prints and calls `exit` (`h.hero`) built at exit 0 and printed `caught 2`, exit 0.
- **PID reuse, Part A** (`p246-scratch/a-pid/`, the critic's index copied into a repository of its own,
  `git init -q`): `fetch` placed `small` and `large`; the author's edit appended to one file of each;
  a second `fetch` killed with `kill -9` once `.fetching-*/replaced-0` stood (`wait` 137), leaving
  `.fetching-67003-1/` with `git-root.txt`, `git-said.txt`, an empty `tree/` and `replaced-0/a.hero`,
  the only copy of `small`'s edit. **Control**: a later `fetch` under another number, exit 0, the dead
  folder and the edit stand. **The dead number given to the later run** (`p246-scratch/a-pid/reuse.sh`
  renames the folder to the shell's own `$$` and `exec`s the compiler, which keeps that number):
  `fetch` printed `kept small`, `placed large`, exit 0, and afterwards **0 `.fetching-*` folders and
  0 files holding the edit** (`grep -rl`). The same with `check main.hero` (`use <small>/a`, `small`'s
  placed tree moved away): `error[package_unread]`, exit 1, the dead folder gone and the files holding
  an edit 2 before, 1 after (`large`'s edit stood in place; `small`'s went with the folder).
- **A killed `check`** (`git` slowed by `p246-scratch/a-pid/slowgit/git` first on `PATH`, the program
  holding no `heroes.packages/`, `ls -a` before: `heroes.toml`, `main.hero`): `kill -9` while the
  wrapper slept left `heroes.packages/.fetching-74654-1/` with `git-root.txt`, `git-said.txt` and an
  empty `tree/`, and `heroes.packages/` itself; the wrapper (74657) was gone a second after (`ps -p`).
- **Ten programs of `examples/` bind `hero_thread_spawn`** (`grep -rl hero_thread_spawn examples
  --include='*.hero' | wc -l`); the lock search (`grep -rln -E 'flock|LockFileEx|F_SETLK|F_OFD_SETLK|
  lockf\(|LOCK_EX|OpenProcess' runtime selfhost tests/harness`) finds 0 files at the base.
- **Every Part B price**, legacy and cl100k_base per draft, is in `spec-warden.md`.

## Questions no one has run, posed as questions

- Whether `shutdown` from `hero_net_close` wakes a thread blocked in `accept` on a listening socket on
  Darwin, Linux and Windows.
- On which thread a POSIX process-directed SIGINT runs its handler while a thread C made is alive, and
  on which thread Windows runs a console control handler.
- How soon a process number recurs on this Mac (the critic read 11,628 numbers in 284 s under that
  evening's load; the cycle's length is not read here).
- Whether `claude -p` can pass its `--max-budget-usd` cap by one request (the blind seat's bound does
  not rest on it; `llm-ergonomist.md`).
