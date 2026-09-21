# Panel 174 — completeness critic (SOUNDNESS lane)

I give no verdict. I name what is missing.

Every number below came from a command run in this seat's session on
2026-09-21. The command is named beside it. Where something could not be run
it says so in those words. Darwin is this Mac (Apple clang). Windows is the
author's box over `ssh win`, checked idle before and after
(`tasklist | grep -icE "heroes.exe|main.exe|p174"` read **0** both times); I
started nothing there that outlived a command and I killed nothing. The
repository was not written to except for this one file; my three probes and
three staged runtime trees live under
`…/scratchpad/p174c-*` and `…/scratchpad/p174c/`.

**The headline, because it is the one that changes a verdict.** The
compiler-engineer's route C **does not repair defect 074 on POSIX**, and their
own patched tree says so. Measured through the real `hero_run_go`, with the
watchdog armed at 120 s exactly as `tests/harness/shell.hero:214` arms it, on a
child that **exits normally** instead of hanging:

```
head  case B's capture is 275 bytes (17 owed), NULs 83, another case's bytes present: YES
ce    case B's capture is 300 bytes (17 owed), NULs 83, another case's bytes present: YES
ffi   case B's capture is  17 bytes (17 owed), NULs  0, another case's bytes present: no
```

`ce` is `…/scratchpad/t174`, the compiler-engineer's own patched tree; `ffi` is
`…/scratchpad/p174-run.c.patched`, the ffi seat's. The probe is
`p174c-interleave-exit.c`, built as
`clang -I <tree>/runtime p174c-interleave-exit.c <tree>/runtime/runtime.c -o x`.

Their report says *"With route C applied, the same probe prints `case B's
capture is 17 bytes`, 0 NULs."* That is true of **their** probe and false of
this one. The difference is one line: their `interleave.c` and their
`probe/orphan.c` both call `hero_run_limit(2)` and make the child **hang**, so
they exercise only the timeout branch — and their POSIX diff puts the group
kill only inside that branch (`git diff runtime/parts/run.c` in `t174`:
`kill(-child, SIGKILL)` at the `slept_ms >= limit_ms` arm, `setpgid` gated on
`hero_run_limit_seconds > 0`). **And the CI run this sitting is about never
reached the timeout branch** (§ 2 below).

---

## 1. A route nobody listed

The five routes on the table plus the compiler-engineer's F and G all keep one
architecture: **the child writes into a FILE the parent opened by NAME and
handed over.** A and E argue about the share mode on that open, B about the
name, C/D/F/G about who else holds it. Nobody asked whether the capture has to
be a named file at all.

### Route H — the capture is a PIPE the parent owns, never a file the child opens

Measured on Darwin with `p174c-pipe.c`
(`clang -Wall -Wextra -o p174c-pipe p174c-pipe.c`), four arms under one
identical orphan — a child that spawns a writer and **exits at once**:

```
[arm 1: FILE — today's shape]
  case B's capture is 139 bytes (19 owed), NULs 40, orphan bytes present: YES

[arm 2a: PIPE, drained to EOF — the naive form]
  the child exited; EOF on the read end after 3000 ms: NEVER — the orphan still
  holds the write end (169 bytes drained)

[arm 2b: PIPE, drained after the wait — route H's real shape]
  case A captured 29 bytes: case-A-real-output\nORPHAN-00\n
  case B's capture is 19 bytes (19 owed), NULs 0, orphan bytes present: no

[arm 3: what the closed read end does to the orphan]
  orphan pid 18332 after the read end closed: gone — SIGPIPE took it
```

What it buys, and why it is not any of A–G:

- **`ERROR_SHARING_VIOLATION` becomes unreachable on the redirect**, because
  only the parent ever opens the capture file, for microseconds, with no other
  process able to contend for it. That is route A's goal without route A's bit.
- **The interleave becomes unreachable**, because the orphan holds a write end
  to an object the next spawn does not reuse. That is route B's property — a
  per-spawn OBJECT — obtained **without a per-spawn NAME**, which matters
  because a name is a thing only `tests/harness/shell.hero` controls (§ 3).
- **The scratch sweep is safe**, because nothing under the scratch is ever held
  by anybody but the parent. Route B does not have that property (§ 3).
- **On POSIX it kills the orphan for free**: arm 3, SIGPIPE. That is part of
  what route C is bought for, at zero Win32 surface. The Windows half of arm 3
  is **unrun** — there the orphan would get `ERROR_BROKEN_PIPE`, not death, and
  nobody has run it.

**What it costs, measured, because a route named without its price is not a
route.** `p174c-pipefull.c`, a child writing into the pipe while the parent
does what `hero_run_go` does today — wait for the child, *then* read:

```
--- 32 KB (under the buffer) ---  child asked to write 32768 bytes; waitpid returned 19702   rc=0
--- 1 MB  (over the buffer)  ---  rc=142   (SIGALRM: the parent never returned)
```

So route H **cannot be written as wait-then-drain**. It needs a concurrent
drain on both arms: the Windows arm blocks in `WaitForSingleObject`
(`runtime/parts/run.c:446-448`) and the POSIX arm blocks in `waitpid` whenever
the watchdog is off (`:544-551`). And the threshold is inside this tree's own
range: the largest expectation in the repository is **38,145 bytes**
(`ls -S $(find tests examples -name '*.expected') | head -1` →
`tests/golden/emit/order-options-and-fn-typedefs.expected`), already past the
32 KB that passed here. That is a real cost and it is why I name H rather than
recommend it — I give no verdict.

### The three shapes the coordinator asked me to look at, each settled

- **The scratch directory's lifetime.** `tests/harness/main.hero:123` makes it,
  `tests/harness/shell.hero:205` re-`mkdir_all`s it on every one of the 81
  calls, and **`main.hero:404` removes it**. That last line is the one nobody
  costed and it is load-bearing for question 3 below.

- **"could the harness simply not share one scratch across suites?"** — this is
  **not a route, and the brief's own data refuses it.** The 101 `run` failures
  are all inside ONE suite; a per-suite scratch would have removed **5 of 106**
  failures in run 35620846459 and **29 of 130** in the reproduced run. Command:
  the suite table in § 5 below, from `win.txt` and `win074.txt`.

- **"could the runtime open the redirect files NON-inheritable and hand the
  child a duplicate?"** — **no, and it collapses into route D.** The ffi seat
  measured that the inherited handle arrives in the child with
  `HANDLE_FLAG_INHERIT` **SET** (their question 4, arm 1), so a duplicate made
  inheritable for one `CreateProcessA` call still arrives inheritable, and the
  child's own `CreateProcessA(..., TRUE, ...)` passes it to `clang.exe`
  regardless. Making it arrive non-inheritable would mean duplicating into the
  child after creation and then making that handle its std handle, which lives
  in the child's `RTL_USER_PROCESS_PARAMETERS`. **That last sentence is
  reasoning from the Win32 contract and not a measurement**; I did not run it,
  and a seat that wants D should run it rather than take it from here.

- **"does anything upstream of `hero_run_go` choose to share?"** — **yes, nine
  times, and route B reaches none of them.** `grep -rn 'process\.run(' selfhost/cli/*.hero`
  returns **9** spawn sites in the compiler. Three pass `out: ""`, which means
  INHERIT (`runtime/parts/run.c:131`): `toolchain.hero:79` (clang, during every
  `heroes build` — route F's target), `verbs.hero:87` (`heroes run`) and
  `verbs.hero:136` (`heroes test`). The other six name a path, and three of
  those are **fixed per build directory and therefore shared by every `heroes`
  process in one checkout**: `build/clang-version.txt`
  (`clang_floor.hero:68`), `build/doctor-out.txt` (`doctor.hero:39`),
  `build/pkg-config-out.txt` and `build/pkg-config-err.txt`
  (`libraries.hero:172-173`). The compiler-engineer named three of these in
  route G; the `pkg-config` pair is not among them. **Route B lives in
  `tests/harness/shell.hero` and route C's job object is created only when a
  watchdog is armed in the compiler-engineer's patch, which `heroes build`
  never does** — so on the compiler-engineer's resolution, the compiler in a
  user's hands keeps the whole class.

---

## 2. A claim asserted and not measured

### The shared brief § 4 asks what creates the orphan on the runner. It is still unrun — but § 5's candidate is now EXCLUDED for that run

The brief § 5 offers the watchdog → `TerminateProcess` → surviving grandchild
route and marks it *unrun: nobody has shown the CI run hit the watchdog*. The
ffi seat then measured that this chain **works** (their question 2) and noted,
correctly, that nobody has shown CI hit it. Neither seat closed it. It closes
from the job log alone:

From `win074.txt` (GitHub job 106504808513, the reproduced run at `497a048f`),
suite summary timestamps, `grep -aE "^[0-9T:.Z-]+ +[a-z]+: [0-9]+ passed"`:

| suite | reproduced run | green run (`winGREEN.txt`) |
|---|---|---|
| `unsupported` ends | 20:33:49.0805632 | 06:20:11.5631776 |
| `run` ends | 20:34:54.6720397 → **65.59 s** | 06:25:25.0776511 → **313.51 s** |
| `annotations` ends | 20:34:54.6855519 → **13.5 ms** | 06:26:06.9386841 → **41.86 s** |
| `determinism` ends | 20:37:55.8804787 → **181.19 s** | 06:29:44.8252633 → **217.89 s** |

`WATCHDOG_SECONDS` is **120** (`tests/harness/shell.hero:428-429`, asserted at
`:749`) and `shell.hero:214` arms it on every call. **The whole 136-case `run`
suite finished in 65.59 seconds.** No call inside it can have waited 120
seconds. The window opened between case 35 and case 36, both inside `run`, so
it was not opened by an earlier suite either — case 35 passed, and it uses the
same two paths.

**So on 2026-09-21 the CI orphan was not created by `TerminateProcess`.** The
shape that created it is the ordinary exit, which is exactly the shape the
compiler-engineer's route C does not close on POSIX and the ffi seat's does.

Corroborating, from the same log: `grep -ac "124"` over `win074.txt` returns 8
hits, none of them a watchdog result (they are timestamps and one
`spread 124` token-table line). The harness step ended
`##[error]Process completed with exit code 1`, not 2.

### `p174c-orphan-exit`, the same finding at the level of the process rather than the bytes

```
=== ORDINARY EXIT, watchdog ARMED at 120 s (what the harness does) ===
head     grandchild 17464: STILL ALIVE
ce       grandchild 17467: STILL ALIVE
ffi      grandchild 17470: gone

=== ORDINARY EXIT, watchdog OFF (what `heroes build` does) ===
head     grandchild 17473: STILL ALIVE
ce       grandchild 17476: STILL ALIVE
ffi      grandchild 17479: gone
```

And, to be fair to the seat, I re-ran **their** probe unchanged
(`cd …/scratchpad/probe && ./orphan-head; ./orphan-C`):

```
watchdog: code=124 status=0 (124 means it fired)   grandchild 17562: STILL ALIVE   [head]
watchdog: code=124 status=0 (124 means it fired)   grandchild 17570: gone          [ce]
```

Their probe passes. **Their prediction is scored by an instrument that cannot
see the gap it leaves**, and that is the sentence I am here to write.

### Still unrun, by everybody, and the cheapest command that would settle it

*Who holds the file on the GitHub runner.* Both seats say so. The ffi seat's
`condition` names the right instrument — a `Get-CimInstance Win32_Process` dump
taken at the moment of the first 32, showing the holder's `ParentProcessId`
chain. **Nobody proposed adding it to the CI leg**, which is a workflow step
and not a language change, and until it exists every run of this defect throws
the one fact the sitting is missing away.

---

## 3. The contradiction between the seats, and which side is checkable

### They contradict each other on route A's DELETE bit, and the ffi seat is right

- compiler-engineer: *"One thing route A is not: harmful with B. Once each file
  has exactly one writer, the share mode stops mattering — which also means A
  becomes unnecessary. I would not spend the three lines."*
- ffi-pragmatist: veto on `FILE_SHARE_WRITE`, **adopt `FILE_SHARE_DELETE`**.

**It is checkable, and the DELETE bit still buys something with B in place.**
Re-ran the ffi seat's `p174-sharebits.exe` on the author's box, unchanged:

```
[READ (today)  ]  DeleteFile on the held file: REFUSED, error 32
                  RemoveDirectory on the scratch: REFUSED, error 145
[READ|DELETE   ]  DeleteFile on the held file: ok      RemoveDirectory: ok
```

Now follow it into this repository. Route B gives each spawn its own name and
changes no share mode, so an orphan holding `scratch/stdout-0042` is still
undeletable. `tests/harness/main.hero:404` is

```heroes
swept = shell.remove_tree(scratch)
if swept.is_err()
    print("harness: could not remove " + scratch)
    exit(2)
```

`shell.hero:615-616` fails on any non-zero; `runtime/parts/dir.c:156` and `:178`
propagate one failed file into `HERO_OS_FAILED`; `runtime/parts/fs.c:142-162`
retries on Windows at 5, 10, 20, 40, 80, 160 and 320 ms — **635 ms total** —
and then returns `HERO_OS_FAILED`, which is **2** (`runtime/hero_os.h:41`).

**So with route B alone, a surviving orphan turns a green harness line into
`harness: could not remove build/harness-<pid>` and `exit(2)`.** The suite
verdict is printed at `main.hero:397`, *before* the sweep, so the leg reads
`harness: N passed, 0 failed` and then goes red anyway. The compiler-engineer
hunted route B's failure modes deliberately — MAX_PATH, a suite that globs the
scratch, the one outside dependency — and this one is not among them.

It has **not** happened yet: `grep -ac "could not remove" win074.txt` returns
**0**, and the job ended with exit code 1. So this is a reachable path, not an
observed one, and I say so in those words.

The compiler-engineer is right about the other bit. `FILE_SHARE_WRITE` is what
opens the trap, measured by both seats independently and again by me
(§ 1 arm 1: 139 bytes where 19 were owed, 40 NULs). Nothing here disturbs that.

### And a SECOND contradiction, which nobody stated: the two seats' "route C" are different patches

| | compiler-engineer (`t174`) | ffi-pragmatist (`p174-run.c.patched`) |
|---|---|---|
| Windows job created | `if (hero_run_limit_seconds > 0)` | **unconditional** (`:509`) |
| Windows kill on ordinary exit | yes, `CloseHandle(job)` + `KILL_ON_JOB_CLOSE` | yes |
| POSIX group created | `if (hero_run_limit_seconds > 0)` | `hero_run_group_is_safe(in_path)` (`:615`) |
| POSIX kill on **ordinary exit** | **no** | yes, `waitid(WNOWAIT)` + `kill(-child)` (`:247-254`) |
| `heroes build` covered | Windows no, POSIX no | Windows yes, POSIX unless stdin is an inherited tty |

Commands: `git diff runtime/parts/run.c` inside `…/scratchpad/t174`, and
`grep -n "CreateJobObjectA\|hero_run_limit_seconds > 0\|waitid\|WNOWAIT\|kill(-\|setpgid\|hero_run_group_is_safe" …/scratchpad/p174-run.c.patched`.

A synthesis that writes "adopt C" without saying **which** C adopts a patch
that, on the evidence above, does not repair the defect on three of the four
platform legs.

**One thing the ffi seat's POSIX gate does that nobody costed, and it goes out
as a question rather than a finding because I did not run it.** Their guard
allows a new process group whenever stdin is not an inherited terminal, so
`heroes run prog < input.txt` from a terminal puts the program in a background
group. A background group is not the terminal's foreground group, so Ctrl-C
would reach `heroes` and not the program. *Does route C's POSIX half change
what Ctrl-C does to `heroes run` with redirected stdin, and does that create
the very orphan class C exists to remove?* The command that settles it:
`script -q /dev/null` around a patched `heroes run prog < file`, with a SIGINT
sent to the foreground group, checking whether `prog` survives.

---

## 4. The question the sitting should have asked and did not

**"Should a machine failure be a case verdict at all?"**

Every route on the table — A through H — is about making the machine stop
failing. Not one is about what the harness should DO when it does. The answer
today is: it produces **130 verdicts about 130 programs, none of which ran**,
and keeps going for another sixteen minutes. Measured from `win074.txt`:

- `run`: 101 lines of `the compiler never ran on … reason is 32`
- `annotations`: 19 groups, same reason
- `determinism`: 10 lines that read, in full, **`cannot dump tests/golden/ir/<case>.hero`** — no reason at all

and in the *earlier* run (`win.txt`, job 106403218648) the five `annotations`
failures read **`the annotations and the diagnostics disagree`**, which is a
statement about the author's program made by a harness whose compiler never
started.

The vocabulary already exists: `shell.hero:183-190`'s `insisted()` **refuses**
rather than returning a `Run` with `started: false`. `status != 0` from
`hero_run_go` is not a program's answer and never was — the harness says so at
`shell.hero:229-235`, in the comment `497a048f` added. **Nobody asked whether a
suite should stop, or mark the remaining cases unrun, on the first
`HERO_OS_FAILED`.** That question is independent of which route lands, it costs
nothing if a route lands and works, and it is the only thing on this page that
would have made the 2026-09-21 leg legible to the person who opened it.

It also bears on the sitting's own cost. The compiler-engineer priced the
"fourth liar" repair at *"up to 250 report sites… a milestone, not a step"* and
said it *"changes what is printed and never whether the defect happens, so it
must not gate this resolution."* **Ten of the 130 failures in the very run this
sitting is about are illegible because of it**, and they are the ten that would
tell the next session whether `determinism`'s failures are defect 074 or
something else. They are also, measured, **exactly `ir` cases 1 to 10 of 23** in
sorted order (`ls tests/golden/ir/*.hero | sed 's|.*/||;s|\.hero$||' | sort | nl`),
which is the only reason anybody can guess.

---

## 5. Brief premises that are false

The coordinator wrote the shared brief and nothing checks a brief against the
world. The ffi seat found one (`grep -c '\.code'` measures the wrong thing
since `497a048f`). Here are the others, each with the command.

### (a) § 2: "the 5 `annotations` failures are **exactly the first 5** of the **10 groups** that suite reports on" — both halves are wrong, and the second half carries the brief's central inference

**The suite reports on 19 groups, not 10.** `suite_annotations.hero:239-255`
walks `RUN_ROOTS` (`tests/golden/fixedbugs`, `tests/golden/surface-fixtures`),
keeps a root that holds cases, adds every subdirectory, and returns
`out.sort()`. Measured:
`(echo tests/golden/fixedbugs; ls -d tests/golden/surface-fixtures/*/) | wc -l`
→ **19**. `497a048f` touched no file under `tests/golden/`
(`git show --stat 497a048f --name-only | grep 'tests/golden'` matches only
prose in the commit body), and `git show 316d69c4:tests/harness/suite_annotations.hero`
has the same `out.sort()` at `:255`, so it was 19 on the failing commit too.
The reproduced run failed **all 19**.

**They are not the first 5.** The five names, from
`grep -a "FAIL annotations/" win.txt`:

```
tests/golden/fixedbugs                        (1)
tests/golden/surface-fixtures/applyx          (2)
tests/golden/surface-fixtures/calledforeign   (3)
tests/golden/surface-fixtures/cross           (4)
tests/golden/surface-fixtures/externroute     (7)
```

Positions **1, 2, 3, 4 and 7** of the sorted 19. `deep` (5) and `deepthread`
(6) did not fail.

**Why this matters more than a number.** § 2's conclusion is *"So one window of
wall-clock time opened inside `run` and closed inside `annotations`. **The
cause has a LIFETIME, not a location.**"* A single continuous window produces
groups 1 to 5. It produced 1, 2, 3, 4, 7. So either the window was not
continuous, **or `deep` and `deepthread` reported a pass while the compiler
never ran for them** — which would be a defect nobody has recorded. I could not
separate the two: I searched those fixture directories for annotation markers
with `grep -rho 'error\[\|warning\['` and the counts do not separate the
failures from the passes, so that grep is not the instrument. **It goes out as
a question**: *does a `surface-fixtures` group with nothing to pin report a
pass when the compiler did not start?* Somebody who knows how
`suite_annotations.hero` pins a diagnostic should run it. Either answer changes
§ 2.

The reproduced run has the opposite shape — `run` 36–136, `annotations` all 19,
`determinism/ir` 1–10, perfectly contiguous. **Two runs, two shapes.** The
brief generalised from one.

### (b) § 2: "Eighteen `abort-*` cases sit inside the first 35" — it is 14

`ls tests/golden/run/*.hero | sed 's|.*/||;s|\.hero$||' | head -35 | grep -c '^abort-'`
→ **14**. Counting `abort` anywhere in the name gives **15**
(`adversarial-overflow-aborts`). The whole directory holds **14** `abort-`
cases (`… | grep -c '^abort-'`), so no spelling reaches 18. Not load-bearing —
it decorates the sentence about cases 35–37 being the only `!sanitizer:` ones,
which **is** true: `grep -rln '!sanitizer:' tests/golden/` returns exactly the
three `c-frees-a-lease-*` files.

### (c) § 1: "the author's own Windows box runs the `run` suite at 130 passed, 0 failed **on the same HEAD**" — the HEAD is not the same

`ssh win 'cd /c/w/heroes && git log --oneline -1'` reads **`e6202a6e`**, one
commit behind `497a048f`, with **10 modified files and one untracked
`heroes.exe`**. I checked whether that invalidates the measurement and **it does
not**: `git diff --stat` there reads `runtime/hero_os.h 13`,
`runtime/parts/run.c 35`, `tests/harness/shell.hero 108` and seven suite files,
which is `git show --stat 497a048f`'s own file list and line counts exactly, and
the `run.c` hunks are `hero_run_why` verbatim with no share-mode change. So the
tree is the sitting's tree even though the HEAD is not. **The sentence is false
as written and true in substance**, and the distinction matters because it was
not checkable from the brief: a reader had no way to know the box was carrying
an uncommitted copy of the very commit under discussion.

### (d) § 4: "This is the only named holder anybody has measured" — true, but the configuration it was measured in no longer exists

`tests/harness/shell.hero:414-417` landed in **`c4b65734`, 2026-08-31**
(`git log -1 --format='%h %ad %s' --date=short c4b65734`), and the comment's own
words are *"…left Windows with **no watchdog**, which showed inside one run: two
orphan `heroes.exe` and a `clang.exe` held `build/harness/stdout` open."* Two
things follow that the brief does not say:

1. The episode happened **with no Windows watchdog at all**, which is the
   opposite configuration from § 5's route (watchdog → `TerminateProcess` →
   orphan).
2. The path it names, `build/harness/stdout`, **no longer exists.** The pid
   entered the scratch name in **`5320c2f3`, 2026-09-03**
   (`git log --oneline -1 -S 'SCRATCH + "-" + shell.pid()' -- tests/harness/main.hero`),
   three days later. Today it is `build/harness-<pid>/stdout`
   (`tests/harness/main.hero:123`).

So the only named holder is evidence that orphans happen on Windows, and is not
evidence about the configuration that failed on 2026-09-21.

### (e) Citations that are off by a line or two, listed once and not argued

- § 6 B cites `tests/harness/suite_determinism.hero:149-154`; the `read_file(scratch + "/stderr")` is at **:150**.
- § 3 cites `runtime/parts/run.c:347,351`; correct (`sed -n '347p;351p'` prints the two `CreateFileA` lines).
- § 5 cites `runtime/parts/run.c:441-452` for the `TerminateProcess` arm; `TerminateProcess` is at **:443**, inside `:441-452`. Correct.
- § 3's "**81**" call sites: confirmed, `grep -rc 'shell\.run(\|shell\.run_fed(\|shell\.run_guarded(' tests/harness/*.hero` sums to **81**.
- § 2's "`ls tests/golden/run/*.hero | wc -l` is **136**": confirmed.
- § 2's identity check, re-run against the **reproduced** log rather than the
  briefed one — `grep -ao "FAIL run/[a-z0-9-]*" win074.txt | … | diff - <(… | tail -n +36)`
  prints nothing. **The 101 are cases 36–136 in that run too.**
- The lane premise "no program in `examples/` or `tests/golden/` declares
  `hero_run_go`": confirmed, `grep -rl 'hero_run_go' examples/ tests/golden/`
  returns 0 files.

### (f) One thing the compiler-engineer asserted that I could not falsify, and one I could narrow

*Could not falsify*: `tests/harness/suite_layout.hero` really does contain **0**
occurrences of `runtime` and **0** of `.c"` (`grep -c`), and
`tests/harness/main.hero:49` states its scope as *"§11's per-file ceiling over
`selfhost/`, tests excluded"*. So no instrument polices `runtime/parts/run.c`'s
length. Their objection to their own verdict stands.

*Narrowed*: **`tests/harness/suite_runtime.hero` does walk `runtime/`** —
`:224`, `shell.list_tree(dir: RUNTIME)` — and carries a `LEAST_FILES` floor on
how many source files are there (`:110`, `:281`). It does not measure length,
so it would not have caught the growth; but it means the `run-win.c` split the
compiler-engineer proposes is **not** invisible to the suites, and whoever lands
it should run `runtime` as well as `layout`.

*Confirmed repo-wide rather than in two directories*: their
`grep -rn 'hero_run_limit' selfhost/ tests/harness/` holds against the whole
tree. Excluding `archive/`, the only **call** anywhere is
`tests/harness/shell.hero:214`; `runtime/hero_os.h:228` declares it,
`runtime/parts/run.c:298` defines it, `shell.hero:41` binds it.

---

## What would make THIS report wrong

- The headline rests on `…/scratchpad/t174` being the compiler-engineer's final
  patch. If they wrote an ordinary-exit sweep after taking that snapshot, my
  `ce` column measures a draft. The command that falsifies it:
  `cd …/scratchpad/t174 && git diff runtime/parts/run.c | grep -c 'waitid\|WNOWAIT'`
  — it reads **0** as I write this.
- The watchdog exclusion in § 2 rests on the suite summary line being printed
  when a suite *finishes*. If `report.verdict` batches, the 65.59 s is not the
  `run` suite's duration. The command that falsifies it: read
  `tests/harness/report.hero`'s verdict path, or compare against the green run
  — where the same lines spread over 12 minutes, which is what made me believe
  them.
- § 5(a)'s two readings — a discontinuous window versus a vacuous pass — are
  both consistent with what I measured, and I did not separate them. Whoever
  does should say which, because the brief's § 2 conclusion depends on it.
- Route H is measured on Darwin only. Its Windows half — anonymous pipe,
  `PeekNamedPipe` drain, whether the orphan's `WriteFile` answers
  `ERROR_BROKEN_PIPE` — is **unrun**.

## Housekeeping

Nothing of mine is running on the author's box
(`ssh win 'tasklist | grep -icE "heroes.exe|main.exe|p174"'` → **0**), and I
left **no new file there**: the only thing I ran was the ffi seat's existing
`/c/w/p174-sharebits.exe`, and `/c/w/heroes` was read only. On Darwin,
`pgrep -fl 'ORPHAN_BYTES_FROM_CASE_A|p174c'` returns nothing. Files I created,
all outside the repository and all prefixed: `p174c-pipe.c`,
`p174c-pipefull.c`, `p174c-orphan-exit.c`, `p174c-interleave-exit.c`,
`p174c-winjob.txt` and the three staged runtime trees under `p174c/`
(`head`, `ce`, `ffi`), in
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/`.
The only file I wrote inside the repository is this one.
