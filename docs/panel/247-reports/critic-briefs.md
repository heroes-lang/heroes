# Panel 247, the completeness critic's first pass, over the briefs

Written 2026-11-13 from 03:52, as the checks ran. Instructions: `docs/panel/247-briefs/critic-briefs.md`.
Read whole: `00-shared.md`, `compiler-engineer.md`, `ffi-pragmatist.md`, `historian.md`,
`llm-ergonomist.md`, `spec-warden.md` (the plain `.md` files; `cmp` against each `.before-critic.md`
copy is below). No verdict on any route: what is missing, unverifiable or false, each with the command
that showed it.

**My tree and compiler**: `git -C .claude/worktrees/b34-round archive ae5e2cedddee93e0d53715a5b5ae6d7833c569df
| tar -x -C .claude/worktrees/p247-scratch/critic/tree`, `TMPDIR` in `critic/tmp`; the seed by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (exit 0), stage 2 by `./heroes build
selfhost/main.hero -o heroes-next` (exit 0, `/usr/bin/time -l`: 889,993,614,174 instructions retired;
its `real 127.96` against `user 97.99` + `sys 21.55` was taken beside other sessions, so no duration);
`./heroes-next --version`: `heroes 0.6.0`, `sources 7cc70d136b590d46`. On the base.

`uptime` at 03:52: load averages 12.77, 5.44, 3.95.

## Findings as they were made

### 03:55 to 04:10, item 1: whose SIGKILL it was (settled, and it is not a SIGKILL anyone sent)

- **The driver is excluded by its own text.** `r2494/storm.py` kills only in its `except
  subprocess.TimeoutExpired` branch, which prints `NOT ended 30 s after the storm: SIGKILLed`; the
  three `-9` lines of `before-storm.log` print `sent 21720`, `17794` and `19602` against `quiet`'s
  1,040,160 and more over the full 4 s, so each process ended during the storm, before the 30 s wait
  began (`cat r2494/storm.py r2494/before-storm.log r2494/before-quiet.log`).
- **macOS recorded each death.** `ls -lT ~/Library/Logs/DiagnosticReports | grep -E 'storm|quiet'`
  (read only; nothing written there) lists five `storm-2026-11-13-01285[78]*.ips` at 01:28:57 to 58,
  the minute `before-storm.log` was written (01:28:58), and three `quiet-2026-11-13-024314*.ips` at
  02:43:14, the minute of `protoC/quiet.log` (02:43:14). Read with `python3 -I` (each report's JSON:
  `exception`, `termination`, `asi`, the faulting thread's frames):
  - `storm`: **3 reports `EXC_BREAKPOINT`, signal `SIGKILL`**, pids 94041, 94060, 94066, and **2
    `EXC_BREAKPOINT`, `SIGTRAP`**, pids 94063, 94069: the log's three `-9` and two `-5`, exactly.
  - Each SIGKILL report's `asi`: `"BUG IN CLIENT OF LIBPLATFORM: Trying to recursively lock an
    os_unfair_lock", "Abort Cause 259"`; its one thread: `_os_unfair_lock_recursive_abort <
    _os_unfair_lock_lock_slow < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk <
    hero_int_to_str < h_storm_on_signal < h_0cb_storm_on_signal < _sigtramp < localeconv_l <
    __vfprintf < _vsnprintf < __snprintf_chk < hero_int_to_str < h_storm_round < h_storm_main`.
    **`main` was inside the runtime's integer formatting, `hero_int_to_str`, which calls `snprintf`,
    holding the locale's `os_unfair_lock` in `localeconv_l`; the signal landed there, the handler's
    `f"caught {sig} ..."` called `hero_int_to_str` again, and Darwin's libplatform killed the process
    for taking that lock twice on one thread.** Nothing is written to stderr on that path.
  - Each SIGTRAP report faults in `_xzm_xzone_malloc_freelist_outlined < hero_malloc_raw <
    hero_alloc_block < hero_str_alloc < hero_int_to_str < h_storm_on_signal`: the allocator's own
    freelist check, the corruption the brief describes.
  - `quiet` under protoC: **2 `SIGABRT`** (`abort < hero_thread_guard < h_quiet_on_signal`, the
    refusal) **and 1 `SIGKILL`, pid 75776**, the same recursive-lock death, but the second taker is
    **protoC's own refusal**: `... < __snprintf_chk < hero_thread_guard < h_quiet_on_signal <
    h_0cb_quiet_on_signal < _sigtramp < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk <
    hero_int_to_str < h_quiet_round`. So the lane's reading (*"outliving 30 s"*) is false, and
    **route C as prototyped formats its refusal with `snprintf` inside the handler, which is not on
    POSIX's async-signal-safe list and on Darwin takes the lock `main` may hold**: the refusal then
    dies silently instead of speaking.
- **The same death under D, in the lane's own runs of D's case, being committed now.**
  `cat b34-scratch/b34-signals/c2494/runs*.log` (written 03:49 and 03:52, after the briefs):
  `withD` 200 runs, **5** and then **12** `did not end with the stop`, each `exit 137` with stdout
  `sender started: 1` (later `starting a sender aimed at main`) and **empty stderr**, the slowest
  run **0.095 s** and **0.186 s**, so never `timeout -s KILL 60`'s bound; `withD-O2` 50 runs, 1 each
  time; `withD` under `DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib` and `withD-san` 50 runs, 0.
  The reports: `withD` **4 SIGKILL, all `_os_unfair_lock_recursive_abort`** with `hero_int_to_str <
  h_fixedbugs2494..._on_signal` interrupting `hero_int_to_str < h_fixedbugs2494..._round`; `noD`
  (the case without D) 10 of 13 reports the same SIGKILL, 3 the xzone SIGTRAP.
- **Tally of every report of these binaries today** (the loop above, by name and faulting frame):
  `storm` 3 SIGKILL (lock), 2 SIGTRAP (xzone), 8 SIGABRT; `quiet` 1 SIGKILL (lock), 2 SIGABRT;
  `noD` 10 SIGKILL (lock), 3 SIGTRAP (xzone); `withD` 4 SIGKILL (lock), 39 SIGABRT; `withD-O2` 30
  SIGABRT; `withD-san` 25 SIGABRT. **All 18 SIGKILL reports are the one recursive-lock death.** The
  reports are a sample, not a count: 17 SIGKILL runs of `withD` left 4 reports.
- **And a second shape of a refusal's end, from 02:00**: two `case` reports (lane's 2496 work, a
  signal landing on a thread inside `_pthread_exit`'s cleanup) end `__abort < __abort < abort <
  hero_abort < hero_panic < hero_thread_guard < ..._on_signal < _sigtramp < _pthread_tsd_cleanup`,
  `EXC_BREAKPOINT`/`SIGTRAP`, `asi` `"abort() called"`: `abort()` from inside a handler did not end
  by SIGABRT there, and Darwin's fallback trapped instead.

**What this changes in the briefs**: 00-shared.md's *"whose it was is a question"* is answered; its
D bullet's *"What it does not see, by the lane's reading"* is a measurement, not a reading, and the
first thing it does not see is the runtime's own `hero_int_to_str`, not only C's allocator or
stdio; its C bullet's *"formats a refusal with `snprintf`"* is the refusal's own failure mode, 1 of
3 on `quiet`; and **D's golden case is flaky on this Mac** (5 and 12 of 200 runs end SIGKILL with
nothing said, against an `.expected` that names the stop, `expected-2494.new`), which the `run` form
will read as red about once in 17 to 40 runs.

### 04:00 to 04:05, item 2: what the prototypes are (holds)

`git -C .claude/worktrees/b34-round archive 62ce17563 runtime | tar -x -C critic/work/rt62`, then
`diff -rq` against each prototype's `runtime/`: protoC differs in `parts/os.c` and `parts/thread.c`
only, protoD in `parts/alloc.c` only. `diff` counts: protoC `thread.c` 56 lines added, 0 removed;
`os.c` one line, `513a514 > hero_signal_learn();`; protoD `alloc.c` 31 added, 0 removed, eight
`HERO_ALLOC_IN();`/`HERO_ALLOC_OUT();` pairs. As 00-shared.md says. Two things it does not say:
protoC's refusal is `snprintf(said, ...)` then `write(2, ...)`, `signal(SIGABRT, SIG_DFL)`,
`abort()` (`diff -u`), which is item 1's failure; and **D as it is being landed is not protoD**: the
`withD` reports fault in `raise < hero_alloc_reentered < hero_alloc_enter < hero_malloc_raw` where
protoD's do `abort < hero_alloc_reentered < hero_malloc_raw`, its words are
`b34-scratch/b34-signals/expected-2494.new`, and it stands uncommitted (`git -C
.claude/worktrees/b34-signals status --short`: ` M runtime/parts/alloc.c` and the three untracked
`fixedbugs-2494-*` files; `git -C .claude/worktrees/b34-round log --oneline b34-signals -1`:
`ae5e2cedd`). D's words carry a remedy: *"To stop on Ctrl-C or SIGTERM, call `hero_net_catch_stop`
... answer any other signal with a handler written in C in the program's own header, never with a
Heroes function"*. That sentence is false under E, F or the route in item 6 below, so it is an input
the seats must see.

### 04:05 to 04:10, item 3: the prices, the hashes and the twelve folders (hold)

- `./heroes-next measure spec/heroes-spec.md` from my tree's root: claude-legacy 8340, cl100k_base
  8490, maximum 8490, real 11108 (the row dated claude-opus-5, 2026-11-13, *"the binding number"*), headroom 5276
  against 16384, 11168 judged. `sed -n 160,166p tests/harness/suite_spec.hero`: `CEILING` 16384 at
  163. As both briefs say.
- The drafts rebuilt from `spec-warden.md`'s words by my own `critic/scripts/drafts.py` (the one
  occurrence of `` `atexit(f: (function() -> ()))`. ``, asserted, then one space and the sentence):
  `shasum -a 256` first 16: x `41ad6690571bd27b`, hr `51ecfbc67c72bff2`, hrr `819cb24d7e9c0ec0`, ha
  `7e1a25c601977d20`, he `1868dce1e004ad46`; the arm hashes of `llm-ergonomist.md`, all four.
  Measured: hr 8369/8519/8519, hrr 8383/8533/8533, ha 8376/8526/8526, he 8365/8515/8515: +29, +43,
  +36, +25, the table exactly.
- The twelve folders: each holds `brief.md`, `heroes-spec.md`, `task.md` and nothing else (`ls -A`);
  `heroes-spec.md` hashes `1868dce1...` for every `-e-`, `819cb24d...` for every `-r-`,
  `41ad6690...` for every `-x-`; `brief.md` `1d0b6a53cddcb0a0` in all twelve; `task.md`
  `0fd42e1c700f113f` for t1, `71fd0e09c938ef5a` for t2. `critic/scripts/blocks.py` extracts the
  three fenced blocks of `llm-ergonomist.md` and compares bytes: brief, t1 and t2 **equal**.
- `claude --version`: `2.1.311 (Claude Code)`; `claude --help` lists all ten flags (a local print,
  no API call). `--max-budget-usd` *"only works with --print"*, which the command uses; `--bare`
  skips *"auto-memory ... and CLAUDE.md auto-discovery"*.
- **Not addressed, though panel 246 handed it to this sheet**: its synthesis, *"The blind folders'
  names: ... a name saying less than `246-llm-ergonomist/` is a question for the next sheet (the
  critic's item 15)"* (`docs/panel/246-*.md:515-517`), after 17 of its 20 `context` answers named a
  working directory. The 247 folders are `.claude/worktrees/p247-scratch/247-llm-ergonomist/t1-r-1`
  and the like: the seat's role and the arm's letter are in the path every session sees.
- The nearest measured real delta of a § 13 sentence, for the spec-warden's scale: the round
  appended to panel 246's file after the base (`git -C .claude/worktrees/b34-round diff ae5e2cedd
  bdeb17737 -- 'docs/panel/246-*.md'`): *"W3 held, T1 priced +94 real on `heroes measure
  spec/heroes-spec.md --refresh`, 11,014 to 11,108 ... and +73 vendored"*, a marginal 94/73 = 1.288
  beside the brief's whole-spec 1.3084. Scale, not a measurement of these drafts.

### 04:05 to 04:12, item 4: panel 246 and a program's own handler (a ruling stands, worded more widely than the brief says)

Read whole: `docs/panel/246-*.md` (599 lines), `246-reports/critic-briefs.md` and
`critic-reports.md` (their handler items), and the sitting's decision issue (`gh issue view 2497
--repo heroes-lang/heroes`, closed, `kind:decision,area:runtime`).

- `246-*.md:318`, inside R4: *"**No Heroes code runs in a handler**: Windows runs a console control
  handler on a thread it creates for it ..., and a Heroes handler that allocates kills its program
  (the critic's item 12)."*
- `246-*.md:304-308`, R4 on the blind t3 grade: t3-x-1, *"a program's own Heroes handler"*, is *"the
  shape the critic measured killing its program"*, so *"t3 measured spellings of a route that is
  unsafe without the stop below"*.
- `246-*.md:449-450`, R4's prior art: *"the refusal of Heroes code in a handler taken from
  Microsoft's HandlerRoutine page"*; issue 2497's `## Prior art` files it as **`Taken from
  Microsoft`**.
- Issue 2497's Recommendation, which its Verdict records the author ratifying at about 07:12 as a
  reading: *"R4, no stop sentence enters the specification, ...; no Heroes code in a handler."*
- At the base, in the runtime this sitting starts from: `runtime/parts/thread.c:99-100` (defect
  2492, merged at `351b6fcf4`) *"panel 246's R4 rules that no Heroes code runs in a handler"*, and
  its refusal at 111-112 tells every program *"give C no Heroes function as a signal handler"*.
- The coordinator's grep, `'own handler\|program.s own'`, cannot match any of these: the phrase is
  *Heroes code in a handler*.

So a sentence ratified as a rule stands, the base's runtime already reads it as one, and D's landing
words repeat it. Refusal (A, C) and deferral (E, F, and the route in item 6) are consistent with
it; a resolution that leaves a program's own Heroes code running inside the signal (D alone, or the
handler "made safe" in place) contradicts its plain words and would amend a ratified resolution,
which is the author's. Whether R4's sentence binds a program's own handler or only the layer's stop
is the reading the sitting must state; nothing in 246 says it is the layer's alone.

### 04:12 to 04:16, item 5: the tracked programs, run (hold), and the search widened

- `./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2496` in my tree:
  `run (only fixedbugs-2496): 1 passed, 0 failed`. `... run fixedbugs-2257`: `run (only
  fixedbugs-2257): 3 passed, 0 failed`: the substring selects three cases, the third
  `fixedbugs-2257-a-sigabrt-handler-of-the-programs-own-c-never-runs-after-a-stop`, a C handler.
- What each is: the 2496 case's `on_signal` runs on a `hero_thread_spawn` worker the signal is sent
  to and calls C's `fresh_heard_one`; `.expected` *"200 rounds, 200 signals handled on the worker
  they were sent to"*. A delivery-detecting refusal (C, F's refusal arm) refuses it; E moves it off
  the worker, which falsifies its title; A refuses it at build. The two 2257 Heroes handlers take
  SIGABRT and never run after a stop; the first calls `exit(code: 0)`, which is `hero_exit`, C's
  `exit()` after a lend audit (`runtime/parts/os.c:1005-1011`), not async-signal-safe were it to run.
- Searched every tracked `.hero` (`grep -rl -E 'function
  (signal|sigset|bsd_signal|sigaction|SetConsoleCtrlHandler|sigvec|signal_set|install_handler|on_signal)\b|"signal\.h"'
  --include='*.hero' .`): ten files, the brief's three plus 436, 437 and 465 (bind `kill` only),
  `docs/panel/173-briefs/q5_raise.hero` (`raise` only), `selfhost/cli/order_search.hero` and
  `shadowed_library.hero` (`"signal.h"` as a header name), `selfhost/foreign_at.hero:409` (a test's
  string). Headers under `tests`, `examples`, `packages` calling `signal(`, `sigaction(`, `sigset(`,
  `bsd_signal(` or `SetConsoleCtrlHandler(`: four, each installing its own C handler or
  `SIG_IGN`/`SIG_DFL`, none taking a function pointer from Heroes. `site`, `docs` outside
  `docs/panel`, `packages`, `editors`: no Heroes handler taught. So no fourth tracked program.
- **The brief's question about a wrapper, run**: `critic/work/wrap/wrap.h` (`static inline int
  wrap_install(int sig, void (*handler)(int))` calling `sigaction` with `SA_RESTART`) and
  `wrap.hero` passing `on_signal`, which builds `f"caught {sig} ..."` and prints it. `heroes-next
  build`: exit 0. Run under `timeout -s KILL 30`: `0`, `caught 30 through the program's own
  sigaction`, `0`, exit 0. A program's own C reaches `sigaction` with a Heroes function as a
  parameter and runs it inside the signal; route A by the callee's name does not see it.

### 04:12 to 04:18, item 6: route F, and the routes nobody listed

- **F is a route, not a restatement of E**: it shares E's delivery and differs in how the call is
  chosen. E chooses at build, by the callee's name (the compiler-engineer brief's item 3 says so),
  so it inherits A's hole, `wrap_install` above; F chooses at run time by C's detection, so it sees
  `wrap_install`'s handler on the thread's own stack and inherits C's holes (the alternate stack,
  frames with no record, Windows, where protoC's probe is `#if !defined(_WIN32)`) and C's cost on
  every call C makes. What F adds that E has not: a Heroes function called by the program's own C
  handler, which expects the call done (or a result) when it returns, so F's deferral must refuse
  there; that case is in neither brief.
- **Missing: the deferral to a safe point on the program's own thread** (neither a runtime thread nor
  a refusal). It is Python's shape by the page panel 246's historian marked `verified`
  (`246-reports/historian.md:342-346`): *"Python signal handlers are always executed in the main
  Python thread ..."*, *"the low-level signal handler sets a flag which tells the virtual machine to
  execute the corresponding Python signal handler at a later point"*, with its caveat *"A
  long-running calculation implemented purely in C ... may run uninterrupted for an arbitrary amount
  of time"*. **00-shared.md names Python's module as E's precedent, "on a runtime thread", and the
  verified page says the main thread**; historian.md describes it as the main thread, so the two
  briefs disagree. OCaml's polling points and Ruby's trap, in historian.md's E list, are this shape
  too. For Heroes: the runtime's C handler records the signal, and compiled code asks at its safe
  points (loop heads, function entries, or the runtime's return from the allocator) and runs the
  Heroes function there, on the home thread or on the interrupted one; on the interrupted one the
  2496 case keeps its meaning. Unbuilt; its cost is a poll every program that hands C a callback
  would pay (the set `callback_guard.hero` already computes), and its latency is Python's caveat.
- **Missing: a runtime-owned door for any signal**, R4's own mechanism (a C handler writing one byte,
  the layer's stop) offered for signals other than INT and TERM, counted or flagged for the program
  to read between steps. t2 (count SIGUSR1) is exactly what the layer's stop cannot do. R4's veto
  (`246-*.md:351-355`) refuses a *stop* door in `hero_os.h`; a door for other signals is unruled and
  would owe Principle 0's ticket. D's words today send *"any other signal"* to *"a handler written in
  C in the program's own header"*, whose two-module trap panel 246's critic measured (`246-reports/
  critic-briefs.md:156-162`: a `static` flag per unit, *"drained after 1000 requests"*).
- **Missing: a refusal by the handler's body, not the callee's name**: a function given as a handler
  is refused only where its body allocates, formats, prints or calls C outside an async-signal-safe
  list; `quiet` (`getpid`), `t3x1` (`close`) and the 2496 case (its own C) stay legal. It keeps A's
  selection hole (how the checker knows a function becomes a handler).
- **Missing: "made safe" in place**, which 00-shared.md names as an option (*"refused, made safe, or
  left"*) and no route is: a handler-time arena for allocation, an integer formatter without
  `snprintf` (`f64.c:267-273`), `print` by `write(2)` inside a handler. Item 1 shows why it is wide:
  the deaths are in C's locale lock, not only the allocator.

### 04:16 to 04:20, item 7: the bounds

- **The blind seat**: 12 × 0.40 = 4.80, inside 5. The runner's 3.60 bounds when a session starts,
  not what the last one costs, so the total is at most 3.60 plus the last session's cost, its cap
  0.40 plus whatever one request passes the cap by. *"A second bound that holds whether or not the
  CLI can pass its cap by one request"* holds only while one request costs under 5.00 less 3.60 less 0.40
  = 1.00 USD; unrun. 246's costs as cited (`246-*.md:553-555`: 3.8216 USD, 0.1419 to 0.3258, cap
  0.45, runner 6.50) and the inference 1.70 to 3.91 for twelve: arithmetic holds. Arm a: sixteen
  sessions at 0.40 is 6.40 by caps; at 0.30 it is 4.80, below 246's largest session (0.3258), so a
  session could stop before its report (the brief's `result` fallback covers that).
- **`timeout 60` is not a bound for these programs**: 00-shared.md's rules and llm-ergonomist.md's
  grading say `timeout 60`, which sends SIGTERM, and t1 asks the program to catch SIGTERM (t3x1 does).
  Panel 246's synthesis (`246-*.md:205-207`) graded with SIGKILL to the group *"since a program
  catching SIGTERM outlives a plain `timeout`"*. Write `timeout -s KILL 60`.
- **`storm.py` bounds time, not bytes**: `p.communicate(timeout=30)` holds every byte of stdout and
  stderr in the driver's memory and prints 3 and 14 lines; `t3run.py` caps each stream at 65,536.
  A graded t2 answer that prints in a loop is held whole for up to 34.3 s. And `storm.py` starts the
  binary with `start_new_session=True`, so an outer `timeout` that ends the driver leaves the binary
  running in its own group (defect 425's shape): run the driver unwrapped, or kill the group.
- **The spec-warden's storm measures nothing of a SIGINT handler**: question 3 asks each handler
  program run *"once with one SIGINT and once under the lane's storm"*; `storm.py` sends
  `signal.SIGUSR1` only, whose default action ends a program that took no SIGUSR1 handler at the
  first signal.
- **t2's rubric rests on a timing nobody measured**: my t2-shaped program at the base
  (`critic/work/t2probe/t2.hero`, 2,000,000 `f"item {i}"` pushed, no handler), `/usr/bin/time -p`
  three runs at load 3.53: real 0.65, 0.35, 0.34 (user 0.32 to 0.33). `storm.py ... 4 0.3` sends
  its first signal at 0.3 s, so a right answer can end with few or none sent, and *"M at least 1"*
  then grades it wrong.
- **Runs a rule forbids**: compiler-engineer.md points C's SIGURG at `tests/golden/lldb/` and
  `suite_lldb`, a whole suite, where 00-shared.md allows whole suites only to a route that changes
  what the checker refuses; say whether `lldb` is asked. No brief asks a paid run beyond the blind
  seat's, a credential, or a write outside its folder.
- **Storms side by side**: each driver spins `kill` for 4 s; several seats' storms beside a round
  gate change the counts of signals sent, so routes are compared by outcome per run, never by counts.

### Other framing facts, checked

Hold, each by its own command: `gh issue view 2494 --json ...`: `kind:defect, class:blocking,
area:runtime`, milestone `M-core-packages`, filed 2026-11-12, no comments; `brief-common.md` step 4
(lines 59-69); the sittings 234 to 246 by the brief's `ls`, and 232, 233 naming no
`M-core-packages`; `.claude/skills/panel/SKILL.md:29`; `git log --oneline ae969c89e..ae5e2cedd | wc
-l`: 25, with `1c4c6f9ae`, `f74f5ca54`, `a833b7a17`, `351b6fcf4` in it; `check-ignore -v`:
`.gitignore:34`; every line count of 00-shared.md and compiler-engineer.md (`wc -l`, sixteen files);
the handler installs (`grep -n -E 'sigaction\(|signal\(SIG'`: `stack.c:1054-1055`, `SA_ONSTACK` 1052,
`sigaltstack` 1009, bounds 335-336, `os.c:471`, 488, Windows 260 and 320, `panic.c:54`, `run.c`
681-702, `hero_net.c` 573-650); `thread.c` 74, 80, 82-101, 102-116; `callback_guard.hero` 78, 95,
121, 140, 144; the thunk's paragraph 16-24 and `hero_callback_of` at 200; `body.hero:6`;
`check/ffi.hero:47`, 150; the spec's 492 lines, 28,770 bytes, sha256, `signal\|handler` 0, lines
403-404, `ships` nothing, line 135; design.md 253, 112 and the six `payment` lines; panel 114's
183-184; every count of `before-*.log`, `protoC/*.log`, `protoD/*.log`, `linux-*.log` (7 refusals in
`linux-protoC.log`); the instruction counts and outputs of `cost/` and `protoD/`; `docker images`:
`heroes-linux-arm64:latest 338a1044ac90`; `tailscale status`: `apponfly-vps ... offline, last seen
3h ago` at 04:15; `HEROES_RUNTIME` in `selfhost/main.hero:291`; the lane's `storm.py`, `storm.hero`
and `quiet.hero` byte-identical to panel 246's critic's (`cmp`).

Not verifiable, no file holds them: **the lane's report** (00-shared.md cites it four times,
ffi-pragmatist.md once; `grep -rl` of its words under `b34-scratch` and `p247-scratch` finds none, and
`b34-round/` holds `report-ci.md`, `report-fsl.md`, `report-layer.md` only); the coordinator's
prompt line *"A NEW diagnostic code or a refusal of a program correct today is a sitting's ruling"*;
the coordinator's 03:16 to 03:20 durations (past, and a waiting run by its own words).

Wrong or misattributed: `dd bs=1 count=4194304` is not in `LINUX-MACHINE.md` (no `dd bs=`,
`count=4194304` or `head -c` in it) but in `p246-scratch/246-ffi-pragmatist/linux-a.sh`;
historian.md's ranges `51-97` and `347-392` hold more than its five names (Python's `signal` page,
libuv's `uv_signal_t`, `signalfd(2)`, FreeBSD's `kqueue(2)`, POSIX.1-2024 § 2.4, all `verified`),
while HandlerRoutine sits at 312 and 394, outside both, and the brief then asks for Python's module,
libuv and POSIX's list again; *"three commits over the base"* is 3 by `--first-parent` and 6 by
`git log`; the round's head is now `bdeb17737` (`git -C .claude/worktrees/b34-round log --oneline
-6`), 83 files over the base, adding `runtime/parts/dir.c` and `fs.c` (defect 2500) and the W3 line
in panel 246's file.

## Questions the sitting should ask and does not

- **What 2494 is, and not only where it was seen** (CLAUDE.md § RUN IT, CL-078). The defect is
  titled and D is built for the allocator; the deaths measured today are two: the allocator's own
  freelist check (SIGTRAP) and Darwin's recursive `os_unfair_lock` in `localeconv_l` under `snprintf`
  (SIGKILL, nothing said), which is **10 of 13** reported deaths of the lane's case without D
  (`noD`) and every SIGKILL of the day. The class is a Heroes body run inside the signal reaching C
  that is not async-signal-safe: `snprintf` through `hero_int_to_str` and the 16 `snprintf` of
  `runtime/parts/f64.c`, stdio through `print`, C's `exit` through `hero_exit`, `malloc`. Each route
  is judged against that class, and D closes one member of it.
- **Does R4's ratified "no Heroes code in a handler" bind a program's own handler?** (item 4). If
  yes, leaving Heroes code inside the signal is closed; if no, the sitting amends a ratified
  resolution and says so to the author.
- **Who owns D's words**, which land in batch 34 before the sitting rules and prescribe a remedy
  (a C handler in the program's own header) that panel 246 measured as a two-module trap and that E,
  F or a safe-point deferral would make false; and **is D's case to stay a flake on this Mac** (5 and
  12 of 200), its handler formatting `{sig}` while `main` formats `{r} {i}`?
- **Is every refusal async-signal-safe end to end**: protoC's `snprintf` lost its own message 1 of 3;
  the base's 2492 refusal formats with `snprintf` and ends through `hero_panic` (`fflush`, `fprintf`)
  and `abort()`; and `abort()` inside a handler on an exiting thread ended in Darwin's `__abort`
  trap, SIGTRAP, not SIGABRT (the two `case` reports of 02:00). What exit status does the harness
  then read, and does the case's `.expected` hold?
- **What glibc does where Darwin kills**: on Linux arm64 D's case read 0 of 50 off the stop, so
  either the handler's `snprintf` inside `main`'s is harmless there or it corrupts in silence;
  unrun, and nobody is asked.
- **What the 2496 golden case means under the adopted route**: its `.expected` says the signals were
  *"handled on the worker they were sent to"*.
- **Whether t2 can grade a right answer right** (item 7's timing).
- **The instrument**: every Darwin storm death names itself in
  `~/Library/Logs/DiagnosticReports/<binary>-*.ips` (`asi` and the faulting frames), read in
  seconds; no brief names it, and two sittings (246's critic, this lane) wrote *"SIGKILL with nothing
  on stderr"* without reading it. The reports are a sample (17 SIGKILL runs of `withD` left 4).

## The list

1. **The `-9` deaths are Darwin's own, not a SIGKILL anyone sent.** Command: `ls -lT
   ~/Library/Logs/DiagnosticReports | grep -E 'storm|quiet'`, each report's JSON read with `python3
   -I` (`exception`, `asi`, faulting frames). Printed: at 01:28:57 to 58 three `EXC_BREAKPOINT`
   `SIGKILL` (pids 94041, 94060, 94066) and two `SIGTRAP` (94063, 94069), the log's three `-9` and
   two `-5`; `asi` *"BUG IN CLIENT OF LIBPLATFORM: Trying to recursively lock an os_unfair_lock"*;
   frames `_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < localeconv_l < __vfprintf <
   _vsnprintf < __snprintf_chk < hero_int_to_str < h_storm_on_signal < h_0cb_storm_on_signal <
   _sigtramp < localeconv_l < ... < hero_int_to_str < h_storm_round`. Repair: 00-shared.md's
   `before-storm.log` bullet says this instead of *"whose it was is a question"*, and § The rules
   names the reports as the instrument every Darwin storm reads.
2. **protoC's refusal kills itself the same way.** Command: the same read of
   `quiet-2026-11-13-024314.0002.ips`. Printed: `SIGKILL`, pid 75776, `... __snprintf_chk <
   hero_thread_guard < h_quiet_on_signal < h_0cb_quiet_on_signal < _sigtramp < localeconv_l < ... <
   hero_int_to_str < h_quiet_round`. Repair: 00-shared.md's C bullet replaces *"the lane's report
   reads it as outliving 30 s"* with this; the compiler-engineer's item 1 and the ffi-pragmatist's
   question 2 say the refusal is written without `snprintf` or stdio, measured losing its message 1
   of 3 on `quiet`.
3. **D does not see it, and D's own case is a flake on this Mac.** Command: `cat
   b34-scratch/b34-signals/c2494/runs*.log` and the reports' tally. Printed: `withD` 5 and 12 of 200
   runs `exit 137`, stderr empty, slowest 0.095 s and 0.186 s; `withD-O2` 1 of 50 each; libgmalloc
   and `withD-san` 0 of 50; four `withD` reports `_os_unfair_lock_recursive_abort` with
   `hero_int_to_str < h_fixedbugs2494..._on_signal` over `hero_int_to_str < ..._round`; `noD` 10 of
   13 the same; Linux arm64 0 of 50 and 0 of 20. Repair: 00-shared.md's D bullet makes *"What it
   does not see"* a measurement naming `hero_int_to_str` (`runtime/parts/f64.c:267-273`), gives the
   flake rate, and question 7 asks each route what it does about this member.
4. **D as landing is not protoD, and its words carry a remedy.** Command: `git -C
   .claude/worktrees/b34-signals status --short`; `cat b34-scratch/b34-signals/expected-2494.new`.
   Printed: ` M runtime/parts/alloc.c` and three untracked `fixedbugs-2494-*`; the stop through
   `raise < hero_alloc_reentered < hero_alloc_enter`; *"... answer any other signal with a handler
   written in C in the program's own header, never with a Heroes function"*. Repair: 00-shared.md
   quotes D's words as landing and asks each seat whether they stay true under its route.
5. **Panel 246's R4 is a ratified sentence, not only a mechanism.** Command: `sed -n
   '304,308p;318p;449,450p' docs/panel/246-*.md`; `gh issue view 2497 --repo heroes-lang/heroes`;
   `sed -n 96,112p runtime/parts/thread.c`. Printed: *"No Heroes code runs in a handler"*; *"a route
   that is unsafe without the stop below"*; *"the refusal of Heroes code in a handler taken from
   Microsoft's HandlerRoutine page"*; 2497's ratified recommendation *"...; no Heroes code in a
   handler."*; thread.c *"panel 246's R4 rules that no Heroes code runs in a handler"* and *"give C
   no Heroes function as a signal handler"*. Repair: 00-shared.md replaces *"not ruled there as far
   as the coordinator read"* with these lines, and the questions ask the sitting to state its
   reading of R4 and to say where a route amends it.
6. **Route A's hole is run, not a question.** Command: `critic/work/wrap/wrap.h` and `wrap.hero`,
   `heroes-next build`, the binary under `timeout -s KILL 30`. Printed: build exit 0; `0`, `caught
   30 through the program's own sigaction`, `0`, exit 0. Repair: 00-shared.md's A bullet replaces
   *"would reach it, a question"* with this run.
7. **A route is missing, and Python is filed under the wrong one.** Command: `sed -n 342,350p
   docs/panel/246-reports/historian.md`. Printed: *"Python signal handlers are always executed in
   the main Python thread"*; *"the low-level signal handler sets a flag which tells the virtual
   machine to execute the corresponding Python signal handler at a later point"*. Repair:
   00-shared.md adds the deferral to a safe point on the program's own thread (home or interrupted)
   with its questions (where the poll stands, its instructions on `guarded.hero` and a loop, the
   latency of a blocking C call, the 2496 case), moves Python there from E, and historian.md moves
   Python, Ruby and OCaml to it.
8. **F is a route, not a restatement** (a reading): E's delivery with C's selection, so it sees
   `wrap_install` where E by name does not, and inherits C's holes and cost. Repair: 00-shared.md's
   F bullet adds the case it alone meets, a Heroes function called by the program's own C handler
   that expects the call done, or a result, when it returns.
9. **Three more routes nobody listed**, each unbuilt: a runtime-owned door for signals other than
   INT and TERM (R4's byte-down-a-pipe, counted; R4's veto at `246-*.md:351-355` refuses a *stop*
   door only); a refusal by the handler's body (allocates, formats, prints, or calls C outside an
   async-signal-safe list) rather than the callee's name; and "made safe" in place (a handler arena,
   an integer formatter without `snprintf`, `print` by `write(2)`). Repair: 00-shared.md lists them
   as candidates a seat may build or price, or says why not.
10. **The 2496 case is a program every refusing and deferring route changes.** Command:
    `./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2496`; its source and
    `.expected`. Printed: `run (only fixedbugs-2496): 1 passed, 0 failed`; *"200 rounds, 200 signals
    handled on the worker they were sent to"*. Repair: question 1 asks what each route does to that
    claim, not only whether the case runs.
11. **`fixedbugs-2257` selects three cases, and one handler calls `exit`.** Command: `... run
    fixedbugs-2257`; `sed -n 1005,1011p runtime/parts/os.c`. Printed: `3 passed, 0 failed`;
    `hero_exit` is `hero_lend_audit_given(...)` then `exit((int)code)`. Repair: 00-shared.md names
    the third case (a C handler); D's residue list adds C's `exit` inside a handler.
12. **`timeout 60` does not bound a program that catches SIGTERM.** Command: `sed -n 92,93p
    00-shared.md`, `sed -n 77,78p llm-ergonomist.md`, `sed -n 205,207p docs/panel/246-*.md`.
    Printed: *"the BINARY run under `timeout 60`"*; *"every binary is run under `timeout 60`"*;
    246 graded with SIGKILL *"since a program catching SIGTERM outlives a plain `timeout`"*. Repair:
    `timeout -s KILL 60` in both briefs.
13. **`storm.py` bounds time, not bytes, and leaves its child behind if killed.** Command: `cat
    r2494/storm.py`, `cat t3x1/t3run.py`. Printed: `p.communicate(timeout=30)` holding all output,
    `start_new_session=True`; `t3run.py` caps each stream at 65,536. Repair: the briefs hand a copy
    of `storm.py` with `t3run.py`'s bounded readers and a group kill at its end, run unwrapped.
14. **The spec-warden's storm sends the wrong signal for its probes.** Command: `grep -n kill
    r2494/storm.py`. Printed: `os.kill(p.pid, signal.SIGUSR1)`. Repair: spec-warden.md question 3's
    programs also take SIGUSR1, or the storm's copy takes the signal's name.
15. **t2's rubric can grade a right answer wrong.** Command: `critic/work/t2probe/t2.hero` built and
    timed three times with `/usr/bin/time -p`. Printed: real 0.65, 0.35, 0.34, against `storm.py`'s
    0.3 s arm. Repair: before any session, a reference t2 answer runs five times under the grading
    command and M is read; where M can be 0 the arm or the task's count changes (six `task.md`
    folders re-hashed), registered before any session.
16. **Panel 246's open question about the blind folders' names is not answered.** Command: `sed -n
    515,517p docs/panel/246-*.md`; `ls p247-scratch/247-llm-ergonomist`. Printed: *"a name saying
    less than `246-llm-ergonomist/` is a question for the next sheet"*; `t1-e-1` to `t2-x-2` under
    `247-llm-ergonomist/`. Repair: neutral folder names (the map in the coordinator's sheet), or
    llm-ergonomist.md says why not.
17. **The runner's second bound is worded past what it holds.** Command: the arithmetic. Printed: 12
    × 0.40 = 4.80; at most 3.60 plus the last session's 0.40 plus one request. Repair: *"under 5
    while one request costs under 1.00 USD, unrun"*, or a runner bound that holds at the cap plus
    the largest single request 246's run files show.
18. **A whole suite pointed at where the rules forbid it.** Command: `grep -n lldb
    compiler-engineer.md`; 00-shared.md's *"Whole suites only where your route changes what the
    checker refuses"*. Repair: compiler-engineer.md says whether `suite_lldb` runs for C's SIGURG or a
    single `lldb` session by hand answers it.
19. **The lane's report is cited five times and is on no disk.** Command: `grep -rl` of its quoted
    words under `b34-scratch` and `p247-scratch`; `ls b34-scratch/b34-round`. Printed: no file;
    `report-ci.md`, `report-fsl.md`, `report-layer.md`. Repair: the report saved beside the briefs
    before any seat starts, or the five citations rewritten as the coordinator's readings with their
    commands.
20. **`dd bs=1 count=4194304` is not the Linux page's.** Command: `grep -n -E 'dd bs=|count=4194304|head
    -c' docs/platforms/linux/LINUX-MACHINE.md`; `grep -rl -F count=4194304 p246-scratch`. Printed:
    nothing; `p246-scratch/246-ffi-pragmatist/linux-a.sh`. Repair: ffi-pragmatist.md cites that
    script or states the bound as its own.
21. **The historian's "already read" list is short of its own ranges.** Command: `grep -n -E
    'https?://'` over `246-reports/historian.md:340-392`; `grep -n HandlerRoutine`. Printed: Python
    `signal`, PEP 475, libuv `uv_signal_t`, Go `os/signal`, `signal-hook`, `signalfd(2)`, `kqueue(2)`,
    POSIX.1-2024 § 2.4; HandlerRoutine at 312 and 394. Repair: historian.md lists all of them, widens
    the range to 400, and drops from its asks what 246 already quoted.
22. **The round moved again under the brief.** Command: `git -C .claude/worktrees/b34-round log
    --oneline -6`; `git diff --stat ae5e2cedd bdeb17737`; the 246 file's diff. Printed: head
    `bdeb17737`, 83 files, `runtime/parts/dir.c` and `fs.c` added to the runtime's list, and *"W3
    held, T1 priced +94 real ... and +73 vendored"*. Repair: 00-shared.md's § The base says so (and
    `3c0920a3b` is 3 commits by `--first-parent`, 6 by `git log`); spec-warden.md gives 94/73 as the
    nearest measured marginal ratio, scale only.
23. **Everything else the briefs state, checked and holding** (§ Other framing facts above): the
    base, the line counts, the handler lines, the spec's size and hash and counts, every price and
    arm hash, the twelve folders byte for byte, the CLI's flags, the logs' counts, the costs, the
    image, the box's status, `HEROES_RUNTIME`, the driver's provenance. No repair.
