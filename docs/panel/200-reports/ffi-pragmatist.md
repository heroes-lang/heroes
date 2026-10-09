# Panel 200, ffi-pragmatist

Copied by the coordinator at 20:32 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

Panel 200, ffi-pragmatist seat. My copy of the frozen tree is in `.claude/worktrees/scratch-b15/200-ffi-pragmatist/`, the running notes in its `notes.txt`. Reported at 20:33. Nothing is left running here, and my folder on the Windows box (`/c/w/p200-ffi-46350`) is removed.

## Q2 (defect 465): ending the program on macOS when its `heroes` is killed with SIGKILL

- **verdict**: approve route A, the sentinel process. Route B, the watcher thread inside the started program, is not enough on its own.
- **section**: design.md §1.12 (a guarantee that ends quietly is not one) and §1.11 (the children are often not Heroes programs). design.md says nothing about a child outliving its runner: grepping for orphan, PDEATHSIG, SIGKILL and outlive found nothing in that sense.
- **experiment**: three routes built in my copy's runtime, compiled with `-Wall -Werror` and the build's `-Werror=shorten-64-to-32 -Werror=sign-conversion` on this Mac (clang 21). The A and B runtimes also compile with `-Wall -Werror` in the Linux arm64 container and on the Windows box; both are switched off on Linux and Windows. The test program loops printing to `/dev/null`; `heroes run` starts it, then `heroes` gets `kill -9`. Before any change: **0 of 5** programs ended, each adopted by launchd.

| | A: sentinel process | B: kqueue watcher thread | C: `heroes run` execs the program |
|---|---|---|---|
| How it works | one process per runner holds a pipe; each child names itself down it before exec | a thread in the program waits on its parent's exit; armed when `HEROES_RUNNER` names its parent; parent 1 at start means end; a second look after registering closes the race | `hero_run_replace` in the runtime, plus `process.replace` and a change in `verbs.hero`; a new `heroes-c` was built from `selfhost/` (exit 0) |
| `kill -9` trials | **10 of 10** after 0.5 s | **4 of 10** after 0.5 s, **10 of 10** after 3 s | **10 of 10** (the program now owns the pid that gets killed) |
| Chain heroes → Heroes runner → heroes → program | 4 of 4 | 4 of 4 | not run |
| A non-Heroes child (`/usr/bin/yes`) | **3 of 3** | **0 of 3** | not run |
| Cost when nothing is killed | +0.95 M instructions once per runner; nothing per launch beyond noise; one extra process (1,088 KB resident, 1 file descriptor) while the runner lives; 0 processes left after a normal exit | +32 KB resident in every program a runner starts (the same in 3 of 3 runs); instructions within ±3 M noise | none: `run hello` 554.1 M instructions against 560.4 M with the fork |

The instruction counts from `time -l` cover only the process it starts, not that process's children (clang's syntax check of the seed read 6.2 s of user time and 130 M instructions). So route A's numbers are the runner's side alone; the sentinel's own instructions are uncounted.

- **argument**: Only A covers what defect 425 needs covered: clang under `heroes build`, `heroes test`'s test binaries, the harness's children, and any non-Heroes program. B misses the `yes` case and leaves the program unguarded between exec and its runtime's start. Under load 16 to 32 that window passed 0.5 s; I traced the cause in `cost/start.log`. B also puts a thread into every program, so a C library's process-wide signals can land on it. My prototype did not block signals on that thread. B also relies on an environment variable that grandchildren inherit. A adds no new kind of fork: `hero_run_go` already forks from a process with threads, and the sentinel only makes async-signal-safe calls. C costs nothing but covers `heroes run` alone.
- **What A still owes**:
  - The guard should be `defined(__APPLE__)`, because `libproc.h` exists only on Darwin.
  - `proc_pidinfo` is not on POSIX's async-signal-safe list.
  - The sentinel can be killed: my first chain test killed one by mistake and left 3 orphans.
  - If a child is reaped by launchd and its pid reused before the sentinel acts, the sentinel's kill could reach the wrong process. A check of the parent and start time would close this; I have not built it.
  - It closes descriptors from a list (`PROC_PIDLISTFDS`) rather than a loop, because `ulimit -n` reads 1,048,576 here.
- **prediction**: under route A, a Heroes program that binds raylib (GLFW and Cocoa initialised) and calls `hero_run_go` starts its sentinel without the Objective-C fork-safety abort, and its child ends when the program is killed with SIGKILL. Not run.
- **condition**: I would move to B if a measurement showed the sentinel's extra process breaking a real binding or caller.
- **Not run**: route D (a pipe read to its end by a thread in the child) and route E (polling `getppid()`), both judged on paper only. D has B's coverage and B's window but no race, because end-of-file stays readable. E wakes the program for its whole life and is dominated by B. Route C's effect on harness callers is also not run: under C, a caller of `heroes run` whose program dies by a signal gets `Ran.signal` set, where before it saw a plain exit of 128 plus the signal. In my run the shell saw exit 134 both ways. C also skips `heroes`'s own leak check on the run path.

## Q1 (defect 453): should `--emit-c` ask clang to check the file before writing it

- **verdict**: approve the check. The way a refusal is told is a reader-facing question for the spec-warden, who is not on this lane.
- **section**: design.md §4.19. Header verification failing at compile time is the property at stake; I could not find a section that covers the single-file artifact.
- **experiment**: a real program, not a hand-edited file, in `q1/`. Two modules bind `a.h` and `b.h`, and each header defines a `static inline twice` with a different type.
  - `heroes run main.hero` prints 6 and 8 and exits 0.
  - `heroes build main.hero --emit-c -o main.c` exits 0.
  - `clang -fsyntax-only` on `main.c`, with `flags()`, `-O0 -g`, `-I runtime -I .`, exits 1 with 3 errors:
    - conflicting types for `twice` at `b.h:1`;
    - the artifact's own `_Static_assert(HERO_RET_I32(twice(...)))` fails, because it is now checked against `a.h`'s `long` version;
    - `-Wshorten-64-to-32` at `right.hero:5`.
- **Cost**, counted with `-fintegrated-cc1` so the count includes the compile itself:

| what clang checks | instructions | peak memory |
|---|---|---|
| a hello artifact | 0.208 B | 28 MB |
| the case above | 0.215 B | |
| `seed/heroes.c` at `-O0 -g` | 26.78 B | 379 MB |
| `seed/heroes.c` at `-O2 -gline-tables-only` | 26.68 B | |

For comparison, `heroes build hello` costs 0.54 B instructions, counting `heroes` alone. The seed gate compiles `seed/heroes.c` anyway, so on that path the check is paid twice.
- **argument**: An artifact nobody compiles carries header checks nobody evaluated, and here one is false. The check must use the build's own words:
  - `flags()` plus `level_words` (the optimisation level and sanitizers change what system headers expand to);
  - the runtime's `-I`, the program's `--include` folders and its own folder;
  - each package's `pkg-config --cflags`, without which clang refuses a header the build found.

  This refusal belongs to none of the six classes in `.claude/rules/c-boundary.md`: each header compiles fine on its own. Blaming it as exit 2 would tell the author the compiler is wrong when nothing in their program is. It needs a message naming both `extern` groups, which is a new diagnostic class and needs a sitting.
- **prediction**: on Windows, a program with one module binding `raylib.h` and another binding `windows.h` builds module by module, and clang refuses its `--emit-c` artifact (the `Rectangle`, `CloseWindow` and `ShowCursor` clash). Not run.
- **condition**: I would change my view if the critic's alternative (compiling the whole-program file as the build's single unit) were measured as cheaper than the separate check.
- **Not run**: I did not read whether the exit-code contract in `.claude/rules/cli-surface.md` allows `--emit-c` to start refusing.

## Q4 (defect 472): the debugger's view of a `step`

**Not run.** The compiler-engineer's variant did not reach me. The Windows box answered at 20:13; I used it only to compile the Q2 runtimes.

Files are in `.claude/worktrees/scratch-b15/200-ffi-pragmatist/`:
- `run.c.A` (route A), `run.c.B` with `os.c.B` (route B), `run.c.orig` and `os.c.orig` (the unchanged files)
- route C: `tree/runtime/parts/run.c`, `tree/runtime/hero_os.h`, `tree/selfhost/cli/process.hero`, `tree/selfhost/cli/verbs.hero`
- `probe/` (`spin.hero`, `outer.hero`, `outer2.hero`, `trial.sh`, `nested.sh`), `cost/` (`runner.c`, `runner2.c`, `execdemo.c`, `start.log`), `q1/` (the Q1 case)
