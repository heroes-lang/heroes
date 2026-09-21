# Panel 174 — report: ffi-pragmatist

Every number below comes from a command run in this sitting, on 2026-09-21, and
the command is named beside it. Windows numbers are from the author's box
(`ssh win`, clang 23.1.1, `Microsoft Windows [Version 10.0.26100.32690]` from
`cmd /c ver`), which was checked idle before each run
(`tasklist | grep -icE "p174|e2e|heroes.exe|main.exe"` read **0** every time, and
**0** after). Darwin is this Mac (Apple clang 21.0.0). Linux is
`docker run --rm -v "$PWD":/src:ro heroes-linux` (Debian clang 22.1.8). The
repository at `/Users/joseph/Temp/heroes/heroes-lang` was not written to except
for this one file; all building was done in a copy.

---

## verdict

**object** to route A as stated, **veto** its `FILE_SHARE_WRITE` bit, and adopt
**C + B + the `FILE_SHARE_DELETE` bit alone**.

Stated so it can be scored: *adding `FILE_SHARE_WRITE` to the two redirect opens
lets an orphaned grandchild's bytes land inside a later case's captured stdout,
measured, on all three platforms; `FILE_SHARE_DELETE` without it buys the whole
of the scratch-cleanup half at zero soundness cost; and a Job Object on Windows
with a guarded process group on POSIX removes the orphan itself, so neither
share bit has to carry the repair.*

## section

**design.md §1.12**, at :576-580 in the quoted rule: *"a guarantee that ends
quietly is not one"*, and CLAUDE.md § Precedence rank 3, which puts robustness
above ergonomics, token cost and compiler size. §1.11 is the reason this
section reaches here at all: with no standard library, `hero_run_go` is the
runtime's own FFI into the operating system, and it is the call every `heroes
build` makes to reach clang.

**Where design.md does not cover my objection, and I say so explicitly**: the
document has no rule about process lifetime, handle inheritance, or what a
runtime owes a descendant it did not create. §4.20 (the runtime, in C) does not
mention it and neither does Part 9. The nearest binding text is §1.12's
*complete and defended boundary*, which is an argument by extension rather than
a citation, and the panel should treat the lifetime rule as **new** rather than
as already decided.

---

## question 1 — the interleaving trap. It fires.

Program: `p174-interleave.c` (Windows) and `p174-posix-interleave.c` (Darwin,
Linux). Three processes, the harness's own shape: a parent opens the redirect
file inheritable and spawns `mid`; `mid` spawns `grand` with
`bInheritHandles = TRUE` and exits at once, which is `heroes.exe` handing its
stdout to `clang.exe` and dying; the parent waits for `mid` only, closes its own
handles exactly where `hero_run_go` closes them, and then reopens the same path
with `CREATE_ALWAYS` for the next case and writes that case's output into it.
`grand` writes marked lines by `WriteFile` straight to the inherited handle, so
no CRT buffer can be blamed.

Commands: `clang -Wall -Wextra -o p174-interleave.exe p174-interleave.c` then
`./p174-interleave.exe` on Windows; `clang -Wall -Wextra` then the binary on
Darwin; the same source compiled and run inside `heroes-linux`. clang accepted
all three.

Windows, both share modes in one run:

```
[narrow  FILE_SHARE_READ share mode]
  the reopen was REFUSED, error 32 - the loud failure
  (that is defect 074's signature: started:false, empty stderr)

[wide    READ|WRITE|DELETE share mode]
  the reopen SUCCEEDED while the orphan still holds the file
  the parent captured 120 bytes
  --- what a caller would read as case B's stdout ---
case-B-real-output\n...............................ORPHAN-05\nORPHAN-06\nORPHAN-07\nORPHAN-08\nORPHAN-09\nORPHAN-10\nORPHAN-11\n
  --- end ---
  bytes from the orphan present: YES
  NUL bytes in the capture: 31
```

Darwin and Linux print the **same 120 bytes, the same 31 NUL bytes and the same
`ORPHAN-05` through `ORPHAN-11`**, with no share mode involved at all, because
POSIX has none.

**What it settles, three things.**

1. Route A's widened mode does not merely permit the reopen. It permits a
   truncating reopen to run underneath a live writer whose file pointer is
   still at the old offset, so the orphan's next write lands past the parent's
   own bytes and the gap is zero-filled. A golden case would be diffed against
   `case-B-real-output` followed by 31 NULs and another program's output. That
   is a silent wrong answer, which CLAUDE.md § Precedence rank 3 refuses.
   **Route A is not safe alone. This is the veto.**
2. The trap is **not new**. It is the POSIX arm's behaviour today, on Darwin and
   on both Linux legs, byte for byte. So route A does not invent a class, it
   imports one that already exists on three of the four platforms this project
   ships and that nobody has looked for. That is an argument for route B
   everywhere rather than for tolerating A on Windows.
3. The narrow arm reproduces defect 074's signature exactly: `error 32`.

### and the trap belongs to one BIT, which route A bundles with another

Program: `p174-sharebits.c`, same orphan grandchild, three share modes, asking
each the two questions that matter. Command:
`clang -Wall -Wextra -o p174-sharebits.exe p174-sharebits.c; ./p174-sharebits.exe`.

```
[READ (today)            ]
  reopen GENERIC_WRITE CREATE_ALWAYS: REFUSED, error 32 -> loud, the trap cannot fire
  DeleteFile on the held file:         REFUSED, error 32
  RemoveDirectory on the scratch:      REFUSED, error 145

[READ|DELETE             ]
  reopen GENERIC_WRITE CREATE_ALWAYS: REFUSED, error 32 -> loud, the trap cannot fire
  DeleteFile on the held file:         ok
  RemoveDirectory on the scratch:      ok

[READ|WRITE|DELETE (A)   ]
  reopen GENERIC_WRITE CREATE_ALWAYS: ok -> captured 50 bytes, orphan bytes present: YES - THE TRAP
  DeleteFile on the held file:         ok
  RemoveDirectory on the scratch:      ok
```

`FILE_SHARE_WRITE` is the bit that creates the trap. `FILE_SHARE_DELETE` is the
bit that buys the second half of `tests/harness/shell.hero:414-417`'s recorded
symptom, *"Windows refuses to delete a file a handle still holds, so the next
run could not clear its own build directory"* - error 145 is
`ERROR_DIR_NOT_EMPTY` and it is that sentence, measured. The two bits are
separable, so the project can have the cleanup without the trap. Route A as
written in the shared brief takes both, and that is what I object to.

---

## question 2 — TerminateProcess and grandchildren. Measured, not read.

Program: `p174-tree.c`, in `terminate` mode. A parent spawns `mid`; `mid` spawns
`grand`, writes `grand`'s pid to a file, then hangs so the watchdog is the thing
that fires; the parent calls `TerminateProcess(mid, 124)`, which is
`runtime/parts/run.c:443` verbatim. Liveness is asked twice, because a pid can
be reused: `OpenProcess` plus `GetExitCodeProcess` on `grand`'s own pid, and
whether the heartbeat file `grand` appends to grows across 2000 ms.

Command: `clang -Wall -Wextra -o p174-tree.exe p174-tree.c; ./p174-tree.exe`.

```
=== today: TerminateProcess on the direct child alone ===
  [before] grand's pid is 1296
  [before] GetExitCodeProcess reads 259 (STILL_ACTIVE - grand is ALIVE)
  [before] heartbeat 100 -> 180 bytes over 2000 ms: STILL WRITING
  TerminateProcess(mid, 124), exactly run.c:443
  [after] grand's pid is 1296
  [after] GetExitCodeProcess reads 259 (STILL_ACTIVE - grand is ALIVE)
  [after] heartbeat 180 -> 260 bytes over 2000 ms: STILL WRITING
  narrow reopen REFUSED, error 32 - the next spawn fails
```

**The coordinator's second item, answered directly: yes, the harness's own
watchdog can create such an orphan, and this is the whole chain in one run.**
`TerminateProcess` kills one process and leaves its child running with the
inherited redirect handle, and the very next `CreateFileA` answers 32. That is
GitHub job 106504808513's line, *"the operating system's own reason is 32"*,
reproduced from the watchdog end.

I also read the documented behaviour, and it agrees: Microsoft's
`TerminateProcess` page states *"TerminateProcess is asynchronous... This
function stops execution of all threads within the process"* and says nothing
about descendants, while the `CreateJobObject` page is where the
kill-a-tree guidance lives. **UNRUN, in those words**: I did not verify the
exact current wording of either page in this session, so treat the citation as
a pointer and the measurement above as the evidence.

The watchdog is **not** the only route, and the sitting should not narrow to it.
`WATCHDOG_SECONDS` is 120 and nobody has shown CI hit it. The `sweep` arm below
shows the same orphan surviving a child that exited **normally**, which is what
`shell.hero:414-417` actually describes.

---

## question 3 — route C's Windows half, written, compiled, and the grandchild dying

Same `p174-tree.c`, `job` mode: `CreateJobObjectA`, `SetInformationJobObject`
with `JobObjectExtendedLimitInformation` and
`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`, `CreateProcessA` with `CREATE_SUSPENDED`,
`AssignProcessToJobObject` while the child cannot yet have spawned anything,
`ResumeThread`, and `TerminateJobObject` at the end.

```
=== route C: a Job Object with KILL_ON_JOB_CLOSE ===
  this process is already inside a job: no
  job created with JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
  mid assigned to the job while suspended
  [before] GetExitCodeProcess reads 259 (STILL_ACTIVE - grand is ALIVE)
  [before] heartbeat 100 -> 180 bytes over 2000 ms: STILL WRITING
  is GRAND in our job? YES
  TerminateJobObject(job, 124)
  [after] OpenProcess refused, error 87  -> no such process
  [after] heartbeat 180 -> 180 bytes over 2000 ms: stopped
  narrow reopen ok - nothing holds the redirect file
```

`grand` was **never named**: `IsProcessInJob` reports it inside our job because
a process created by a process in a job joins that job by default. That is the
property route D does not have. The narrow reopen then succeeds with the share
mode untouched, which is the point: **route C repairs defect 074 without
widening anything.**

**The arm that decides route C on CI**, because the author's box is not where
the defect lives. `p174-tree.exe job-nested` puts this process into an outer job
first, which is the shape a GitHub Actions runner imposes, and then does the
same thing:

```
=== route C, with this process ALREADY in a job: YES ===
  this process is already inside a job: YES
  mid assigned to the job while suspended
  is GRAND in our job? YES
  TerminateJobObject(job, 124)
  [after] OpenProcess refused, error 87  -> no such process
  [after] heartbeat 180 -> 180 bytes over 2000 ms: stopped
  narrow reopen ok - nothing holds the redirect file
```

Nesting is accepted. Had it not been, route C would have worked on the author's
machine and failed on the only machine where defect 074 has ever been seen, and
nobody would have found out until the next red run.

**Cost, one job per call and not one for the process.** One for the process
would kill every child at once, which is not what a synchronous "run this and
give me its exit code" may do. Priced with `p174-jobcost.c`, 200 spawn-and-wait
cycles each way, six rounds: `+1.5%`, `+27.6%`, `+39.1%`, `-0.2%`, `+19.9%`,
`+5.3%` against a plain spawn of 2.7 to 3.1 ms. **That instrument cannot settle
it** and I will not pretend otherwise: the spread crosses zero, so the four
Win32 calls cost less than `CreateProcess`'s own run-to-run variance on this
machine. What would settle it is the net's wall time before and after on a quiet
box, which is a milestone's measurement and not a panel's.

---

## question 4 — route D does not reach the grandchild, and its only escape is worse

Program: `p174-handlelist.c`. `STARTUPINFOEXA`,
`InitializeProcThreadAttributeList`, `UpdateProcThreadAttribute` with
`PROC_THREAD_ATTRIBUTE_HANDLE_LIST`, `CreateProcessA` with
`EXTENDED_STARTUPINFO_PRESENT`. `mid` reports, into a file it opens by name so
the report survives whatever happened to the handle, what its stdout actually
is.

```
[arm 1: the redirect handle IS in the list, as it must be]
  handle list carries 1 handle(s); the redirect handle is IN it
  CreateProcess ok
  narrow reopen REFUSED, error 32 - the GRANDCHILD holds it
  <p174-hl-mid.txt>
[mid] GetHandleInformation: ok (error 0), HANDLE_FLAG_INHERIT SET
[mid] GetFileType: 1 (error 0)   [1=DISK]
[mid] WriteFile to stdout: ok, 15 bytes, error 0

[arm 2: the redirect handle is NOT in the list]
  CreateProcess ok
  narrow reopen ok - nothing holds it
  <p174-hl-mid.txt>
[mid] GetHandleInformation: FAILED (error 6), HANDLE_FLAG_INHERIT clear
[mid] GetFileType: 0 (error 6)   [0=UNKNOWN]
[mid] WriteFile to stdout: FAILED, 0 bytes, error 6
  <p174-hl.txt> 0 bytes
```

**No.** Arm 1 is the only configuration the harness can use, because the child's
stdout **is** the redirect handle, and the mechanism is visible in the child's
own report: the handle arrives with `HANDLE_FLAG_INHERIT` **SET**, so it is
inheritable again in the child's table and the child's own
`CreateProcessA(..., TRUE, ...)` hands it straight to `clang.exe`. The
restriction governs one generation and does not travel.

Arm 2 is worse than useless and this is the finding I did not expect.
Excluding the redirect handle does **not** make `CreateProcess` fail. It
succeeds, the child's stdout is a dangling handle value, every write answers
error 6 (`ERROR_INVALID_HANDLE`), and the harness captures **0 bytes** with no
diagnostic anywhere. A suite comparing against an empty stream would report
whatever an empty stream reports. **Route D is refused on both arms**: one does
not reach the holder, the other manufactures silent empty captures.

---

## question 5 — the POSIX half must not regress, and the naive form does

Program: `p174-posix-tree.c`, three arms, Darwin and Linux from one source.

```
[today: no group, SIGKILL the direct child]
  [after] heartbeat 120 -> 180 over 1500 ms: STILL WRITING

[route C: setpgid in the child, SIGKILL the group]
  [after] heartbeat 120 -> 120 over 1500 ms: stopped

[route C, the case that matters: the child EXITS, the group is swept]
  the child exited (code 0) and is still a zombie, so its pid is pinned
  sweeping the group BEFORE reaping it
  [after] heartbeat 120 -> 120 over 1500 ms: stopped
```

Identical on Linux (`kill(pid,0)` still succeeds there only because PID 1 in the
container is not reaping; the heartbeat is the signal that survives that, which
is why the probe asks twice).

**Two things this arm produced that the shared brief did not list.**

**(a) `waitid` with `WNOWAIT`, and it is a soundness matter rather than a
nicety.** The sweep on the ordinary exit is `kill(-pgid, SIGKILL)`, and the
group id is the child's pid. Reap the child first and the pid is released, so
the runtime would be signalling a group number the kernel may already have
handed to somebody else. `waitid(P_PID, child, &info, WEXITED | WNOWAIT)`
reports the exit without reaping, the zombie pins the pid, the group dies, and
only then does `waitpid` reap. Measured working on Darwin and Linux.

**(b) the naive `setpgid(0, 0)` breaks `heroes run` on a program that reads the
keyboard, and I found it by attacking the shape beside the repair (CLAUDE.md
§ RUN IT).** Program `p174-posix-tty.c`, under a pty
(`script -q /dev/null ./p174-posix-tty` on Darwin, `script -qec ./tty /dev/null`
in the container):

```
[control: no setpgid, the runtime as it stands]   the child exited, code 3
[route C naive: setpgid(0,0) always]              the child is STOPPED by signal 21
                                                  (Stopped (tty input)) - it will never
                                                  exit, and the caller hangs until the watchdog
[route C guarded: stdin inherited AND a tty]      the guard refuses a new group; the child
                                                  blocks in read, which is correct
[route C guarded: stdin redirected to a file]     the guard ALLOWS a new group; exited, code 0
```

Same four lines on Darwin and on Linux. A process in a background group that
reads an inherited terminal takes SIGTTIN and stops for ever, and
`hero_run_go` leaves stdin inherited whenever `in_path` is empty. So the POSIX
half must be **guarded**, and the guard asks the value rather than the world
(CLAUDE.md §11):

```c
static int hero_run_group_is_safe(const char *in_path) {
    if (!hero_run_inherits(in_path)) return 1;   /* stdin is a file */
    return !isatty(0);
}
```

The honest cost, named rather than hidden: `heroes run` on a program reading the
terminal keeps today's behaviour **and today's orphan risk**. The harness never
takes that branch, because it is not a terminal on CI and it redirects stdin for
`run_fed`.

---

## the proposal, compiled and run against the real `hero_run_go`

The patch is `+69 code lines, -10` in `runtime/parts/run.c` (the rest of the
diff is comment). It is three things and no more:

1. `FILE_SHARE_READ | FILE_SHARE_DELETE` on the three redirect opens at
   `:347`, `:351`, `:381`. **`FILE_SHARE_WRITE` is not added.**
2. Windows: a per-call Job Object with `KILL_ON_JOB_CLOSE`, assigned while the
   child is suspended, terminated on the timeout **and on the ordinary exit**.
3. POSIX: the guarded `setpgid`, `kill(-pgid, SIGKILL)` on the timeout, and the
   `waitid(WNOWAIT)` sweep on the ordinary exit.

The diff is at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/p174-run.c.diff`
and the whole patched file beside it as `p174-run.c.patched`.

**Built, with the documented lines:**

| platform | command | result |
|---|---|---|
| Darwin arm64 | `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-p174` | built |
| Linux x86-64 | the same line inside `docker run --rm -v "$PWD":/src:ro heroes-linux` | built |
| Windows x86-64 | `clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes-p174.exe` | built, 3 warnings, all pre-existing `getenv` deprecations from the MSVC CRT |

`clang -Wall -Wextra -I runtime -c runtime/runtime.c` on Windows emits **0**
warnings whose text names `parts/run.c`.

**Works, not merely compiles.** On Darwin the patched compiler compiled itself,
`./heroes-p174 build selfhost/main.hero -o heroes-next-p174`, `real 77.21 user
71.55 sys 3.91` (the ratio is healthy, so the number stands), and
`./heroes-p174 test selfhost/main.hero` read **675 tests, all passed**. On
Windows `./heroes-p174.exe run p174-hello.hero` and
`build ... && ./p174-hello.exe` both printed their line: every clang call in
those runs went through the job object.

**And the defect itself, at the API rather than at a probe.** `p174-endtoend.c`
links the real `runtime/runtime.c` and calls the real `hero_run_go` twice over
one pair of paths, exactly as `tests/harness/shell.hero` does at its 81 call
sites, running a program that leaves a grandchild behind. Built both ways on the
same machine, with 40 s between the runs so the first run's orphan could not
spoil the second (it did spoil my first attempt, which is the reason that line
is here):

```
########## PRISTINE runtime (windows) ##########
  first call     exit 0, status 0, why 0
  the grandchild's heartbeat -1 -> 30 over 1500 ms: STILL WRITING - it outlived the call
  second call    exit -1, status 2, why 32
  VERDICT: second call FAILED - this is defect 074

########## PATCHED, FINAL (windows) ##########
  first call     exit 0, status 0, why 0
  the grandchild's heartbeat -1 -> -1 over 1500 ms: stopped - the call took it with it
  second call    exit 0, status 0, why 0
  VERDICT: second call ran normally
```

`status 2` is `HERO_OS_FAILED` and `why 32` is `ERROR_SHARING_VIOLATION`. That
is GitHub job 106504808513's 120 identical lines, reproduced through the
runtime's own entry point and then removed by the patch. On Darwin the same test
shows the orphan surviving the pristine call and dying under the patched one,
with both second calls succeeding, because POSIX never refuses.

---

## why B as well, when C already repairs it

Because C rests on a premise about **who** the holder is, and the shared brief
says in its own words that nobody has measured what creates the orphan on the
runner. C closes every holder that descends from a `hero_run_go` child, which is
the only holder anybody has named (`shell.hero:414-417`). A holder that is not a
descendant - a crash reporter, a scanner, a process from outside the tree - is
untouched by C and untouched by the DELETE bit, and is still refused loudly by
the narrow share mode. **B removes the class regardless of the holder's
identity**, because a path nobody reuses cannot be contended. CLAUDE.md
§ Precedence and § 4 both say take the most robust resolution and never the
compromise, so the answer is both.

**Route B's own exposure, measured, because the brief asked (question 2 of
§ 7).** `hero_run_go` opens with `CreateFileA`, the **ANSI** entry point, whose
limit is `MAX_PATH` and is not lifted by the long-path registry switch without a
manifest. `p174-pathlen.c` walks a directory chain until it breaks:

```
MAX_PATH is 260
at 256 characters the file opened, but CreateDirectoryA for the next level answered 3
the last file length accepted was 256
```

`tests/harness/main.hero:98` makes the scratch `build/harness` plus `-` plus the
pid, so the absolute path of `<scratch>/stdout-0042` on a runner is on the order
of 50 characters against a measured cliff of 256. **Route B has roughly 200
characters of headroom and no MAX_PATH problem.** It is a Heroes-side change in
`shell.hero` and does not touch the C boundary, so my seat has no veto over it
and no objection to it.

---

## argument (116 words)

Widening the share mode buys the reopen by permitting a truncating open under a
live writer. I compiled the three-process case and ran it: the harness captures
its own case's line, 31 NUL bytes, then seven lines of another program's output,
identically on Windows, Darwin and Linux. A loud `error 32` becomes a silent
wrong diff, which §1.12 refuses. The two bits are separable: `READ|DELETE`
refuses the reopen and still lets the scratch be cleaned. Meanwhile
`TerminateProcess` leaves the grandchild `STILL_ACTIVE` with the handle, and a
Job Object with `KILL_ON_JOB_CLOSE` takes it, nested inside an existing job as
CI requires. `PROC_THREAD_ATTRIBUTE_HANDLE_LIST` cannot reach a grandchild and
its only escape produces empty captures.

## prediction

Scoreable by the milestone that lands this:

1. With **C plus the DELETE bit alone**, and the redirect paths left shared, a
   Windows CI leg on the current HEAD reads **0 failures carrying
   `the operating system's own reason is 32`**. Falsified by one such line.
2. With **route A as the shared brief states it** (all three bits, no C, no B),
   a Windows leg goes green on the 32s and a `tests/golden/run/` case adjacent
   to one of the three `!sanitizer:` cases eventually fails a **content** diff
   whose actual output contains NUL bytes. That is the trade the veto refuses.
3. The `waitid(WNOWAIT)` sweep costs **nothing measurable** on the POSIX legs:
   the net's wall time on Darwin moves by less than its own run-to-run spread.
4. `examples/ledger/db/sqlite.hero` and every other binding in the tree is
   **unchanged**: this touches no `extern`, no `ptr`, no `cstr`, no layout and
   no calling convention, so `importc`-style header verification is untouched.
   Prediction 4 is what my veto exists to protect and it is the cheapest to
   score: `git diff --stat` on the landing commit names no file under
   `selfhost/emit/` or `examples/`.

## condition

What would change my verdict, each with the command:

- **The veto lifts** if somebody shows the interleave cannot occur in the
  harness's real timing. The command: run `p174-interleave.exe` with the
  orphan's write interval raised until it never lands inside the capture window,
  and report the interval. I measured it firing at 200 ms against a 1500 ms
  window; a rule that bounds the orphan's lifetime below the window would make A
  safe, and nobody has one.
- **Route C alone becomes sufficient** if the CI holder is shown to be a
  descendant of a `hero_run_go` child. The command:
  `gh api repos/heroes-lang/heroes/actions/jobs/<id>/logs` against a leg carrying
  a `Get-CimInstance Win32_Process` dump taken at the moment of the first 32,
  showing the holder's `ParentProcessId` chain reaching `heroes.exe`. Then B is
  belt and braces rather than the load-bearing half, and the panel may drop it
  on cost.
- **Route C's Windows half falls** if `AssignProcessToJobObject` is refused on
  the GitHub runner. The command, on a CI leg:
  `./p174-tree.exe job-nested` and read whether it prints
  `AssignProcessToJobObject FAILED`. It passed nested on
  `Microsoft Windows [Version 10.0.26100.32690]`; the runner image is not this
  machine and that sentence is a question until it is run there.
- **The POSIX guard falls** if a program in the corpus reads an inherited
  terminal through `hero_run_go`. I searched: `grep -rln 'extern "' examples
  tests/golden | xargs grep -l "fork\|CreateProcess\|posix_spawn\|popen"` returns
  **0** files, so no corpus program spawns its own process either. That is a
  negative claim resting on those four names, and a fifth spawn primitive would
  falsify it.

---

## housekeeping

Nothing of mine is running on the author's box
(`tasklist | grep -icE "p174|e2e|heroes-p174"` reads **0**), and I killed
nothing I did not start. Files left there, all prefixed as the brief asked:
`/c/w/p174-interleave.c`, `p174-tree.c`, `p174-handlelist.c`,
`p174-sharebits.c`, `p174-cleanup.c`, `p174-jobcost.c`, `p174-pathlen.c` with
their `.exe`s, and `/c/w/p174-build/` holding the patched `runtime/`, a pristine
`orig/runtime/`, `p174-endtoend.c` and the two `e2e-*.exe` so the comparison
above can be re-run without rebuilding. The tarballs were deleted; the directory
is 60 MB. The author's `/c/w/heroes` was read only, and was **not** used to
build, because `git status --porcelain` there shows **ten modified files and one
untracked `heroes.exe`** from an earlier session, `runtime/parts/run.c` among
them, which would have confounded every comparison above. It is untouched by
me: `find /c/w/heroes -newermt "2026-09-21 22:40" -not -path "*/.git/*" -type f`
returns nothing, and this session's first command there ran after 22:45.
