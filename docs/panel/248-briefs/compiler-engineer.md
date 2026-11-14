# Panel 248, compiler-engineer

Read `00-shared.md` first, whole: the base, the rules (**your binaries are named `ce-...`**, every
program that may not end has its bounds there), the platforms (**the Windows box is off**; zig here
compiles MinGW's target, the project's is MSVC's), why the sitting is the full panel, and the five
questions with every fact this brief leans on. You judge each question from the compiler's and the
runtime's side, with a veto on soundness. Your folder is
`.claude/worktrees/p248-scratch/248-compiler-engineer/`, your tree `tree/` in it, a `git archive` of
**`df7612ad3`** (tree `f72f48da250e919a68ebc6a98be0facccc29a052`), your compiler built there from the
seed in two stages (`sources 4c9a6a9800095eef`). Your report is
`docs/panel/248-reports/compiler-engineer.md` in `.claude/worktrees/p248`, in your definition's output
structure (`.claude/agents/compiler-engineer.md`, its line 36), one section per question, written as you
go. **The route you recommend on each question, you build in your tree and run on the cases you
name**; a route that does not build is reported as not built, never adopted (`/panel` 3c). Line counts
are `wc -l` at the base; where a module's length decides anything, ask
`tests/harness/suite_layout.hero`'s unit, not `wc -l` (`.claude/rules/module-shape.md`).

## Where the code is (`wc -l` and `grep -n` at the base, 00-shared.md has the rest)

- `runtime/parts/sigguard.c`, 364: the thunk's three questions (`hero_callback_entered`, 316-345), the
  probes (175-227), the one stop (121-148), the Windows arm (347-364). `runtime/parts/sigdoor.c`, 137:
  G, `hero_signal_watch` at 93, `hero_signal_take` at 108, `hero_signal_fd` at 115, the Windows arm's
  three at 125, 130 and 135. `runtime/parts/alloc.c`, 1540, D's guard from 481. `runtime/parts/thread.c`,
  144.
- `selfhost/emit/callback_thunk.hero`, 343: the thunk's first line (146) and `hero_signal_learn` in
  `hero_callback_of` (216). `selfhost/emit/callback_guard.hero`, 207.
- `selfhost/cli/supplied.hero`, 357, and the `supplied_*.hero` modules beside it (`ls selfhost/cli |
  grep -c supplied`: 12 files with it): panel 239's member, which asks the link who supplied each name
  the runtime calls (its doc, lines 1-29). `selfhost/cli/link_flags.hero`, 215: the link line's words,
  `msvc(target)` at 155-156.
- `runtime/platform/hero_net.c`, 1235, and `runtime/hero_net.h`, 176 (00-shared.md § Questions 2, 4
  and 5 give the lines). `runtime/parts/run.c`, 2104, and `runtime/hero_os.h`, 513 (§ Question 3).
- `packages/core/0.3.0/files/net.hero`, 288; `net/hero_net.hero`, 173; `net/http.hero`, 150;
  `net/http/internal/server.hero`, 217; `net/http/internal/lines.hero`, 173. `examples/server/main.hero`,
  206.
- `tests/harness/case_setting.hero`, 498; `suite_fetch.hero`, 503; `platform_skips.hero`, 335.

## The cases you run, narrowed to each with the harness's third word

`ls tests/golden/run | grep -E '2494|platform-layer|signal|a-child'` at the base lists, among others:
the 2494 cases (`fixedbugs-2494-...`: 14 programs, `ls tests/golden/run | grep -E
'^fixedbugs-2494-.*\.hero$' | wc -l`, their headers, and one `.case` folder, among them
`a-heroes-function-a-c-handler-calls-is-refused`, `-the-programs-own-sigaction-installs-is-refused`,
`a-signal-the-programs-own-c-holds-with-sigset-stays-pending`,
`the-runtimes-door-counts-each-signal-the-program-sends-itself`); `fixedbugs-2496-...`;
`fixedbugs-2257-...`; `fixedbugs-an-ignored-signal-stays-ignored`; the layer's
(`the-platform-layer-ends-a-call-its-socket-closes`, `-ends-at-a-second-request-to-end`,
`-stops-a-server-on-a-request-to-end`, `-stops-a-server-on-an-interrupt`,
`-hands-a-connection-to-a-thread-as-a-number`, `-runs-the-leak-check-after-a-stop`, and others); and
the spawn's (`a-child-gets-its-environment-at-the-spawn`, `a-child-is-found-through-the-parents-path`,
`every-argument-a-program-hands-a-child-comes-back-whole`, `fixedbugs-436-...`, `fixedbugs-2503-...`).
Run each group your route touches, `./heroes-next run tests/harness/main.hero -- ./heroes-next run
<substring>`, one at a time; a route under `runtime/` also owes the `runtime` suite
(`.claude/rules/verification.md`, the `runtime/**` row), named in your report.

## Question 1: defect 2522 and the runtime defining the C library's installers

1. **Reproduce R3's two cases first, at the base** (00-shared.md § Question 1 names them and their
   build lines): `ce-flib-nofp`, run 20 times on this Mac, each run's exit and output kept (the
   coordinator's one run: exit 0, `answer ran for 30`); `ce-fliblocale`, five storms on this Mac, only
   runs whose output holds `armed` counted, every report read by your binary's name and the storm's
   pids. The Linux arm64 leg is the ffi-pragmatist's, or your own script by the Docker line in
   00-shared.md (`--name ce-248-<n>`, `docker ps` and `docker stop` after).
2. **Build route I far enough to judge it from the runtime's side**: the runtime's definitions of the
   installers (Darwin's ten and glibc's eight linkable, `sigvec`, and `sigignore` and `siginterrupt`
   mirrored, 00-shared.md), the per-thread depth the thunk asks, and the probes on the C library's own
   entry point. Say, by reading each of the 27 installs and the 16 mask calls 00-shared.md lists, which
   would then pass through the runtime's definitions and whether any must not (the stop's handler,
   the stack guard, `run.c`'s forwarding, the probes, and `sigguard.c:139` and `panic.c:54`, inside the
   refusal and panic paths); what moves in the emitted C (likely nothing; say so with a `cmp` of
   `--emit-c` for one 2494 case, the line `hero_program_sources` taken out,
   `.claude/rules/verification.md` § The formatter's probe); and **what panel 239's member does**:
   build a program whose `link` group's library defines `sigaction`, and one whose library defines
   `bsd_signal` (a name the runtime does not call today), and read what `selfhost/cli/supplied.hero`
   tells each at the base and under route I. A widened refusal is the escape's *widened diagnostic*:
   say so in those words.
3. **Under `--sanitize`**: build `ce-fliblocale` and one 2494 case with route I and `--sanitize` at `-O0`
   and `-O2`, and say whether the sanitizer's own `signal` and `sigaction` (dyld substitutions on this
   Mac, weak symbols in the Linux archive, 00-shared.md) and the runtime's definitions link, and which
   one a call reaches.
4. **Route W from the link's side, its premise corrected**: `--wrap` exists on GNU ld, `ld.lld` and lld's
   MinGW driver and on none of Apple's `ld`, `ld64.lld` or `lld-link`, and it wraps the runtime's own
   object's calls as surely as an archive's (00-shared.md, re-run). Say whether the runtime could call
   `__real_<name>` (and what it then needs on a link that does not wrap), or the link be split so the
   runtime's object is linked apart, how `link_flags.hero` would carry either, and what Darwin and an
   MSVC link offer in its place (Apple `ld`'s `-alias` and `-interposable`, `lld-link`'s
   `/alternatename`, each from its own manual or help, or that it offers nothing). Route R, the archives'
   references renamed at build, is W's effect without the flag: say whether the compiler could carry an
   `objcopy` step, where Xcode ships no `llvm-objcopy`.
5. **The other routes from the runtime's side**, each built far enough to price or said why not:
   I-onstack (what giving every claimed thread an alternate stack costs, and what forcing `SA_ONSTACK`
   on a program's C handler changes for it); B (what blocking the asynchronous signals on every
   claimed thread does to the 2494 and 2496 cases and to `run.c`'s forwarding); T (a bounded scan
   against the walk, and the false refusal it risks, built on `sigguard.c`'s bounds).
6. **Cost**: a callback C calls in a loop (panel 247's `qsortcost`,
   `.claude/worktrees/p248-scratch/briefs/ev247/247-compiler-engineer/cases/qsortcost.hero` and `.h`,
   copied, built as `ce-qsortcost`), counted in instructions at the base and under the route you
   recommend, three runs each; and `heroes check selfhost/main.hero` only if your route moves the
   compiler's own code.
7. **Your verdict**: may the runtime define the names, under which rule and with which amendment to
   `.claude/rules/c-boundary.md`'s panel 239 member; and if not, which route closes R3's two cases (20 of
   20 each, armed runs counted, on Darwin and in Linux arm64), saying for each platform what it closes:
   on Darwin a refusal with R1's words before the callback's first C call, on Linux arm64 no Heroes code
   inside the signal.

## Question 2: defect 2502

Build the route you recommend in `runtime/platform/hero_net.c` of your tree: (a) the lane's per-thread
wake (its prototype `.claude/worktrees/b35-scratch/b35-runtime/r2502/hero_net_proto.c`, read-only,
`diff` it against the base's file before you take anything), (b) one shared wake with a generation, (c)
one per socket, or (e) one poller for the process (00-shared.md). Measure with **`waiters-checked.c`**
(copied from `.claude/worktrees/p248-scratch/briefs/r2502/`, built as `ce-waiters`; the original hangs
past the descriptor limit by its own ordering, 00-shared.md): instructions and involuntary context
switches at 0, 5 and 10 s against the base's 271.6 to 275.0 million at 5 s; a run under `ulimit -n 256`
past the limit, and what the layer answers there (`HERO_NET_LIMIT`, a fall-back to slices, or a wait
that never ends). **Then a server-shaped count**: a server process holding N accepted connections whose
peers live in another process, `lsof -p` read at N = 100 and at the largest N your route fits under
`ulimit -n 256`, at the base and under your route. Run the layer's `run` cases named above. Say what
`hero_net.h:95-101` and `136-138` would promise instead, and what your route leaves for the Windows arm
(the ffi-pragmatist compiles it, for MinGW).

## Question 3: defect 2490

Build the door in your tree: its declaration in `runtime/hero_os.h` beside `hero_run_env` (363-377),
its staging and reset in `runtime/parts/run.c`, the POSIX child's `chdir` placed where you say, or
`fchdir` on a folder the parent opened at staging (00-shared.md lists both shapes), read against the
streams' `open`s at 1416-1446 and the `exec` at 1460-1467; and the Windows arm's `lpCurrentDirectory`
(1794-1797), compiled by `zig cc -target x86_64-windows-gnu` (MinGW's; the MSVC build is the CI's). Say
the order the door promises for a relative folder, relative stream paths and a relative program path,
on POSIX and on Windows (where `CreateProcessW`'s page, quoted in 00-shared.md, looks for the program in
the parent's folder). **And the child's existing calls**: POSIX's list (00-shared.md) holds `chdir` and
`fchdir` and not `getrlimit`, `setrlimit` or `execvp`, which the child calls today (1382-1400, 1460):
say whether they stand after a threaded `fork`, and what would replace each if not. Then move the net in
your tree: `case_setting.ran` and `suite_fetch` giving the folder at the spawn instead of
`/usr/bin/env -C` (`case_setting.hero:258`, `suite_fetch.hero:342-343`), `skip_here` and `suite_fetch`'s
skip (`case_setting.hero:151-161`, `suite_fetch.hero:141-152`) no longer naming the folder. Run, on this
Mac, the three `unsupported/fixedbugs-2344-` folder cases (the `unsupported` form narrowed to
`fixedbugs-2344-an-empty`, then `fixedbugs-2344-a-trailing`), the fetch case (`fetch` narrowed to
`fixedbugs-2479`) and the spawn's `run` cases; add a case for the door itself (a child started in a
folder of its own printing `pwd`, and a missing folder, told as you propose); and the net's own tests
(`./heroes-next test tests/harness/main.hero`), since `tests/harness/` moves, saying first whether they
run in your copy, which has no `.git` (00-shared.md § The base). Say which of the four cases would run on
Windows, each read, the run owed; and whether a `heroes` flag that starts the compiler in a folder (the
critic's route for the four cases alone) would be better than the door, against
`.claude/rules/cli-surface.md`.

## Question 4: a stop the program asks itself

Build `hero_net_ask_stop` (or the spelling you propose) in `hero_net.c` and `hero_net.h`, its POSIX and
Windows arms, and say: what it does before `hero_net_catch_stop` and after; what a Ctrl-C after it means
(today any signal after the first ends the process, `hero_net.c:556-577`); how it wakes a waiter on
POSIX (the stop's pipe, made only by `hero_net_catch_stop`) and on Windows (the slices); and how it
meets your answer to question 2. Then a program shaped like `examples/server/main.hero` whose handler
asks the stop at `/quit`, built as `ce-quitserver` in your tree and run under `timeout -s KILL 60`, its
clients `curl --max-time 5` under `timeout -s KILL 90`: it ends at exit 0 with its drain line and the leak
check clean, its output through `head -c 65536`. Say what `<core>/net` would gain (the door, and the
reader `hero_net_stop_asked`, which its binding lacks), in which new version of `core` (a published
version is never edited, `examples/packages/README.md:52-53`), and whether spec § 13's line 404 owes a
sentence (the spec-warden prices the candidates): the blind seat's card for task t4 spells the face as
`net.ask_stop() -> ()?` and `net.stop_asked() -> bool` (`llm-ergonomist.md`), so if your spelling
differs, say it, since the sessions are re-prepared to the built spelling before any runs.

## Question 5: a connection that sends nothing

First measure the shapes 00-shared.md reads and has not run, each bounded as its table says (the server
under `timeout -s KILL 60`, each client under `curl --max-time 5` or a probe's own 10 s limit, the client
side under `timeout -s KILL 90`, outputs through `head -c 65536`): a server built like
`examples/server/main.hero` (copied into your folder as `ce-server`; a `room` smaller than 256 in your
copy is a fair probe if you say so) holding `room` connections whose oldest sends nothing, and **the
narrower shape**, one quiet connection accepted first and `room - 1` that finish after it; in each, a
new client's `GET /hello`: answered, or not, within 10 s. Then build the deadline you recommend (per
call, per socket as Go's `SetReadDeadline`, `net/http`'s idle limit alone, or **a join that does not
wait**, which moves spec line 377 and is the full panel's) in your tree, with the clock you name
(`runtime/platform/hero_time.c` and `the-platform-layer-clock-never-goes-back` are the layer's own
clock), and run the same probes on it: the quiet connection answered `timed_out` (or the kind you
propose) at its deadline, the new client answered. Count what a call with no deadline set pays in
instructions against the base. Say the face in `net` and `net/http` and the core version; the blind
seat's card for task t5 spells it `net.set_deadline(c: Conn, ms: i64) -> ()?` (`llm-ergonomist.md`), so
say if yours differs.

## What your verdict owes

For each question: the route you would adopt, built or not, its lines per file, what moves (the
compiler's own tests, the `run` cases above, the `runtime` suite where `runtime/` moved, the net's own
tests where `tests/harness/` moved, the emissions where `selfhost/emit/` moved), what it refuses or
changes that runs today, its cost in instructions, and every crash report its storms left (armed runs
alone counted); whether it needs a spec sentence, a new or widened diagnostic, or a reader-facing card
change (the escape); and a veto, if you cast one, names the program that corrupts memory or dies in
silence under the route vetoed, built and run.
