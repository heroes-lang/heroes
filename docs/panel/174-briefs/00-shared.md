# Panel 174 — shared brief: the harness's two redirect files, and the process that will not let go

**Lane: SOUNDNESS** (`compiler-engineer`, `ffi-pragmatist`). No surface, no
diagnostic class and no spec token is touched: the subject is
`runtime/parts/run.c`'s process runner and `tests/harness/shell.hero`, and no
program in `examples/` or `tests/golden/` declares `hero_run_go`. If a seat
thinks the full panel is owed, say so in its report and the coordinator will
convene it.

**Every number below was produced by a command run 2026-09-21 while this brief
was being written, and the command is named beside it.** Where a sentence could
not be run it says so in those words.

---

## 1. What happened

GitHub Actions run **35620846459**, the push carrying M-declared-extents step
32. The **Windows x86-64** leg read `harness: 1803 passed, 106 failed`; Darwin
arm64, Linux x86-64 and Linux arm64 each read 0 failed.
(`gh api repos/heroes-lang/heroes/actions/jobs/106403218648/logs`)

- `run`: **35 passed, 101 failed**
- `annotations`: **169 passed, 5 failed**
- the other 22 suites: **0 failed**

**The re-run of that same commit, Windows alone, was GREEN**: run
**35627328143**, `harness: 1903 passed, 0 failed`
(`gh workflow run ci.yml -f only=windows`, then its job log). So the defect is
**intermittent**. The author's own Windows box runs the `run` suite at
**130 passed, 0 failed** on the same HEAD (ssh, measured twice).

Every one of the 101 failures read exactly `did not build (exit -1):` with an
**empty stderr**. That `-1` is `tests/harness/shell.hero`'s sentinel beside
`started: false`, i.e. *the process never started* — it is not an exit code any
process produced. The instrument that reported it has since been repaired
(commit `497a048f`); **this sitting is about the failure, not about the
message.**

## 2. The shape of the failure, measured from the log

- The 101 `run` failures are **exactly cases 36 to 136** of
  `tests/golden/run/` in walking order. Command:
  `grep -o "FAIL run/[a-z0-9-]*" win.txt | sed 's|FAIL run/||' | sort > a;
   ls tests/golden/run/*.hero | sed 's/.*\///;s/\.hero$//' | sort | tail -n +36 > b;
   diff a b` — **prints nothing**. `ls tests/golden/run/*.hero | wc -l` is
  **136**, and 35 + 101 = 136.
- The 5 `annotations` failures are **exactly the first 5** of the 10 groups
  that suite reports on.
- **No suite runs between those two** (`tests/harness/main.hero`, the call
  order).

So one window of wall-clock time opened inside `run` and closed inside
`annotations`. **The cause has a LIFETIME, not a location.**

Case 35 (`c-frees-a-lease-and-the-runtime-names-it`) passed; case 36
(`c-frees-a-lease-on-a-later-call`) is the first casualty. Cases 35, 36 and 37
are the only three in the tree carrying `!sanitizer:`, the only three whose
programs die by heap corruption rather than by an ordered `abort()`. Eighteen
`abort-*` cases sit inside the first 35 without opening anything.

## 3. THE MECHANISM, measured on Windows tonight

`runtime/parts/run.c:347,351` open the two redirect files as

```c
CreateFileA(out_path, GENERIC_WRITE, FILE_SHARE_READ, &inherit,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
```

with `inherit.bInheritHandle = TRUE`, and `CreateProcessA(..., TRUE, ...)` at
`:427` passes them to the child.

A probe compiled and run on the author's Windows box
(`/c/w/shareprobe.c`, `clang -o shareprobe.exe shareprobe.c; ./shareprobe.exe`)
answered:

```
ERROR_SHARING_VIOLATION is 32, ERROR_ACCESS_DENIED is 5

narrow   second open REFUSED while the first is held, error 32
wide     second open ok while the first is held
```

where `narrow` is `FILE_SHARE_READ` and `wide` is
`FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE`.

**So: while ANY process holds one of those two handles, the next
`CreateFileA` fails with 32, `hero_run_go` sets `HERO_OS_FAILED` and returns
-1, and the caller sees `started: false` with an empty stderr.** That is the
observed signature exactly.

**`hero_run_go` reuses the SAME two paths for every call.**
`tests/harness/shell.hero:206-207`:

```heroes
out_path = scratch + "/stdout"
err_path = scratch + "/stderr"
```

and `tests/harness/main.hero:123` builds ONE scratch for the whole net
(`scratch = SCRATCH + "-" + shell.pid().to_str()`), handed to every suite. So a
single holder blocks **every suite** until it lets go. Measured call sites of
`shell.run` / `run_fed` / `run_guarded`: **81**
(`grep -rc 'shell\.run(\|shell\.run_fed(\|shell\.run_guarded(' tests/harness/*.hero`).

## 4. WHO HOLDS IT — already recorded in this repository

`tests/harness/shell.hero:414-417`, written when Windows had no watchdog:

> *"two orphan `heroes.exe` and a `clang.exe` held `build/harness/stdout` open,
> and Windows refuses to delete a file a handle still holds, so the next run
> could not clear its own build directory."*

The same paragraph is repeated at `:741-744` inside the watchdog's own test.

**This is the only named holder anybody has measured, and it is the harness's
own orphaned descendants.** It is not WerFault and not an antivirus.

Corroboration from tonight, unplanned: stopping the local `ssh` client did NOT
kill the remote process. `heroes.exe` PID 5124 went on running the `run` suite
for twenty minutes after its session closed, with `main.exe` and a grandchild
under it (`Get-CimInstance Win32_Process`, which printed their command lines
and parent ids).

**What is NOT measured**, and goes out as a question rather than a premise:
*what creates the orphan on the GitHub runner.* A probe run on the author's box
tonight (`/c/w/windowprobe.c`) reproduced the harness's own loop — open the two
files narrow and inheritable, spawn the program, wait, close our handles, then
try to reopen once every 250 ms for 30 s — against all three heap-corrupting
cases and one ordinary aborting case:

```
c-frees-a-lease-and-the-runtime-names-it   exited 0xC0000374   reopened at once
c-frees-a-lease-on-a-later-call            exited 0xC0000374   reopened at once
c-frees-a-lease-through-a-callback         exited 0xC0000374   reopened at once
abort-array-index                          exited 0xC0000409   reopened at once
```

`0xC0000374` is `STATUS_HEAP_CORRUPTION`, `0xC0000409` is
`STATUS_STACK_BUFFER_OVERRUN`. **Nothing lingers on that box.** So the crash
itself is not sufficient; something about the runner is, and nobody has seen it.

## 5. The watchdog, and what `TerminateProcess` does not do

`runtime/parts/run.c:441-452`: on `WAIT_TIMEOUT` the arm calls
`TerminateProcess(child.hProcess, 124)`, waits 5 s and closes the handles.
`WATCHDOG_SECONDS` is **120** (`tests/harness/shell.hero`, the constant and its
`assert`).

**`TerminateProcess` kills one process and not its descendants** — Microsoft's
documented behaviour; a seat should confirm it rather than take it from here.
A terminated `heroes.exe` that had spawned `clang.exe` therefore leaves the
clang running, **holding the inherited redirect handle.** That is a route from
the watchdog to the recorded holder, and it is **unrun**: nobody has shown the
CI run hit the watchdog.

## 6. The routes on the table, and the trap in the obvious one

**A — widen the share mode** to `FILE_SHARE_READ | FILE_SHARE_WRITE |
FILE_SHARE_DELETE` on the three redirect opens. Measured tonight to make the
second open succeed.

> **THE TRAP, and it is why this sitting exists.** The holder's handle is one
> WE created and the child inherited. With wide sharing the reopen succeeds and
> the file is truncated by `CREATE_ALWAYS` — while the orphan still holds a
> handle with its own file pointer. **If the orphan then writes, its bytes land
> in the file the harness is capturing a DIFFERENT case into.** A loud failure
> becomes a possibly silent wrong answer, which CLAUDE.md § Precedence rank 3
> forbids and which panel 173 refused in another form four days ago. **Unrun**:
> nobody has demonstrated the interleaving.

**B — one redirect path per spawn**, e.g. a counter in the name, removed after
the streams are read. An orphan then holds a file nobody will reuse, whoever
the orphan is. Costs: file churn under the scratch; a removal per call; and
`tests/harness/suite_determinism.hero:149-154`'s `last_error` reads
`scratch + "/stderr"` by name and would have to be told the path (measured: it
is the ONLY dependency outside `shell.hero` —
`grep -rn 'scratch + "/stdout"\|scratch + "/stderr"' tests/harness/*.hero`
returns three lines, two of them `shell.hero:206-207`).

**C — kill the process TREE**: a Windows Job Object with
`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`, and a POSIX process group with
`killpg`. Closes the orphan at its source rather than tolerating it. Costs: new
Win32 surface in the runtime; the POSIX half changes `hero_run_child`.

**D — stop the grandchild inheriting**: `STARTUPINFOEX` with
`PROC_THREAD_ATTRIBUTE_HANDLE_LIST`. A seat should say whether this limits only
the DIRECT child's inheritance, in which case it does not reach the recorded
holder (`clang.exe` spawned by `heroes.exe`).

**E — bounded retry** on 32 / 5 / 33 with backoff. Cheap, fires only on
failure. It tolerates rather than removes, and it needs a policy nobody has
measured.

## 7. What the sitting must answer

1. Which route, or combination, and **is A safe alone** given the trap in § 6?
2. Does B's per-call path have a failure mode nobody listed — path length on
   Windows (MAX_PATH), the cost of a removal per spawn across the net's
   thousands of calls, a suite that globs the scratch?
3. Is C owed regardless, because an orphan left running is a second defect
   (it wastes the runner and it is what § 4's comment describes)?
4. **What would have to be true for a route nobody listed to exist?**

## 8. Rules that bind every seat

- **Build in a COPY.** `cp -r` the tree to your scratchpad and `rm -rf target
  build` there. Never touch `/Users/joseph/Temp/heroes/heroes-lang`.
- **The author's Windows box is ON** and reachable as `ssh win`, repository at
  `/c/w/heroes`, clang 23.1.1. **It is shared**: before running anything on it,
  check `tasklist | grep -icE "heroes.exe|main.exe"` is 0, and kill nothing you
  did not start. A suite reading a tree owns that tree until it exits (CL-025);
  two of tonight's measurements were spoiled by ignoring this.
- **Every number in your report comes from a command you ran**, and you name
  the command. An inference is not a measurement. A negative claim goes out as
  a question naming what you searched for.
- Darwin is this Mac; Linux is `docker run --rm -v "$PWD":/src:ro heroes-linux`
  (`docs/ref/environment/linux/LINUX-MACHINE.md`).
