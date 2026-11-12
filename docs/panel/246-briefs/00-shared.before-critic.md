# Panel 246, the brief every seat reads first

Convened 2026-11-12 by the coordinator, in M-core-packages (the GitHub milestone of defect 2453 and of
features 893, 925, 1069, 1072, 1073 and 2295, `gh issue view <n> --repo heroes-lang/heroes`), on the
author's answer `2a 3a` of 2026-11-12 at about 00:30 to the coordinator's `/decide` batch: **(2a)**
defect 2453's repair goes through the SOUNDNESS lane, compiler-engineer and ffi-pragmatist, on route A;
**(3a)** a FULL sitting on the server step's two questions, the blind seat the only paid run, **with a
ceiling of 10 USD for the whole sitting**. The coordinator holds both in one sitting, in two parts,
so the shared brief and the completeness critic are written once. The critic reads these briefs
before any seat starts and the reports after.

## The base

`main` at `b8c8240d43ae9fc3cb336816d70f42cf5cf66628` (`git rev-parse HEAD` and `git rev-parse
origin/main` in `.claude/worktrees/p246`, 2026-11-12 at 00:48; `git status --short` was empty before
these briefs were written). Every seat copies that
commit and builds its own compiler from its seed in two stages:

```
git -C /Users/joseph/Temp/heroes/heroes-lang worktree add --detach \
    .claude/worktrees/p246-scratch/246-<seat>/tree b8c8240d4     # the coordinator, before launch
cd .claude/worktrees/p246-scratch/246-<seat>/tree
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 017930ae6061c8b4
```

Measured by the coordinator while writing, in `.claude/worktrees/p246`, under `/usr/bin/time -p` on
a machine other sessions were loading (`uptime`: load averages 4.33, 9.78, 8.40): the seed `real
10.60` (`user 10.26`), stage 2 `real 124.36` (`user 93.48`, `sys 21.71`), and `heroes-next
--version` printed `heroes 0.6.0` and `sources 017930ae6061c8b4`. A seat whose stage 2 prints
another `sources` line is not on the base; it says so before it measures anything. **No duration a
seat writes is a measurement while other sessions run** (CLAUDE.md § Verification, CL-025): a seat
that times something reports `real` against `user` plus `sys` and discards a waiting run.

## Where each seat works, and why it is not the session scratchpad

The coordinator's instruction named `<session scratchpad>/246-<seat>/`, under `/private/tmp`.
**CLAUDE.md § Hard stops says no file is written outside this repository's root** (author,
2026-10-08), and `.claude/rules/verification.md` names its one exception, the hooks' own tests; panel
234 recorded two coordinator files written to the system's temporary folder as breaches
(`grep -n 'outside the repository' docs/panel/234-*.md`). Hard stops rank first (CLAUDE.md
§ Precedence), so this brief moves every seat's folder to the same name inside the root,
**`.claude/worktrees/p246-scratch/246-<seat>/`**, ignored by git, as panels 239 and 244 kept their
blind folders under `.claude/worktrees/p2NN-scratch/` (`grep -n 'inside the repository' -r
docs/panel/239-briefs/blind/README.md docs/panel/244-briefs/blind/README.md`). The coordinator may
overrule this with the author's word; the brief does not assume it.

## The rules every seat obeys

- **Your own folder, and nothing else**: `.claude/worktrees/p246-scratch/246-<seat>/`, the tree under
  `tree/` and every probe beside it. Build your compiler there from the seed; never use the trunk's
  `./heroes`, `.claude/worktrees/p246`'s `heroes-next` (the coordinator's, for these briefs) or another
  seat's folder; never read, build or run inside another seat's folder. Set
  `TMPDIR=<your folder>/tmp` before any build, so nothing a tool writes lands under `/tmp`.
- **Your report**: `docs/panel/246-reports/<seat>.md` in `.claude/worktrees/p246`
  (`mkdir -p` it), written as you go and not only at the end, in your definition's output structure,
  one section per part you judge. Write nothing else in `.claude/worktrees/p246`.
- **Every number with the command that produced it, every negative sentence run or posed as a
  question**, the words *only*, *never*, *provably* with what was searched and where (`/panel` step 2;
  CL-077). Where this brief is wrong, say so first: a seat that finds its brief false is the
  measurement the critic needs.
- **No paid run your brief does not name**: no `claude -p`, no API call, no `heroes measure
  --refresh`, no cloud run. One you find worth running goes in your report with its size and cost.
- **Never print a credential**: `gh auth status` only, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag.
- **A program that may not end** (a server loop, a probe of a stop) is built with `heroes build` and
  its BINARY run under `timeout 30`, its output to `/dev/null` or through `head -c 65536` into a file,
  and after a timeout `pgrep -f <your folder>` is read and anything left is stopped before a verdict
  is written (`.claude/rules/verification.md` § A run that may not end).
- **One heavy command at a time** (a stage 2 build, a suite); other seats and two lanes share this
  machine. Whole suites only where your route changes what the checker refuses, one at a time, each
  named in the report with why.

## Part A: defect 2453, the soundness lane (compiler-engineer, ffi-pragmatist)

**The proposal, route A as lanes b31-pkg and b32-pkg wrote it** (the issue's two comments):

1. Each run of `heroes add` or `heroes fetch` takes an exclusive lock the operating system releases
   when the process dies (`flock` on POSIX, `LockFileEx` on Windows), on a file inside its own
   `heroes.packages/.fetching-<pid>-<n>/` folder, and holds it until the run ends.
2. Before each rename it writes its ledger into that folder: what it moves aside, what it places.
3. A later `add` or `fetch` first recovers every `.fetching-*` folder whose lock it can take: keeps a
   placed tree the catalogue names, removes any other tree the dead run placed, puts back what the dead
   run moved aside, then removes the folder; a folder whose lock it cannot take is a live run's, left.
4. Defect 2434's *leaves the program's folder as it found it* (`selfhost/fetch/staging.hero:98-108`),
   carried from a refusal to a kill.

**The alternatives**: (B) report the leftovers and remove nothing; (C) pin them as a known cost.

**Questions** (each seat's brief says what to build for each):
- **A1, the fifth input class.** Panel 235 refused *a lock* as a fifth input class, and it is **R12,
  not R19**, that says so (`grep -n 'fifth' docs/panel/235-*.md`, line 723): *"A fifth input class, a
  lock, a workspace or a configuration beside the catalogue, stays refused on panel 056's three
  conditions."* R19 (line 788) fixes *"the placement folder; the verbs, their flags and the catalogue
  each writes, byte for byte; the error codes and texts; and the exit codes"* as goldens.
  `.claude/rules/cli-surface.md:76-78` reads *"a second per-project file beside the catalogue, a lock,
  a workspace or a configuration, is a fifth class"*, and its lines 54-57 count the input classes as
  inputs *the tool* reads: a `.hero` file, argv, the machine's environment, the catalogue. Whether a
  lock file private to one run, inside a folder no `use` can spell, read by no `build`, `check` or
  `run`, is that class or another thing is the question; and whether a line recovery prints is R19's
  fixed interface (`tests/golden/fetch/` holds 47 cases, 5 of them defect 2434's, `ls
  tests/golden/fetch | wc -l` and `| grep -c 2434`), which would be a new message the soundness lane
  cannot add (the `/panel` skill's lane rule: no surface, no diagnostic, no spec token).
- **A2, the door's shape on three platforms.** `grep -rln -E 'flock|LockFileEx|F_SETLK|F_OFD_SETLK|
  lockf\(|LOCK_EX|OpenProcess' runtime selfhost tests/harness` finds **0 files**. The `hero_fs_*`
  doors are declared in `runtime/hero_os.h` (lines 193 to 300) and bound by
  `selfhost/cli/process.hero:54-62`; **they are not compiler-private**: decision 2228 lets a program
  bind `hero_os.h` (design.md, `grep -n 'Amended 2026-10-27' docs/design.md`), and panel 232's
  paragraph promises *"the functions `hero_os.h` declares at 1.0.0"*
  (`docs/panel/232-*.md:272-274`). The compiler's own header is `runtime/hero_compiler.h` (80 lines,
  `wc -l`): *"the doors the compiler binds for itself, which no program needs"*. Which header, which
  part of `runtime/parts/`, which signature, what it answers on a file system that has no lock.
- **A3, the shapes beside it**: a lock on a network or FAT file system; a staging folder whose owner
  is alive in another session; PID reuse; a child (`git`, `curl`) that inherits the lock and outlives
  its killed parent; two recovering runs at once; a kill between the two renames of
  `staging.placed` (`staging.hero:73-90`); a killed `fetch` that had replaced an author's edited tree
  with the catalogue's (the second comment: the edit then stands only in `.fetching-<pid>-1/replaced-0`).
- **A4, the cases that prove it**: `kill -9` at each phase of `add` and `fetch`, and what a golden can
  hold of it (the `fetch` form has no kill step today: `grep -n -i kill tests/harness/suite_fetch.hero`
  prints nothing; a case can plant a dead run's folder instead, a question).

## Part B: the server step, the full panel (all five seats)

**The proposal, as spec diffs and candidate spellings** (every price below is the vendored maximum of
`heroes measure` run from the worktree's root on a copy of the spec with the sentence in place; the
real delta needs `--refresh`, a paid call nobody is named for):

- **B1** (feature 1073): spec § 13 opens *"Anything beyond this document comes from packages or C
  libraries; threads, processes and files from the language's `hero_os.h`."*
  (`spec/heroes-spec.md:377`, the one line `grep -n -i thread spec/heroes-spec.md` finds). Append:
  - **T1** (+73): *"A thread runs a function of one `i64` and gives back one, and nothing else crosses:
    `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` starts it and
    `hero_thread_join(handle: i64) -> i64` waits for its answer."*
  - **T2** (+15): *"Into a thread and back crosses one `i64` and nothing else."*

  Beside them, a wider rung already ruled once: panel 113's R5 said the body signature *"admits only
  what is measured safe to cross. Today that is the scalars and **`str`**"*, on 250,000 concurrent
  operations on one `HeroStr` with 0 races (`grep -n 'the scalars and' docs/panel/113-*.md`: line
  276), and the runtime shipped `int64_t` alone (`runtime/hero_os.h:435`). Panel 232's promise lets a
  1.x add to `hero_os.h` and never narrow or change it, so a rung written now in the spec is the one
  promised at 1.0.0.
- **B2** (feature 1069): a runtime part owns a stop flag that SIGINT and SIGTERM set (a console control
  handler on Windows), and answers a Heroes call the server's loop asks between connections; never a
  mutable global (design.md Part 6, `grep -n 'Global mutable state' docs/design.md`: line 3572).
  Candidate spellings, against any other route a seat finds:
  - **S1** (+70, § 13): *"`hero_stop_catch()` makes Ctrl-C and a request to end (SIGINT, SIGTERM) set a
    flag instead of ending the program, and `hero_stop_asked() -> i64` answers 1 once one has come: a
    loop asks between its steps, finishes what it holds and returns from `main`."*
  - **S2** (+65, § 13): *"From the first call of `hero_stop_asked() -> i64`, Ctrl-C and a request to end
    (SIGINT, SIGTERM) no longer end the program; it answers 1 once one has come, and a loop that asks
    between its steps finishes what it holds and returns from `main`."*
  - **S3** (+58, § 11, a built-in after `exit`): *"`stop_asked() -> bool` (false until Ctrl-C or a
    request to end, SIGINT or SIGTERM, arrives; from its first call such a request no longer ends the
    program, so a loop that asks between its steps finishes and returns from `main`)."*
  - T1 with S1 together: +143; T1 with S3: +131.
- **B3** (feature 925, with 893 and 1072): whether anything a program can make cross into a thread
  reaches copy-on-write's `refcount == 1` test-then-mutate under the rung
  (`grep -n 'refcount == 1' runtime/parts/*.c`: `cow.c:44`, `:88`, `:93`, `map-write.c:128`), and what
  the server step needs (one connection per thread, issue 893; *"writable, one thread"* in
  `docs/roadmap/milestones/M-core-packages.md:35`; the long run of 1072).

These spellings are the coordinator's drafts, written to be priced and tested, never a ruling; the
names `hero_stop_catch`, `hero_stop_asked` and `stop_asked` are candidates.

## What the coordinator measured while writing (each with its command)

- **The spec**: 492 lines, 28,431 bytes (`wc -l`, `wc -c`), sha256 `fa765470cf65...c0e`
  (`shasum -a 256`); `./heroes-next measure spec/heroes-spec.md`: claude-legacy 8236, cl100k_base 8386,
  maximum 8386, **real 10973** (*"claude-opus-5, 2026-11-11"*, *"the binding number"*, the instrument's
  pin for these bytes, no `STALE` printed), headroom 5411 against 16384, the FFI floor's 60 making
  11033. The ceiling: design.md:255 and `tests/harness/suite_spec.hero:162-163`.
- **What crosses today**, run with `heroes-next` in `.claude/worktrees/p246-scratch/probe-cross/`: a
  `hero_thread_spawn` bound with a body `(function(x: str) -> i64)` and `arg: str`
  (`p246-scratch/probe-cross/s.hero`) passes `check` at exit 0 and is refused at
  `build`, exit 1, `error[ffi_callback_type]` (*"clang read the header, and it says `long long
  (*)(long long)`"*); with `(function(x: [i64]) -> i64)` it is refused at `check`, exit 1,
  `error[ffi_type]`. The checker's rule is `selfhost/check/ffi.hero:79-94` (`crosses_the_boundary`,
  where `.str` and `.ptr` answer yes); the runtime's is `runtime/hero_os.h:417-447`.
- **A constant read on two threads**: `constant WORDS: [str]` read and copied-then-written by two
  spawned bodies (`p246-scratch/probe-const/main.hero`) built at exit 0 and printed `30`; its
  `--emit-c` holds `HERO_ARRAY_STATIC(hero_constant_h_main_WORDS_4, ...)` returned by
  `h_main_WORDS()`. Unrun: any sanitizer.
- **A mutable global and a signal handler**: `stopped: bool @= false` at the top of a module is
  `error[expected_declaration]`, exit 1; a Heroes function handed to libc's `signal` for SIGINT, which
  prints and calls `exit`, built at exit 0 and printed `caught 2` at exit 0 after `kill(getpid(),
  SIGINT)` (`p246-scratch/probe-cross/h.hero`).
- **Ten programs of `examples/` bind `hero_thread_spawn`** (`grep -rl hero_thread_spawn examples
  --include='*.hero'`); `examples/threads/main.hero` opens *"what crosses a thread boundary here is
  one `i64`, in and out, and that is not a limitation somebody forgot to lift. It is the model."*
- **Feature 2295, the platform layer, is in flight** in lane `core-platform`
  (`git worktree list`: `.claude/worktrees/core-platform` at `b8c8240d4`, no commit of its own when
  read). No seat depends on its code; a route that would put a door in the layer says so and is priced
  without it.

## Premises the coordinator could not verify, posed as questions

- Whether recovery after a PID is reused is already unsafe today: `staging.opened` removes a folder
  of its own name before making it (`staging.hero:47-48`), and `hero_run_serial` restarts at 1 in
  every process (`runtime/parts/run.c:1425-1428`), so a later run handed a dead run's pid would remove
  that run's `.fetching-<pid>-1/` and the `replaced-N` in it. **An inference from reading, unrun**; if
  it holds, 2453 (classed `improvement`) has a `blocking` shape beside it.
- Whether a killed `heroes build` also leaves a staging folder: `selfhost/module/unplaced.hero:46`
  calls `staging.begun` on the build's error path for a `file://` source, so route A's recovery in
  `add` and `fetch` alone may not cover every writer of `.fetching-*`. Read, unrun.
- Whether a console control handler on Windows runs on a thread the runtime did not start, in which
  case `runtime/parts/thread.c`'s guard (*"ran on a thread this program did not start"*) would stop a
  Heroes function bound as one. Not read today; the ffi-pragmatist reads Microsoft's page.
- Whether a stop flag that SIGINT sets is ever seen by a loop blocked in `accept` or `read`, which
  restart after a handler installed with `SA_RESTART`. Unrun.
