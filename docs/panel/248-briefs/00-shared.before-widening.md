# Panel 248, the brief every seat reads first

Convened 2026-11-14 by the coordinator, in M-core-packages (`gh issue view 2522 --repo
heroes-lang/heroes --json milestone -q .milestone.title`: `M-core-packages`). **The full panel, all
five seats**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory) and
the llm-ergonomist (the blind seat); the completeness critic read these briefs before any seat
(`docs/panel/248-reports/critic-briefs.md`, 29 items, every one applied below; the first versions kept
as `248-briefs/*.before-critic.md`), then the three added briefs (its § Second pass, 15 items; the
blind seat's repaired before any session, items 3, 4, 5, 11, 12 and 14 applied to this file,
`spec-warden.md` and `historian.md`, the texts before kept as `*.before-second-pass.md`), and reads the
reports after. **It was first drafted for the
soundness lane and escalated on the critic's item 23** (§ The lane, below). **Five questions, each a
mechanism of the runtime or of its platform layer and the face it shows**: the C library's handler
installers and the runtime (defect 2522), the platform layer's waits (defect 2502), a child's working
folder (defect 2490), a stop the program asks itself, and a deadline on a connection.

**One paid run is named, the blind seat's** (`llm-ergonomist.md`): twelve `claude -p` sessions,
prepared and **unrun until the author's yes**, each capped at 0.40 USD, the proposed ceiling 5 USD;
without the yes the synthesis records the seat as unrun. **No other seat and no critic starts a paid
run.** The spec's real count by `heroes measure --refresh` is an API call to
`https://api.anthropic.com/v1/messages/count_tokens` (`selfhost/cli/refresh.hero:37`), which this
project's rules count among paid runs (`.claude/skills/panel/SKILL.md:292-293`, *"a `claude -p`
session, an API call, a cloud run"*); it is not named, and the spec-warden's brief says so.

**Where this brief is wrong, say so first.** Every number, path and count below carries the command
that produced it, run by the coordinator between about 08:45 and 09:15 and again, for the critic's
items, between about 09:40 and 10:30 on 2026-11-14, every file of those runs under
`.claude/worktrees/p248-scratch/briefs/` (the repairs' under `briefs/fix/`); a figure copied from an
issue, a report or an earlier sitting says *carried* beside it, and is yours to re-run before you lean
on it.

## The mandate

- **CLAUDE.md § 4** (its lines 200-201): *"Ask once per milestone, then convene without asking
  again"*. The milestone's gate was asked at panel 242: its status, `docs/panel/242-*.md:3-4`, *"convened
  without asking again (the author's panel gate for M-core-packages was asked at 07:35 on
  2026-11-10)"*; panel 247's, `247-*.md:3-4`, *"convened ... by the coordinator of M-core-packages under
  CLAUDE.md § 4 (a sitting asked for once per milestone; this milestone's run from 234)"*. The sittings
  since 234 (`ls docs/panel | grep -E '^2(3[4-9]|4[0-7])-[a-z]' | sed 's/-.*//' | sort -u`): 234 to 247,
  every number; the files naming M-core-packages anywhere (`grep -l 'M-core-packages'
  docs/panel/2*.md`): 234, 235, 240, 241, 242, 243, 246, 247; 237 to 239 name M-package-manager in their
  first twelve lines and 236 first at its line 75 (`head -12 <f> | grep -o -E 'M-[a-z]+(-[a-z]+)+'` and
  `grep -n -o -m1 M-package-manager` over each); 245 names no milestone at all (`grep -o -E
  'M-[a-z]+(-[a-z]+)+' docs/panel/245-*.md` prints nothing). So this sitting is convened without asking,
  and its one paid run is put to the author.

**Who sent each question here**, read today:

1. Defect 2522's *Where to look* (`gh issue view 2522 --repo heroes-lang/heroes`): *"the candidate
   repair is the C library's installers interposed in R4's corrected form, which lands only with a
   soundness sitting's ruling on the runtime defining the C library's names"*; panel 247's R4
   (`docs/panel/247-*.md:208-211`) and its ratification (`:449-451`, *"What it does not: R4's
   interposition, which waits for a soundness sitting"*).
2. Defect 2502's comment of 2026-11-13 (`gh issue view 2502`, posted twice, at 10:18 and 10:36, the
   same words): lane b35-runtime *"stopped on it, since it changes what `hero_net.h` promises"*, and
   recommended *"a soundness sitting, a follow-up to panel 246"*.
3. Defect 2490's own text, *"a change at the C boundary, so a sitting's question before it is
   built"*, and its comment of 2026-11-13 (posted twice the same way): *"a new door in `hero_os.h` for a
   child's working folder, staged as `hero_run_env` is, which decision 2228 makes part of the
   language's promised door"*.
4. and 5. The coordinator's, from batch 35's lane notes. **These briefs could not find the note**:
   `grep -rl -i -E 'stop_asked|ask.{0,20}stop|deadline|nothing.{0,30}(send|sent)|quiet connection|holds
   its thread' .claude/worktrees/b35-scratch --include='*.md' --include='*.txt'` printed 276 files
   (`briefs/fix/b35-grep.txt`): 227 under `site-repro/`, a copy of the repository's documents; and 49
   others, 24 under `b35-ffi/emitcmp` (build outputs), 10 under `b35-signals/runs2` (case runs), 4
   under `t2298/` (3 in `heroes.packages`, 1 in `build`), 4 close texts (`close/final` 2,
   `close/close-2513.md`, `close/bodies` 1), 2 under `b35-signals/swp6`, 3
   `b35-publish/driver-added*.txt` (the driver's emissions), `msg-spec-247.txt` and
   `fix/msg-skips.txt` (commit messages' drafts). The matches the critic read in them (a string
   *"the program was asked to stop"*, a declaration of `hero_net_stop_asked`) hold no such note. No
   issue exists for either (the searches are in each question's section).

## The lane: the full panel, and why

The draft read the questions as the soundness lane's (`SKILL.md:18-23`: *"Use it when the proposal
changes **no surface, no diagnostic and no spec token**: a runtime primitive, an emitter mechanism, an
internal representation"*). **The critic's item 23 showed three of the five have a sentence of
the specification or a diagnostic class behind them**, and `SKILL.md:33-34` decides it: *"A question
that *looks* internal but has a sentence in the spec behind it is a full panel."*

- **Question 4**: spec § 13's line 404 (`sed -n '403,404p' spec/heroes-spec.md`) names
  `hero_net_catch_stop() -> i64` and `hero_net_stop_asked() -> i64` and says what makes the second
  answer 1 (*"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1"*). A door that
  makes it answer 1 another way has that sentence behind it, whether or not *"only"* becomes false.
- **Question 5**: one of its routes, a join that does not wait (below), moves line 377's sentence
  naming `hero_thread_join(handle: i64) -> i64` (*"waits for its answer"*); and its face is a new
  function on the `net` package's card, which panel 246 weighed inside a full sitting
  (`246-*.md:351-355`).
- **Question 1**: a route that defines installers the runtime does not call today (`bsd_signal`,
  `sigset`, `sigvec`, `ssignal`, `sysv_signal`, Darwin's four undeclared names, below) widens which
  libraries panel 239's member `ffi_defined_elsewhere` refuses (`.claude/rules/c-boundary.md:186-220`:
  *"a C name a group binds, or the runtime calls or defines, that a library the program names other
  than the one meant would supply"*), a change to what a diagnostic class covers; and R4 itself asks
  that member *"amended in the same commit"*.
- **The precedents beside them sat in full**: panel 242, whose R6 made `hero_run_env` (`242-*.md:176-188`),
  four seats (its lines 3-7); panel 246, whose R4 made the stop, *"`3a`, a full sitting on the server
  step's two questions"* (its lines 3-8); panel 247, which put hg3 in the spec, all five.

**The escape, and it binds every seat**: a seat whose route needs a spec sentence (a door a reader
must be told of, or a sentence of line 377 or 404 that a new door makes false), **a new diagnostic or
a widened one** (a class that refuses what it did not, `ffi_defined_elsewhere` above all), or a change a
reader of the `net` package's card must learn, says so in its report in those words, so the synthesis
prices it with the spec-warden and the blind seat.

## The base

**`df7612ad3dc62dcc6d90bccc650adf6b9736078f`**, its tree **`f72f48da250e919a68ebc6a98be0facccc29a052`**
(`git rev-parse 'df7612ad3^{tree}'`), `git log -1`: *"Batch 35 round closed: the compiler counted again,
every gate read green"*, 2026-11-14 00:53:52 +0100, the head of branch `p248` in
`.claude/worktrees/p248` (the critic read the same, its section A). It holds batch 35's landing of
panel 247 (`runtime/parts/sigguard.c`, `runtime/parts/sigdoor.c`, spec § 13's lines 403-404). Every
path below is from the repository's root, `/Users/joseph/Temp/heroes/heroes-lang`. Every seat that
builds copies the commit and builds its own compiler in two stages:

```
mkdir -p .claude/worktrees/p248-scratch/248-<seat>/tree .claude/worktrees/p248-scratch/248-<seat>/tmp
git -C .claude/worktrees/p248 archive df7612ad3dc62dcc6d90bccc650adf6b9736078f | tar -x -C .claude/worktrees/p248-scratch/248-<seat>/tree
cd .claude/worktrees/p248-scratch/248-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 4c9a6a9800095eef
```

Measured by the coordinator at 08:55 to 08:58 in `.claude/worktrees/p248-scratch/briefs/base/tree`,
made exactly that way: the archive holds no `build/` (`ls build`: *No such file or directory*), the
seed and stage 2 both exit 0, `heroes-next --version` printed `heroes 0.6.0` and `sources
4c9a6a9800095eef`, and `heroes-next doctor` printed `ok   runtime      runtime`. The same `sources`
line came out of the same two stages in the Linux arm64 image (below). The durations, on a machine at
load average 34 (`uptime` at 08:58: *34.41 84.57 50.15*), are not measurements. **A seat whose stage 2
prints another `sources` line is not on the base and says so first. No duration a seat writes is a
measurement while other sessions run** (CLAUDE.md § Verification, CL-025): count instructions
(`/usr/bin/time -l`, *instructions retired*), never seconds, or say why a clock was still. **The copy
has no `.git`**: `.claude/rules/platforms.md` § Measuring in a copy says the `records` suite cannot run
there; whether the net's own tests (`./heroes-next test tests/harness/main.hero`, which the
compiler-engineer's question 3 asks) pass in such a copy is unrun, posed (the critic's item 29): a seat
that runs them says what they need of `.git`, if anything.

## Where each seat works, and the rules every seat obeys

Every seat's folder is **`.claude/worktrees/p248-scratch/248-<seat>/`**, ignored by git (`git
check-ignore -v .claude/worktrees/p248-scratch/briefs`: `.gitignore:34:.claude/worktrees/`), never the
session scratchpad, never `/tmp`: CLAUDE.md § Hard stops writes no file outside the repository's root.
The blind seat's sessions stand apart, in `.claude/worktrees/p248-scratch/s/` (`llm-ergonomist.md`).
Read-only to the seats, copied into your own folder before use:

- `.claude/worktrees/p248-scratch/briefs/tools/storm.py` and `ips.py`, copied from
  `.claude/worktrees/p247-scratch/tools/` (`shasum -a 256`: `8e6b762a...` and `92071c5e...`, which the
  critic re-read): panel 247's storm driver (`python3 -I storm.py <binary> <SIGNAME> <seconds>
  <arm-delay>`, the binary in a session of its own, its output kept to 65,536 bytes a stream, SIGKILL to
  its group at the end) and its crash-report tally (`python3 -I ips.py ~/Library/Logs/DiagnosticReports
  <binary-name>...`, `SHOW=1` for one stack per kind). **The driver arms by the clock**: it sleeps
  `<arm-delay>` and sends whether or not the program has installed its handler (its lines 33-38, the
  critic's item 25), and the critic's C twin read *"sent 66 SIGUSR1; exit -30"* with no `armed` line in
  one run of three. **So every storm target prints `armed` once its handler stands, and a run whose
  output lacks it is not counted.**
- `.claude/worktrees/p248-scratch/briefs/ev247/`, panel 247's evidence unpacked
  (`tar -xJf docs/panel/247-reports/evidence.tar.xz`): `247-ffi-pragmatist/h/fliblocale.hero` (37 lines),
  `h/flib.hero` (19), `h/sigcb.h` (4), `h/lib/sigcb.c` (21) (`wc -l`), R3's two programs and their library;
  `247-ffi-pragmatist/c/ipose.c` and `ipose_lib.c`, the interposition prototype (the report's `F/c/`);
  `critic2/work/direct/direct.hero`, `dlocale.hero` and `critic2/work/hold/hold.hero`, 247's critic's.
  Never unpack an archive inside the worktree.
- `.claude/worktrees/p248-scratch/briefs/r2502/waiters.c` (67 lines), lane b35-runtime's probe of
  defect 2502, and **`waiters-checked.c`** beside it, the critic's copy with one line added (its line 55,
  the connect checked before the accept; § Question 2 says why); `.claude/worktrees/p248-scratch/briefs/r2522/`
  and `briefs/fix/` (the coordinator's runs). The lanes' and the critic's folders under
  `.claude/worktrees/` are read-only too.

The rules:

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool. Never the trunk's `./heroes`, a compiler of
  `.claude/worktrees/p248`, of `p248-scratch/briefs/`, of a lane or of another seat; never read, build
  or run inside another seat's folder, `p248-scratch/s/` or a lane's worktree.
- **Your report**: `docs/panel/248-reports/<seat>.md` in `.claude/worktrees/p248`, written as you go,
  not only at the end, in your definition's output structure (`.claude/agents/<seat>.md`). Write
  nothing else in `.claude/worktrees/p248`.
- **Every seat that builds builds the route it recommends, in its own copy, and runs the cases it
  names, before the synthesis** (`SKILL.md` 3c): a route that does not build is reported as not built,
  never adopted.
- **Every number with the command that produced it; every negative sentence run, or posed as a
  question**; the words *only*, *never*, *provably* with what was searched and where (`/panel` step
  2; CL-077).
- **No paid run but the blind seat's, and that one only on the author's yes**: no other `claude -p`,
  no API call, no `heroes measure --refresh`, no cloud run. One you find worth running goes in your
  report with its size and cost, for the coordinator.
- **Never print a credential**: `gh auth status` at most, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag. `gh issue view` and `gh issue list` are reads and allowed.
- **One binary name per seat** (the critic's item 24: both seats were pointed at `-o fliblocale`, and
  `ls ~/Library/Logs/DiagnosticReports | grep -c '^fliblocale-2026-'` already prints 3, the
  coordinator's): every binary you build is named with your prefix, **`ce-`** for the compiler-engineer,
  **`fp-`** for the ffi-pragmatist, **`sw-`** for the spec-warden (`ce-fliblocale`, `fp-flib-nofp`),
  `ips.py` asked of those names alone, and every report you count read by the pids your storm printed.
- **Every Darwin death reads its crash report**: a recursive lock in libplatform traps
  (`EXC_BREAKPOINT`) and the report names SIGKILL, with nothing on stderr (the critic's section B, a
  plain C program locking one `os_unfair_lock` twice, `exit 137` with the same report), so a run is
  judged by its `.ips` reports, which are a sample, not a count.
- **Every program that may not end has its bounds named here**, and a seat that runs another names
  its own in its report first (`.claude/rules/verification.md` § A run that may not end is bounded in
  time and in bytes; the critic's item 27). Built with `heroes build` or clang, the BINARY run under
  `timeout -s KILL <seconds>`, never `timeout heroes run` and never a plain `timeout`, whose SIGTERM a
  program that caught the stop outlives; its output to `/dev/null` or through `head -c 65536` into a
  file; afterwards `pgrep -f <your folder>` read and anything left stopped before a verdict. Here
  `timeout` is GNU coreutils 9.12 (`/opt/homebrew/bin/timeout`), which kills the command's process group
  (`timeout -s KILL 2 sh -c 'sleep 31 & sleep 32'`: exit 137, `pgrep` found no `sleep` after), so a C
  child forked by the probe dies with it; **a child the Heroes runtime spawns has a group of its own**
  (`runtime/parts/run.c:1372`, `setpgid(0, 0)`) and does not, so look for it by its own name. The
  bounds:

  | program | time | bytes |
  |---|---|---|
  | a storm's target | `storm.py`'s own (30 s after the storm, then SIGKILL to the group), run unwrapped | 65,536 a stream, the driver's |
  | `waiters` / `waiters-checked` | `timeout -s KILL 60`; under `ulimit -n 256`, `timeout -s KILL 30` | `head -c 65536` |
  | a server of question 4 or 5 | `timeout -s KILL 60` | `head -c 65536` |
  | each client of question 4 or 5 (`curl`, or a probe of yours) | `curl --max-time 5`, or the probe's own 10 s limit, the whole client side under `timeout -s KILL 90` | `head -c 65536` |
  | a ThreadSanitizer probe (question 4) | `timeout -s KILL 120` | `head -c 65536` |
  | a fork-and-`chdir` stress (question 3) | a fixed count of forks, at most 10,000, under `timeout -s KILL 120` | `head -c 65536` |
  | a Docker run | below, § The platforms | `head -c` inside the script |

- **One heavy command at a time** (a stage 2 build, a suite): other sessions build on this Mac.
  Whole suites only where your route changes what the checker refuses, named in your report with why;
  otherwise the `run` form narrowed to your cases (`./heroes-next run tests/harness/main.hero --
  ./heroes-next run <case-substring>`), one at a time.

## The platforms

- **Darwin arm64**: this Mac, Apple clang 21.0.0 (`heroes-next doctor`: *Apple clang version 21.0.0
  (clang-2100.4.1.1)*), Apple's linker `ld-27106.1` (`ld -v`). `pmset -g assertions` read
  `PreventUserIdleSystemSleep 1` at 08:50, so something already holds the machine awake
  (`.claude/rules/verification.md` § A long run holds the machine awake): ask it again before a long
  run, `caffeinate -i` only where it reads 0.
- **Linux arm64**: the image `heroes-linux-arm64:latest`, `338a1044ac90` (`docker images
  heroes-linux-arm64`); inside it `uname -m` *aarch64*, `ldd --version` *Debian GLIBC 2.41-12+deb13u3*,
  *Debian clang version 22.1.8*, `ulimit -n` and `ulimit -Hn` both 1048576 (one `docker run --rm` of
  those five commands). Write a script into a folder of yours and run it from the repository's root:

  ```
  git -C .claude/worktrees/p248 archive df7612ad3dc62dcc6d90bccc650adf6b9736078f | timeout -s KILL 900 docker run -i --rm --name <prefix>-248-<n> -v <your folder>/linux:/out heroes-linux-arm64:latest bash /out/run.sh
  docker ps --filter name=<prefix>-248-<n> --format '{{.Names}} {{.Status}}'   # after it, whatever its exit
  docker stop -t 1 <prefix>-248-<n>                                            # where that line printed a container
  ```

  the script reading the tree from its stdin (`tar -xf - <&0` into a folder of the container's own),
  building the seed and stage 2 there, **every probe inside it under its own `timeout -s KILL`** (the
  image has GNU `timeout`), writing its log under `/out` and bounding every output with `head -c`.
  **The outer `timeout` bounds the client, not the container** (the critic's item 26, re-run by the
  coordinator: `timeout -s KILL 8 docker run -i --rm --name p248-coord-bound heroes-linux-arm64:latest
  sleep 40` printed exit 137, then `docker ps` *"p248-coord-bound Up 7 seconds"*; `docker stop` ended
  it), and `pgrep -f <your folder>` does not see a container's processes. One container of yours at a
  time. Measured with `.claude/worktrees/p248-scratch/briefs/r2522/linux/run.sh` at about 09:00:
  `docker exit 0`, stage 2 `sources 4c9a6a9800095eef`.
- **Windows: the box is OFF tonight.** `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at
  08:51 printed *"ssh: Could not resolve hostname apponfly-vps: nodename nor servname provided, or not
  known"*, exit 255, as the coordinator's own did at 08:47 and the critic's after. `.claude/rules/platforms.md`
  § The Windows box: *"Paid by the hour, usually off, and only the author can start it"*. **A seat that
  needs a Windows measurement names it in its report as owed**, and **the CI's Windows leg judges after
  the push** (`.claude/rules/verification.md` § The optimistic chain, its item 4). **The project's
  Windows target is MSVC's** (`docs/platforms/windows/WINDOWS-MACHINE.md:346` and `:357`, *"target
  `x86_64-pc-windows-msvc`"* for both boxes; `.github/workflows/ci.yml:540`, *"Windows carries clang
  with Visual Studio"*; `selfhost/cli/link_flags.hero:136` and `155-156` ask `-windows-msvc`), **and zig
  here compiles MinGW's alone**: `zig cc -target x86_64-windows-gnu -c` of a file including `windows.h`
  and `signal.h` exited 0, and `-target x86_64-windows-msvc -c` exited 1, *"'windows.h' file not found"*
  (`zig version` 0.15.2, at `/Users/joseph/.zvm/bin/zig`; `briefs/fix/zig/`). So a Windows arm compiled
  with zig, `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR` inside your folder, **stands for MinGW**,
  never run; where the MSVC toolchain would differ (a link flag, `--wrap` above all), say so. *How the
  box is driven when it is on*, the coordinator's practice, not run tonight: files under `/c/w/` in Git
  Bash; a long run started detached, a `.cmd` running `bash -lc "..."` into a log, started by
  PowerShell's `Invoke-CimMethod -ClassName Win32_Process -MethodName Create`, since an ssh session
  that drops kills its children; a file over a few kilobytes sent as base64 in chunks under 6,000
  characters, since cmd.exe truncates long lines.

## Question 1: defect 2522, and whether the runtime may define the C library's installers

**The defect** (`gh issue view 2522 --repo heroes-lang/heroes`: OPEN, `kind:defect, class:blocking,
area:runtime`, M-core-packages, filed 2026-11-13, no comments), panel 247's R3 residue: a library of
the program, built `-O2 -fomit-frame-pointer`, installs its own handler with `sigaction` and calls the
program's callback from it; the callback, a Heroes function that builds no value, passes all three of
R1's questions and D's guard. The issue's measurements, by the lane, *carried*: Darwin `exit -9` in 20
of 20 storms, every report SIGKILL in the locale lock; Linux arm64 exit 0 in 5 of 5, the function
running inside the signal through 1.35 to 2.17 million signals.

**R3's two cases, named** (`docs/panel/247-*.md:191-195`: *"a library built without frame records,
`flib-nofp` ... `c-fliblocale` dies by SIGKILL in silence on Darwin and runs inside the signal on Linux
arm64 ... Its cases are those two"*; the critic's item 14), each built with the library panel 247's
Linux script built (`ev247/247-ffi-pragmatist/linux-247.sh:108`, **a static archive**) and the
programs as its lines 115 and 117 build them:

```
clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. lib/sigcb.c -o lib-nofp/sigcb.o && ar rcs lib-nofp/libsigcb.a lib-nofp/sigcb.o
./heroes-next build flib.hero --library lib-nofp -o <prefix>flib-nofp          # one raise from main, the library's handler calling back
./heroes-next build fliblocale.hero --library lib-nofp -o <prefix>fliblocale    # a storm; the callback calls localeconv while main formats
```

**Run at the base by the coordinator** (`.claude/worktrees/p248-scratch/briefs/r2522/`, the base's
`heroes-next`, every build exit 0):

- **`flib-nofp`, Darwin arm64**, `timeout -s KILL 20 ./coord-flib-nofp`: exit 0, stdout `0`, `answer
  ran for 30`, `0`, `60`, stderr empty: the Heroes function ran inside the signal. Linux arm64 unrun
  tonight.
- **`fliblocale`, Darwin arm64**, `python3 -I ../tools/storm.py ./fliblocale SIGUSR1 4 0.3` three
  times at 08:59:40: **3 of 3 armed, 3 of 3 `exit -9`**, after 17,070 to 17,777 signals, stderr empty;
  `ips.py ... fliblocale`: **3 reports, all SIGKILL, the lock**, their pids the storm's three (52205,
  52208, 52211, the critic's reading), `SHOW=1`'s stack `_os_unfair_lock_recursive_abort <
  _os_unfair_lock_lock_slow < localeconv_l < h_fliblocale_answer < h_0cb_fliblocale_answer <
  sigcb_handler < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk < hero_int_to_str <
  h_fliblocale_round < h_fliblocale_main`.
- **`fliblocale`, Linux arm64** (`r2522/linux/run.sh`, the line in § The platforms without `--name`,
  which this run predates): **3 of 3 exit 0**, `built 20000000 words`, after 2,102,440 to 2,240,205
  signals, no refusal.

**The Darwin death needs no Heroes code** (the critic's item 15, re-run): a C program whose own handler
calls `localeconv` while `main` formats with `snprintf` (`briefs/fix/ctwin/coord-ctwin.c`, the critic's
`critic-ctwin.c` copied, `clang -O2 -fomit-frame-pointer`), three storms: **3 of 3 armed, 3 of 3 `exit
-9`**, after 16,566 to 16,848 signals; `ips.py ... coord-ctwin`: `('coord-ctwin', 'SIGKILL', 'lock') 3`,
stack `_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < localeconv_l < handler <
localeconv_l < __vfprintf < _vsnprintf < snprintf < main`. **So what a route must close differs by
platform**: on Darwin the SIGKILL is the C library's own re-entry, and what R3 asks is a refusal
**before** the Heroes function's first C call, at the thunk's first line, so the program ends with
R1's words instead of a silent kill; on Linux arm64 the program exits 0, and what is broken is panel
246's R4 alone (Heroes code inside the signal). Every route below says which of the two it closes on
each platform.

**What the base does** (`wc -l`: `runtime/parts/sigguard.c` 364): `hero_callback_entered`, the thunk's
first line (emitted at `selfhost/emit/callback_thunk.hero:146`), asks the exact test at
`sigguard.c:317-320` (the thunk's return address against the delivery's, learned at the first crossing
of a function into C by `hero_signal_learn`, 196-227), refuses a thread nothing claimed at 321-325,
asks `SS_ONSTACK` where the frame lies outside the thread's stack at 330-334, and walks at most
`HERO_SIGNAL_WALK` (8, line 156) frame records at 337-344. **Its probes install through the C library's
own `sigaction`**, *"never through a definition of this runtime's (a runtime that defined it learned
its own wrapper's address and refused nothing: panel 247's critic)"* (175-186). What it does not see is
written at 49-57: *"a handler whose frames keep no record (a library built `-fomit-frame-pointer`),
one more than HERO_SIGNAL_WALK frames below the Heroes function ..., and Windows"*; its Windows arm
(347-364) asks nothing. D, the allocator's guard (`runtime/parts/alloc.c`, its comment from 481), stands
beneath and does not fire here: the callback allocates nothing.

**The rulings that bind it**, each read today:

- Panel 247's **R3** (`docs/panel/247-*.md:191-195`): the residue is *"`blocking` ... Never a known
  cost: it blocks M-core-packages' tag like any `blocking` defect."*
- Panel 247's **R4** (`:197-211`), whose conditions any interposition meets, in its words: *"the
  probes through the C library's own `sigaction`; every installer each C library exports defined with
  its whole meaning (`SIG_HOLD`, `SIG_IGN`, `SIG_DFL`, the previous handler answered);
  `sigaction(sig, NULL, &old)` answering the program's own handler; `siglongjmp` out of a handler (the
  ffi-pragmatist's condition (a)); and R3's two cases refused 20 of 20. The runtime defining names of
  the C library is an architectural act no seat measured against panel 239's rule
  (`.claude/rules/c-boundary.md`, `ffi_defined_elsewhere`), so it lands, if it does, only with a
  soundness sitting's ruling and that rule amended in the same commit (CLAUDE.md § 4: architecture is a
  trigger)."* This sitting is that ruling's, sat in full.
- Panel 247's **R1** and **R2** (`:153-189`), the questions and D as they landed; **R10**
  (`:286-291`), Windows.
- Panel 246's **R4** (`docs/panel/246-*.md:318`): *"No Heroes code runs in a handler"*, ratified
  2026-11-12.
- Panel 239's member, `ffi_defined_elsewhere` (`.claude/rules/c-boundary.md:186-220`; the code,
  `selfhost/cli/supplied.hero`, 357 lines), told on the binding; and the line on the runtime's names
  (`:345-347`, *"a name the runtime calls or defines is the author's where a library a `link` or a
  `pkgconfig` answer of the program names supplies it"*).

**What panel 247 measured of interposition**, *carried* (its reports, read today, each yours to re-run):
the ffi-pragmatist's `F/c/ipose.c` (here `ev247/247-ffi-pragmatist/c/ipose.c`), the executable defining
`sigaction`, `signal` and `sigset` (`docs/panel/247-reports/ffi-pragmatist.md:173-191`): on Darwin a
static library without frame pointers *"seen, 3 of 3"*, a shared library *"missed, 3 of 3: Darwin's
two-level namespace binds a dylib's `sigaction` to libSystem's"*; in Linux arm64 *"every row seen
inside a handler, the shared library's three included, with or without `-rdynamic`"* (`:280-282`); what
it left (`:376-383`): installers it did not define, *"a raw `rt_sigaction` system call"*, and a handler
left by `siglongjmp`. 247's critic (`docs/panel/247-reports/critic-reports.md:84-110`): the probe
through the runtime's own `sigaction` blinded the exact test (`direct.hero`, `dlocale.hero`: 3 of 3
SIGKILL in silence), and `sigset(SIGUSR1, SIG_HOLD)` *"exit 134, a false message"*.

**The installers each C library exports, run tonight** (the critic's items 3, 4 and 5, each re-run by
the coordinator):

- **Darwin: ten names install a handler.** Every `sig`-prefixed export of the SDK's stubs (`tr -s "[],'
  \n" '\n' < $(xcrun --show-sdk-path)/usr/lib/system/<lib>.tbd | grep -E '^_+(sig|bsd_sig|platform_sig|psig)'`):
  `libsystem_kernel` `___sigaction` (C's `__sigaction`) beside `___sigaltstack`, `___sigreturn` and
  the wait and mask calls; `libsystem_c` `bsd_signal`, `sigaction`, `signal`, `sigset`, `sigvec`,
  `sighold`, `sigignore`, `siginterrupt`, `sigpause`, `sigrelse`, `psignal`, the set operations, and
  **`__sigaction_nobind`, `__signal_nobind`, `__sigvec_nobind`** (C's `_sigaction_nobind`,
  `_signal_nobind`, `_sigvec_nobind`); `libsystem_platform` **`___platform_sigaction`** (C's
  `__platform_sigaction`) beside `__sigtramp`, `_siglongjmp`, `_sigsetjmp`. The SDK's headers declare
  five that take a handler (`signal.h:82` `bsd_signal`, `87` `sigaction`, `102` `sigset`, `111` `sigvec`,
  `sys/signal.h:392` `signal`) and none of the four undeclared ones (`grep -rn -E
  '_sigaction_nobind|_signal_nobind|_sigvec_nobind|__platform_sigaction' $SDK/usr/include` printed
  nothing). **Run** (`briefs/fix/darwin/nobind.c` and `platform.c`, the critic's, `clang -O0`, each under
  `timeout -s KILL 20`, the executable defining `sigaction`): *"_sigaction_nobind ret 0 handler ran 1"*,
  *"_signal_nobind ret 0x0 handler ran 1"*, *"_sigvec_nobind ret 0 handler ran 1"*, *"calls that reached
  the executable's own sigaction: 0"*, and *"__platform_sigaction ret 0 handler ran 1"*. So the ten
  are the five declared, `__sigaction`, and those four, and a library calling any of the four passes a
  runtime definition of the declared five and of `__sigaction` (the critic's second pass, item 14; the
  probe defined `sigaction` alone, so the `__sigaction` half is the critic's reading, unrun here).
  `sigignore` and `siginterrupt` on Darwin are unrun.
- **glibc 2.41, Linux arm64** (`briefs/fix/linux/run.sh`, one container, exit 0, `fix-linux.log`): the
  exported names by `nm -D --defined-only /lib/aarch64-linux-gnu/libc.so.6`, the filter of the first
  draft: `__sigaction`, `__sigpause`, `__sysv_signal`, `bsd_signal`, `sigaction`, `sighold`,
  `sigignore`, `siginterrupt`, `signal`, `sigpause`, `sigrelse`, `sigset`, `sigvec`, `ssignal`,
  `sysv_signal`. **Their prototypes** (`grep -n` of `/usr/include/signal.h`): `__sysv_signal` 77,
  `sysv_signal` 80, `signal` 88, `bsd_signal` 104, `ssignal` 127, `sigaction` 243, `siginterrupt` 324,
  `sigignore` 363, `sigset` 367, the handler takers each `extern __sighandler_t <name> (int __sig,
  __sighandler_t ...)`. **`sigvec` is a compat symbol only**: the unfiltered list prints
  `sigvec@GLIBC_2.17` with one `@` (`sigaction@@GLIBC_2.17`, `signal@@GLIBC_2.17` the defaults), and a
  fresh link calling it printed *"undefined reference to `sigvec'"*, so a library built today cannot
  call it and only a binary linked against an older glibc does: **eight linkable handler takers**
  (`__sigaction`, `__sysv_signal`, `bsd_signal`, `sigaction`, `signal`, `sigset`, `ssignal`,
  `sysv_signal`), `sigvec` reachable from an old binary alone, and **`__libc_sigaction@@GLIBC_PRIVATE`**
  exported, private.
- **Which of them reach an executable's own `sigaction`, run** (the critic's `reach.c` and its shared
  library, re-run in the same container): from the program, `signal`, `bsd_signal`, `sysv_signal`,
  `__sysv_signal`, `ssignal`, `sigset`, `__sigaction` each *"handler ran 1, reached the executable's
  sigaction 0"*, `sigaction` *"reached ... 1"*; from a shared library, `signal`, `bsd_signal`,
  `sysv_signal`, `ssignal`, `sigset` reached 0 and `sigaction` 1. **And two names that take no handler
  change what a definition's table would answer**: `siginterrupt` reached it 0 times, and `sigignore` 0
  times with *"disposition now SIG_IGN 1"*: a route that keeps the program's handler to answer
  `sigaction(sig, NULL, &old)` is made stale by a library's `sigignore` or `siginterrupt`, so they
  belong in its list of what must be defined or mirrored.
- **Windows**: UCRT's installers are unread; **the runtime itself installs with more than `signal`**
  (`grep -n -E '(SetConsoleCtrlHandler|SetUnhandledExceptionFilter|AddVectoredExceptionHandler)\('
  runtime/parts/*.c runtime/platform/*.c`): `AddVectoredExceptionHandler` at `os.c:318`, `stack.c:618`
  and `stack.c:1210`, `SetUnhandledExceptionFilter` at `os.c:319`, `SetConsoleCtrlHandler` at
  `hero_net.c:539`; a program's DLL may call any of them, and the seat says which reach where.

**The runtime's own installs** (`grep -n -E '(^|[^_a-z])(sigaction|signal)\(' runtime/parts/*.c
runtime/platform/*.c`, comment lines dropped): 27 lines, in `parts/os.c` (260, 272, 320, 321, 440, 471,
472), `parts/panic.c` (54), `parts/sigdoor.c` (104), `parts/run.c` (781, 801, 807, 822, 1220, 1222,
1269, 1392), `parts/stack.c` (686, 1054, 1055), `parts/sigguard.c` (139, 183, 185) and
`platform/hero_net.c` (573, 584, 599, 650). By the critic's reading, `os.c:260`, `272`, `320` and `321`
are Windows arms (`hero_lease_abrt_win`, UCRT's `signal`), and `sigguard.c:139` and `panic.c:54` are
`signal(SIGABRT, SIG_DFL)` inside the refusal and panic paths, which a runtime definition of `signal`
would then serve from inside a handler. The runtime calls `pthread_sigmask` or `sigprocmask` on 16
lines more (`grep -h -E '(^|[^_a-z])(pthread_sigmask|sigprocmask)\(' runtime/parts/*.c runtime/platform/*.c
| grep -v -E '^\s*(\*|/\*)' | wc -l`).

**Other interposers in the same program** (the critic's item 7, re-run): **the address sanitizer,
which the `run` form uses at `-O0` and `-O2`** (`.claude/rules/c-boundary.md:358-363`). On this Mac,
`nm` of Xcode's `lib/clang/21/lib/darwin/libclang_rt.asan_osx_dynamic.dylib` lists `t _wrap_signal`, `t
_wrap_sigaction`, `s __ZL19substitution_signal` and `s __ZL22substitution_sigaction`, **all four local**,
and `otool -l` a `__DATA,__interpose` section of `0x1300` bytes: the sanitizer is itself a dyld
interposer, loaded as a dependent dylib, with no `DYLD_INSERT_LIBRARIES`. In the Linux image, `nm` of
`/usr/lib/llvm-22/lib/clang/22/lib/linux/libclang_rt.asan-aarch64.a` lists `W signal`, `W sigaction`, `W
__interceptor_signal`, `W __interceptor_sigaction` (weak), `T ___interceptor_signal`, `T
___interceptor_sigaction` (strong) and `U sigaction`. Neither takes `bsd_signal`, `sigset` or `sigvec`
by these names. What a runtime definition of the same names does beside them is unrun.

**The candidate routes, none built at the base**, each to be built or priced, or said why not, and
each saying what it closes on Darwin and on Linux arm64:

- **I, R4's corrected form**: the runtime defines every installer each platform's C library exports
  (Darwin's ten, glibc's eight linkable and `sigvec`, and `sigignore` and `siginterrupt` mirrored), with
  its whole meaning, keeps a per-thread depth around the program's handler, and the thunk asks it; the
  probes stay on the C library's own entry point. Its residue, measured: a Darwin dylib (two-level
  namespace, below), a raw system call, and the undeclared names if it defines only the declared.
- **I-dylib, route I shipped as a dylib with an `__interpose` section**, as the sanitizer is (above): a
  dyld interposer that reaches every image's imports without `DYLD_INSERT_LIBRARIES`. The runtime is
  linked into a program as an object today (`briefs/r2522/build/runtime-3068a62d1f01ed59-407f74943346dd0f.o`
  beside the program's archive), so this changes how every program links on Darwin. Unbuilt; the
  critic's.
- **I-onstack, route I with `SA_ONSTACK` forced**: the runtime's installer definitions add `SA_ONSTACK`
  to every handler they install and give every claimed thread an alternate stack; then R1's second
  question (`SS_ONSTACK`, `sigguard.c:330-334`) sees every handler that went through a definition, with
  or without frame records, and no per-thread depth is kept. I's coverage residue, and it changes what
  a C handler's stack is. Unbuilt; the critic's.
- **B, the asynchronous signals blocked on every claimed thread**, one runtime thread leaving them
  open, so a C handler runs where 2492's thread guard (`sigguard.c:321-325`) already refuses a Heroes
  function, with no walk. Panels 246 and 247 weighed a `sigwait` thread to count a signal
  (`246-reports/critic-reports.md:383`, `247-reports/ffi-pragmatist.md:294`), not as this guard. Its
  residue: a fault's signal reaches the faulting thread whatever its mask; a thread-directed `raise` or
  `pthread_kill` to a claimed thread stays pending (the 2494 case *"the runtime's door counts each
  signal the program sends itself"*); a C library that unblocks on a claimed thread. Unbuilt; the
  critic's.
- **T, a bounded scan of the stack for the delivery's trampoline** (the return address the exact test
  already learns), in place of a walk by frame records: it sees a handler with no frame records, and
  may read a stale word as a delivery and refuse a correct program. Unbuilt; the critic's.
- **W, the link wraps the installers** (`--wrap=<name>`). **Where it exists, by each linker's own help**
  (the critic's item 8, re-run): GNU `ld.bfd` (`--wrap SYMBOL  Use wrapper functions for SYMBOL`),
  `ld.lld` 22 (`--wrap=<symbol>  Redirect symbol references to __wrap_symbol and __real_symbol
  references to symbol`) and lld's MinGW driver (`ld.lld -m i386pep --help`, a `--wrap` line), in the
  Linux image; **and on none of** Apple's `ld` (`ld -help`: *"ld: unknown options: -help"*; `man ld | col
  -b`, 1,575 lines, 0 holding `wrap`; it has `-alias` at 608, `-alias_list` 613, `-flat_namespace` 618,
  `-interposable` 1027, `-force_flat_namespace` 1079, `-interposable_list` 1248), `ld64.lld` (0 lines
  holding `wrap` in `--help` and `--help-hidden`) or `lld-link` (0 in `-help` and `-help-hidden`; it has
  `/alternatename:<value>  Define weak alias`); MSVC's `link.exe` cannot be run here. **And it does not
  leave the runtime alone** (the critic's item 10, re-run in `briefs/fix/linux/run.sh`): with
  `-Wl,--wrap=sigaction`, *"runtime object's call wrapped: 1"* and *"static archive's call wrapped: 1"*
  under `ld.bfd` and `ld.lld` alike, and with the archive made a shared library *"static archive's call
  wrapped: 0"*. `--wrap` redirects every undefined reference of every object on the link line, and the
  runtime is one; so W needs the runtime to call the real installer as `__real_<name>` (which exists only
  on a wrapped link) or the link split, never reaches a shared library, and has no Darwin or MSVC form
  by these helps.
- **R, the references inside the program's static archives renamed at build time** (an `objcopy
  --redefine-sym` step): W's effect without W's link flag and without touching the runtime's object.
  The Linux image has `/usr/bin/llvm-objcopy` and `/usr/bin/objcopy` (`command -v`); on this Mac `xcrun
  --find llvm-objcopy` printed *"unable to find utility "llvm-objcopy""*. W's residue for shared
  libraries. Unbuilt; the critic's.
- **U, a fourth question by the platform's unwind tables**, not frame records (libunwind's
  `unw_is_signal_frame`, which Darwin's `libunwind.tbd` exports by the critic's search;
  `_Unwind_Backtrace`), so a library built without frame pointers but with unwind tables is climbed.
  Whether an unwinder is safe inside a signal (whether glibc's `dl_iterate_phdr`, which an unwinder may
  call, or Darwin's takes a lock is unread here) and what it costs every callback are its questions.
- **M, the thread's signal mask, a question rather than a route** (the critic's item 12). Darwin's
  `man 2 sigaction` (`briefs/fix/man-sigaction.txt`, its lines 71-75): *"When a signal is delivered to a
  process a new signal mask is installed for the duration of the process' signal handler (or until a
  sigprocmask(2) system call is made). This mask is formed by taking the union of the current signal
  mask set, the signal to be delivered, and the signal mask associated with the handler to be
  invoked."*; lines 115-117: *"SA_NODEFER If this bit is set, further occurrences of the delivered
  signal are not masked during the execution of the handler."* And glibc's `sysv_signal` installs with
  it, run tonight (`briefs/fix/linux/run.sh`): *"sysv_signal: SA_NODEFER 1 SA_RESETHAND 1 SA_RESTART 0"*,
  against `signal`'s *"SA_NODEFER 0 SA_RESETHAND 0 SA_RESTART 1"*. So M misses every `SA_NODEFER`
  handler and one that calls `sigprocmask`, and it needs the thread's own mask outside any handler,
  which the program's C changes without telling the runtime (the runtime's own 16 lines above); it is a
  route only with an interposer of `pthread_sigmask` and `sigprocmask` under it, I's problem again.
- **K, the residue kept as a known cost**: R3 forbids it in its own words; only the author's change to
  R3 admits it, and a seat that recommends it says so in those words. Not a route this sitting adopts.
- *A thunk section refused by the installers* (an installer definition refusing a handler whose address
  lies in the thunks' section) closes only the direct case, which R1's exact test already refuses, and
  not R3's, a C handler that *calls* the thunk: a restatement, the critic's item 13, listed so no seat
  re-derives it.

**Darwin's flat namespace, run** (the critic's item 11, re-run in `briefs/fix/darwin/`, the critic's
`ffmain.c` and `fflib.c`: an executable defining `sigaction` that counts and forwards by
`dlsym(RTLD_NEXT)`, a dylib calling `sigaction` and `signal`): plain, `-Wl,-flat_namespace` and
`-Wl,-force_flat_namespace` on the executable each printed *"dylib sigaction: handler ran 1, reached the
executable's 0"* and *"dylib signal: handler ran 1, reached the executable's 0"*, and the linker
printed *"ld: warning: -force_flat_namespace is no longer supported, using -flat_namespace instead"*.
So neither flag on the executable reaches a two-level dylib; what stays unrun is `__interpose` in the
executable and a dylib itself linked flat.

**The questions the sitting rules**:

1. **May the runtime define names of the C library at all**, under CLAUDE.md § 4 (architecture), and
   what panel 239's member then refuses: does the runtime's own definition of `sigaction`, or of a name
   it does not call today, make a program's library that also defines it a refused program, and should
   it (the escape's *widened diagnostic*)?
2. **How, on each platform**: Darwin's two-level namespace (`__interpose` in the executable, a dylib
   linked flat, I-dylib, and what System Integrity Protection or the hardened runtime refuses); glibc's
   resolution (the shared library's rows above, a library loaded by `dlopen`, a raw
   `syscall(SYS_rt_sigaction, ...)`); Windows (UCRT's `signal` called from a DLL against one linked into
   the executable, and the three Win32 installers above).
3. **What it breaks**: programs and libraries that call the real installer (`SIG_HOLD`, `SIG_IGN`,
   `SIG_DFL`, `sigaction(sig, NULL, &old)`, `sigignore`, `siginterrupt`, `siglongjmp` out of a handler);
   the sanitizer beside it; a library of the program defining `sigaction` (a crash reporter's shape);
   static linking (`grep -rn -E '"-static"|--static|static_link' selfhost/cli/*.hero` printed nothing, so
   a fully static executable is no mode of `heroes build` by that search; a static archive of the
   program, as R3's library is, is).
4. **The alternatives**: every route above and any a seat finds, each with what it closes on Darwin
   (a refusal with words before the first C call) and on Linux arm64 (no Heroes code inside the
   signal), and its residue, run where it can be.

## Question 2: defect 2502, the platform layer's waits

**The defect** (`gh issue view 2502`: OPEN, `kind:defect, class:improvement, area:runtime`,
M-core-packages): every waiting call of the layer waits in slices of 50 ms so a close or the stop ends
it. **Measured at the base, five runs** (lane b35-runtime's probe `waiters.c`, 100 threads each waiting
5 s in `hero_net_recv` on a quiet connection of its own, then each peer closed; or 100 threads in
`nanosleep` 5 s; built against the base's own layer, `clang -O2 -I <tree>/runtime waiters.c
<tree>/runtime/platform/hero_net.c -o waiters -lpthread`, `diff` of the lane's `hero_net_base.c`
against the base's `hero_net.c`: one comment, lines 1169-1171; each under `timeout -s KILL 60
/usr/bin/time -l`): **100 waiting 5 s, about 271.6 to 275.0 million instructions retired and 10,307 to
10,472 involuntary context switches**; 100 sleeping, 21.7 to 22.0 million and 222 to 236. The waiting
runs: the coordinator's four (273,025,580; 274,804,922; 274,702,290; 274,972,249, `briefs/r2502/runs.log`
and `fix.log`) and the critic's one (271,605,305 and 10,467, its section E); the sleeping runs the
coordinator's three and the critic's one (21,847,841 and 226). About twelve times the sleepers' count.
The issue's own figures (273,547,249 against 28,434,810) and the comment's (281 to 285 M against 22 M)
are *carried*; its *"about 1.8 cores at 10,000 idle waiters"* is the lane's arithmetic, an inference,
unrun at that size.

**The probe's trap, and the corrected probe** (the critic's item 17, re-run): `waiters.c:54-56` calls
`hero_net_accept` before it checks the `hero_net_connect` it just made, so past the descriptor limit a
refused connect leaves `accept` waiting for a connection that never comes. Under `ulimit -n 256`,
`waiters 126 wait 1` printed `126 waited`; `waiters 127 wait 1` printed nothing and was killed by
`timeout -s KILL 30`, exit 137 (09:45:08 to 09:45:38). **`waiters-checked.c`**, the critic's copy with
the connect checked first (its line 55), printed *"no connect 126, kind 9"* and exit 1 at once
(`HERO_NET_LIMIT`, `hero_net.h:65`). **Run the corrected probe**, or a probe of your own that checks every
call; a seat that reads *"a wait that never ends"* from the original past the limit reads the probe's
wait, not the layer's.

**And its counts are twice a server's** (the critic's item 18): the probe holds both ends of every
connection in one process (`peer[i]` and `mine[i]`, `waiters.c:42-56`), so the lane's *"201 IPv4"* for
100 connections and *"409 descriptors instead of 207"* (*carried*) count both ends. **A server-shaped
count is owed**: a server holding N accepted connections, its peers in another process, under each
route, counted with `lsof -p` at N = 100 and at the largest N that fits under `ulimit -n 256`. The
critic's arithmetic for a server, an inference until a seat runs it: about N + 6 descriptors at the
base (N sockets, the listener, three streams, the stop's two ends), about 3N + 6 for a pipe per socket
or per waiting thread (N up to 83 under 256), about 2N + 6 for one `eventfd` or `kqueue` per socket (N up
to 125), N + 8 for one shared wake; and `HERO_SPAWN_SLOTS`, 256 (`runtime/parts/spawn.c:105`), caps a
thread per connection anyway.

**What the base does** (`wc -l`: `runtime/platform/hero_net.c` 1235, `runtime/hero_net.h` 176):
`HERO_NET_SLICE_MS` 50 (`hero_net.c:172`); every socket non-blocking, a call tries first and waits in
slices (46-59, *"A wait has no other bound"*, 54); one slice (`hero_net_wait`, 714-732): `poll` on the
socket and the stop pipe's read end on POSIX (720-721), `WSAPoll` on the socket alone on Windows
(716-717), the stop asked after (728); the loops in `hero_net_accept` (961-985) and `hero_net_move`
(1045-1089) ask between slices whether the socket was closed; `hero_net_close` shuts a socket a call is
busy on and leaves the last call to close it (1131-1164, `hero_net_shut` at 1149). **The promise in the
header**: a waiter on a closed socket *"ends within about 50 ms ... since every call waits in slices of
that"* (`hero_net.h:95-101`), and a waiting call after a stop *"within about 50 ms"* (136-138). **The
stop's pipe**: made by `hero_net_stop_arm` (613-657), which `hero_net_catch_stop` calls (660-669),
written once by the handler and never read, so its read end stays readable (62-67); on Windows the
console handler counts and the waits learn of it *"within a slice"* (68-69, 513-545).

**The rulings**: panel 246's **R4** (`docs/panel/246-*.md:301-326`): the stop pipe, *"every blocking call
of the layer ... waits on it beside its socket"*, and *"its Windows arm (`WSAEventSelect` and
`WaitForMultipleObjects` on an event the console handler sets, compiled and unrun) are the landing's"*
(`:320-323`), where the landing took the slices instead (`hero_net.c:68-69`); defect 2499, the close that
ends a waiter, whose reasons are `hero_net.c:28-59` (a shutdown that woke nothing on Darwin and nothing
on Windows). CLAUDE.md § 2: *"Before `1.0.0`, no form enters to keep an older program compiling"*, so a
changed promise in `hero_net.h` is told, not kept beside the new one.

**The lane's prototype**, *carried* (its comment on 2502): a wake pipe per thread made at its first
wait, each socket keeping its waiters, a close writing one byte to each; waiting then flat in
instructions over 0, 5 and 10 s; a close ending a waiter in 99 to 131 µs; *"on Windows `WSAPoll` cannot
watch a pipe or an event"*. Its files, read-only: `.claude/worktrees/b35-scratch/b35-runtime/r2502/hero_net_proto.c`
(1295 lines), `closer.c`, `proto.log`, `fds.log`, `closer.log`.

**The descriptor budget, read tonight**: `launchctl limit maxfiles` printed `maxfiles 256 unlimited`,
launchd's own limits, the 256 the lane's comment calls *"Darwin's default soft limit"*; **this session's
shell reads `ulimit -n` 1048576** and `ulimit -Hn` unlimited, so a measurement in a shell like it does
not meet 256 (which processes start at 256 is a question): run one under `ulimit -n 256`;
`sysctl kern.maxfilesperproc` 92160; the Linux image 1048576 soft and hard. The layer's table holds
65,536 slots (`hero_net.c:166`).

**The candidate routes**: (a) a wake descriptor per waiting thread, the lane's; (b) one shared wake
descriptor with a generation counter, every waiter woken by any close and asking its own socket again;
(c) one per socket, made with the socket; (d) the status quo, or the slice lengthened or grown while a
wait stays quiet; (e) **one poller for the process**: a runtime thread polls every waiting socket on one
`kqueue` or `epoll` (and the stop's pipe) and wakes each waiter by a condition variable, so a waiter
holds no descriptor of its own, a close is a mark and a signal of the variable, and question 5's
deadline is a timer in that one poller; its cost a thread and a hand-off per wait (the critic's item 28,
unbuilt; whether Go's network poller is this shape is the historian's to read); (f) **a thread-directed
signal as the wake**: the closer `pthread_kill`s the waiting thread with a signal of the runtime's,
`poll` returns `EINTR`, the waiter asks its socket again, no descriptor at all; it meets defect 2494's
world (the runtime's probes already raise SIGURG, `sigguard.c:175-186`) and a program's C that takes the
same signal, and has no Windows arm (the critic's item 28, unbuilt). For each POSIX route with a
descriptor, `eventfd` on Linux, `kqueue`'s `EVFILT_USER` on Darwin, or a pipe. **Windows**:
`WSAEventSelect` with `WaitForMultipleObjects` (panel 246's recorded arm), or a loopback socket pair
`WSAPoll` can watch (the coordinator's addition, unbuilt), or the slices kept there alone. **The
questions**: what each route costs in descriptors (server-shaped), in instructions while waiting and in
a close's latency; what the slice-free wait does where no descriptor can be made (`HERO_NET_LIMIT`, or a
fall-back to slices, the lane's recommendation); what `hero_net.h`'s two *"within about 50 ms"*
sentences become; and how questions 4 and 5 lean on it (a stop the program asks must wake the waits; a
deadline needs a timed wait, and today the slice is the only clock a wait has).

## Question 3: defect 2490, a child's working folder

**The defect** (`gh issue view 2490`: OPEN, `kind:defect, class:improvement, area:runtime`,
M-core-packages): `hero_run_go` gives a child no working folder, so the net starts four cases'
commands through `/usr/bin/env -C`, which Windows has not, and skips them there. **The counts, re-run
with the issue's commands**: `find tests/golden -name directory` printed 3
(`unsupported/fixedbugs-2344-an-empty-cpath.case`, `-an-empty-element-of-c-include-path.case`,
`-a-trailing-separator-of-c-include-path.case`, each `directory` reading `here`), and `grep -l '^cd '
tests/golden/fetch/*/command` printed 1
(`fetch/fixedbugs-2479-a-command-run-from-inside-a-placed-tree-writes-nothing-there.case`, 9 of its
11 command lines opening `cd`, `grep -c '^cd '`; no link in it, `ls`: `app command expected idx`, the
critic's). `tests/harness/platform_skips.hero:151-153` names `fixedbugs-2344-an-empty-cpath` among the
cases `darwin_alone` skips on every other platform anyway, so **at most three could run on Windows
with the door, by reading**. The two other 2344 cases' variables are `C_INCLUDE_PATH=:` and
`C_INCLUDE_PATH=/nonexistent-2344:` (their `environment` files), whose `:` the lane's comment says
clang on Windows may read otherwise (*"unread and unrun"*). The Windows net runs every suite in its parts
(`ci.yml:1259-1270`, by the critic's reading), so the fetch case would be judged there.

**What the base does** (`wc -l`: `runtime/parts/run.c` 2104, `runtime/hero_os.h` 513): `hero_run_go`
(`run.c:1600`); on POSIX `fork` (1939), then in the child (`hero_run_child`, 1333): on Linux
`prctl(PR_SET_PDEATHSIG, SIGKILL)` and `getppid`/`raise` (1354-1355), `setpgid` (1372), the parent's
caught signals given back (1376, `sigaction` and `sigprocmask` in `hero_run_child_signals`, 814-825),
the write ceiling's `getrlimit`, `sigaction` and `setrlimit` (1382-1400), **the three streams opened by
their paths as given** (1416-1446, each failure written down the report pipe and `_exit(126)`, 1422, 1433,
1444), then `execvp`, `execvP` or `execvpe` (1460-1467), `_exit(127)` (1477). On Windows
`CreateProcessW(NULL, wide_line, NULL, NULL, TRUE, ..., env_block, NULL, &startup, &child)`
(1794-1797), the NULL before `&startup` being `lpCurrentDirectory`; `env_block` is
`hero_run_win_environment`'s (`run.c:1636`), NULL unless a variable is staged (`:378`), built from
`GetEnvironmentStringsW` (`:390`). **The door it would sit beside**: `hero_run_env` (`run.c:155`;
`hero_os.h:363-377`), panel 242's **R6** (`docs/panel/242-*.md:176-188`, ratified 2026-11-10 at
`:276-277`): staged for the next child alone, *"so no process-wide write exists"*, the program found
*"through the parent's `PATH` on every system"*. Decision 2228 (`gh issue view 2228`, CLOSED): `hero_os.h`
*"is the language's door"*, answered by the author 2026-10-27; spec § 13 names *"processes ... from the
language's `hero_os.h`"* (`spec/heroes-spec.md:377`). **The net**: `tests/harness/case_setting.hero:151-161`
(`skip_here`), 258 (the `env -C` line); `suite_fetch.hero:141-152` (its skip), 217 (`ENV`), 342-343.

**POSIX's list, read tonight** (the critic's item 19, re-fetched: `curl -s -L -o
briefs/fix/web/v2chap02.html https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html`,
330,207 bytes, the page naming *"IEEE Std 1003.1-2024"*): its line 3017, *"The following table defines a
set of functions and function-like macros that shall be async-signal-safe. Therefore, applications can
call them, without restriction, from signal-catching functions."*; the table's names, extracted by `grep
-o -E '<i>[A-Za-z_0-9]+</i>\(\)'` from that line to the second `</table>` after it: 231
(`briefs/fix/web/async-safe.txt`). **On it** (`grep -c -x`): `chdir`, `fchdir`, `setpgid`, `sigaction`,
`sigprocmask`, `pthread_sigmask`, `open`, `dup2`, `close`, `write`, `_exit`, `getppid`, `raise`, `execl`,
`execle`, `execv`, `execve`, `fexecve`. **Not on it**: `getrlimit`, `setrlimit`, `execvp`,
`posix_spawn`; and `execvpe` (glibc), `execvP` (Darwin) and `prctl` (Linux) are outside the standard.
**The fork page** (`.../functions/fork.html`, 24,028 bytes, the same hour): *"If a multi-threaded process
calls fork(), the new process shall contain a replica of the calling thread and its entire address
space, possibly including the states of mutexes and other resources. Consequently, the application
shall ensure that the child process only executes async-signal-safe operations until such time as one
of the exec functions is successful."* **So the child already makes calls POSIX does not promise safe
after a threaded `fork`**: the write ceiling's `getrlimit` and `setrlimit` (`run.c:1382-1400`) and
`execvp`'s path search (1460); `chdir` would be one of the safest calls it makes. Whether the existing
ones stand is a question about `hero_run_child` as it is, which the sitting asks beside the door's.

**`CreateProcessW`, read tonight** (the critic's item 20, re-fetched: `curl -s -L -o
briefs/fix/web/createprocessw.html
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw`,
76,648 bytes, its `updated_at` 2025-07-01T18:41:00Z): of `lpCurrentDirectory`, *"The full path to the
current directory for the process. The string can also specify a UNC path. If this parameter is NULL,
the new process will have the same current drive and directory as the calling process."* **The page
says nothing of a relative folder or a missing one**: split into 152 sentences, none matched
`relative|ERROR_DIRECTORY|does not exist|PATH_NOT_FOUND` (`python3 -I`, `briefs/fix/web/cpw.txt`); both
are owed to a run on Windows. **Two more of its sentences bear on the door**: *"If the file name does not
contain a directory path, the system searches for the executable file in the following sequence: The
directory from which the application loaded. The current directory for the parent process."*, so with
`lpApplicationName` NULL, as `run.c:1794` calls it, a program named without a folder is looked for in
the **parent's** folder whatever `lpCurrentDirectory` says, where POSIX's `execvp` after a `chdir`
resolves a relative path in the child's; and *"If an application provides an environment block, the
current directory information of the system drives is not automatically propagated to the new process.
For example, there is an environment variable named =C: whose value is the current directory on drive
C."*, followed by *"An application must manually pass the current directory information to the new
process"*: whether `hero_run_win_environment`'s block keeps the `=X:` entries, so a drive-relative path
in the child resolves, is a question nobody had listed.

**The questions**: the door's name and staging (given back by `hero_run_reset` as `hero_run_env`'s
variables are); **POSIX**: `chdir` between `fork` and `exec` in a parent that may hold other threads,
on the list above, and the child's existing `getrlimit`, `setrlimit` and `execvp`, off it: do they stand,
and what replaces them if not; **the order** the door promises against the three streams the child
opens by path and the program's own path when relative, on POSIX and on Windows, where the page above
gives the parent's folder for the program; **the shapes**: `chdir` in the child; **`fchdir` on a folder
the parent opened** (`open(dir, O_DIRECTORY | O_CLOEXEC)` at staging or before `fork`), so a missing or
unreadable folder is the parent's error told before any child exists and a relative folder is resolved
once, in the parent's folder (the critic's item 28, unbuilt); **POSIX 2024's
`posix_spawn_file_actions_addchdir`**, read (the critic's second pass, item 12, re-run: `curl -s -L -w
'%{http_code}' https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn_file_actions_addchdir.html`
printed 200, the page holding *"Issue 8"*), which **Darwin's SDK declares without `_np`**
(`$(xcrun --show-sdk-path)/usr/include/spawn.h:72`, `addfchdir` at 75, the `_np` forms marked
`__API_DEPRECATED("posix_spawn_file_actions_addchdir(3) has replaced
posix_spawn_file_actions_addchdir_np(3)", macos(10.15, 26.0))` at 184-188) and **glibc 2.41 only with
it** (the Linux image's `/usr/include/spawn.h:206` `addchdir_np`, `214` `addfchdir_np`), against
`hero_run_go`'s `fork` for its `setpgid`, terminal and write ceiling; and, for the four cases alone, **a `heroes` flag that starts the compiler
in a folder** (git's `-C`), which would serve the net and no other program and is governed by
`.claude/rules/cli-surface.md` (the critic's item 28); **Windows**: a relative or missing
`lpCurrentDirectory` and the `=X:` entries, compiled and owed; **a missing folder** told how, and by
which number `hero_run_why` answers; and which of the four cases the door would let run on Windows,
each read, the run owed to the CI's leg.

## Question 4: a stop the program asks itself

**No issue exists**: `gh issue list --repo heroes-lang/heroes --state all --search "<terms>"` with
*ask stop*, *stop door*, *hero_net_ask_stop*, *program asks its own stop*, *stop asked by the program*,
*request to quit*, *shutdown server* named no such door (the nearest, 1069, *"a service that stops
cleanly is not writable"*, CLOSED, is the feature panel 246's R4 answered; 2265 and 2082 are other
things).

**The doors there are** (`grep -rn -E 'hero_net_[a-z_]+' runtime/hero_net.h | grep -oE 'hero_net_[a-z_]+\('
| sort -u`): 13, `accept`, `catch_stop`, `close`, `connect`, `last_error`, `last_os_error`, `listen`,
`number`, `of`, `port`, `recv`, `send`, `stop_asked`, each `hero_net_`. **The stop**: `hero_net_catch_stop`
(`hero_net.h:130-162`) and the reader `hero_net_stop_asked` (164-168, *"A reader ...: it changes nothing"*).
`grep -n -i -E 'ask|request' runtime/hero_net.h` printed lines 68, 73, 130, 136 and 164-168, every one
about a signal's stop or a port; no door asks a stop from the program. **What the base does**: one
counter, `hero_net_stop_asks` (`hero_net.c:503-506`, *"0 none, 1 the program stops, more ends it"*);
the POSIX handler counts and writes one byte at the first signal, and **any other, a second one, puts
the signal's default back and sends it again** (556-577); Windows' console handler answers `FALSE` at
the second, ending the process (520-533); before `hero_net_catch_stop` the pipe's ends are -1
(549-550) and a wait's `poll` passes over it (719-720), every wait asking the counter between slices
(728, 965, 1077).

**`<core>/net`** (`packages/core/0.3.0/files/net.hero`, 288 lines): `catch_stop` (203) and no reader of
the stop; its binding `net/hero_net.hero` (173 lines) binds `hero_net_catch_stop` (44) and **not**
`hero_net_stop_asked` (`grep -rn 'stop_asked' packages/ examples/ tests/golden --include='*.hero'`
printed two `run` cases of the layer and nothing under `packages/` or `examples/`). A published version
is never edited (`examples/packages/README.md:52-53`, *"a change to a package is a new version"*), so
any face in `net` is a new version of `core`.

**The program it is for**: `examples/server/main.hero` (206 lines) ends its loopback shape by a count,
`most: 1` (63), and its server by the stop alone (`listening`, 94-96; the accept's `STOPPED` ending the
loop, 128-131); its handler, `answer` (162-165), runs inside `http.serve_conn` on a connection's own
thread (151-158), so a door it could call to end the server must be callable from any thread.

**The rulings**: panel 246's R4 and its veto (`docs/panel/246-*.md:301-326`, `:351-355`): *"no stop
sentence in the specification and no stop door in `hero_os.h` or among the built-ins; the stop the
layer owns is documented in `hero_net.h` and the `net` package's card at 0 spec tokens"*; panel 247's
R5 (`docs/panel/247-*.md:213-246`) discharged the first half by its return condition and put hg3 in
spec § 13 (`spec/heroes-spec.md:404`, *"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64`
answer 1"*), which is why this question is the full panel's (§ The lane); panel 247's R7 (`:255-271`):
G, `hero_signal_watch`, answers -1 for SIGINT and SIGTERM, *"it is not a stop door"*.

**The questions**: a door such as `hero_net_ask_stop()`, what it does before `hero_net_catch_stop` and
after; whether a Ctrl-C after the program asked its stop ends the process at once, as a second signal
does today, or counts as the first; its thread-safety (no signal, any thread, the counter and the pipe's
write); what wakes a wait on Windows, where only the slices see the counter; whether `<core>/net` gains
it and the reader `stop_asked`, in which version; whether line 404 owes a sentence (the spec-warden
prices the candidates); and what a reader of the spec and the card writes with each face (the blind
seat's task t4, `llm-ergonomist.md`).

## Question 5: a connection that sends nothing

**No issue exists**: the same search with *deadline*, *timeout net*, *SetReadDeadline*, *read deadline*,
*idle connection*, *connection sends nothing*, *slowloris*, *hero_net_recv timeout* named none (2502
answered *idle connection*). **No sitting ruled one**: `grep -l -i -E 'SetReadDeadline|SetDeadline|read
deadline' docs/panel/*.md docs/panel/*/*.md` printed, besides these briefs, nothing; panel 246's critic
listed *"an `accept` with a deadline"* as a route nobody built (`docs/panel/246-reports/critic-reports.md:371-375`,
`:512-514`), which 246 recorded under its § Conservative alternatives (`docs/panel/246-*.md:424-426`).

**What the grep found**: `grep -n -i -E 'timeout|deadline|timed_out|time out'` over `runtime/hero_net.h`,
`packages/core/0.3.0/files/net.md`, `net/http.md`, `net.hero`, `net/hero_net.hero`, `net/hero_net.md`,
`net/http.hero`, `net/http/internal/*.hero` and `net/internal/failure.hero` printed the kind
`HERO_NET_TIMED_OUT` (`hero_net.h:62`), the word *"`timeout`'s"* in the stop's comment (131), the code
`timed_out` (`net.md:38`, `net.hero:41-42`, `net/hero_net.hero:29`, `net/hero_net.md:8`,
`net/internal/failure.hero:52-53`), and no door taking a time. The layer sets that kind only from the
system's own `WSAETIMEDOUT` and `ETIMEDOUT` (`hero_net.c:264-265`, `291-292`), and *"A wait has no
other bound"* (54).

**What a quiet connection does to the server**, read: `served` (`examples/server/main.hero:110-136`)
joins `threads[0]` before the next accept whenever `threads.len() >= room` (115-117), `room` being
`hero_thread_limit()` (86), which answers `HERO_SPAWN_SLOTS`, 256 (`runtime/parts/spawn.c:105`, `634`);
a connection's thread waits in `http`'s head read (`net/http/internal/lines.hero:58`, `net.read`) until
the peer sends, closes, or the stop comes. **And a thread leaves `threads` only at that join** (the
critic's item 21): a finished thread keeps its place until it is the oldest, so by reading **one quiet
connection accepted first, followed by 255 that finish, stops the accept as surely** as 256 quiet ones:
a reading, unrun, which the seats run beside the wider shape, bounded (the table in § Where each seat
works: the server under `timeout -s KILL 60`, each client under `curl --max-time 5`). `hero_os.h` offers
no join that does not wait (`grep -n -E '^[a-z_0-9 *]+hero_thread_[a-z_]+\(' runtime/hero_os.h`:
`spawn` 441, `spawn_sized` 447, `join` 449, `limit` 453), so the example cannot join whichever thread
finished. And a connection crosses to its thread as a number (`hero_net.h:103-121`), after which no
handle of `main`'s names it, so `main` cannot close a quiet one to end its wait: read, unrun.

**The candidate routes**: a deadline per call (a `recv` or `accept` that takes a limit); per socket, as
Go's `SetReadDeadline` and `SetWriteDeadline` (an absolute time the socket's waits end at, cleared by
setting none); a limit in `net/http`'s `serve_conn` alone (Go's `ReadHeaderTimeout` and
`IdleTimeout`, the historian's to read); **a join that does not wait**, so `served` joins whichever
thread finished, in two shapes the spec-warden prices (the critic's second pass, item 4): `hero_thread_join`
itself answering "not yet" for a thread still running (`spec-warden.md`'s q5n), which makes spec § 13's
line 377 (*"`hero_thread_join(handle: i64) -> i64` waits for its answer"*) false, or a door beside a join
that still waits, saying whether the answer is there (its q5j), which leaves it true; either is the full
panel's (the critic's item 21; § The lane); and the answer's kind, `HERO_NET_TIMED_OUT` reused or a new one. The
sockets are non-blocking (`hero_net.c:46-48`) and every wait is the layer's own `poll` or `WSAPoll`, so
the system's `SO_RCVTIMEO` should reach no wait of the layer's and a deadline would be that wait's own
bound, on every platform: an inference from reading, which the ffi-pragmatist runs. How it is clocked
(a clock that never goes back) is its question. **The face** in `net` and `net/http` is what a program
writer reads on the package's card: the spec-warden says whether any of it owes the spec a sentence,
and the blind seat's task t5 measures what a reader writes with a deadline on the card and without.

## What the coordinator measured (each with its command, above)

The base's two stages and their `sources` line, on this Mac and in Linux arm64; R3's two cases at the
base (`flib-nofp` on Darwin; `fliblocale` on Darwin and Linux arm64, `briefs/r2522/`); the C twin
(`briefs/fix/ctwin/`); 2502's probe, four waiting runs and three sleeping, and the trap under `ulimit -n
256` (`briefs/r2502/`); Darwin's exports and the four undeclared installers run, the flat namespace run
(`briefs/fix/darwin/`); glibc's prototypes, versions, reach, `sysv_signal`'s flags and `--wrap`'s reach
(`briefs/fix/linux/fix-linux.log`); the linkers' helps; the sanitizers' symbols; the zig targets
(`briefs/fix/zig/`); POSIX's list and the fork and `CreateProcessW` pages (`briefs/fix/web/`); the runtime's
27 installs, its 16 mask calls and its Windows installers; the four cases of 2490; the descriptor
limits; the Docker bound (`p248-coord-bound`); the platforms' state; the issue searches
(`briefs/searches.txt`, 22 terms) and the b35 search (`briefs/fix/b35-grep.txt`); the spec's count at the
base (`spec-warden.md`). **Line counts** (`wc -l`): `runtime/parts/sigguard.c` 364, `sigdoor.c` 137,
`thread.c` 144, `alloc.c` 1540, `run.c` 2104, `os.c` 1019, `stack.c` 1213, `spawn.c` 634;
`runtime/platform/hero_net.c` 1235; `runtime/hero_net.h` 176, `hero_os.h` 513, `heroes_runtime.h` 995;
`selfhost/emit/callback_thunk.hero` 343, `callback_guard.hero` 207; `selfhost/cli/supplied.hero` 357,
`link_flags.hero` 215; `packages/core/0.3.0/files/net.hero` 288, `net/hero_net.hero` 173, `net/http.hero`
150, `net/http/internal/server.hero` 217, `net/http/internal/lines.hero` 173; `examples/server/main.hero`
206; `tests/harness/case_setting.hero` 498, `suite_fetch.hero` 503, `platform_skips.hero` 335;
`spec/heroes-spec.md` 492 lines and 29,187 bytes (`wc -l -c`).

## Questions no one has run, posed as questions

- Whether dyld honours an `__interpose` section in a program's own executable, and whether a dylib
  linked flat reaches the runtime's `sigaction`, on this Mac.
- Whether a runtime definition of `signal` and `sigaction` and the sanitizer's own (dyld substitutions
  on Darwin, weak symbols on Linux) can stand in one program, at `--sanitize`'s `-O0` and `-O2`.
- What `sigignore` and `siginterrupt` do to a definition's table on Darwin.
- Whether an unwinder can find the delivery's frame through `sigcb_handler` built
  `-fomit-frame-pointer`, and at what cost a callback.
- What a slice-free wait costs on Windows, and whether `WSAPoll` can watch a loopback socket pair as a
  wake descriptor.
- What a server holding N connections spends in descriptors under each wake route, N = 100 and the
  largest N under `ulimit -n 256`.
- Whether the child's `getrlimit`, `setrlimit` and `execvp` after a threaded `fork` stand, and what
  `CreateProcessW` does with a relative or missing `lpCurrentDirectory` and a block without `=X:`.
- Whether the two `fixedbugs-2344` folder cases would pass on Windows with the door.
- Whether the net's own tests pass in a `git archive` copy with no `.git`.
- Whether a server built like `examples/server/main.hero` stops accepting with one quiet connection
  first and 255 finished after it, and with 256 quiet ones.
- `flib-nofp` in Linux arm64 at the base.
