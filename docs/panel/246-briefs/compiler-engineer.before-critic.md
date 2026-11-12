# Panel 246, compiler-engineer

Read `00-shared.md` first, whole. You judge **Part A** (the soundness lane, with the ffi-pragmatist)
and **Part B** (the full panel). Your folder is `.claude/worktrees/p246-scratch/246-compiler-engineer/`,
your tree `tree/` in it at `b8c8240d4`, your compiler built there from the seed in two stages
(00-shared.md § The base). Your report is `docs/panel/246-reports/compiler-engineer.md` in
`.claude/worktrees/p246`, one section per part, each in your definition's output structure, written
as you go. Every route you weigh, you **build** in your tree and run on the named cases; a route that
does not build is reported as not built, never adopted. Line counts below are `wc -l` at the base, not
`tests/harness/suite_layout.hero`'s unit; where a count decides anything, ask the instrument.

## Part A: defect 2453

**Where the code is** (`wc -l`, then `grep -n` for each line cited):

- `selfhost/fetch/staging.hero`, 286 lines: `opened` 46-50 (the folder's name from `process.pid()` and
  `process.serial()`, and `process.remove_tree(folder)` before it is made, line 48); `placed` 57-92
  (the tree moved aside 73-82, the new one renamed into place 83-89, the ledger's in-memory push 90);
  `Ledger` 109-120; `begun` 129-133; `undone` 139-161; `ended` 166-174.
- `selfhost/fetch/add.hero`, 458 lines: `staging.begun` at 90, `ended` at 94, `undone` at 97, the
  catalogue written by `publish.rewrite` at 116.
- `selfhost/fetch/fetch_all.hero`, 260 lines: `begun` at 53, `ended` at 61 and 96 (*"Every tree fetch
  places is one its catalogue names, so the run is never taken back"*, 94-95).
- `selfhost/module/unplaced.hero`, 112 lines: `staging.begun` at 46 and `ended` at 55, on a build's
  error path (00-shared.md § Premises).
- `selfhost/cli/process.hero`, 475 lines: the `hero_os.h` group 40-68, `pid` 275, `serial` 282.
- `runtime/hero_os.h`, 488 lines (the `hero_fs_*` doors 172-300, `hero_os_pid` 333, `hero_run_serial`
  415); `runtime/hero_compiler.h`, 80 lines; `runtime/parts/fs.c` 415, `replace.c` 1268, `run.c` 1962
  (the serial 1425-1428; children and their process groups, 505-560).

**Build and run, in this order:**

1. **Today's behaviour at the base**, so every later number has its control: `kill -9` of `heroes add`
   and of `heroes fetch` at each phase below, and what a later `add` and a later `fetch` leave. The
   lanes' figures in the issue's comments (kills at 0.4 to 1.6 s of a 2.7 s run) are theirs, unrun by
   the coordinator: reproduce them or say they do not reproduce.
2. **Route A in your tree**: the lock door (its header, part, signature and answers yours to choose,
   with A2's question answered: `hero_os.h`, which panel 232 promises at 1.0.0, or
   `hero_compiler.h`, the compiler's own), its POSIX arm, the ledger written to disk before each rename
   in a format recovery reads back, recovery at the start of `add` and `fetch`, and what `build`'s
   `unplaced.hero` path does with a folder it makes. Give the lines per file, and what moves: the
   compiler's own tests, `tests/golden/fetch/` (47 cases), the emissions where `runtime/` moved.
3. **The phases**, each a kill and a later run, route A against the base: (a) the run's folder made,
   nothing placed; (b) between `placed`'s two renames, where the key names no folder; (c) a tree
   placed, the catalogue not yet written (`add`); (d) a `fetch` that had moved an author's edited tree
   aside for the catalogue's (the second comment's case: say whether recovery puts the edit back, which
   undoes what the fetch was for, or drops it, which a fetch that ended would have done, and which
   *leaves the folder as it found it* means); (e) while a `git` or `curl` child of the run is alive.
   How you hit a phase deterministically is yours (a slow `file://` index, a stopped process, a
   debugger); say which.
4. **A live owner**: a run held open (stopped, or blocked on a slow source) while a second run starts;
   its folder must stand untouched. And two recovering runs at once.
5. **PID reuse**: settle 00-shared.md's inference, that `opened`'s `remove_tree` at line 48 removes a
   dead run's folder when a later run is handed its pid, `replaced-N` included; reproduce it or show
   why not. If it holds, say whether route A closes it (the lock, not the name, decides) and whether
   it is a defect of its own today.
6. **The cases**: what a golden under `tests/golden/fetch/` can hold of a kill (the form has no kill
   step: a dead run's folder planted with an unlocked lock file and its ledger is one way, a question),
   and whether recovery prints anything, which is A1's R19 question.
7. **Routes B and C, priced the same way**: B's report (where, in whose words, at which exit code) and
   C's pin (which golden, whose header reason).
8. **Linux arm64**: write `linux-a.sh` in your folder, the body `docs/platforms/linux/LINUX-MACHINE.md`
   § How to run something gives (`docker run --rm -v "$PWD":/src:ro heroes-linux-arm64 bash -c 'tar -C
   /src --exclude=.git --exclude=build -cf - . | tar -xf - ; ...'`, no Dockerfile), run from your
   `tree/`, which builds the seed, stage 2 and your kill cases; the coordinator runs it
   (`heroes-linux-arm64:latest` is image `338a1044ac90`, `docker images`) and hands you its log. Until
   then the Linux leg is unrun in your report. **Windows**: `find /opt/homebrew -maxdepth 6 -name
   windows.h` found nothing and `which x86_64-w64-mingw32-gcc` found nothing, so the `LockFileEx` arm
   is written and not compiled here (a question whether another route compiles it on this Mac); say
   where it would first be compiled.

## Part B: the server step

**B1, what crosses into a thread.** The rule as the compiler holds it: `selfhost/check/ffi.hero`, 412
lines, `crosses_the_boundary` 79-94 (`.str` and `.ptr` answer yes, `.array` and `.map` no), `refused`
144, `result_may_stand` 290; `selfhost/emit/ffi_narrowed.hero`, 499 lines, `ffi_callback_type` at 238-
258; `runtime/hero_os.h:417-447`; `runtime/parts/spawn.c`, 444 lines, `hero_thread_claim()` at 226;
`runtime/parts/thread.c`, 99 lines, the guard. 00-shared.md measured a `str` body refused at build by
clang's header and a `[i64]` body at check. Run every claim T1 and T2 would make, and the shapes
beside: a `ptr` body, a record of a group, `f64`, `bool`, a `-> ()` body, a body that is itself a
callback. Say whether a `str` body should be refused by the checker on the author's line rather than
by clang at build (design.md §4.17's deliverable), its lines, and whether that is a diagnostic class
this sitting must rule on. And the wider rung panel 113's R5 named (00-shared.md § Part B, B1): a
spawn door whose body takes a `str`, built beside the `i64` one in your tree, and that sitting's
measurement run again on it (one `str` shared by many threads, under `--sanitize` and under
ThreadSanitizer as B3 says), so the sitting can say whether the rung it writes into the spec, which
panel 232 then promises, should be `i64` alone.

**B2, the stop.** Build the shape the proposal names in your tree: a runtime part owning a flag
SIGINT and SIGTERM set (an `_Atomic` or `volatile sig_atomic_t`, its handler async-signal-safe), and
the Heroes call; a Heroes loop that asks between its steps; the built binary run under `timeout 30`,
sent `kill -INT <pid>` and `kill -TERM <pid>`, its drain and its exit code read. Then the shapes
beside it:

- a loop blocked in a C call when the signal comes (a `read` on a pipe nobody writes; an `accept` on a
  loopback socket): does the loop see the flag, with the handler installed with and without
  `SA_RESTART`, and what does the second choice do to every other blocking call a package makes;
- the same binary under `heroes run`, whose waiting parent forwards the four ending signals to the
  child's group and sends SIGKILL after `HERO_RUN_GRACE_MS`, 2000 ms (`grep -n 'GRACE_MS'
  runtime/parts/run.c`), and the runtime's own rule that it catches those signals only *"where this
  process had left them at their default"* (`run.c:518-525`): a program that armed the stop and then
  runs a child;
- a second Ctrl-C after the first: ends at once, or is swallowed;
- S1, S2 and S3 priced as code: S3 is a built-in, so count where it lands (inventory, checker,
  emitter, the spec's built-ins table the `spec` suite reads, `selfhost/inventory.hero`). **Build S1
  and S2 exactly as 00-shared.md spells them** (`hero_stop_catch()`, `hero_stop_asked() -> i64`, in
  `hero_os.h`; S1 arms at `hero_stop_catch()`, S2 at the first `hero_stop_asked()`, so they are two
  runtimes), and S3 (`stop_asked() -> bool`) if you can: each tree is the grading compiler of the
  blind seat's task t3 for that arm (`llm-ergonomist.md` § Grading), so say its path and its
  `--version` second line;
- where the door lives, each priced: `hero_os.h` (promised at 1.0.0, panel 232); the platform layer
  of feature 2295 (in flight, lane `core-platform`, never depended on); a package's own header, which
  feature 1069's re-check of 2026-10-25 built as a three-line `stopflag.h` (`gh issue view 1069 --repo
  heroes-lang/heroes --comments`) and which panel 234's R6 may refuse (*"no package ships platform C of
  its own"*, `grep -n 'ships platform C' docs/panel/234-*.md`).

**B3, copy-on-write and the threads.** The four sites, `grep -n 'refcount == 1' runtime/parts/*.c`
(`cow.c:44`, `:88`, `:93`, `map-write.c:128`); panel 113's R4 return condition, *a program that
corrupts memory through those sites with every reference counted* (`docs/panel/113-*.md:246-257`).
Enumerate from the tree, not from this list, every way a counted value can be reached from two threads
under the rung: a `constant` (00-shared.md: `HERO_ARRAY_STATIC`, `heroes_runtime.h:196-197` says a
static's count is below zero), a string literal, `args()`, a value C hands back, a `ptr` a program's
own C turns into an `i64`, a callback C calls from its own thread (the guard), the stop flag. Run the
reachable ones under ThreadSanitizer: `heroes build --sanitize` is `-fsanitize=address,undefined`
(`selfhost/cli/flags.hero:235`), so TSan means building the emitted C by hand; say how. Then what the
server step needs: the descriptor crossing as the `i64`, a request built inside its thread, and any
state shared across connections (a request counter, a cache), which the rung does not let a program
write; and whether the step is one thread (`M-core-packages.md:35`) or one connection per thread
(issue 893).
