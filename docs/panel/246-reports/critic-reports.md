# Panel 246, the completeness critic's second pass, over the seats' reports

Written 2026-11-12 from 06:31, under `docs/panel/246-briefs/critic-reports.md`, by the completeness
critic. Not a judge: no verdict on any route. Every fact carries the command run from
`.claude/worktrees/p246` or from my folder `.claude/worktrees/p246-scratch/critic2/` (below `K/`) and
what it printed. Written as I go; the numbered list and the grading table at the end are the report.

## Setup

- Read, whole: `00-shared.md`, every brief of `docs/panel/246-briefs/`, my first pass
  (`246-reports/critic-briefs.md`), and the reports `compiler-engineer.md` (669 lines),
  `ffi-pragmatist.md` (512), `spec-warden.md` (431), `historian.md` (630) and `llm-ergonomist.md`
  (2402) (`wc -l`).
- `uptime` at 06:31: load averages 21.52 51.13 63.00. No duration here is a measurement.
- My base: `git -C .claude/worktrees/b33-round archive ef50a7478 | tar -x -C K/tree` (`rev-parse`
  printed `ef50a7478c241f765c0b308cb55282f95d64053f`, tree `084b36b7d5db...`); `K/` is ignored
  (`git check-ignore -v`: `.gitignore:34:.claude/worktrees/`).
- The two Linux logs at 06:31: the compiler-engineer's `linux-a.log` 0 bytes, the ffi-pragmatist's
  501 bytes ending at `seed exit 0` (`ls -la`, `cat`): both legs still running when I started.

## Log, as I went

### 06:31 to 06:36, my compiler at the base

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in `K/tree`, exit 0 (`real 22.02`,
  `user 16.73`); `./heroes build selfhost/main.hero -o heroes-next`, exit 0 (`real 273.49`, `user
  150.85`, `sys 28.73`: a waiting run, not a duration); `./heroes-next --version`: `heroes 0.6.0`,
  **`sources 796d1133542b1909`**, the base's. Run from `K/grade/` (which holds no `runtime/`),
  `heroes-next doctor` names `K/tree/runtime`.

### 06:36 to 06:45, contradiction 1: where route A's lock stands, and what that costs, run

**Read** (`diff -u 246-compiler-engineer/tree/selfhost/fetch/staging.hero tree-a/...`, `cat
tree-a/selfhost/fetch/leftover.hero tree-a/runtime/parts/lock.c`):

- The compiler-engineer's lock is **inside** the run's folder, `<folder>/lock`, and is taken
  **after** the folder is made: `opened` runs `while process.path_exists(folder)` (route D, the next
  serial), then `process.mkdir_all(folder)`, then `leftover.held(folder)` =
  `hero_lock_take(folder + "/lock", make: 1)` (`open(O_RDWR|O_CLOEXEC|O_CREAT|O_EXCL)`, then
  `flock(LOCK_EX|LOCK_NB)`).
- **The ffi-pragmatist's identity check is in it, on POSIX only**: `lock.c`, after the `flock`,
  `fstat(fd)` against `stat(path)`, `st_dev` and `st_ino`, `HERO_LOCK_HELD` where they differ. The
  Windows arm (`CreateFileW` with `FILE_SHARE_DELETE`, `LockFileEx`) has no such check.
- Recovery (`leftover.undone`) takes `hero_lock_take(folder/lock, make: 0)` (no `O_CREAT`; a missing
  file answers *held*, so a lockless folder is left), then removes the ledger, then **releases the
  lock, then removes the folder**; `ended` and `dropped` do the same (*"the lock first, since Windows
  removes no file a process holds open"*). The ffi-pragmatist's `proto.c` removes while holding in
  both its layouts, so **no seat measured the compiler-engineer's order**.

**Run** (`K/lockrace/`): `ce-lock.c` is the compiler-engineer's `lock.c` copied (`cmp`: same),
included unmodified by `race.c`, which drives it in `tree-a`'s order under the ffi-pragmatist's load
shape (eight owners alive at once, one in three SIGKILLed at a random moment, 1 to 20 ms of life,
recoverers sweeping `.fetching-*`). An owner whose `hero_lock_take(..., 1)` fails is a **birth
lost** (the run refused `cannot_place`); one whose folder vanished between its lock and its `tree/`
is **other** (`gone`); one whose `tree/alive` vanished while it lived is a **violation**. `clang
-std=c11 -O1 -Wall -Wextra`, exit 0; every run under `timeout -s KILL 60`, `pgrep -fl lockrace/`
empty after.

| order, recovery | runs | started | births lost | gone | violations | lockless corpses left |
|---|---|---|---|---|---|---|
| the compiler-engineer's (release, then remove), 3 continuous recoverers | 3 | 3,801 / 3,659 / 4,070 | 182 / 178 / 233 | 67 / 41 / 42 | 1 / 1 / 0 | 5 / 3 / 6 |
| the same, **no recoverer but each owner's own sweep at its start** (as `begun` and `packages.reach` run `recovered`) | 3 | 3,776 / 3,714 / 3,989 | 17 / 20 / 15 | 2 / 3 / 0 | 0 / 0 / 0 | 2 / 5 / 8 |
| remove, then release (`race2 ... 0 0`), 3 recoverers | 1 | 3,816 | 292 | **0** | 0 | 2 |
| the ffi-pragmatist's `proto.c` (copied), **beside**, identity on, 3 recoverers | 1 | 3,603 | **0** | | **0** | **0** |
| `proto.c`, inside, a lockless folder read as alive | 1 | 3,673 | 272 | | 0 | 5 |

**What the log says the vanished folders are** (`race2 r2a 8 3 8 0 1 r2a.log`, a line per birth,
removal and vanishing): 42 `gone`, **no name born by two process numbers** (4,067 births, 4,067
distinct numbers, 57850 to 62440), and every one removed by a recoverer *after* its owner's birth.
So it is no PID reuse: a recoverer takes the fresh lock in the owner's `open`-to-`flock` window,
releases it, and while it is still removing, the owner's `flock` succeeds and its identity check
passes, because the file has not yet been unlinked. **The identity check does not close the window
the compiler-engineer's order opens**; removing before releasing closes it (0 `gone`) and leaves the
`open`-to-`flock` window (292 births lost). The ffi-pragmatist's order (the lock beside the folder,
taken before `mkdir`, the folder removed while the lock is held) read 0, 0 and 0 under the same load
here, and on Linux arm64 (below).

What this is not: a field rate. Continuous recoverers are a stress; the self-sweep rows are the
code's own shape at eight runs starting together on one program. In `tree-a`'s real code the `gone`
case does not fail as my harness does: `opened` makes `tree/` with `mkdir_all`, which would make the
folder again, so the run goes on in a folder with **no lock file**, which no later run recovers
(read, not run): if it is then killed, its `replaced-N` is hidden for ever, the base's behaviour.

### 06:35 to 06:50, the two Linux arm64 legs, read

- **The ffi-pragmatist's** (`246-ffi-pragmatist/linux-a.log`, ending `== done`, read at 06:35 and
  whole): stage 2 `sources 796d1133542b1909`. **A-4 on Linux**: beside, identity on, 0 violations
  twice; **beside, identity off, 0 violations twice** (3,494 and 3,627 owners), so the seat's
  prediction *"more than 0 on both identity=0 runs"* is **falsified on Linux** (met on this Mac, 65
  to 76); inside, a lockless folder read as alive, **3 births lost and 69 folders left** (Darwin: 211
  and 4). A-1's table on overlay and `/dev/shm` equals Darwin's. `close-wakes`: `hero_net_close` of
  the listener **wakes** the blocked `accept` on Linux (`returned NULL, kind 11 os 22`) and the late
  client is `refused`, unlike Darwin's `MADE`: the seat's candidate defect 1 is Darwin's alone, as it
  predicted. `whichthread2`: a signal reaching a `hero_thread_spawn` worker before its claim, **exit
  134 in 10 of 10** with no pause (Darwin 3 of 5), 0 of 10 with one: the seat's candidate defect 2
  holds on Linux, more often. `disp.c` **did not compile** there (`struct sigaction` incomplete, no
  feature macro), so *"a job started with `&` inherits SIGINT ignored"* is unmeasured on Linux. The
  draft stop ended a Heroes server *"within 1 tenths of a second of SIGTERM"*; the `str` rung 20 of 20
  plain and under `--sanitize` with LeakSanitizer; the lease carried past `end_lease` printed `118
  bytes` plain (Darwin `0`) and `heap-use-after-free` under `--sanitize`, exit 1.
- **The compiler-engineer's** (`246-compiler-engineer/linux-a.log` as it stood at 06:44, 6,645
  bytes): **void as a measurement of Part A**. Every case's index is `file://../idx`
  (`fx-src/app/heroes.toml:1`), every `fetch` and `add` stopped at `error[no_curl]`, exit 2, before
  the phase it meant to judge, and the first kill (`killed fetch 2905, wait 2`) killed a run that had
  already ended. **Reproduced on this Mac** (`K/nocurl/`): the same fixture, `git init -q`, `PATH`
  holding only a link to `/usr/bin/git`: `heroes-next fetch` printed `heroes.toml:3:
  error[no_curl]: ...` twice, exit 2; with `curl` on `PATH`, `placed small`, `placed large`, exit 0.
  `selfhost/fetch/index_source.hero:1-3`: a static index is *"files a site or a folder serves, read
  with curl"*. **So "no case needs curl" is a framing error of the briefs, and it is mine first**: my
  first pass's item 8 proposed it as the repair (*"and no case needs `curl`"*), 00-shared.md § Part A
  Platforms and both seat briefs copied it, and the compiler-engineer's `parta2.sh` asserts it in
  its own comment (line 4, *"needs no curl"*). A `file://` index needs `curl`; whether a `git`
  source (`fetch/git_source.hero`) needs only `git` is unrun here. The
  coordinator's message of 06:45 says a re-run with `curl` installed is under way; its log was
  **pending** when this section was written (checked again below).

### 06:44 to 06:48, contradiction 2: the stop, run where it can be

**Three mechanisms carry the name "(c)"**, which no report says:

- the compiler-engineer's `s1c/` (`diff s1/runtime s1c/runtime`): `hero_net.c:512` becomes `} while
  (got == HERO_NET_NO_FD && hero_net_interrupted(code) && hero_stop_asked() == 0);`, so `accept`
  returns only where the signal interrupted **the thread blocked in it** (`EINTR`, `sa_flags` 0);
- the ffi-pragmatist's `tree-rung/` (`parts/stop.c`, 127 lines): the handler writes a byte down a
  close-on-exec pipe, and the layer's `accept` `poll`s that descriptor beside its socket, answering
  `HERO_NET_STOPPED` (12): (b) inside (c);
- the historian's (c): *"the server closes its own listener"* (Go's `Server.Shutdown`, Java's
  `ServerSocket.close`), which leans on `hero_net_close` waking `accept`: on Darwin it does not (the
  ffi-pragmatist's `close-then-connect`, `MADE`), on Linux it does (its log, above).

**Run, the shape that tells the first two apart** (`K/probes/acc-worker/`): my base `heroes-next`
copied beside a copy of each seat's runtime (`diff -rq` against the seat's: equal; `heroes doctor`
from the probe's folder names my copy), one program: S1 armed (`hero_stop_catch()`; the
ffi-pragmatist's copy arms by a first `hero_stop_asked()`), a `hero_thread_spawn` worker that listens
on port 0 and blocks in `hero_net_accept`, **main waiting in `hero_thread_join`**. Driver
`K/tools/t3run.py`: the binary started with SIGINT and SIGTERM at their defaults and no signal
blocked, the signal at 2.5 s, 10 s allowed, then SIGKILL to its group; `pgrep -fl acc-worker` empty
after.

| runtime | SIGTERM | SIGINT |
|---|---|---|
| the compiler-engineer's `s1c` | **alive 10 s after, SIGKILLed** (exit -9), no output | **the same** |
| the ffi-pragmatist's `tree-rung` | exit 0, **0.00 s** after, `acceptor answered 0, stop asked 1` | the same |
| control: the compiler-engineer's own `b2/accept2.hero` (main in `accept`, a worker stepping) on my `s1c` copy | exit 0, `accept returned nothing`, `worker saw the stop after 185 steps` | |

So the compiler-engineer's (c) stops a server only where the signal lands on the thread in `accept`;
with `accept` on a worker and main elsewhere (the ffi-pragmatist's which-thread rows: main not
blocking takes the handler 20 of 20, on Darwin and on Linux), it had not returned 10 s after either
signal. The ffi-pragmatist's
(c) woke it at once. That is the measured answer to the compiler-engineer's own `condition` (*"a run
in which (c) does not wake a thread blocked in `accept` (the signal taken by another thread), which
would send me to route (b)"*): met on this Mac, Darwin; Linux unrun for this shape (its delivery rule
measured the same as Darwin's in the ffi-pragmatist's log).

**The spec-warden's veto** is within its seat: `.claude/agents/spec-warden.md:45-46`, *"or if
Principle 0's burden of proof is unmet"*. Its W4 (*"still running 3 s after SIGINT with no client"*
under S1 to S3 without (b) or (c)) was scored by the compiler-engineer's `accept1` under S1 (alive at
`ELAPSED 04:25`, only `kill -9`), not re-run by me. What the blind seat's t3 adds to its *"no measured
thesis effect"* is in the grading below: arm x one `right` of two, each of p, q and r two of two, on
the `sleep` loop the spec-warden said cannot see the server's shape.

### 06:48 to 06:50, contradiction 3: what crosses into a thread

- **The compiler-engineer and the ffi-pragmatist approve the same two doors under two names.** The
  compiler-engineer's *"numbered handle"* (`netnum/`: `hero_net_number(s) -> i64`, bound `consumes`,
  and `hero_net_of(n) -> Socket acquires hero_net_close`, 14 lines in the layer) and the
  ffi-pragmatist's *"rung that moves a handle (the four header lines as two `hero_net.h` doors with
  marks)"* (`carry.h`'s `socket_as_number` `consumes`, `number_as_socket` `acquires`) are one
  mechanism: the layer's handle is already `(life << 32) | (index + 1)` and a stale one is refused.
  The compiler-engineer says *"Neither needs a handle rung"*, the ffi-pragmatist *"I approve a rung
  that moves a handle"*: the words differ, the doors do not. Both measured a stale number refused
  (`send -1, kind 2`, `not open`), the ffi-pragmatist on Darwin and Linux (its log's `stale` rows,
  plain and `--sanitize`). **The spec-warden's question 6** (*"a numbered handle ... reopens what
  panel 234's R3 says the wrapping is for"*) is contradicted by both runs for a stale number.
- **What neither measured, run** (`K/probes/numbered/dropped.hero`, my base compiler,
  `carry.h` copied): a socket accepted, turned into its number (`consumes`), and never taken back,
  `main` returning: plain **exit 0**, prints `the accepted socket is the number 3; nobody takes it
  back`, nothing on stderr; `--sanitize` the same, exit 0. Once a handle becomes an `i64` the marks
  stop counting it, and a number dropped is a descriptor that stays open with no message: the leak
  check task 1072 wants for a long-running server is blind to it. Not run: the compiler-engineer's
  `netnum` doors themselves (the same `consumes` shape, read).
- **Two `str` rungs, two semantics, each measured clean and never side by side.** The
  compiler-engineer's `hero_thread_spawn_str` (23 lines) **shares** the `str` by its atomic count
  (panel 113's own measurement: one `HeroStr` under 250,000 concurrent operations), 0
  ThreadSanitizer warnings with the instrument proved live. The ffi-pragmatist's (41 lines) **copies**
  the bytes on the spawning thread (design.md:4089, *"copying at the boundaries"*), 20 of 20 plain and
  `--sanitize` on Darwin and on Linux. Which one panel 113's R5 ratified is a reading for the
  synthesis; a grep of `runtime/parts/str.c` for a uniqueness test finds none (`refcount` appears at
  117, 196, 201, 207 and 215 only, an initial store, loads, an add and a subtract), which is the
  compiler-engineer's ground for sharing being safe; it rests on `str` never being mutated in place,
  a property of today's runtime, not a rule anyone wrote down as one (a question).
- **Where `i64`-only was written, read through git** (`git -C .claude/worktrees/b33-round log -S
  hero_thread_spawn -- runtime/hero_os.h runtime/runtime.c runtime/parts/spawn.c`: `ea4a6fda7`
  2026-09-06 12:22 and `0650990ff` 2026-10-03): `ea4a6fda7`'s body says *"AND THE ISOLATION IS THE
  TYPE RULE ... `crosses_the_boundary` judges the callback's own parameters and result ... A `[T]` or
  `{K: V}` in that signature is `error[ffi_type]`"*. Its author believed the checker decided what
  crosses; the checker says yes to `str`, and the `int64_t (*)(int64_t)` prototype is what refuses it
  at `build`. That reads as the compiler-engineer reads it, a gap and no ruling; the spec-warden's
  *"for the author to rule"* stands, with this commit body as the evidence to put beside R5.

### 06:36 to 06:50, the blind seat graded by its registered rule

**How**: `K/tools/extract.py` took each session's files from `result` in its `run.json` (the
report's `### \`main.hero\``, `### \`bands.h\`` and `**\`main.hero\`**` fences; 2 files for t1-x-1
and t1-x-2, 1 for each other) into `K/grade/<session>/`, a folder holding no `runtime/`. **t1, t2 and
t3 under x**: my base `K/tree/heroes-next` (`sources 796d1133542b1909`, its runtime beside it).
**t3 under p and q**: the compiler-engineer's `s1/heroes` and `s2/heroes` (`--version`: `sources
796d1133542b1909`; `doctor` from `K/grade/` names `246-compiler-engineer/s1/runtime` and
`s2/runtime`, whose `hero_os.h:449-450` and `:448` declare exactly S1's two names and S2's one);
**under r**: `tree-s3/heroes-next` (`sources 06b60f830cca15c0`, `inventory.hero:138` the built-in
`stop_asked`, `library_source.hero:280-281` its body). Each run only from my folder; `find
246-compiler-engineer/s1 s2 tree-s3 -newer K/tmp/marker-before-t3` printed nothing after the builds.
`heroes build main.hero -o prog`, then t1 and t2 binaries under `timeout -s KILL 60`, output into
files; t3 binaries under `K/tools/t3run.py` (SIGINT and SIGTERM at their defaults in the child, no
signal blocked, so a background job's inherited ignore cannot decide a grade; the signal at 2.5 s, 2 s
the rule's bound, 10 s allowed for the record, then SIGKILL to the group, each stream bounded at
65,536 bytes). `pgrep -fl critic2/grade` empty after every batch. Log: `K/t3-runs.log`.

| session | arm | build | run | class | note |
|---|---|---|---|---|---|
| t1-x-1 | x | exit 0 | exit 134, stdout empty, `panic: main.band_sum ran on a thread this program did not start ... (design.md Part 7.13)` | **stopped** | its own `bands.h` wrapping `pthread_create` |
| t1-x-2 | x | exit 0 | exit 134, the same panic twice | **stopped** | the same shape; it noted the folder's path |
| t1-y-1 | y | exit 0 | `170666346666800000` twice, exit 0 | **right** | 8 spawned, then 8 joined (read) |
| t1-y-2 | y | exit 0 | the same | **right** | 8 spawned, then joined |
| t1-z-1 | z | **exit 1**, `error[ffi_unknown_name]: hero_os.h declares no hero_thread` at `record Thread tag hero_thread` | | **refused** | names and body type right; it noted the path |
| t1-z-2 | z | exit 1, the same | | **refused** | the same guess |
| t2-x-1 | x | **exit 1**, `error[reserved_word]: func is not a word in this language` (C11 `thrd_create` from `threads.h`) | | **refused** | the constant, read in four bodies; nothing crosses (`nullptr`) |
| t2-x-2 | x | exit 1, `ffi_unknown_name` `hero_thread` | | **refused** | a constant; an `i64` index would cross |
| t2-y-1 | y | exit 0 | `4 2 6 2 14`, exit 0 | **right** | rebuilt in the thread by a `match` returning literals; an `i64` index crosses |
| t2-y-2 | y | exit 0 | `4 2 6 2 14`, exit 0 | **right** | `constant SENTENCES: [str]` read on four threads; an `i64` index crosses |
| t2-z-1 | z | exit 1, `ffi_unknown_name` `hero_thread` | | **refused** | a constant; index |
| t2-z-2 | z | exit 1, the same | | **refused** | an array literal built inside the thread; index |
| t3-x-1 | x | exit 0 | INT and TERM each: `tick 1` to `tick 3`, `stopped after 3 ticks`, exit 0, **0.00 s** after the signal | **right** | a **Heroes** handler through `signal` closes a `dup2`'d descriptor the loop probes; `sleep` cut short, so the step in hand is not a full second (the rule does not ask) |
| t3-x-2 | x | **exit 1**, `error[empty_record]` at `record sigset_t partial`, and `line_end_before_continuation` | | **refused** | `sigprocmask` and `sigpending`, route (a) by hand |
| t3-p-1 | p | exit 0 | INT, TERM: 3 ticks, `stopped after 3 ticks`, exit 0, 1.00 s after | **right** | `sleep(left)` until the second is whole |
| t3-p-2 | p | exit 0 | INT, TERM: the same, 0.00 s after | **right** | `_ = sleep(1)`, cut short |
| t3-q-1 | q | exit 0 | INT, TERM: the same, 1.00 s after | **right** | |
| t3-q-2 | q | exit 0 | INT, TERM: the same, 1.00 s after | **right** | |
| t3-r-1 | r | exit 0 | INT, TERM: the same, 1.00 s after | **right** | |
| t3-r-2 | r | exit 0 | INT, TERM: the same, 1.00 s after | **right** | |

**Beyond the rule, said as such**: t2-x-1 with `func` renamed (`K/extra/t2-x-1-next/`) is refused
next by `error[ffi_missing_header]: threads.h is not on this machine's include path` (the SDK has
none, `ls $(xcrun --show-sdk-path)/usr/include/threads.h`: no such file).

**The registered decision rules, applied**: B1, over t1 and t2, x **0** right of 4, y **4** of 4, z
**0** of 4: T1's arm is +4 against the rule's +2, **T1 enters by the rule**; T2's arm is +0, **T2
does not**. B2: p 2 of 2, q 2 of 2, r 2 of 2, **a three-way tie**, said as a tie; arm x, what a
reader writes today: one right with a Heroes signal handler, one refused.

**The spec-warden's registered predictions, scored**: **W1 holds by its falsifiers** (x holds 0
`right`; no x answer refused by `ffi_callback_type` alone), but one of x's four (t2-x-1,
`reserved_word`) failed in a class W1's list does not name. **W2 holds** (y at x + 4, z at x + 0).
**W2's argument does not**: it reads *"a T2 reader still has to guess `hero_thread_spawn`"*, and all
four z answers (and t2-x-2 under x) wrote `hero_thread_spawn` and `hero_thread_join` by name with the
body `(function(i64) -> i64)` right; each failed on a **handle the header does not have**, `record
Thread tag hero_thread` with `-> Thread acquires hero_thread_join`, where the header answers
`int64_t`. The mistake T2 leaves open is the result's type, which none of the spec-warden's nine
probes (`q1a` to `q6`) tried, and the message it meets, *"check the spelling, or name the header that
does declare `hero_thread`"*, does not say the handle is an `i64`.

**The two sessions that saw the folder's path.** t1-x-2 (*"The working-directory path mentions
"heroes-lang" and "llm-ergonomist""*) and t1-z-1 (it names `p246-scratch/246-llm-ergonomist/t1-z-1`)
each answered `context` **no**, so the registered rule voids neither. Each gave the class and the
shape of its own twin, which did not mention the path (t1-x-2 stopped as t1-x-1, its own pthread
header; t1-z-1 refused as t1-z-2, the same `hero_thread` guess at the same binding), and neither used
anything the spec does not say: the one an x reader cannot know, `hero_thread_spawn`, is absent from
t1-x-2, and t1-z-1's invented tag is the one every z answer invented. **And the path was not theirs
alone**: 17 of the 20 `context` answers name a working directory (`python3 -I` over each `result`'s
`## context`: all but t1-y-2, t2-z-2 and t3-r-1), so voiding the two that read it aloud would leave
the same exposure in fifteen that did not. Voiding both changes no decision: x 0 of 3, z 0 of 3, y 4
of 4. Under the hard stop every folder a blind session can have is under
`/Users/joseph/Temp/heroes/heroes-lang/`, the root's own path; whether panels 239's and 244's
sessions read it too is unrun here, and a folder name that says less than `246-llm-ergonomist/`
(`blind/s07`) is a question for the next sheet.

### 06:50 to 06:52, claims asserted and not measured (each named by the seat's own words)

- **The compiler-engineer, route A's replay**: *"Each of the six states a kill can leave a row in ...
  replays to the tree the run found, and a second replay changes nothing. That sentence is a reading
  of the replay"* (its own words); four of the six were run. **Its (f) and (g)** measured a live run
  held long (slowed `git`) and two recoverers over one dead folder; **no run of it asked a live run
  at its birth**, which is where my harness above finds the windows.
- **The compiler-engineer, Windows**: route A's Windows arm (`FILE_SHARE_DELETE`, `LockFileEx`, the
  lock released before the folder is removed) and S1's `SetConsoleCtrlHandler` are compiled with `zig
  cc`, never run; the release-before-remove window above has no identity check on that arm at all
  (read). **The lockless fallback** where `flock` is refused (`ENOTSUP`, `ENOLCK`): owed, unbuilt, by
  its own account; the historian read in Cargo's `flock.rs` that on NFS *"the failure mode ... is
  blocking forever, even if the "non-blocking" flag is passed"* (cargo issue 2615, not read): if so,
  a `heroes check` on an NFS home folder would hang in `recovered` before it compiles anything, a
  question no seat can run here (no NFS share).
- **The compiler-engineer, the compiler's own tests at the base**: *"very likely hold one red test
  (defect 2355's ...), an inference from route A's run over byte-identical inputs, unrun on the
  base"*. Run by me below.
- **The compiler-engineer, S3**: *"S3's re-blessing is every line after the library's bindings in up
  to 861 emitted files, and the `lldb` sessions that pin `#line`s are a question (unrun)"*; and S1's
  flag *"`static _Atomic int`, lock-free on every target"*, compiled for Windows, not asked there
  (`ATOMIC_INT_LOCK_FREE` read on no target).
- **The ffi-pragmatist**: route A *"can still remove a remote live run's folder"* on a mount whose
  locks are local (`locallocks`, `local_lock=`): from manual pages, unrun; two PID namespaces on one
  bind mount: unrun; its `tree-rung` stop's Rule 3 standing (*"would owe two `SHARED_BY_DECISION`
  lines"*): the `runtime` suite was run on `tree-door` (the lock) and never on `tree-rung` (the
  stop); its Windows `CTRL_CLOSE_EVENT` wait of 4,500 ms: compiled, unrun; *"a Heroes handler there
  always meets `thread.c`'s guard"*: Microsoft's pages, unrun.
- **The spec-warden**: W3 (T1 at +80 to +110 real) needs a `--refresh`, a paid call nobody ran; its
  reading that S3 sits where the `spec` suite's `offered` check does not look, *"mine and unrun"*;
  *"BSD `signal` restarts it in the kernel anyway"*, read; question 6's *"Copy-on-write owes no spec
  token ... my reading, not a run"*: the compiler-engineer's `b3/constshare.hero` ran that shape
  under ThreadSanitizer, 0 warnings, so the reading has a run behind it now, in another seat's report.
- **The historian**: its search line says *"the synthesis runs `grep -n -i
  'signal-hook\|set_wakeup_fd\|controlling_process\|lockedfile\|temproots' docs/panel/*.md` before any
  prior-art line is filed"*. Run by me from `.claude/worktrees/p246`, panel 246's own files left out:
  **nothing printed**, so no earlier sitting cites those five names. Cargo issue 2615, Darwin's own
  `kqueue(2)`, `flock(2)` and `accept` pages: not read, by its own account.
- **The coordinator's blind sheet**: *"Graded with a compiler that holds the arm's exact spelling"*:
  held (above); *"a program that may not end is run as its binary under `timeout`"*: the spec-warden
  measured a plain `timeout` outlived by a program that catches SIGTERM, and every t3 answer of p, q
  and r catches it, so the grade above used SIGKILL to the group as the bound.

### 06:48 to 06:52, the question under defect 2453's class, measured

Defect 2453 was reclassed `blocking` on *"a silent loss of the author's work"*: a later run handed a
dead run's number removes `replaced-N`, *"the one copy of the author's edit, silently at exit 0"*
(00-shared.md § Part A). **What an ended `fetch` does to the same edit** (`K/nocurl/fx/app`, the
compiler-engineer's fixture copied, `git init -q`, my base compiler, `curl` on `PATH`): `fetch`
placed `small` and `large`; one line appended to `small`'s placed `a.hero`; `check main.hero`
printed `error[package_unread]: ... the tree placed at heroes.packages/small@1.0.0-0ce32cb1800c77e7/ is
sha256-33c8..., and the catalogue's sum for it is sha256-1909...: a file of it was changed, added or
taken out since it was placed` and, after the compiler's dash, `heroes fetch places it again`, exit
1; then **`fetch` printed
`placed small 1.0.0 ...`, `kept large 1.0.0 ...`, exit 0, and `grep -rl 'the author edited this' .`
found nothing**; `heroes.packages/` held the two placed trees and no `.fetching-*`. So an ended
`fetch` removes that edit with no word about it, every time, by design; panel 235 counts *"the placed
tree edited"* among the cases that must exit 1 (`grep -n edit docs/panel/235-*.md`: 386, 645-649),
and `check` tells the author `heroes fetch places it again` before it happens. And a refused `fetch`
puts the edit back (`staging.undone`), so a killed run's `replaced-N` is lost against what a refused
run would have done, and never against what an ended one does. No brief and no report asks which of
the two a killed run is owed, or whether an edit inside a placed tree is the author's work at all;
the answer decides whether 2453 is `blocking` (the tag waits) or leftovers (`adjacent` or
`improvement`, the tag does not), and route A's put-back (the compiler-engineer's choice in (d))
only hands the edit to the next `fetch`, which drops it.

### 06:52 to 06:54, the compiler's own tests at the base, run

`K/tree/heroes-next test selfhost/main.hero` (my stage 2, `sources 796d1133542b1909`): **`2108
tests, 1 failed`**, exit 1 (`real 340.54`, `user 240.75`: not a duration). The one: `FAIL "every word
a module hands clang is on the line the edits are asked of (defect 2355)"`, `assert failed: unlisted
== []` at `cli/clang_jobs.hero:182:12`, `left: ["selfhost/cli/doctor.hero: -dumpmachine"]`. The
compiler-engineer's inference is now a measurement **of the base**. `git -C
.claude/worktrees/b33-round log -L124,124:selfhost/cli/doctor.hero ef50a7478 --no-patch` (the line
holding `"-dumpmachine"`) names **`ef50a7478` itself**, the round's merge of lane `core-platform`. So
the head of batch 33's round holds a red compiler test that the round's gate owes; it is no part of
defect 2453 or of the server step.

### 06:54 to 06:56, routes nobody listed, two of them run

- **One lock per program, shared by every run and taken exclusively by recovery** (Part A). Every run
  holds a **shared** `flock` on one file, `heroes.packages/.runs`, never removed, for its whole life,
  taken **before** it makes any folder; recovery takes an **exclusive** `flock` without waiting and,
  only when it gets it (no run alive), undoes every `.fetching-*`. No lock inside a folder, no
  identity question, no window at a birth. Cargo's package cache
  coordinates *"multiple cargos ... running at the same time"* with modes of one lock (the
  historian's `cache_lock` line). **Run** (`K/lockrace/shared.c`, `clang -std=c11 -O1 -Wall -Wextra
  -Werror` exit 0, the same load): 3 continuous recoverers, 3,172 and 3,836 owners: **0 violations, 0
  births lost, 0 gone**, dead folders standing when the owners stopped 17 and 0, **0 after one quiet
  sweep**; each owner's own sweep at its start instead, 3,880 and 4,092 owners: **0, 0, 0**, 18 and 0
  standing, 0 after one quiet sweep. Its cost, read: recovery waits for a moment no run is alive, so
  an editor that keeps a `check` always running defers it; R12's *"a lock ... beside the catalogue"*
  is the reading it must answer, as route A's must; Windows' `LockFileEx` without
  `LOCKFILE_EXCLUSIVE_LOCK` is a shared lock (unrun, no box).
- **A timed `accept`** (Part B). `grep -n -i 'timeout\|deadline\|wait\|poll' runtime/hero_net.h` finds
  only the two comments *"waiting for one"* (lines 77, 87): the layer has no call with a deadline,
  though `HERO_NET_TIMED_OUT` (7) is a kind. Python's `socketserver.serve_forever` *"Poll[s] for
  shutdown every poll_interval seconds"* (the historian quotes it, filed under (c)): an `accept` that
  returns at a deadline lets S1's flag, asked between returns, stop a server whichever thread took the
  signal, with no descriptor and no `EINTR` question. Unbuilt; its price is a wake every interval and a
  stop up to one interval late.
- **Liveness by the pair (process number, start time)**, no lock file at all (Part A): a folder named
  `.fetching-<pid>-<start>-<n>` is dead where no process of that number started at that time lives
  (`proc_pidinfo` on Darwin, `/proc/<pid>/stat`'s start time on Linux, `GetProcessTimes` on Windows).
  Unbuilt; three doors, and a second machine on a shared mount reads every run as dead, as route A
  does under local-only locks.
- **A signal thread** (`sigwait` on a runtime thread, every other thread blocking the stop's two):
  named in the historian's brief under (a), built by no seat; it meets the compiler-engineer's
  `child2` measurement (a child started with the stop's signals blocked ran on deaf, `survived`) unless
  `run.c` clears the two before `exec`.

### 06:56 to 06:59, a Heroes function as a POSIX signal handler, run

Every program-side stop of this sitting binds a **Heroes** function as the handler through C's
`signal`: the coordinator's `h.hero` (it prints and calls `exit`), the critic's `stop1`, the
spec-warden's `q4-accept` and `q5-*`, the compiler-engineer's `exit1`, the ffi-pragmatist's
`whichthread`, and the blind arm x's one right answer, t3-x-1. `grep -l -i 'async-signal'
docs/panel/*.md` names 076 and 156, neither about a Heroes handler; no report asks whether a Heroes
body is safe inside a handler on the thread it interrupts. **Run** (`K/probes/handler/`, my base
compiler; driver `K/tools/storm.py`: the binary started, 0.3 s to arm, then `SIGUSR1` sent as fast as
`kill` returns for 4 s, then 30 s allowed and SIGKILL to the group; `pgrep` empty after):

| handler | main | runs | outcome |
|---|---|---|---|
| none sent (control) | 200 rounds of 100,000 `f"word {r} {i}"` pushed and dropped | 1 | `built 20000000 words`, exit 0 (`real 6.73`) |
| `said = f"caught {sig} while main was allocating"` (allocates) | the same | 5 | **5 of 5 died**: 3 `panic: the process is dying of SIGTRAP, under storm.on_signal, and this runtime did not raise it ... memory given back twice among them`, exit -5; 2 killed by SIGKILL (-9) with nothing on stderr; 13,032 to 20,381 signals sent |
| the same, `--sanitize` | the same | 2 | **2 of 2 died**, SIGKILL, no report (19,220 and 23,214 sent) |
| `_ = getpid() + sig` (allocates nothing) | the same | 3 | 3 of 3 `built 20000000 words`, exit 0, about a million signals each |

So a Heroes handler that allocates (an `f`-string, a `print` of one, a `+` on `str`) re-enters the
runtime's allocator from inside the signal and the program dies of a heap the C library calls
misused; one that allocates nothing survived. The compiler accepts both at exit 0 and says nothing.
A storm is a stress, not a field rate: one Ctrl-C lands inside `malloc` rarely, and lands there
sometime. Not run: Linux, and a single signal's odds.

Beside the grade, for B3: t2-y-2 (`constant SENTENCES: [str]` read on four threads) built with
`--sanitize` and run three times: `4 2 6 2 14`, exit 0, 0 bytes on stderr each time.

## The list: each item the fact, the command, what it printed, what the synthesis must do with it

**Contradictions between seats**

1. **Route A's lock: the compiler-engineer's stands inside the run's folder, after it is made, and is
   released before the folder is removed; the identity check is in it, on POSIX only.** Command:
   `diff -u 246-compiler-engineer/tree/selfhost/fetch/staging.hero tree-a/...`, `cat
   tree-a/runtime/parts/lock.c tree-a/selfhost/fetch/leftover.hero`; then `K/lockrace/race.c`
   driving that `lock.c`, byte for byte (`cmp`: same), in that order, under the ffi-pragmatist's
   load. Printed: with 3 continuous recoverers, births lost 182, 178, 233, folders gone under a live
   run 67, 41, 42, violations 1, 1, 0, lockless corpses 5, 3, 6; with each run's own sweep only, lost
   17, 20, 15, gone 2, 3, 0, corpses 2, 5, 8; the log of one run: 4,067 births, 4,067 distinct
   numbers, every vanished folder born once and removed by a recoverer after its birth. The
   ffi-pragmatist's `proto.c`, beside, identity on, the same load here: 0, 0, 0; on Linux arm64 in
   its log: 0 and 0, twice. **The synthesis**: neither seat's *approve* covers the other's shape;
   only the ffi-pragmatist's (the lock beside the folder, taken before `mkdir`, the folder removed
   while it is held, dropped after) read 0, 0, 0 on both platforms, and `tree-a` as built is not
   that shape; say which placement and order is adopted, and that the identity check does not
   close the window a release-before-remove order opens (Windows' reason for that order is answered
   by the lock standing outside the folder).
2. **The ffi-pragmatist's prediction about the identity check is falsified on Linux.** Command: its
   `linux-a.log`, A-4. Printed: beside, identity off, `VIOLATIONS 0` twice (3,494 and 3,627 owners),
   against 65 to 76 per 8 s on this Mac. **The synthesis**: the check's necessity is a Darwin
   measurement; keep it (it costs a `stat`) and do not cite Linux for it.
3. **"(c)" names three mechanisms, and the compiler-engineer's does not stop a server whose
   `accept` is on a worker.** Command: `K/probes/acc-worker/s1c.hero` and `rung.hero`, my base
   compiler beside copies of `246-compiler-engineer/s1c/runtime` and `246-ffi-pragmatist/tree-rung/runtime`
   (`diff -rq`: equal), `python3 -I K/tools/t3run.py <bin> SIGTERM|SIGINT`. Printed: `s1c`, both
   signals, `NOT ended 10 s after the signal: SIGKILLed`; `tree-rung`, both, exit 0 at 0.00 s,
   `acceptor answered 0, stop asked 1`; the control, the compiler-engineer's own `accept2.hero` on my
   `s1c` copy, exit 0, `accept returned nothing`. The historian's (c), closing the listener, does not
   wake `accept` on Darwin (`MADE`) and does on Linux (`refused`), in the ffi-pragmatist's logs.
   **The synthesis**: the compiler-engineer's own `condition` (*"(c) does not wake a thread blocked
   in `accept` (the signal taken by another thread), which would send me to route (b)"*) is met on
   Darwin; if a stop is adopted, "(c)" must be written as the layer waiting on a stop descriptor (the
   ffi-pragmatist's, measured on both platforms), never as `EINTR` alone or as closing the listener.
4. **The spec-warden's veto of S1 to S3 is within its seat, and the only thesis measurement is on
   the shape it calls blind.** Command: `sed -n 45,46p .claude/agents/spec-warden.md` (*"or if
   Principle 0's burden of proof is unmet"*); the t3 grade below. Printed: x one `right` of two; p, q
   and r two of two each. The blind sheet registered a bar for admitting a T sentence (+2 over x) and
   **none for admitting a stop**, only for choosing among spellings. **The synthesis**: record that
   t3 measured +1 of 2 for every spelling against x, below the bar B1 set for itself, on a `sleep`
   loop, and say in so many words whether it reads that as the thesis effect Principle 0 asks for;
   a seat's veto is a refusal (CLAUDE.md § 4), and the historian's *approve with (b) or (c)* is
   advisory beside it.
5. **The numbered handle and the handle-moving rung are the same two doors; the spec-warden's
   danger is answered for a stale number and not for a dropped one.** Command: the
   compiler-engineer's `netnum/` and the ffi-pragmatist's `carry.h`, read; `K/probes/numbered/dropped.hero`
   (`carry.h` copied), plain and `--sanitize`. Printed: a stale number `send -1, kind 2` in both seats'
   runs (Darwin and Linux); a socket turned into its number and never taken back, **exit 0, nothing
   on stderr, twice**. **The synthesis**: name the doors once (`hero_net_number` and `hero_net_of`, or
   whatever it rules) and say that once a handle is an `i64` the marks stop counting it: a number
   dropped is an open descriptor no check reports, the class task 1072's long run exists to catch.
6. **What crosses: two `str` rungs and one unruled narrowing.** Command: the two seats' door files,
   read; `git -C .claude/worktrees/b33-round log -S hero_thread_spawn -- runtime/hero_os.h
   runtime/runtime.c runtime/parts/spawn.c` and `show ea4a6fda7`. Printed: `ea4a6fda7` (2026-09-06
   12:22), *"AND THE ISOLATION IS THE TYPE RULE ... `crosses_the_boundary` judges the callback's own
   parameters"*, no sentence on R5 or on `str`. The compiler-engineer's rung **shares** by count (23
   lines, ThreadSanitizer 0), the ffi-pragmatist's **copies** (41 lines, 20 of 20 on both platforms).
   **The synthesis**: put R5 against journal 034:79 to the author with that commit body beside it (its
   author believed the checker decided, and the prototype narrowed); if a `str` rung is ruled, say
   which semantics, since the two measured clean on different shapes and never side by side.
7. **The compiler-engineer's Linux leg was void, and "no case needs curl" is a framing error that
   began in my first pass.** Command: `246-compiler-engineer/linux-a.no-curl.log` (and the 06:44 copy
   I read); `K/nocurl/`: the fixture with only `git` on `PATH`. Printed: `error[no_curl]` on every
   `fetch` and `add`, exit 2; here the same, exit 2, and with `curl`, `placed small`, `placed large`,
   exit 0. `selfhost/fetch/index_source.hero:1-3`: a static index is *"read with curl"*. **The
   synthesis**: my item 8 of the first pass wrote *"no case needs `curl`"* as a repair, 00-shared.md
   and both seat briefs copied it, and it was false of a `file://` index; the re-run with `curl` is
   the leg to cite: read whole at 07:03 (postscript), its Part A prediction holds on Linux.

**Claims asserted and not measured, and routes nobody listed**

8. **The base's own compiler tests hold one red test.** Command: `K/tree/heroes-next test
   selfhost/main.hero`. Printed: `2108 tests, 1 failed`, `FAIL "every word a module hands clang is on
   the line the edits are asked of (defect 2355)"`, `left: ["selfhost/cli/doctor.hero:
   -dumpmachine"]`; `git log -L124,124:selfhost/cli/doctor.hero ef50a7478` names `ef50a7478`, the
   round's merge of `core-platform`. **The synthesis**: none for this sitting; the coordinator owes it
   to batch 33's gate (a red the round's head carries), and the compiler-engineer's `test-a.log` red is
   the base's, not route A's.
9. **Unmeasured, by the seats' own words or by reading**: the compiler-engineer's six-state replay
   (four run), its Windows arms, its lockless fallback (owed), S3's 861 re-blessed files and the
   `lldb` sessions; the ffi-pragmatist's local-only network locks, two PID namespaces, `tree-rung`'s
   Rule 3 (the `runtime` suite ran on `tree-door` only), its Windows `CTRL_CLOSE_EVENT` wait; the
   spec-warden's W3 (+80 to +110 real, a paid `--refresh`) and its `offered`-check reading; the
   historian's Cargo issue 2615 (`flock` on NFS *"blocking forever, even if the "non-blocking" flag is
   passed"*, which would hang a `heroes check` in route A's recovery: a question, no share here).
   Run by me: the historian's `grep` of earlier sittings for `signal-hook`, `set_wakeup_fd`,
   `controlling_process`, `lockedfile`, `temproots`: nothing printed outside panel 246. **The
   synthesis**: carry each as unrun in its row, never as a premise.
10. **A route nobody listed for Part A, run: one shared lock per program.** Command: `K/lockrace/shared.c`
   (every run holds a shared `flock` on `heroes.packages/.runs` for its life, taken before any
   folder; recovery only under an exclusive one taken without waiting). Printed: 3,172, 3,836, 3,880
   and 4,092 owners, **0 violations, 0 births lost, 0 gone** in all four runs, dead folders standing
   while owners ran 0 to 18, **0 after one quiet sweep**. **The synthesis**: price it beside the
   ffi-pragmatist's beside-the-folder lock (both 0, 0, 0 here); its cost is recovery deferred while
   any run lives, and it owes R12 the same answer route A does.
11. **Routes nobody listed for Part B**: an `accept` with a deadline (`grep -n -i
    'timeout\|deadline\|wait\|poll' runtime/hero_net.h` finds none; `HERO_NET_TIMED_OUT` exists as
    a kind), so S1's flag asked between deadlines stops a server whichever thread took the signal;
    a `sigwait` thread (named by the historian's brief, built by no seat). Both unbuilt. **The
    synthesis**: list them as priced alternatives to the ffi-pragmatist's descriptor, or say why not.
12. **A Heroes function given to `signal` that allocates kills its program under signals.** Command:
    `K/probes/handler/storm.hero` and `quiet.hero`, `python3 -I K/tools/storm.py <bin> 4 0.3`.
    Printed: the allocating handler, 5 of 5 died (3 `panic: the process is dying of SIGTRAP, under
    storm.on_signal ... memory given back twice`, 2 SIGKILL), 2 of 2 under `--sanitize`; the handler
    that allocates nothing, 3 of 3 exit 0 over about a million signals each; the control exit 0. The
    compiler accepts both. **The synthesis**: the program's-own-handler route (feature 1069's
    re-check, `h.hero`, arm x's one right answer) is a robustness question before it is a token one;
    a stop whose handler is the runtime's C (S1 to S3, (c)) does not have it; the coordinator weighs
    filing it (a program that can corrupt memory accepted at exit 0 reads as `blocking` by
    `.claude/rules/verification.md` § Bounded discovery), no GitHub write by me.

**The blind seat**

13. **Graded by its registered rule** (the table below). Command: `K/tools/extract.py`, then each
    answer built with its arm's compiler (`K/tree/heroes-next`; `246-compiler-engineer/s1/heroes`,
    `s2/heroes`, `tree-s3/heroes-next`, run from `K/grade/`, nothing written in their folders) and run
    under `timeout -s KILL` or `K/tools/t3run.py`. Printed: B1, x 0 of 4, **y 4 of 4**, z 0 of 4; B2,
    p, q, r 2 of 2 each, x 1 of 2. **The synthesis**: by the registered rule **T1 enters** (+4
    against +2) and **T2 does not** (+0); B2 is **a three-way tie**, so the blind seat names no
    spelling; arm x's two answers are a Heroes signal handler (right, item 12's hazard with a handler
    that allocates nothing) and a refused `sigprocmask` by hand.
14. **W1 and W2 hold; W2's argument does not.** Command: the grade, and `grep -o
    'hero_thread_[a-z]*'` over each arm's spec (x and z: none). Printed: all four z answers and t2-x-2
    wrote `hero_thread_spawn` and `hero_thread_join` exactly, with the body type right, and were refused
    on an invented handle, `error[ffi_unknown_name]: hero_os.h declares no hero_thread`. **The
    synthesis**: T2's failure is the result's type, not the name; the message a z reader meets does not
    say the handle is an `i64`; t2-x-1's `reserved_word` is a failure class W1 did not list.
15. **The two sessions that saw the folder's path stand.** Command: their `## context`, and
    `python3 -I` over all twenty `result`s. Printed: both answered `no`; each matches its twin's class
    and error; 17 of 20 `context` answers name a working directory. **The synthesis**: void neither
    (the registered rule voids a `yes`; voiding both changes no decision, x 0 of 3, z 0 of 3, y 4 of 4),
    and say the path reached most sessions, not two.

**The question the sitting should have asked**

16. **Is an edit inside a placed tree the author's work that `heroes` owes?** Command: `K/nocurl/fx/app`,
    an edit appended to `small`'s placed `a.hero`, `check`, then `fetch`, then `grep -rl`. Printed:
    `check` `error[package_unread] ... heroes fetch places it again`, exit 1; **`fetch` `placed small
    1.0.0 ...`, exit 0, and no file holds the edit**; the compiler-engineer's own Linux re-run shows
    the same in its control 1a (`linux-a.log`, base: *"files holding the edit"* lists `large`'s
    `m0000.hero` before the later `fetch` and only the dead folder's `replaced-0/a.hero` after it, exit
    0); panel 235 lists *"the placed tree edited"* among
    the cases that must exit 1. Defect 2453 is `blocking` on *"a silent loss of the author's work"*,
    and an ended `fetch` loses the same edit silently by design, while a refused one puts it back.
    **The synthesis**: put to the author which of the two a killed run is owed, and whether such an
    edit is the author's work at all: if it is, the ended `fetch`'s silent discard is the larger,
    unfiled defect and route A's put-back only hands the edit to the next `fetch`; if it is not, 2453's
    loss is leftovers, its class is `adjacent` or `improvement`, and M-core-packages' tag does not wait
    on it. Either answer changes what Part A repairs, not only how.

## The grading table

| session | arm | grading compiler | build | run | class |
|---|---|---|---|---|---|
| t1-x-1 | x | base stage 2 | 0 | 134, the C-thread panic | stopped |
| t1-x-2 | x | base stage 2 | 0 | 134, the same | stopped |
| t1-y-1 | y | base stage 2 | 0 | `170666346666800000` twice, 0 | **right** |
| t1-y-2 | y | base stage 2 | 0 | the same | **right** |
| t1-z-1 | z | base stage 2 | 1, `ffi_unknown_name` `hero_thread` | | refused |
| t1-z-2 | z | base stage 2 | 1, the same | | refused |
| t2-x-1 | x | base stage 2 | 1, `reserved_word` `func` | | refused |
| t2-x-2 | x | base stage 2 | 1, `ffi_unknown_name` `hero_thread` | | refused |
| t2-y-1 | y | base stage 2 | 0 | `4 2 6 2 14`, 0 | **right** |
| t2-y-2 | y | base stage 2 | 0 | `4 2 6 2 14`, 0 | **right** |
| t2-z-1 | z | base stage 2 | 1, `ffi_unknown_name` `hero_thread` | | refused |
| t2-z-2 | z | base stage 2 | 1, the same | | refused |
| t3-x-1 | x | base stage 2 | 0 | INT and TERM: ticks 1 to 3, `stopped after 3 ticks`, 0, at 0.00 s | **right** |
| t3-x-2 | x | base stage 2 | 1, `empty_record`, `line_end_before_continuation` | | refused |
| t3-p-1 | p | `s1/heroes` | 0 | INT and TERM: ticks 1 to 3, `stopped after 3 ticks`, 0, at 1.00 s | **right** |
| t3-p-2 | p | `s1/heroes` | 0 | the same, at 0.00 s | **right** |
| t3-q-1 | q | `s2/heroes` | 0 | the same, at 1.00 s | **right** |
| t3-q-2 | q | `s2/heroes` | 0 | the same, at 1.00 s | **right** |
| t3-r-1 | r | `tree-s3/heroes-next` | 0 | the same, at 1.00 s | **right** |
| t3-r-2 | r | `tree-s3/heroes-next` | 0 | the same, at 1.00 s | **right** |

Totals: B1 (t1 and t2) x 0, y 4, z 0 of 4 each; B2 (t3) x 1, p 2, q 2, r 2 of 2 each. Not
provisional: every arm had a seat-built compiler.

## Postscript, 07:03: the compiler-engineer's Linux re-run, read whole

`246-compiler-engineer/linux-a.log` (467 lines, ending `linux-a.sh done`, `command -v curl`:
`/usr/bin/curl`; the void first leg kept as `linux-a.no-curl.log`, 25,204 bytes). Stage 2 in the
container: base `sources 796d1133542b1909`, route D `201bf3d413214e1f`, route A2 `7d9014d844216c9c`,
the Mac's three keys. **Base**: 1b, a later `fetch` handed the dead number, exit 0, **0 folders and
no file holding the edit**; 1c, a later `check`, exit 1, `small`'s edit gone; 1a and 1d keep the dead
folder and the edit. **Route D**: 1b and 1c keep the edit in the dead folder, renamed to the reused
number (`.fetching-59362-1`, `.fetching-62384-1`); every killed run's folder stands through every
later run. **Route A2**: 0 `.fetching-*` after every later run in phases a, b, c, d and f; 1c and 1d
put `small`'s edit back at its key; after the killed `check` and the killed `add`, `heroes.packages`
gone; f, the live run's first folder stood through the second `fetch` (`.fetching-147810-1`) and
none was left at its end; g, ten trials of two recovering `check`s at once, exits `1 1`, 0 folders,
the edit in place, **10 of 10**. So the compiler-engineer's Part A prediction for Linux (*"0
`.fetching-*` folders after every later run under `heroes-a2`, and the edit lost under the base in
its 1b and 1c"*) **holds**. What the leg does not ask, as on the Mac: a live run at its birth beside a
recovering run, the window item 1 measures. And its control 1a shows an ended `fetch` dropping
`large`'s edited `m0000.hero` at exit 0 on Linux too (item 16).
