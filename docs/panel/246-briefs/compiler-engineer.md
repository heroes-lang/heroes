# Panel 246, compiler-engineer

Read `00-shared.md` first, whole. You judge **Part A** (the soundness lane, with the ffi-pragmatist)
and **Part B** (the full panel). Your folder is `.claude/worktrees/p246-scratch/246-compiler-engineer/`,
your tree `tree/` in it, a `git archive` of **`ef50a7478`** (tree `084b36b7d5db6b342c281468fff120b2fe98349b`),
your compiler built there from the seed in two stages (00-shared.md § The base, `sources
796d1133542b1909`). Your report is `docs/panel/246-reports/compiler-engineer.md` in
`.claude/worktrees/p246`, one section per part, each in your definition's output structure, written
as you go. Every route you weigh, you **build** in your tree and run on the named cases; a route that
does not build is reported as not built, never adopted. Line counts are `wc -l` at the base, not
`tests/harness/suite_layout.hero`'s unit; where a count decides anything, ask the instrument.

## Part A: defect 2453, `blocking` since 2026-11-12

**Where the code is** (`wc -l` and `grep -n` at the base):

- `selfhost/fetch/staging.hero`, 286 lines: `opened` 46-50 (the folder's name from `process.pid()` and
  `process.serial()`, and `written(process.remove_tree(folder))?` at line 48 before it is made);
  `placed` 57-92 (moved aside 73-82, the new tree renamed into place 83-89, the in-memory ledger's push
  90); `Ledger` 109-120, `found` at 119; `begun` 129; `undone` 139-161; `ended` 166-174.
- `selfhost/fetch/add.hero`, 458 lines: `begun` 90, `ended` 94, `undone` 97, `publish.rewrite` 116.
  `selfhost/fetch/fetch_all.hero`, 260: `begun` 53, `ended` 61 and 96.
- `selfhost/module/packages.hero`, 301 lines: `unplaced.refusal` at 121, on every verb that resolves
  a `use <name>/...`; `selfhost/module/unplaced.hero`, 112: `staging.begun` 46, `ended` 55.
- `selfhost/cli/stub.hero`, 393 lines: `heroes new`'s staging rule, 175-176 and 180-184 (route D's
  precedent). `selfhost/cli/process.hero`, 475: the `hero_os.h` group 40-68, `pid` 275, `serial` 282.
- `runtime/hero_compiler.h`, 80 lines (the door's home in this lane); `runtime/hero_os.h`, 488;
  `runtime/parts/fs.c` 415, `replace.c` 1335, `run.c` 1962 (the serial 1425-1428; children ended with
  their parent, 506-557 and 734-783; `execvp` at 1340).
- `runtime/heroes_guard_open.h` and `heroes_guard_close.h`, 649 lines each, 241 `hero_` names each;
  `selfhost/emit/guarded_names.hero`, 274; `tests/harness/suite_runtime.hero`, 1085, Rule 3 at 148,
  `SHARED_BY_DECISION` at 211, `panic.c hero_runtime_spoke` allow-listed at 233.

**Build and run, in this order:**

1. **The base's behaviour**, so every later number has its control: the coordinator's PID-reuse run
   (00-shared.md § What the coordinator measured, `p246-scratch/a-pid/`, `reuse.sh`) reproduced in your
   folder with your compiler, for `fetch` and for `check`; then `kill -9` of `add`, `fetch` and `check`
   at each phase of step 3 and what a later `add`, `fetch` and `check` leave.
2. **Route A in your tree**, its door in `runtime/hero_compiler.h` (00-shared.md § Part A: the lane is
   the soundness lane, so its door is compiler-private; if your route needs a door in `hero_os.h`,
   build it all the same, and say in so many words that it is **outside this lane**, for the synthesis
   to return to the author). The ledger written to disk before each rename, holding `found`; recovery;
   and what A2 asks of the `unplaced.hero` path. Give the lines per file and what moves: the compiler's
   own tests, `tests/golden/fetch/` (47 cases), the emissions where `runtime/` moved, `suite_runtime`'s
   Rule 3 (the held descriptor is an object a runtime part keeps: `_Thread_local`, `_Atomic` or an
   allow-listed line, `hero_runtime_spoke`'s the precedent), and the guard headers if the door is in
   `hero_os.h` (three files, `guarded_names.hero`'s tests red on a word they miss).
3. **The phases**, each a kill and a later run, A and D against the base: (a) the run's folder made,
   nothing placed; (b) between `placed`'s two renames; (c) a tree placed, the catalogue not written
   (`add`); (d) a `fetch` that moved an author's edited tree aside for the catalogue's: does recovery
   put the edit back (undoing what the fetch was for) or drop it (what an ended fetch does), and which
   is *as it found it*; (e) a `check` killed while its `git` child runs (00-shared.md measured it
   leaving a folder with no lock and a `heroes.packages/` the program did not have); (f) a live run
   held open (stopped, or on a slow source) while a second starts; (g) two recovering runs at once.
4. **Route D**, built: `opened` refuses a standing folder or takes the next serial. Alone, and as part
   of A; and say for B and C what each carries of it, since without it neither is true of the code.
5. **The cases**: what a golden under `tests/golden/fetch/` can hold (no kill step today; a dead run's
   folder planted with an unlocked lock file and its ledger, a question), and whether recovery prints
   anything (A1, R19's interface).
6. **Linux arm64**: `linux-a.sh` in your folder, the body of `docs/platforms/linux/LINUX-MACHINE.md` §
   How to run something (a read-only mount copied inside, no Dockerfile), each case `git init -q`ing
   its own repository around itself and none needing `curl` (the image has none); the coordinator runs
   it with `heroes-linux-arm64:latest` and hands you the log, the leg unrun in your report until then.
   **Windows**: your door's arm compiled with `zig cc -target x86_64-windows-gnu`,
   `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR` inside your folder (00-shared.md § Part A,
   Platforms), a compile and not a run; its run is the CI's Windows leg after the push.

## Part B: the server step

**B1, what crosses into a thread.** The rule as the compiler holds it: `selfhost/check/ffi.hero`, 412
lines, `crosses_the_boundary` 79-94 (`.str` and `.ptr` answer yes, `.array` and `.map` no);
`selfhost/emit/ffi_narrowed.hero`, 499, `ffi_callback_type` at 238-258; `runtime/hero_os.h:417-447`,
whose comment at 430-434 (*"WHAT MAY CROSS IS DECIDED BY THE CHECKER"*) is false today;
`runtime/parts/spawn.c`, 444, `hero_thread_claim()` at 226; `runtime/parts/thread.c`, 99, the guard.
Run every claim T1 and T2 would make and the shapes beside: a `ptr`, `f64` and `bool` body, a record
of a group, a handle (00-shared.md measured `Socket` refused by clang), a `-> ()` body. Whether a body
clang refuses should be refused by the checker on the author's line (design.md §4.17), its lines, and
whether that is a diagnostic class this sitting must rule on. Then the wider rung: panel 113's R5
(scalars and `str`, ratified) against journal 034:79 (`i64` alone, no reason given); find where the
narrowing was decided, in commits (`git log -S hero_thread_spawn -- runtime/hero_os.h` in a copy that
has history: run it through `git -C .claude/worktrees/b33-round log`, read-only) and records, or say
it is a gap; build a spawn door whose body takes a `str` beside the `i64` one and run panel 113's
measurement again on it (one `str` shared by many threads, under `--sanitize` and ThreadSanitizer).

**B2, the stop.** Build in your tree, each run as a binary under `timeout 30` and sent `kill -INT` and
`kill -TERM`, its drain, its output and its exit code read:

- **S1 and S2 exactly as 00-shared.md spells them** (`hero_stop_catch()`, `hero_stop_asked() -> i64`,
  in `hero_os.h`; S1 arms at `hero_stop_catch()`, S2 at the first `hero_stop_asked()`, so two
  runtimes), and **S3** (`stop_asked() -> bool`, a built-in: count where it lands, inventory, checker,
  emitter, `selfhost/inventory.hero`, the spec's built-ins table the `spec` suite reads) if you can.
  Each tree is the grading compiler of the blind seat's task t3 for its arm (`llm-ergonomist.md`
  § Grading): say its path and its `--version` second line. These doors are in `hero_os.h` and so a
  surface; Part B is the full panel, which may weigh one.
- **Routes (a), (b) and (c)** of 00-shared.md: the signals blocked and asked with `sigpending`; the stop
  as a descriptor polled beside the listening socket; the stop inside the layer's own `accept`
  (`runtime/platform/hero_net.c`, 651 lines, `hero_net_accept` 499-524, which asks `accept` again after
  `EINTR` at 509-512, so a loop that asks *between connections* is never reached while it waits:
  measure it with a loopback listener and no client).
- **The program's own header**, feature 1069's route: `p246-scratch/a-probes/stop1` and `stop2`
  measured one module draining after 3 and two modules after the probe's cap of 1000; whether a rule
  the compiler can hold (one unit, or a refusal) would make it safe, and its price.
- **Which thread runs the handler** with a thread C made alive (a `pthread_create` from a program's C)
  when SIGINT arrives, and with a `hero_thread_spawn` worker alive; **four signals or two**
  (`runtime/parts/run.c:518-525` catches four while a child is waited on, *"where this process had left
  them at their default"*), and the same binary under `heroes run`, whose waiting parent forwards them
  and sends SIGKILL after `HERO_RUN_GRACE_MS`, 2000 ms (`grep -n 'GRACE_MS' runtime/parts/run.c`);
  a second Ctrl-C after the first.
- **Task 1072's leak check**: a server-shaped loop stopped by each route that returns from `main`, and
  the leak line it prints (`hero_runtime_check_leaks()`, `selfhost/emit/c_main.hero:83` and `:102`),
  against a stop through `exit`, which is *"Not the leak gate"* (`runtime/parts/os.c:1005-1006`).
- **Rule 3** judges the flag: `_Atomic`, or `volatile sig_atomic_t` allow-listed with its reason as
  `hero_runtime_spoke` is (`runtime/parts/panic.c:32`; `suite_runtime.hero:233`). Run the `runtime`
  suite on the tree that holds it.

**B3, one connection per thread, and copy-on-write.** A connection is a handle (panel 234's R3;
`runtime/hero_net.h`, 102 lines, `hero_net_socket` at 45-46), and a handle body is refused by clang
against `hero_os.h` (00-shared.md). Price the three ways out by building each far enough to run a
loopback server and client in one process: a numbered handle (and what R3's *never another socket
that took its number*, `hero_net.h:33-36`, then costs), a spawn door that moves a handle with its marks
(what the checker and `ffi_narrowed.hero` must learn), or one thread. Then the four sites, `grep -n
'refcount == 1' runtime/parts/*.c` (`cow.c:44`, `:88`, `:93`, `map-write.c:128`), against panel 113's
R4 return condition, *a program that corrupts memory through those sites with every reference
counted* (`docs/panel/113-*.md:246-257`): enumerate from the tree every way a counted value reaches
two threads under the rung (a `constant`, a string literal, `args()`, a value C hands back, a `ptr` a
program's C turns into an `i64`, a callback C calls from its own thread, the stop flag), and run the
shapes panel 113 did **not** try (feature 925: `sort` through a function pointer, the `eq` and `hash`
descriptor walks, the drop-list drain under contention) and panel 111's four classes
(`docs/panel/111-*.md:125-170`). ThreadSanitizer means building the emitted C by hand: `heroes build
--sanitize` is `-fsanitize=address,undefined` (`selfhost/cli/flags.hero:235`); say how you built it.
