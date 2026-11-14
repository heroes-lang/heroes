# Panel 248, the brief every seat reads first

Convened 2026-11-14 by the coordinator, in M-core-packages (`gh issue view 2522 --repo
heroes-lang/heroes --json milestone -q .milestone.title`: `M-core-packages`). **The soundness lane**:
the compiler-engineer and the ffi-pragmatist, a completeness critic on these briefs before any seat
starts and on the reports after. **Five questions, each a mechanism of the runtime or of its platform
layer**: the C library's handler installers defined by the runtime (defect 2522), the platform
layer's waits (defect 2502), a child's working folder (defect 2490), a stop the program asks itself,
and a deadline on a connection. **No paid run is named in any brief of this sitting**, so no seat and
no critic starts one.

**Where this brief is wrong, say so first.** Every number, path and count below carries the command
that produced it, run between about 08:45 and 09:15 on 2026-11-14; a figure copied from an issue, a report or
an earlier sitting says *carried* beside it, and is yours to re-run before you lean on it.

## The mandate, and why this lane

- **CLAUDE.md § 4** (its lines 200-201): *"Ask once per milestone, then convene without asking
  again"*. The sittings since 234 (`ls docs/panel | grep -E '^2(3[4-9]|4[0-7])-[a-z]' | sed 's/-.*//'
  | sort -u`): 234 to 247, every number. Of those, the files that name M-core-packages anywhere
  (`grep -l 'M-core-packages' docs/panel/2*.md`): 234, 235, 240, 241, 242, 243, 246, 247; 236 to
  239 name M-package-manager in their first twelve lines (`head -12 | grep -o -E 'M-[a-z]+(-[a-z]+)+'`
  over each). Panel 245's status says *"the gate asked once for the milestone, at panel 242"*
  (`docs/panel/245-*.md:6`). So this sitting is convened without asking.
- **The lane**, `.claude/skills/panel/SKILL.md:18-23`: *"a change with no surface, no diagnostic and no
  spec token: a runtime primitive, an emitter mechanism, an internal representation"*. Each question
  below is a door of `hero_net.h` or `hero_os.h` or a mechanism behind one, and none is expected to
  change what the checker refuses or what the spec says.
- **The escape, and it binds every seat.** Spec § 13 already names doors of both headers
  (`spec/heroes-spec.md:377`: *"threads, processes and files from the language's `hero_os.h`, and
  sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`"*; line 404, panel 247's
  R5: *"after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C and SIGTERM only make
  `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped"*). **A seat
  whose route needs a spec sentence** (a door a reader must be told of, or a sentence of line 404 that
  a new door makes false), **a new diagnostic, or a change a reader of the `net` package's card must
  learn, says so in its report in those words**, and the coordinator escalates that question to the
  full panel (`SKILL.md:29`, *"When in doubt take the full panel"*). Question 5 asks this outright.

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
4. and 5. The coordinator's. **They stand in no file of the tree and no issue**: the coordinator
   wrote them on 2026-11-13, while batch 35's HTTP server landed, into its own working notes, which
   live outside the repository (one line: *"hero_net_ask_stop (a program asks its own stop; loopback
   server ends by count today), idle-client defence"*), and these briefs' search found no lane report
   that holds them: `grep -rl -i -E 'stop_asked|ask.{0,20}stop|deadline|nothing.{0,30}(send|sent)|quiet
   connection|holds its thread' .claude/worktrees/b35-scratch --include='*.md' --include='*.txt' | grep -v
   /site-repro/` printed a commit message's draft (`msg-spec-247.txt`), the signals lane's case runs,
   close texts and build caches, none of them such a note. So every fact behind them is this brief's
   own, run tonight (each question's section), and no issue exists for either (the searches are there
   too); the sitting's decision is where they first become a record, and the synthesis files what it
   leaves open as issues.

## The base

**`df7612ad3dc62dcc6d90bccc650adf6b9736078f`**, its tree **`f72f48da250e919a68ebc6a98be0facccc29a052`**
(`git rev-parse 'df7612ad3^{tree}'`), `git log -1`: *"Batch 35 round closed: the compiler counted again,
every gate read green"*, 2026-11-14 00:53:52 +0100, the head of branch `p248` in
`.claude/worktrees/p248`, whose `git status --short` printed nothing when these briefs were begun. It
holds batch 35's landing of panel 247 (`runtime/parts/sigguard.c`, `runtime/parts/sigdoor.c`, spec
§ 13's lines 403-404). Every path below is from the repository's root,
`/Users/joseph/Temp/heroes/heroes-lang`. Every seat copies the commit and builds its own compiler in
two stages:

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
(`/usr/bin/time -l`, *instructions retired*), never seconds, or say why a clock was still.

## Where each seat works, and the rules every seat obeys

Every seat's folder is **`.claude/worktrees/p248-scratch/248-<seat>/`**, ignored by git (`git
check-ignore -v .claude/worktrees/p248-scratch/briefs`: `.gitignore:34:.claude/worktrees/`), never the
session scratchpad, never `/tmp`: CLAUDE.md § Hard stops writes no file outside the repository's root.
Read-only to the seats, copied into your own folder before use:

- `.claude/worktrees/p248-scratch/briefs/tools/storm.py` and `ips.py`, copied from
  `.claude/worktrees/p247-scratch/tools/` (`shasum -a 256`: `8e6b762a...` and `92071c5e...`): panel
  247's storm driver (`python3 -I storm.py <binary> <SIGNAME> <seconds> <arm-delay>`, the binary in a
  session of its own, its output kept to 65,536 bytes a stream, SIGKILL to its group at the end) and
  its crash-report tally (`python3 -I ips.py ~/Library/Logs/DiagnosticReports <binary-name>...`,
  `SHOW=1` for one stack per kind).
- `.claude/worktrees/p248-scratch/briefs/ev247/`, panel 247's evidence unpacked
  (`tar -xJf docs/panel/247-reports/evidence.tar.xz`), among it
  `247-ffi-pragmatist/h/fliblocale.hero` (37 lines), `h/sigcb.h` (4) and `h/lib/sigcb.c` (21)
  (`wc -l`), defect 2522's program and library. Never unpack an archive inside the worktree.
- `.claude/worktrees/p248-scratch/briefs/r2502/waiters.c` (67 lines), lane b35-runtime's probe of
  defect 2502, and `.claude/worktrees/p248-scratch/briefs/r2522/` (the coordinator's reproduction of
  2522, below). The lanes' own folders under `.claude/worktrees/b35-scratch/` are read-only too.

The rules:

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool. Never the trunk's `./heroes`, a compiler of
  `.claude/worktrees/p248`, of `p248-scratch/briefs/`, of a lane or of the other seat; never read, build
  or run inside the other seat's folder or a lane's worktree.
- **Your report**: `docs/panel/248-reports/<seat>.md` in `.claude/worktrees/p248`, written as you go,
  not only at the end, in your definition's output structure (`.claude/agents/<seat>.md`). Write
  nothing else in `.claude/worktrees/p248`.
- **Every seat builds the route it recommends, in its own copy, and runs the cases it names, before
  the synthesis** (`SKILL.md` 3c): a route that does not build is reported as not built, never
  adopted.
- **Every number with the command that produced it; every negative sentence run, or posed as a
  question**; the words *only*, *never*, *provably* with what was searched and where (`/panel` step
  2; CL-077).
- **No paid run**: no `claude -p`, no API call, no `heroes measure --refresh`, no cloud run. One you
  find worth running goes in your report with its size and cost, for the coordinator.
- **Never print a credential**: `gh auth status` at most, never `gh auth token`. No GitHub write, no
  issue, no commit, no push, no tag. `gh issue view` and `gh issue list` are reads and allowed.
- **A program that may not end** (a server, a storm's target, a waiter) is built with `heroes build`
  and its BINARY run under `timeout -s KILL <seconds>`, never `timeout heroes run` and never a plain
  `timeout`, whose SIGTERM a program that caught the stop outlives; its output to `/dev/null` or through
  `head -c 65536` into a file; after it, `pgrep -f <your folder>` is read and anything left is stopped
  before a verdict (`.claude/rules/verification.md` § A run that may not end is bounded in time and in
  bytes). Send signals only to processes you started. A storm runs under `storm.py` unwrapped (it
  bounds itself and leaves nothing behind).
- **Every Darwin death reads its crash report**: a SIGKILL from Darwin's libplatform writes nothing to
  stderr (2522's reproduction below), so a run is judged by its `.ips` reports, which are a sample,
  not a count.
- **One heavy command at a time** (a stage 2 build, a suite): other sessions build on this Mac.
  Whole suites only where your route changes what the checker refuses, named in your report with why;
  otherwise the `run` form narrowed to your cases (`./heroes-next run tests/harness/main.hero --
  ./heroes-next run <case-substring>`), one at a time.

## The platforms

- **Darwin arm64**: this Mac, Apple clang 21.0.0 (`heroes-next doctor`: *Apple clang version 21.0.0
  (clang-2100.4.1.1)*). `pmset -g assertions` read `PreventUserIdleSystemSleep 1` at 08:50, so
  something already holds the machine awake (`.claude/rules/verification.md` § A long run holds the
  machine awake): ask it again before a long run, `caffeinate -i` only where it reads 0.
- **Linux arm64**: the image `heroes-linux-arm64:latest`, `338a1044ac90` (`docker images
  heroes-linux-arm64`); inside it `uname -m` *aarch64*, `ldd --version` *Debian GLIBC 2.41-12+deb13u3*,
  *Debian clang version 22.1.8*, `ulimit -n` and `ulimit -Hn` both 1048576 (one `docker run --rm` of
  those five commands). The way the coordinator ran it tonight, which a seat may run itself, one
  container at a time: write a script into a folder of yours, then from the repository's root

  ```
  git -C .claude/worktrees/p248 archive df7612ad3dc62dcc6d90bccc650adf6b9736078f | timeout -s KILL 900 docker run -i --rm -v <your folder>/linux:/out heroes-linux-arm64:latest bash /out/run.sh
  ```

  the script reading the tree from its stdin (`tar -xf - <&0` into a folder of the container's own),
  building the seed and stage 2 there, writing its log under `/out` and bounding every output with
  `head -c`. Measured with `.claude/worktrees/p248-scratch/briefs/r2522/linux/run.sh` at about 09:00:
  `docker exit 0`, stage 2 `sources 4c9a6a9800095eef`.
- **Windows: the box is OFF tonight.** `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at
  08:51 printed *"ssh: Could not resolve hostname apponfly-vps: nodename nor servname provided, or not
  known"*, exit 255, as the coordinator's own did at 08:47. `.claude/rules/platforms.md` § The Windows
  box: *"Paid by the hour, usually off, and only the author can start it"*. **A seat that needs a
  Windows measurement names it in its report as owed**; it compiles its Windows arm with `zig cc
  -target x86_64-windows-gnu` (`zig version`: 0.15.2, at `/Users/joseph/.zvm/bin/zig`), with
  `ZIG_GLOBAL_CACHE_DIR` and `ZIG_LOCAL_CACHE_DIR` inside its folder, and never runs it; **the CI's
  Windows leg judges after the push** (`.claude/rules/verification.md` § The optimistic chain, its
  item 4). *How the box is driven when it is on*, the coordinator's practice, not run tonight: files
  under `/c/w/` in Git Bash; a long run started detached, a `.cmd` running `bash -lc "..."` into a log,
  started by PowerShell's `Invoke-CimMethod -ClassName Win32_Process -MethodName Create`, since an ssh
  session that drops kills its children; a file over a few kilobytes sent as base64 in chunks under
  6,000 characters, since cmd.exe truncates long lines.

## Question 1: defect 2522, and whether the runtime may define the C library's installers

**The defect** (`gh issue view 2522 --repo heroes-lang/heroes`: OPEN, `kind:defect, class:blocking,
area:runtime`, M-core-packages, filed 2026-11-13, no comments), panel 247's R3 residue: a library of
the program, built `-O2 -fomit-frame-pointer`, installs its own handler with `sigaction` and calls the
program's callback from it; the callback, a Heroes function making one C call (`localeconv`) and
building no value, passes all three of R1's questions and D's guard. The issue's measurements, by the
lane, *carried*: Darwin `exit -9` in 20 of 20 storms, every report SIGKILL in the locale lock; Linux
arm64 exit 0 in 5 of 5, the function running inside the signal through 1.35 to 2.17 million signals.

**Reproduced at the base by the coordinator**, the evidence's files copied into
`.claude/worktrees/p248-scratch/briefs/r2522/`, the library built as panel 247's Linux script built it
(`ev247/247-ffi-pragmatist/linux-247.sh:108`: `clang -std=gnu11 -c -O2 -fomit-frame-pointer ... && ar
rcs lib-nofp/libsigcb.a`, **a static archive**), the program by `../base/tree/heroes-next build
fliblocale.hero --library lib-nofp -o fliblocale` (exit 0):

- **Darwin arm64**, `python3 -I ../tools/storm.py ./fliblocale SIGUSR1 4 0.3` three times at
  08:59:40: **3 of 3 `exit -9`**, after 17,070 to 17,777 signals, stdout `armed` and nothing else,
  stderr empty; `ips.py ... fliblocale`: **3 reports, all SIGKILL, the lock**, `SHOW=1`'s stack
  `_os_unfair_lock_recursive_abort < _os_unfair_lock_lock_slow < localeconv_l < h_fliblocale_answer <
  h_0cb_fliblocale_answer < sigcb_handler < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk <
  hero_int_to_str < h_fliblocale_round < h_fliblocale_main`. `pgrep` found nothing left.
- **Linux arm64** (`r2522/linux/run.sh`, the line above): **3 of 3 exit 0**, `built 20000000 words`,
  after 2,102,440 to 2,240,205 signals, no refusal.

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
  trigger)."*
- Panel 247's **R1** and **R2** (`:153-189`), the questions and D as they landed; **R10**
  (`:286-291`), Windows.
- Panel 246's **R4** (`docs/panel/246-*.md:318`): *"No Heroes code runs in a handler"*, ratified
  2026-11-12.
- Panel 239's member, `ffi_defined_elsewhere` (`.claude/rules/c-boundary.md:186-220`; the code,
  `selfhost/cli/supplied.hero`, 357 lines): *"a C name a group binds, or the runtime calls or defines,
  that a library the program names other than the one meant would supply"*, told on the binding.

**What panel 247 measured of interposition**, *carried* (its reports, read today, each yours to re-run):
the ffi-pragmatist's `F/c/ipose.c` (in the evidence as `ev247/247-ffi-pragmatist/c/ipose.c` and
`ipose_lib.c`), the executable defining `sigaction`, `signal` and `sigset`
(`docs/panel/247-reports/ffi-pragmatist.md:173-191`): on Darwin a static library without frame pointers
*"seen, 3 of 3"*, a shared library *"missed, 3 of 3: Darwin's two-level namespace binds a dylib's
`sigaction` to libSystem's"*; in Linux arm64 *"every row seen inside a handler, the shared library's
three included, with or without `-rdynamic`"* (`:280-282`); what it left (`:376-383`): installers it did
not define, *"a raw `rt_sigaction` system call"*, and a handler left by `siglongjmp`. The critic's re-run
(`docs/panel/247-reports/critic-reports.md:84-110`): libc's own `signal`, `sigset`, `bsd_signal`,
`sigvec` and `__sigaction` *"never reach the executable's own `sigaction`"* on Darwin, and none of
glibc's does; the probe through the runtime's own `sigaction` blinded the exact test (`direct.hero`,
`dlocale.hero`: 3 of 3 SIGKILL in silence); and `sigset(SIGUSR1, SIG_HOLD)` *"exit 134, a false
message"*.

**The installers each C library exports, read tonight:**

- **Darwin**, the SDK's stubs (`tr -s "[],' \n" '\n' < $(xcrun --show-sdk-path)/usr/lib/system/<lib>.tbd
  | grep -x -E '_{1,3}(sigaction|signal|sigset|bsd_signal|sigvec|ssignal|sysv_signal|sigignore|sighold|sigrelse|sigpause|siginterrupt|sigaction_self)'`):
  `libsystem_kernel` exports `___sigaction` (C's `__sigaction`); `libsystem_c` exports `bsd_signal`,
  `sigaction`, `sighold`, `sigignore`, `siginterrupt`, `signal`, `sigpause`, `sigrelse`, `sigset`,
  `sigvec`; `libsystem_platform` and `libsystem_pthread` none of these. Of them, the SDK's headers
  declare five that take a handler (`grep` of `signal.h` and `sys/signal.h`): `bsd_signal`
  (`signal.h:82`), `sigaction` (87), `sigset` (102), `sigvec` (111), `signal` (`sys/signal.h:392`);
  `__sigaction` is exported and declared in neither.
- **glibc 2.41**, Linux arm64 (`nm -D --defined-only /lib/aarch64-linux-gnu/libc.so.6`, names filtered
  the same way): `__sigaction`, `__sigpause`, `__sysv_signal`, `bsd_signal`, `sigaction`, `sighold`,
  `sigignore`, `siginterrupt`, `signal`, `sigpause`, `sigrelse`, `sigset`, `sigvec`, `ssignal`,
  `sysv_signal`; nine of them take a handler by name (`__sigaction`, `__sysv_signal`, `bsd_signal`,
  `sigaction`, `signal`, `sigset`, `sigvec`, `ssignal`, `sysv_signal`), the nine panel 247's
  ffi-pragmatist counted. Their prototypes are unread here: read them.
- **Windows' UCRT**: unread; the seat says which names install a handler there.

**The runtime's own installs, which any definition of those names by the runtime reaches** (`grep -n -E
'(^|[^_a-z])(sigaction|signal)\(' runtime/parts/*.c runtime/platform/*.c`, comment lines dropped): 27
lines, in `parts/os.c` (260, 272, 320, 321, 440, 471, 472), `parts/panic.c` (54), `parts/sigdoor.c`
(104), `parts/run.c` (781, 801, 807, 822, 1220, 1222, 1269, 1392), `parts/stack.c` (686, 1054, 1055),
`parts/sigguard.c` (139, 183, 185) and `platform/hero_net.c` (573, 584, 599, 650). Which of them are
Windows arms is for the seats to read.

**The candidate routes, none built at the base**:

- **I, R4's corrected form**: the runtime defines every installer each platform's C library exports,
  with its whole meaning, keeps a per-thread depth around the program's handler, and the thunk asks it;
  the probes stay on the C library's own entry point. Its open questions are below.
- **W, the link wraps the installers for the libraries the program names**, and for nothing else
  (`--wrap=<name>` on GNU ld and lld, so only references from the objects and archives on the link line
  are redirected): the coordinator's addition, unbuilt; it leaves the runtime's 27 calls and the C
  library alone, and its questions are what Apple's linker offers in its place (unread here: no page
  of it was read), shared libraries, and how `selfhost/cli/link_flags.hero` (215 lines) would carry it.
- **U, a fourth question by the platform's unwind tables**, not frame records (libunwind's
  `unw_is_signal_frame`, `_Unwind_Backtrace`), so a library built without frame pointers but with unwind
  tables is climbed: the coordinator's addition, unbuilt; whether an unwinder is safe inside a signal
  (whether glibc's `dl_iterate_phdr`, which an unwinder may call, takes a lock is unread here) and what
  it costs every callback are its questions.
- **M, the thread's signal mask**: if a delivery adds its signal to the thread's mask unless the
  handler asked `SA_NODEFER` (`sigaction`'s manual says whether; unread tonight), a thunk could compare
  the mask with the thread's own: the coordinator's addition, unbuilt, and it may refuse a correct
  program that blocks a signal itself and calls back (`qsort` under `pthread_sigmask`): say whether it
  is a route at all.
- **K, the residue kept as a known cost**: R3 forbids it in its own words; only the author's change to
  R3 admits it, and a seat that recommends it says so in those words.

**The questions the sitting rules**:

1. **May the runtime define names of the C library at all**, and under which rule: CLAUDE.md § 4
   (architecture), panel 239's member (`ffi_defined_elsewhere`: does the runtime's own definition of
   `sigaction` make a program's library that also defines it a refused program, and should it?), and
   `.claude/rules/c-boundary.md`'s line on the runtime's names (`:345-347`, *"a name the runtime calls
   or defines is the author's where a library a `link` or a `pkgconfig` answer of the program names
   supplies it"*).
2. **How, on each platform**: Darwin's two-level namespace (a dylib's `sigaction` bound to libSystem;
   what `-flat_namespace`, dyld's `__DATA,__interpose`, or `DYLD_INSERT_LIBRARIES` would do, each unrun
   here, and what System Integrity Protection refuses); glibc's resolution (an executable's definition
   preempting libc's for shared objects, libc's own internal calls that never reach it, a library
   loaded by `dlopen`, a raw `syscall(SYS_rt_sigaction, ...)`); Windows' UCRT (a `signal` called from a
   DLL against one linked into the executable).
3. **What it breaks**: the programs and libraries that call the real installer (`SIG_HOLD`, `SIG_IGN`,
   `SIG_DFL`, `sigaction(sig, NULL, &old)`, `siglongjmp` out of a handler); other interposers (the
   address sanitizer, which the `run` form uses at `-O0` and `-O2`,
   `.claude/rules/c-boundary.md:358-363`: on this Mac `nm` of Xcode's
   `lib/clang/21/lib/darwin/libclang_rt.asan_osx_dynamic.dylib` lists `_wrap_signal`, `_wrap_sigaction`
   and the local symbols `substitution_signal` and `substitution_sigaction`; in the Linux image `nm` of
   `/usr/lib/llvm-22/lib/clang/22/lib/linux/libclang_rt.asan-aarch64.a` lists `signal`, `sigaction`,
   `__interceptor_signal` and `__interceptor_sigaction`, each `W`, weak; what a runtime definition of
   the same names does beside them is unrun; and a library of the program defining `sigaction`); static
   linking (`grep -rn -E '"-static"|--static|static_link' selfhost/cli/*.hero` printed nothing, so a
   fully static executable is no mode of `heroes build` by that search; a static archive of the
   program, as 2522's library is, is).
4. **The alternatives with their residues**: W, U, M and K above, and any route a seat finds that this
   list does not hold.

## Question 2: defect 2502, the platform layer's waits

**The defect** (`gh issue view 2502`: OPEN, `kind:defect, class:improvement, area:runtime`,
M-core-packages): every waiting call of the layer waits in slices of 50 ms so a close or the stop ends
it. **Measured again tonight by the coordinator**, lane b35-runtime's probe `waiters.c` (100 threads
each waiting 5 s in `hero_net_recv` on a quiet connection of its own, then each peer closed; or 100
threads in `nanosleep` 5 s) copied to `.claude/worktrees/p248-scratch/briefs/r2502/` and built against
the base's own layer (`clang -O2 -I <tree>/runtime waiters.c <tree>/runtime/platform/hero_net.c -o
waiters -lpthread`; `diff` of the lane's `hero_net_base.c` against the base's `hero_net.c`: one comment,
lines 1169-1171), three runs each under `timeout -s KILL 60 /usr/bin/time -l`:

| | instructions retired | involuntary context switches |
|---|---|---|
| 100 waiting 5 s | 273,025,580; 274,804,922; 274,702,290 | 10,338; 10,449; 10,472 |
| 100 sleeping 5 s | 22,022,639; 21,849,546; 21,667,267 | 225; 236; 222 |

The issue's own figures (273,547,249 against 28,434,810) and the comment's (281 to 285 M against 22
M) are *carried*; its *"about 1.8 cores at 10,000 idle waiters"* is the lane's arithmetic, an
inference, unrun at that size.

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
instructions over 0, 5 and 10 s; a close ending a waiter in 99 to 131 µs; *"two descriptors per thread
that has waited, so a program with 100 waiting connections needs 409 descriptors instead of 207, past
Darwin's default soft limit of 256"*; *"on Windows `WSAPoll` cannot watch a pipe or an event"*. Its
files, read-only: `.claude/worktrees/b35-scratch/b35-runtime/r2502/hero_net_proto.c` (1295 lines),
`closer.c`, `proto.log`, `fds.log`, `closer.log`.

**The descriptor budget, read tonight**: `launchctl limit maxfiles` printed `maxfiles 256 unlimited`,
launchd's own limits, the 256 the lane's comment calls *"Darwin's default soft limit"*; **this session's
shell reads `ulimit -n` 1048576** and `ulimit -Hn` unlimited, so a measurement in a shell like it does
not meet 256 (which processes start at 256 is a question): run one under `ulimit -n 256`;
`sysctl kern.maxfilesperproc` 92160; the Linux image 1048576 soft and hard. The layer's table holds
65,536 slots (`hero_net.c:166`).

**The candidate routes**: (a) a wake descriptor per waiting thread, the lane's; (b) one shared wake
descriptor with a generation counter, every waiter woken by any close and asking its own socket again;
(c) one per socket, made with the socket; (d) the status quo, or the slice lengthened or grown while a
wait stays quiet; and for each POSIX route, `eventfd` on Linux, `kqueue`'s `EVFILT_USER` on Darwin, or a
pipe. **Windows**: `WSAEventSelect` with `WaitForMultipleObjects` (panel 246's recorded arm), or a
loopback socket pair `WSAPoll` can watch (the coordinator's addition, unbuilt), or the slices kept
there alone. **The questions**: what each route costs in descriptors, in instructions while waiting and
in a close's latency; what the slice-free wait does where no descriptor can be made (`HERO_NET_LIMIT`,
or a fall-back to slices, the lane's recommendation); what `hero_net.h`'s two *"within about 50 ms"*
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
(`fetch/fixedbugs-2479-a-command-run-from-inside-a-placed-tree-writes-nothing-there.case`, nine of its
eleven command lines opening `cd <work>/...`). `tests/harness/platform_skips.hero:151-153` names
`fixedbugs-2344-an-empty-cpath` among the cases `darwin_alone` skips on every other platform anyway, so
**at most three could run on Windows with the door, by reading**. The two other 2344 cases' variables
are `C_INCLUDE_PATH=:` and `C_INCLUDE_PATH=/nonexistent-2344:` (their `environment` files), whose `:`
the lane's comment says clang on Windows may read otherwise (*"unread and unrun"*): whether they pass
there is the seats' question.

**What the base does** (`wc -l`: `runtime/parts/run.c` 2104, `runtime/hero_os.h` 513): `hero_run_go`
(`run.c:1600`); on POSIX `fork` (1939), then in the child (`hero_run_child`, 1333): `setpgid` (1372),
the parent's caught signals given back (1376), the write ceiling (1382-1400), **the three streams
opened by their paths as given** (1416-1446, each failure written down the report pipe and `_exit(126)`,
1422, 1433, 1444), then `execvp`, `execvP` or `execvpe` (1460-1467), `_exit(127)` (1477). On Windows
`CreateProcessW(NULL, wide_line, NULL, NULL, TRUE, ..., env_block, NULL, &startup, &child)`
(1794-1797), the NULL before `&startup` being `lpCurrentDirectory`. **The door it would sit beside**:
`hero_run_env` (`run.c:155`; `hero_os.h:363-377`), panel 242's **R6** (`docs/panel/242-*.md:176-188`,
ratified 2026-11-10 at `:276-277`): staged for the next child alone, *"so no process-wide write
exists"*, the program found *"through the parent's `PATH` on every system"*. Decision 2228 (`gh issue
view 2228`, CLOSED): `hero_os.h` *"is the language's door"*, answered by the author 2026-10-27; spec
§ 13 names *"processes ... from the language's `hero_os.h`"* (`spec/heroes-spec.md:377`). **The net**:
`tests/harness/case_setting.hero:151-161` (`skip_here`), 258 (the `env -C` line); `suite_fetch.hero:141-152`
(its skip), 217 (`ENV`), 342-343.

**The questions**: the door's name and staging (given back by `hero_run_reset` as `hero_run_env`'s
variables are); **POSIX**: `chdir` between `fork` and `exec` in a parent that may hold other threads,
read against POSIX's list of async-signal-safe functions (the lane's comment says *"`chdir` and
`fchdir` are on POSIX 2024's list"*, *carried*: read the page and quote it), with the other calls the
child already makes there read against the same list; **its order** against the three streams the
child opens by path and the program's own path when it is relative (before `chdir`, the parent's
folder; after, the child's); `posix_spawn_file_actions_addchdir_np` or POSIX 2024's
`posix_spawn_file_actions_addchdir` as the other shape, against `hero_run_go`'s `fork` for its
`setpgid`, terminal and write ceiling; **Windows**: what `lpCurrentDirectory` takes (a relative path, a
missing folder, the error that answers), from Microsoft's page, compiled and owed; **a missing folder**
told how, and by which number `hero_run_why` answers; and which of the four cases the door would let run
on Windows, each read, the run owed to the CI's leg.

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
answer 1"*); panel 247's R7 (`:255-271`): G, `hero_signal_watch`, answers -1 for SIGINT and SIGTERM,
*"it is not a stop door"*.

**The questions**: a door such as `hero_net_ask_stop()`, what it does before `hero_net_catch_stop` and
after; whether a Ctrl-C after the program asked its stop ends the process at once, as a second signal
does today, or counts as the first; its thread-safety (no signal, any thread, the counter and the pipe's
write); what wakes a wait on Windows, where only the slices see the counter; whether `<core>/net` gains
it and the reader `stop_asked`, in which version; and whether line 404's *"only"* becomes false with it,
which is the escape above.

## Question 5: a connection that sends nothing

**No issue exists**: the same search with *deadline*, *timeout net*, *SetReadDeadline*, *read deadline*,
*idle connection*, *connection sends nothing*, *slowloris*, *hero_net_recv timeout* named none (2502
answered *idle connection*). **No sitting ruled one**: `grep -l -i -E 'SetReadDeadline|SetDeadline|read
deadline' docs/panel/*.md docs/panel/*/*.md` printed nothing; panel 246's critic listed *"an `accept`
with a deadline"* as a route nobody built (`docs/panel/246-reports/critic-reports.md:371-375`,
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
joins the oldest thread before the next accept once `threads.len() >= room` (115-117), `room` being
`hero_thread_limit()` (86), which answers `HERO_SPAWN_SLOTS`, 256 (`runtime/parts/spawn.c:105`, `634`);
a connection's thread waits in `http`'s head read (`net/http/internal/lines.hero:58`, `net.read`) until
the peer sends, closes, or the stop comes. **So a server holding 256 connections whose oldest sends
nothing stops accepting until that peer closes: an inference from reading, unrun**, and the seats run
it. And a connection crosses to its thread as a number (`hero_net.h:103-121`), after which no handle of
`main`'s names it, so `main` cannot close a quiet one to end its wait: read, unrun.

**The candidate routes**: a deadline per call (a `recv` or `accept` that takes a limit); per socket, as
Go's `SetReadDeadline` and `SetWriteDeadline` (an absolute time the socket's waits end at, cleared by
setting none); a limit in `net/http`'s `serve_conn` alone (Go's `ReadHeaderTimeout` and
`IdleTimeout`); and the answer's kind, `HERO_NET_TIMED_OUT` reused or a new one. The sockets are
non-blocking (`hero_net.c:46-48`) and every wait is the layer's own `poll` or `WSAPoll`, so the
system's `SO_RCVTIMEO` should reach no wait of the layer's and a deadline would be that wait's own
bound, on every platform: an inference from reading, which the ffi-pragmatist runs. How it is clocked
(a clock that never goes back) is its question. **And the lane question**: the face in `net` and `net/http` is something a
program writer reads on the package's card; whether that keeps it in this lane, as panel 246 kept the
stop's documentation at 0 spec tokens in `hero_net.h` and the card, or sends it to the full panel, each
seat says, with why.

## What the coordinator measured (each with its command, above)

The base's two stages and their `sources` line, on this Mac and in Linux arm64; 2522 reproduced on
both (`.claude/worktrees/p248-scratch/briefs/r2522/storms.log` and `linux/linux.log`); 2502's probe,
three runs each (`r2502/runs.log`); the installers' lists (Darwin's stubs, glibc's `nm -D`); the
runtime's 27 installs; the four cases of 2490; the descriptor limits; the platforms' state; the issue
searches (`.claude/worktrees/p248-scratch/briefs/searches.txt`, 22 terms). **Line counts** (`wc -l`):
`runtime/parts/sigguard.c` 364, `sigdoor.c` 137, `thread.c` 144, `alloc.c` 1540, `run.c` 2104,
`os.c` 1019, `stack.c` 1213, `spawn.c` 634; `runtime/platform/hero_net.c` 1235; `runtime/hero_net.h`
176, `hero_os.h` 513, `heroes_runtime.h` 995; `selfhost/emit/callback_thunk.hero` 343,
`callback_guard.hero` 207; `selfhost/cli/supplied.hero` 357, `link_flags.hero` 215;
`packages/core/0.3.0/files/net.hero` 288, `net/hero_net.hero` 173, `net/http.hero` 150,
`net/http/internal/server.hero` 217, `net/http/internal/lines.hero` 173; `examples/server/main.hero`
206; `tests/harness/case_setting.hero` 498, `suite_fetch.hero` 503, `platform_skips.hero` 335.

## Questions no one has run, posed as questions

- Whether dyld honours an `__interpose` section in a program's own executable, and whether
  `-flat_namespace` makes a dylib's `sigaction` reach the runtime's, on this Mac.
- Whether the sanitizers' interceptors of `signal` and `sigaction` and a runtime definition of the same
  names can stand in one program, at `--sanitize`'s `-O0` and `-O2`.
- Whether glibc's `bsd_signal`, `sysv_signal`, `ssignal` or `sigset`, called by a library, ever reach
  an executable's `sigaction` (panel 247's critic read none did; re-run it at the base).
- Whether an unwinder can find the delivery's frame through `sigcb_handler` built
  `-fomit-frame-pointer`, and at what cost a callback.
- What a slice-free wait costs on Windows, and whether `WSAPoll` can watch a loopback socket pair as a
  wake descriptor.
- Whether `chdir` stands on POSIX's list of async-signal-safe functions, and what `CreateProcessW`
  does with a relative or missing `lpCurrentDirectory`.
- Whether the two `fixedbugs-2344` folder cases would pass on Windows with the door.
- Whether a server built like `examples/server/main.hero` stops accepting while 256 quiet connections
  hold its threads.
