# Panel 249, the brief every seat reads first

Convened 2026-11-14 by the coordinator, in M-core-packages (`gh issue view 2522 --repo heroes-lang/heroes
--json milestone -q .milestone.title`: `M-core-packages`), as **the follow-up sitting panel 248's R1 owes
on defect 2522** (`docs/panel/248-the-c-librarys-installers-wait-for-a-census-*.md:208-223`), convened
once 248's ratification was read: *"defect 2522 open and `blocking`, its follow-up sitting convened now,
as R1 says"* (`:493-505`, the author's answer at about 21:47); decision 2527 is closed (`gh issue view
2527 --repo heroes-lang/heroes --json state,closedAt`: `CLOSED 2026-11-14T20:56:59Z`). **The soundness
lane and the historian**: the compiler-engineer, the ffi-pragmatist and the historian (advisory). **The
completeness critic reads these briefs before any seat starts** (`critic-briefs.md`, beside this file)
and the reports after. **No paid run is named in this sitting**, so none is started by anyone (§ The
rules).

**Where this brief is wrong, say so first.** Every number, path and count below carries the command that
produced it, run by the coordinator between 22:08 and the time at the foot of this file on 2026-11-14,
every file of those runs under `.claude/worktrees/p249-scratch/briefs/` (called `briefs/` below); a
figure copied from a record, an issue or an earlier sitting says *carried* beside it and is yours to
re-run before you lean on it. **The completeness critic's first pass** (`249-reports/critic-briefs.md`, 25
items) was applied before any seat started; the texts before it are kept as `*.before-critic.md` beside
this file, and its item 20, the lane, is the coordinator's.

## The mandate

- **CLAUDE.md § 4** (its lines 200-201): *"Ask once per milestone, then convene without asking again,
  choosing only the seats whose input differs"*. M-core-packages' gate was asked at panel 242 (248's
  status, `248-*.md:3-4`, *"convened ... without asking again (the milestone's gate was asked at panel
  242 ...)"*, carried); this sitting is 248's own order, R1's follow-up.
- **Who sent the question**: 248's R1 (`248-*.md:208-223`), and defect 2522's second comment
  (`gh issue view 2522 --repo heroes-lang/heroes --comments`, posted 2026-11-14T20:31:50Z by `gh issue
  view 2522 --json comments -q '[.comments[].createdAt]'`), which restates the census and the three
  candidates *"named and unbuilt"*. Its first comment (2026-11-14T10:52:00Z) is the CI's widening (§ Question
  2, candidate C).
- **What this sitting rules**: the census panel 248's completeness critic asked for (item 20,
  `docs/panel/248-reports/critic-reports.md:230-246` and `:341-345`); each candidate route run against
  the census's first entries; and what closes 2522 if no candidate is sound everywhere. **What it does
  not rule**: R1 and R2 of panel 247 (the thunk's three questions and the allocator's guard D, which stand
  beneath, `248-*.md:224-226`), panel 246's R4 (*"No Heroes code runs in a handler"*,
  `docs/panel/246-*.md:318`, carried), and panel 247's R3, whose words bind every answer: the residue is
  *"`blocking` ... Never a known cost"* (`docs/panel/247-*.md:191-195`).

## The lane, what it gives up, and the escape

`SKILL.md:18-23` (the panel skill, `.claude/skills/panel/SKILL.md`): the soundness lane is the
compiler-engineer and the ffi-pragmatist, *"Use it when the proposal changes **no surface, no diagnostic
and no spec token**"*. The historian sits beside them because the census's other half is the libraries'
documented behaviour and the precedents, which neither building seat is set to read (CLAUDE.md § 4,
*"choosing only the seats whose input differs"*). **What the lane gives up**: the spec-warden's real
token counts and the blind seat's reading; no spec sentence, diagnostic class or card is changed by the
questions as posed.

**The escape, and it binds every seat** (`SKILL.md:29`: *"When in doubt take the full panel"*; panel
248 was drafted for this lane and escalated on its critic's item 23, `248-reports/critic-briefs.md:603-606`,
carried). A seat whose route or whose answer to question 3 needs **a spec sentence** (spec § 13's lines
403-404 say *"No function of the program is a signal handler"*, `sed -n 403,404p spec/heroes-spec.md`),
**a new or a widened diagnostic** (a compiler message a program did not get yesterday; panel 239's
`ffi_defined_elsewhere` widens under any route that makes the runtime define a C library name it does
not call today, which both of 248's building seats measured, `248-reports/compiler-engineer.md:162-177`
and `248-reports/ffi-pragmatist.md:304-307`, carried), **new words of a runtime refusal** a reader must
learn, or **a change a binding's author must learn** (a file beside every program, a link flag, a
reserved signal), **says so in its report in those words**, and the synthesis convenes the spec-warden,
and the blind seat where a reader could get it wrong, before it adopts that route.

## The base

**`722ae76bf693bebdb7a5cca37052ed92646753c0`**, its tree **`ce81c0b18b1fbeb616f95aeef619d5b8cd23bfbe`**
(`git rev-parse HEAD` and `git rev-parse 'HEAD^{tree}'` in `.claude/worktrees/p249`), `git log -1`:
*"Panel 248 ratified, R1 to R6, and the two choices of R5's landing kept"*, 2026-11-14 21:55:01 +0100,
the head of branch `p249`, `git status --short` printing 0 lines at 22:08. It holds batch 36's landing
of 248's R2 to R5: `git log --oneline df7612ad3..722ae76bf | wc -l` prints 37 commits since 248's base,
and `git diff --stat df7612ad3 722ae76bf -- runtime/` 7 files, 1,605 insertions and 214 deletions,
almost all in `runtime/platform/hero_net.c` (+1,104, -137) and `runtime/parts/run.c` (+406, -64)
(`git diff --numstat`). **The signal
guard did not move**: `git diff --stat df7612ad3 722ae76bf --` over the nine paths `runtime/parts/sigguard.c`,
`sigdoor.c`, `stack.c`, `thread.c`, `runtime/runtime.c`, `selfhost/emit/callback_thunk.hero`,
`callback_guard.hero`, `selfhost/cli/flags.hero` and `supplied.hero` prints nothing; `alloc.c`, outside
them, moved +12 and -4 for defect 2490's leak check (`hero_run_folders_held_at_exit`, read with `git
diff`; the critic's item 1 corrected this sentence). So 248's measurements of the guard stand on the same code, and are *carried*. Every path below is from the
repository's root, `/Users/joseph/Temp/heroes/heroes-lang`.

Every seat that builds copies the commit and builds its own compiler in two stages:

```
mkdir -p .claude/worktrees/p249-scratch/249-<seat>/tree .claude/worktrees/p249-scratch/249-<seat>/tmp
git -C .claude/worktrees/p249 archive 722ae76bf693bebdb7a5cca37052ed92646753c0 | tar -x -C .claude/worktrees/p249-scratch/249-<seat>/tree
cd .claude/worktrees/p249-scratch/249-<seat>/tree
export TMPDIR=$PWD/../tmp
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes       # the seed
./heroes build selfhost/main.hero -o heroes-next                 # stage 2
./heroes-next --version                                          # second line: sources 2ab7ca0a644692a1
```

Measured by the coordinator from 22:08 in `briefs/base/tree` (the seed's file 22:12:17, stage 2's 22:14:54, `stat`), made exactly that way: the archive
holds no `build/` (`ls build`: *No such file or directory*), the seed exit 0, stage 2 exit 0,
`heroes-next --version` printed `heroes 0.6.0` and **`sources 2ab7ca0a644692a1`**, and `heroes-next
doctor` printed `ok   runtime      runtime` and *Apple clang version 21.0.0 (clang-2100.4.1.1)*. The same
`sources` line came out of the same two stages in the Linux arm64 image (`briefs/linux/base-arm64.log`,
22:15:21 to 22:17:42 by the `date` around it, `docker exit 0`) and in the emulated x86-64 image (§ The platforms). **No duration a seat
writes is a measurement while other sessions run** (CLAUDE.md § Verification): `uptime` read load
averages *14.84 131.63 167.39* at 22:08 and *282.80 156.39 142.40* at 22:24, other sessions building;
count instructions (`/usr/bin/time -l`, *instructions retired*), never seconds, or say why a clock was
still. **A seat whose stage 2 prints another `sources` line is not on the base and says so first.**

**The copy has no `.git`**, and the provenance hook then judges a harness run in it against the trunk's
tree: 248's compiler-engineer met it and gave the copy a `.git` FILE reading `gitdir: /nonexistent-<seat>`,
which makes the hook judge the copy and keeps git inside it from climbing to the trunk's repository
(`docs/panel/248-reports/compiler-engineer.md:18-27`, carried; the hook's rule is
`.claude/hooks/trees.py`, the nearest directory holding `seed/heroes.c` and a `.git` entry). The `run`
form ran in a `git archive` copy with no `.git` in 248's Linux containers (`248-reports/ffi-pragmatist.md:451-452`,
carried).

## Where each seat works, and the rules every seat obeys

Every seat's folder is **`.claude/worktrees/p249-scratch/249-<seat>/`**, ignored by git (`git
check-ignore -v .claude/worktrees/p249-scratch` in the trunk: `.gitignore`'s `.claude/worktrees/`
line), never the session scratchpad, never `/tmp`, never the home directory: CLAUDE.md § Hard stops
writes no file outside the repository's root. The critic's is `.claude/worktrees/p249-scratch/critic/`.
**Read-only to every seat, copied into your own folder before use**, under `briefs/inputs/`, each file's
first sixteen hex digits of `shasum -a 256` and its `wc -l` in `briefs/inputs-manifest.txt` (30 lines):

- `tools/storm.py` (`8e6b762aaff74575`, 57 lines) and `tools/ips.py` (`92071c5e2328c61b`, 31), panel
  247's storm driver and crash-report tally, the same hashes 248's brief named (copied from
  `.claude/worktrees/p248-scratch/briefs/tools/`). `python3 -I storm.py <binary> <SIGNAME> <seconds>
  <arm-delay>` starts the binary in a session of its own, keeps 65,536 bytes a stream and sends SIGKILL to
  its group at the end; **it arms by the clock** (its lines 33-36), so **every storm target prints
  `armed` once its handler stands, and a run whose output lacks it is not counted**. `python3 -I ips.py
  ~/Library/Logs/DiagnosticReports <binary-name>...` tallies reports by signal and kind (`SHOW=1`, one
  stack per kind), a sample, never a count: match the reports' pids to the storms'.
- `ev247/`, panel 247's evidence unpacked from the frozen tree (`tar -xJf
  .claude/worktrees/p249/docs/panel/247-reports/evidence.tar.xz`): R3's programs and library,
  `247-ffi-pragmatist/h/flib.hero` (`c321647e18344be9`, 19 lines), `h/fliblocale.hero`
  (`618391f665694fdc`, 37), `h/sigcb.h` (`8f94416fb6d4dce3`, 4), `h/lib/sigcb.c` (`d14966c6e36aea9d`,
  21); panel 247's cost ladder `247-compiler-engineer/cases/qsortcost.hero` and `.h` (22 and 26 lines).
  Never unpack an archive inside a worktree.
- `p248-fp/`, panel 248's ffi-pragmatist's probes (copied from
  `.claude/worktrees/p248-scratch/248-ffi-pragmatist/`): `gc-8.2.8.tar.gz` (bdwgc's release,
  `7649020621cb2632`, the hash 248's report names), `gcreal.c` (39 lines, a list rooted on `main`'s stack,
  another thread forcing collections), `gcshape.c` (66), `inst.c` (204, route I-onstack's definitions in
  C), `inst_mask.c` (247, route I-mask's, its marker `#define HERO_MARK SIGURG` at line 64), `rows.c` (152, R4's rows), `diff.c` (108, the differential
  test), `siginstall_mask.c` (236, `#define HERO_SIGNAL_MARK SIGURG` at line 52) and `sigguard_mask.c` (374,
  whose probes still borrow SIGURG at 184-190), I-mask built into the Linux runtime,
  `phases.sh` (206, its Linux script); `p248-fp/darwin/ipose_kernel.c` (23, the one-name I-dylib) and
  `ipose_mask.c` (37); `p248-fp/alt/` `b.c`, `t.c`, `u.c`, `ulib.c`, `maskcost.c` (routes B, T, U and
  I-mask's cost).
- `p248-critic/`, 248's critic's: `swait.c` (16 lines, a program blocking SIGURG and waiting for it in
  `sigtimedwait`), `hunblock.c` (18, a handler that unblocks before calling back), `inh.c` (15, a mask
  inherited with SIGURG blocked), and its Linux script as `critic-r2-linux-run.sh` (39).
- `p248-ce/siginstall.c` (250 lines, `df1390978d6811c9`), 248's compiler-engineer's route I-onstack as
  built into the runtime.

The rules:

- **Your own folder, and nothing else**: the tree in `tree/`, every probe beside it, `TMPDIR` set to
  its `tmp/` before any build or tool. Never the trunk's `./heroes`, a compiler of `.claude/worktrees/p249`,
  of `briefs/`, of a lane or of another seat; never read, build or run inside another seat's folder or a
  lane's worktree (`.claude/worktrees/b37-*` were open when this was written, `ls .claude/worktrees`).
- **Your report**: `docs/panel/249-reports/<seat>.md` in `.claude/worktrees/p249` (make the folder if it
  is absent), written as you go, not only at the end, in your definition's output structure
  (`.claude/agents/<seat>.md`). Write nothing else in `.claude/worktrees/p249`.
- **Every seat that builds builds the route it recommends, in its own copy, and runs the cases it
  names, before the synthesis** (`SKILL.md` 3c, its lines 274-277): a route that does not build is
  reported as not built, never adopted.
- **Every number with the command that produced it; every negative sentence run, or posed as a
  question**; the words *only*, *never*, *no library*, *provably* with what was searched and where
  (`/panel` step 2; CL-077). A census row that says a library does not use a signal says how that was
  asked (its imports, its source at a named version, a run) and is never read from memory.
- **No paid run**: no `claude -p`, no API call, no `heroes measure --refresh`, no cloud run
  (`SKILL.md:288-295`). One you find worth running goes in your report with its size and cost, for the
  coordinator.
- **Never print a credential**: `gh auth status` at most, never `gh auth token` (`SKILL.md:296-304`). No
  GitHub write, no issue, no comment, no commit, no push, no tag. `gh issue view`, `gh run view` and a
  `gh api` GET are reads and allowed.
- **No file outside the repository's root, and the tools that write one by default**: no `brew install`
  (Homebrew's prefix is `/opt/homebrew`, whose `Cellar` the table below reads); no `docker pull` and no image built or kept (Docker's store is
  its VM's `/var/lib/docker`, `docker info`, held under `~/Library/Containers/com.docker.docker`, which
  `ls -d` finds); `apt-get` only inside a `docker run --rm` container, whose writes leave with it (panel
  188's ffi-pragmatist fetched `clang-18` so, `docs/panel/188-reports/ffi-pragmatist.md:466-469`); and
  **Go only with every one of its writes moved into your folder: the exports below come before EVERY
  `go` command, `go version` and `go env` included.** A bare `go version`, `HOME` unmoved, run by this
  sitting's critic at about 22:54, wrote three files under the home directory's Go telemetry folder,
  born 22:54:53 by `stat -f %SB` (`249-reports/critic-briefs.md` § B and item 23, reported there for the
  author). The coordinator's run at 22:15 (`go.mod` 22:15:43, the archive 22:15:46, `stat`), with this
  Mac's `/opt/homebrew/bin/go`, ran `go version` (*go1.27.3 darwin/arm64*) after the exports:

  ```
  export HOME=<your folder>/gohome GOCACHE=<your folder>/gohome/gocache GOPATH=<your folder>/gohome/gopath \
         GOMODCACHE=<your folder>/gohome/gopath/pkg/mod GOTMPDIR=<your folder>/tmp TMPDIR=<your folder>/tmp \
         GOTOOLCHAIN=local GOPROXY=off GOFLAGS=-modcacherw
  go build -buildmode=c-archive -o libgoanswer.a .       # in a folder holding main.go and go.mod
  ```

  `go env GOTELEMETRYDIR` then named a path under `briefs/go/home/`, `go build -buildmode=c-archive` exit
  0 (`libgoanswer.a` 3,559,248 bytes, `libgoanswer.h` 1,917, `ls -la`), and the home directory's Go
  telemetry folder kept its modification time (`stat -f %m`: 1727731417 before and after), with no
  `~/Library/Caches/go-build` and no `~/go` before or after, and `find ~/Library/Application\ Support/go
  -newer go.mod` printing nothing. The telemetry directory follows `HOME` (`go env GOTELEMETRYDIR`), and
  the home directory already holds one Go wrote on an earlier day: Go run with `HOME` unmoved writes there.
- **One binary name per seat**: every binary you build carries your prefix, **`ce-`** for the
  compiler-engineer, **`fp-`** for the ffi-pragmatist (`ce-fliblocale`, `fp-gc-imask`), and `ips.py` is
  asked of those names alone; every Docker container is named `<prefix>-249-<n>`.
- **Every Darwin death reads its crash report**: a recursive lock in libplatform traps and the report
  names SIGKILL with nothing on stderr (248's brief, its critic's section B, carried; R3's storms at this
  base read so today, § Question 1), so a run is judged by its `.ips` reports and their pids.
- **Every program that may not end is bounded in time and in bytes**
  (`.claude/rules/verification.md` § A run that may not end is bounded in time and in bytes), and a seat
  that runs one not in the table names its bounds in its report first. Built with `heroes build`,
  clang or `go build`, the BINARY run under `timeout -s KILL <seconds>`, never `timeout heroes run` and
  never a plain `timeout` (a program that caught SIGTERM outlives it); its output to `/dev/null` or
  through `head -c 65536` into a file; afterwards `pgrep -f <your folder>` read and anything left
  stopped before a verdict. Here `timeout` is `timeout (GNU coreutils) 9.12` (`/opt/homebrew/bin/timeout
  --version`), which kills the command's process group; **a child the Heroes runtime spawns has a group
  of its own** (`setpgid(0, 0)` at `runtime/parts/run.c:1648` and `:1659`, `grep -n 'setpgid(0, 0)'`) and is looked for by
  its own name. The bounds:

  | program | time | bytes |
  |---|---|---|
  | a storm's target | `storm.py`'s own (30 s after the storm, then SIGKILL to the group), run unwrapped | 65,536 a stream, the driver's |
  | a storm's target that spawns a child (a Heroes launch, a library's worker process) | not stormed through `storm.py` unless every child's output goes to `/dev/null`: the driver's reader threads end only at end of file (`storm.py:52`, `for t in ts: t.join()`), and the runtime gives a child a group of its own, so a child holding the pipes outlives the group's SIGKILL and hangs the driver (the critic's item 22); `pgrep -f <child's name>` after every storm | the same |
  | a census probe (a library under a route, a collector's loop, a Go archive's program) | `timeout -s KILL 60` | `head -c 65536` |
  | a waiter for a signal (`sigtimedwait`, `sigwait`, `signalfd`) | its own timeout of at most 10 s, under `timeout -s KILL 30` | `head -c 65536` |
  | a stage 2 build | `timeout -s KILL 1800` on this Mac or in Linux arm64; **`timeout -s KILL 3000` in the emulated x86-64 image** (below) | its log under your folder |
  | a Docker run | below, § The platforms | `head -c` inside the script |

- **One heavy command at a time** (a stage 2 build, a suite, an emulated container): other sessions
  build on this Mac. Whole suites only where your route changes what the checker refuses, named with
  why; otherwise the `run` form narrowed to your cases (`./heroes-next run tests/harness/main.hero --
  ./heroes-next run <case-substring>`), one at a time. **A route under `runtime/` owes the `runtime`
  suite** (`.claude/rules/verification.md`, the `runtime/**` row); **a route under `selfhost/cli/` owes
  what that file's row names** (`flags.hero`: `run` whole and `cache` alone, the same table).

## The platforms

- **Darwin arm64**: this Mac, macOS 27.2 (`sw_vers -productVersion`), Darwin 27.2.0 (`uname -r`), SDK 27.2
  (`xcrun --show-sdk-version`), Apple clang 21.0.0 (`heroes-next doctor`), Apple's linker `ld-27106.1`
  (`ld -v`). `pmset -g assertions` read `PreventUserIdleSystemSleep 1` at 22:08, Amphetamine holding it
  (`.claude/rules/verification.md` § A long run holds the machine awake): ask it again before a long run,
  `caffeinate -i` only where it reads 0.
- **Linux arm64**: the image `heroes-linux-arm64:latest`, `338a1044ac90`, created 2026-09-17 (`docker
  images --format '{{.Repository}}:{{.Tag}} {{.ID}} {{.CreatedAt}}'`), `arm64` (`docker image inspect
  --format '{{.Architecture}}'`); inside it `uname -m` *aarch64*, `ldd --version` *Debian GLIBC 2.41-12+deb13u3* (one
  `docker run --rm` between the `date` readings 22:39:36 and 22:39:52), and `doctor` *Debian clang version 22.1.8 (1~deb13u4)*
  (`briefs/linux/base-arm64.log`). A second arm64 image stands, `heroes-linux-arm64:sdl3-b14`
  (`75742c2bf90b`, 2026-10-07), whose contents this brief did not read. Write a script into a folder of
  yours and run it from the repository's root:

  ```
  git -C .claude/worktrees/p249 archive 722ae76bf693bebdb7a5cca37052ed92646753c0 | timeout -s KILL 1800 docker run -i --rm --name <prefix>-249-<n> -v <your folder>/linux:/out heroes-linux-arm64:latest bash /out/run.sh
  docker ps --filter name=<prefix>-249-<n> --format '{{.Names}} {{.Status}}'   # after it, whatever its exit
  docker stop -t 1 <prefix>-249-<n>                                            # where that line printed a container
  ```

  the script reading the tree from its stdin (`tar -xf - <&0` into a folder of the container's own),
  building the seed and stage 2 there, **every probe inside it under its own `timeout -s KILL`** (the
  image has GNU `timeout`), writing its log under `/out` and bounding every output with `head -c`. **The
  outer `timeout` bounds the client, not the container** (248's critic item 26, re-run by 248's
  coordinator: a killed `docker run` left its container *Up*, carried), and `pgrep -f <your folder>`
  does not see a container's processes. One container of yours at a time. **`apt-get` works in it**: one
  `docker run --rm` at 22:14 (`briefs/linux/probe.sh`; its `apt-update.txt` 22:14:56, `stat`) printed `apt-get update exit 0` and the candidates
  below (§ Question 1, the libraries).
- **Linux x86-64, emulated**: the image `heroes-linux:latest`, `6f78598a4eba`, created 2026-09-09,
  `amd64` (`docker image inspect`); inside it `uname -m` *x86_64*, *Debian GLIBC 2.41-12+deb13u3*,
  *Debian clang version 22.1.8 (++20260613092233+e80beda6e255-1~exp1~20260613092250.77)*, `ulimit -n`
  1048576 (one `docker run --rm` between the `date` readings 22:11:59 and 22:13:16, which printed the platform-mismatch warning that
  `docs/platforms/linux/LINUX-MACHINE.md:170-178` calls expected, and says why `--platform` stays off the
  line). It runs on this arm64 Mac by emulation: which emulator Docker uses, `docker info` did not say
  (its `Architecture: aarch64`, `Kernel Version: 7.0.14-linuxkit`). It left the routine on 2026-10-02
  (`.claude/rules/platforms.md:20-24`), and lane b36-net measured in it on 2026-11-14 (`248-*.md:529`,
  *"emulated Linux x86-64 every step passed"*, carried). **It is not the CI's leg**: the CI's Linux
  x86-64 job runs `ubuntu-26.04` (`gh api repos/heroes-lang/heroes/actions/jobs/132377462166 -q .labels`)
  with *Ubuntu clang version 21.1.8 (6ubuntu1)* (that job's log, line 1329, `briefs/ci/job-132377462166.log`).
  A seat that measures there says *emulated*, names the clang, and leaves the leg's verdict to the CI.
  **The base in it** (`briefs/linux/base-amd64.log`, 22:17:42 to 22:36:09 by the `date` around it, `docker exit 0`): the seed exit 0
  after 27 s, stage 2 exit 0 *"after 1102 s"* counted from the start, the seed's 27 included (`base.sh`
  sets its clock once, at its line 4; the critic's item 2), so stage 2 alone about 1,075 s of wall at the
  load above (not a measurement: plan for it), bounded there at 3000 s; `sources 2ab7ca0a644692a1`,
  `doctor` printing `arch x86_64`. A container that holds two emulated stage 2s, a harness build and its
  cases is split in two, each bounded on its own.
- **Windows: the box is OFF.** `ssh -o BatchMode=yes -o ConnectTimeout=15 win 'echo up'` at 22:11:59
  printed *"ssh: connect to host apponfly-vps port 22: Operation timed out"*, exit 255.
  `.claude/rules/platforms.md` § The Windows box: *"Paid by the hour, usually off, and only the author can
  start it"*. **A seat that needs a Windows measurement names it in its report as owed**, and **the CI's
  Windows leg judges after the push** (`.claude/rules/verification.md` § The optimistic chain, item 4).
  What 248 measured there, *carried*: an executable cannot define `signal` beside UCRT with either linker
  the project uses (*"duplicate symbol: signal"*, `LNK1169`, `248-reports/ffi-pragmatist.md:270-275`); a
  console's SIGINT handler runs on a thread Windows makes, and `raise` runs its handler synchronously on
  the calling thread (`:284-290`); panel 247's R10 (`docs/panel/247-*.md:286-291`): no probe and no walk on
  Windows.

## Question 1: the census

**What a route may change in what a program's own C observes, and which libraries a Heroes program
plausibly binds depend on it** (248's critic, item 20, `248-reports/critic-reports.md:341-345`: *"make
that list before choosing ... and run each route against its first entries"*). Eight things, the
coordinator's naming of the critic's question and this sitting's charge, the last four added on this
sitting's critic's item 14: **(1) the stack its handler runs on; (2) its thread's signal mask, in a
handler and outside one; (3) the signal numbers it may use (block, wait for, send, install); (4) the
frames its own handler finds when it walks or reads its stack; (5) the action a query answers
(`sigaction(sig, NULL, &old)`), and the order handlers are installed and chained in, a constructor's
before `main` included; (6) the context a handler reads and writes (the kernel's `siginfo_t` and
`ucontext_t`, which a handler may edit to resume elsewhere); (7) `errno` and `SA_RESTART` as the
interrupted code sees them; (8) which thread a process-directed signal reaches.** And for each library:
which it depends on, on which platform, measured.

### What the thunk asks today, and what R3 leaves

`runtime/parts/sigguard.c`, 364 lines (`wc -l`), unchanged since 248's base (§ The base):
`hero_callback_entered` at 316, the thunk's first line (emitted at `selfhost/emit/callback_thunk.hero:146`,
343 lines), asks **the exact test** at 320 (the thunk's return address against the delivery's, learned
by `hero_signal_learn` at 196, whose two probes install **SIGURG** through the C library's own
`sigaction` and raise it, 175-186, with SIGURG let through on that thread for the probes and the mask put
back, 205-216); refuses a thread nothing claimed at 324; asks **`SS_ONSTACK`** at 332 where the thunk's
frame lies outside the thread's stack; and **walks** at most `HERO_SIGNAL_WALK` (8, line 156) frame
records from 338. Its comment names what it does not see (49-57): *"a handler whose frames keep no
record (a library built `-fomit-frame-pointer`), one more than HERO_SIGNAL_WALK frames below the Heroes
function ..., and Windows"*. Every claimed POSIX thread holds a 256 KiB alternate signal stack of the
runtime's (`runtime/parts/stack.c:994-1009`, `size_t size = 256 * 1024` at 1001, `sigaltstack` at 1009),
which the guard yields to ASan's own under `--sanitize` (`stack.c:96-100`, `567`).

**R3's two cases at this base, Darwin arm64** (`briefs/r2522/`, the base's `heroes-next`, the library
built as 248's brief built it: `clang -std=gnu11 -c -O2 -fomit-frame-pointer -I. lib/sigcb.c -o
lib-nofp/sigcb.o && ar rcs lib-nofp/libsigcb.a lib-nofp/sigcb.o`, then `heroes-next build flib.hero
--library lib-nofp -o coord-flib-nofp` and the same for `fliblocale.hero`, every step exit 0):

- `coord-flib-nofp`, three runs under `timeout -s KILL 20`: **3 of 3 exit 0**, stdout `0|answer ran for
  30|0|60|` (`flib-<n>.out`), stderr 0 bytes: the Heroes function ran inside the signal. The exit codes
  were printed by the coordinator's shell and kept in no file; the critic re-ran a copy, 3 of 3 exit 0
  (`249-reports/critic-briefs.md` item 6).
- `coord-fliblocale`, `python3 -I storm.py ./coord-fliblocale SIGUSR1 4 0.3` three times at 22:20:02:
  **3 of 3 armed, 3 of 3 `exit -9`**, after 12,289, 12,903 and 9,550 signals (pids 67979, 67990,
  68003); `ips.py ... coord-fliblocale`: `('coord-fliblocale', 'SIGKILL', 'lock') 3`, the three reports'
  pids the storms' three (read from each report's JSON); `SHOW=1`'s stack `_os_unfair_lock_recursive_abort <
  _os_unfair_lock_lock_slow < localeconv_l < h_fliblocale_answer < h_0cb_fliblocale_answer < sigcb_handler
  < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk < hero_int_to_str < h_fliblocale_round <
  h_fliblocale_main < main < start`.
- **Linux arm64 at this base** (`briefs/linux/census/census.sh`, one `docker run --rm` from 22:36:38 to
  22:40:21 and `docker exit 0` by the coordinator's shell, the log `census.log` itself holding no `date`; the same library and programs built there with the
  container's own stage 2, `sources 2ab7ca0a644692a1`): `coord-flib-nofp` **3 of 3 exit 0**, stdout `0|answer
  ran for 10|0|20|` (SIGUSR1 is 10 there), stderr 0 bytes; `coord-fliblocale` three storms, **3 of 3 armed,
  3 of 3 exit 0**, *"built 20000000 words"*, after 1,449,252, 1,407,350 and 1,381,368 signals: the Heroes
  function ran inside the signal every time, as 248's seats read at its base
  (`248-reports/compiler-engineer.md:192-204`, `248-reports/ffi-pragmatist.md:212-217`, carried).

**What a route must close, by platform** (248's brief, its critic's item 15, `248-briefs/00-shared.md:315-325`, carried): on Darwin the
SIGKILL is the C library's own re-entry, which a C program alone dies of too, so what R3 asks there is a
refusal **before** the Heroes function's first C call, with R1's words (whose remedy defect 2533 calls
false until it lands, § Question 3); on Linux the program runs on, and
what is broken is panel 246's R4 alone (Heroes code inside the signal).

### What each route changes, as the routes are described (the coordinator's reading, unrun as tables; the cells 248 measured cite it)

**Names first** (the critic's item 16): **B' is panel 248's route I** (`248-briefs/00-shared.md:449-452`,
*"I, R4's corrected form ... keeps a per-thread depth around the program's handler"*), primed so it is not
read as 248's route B, the asynchronous signals blocked on every claimed thread, which 248's
ffi-pragmatist vetoed (`248-*.md:330-333`); a seat reading 248's reports looks for *route I*. **D-tramp is
248's one-name I-dylib** (`248-reports/ffi-pragmatist.md:83`, *"an I-dylib interposing the one name
`__sigaction` sees every installer"*) **carrying route I's depth in a trampoline instead of a wrapper
handler**: what is new is that the kernel then keeps the program's handler, flags and mask, so I's table
goes; what is not new is the I-dylib's price, which 248 objected to (`248-*.md:318-323`).

Points (1) to (4):

| route | (1) the handler's stack | (2) the thread's mask | (3) the signal numbers | (4) the frames a handler finds |
|---|---|---|---|---|
| I-onstack (vetoed, 248's R1) | the runtime's alternate stack, `SA_ONSTACK` forced: bdwgc 8.2.8 faulted 5 of 5 in Linux arm64 (`248-reports/ffi-pragmatist.md:432-436`, carried) | unchanged | none taken | moved to the alternate stack |
| I-mask with SIGURG (not adopted, 248's R1) | unchanged (`:349-363`) | the marker added to every handler's mask; a program's own block of the marker dropped outside a handler (248's critic, items 3 and 6, `critic-reports.md:40-50`, `:66-78`) | SIGURG becomes the runtime's | unchanged |
| A on Linux | unchanged | as I-mask, the marker a real-time signal | that signal becomes the runtime's | unchanged |
| A on Darwin | unchanged | as I-mask with a classic signal, if a census shows one free; there is no real-time signal (§ Question 1, the facts) | that signal becomes the runtime's | unchanged |
| B' (248's route I) | unchanged | unchanged, unless the wrapper's own mask differs | none taken | **one more frame**, the wrapper's, between the delivery and the library's handler |
| C | unchanged | unchanged | none taken | **x86-64 only** (arm64 kept records at `-O2` already, § C below): a record in every frame of the program's own C and of the runtime; **never in the C library's own frames between a handler and the callback** (`exit`'s, `fork`'s, `qsort`'s), nor in a library built apart |
| D-tramp (Darwin) | unchanged | unchanged | none taken | the runtime's trampoline's frame where `_sigtramp`'s stood |

Points (5) to (8) (the critic's § D, on its brief's item 2, and its list's item 14):

| route | (5) the action a query answers, and the order of installs | (6) the context a handler reads and writes | (7) `errno` and `SA_RESTART` | (8) which thread a process-directed signal reaches |
|---|---|---|---|---|
| I-onstack, I-mask, A | the kernel's own (`SA_ONSTACK` or the marker shown in it); the definitions are reached by a constructor that installs before `main`, a Go archive's included, before the runtime has started: whether their marker or key work then is a question | the kernel's own | the kernel's flags as the program asked them; the definitions' own calls must keep `errno` | unchanged, except for the marker itself under I-mask and A (its block dropped outside a handler) |
| B' (248's route I) | **from the runtime's table, not the kernel's**: a raw `rt_sigaction` query answers the wrapper; the same constructor question | the wrapper must hand the handler the kernel's own `siginfo_t` and `ucontext_t` (Go edits the context to preempt and to panic, below) | the wrapper must keep `errno` and install with the program's own flags, or an interrupted `read` restarts where it returned `EINTR`, or the reverse | unchanged, unless the wrapper's mask differs |
| C | unchanged | unchanged | unchanged | unchanged |
| D-tramp (Darwin) | the kernel's own (the program's handler) | the trampoline must hand on the context and the sixth argument `_sigtramp` keeps (§ D below) | the trampoline must keep `errno` | unchanged |

### The libraries: what this sitting names, and what is here to build

**The list's first source is the world**: the libraries the tree's own programs bind, `git ls-files
'*.hero' | xargs grep -h -o -E '\blink "[^"]+"' | sort | uniq -c | sort -rn` and the same for
`pkgconfig "..."`. Among real names it prints `sqlite3` 37 times (`grep -l` over the same files: 34
files, 2 under `examples/`), `m` 33, `z` 8, `pthread` 5, `curl` 2, `ssl`, `crypto`, `ncurses`, `json-c`
and `SDL3` once each, `raylib` 15 times as `pkgconfig`, `libcrypto` 3, `sdl3` 2, `libiodbc` 2 and `libuv`
once (`tests/golden/run/fixedbugs-413-libuv-keeps-every-handles-address.hero`); the rest of each list
are cases' deliberately wrong names.

**The rule that picks the first entries** (the critic's item 13): **the coordinator's charge** (collectors
that read their own frame, bdwgc; Go's `c-archive` and its signal forwarding; ART; the users of SIGURG
and of the real-time signals; libuv; OpenSSL; SDL; sqlite) **and every library the tree binds that
imports an installer on at least one platform by the import list below: ncurses, and curl** (Debian's
build alone). **Beyond the first entries, from the world and unrun**: the critic counted 25 of the 357
Homebrew dylibs on this Mac and 29 of the 155 shared libraries in the Linux arm64 image importing an
installer (`249-reports/critic-briefs.md` § D, item 3, its files `critic/world/mac-installers.txt` and
`critic/linux/linux.log`, carried), among them a crash reporter (Abseil's
`libabsl_failure_signal_handler.2608.0.0.dylib`, on this Mac by `ls /opt/homebrew/lib`, importing
`_sigaction` and `_signal` by `nm -u`), GLib and libevent (event loops beside libuv), embedded
interpreters (libpython, libperl, Node's `libnode`) and the sanitizer runtimes other than ASan (UBSan,
HWASan). A census row a seat adds names where it came from.

**What is installed, and what can be**:

| library | this Mac (read-only: `readlink -f /opt/homebrew/opt/<name>`, the SDK) | Linux arm64 image (`dpkg-query -W`; `apt-cache policy` after `apt-get update`, 22:14) |
|---|---|---|
| bdwgc | not installed (`/opt/homebrew/opt/bdw-gc` absent); the 8.2.8 tarball in `briefs/inputs/p248-fp/`, which 248 built in Linux arm64 alone: whether it builds on this Mac is unrun | `libgc1 1:8.2.8-1` installed; `libgc-dev` candidate `1:8.2.8-1` |
| Go | `go1.27.3 darwin/arm64` (`go version`, run after the exports), Homebrew's `go` links to `Cellar/go/1.27.3` | not installed; `golang-go` candidate `2:1.24~2`, a metapackage whose source is `golang-1.24-src 1.24.4-1` (the critic's run, item 7) |
| libuv | `Cellar/libuv/1.53.0` | `libuv1t64 1.50.0-2` installed; `libuv1-dev` candidate `1.50.0-2` |
| OpenSSL | `openssl@3` links to `Cellar/openssl@3/3.5.9`; `openssl` links to `Cellar/openssl@4/4.1.0` | `libssl-dev 3.5.7-1~deb13u2` installed |
| SDL | `sdl3` links to `Cellar/sdl3/3.4.20`; `sdl2` links to `Cellar/sdl2-compat/2.32.74` | not installed; `libsdl3-dev` candidate `3.2.10+ds-1`, `libsdl2-dev` `2.32.4+dfsg-1` |
| sqlite | the SDK's `usr/lib/libsqlite3.tbd` and `usr/include/sqlite3.h`; `sqlite` links to `Cellar/sqlite/3.54.1` | `libsqlite3-dev 3.46.1-7+deb13u2` installed |
| ncurses | `Cellar/ncurses/6.6` | `libncursesw6 6.5+20250216-2` installed |
| curl | `Cellar/curl/8.23.0` | `libcurl4t64 8.14.1-2+deb13u5` installed |
| ART | not buildable here (an Android runtime); the historian's | the same |

**A census seed a command can count, and nothing more than that, with one name list on both platforms**
(the critic's item 10: the first version filtered the jump family on Linux alone, so libcrypto's rows read
as a platform difference that was the filter's). The list is `briefs/import-names.txt`, 42 names (`tr '|'
'\n' | wc -l`): the installers (`sigaction`, `signal`, `bsd_signal`, `sigset`, `sigvec`, `sysv_signal`,
`__sysv_signal`, `ssignal`, `__sigaction`, `sigignore`, `siginterrupt`), the mask calls and
`sigaltstack`, the waits (`sigwait`, `sigwaitinfo`, `sigtimedwait`, `sigsuspend`, `signalfd`) and the waits
that take a mask (`epoll_pwait`, `ppoll`, `pselect`), the senders (`kill`, `raise`, `pthread_kill`), the
timers (`setitimer`, `timer_create`, `alarm`), the jump family (`setjmp`, `_setjmp`, `__sigsetjmp`,
`sigsetjmp`, `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`), `dlopen`, `dlsym`, `syscall`,
`__libc_current_sigrtmin`, `__libc_current_sigrtmax` and `posix_spawnattr_setsigmask`; a Darwin symbol is
read with one leading underscore taken off. This Mac by `nm -u` (`briefs/imports-mac.txt`); Linux arm64 by
`nm -D --undefined-only` (`briefs/linux/census2/census2.log`, one `docker run --rm` from 23:16:14 to
23:16:22 by the coordinator's `date`, the log's own `date -u` 22:16:14 and 22:16:22 UTC, `docker exit 0`;
SDL and `libgc-dev` installed there by `apt-get install libsdl3-0 libsdl2-2.0-0 libgc-dev`, exit 0):

| library | this Mac | Linux arm64 |
|---|---|---|
| bdwgc | not installed | `libgc.so.1` (1:8.2.8-1): `__sigsetjmp _setjmp dlopen pthread_kill pthread_sigmask sigaction signal sigsuspend` |
| libuv | `libuv.1.dylib` (1.53.0): `dlopen dlsym kill posix_spawnattr_setsigmask pthread_sigmask sigaction signal sigprocmask` | `libuv.so.1` (1.50.0-2): `dlopen dlsym epoll_pwait kill pthread_sigmask sigaction signal sigprocmask syscall` |
| libcrypto | `libcrypto.3.dylib` (3.5.9) and `libcrypto.4.dylib` (4.1.0), each: `_longjmp _setjmp dlopen dlsym sigaction signal` | `libcrypto.so.3` (3.5.7): `_longjmp _setjmp dlopen dlsym sigaction signal syscall` |
| libssl | `libssl.3.dylib`: none | `libssl.so.3`: none |
| SDL3 | `libSDL3.0.dylib` (3.4.20): `dlopen dlsym kill pthread_sigmask sigaction` | `libSDL3.so.0` (3.2.10): `__libc_current_sigrtmax __libc_current_sigrtmin dlopen dlsym kill pthread_sigmask raise sigaction signal sigprocmask sigtimedwait syscall` |
| SDL2 | `libSDL2-2.0.0.dylib` (sdl2-compat 2.32.74): `dlopen dlsym`, **and it loads SDL3 by `dlopen`** (below) | `libSDL2-2.0.so.0` (2.32.4): the SDL3 list less `kill` |
| sqlite | Homebrew's `libsqlite3.dylib` (3.54.1): `dlopen dlsym` | `libsqlite3.so.0` (3.46.1): `dlopen dlsym` |
| ncurses | `libncursesw.6.dylib` (6.6): `kill sigaction sigprocmask` | `libncursesw.so.6` (6.5): `dlopen dlsym kill sigaction sigprocmask` |
| curl | `libcurl.4.dylib` (8.23.0): none | `libcurl.so.4` (8.14.1): `sigaction` |
| raylib | `libraylib.dylib` (6.2): `dlopen dlsym` | not read |
| json-c | `libjson-c.dylib` (0.19): none | not read |
| a Go archive | `briefs/go/src/libgoanswer.a` (Go 1.27.3, the coordinator's): `kill pthread_kill pthread_sigmask` (twice) `raise sigaction sigaltstack` (the critic's item 9: built and not put through the seed until this repair) | not built |

What the seed shows beyond its rows:

- **A library reached by `dlopen`** (the critic's item 11): sdl2-compat names no SDL3 at link time (`otool -L`
  lists only the system's frameworks and libraries), imports `_dlopen`, `_dlsym` and `_dlclose`, and
  holds `@loader_path/libSDL3.dylib`, `@executable_path/libSDL3.dylib` and three `SDL3.framework` paths
  (`strings`). So a program that binds SDL2 on this Mac gets SDL3's installers through `dlopen`, which no
  reading of SDL2's imports shows.
- **A raw system call is possible and unknowable from imports** (the critic's item 12): Linux libuv,
  libcrypto and both SDLs import `syscall`.
- **Debian's libcrypto and Darwin's both import `_setjmp` and `_longjmp`**, the pair that *"save and
  restore only the register set and the stack"*, not the signal mask (`man 3 setjmp`, its lines 57-58, on
  this Mac): the shape of the depth question of B' and D-tramp.
- **Both SDLs on Linux ask glibc for its real-time range and wait for a signal**, by their imports; which
  signal and why is the census's to run or read, and it bears on candidate A.
- **Debian's bdwgc takes SIGPWR and SIGXCPU, run** (the critic's item 8, re-run by the coordinator in the
  same container: `briefs/linux/census2/gcsig.c`, `GC_INIT()` with `GC_THREADS`, `clang -O0 ... -lgc
  -lpthread`, exit 0): *"GC_get_suspend_signal 30 GC_get_thr_restart_signal 24"*, *"sig 30 flags
  0x10000004 restart 1 onstack 0 siginfo 1"*, *"sig 24 flags 0x10000000 restart 1 onstack 0 siginfo 0"*.
  So Debian builds `libgc1` without `GC_USESIGRT_SIGNALS`, both handlers with `SA_RESTART` and neither with
  `SA_ONSTACK`.

**An import is not a behaviour**: which signal each installs, on which thread, with which flags and
whether it calls back, is the census's to run or read.

**Facts read today for the first entries, each with its source**:

- **Go 1.27.3** (the Mac's toolchain, `/opt/homebrew/Cellar/go/1.27.3/libexec/src`, read by `grep -n` and
  `sed`): `runtime/signal_unix.go` (1,474 lines) line 74 **`const sigPreempt = _SIGURG`**; lines 178-182,
  *"When built using c-archive or c-shared, only install signal handlers for synchronous signals and
  SIGPIPE and sigPreempt"*, so a Go archive a Heroes program links installs a SIGURG handler of its own;
  line 135, `fwdSig[i] = getsig(i)`, every handler found at start saved and forwarded to; `doSigPreempt` at
  342, which **edits the interrupted context**, `ctxt.pushCall(abi.FuncPCABI0(asyncPreempt), newpc)` at
  348, and `preparePanic` (`signal_arm64.go:62`, `signal_amd64.go:49`), which turns a fault into a panic the
  same way; line 1124, `throw("non-Go code set up signal handler without SA_ONSTACK flag")` in
  `sigNotOnStack` (1117-1125, *"This can only happen if non-Go code called sigaction without setting the
  SS_ONSTACK flag"*). **Where Go installs**: on Linux through libc's `sigaction`
  (`runtime/cgo/gcc_sigaction.c:63`), unless without cgo or in a forked child, where
  `cgo_sigaction.go:36-37` takes `sysSigaction`, a raw system call; on Darwin through libSystem's
  (`sys_darwin.go:326`, `func sigaction`, and `:607`, `//go:cgo_import_dynamic libc_sigaction sigaction
  "/usr/lib/libSystem.B.dylib"`). So the definitions of A and B' see a Go archive's installs, made from
  its constructor before `main`. `os/signal/doc.go` (234 lines), 129-133: *"If the non-Go code installs any
  signal handlers, it must use the SA_ONSTACK flag with sigaction. Failing to do so is likely to cause the
  program to crash if the signal is received."*; 195-200: *"Go code built with -buildmode=c-archive or
  -buildmode=c-shared will not install any other signal handlers by default. If there is an existing
  signal handler, the Go runtime will turn on the SA_ONSTACK flag and otherwise keep the signal
  handler."* `runtime/sigtab_linux_generic.go` lines 42-44 give signals 32, 33 and 34 `_SigSetStack +
  _SigUnblock`, commented *"SIGCANCEL"*, *"SIGSETXID"* and *"musl SIGSYNCCALL; see issue 39343"*, and 35 to
  64 `_SigNotify`. **At Go 1.24.4, the Linux image's** (the critic's run, item 7, `golang-1.24-src` under
  `/usr/share/go-1.24/src`, carried): the same clauses at other lines, `sigPreempt` at 73, the c-archive
  clause at 177-179, the throw at 1116, `doc.go`'s two sentences at 128-129 and 196, the sigtab at 42-44,
  and `cgo_sigaction.go:34` the same test. A seat citing a line names the version.
- **bdwgc 8.2.8** (`tar -xzOf briefs/inputs/p248-fp/gc-8.2.8.tar.gz`): `include/private/gc_priv.h` line
  3051 `# define SIG_SUSPEND SIGPWR` on Linux, and **3064-3067 `_SIGRTMIN + 6` / `SIGRTMIN + 6`** in
  another branch; `pthread_stop_world.c` 169-183, `SIG_THR_RESTART` **`SIGRTMIN + 5` under
  `GC_USESIGRT_SIGNALS`** (172-175) and `SIGXCPU` in the default branch (182); its suspend handler stores
  `GC_approx_sp()` (323), and installs with `SA_RESTART` or 0 (1408, 1410); the file compiles only
  `!defined(GC_DARWIN_THREADS)` (20-22), and `darwin_stop_world.c` suspends by `thread_suspend` (536, 640)
  and reads registers by `thread_get_state` (192, 208). So on Darwin its stop-the-world takes no signal,
  by that reading; whether another of its paths does (an incremental mode's fault handler) is unread.
  Debian's build, run: SIGPWR and SIGXCPU (above).
- **glibc 2.41, Linux arm64** (`briefs/linux/rt.c`, run in the image at 22:14): *"SIGRTMIN 34 SIGRTMAX 64
  NSIG 65 __libc_current_sigrtmin 34"*, and *"SIGURG 23 SIGPWR 30 SIGXCPU 24 SIGUSR1 10 SIGUSR2 12 SIGSYS
  31 SIGSTKFLT 16"*.
- **Darwin 27.2's SDK** (`$(xcrun --show-sdk-path)/usr/include/sys/signal.h`): `__DARWIN_NSIG 32` (line
  76, *"counting 0; could be 33 (mask is 1-32)"*), and the 31 numbered signals (`grep -E '^#define +SIG[A-Z0-9]+
  +[0-9]+'`): 1 SIGHUP, 2 SIGINT, 3 SIGQUIT, 4 SIGILL, 5 SIGTRAP, 6 SIGABRT, 7 SIGEMT (and SIGPOLL), 8
  SIGFPE, 9 SIGKILL, 10 SIGBUS, 11 SIGSEGV, 12 SIGSYS, 13 SIGPIPE, 14 SIGALRM, 15 SIGTERM, 16 SIGURG, 17
  SIGSTOP, 18 SIGTSTP, 19 SIGCONT, 20 SIGCHLD, 21 SIGTTIN, 22 SIGTTOU, 23 SIGIO, 24 SIGXCPU, 25 SIGXFSZ, 26
  SIGVTALRM, 27 SIGPROF, 28 SIGWINCH, 29 SIGINFO, 30 SIGUSR1, 31 SIGUSR2. **No real-time signal**: `grep -n
  -E 'SIGRTMIN|SIGRTMAX'` over its `signal.h` and `sys/signal.h` exit 1.
- **The runtime's own use of signals at this base**: `grep -o -h -E '\bSIG[A-Z0-9]+\b' runtime/parts/*.c
  runtime/platform/*.c runtime/*.h`, the dispositions left out, counts SIGURG 9 times (sigguard.c's probe
  and `sigdoor.c:35`'s comment, *"SIGURG is borrowed for an instant by `parts/sigguard.c`"*), SIGUSR1 once
  (a comment, `sigdoor.c:21`), no SIGUSR2, no SIGPWR, no real-time signal. Its handler installs: 27 lines
  (`grep -n -E '(^|[^_a-z])(sigaction|signal)\(' runtime/parts/*.c runtime/platform/*.c`, comment lines
  dropped), in `os.c` (260, 272, 320, 321, 440, 471, 472), `panic.c` (54), `run.c` (1068, 1088, 1094, 1109,
  1507, 1509, 1556, 1679), `sigdoor.c` (104), `sigguard.c` (139, 183, 185), `stack.c` (686, 1054, 1055) and
  `hero_net.c` (1269, 1280, 1295, 1355); its mask calls **18 lines** (`grep -h -E
  '(^|[^_a-z])(pthread_sigmask|sigprocmask)\(' ... | grep -v -E '^\s*(\*|/\*)' | wc -l`), 16 at 248's base by
  248's brief; the two new ones are R2's poller thread, born with every signal blocked
  (`hero_net.c:1115-1122`, `sigfillset`, then `pthread_sigmask(SIG_SETMASK, &all, &was)` around
  `pthread_create`). **The stack guard chains a fault not its own** to the handler it found
  (`stack.c:670-685`, `hero_stack_pass_on`, *"A fault that is not ours goes to whoever was there before
  us"*), so a Go archive's fault handler is reached through the runtime's guard at the base already. A
  route that passes the runtime's own installs and mask calls through definitions meets all of these.

### The question, as the seats split it

- **The ffi-pragmatist builds and runs**: each first entry above that this Mac or the Linux arm64 image can
  build or install, under the base and under each candidate of question 2 (and, for comparison, I-onstack
  and I-mask as 248 built them), on the eight points, with a probe of each library's own signal use
  first (which signals, which thread, which flags, a call back or not). bdwgc in Linux arm64 (248's
  `gcreal.c`, and Debian's `libgc1`) and on this Mac (built from the tarball in a folder of its own;
  Darwin's stop-the-world is Mach's by the reading above, unrun); a Go `c-archive` on this Mac (Go 1.27.3,
  the environment above, before every `go` command) and in Linux arm64 (`golang-go` 1.24 by `apt-get` in the
  container), its preemption under load, its panics and its `os/signal.Notify` for an asynchronous signal;
  libuv's `uv_signal_t`; OpenSSL's start-up; SDL's start-up handlers and, on this Mac, SDL2 reaching SDL3
  through `dlopen`; ncurses' and curl's; sqlite as the control with callbacks no signal reaches.
- **The historian reads**: each library's documented signal behaviour (its pages or source at a named
  version, dated), the precedents for each candidate, and ART, which cannot be built here.
- **The compiler-engineer reads and runs the runtime's half**: the runtime's own 27 installs and 18 mask
  calls under each candidate, the probe's SIGURG, the stack guard's alternate stack and its chain, and the
  D cases.
- **Every seat**: who owns SIGURG at this base (the probe borrows it for an instant, `sigguard.c:175-195`;
  Go's archive installs a handler for it), and what a Heroes program that binds such a library is told
  today.

## Question 2: the candidates, run against the census's first entries

**248 named three and built none** (`248-*.md:213-223`); question 2 asks each to be built far enough to
run R3's two cases and the census's first entries, on Darwin arm64 and Linux arm64, and says which it
closes on each platform: **on Darwin a refusal with R1's words before the callback's first C call** (words
defect 2533 calls false until it lands, § Question 3), **on Linux no Heroes code inside the signal**.
The routes 248 refused stay refused unless a seat names the measurement that changes the refusal:
I-onstack (the ffi-pragmatist's veto, bdwgc), I-mask with SIGURG (its own withdrawal condition met), B and
T (vetoed), U, W, R and I-dylib (objected to), K (R3's own words) (`248-*.md:318-323`, carried). **So do
panel 247's** (the critic's item 18): A by name and E (vetoed), A's declaration form (objected), H and I of
247's lettering (vetoed), F and S (objected) (`docs/panel/247-*.md:293-297`), so a seat does not re-derive
E (the Heroes function run later on a thread of the runtime's) or S (at a safe point of the program's own
thread) as unlisted.

### A, a marker signal the runtime owns and no library uses

I-mask's mechanism, built by 248's ffi-pragmatist (`248-reports/ffi-pragmatist.md:349-363`, carried):
the runtime's definitions of the installers add a marker signal to the mask of every handler installed
with a function, the mask calls let the marker go from blocked to unblocked and never back, and the thunk
asks whether the marker is blocked on its thread. Its cost was 1,346 instructions a query on Darwin
(`:381-385`, carried). **Its residues, measured with SIGURG as the marker by 248's critic, carried**: a
correct program that blocks the marker and waits for it in `sigtimedwait` loses it (item 3,
`critic-reports.md:40-50`); a handler that unblocks before calling back passes the route (item 5,
`:62-65`); an inherited block of the marker is cleared by a side effect of the probe's restore (item 6,
`:66-78`); on Darwin every program that hands C a function would need a dylib beside it, `heroes build`
giving a program no `LC_RPATH` and dyld refusing, under the hardened runtime, a dylib not signed by the
program's Team ID (item 8, `:89-98`); and the Linux arm needs the `longjmp` family defined
(`ffi-pragmatist.md:399-407`). Candidate A keeps the mechanism and changes the marker:

- **On Linux, a real-time signal**: glibc 2.41's range is 34 to 64 (above). Which of them a census entry
  takes is the question: bdwgc takes `SIGRTMIN + 5` and `+ 6` where it is built `GC_USESIGRT_SIGNALS`
  (Debian's is not, run above); both Linux SDLs import `__libc_current_sigrtmin` and `sigtimedwait`
  (above); musl reserves 34 by Go's comment, unrun here: the one musl image this Mac holds is another
  project's, `postgres:16.15-alpine` (`docker images` unfiltered; `arm64`, Alpine 3.24.3,
  `/lib/ld-musl-aarch64.so.1`, one `docker run --rm` between 22:41:12 and 22:43), with no `cc`, `clang` or
  `gcc` on its `PATH` (`command -v` printed none), so a seat that wants musl's range from it asks the
  coordinator first, and no image is pulled; Go's runtime treats 35 to 64 as `_SigNotify`; POSIX timers
  and AIO name a signal of the program's choosing (a question). **Whether the runtime would choose its
  marker at run time** (glibc's `SIGRTMIN` is a function call, `signal.h:387`, above) and whether a library
  can see that a number is taken are the seats'.
- **On Darwin, none exists** (above). What then is the question 248 left: a classic signal no census
  entry uses (which, asked of every entry's imports and sources, never from memory), a mark that is not a
  signal (§ D below, the trampoline, the sweep, the C library's own mark), or the two-level residue told
  (question 3). Whatever the marker, 248 measured that definitions in the executable miss a two-level
  dylib's installer (`248-reports/ffi-pragmatist.md:130-136`, `:50-64`, carried) and that only a dylib
  with an `__interpose` section reached it.
- Every residue item 3, 5 and 6 measured is re-run with the new marker; item 8's price is the Darwin
  arm's whatever the marker; and the definitions are reached by a Go archive's constructor before `main`
  (point (5)).

### B', a wrapper handler with a per-thread depth (248's route I)

Panel 247's R4 (`docs/panel/247-*.md:197-211`), the table both of 248's building seats set aside: the
compiler-engineer's objection (`248-reports/compiler-engineer.md:493-500`, carried): a table of the
program's handlers per signal to answer `sigaction(sig, NULL, &old)`, `sigignore` and `siginterrupt`
defined so the table does not go stale (248's brief measured a library's `sigignore` changing the
disposition with 0 calls reaching an executable's `sigaction`, `248-briefs/00-shared.md:413-416`,
carried), the wrapper's own `SA_RESETHAND` and `SA_NODEFER` behaviour re-made, and **a depth a
`siglongjmp` out of the handler leaves raised for ever**, which refuses every later callback of a correct
program (247's R4 condition (a)). **Posed, unrun**: whether a depth entry that records the wrapper's
stack pointer at entry, compared with the thunk's own frame, tells a handler still running from one left
by `siglongjmp`, `longjmp` or `_longjmp` (which restores no mask, above), on both POSIX legs and on the
alternate stack; what the wrapper's frame does to point (4); and points (5) to (7): the wrapper hands the
handler the kernel's own context (Go's preemption and panics edit it), keeps `errno`, and installs with
the program's own flags. On Darwin the wrapper needs whatever reaches a two-level dylib (above). **Linux's
`sa_restorer`** (the critic's item 19): the kernel's action on Linux holds `sa_handler`, `sa_flags`,
`sa_restorer` (under `SA_RESTORER`) and `sa_mask` (`/usr/include/asm-generic/signal.h:74-81` in the arm64
image, which its `asm/signal.h:26` includes); `sa_restorer` is where a handler returns, a field on the way
out that glibc fills with its own, and nothing on the way in. Whether a restorer of the runtime's could
serve B' (it cannot raise a depth before the handler; it sees the return) is the seats'.

### C, frame records kept in every unit the compiler builds

**The premise, measured today where 2522's comment called it an inference**:

- **The CI's reds**: run 43571761187, job 132377462166 (`gh api repos/heroes-lang/heroes/actions/jobs/132377462166
  -q '.name, .conclusion, .head_sha'`: *Linux x86-64*, `failure`, `df7612ad3...`, 248's base), its log
  (`gh api --allow-escape-sequences .../jobs/132377462166/logs`, 475,097 bytes, `briefs/ci/`): line 4112
  `run: 803 passed, 3 failed`; lines 4315-4350, three `FAIL run/fixedbugs-2494-... at -O2`:
  `-a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` (it printed *"the atexit function
  ran: 42"*, exit 0), `-a-heroes-function-a-c-handler-calls-is-refused` (*"the answer ran for signal 10"*,
  *"raised: 0"*, *"seen: 20"*, exit 0) and `-a-heroes-function-a-c-handler-reaches-through-fork-is-refused`
  (*"prepare ran: 42"*, *"the parent goes on"*, *"raised: 0"*, *"after the signal"*, exit 0); `grep -c 'at
  -O2'` over the log: 3; none at `-O0` (the critic's `grep -c -E 'FAIL run/.* at -O0'`: 0). **The run at
  this base**, 43640139573 (`gh run list --repo heroes-lang/heroes --branch main --limit 6`), and its Linux
  x86-64 job `132585763737` read `in_progress` at 22:39:11 and 22:41:12 (`gh api
  repos/heroes-lang/heroes/actions/jobs/132585763737`), and at 23:10 at step 20, *"The net, in Heroes"* (the
  critic's item 5); **its read is the compiler-engineer's** (`gh run view 43640139573 --repo
  heroes-lang/heroes`, the `gh api` above, and the log command above with that job's number).
- **The mechanism, read from clang's own output on all three cases' handlers**: `called_handler` of
  `tests/golden/run/fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused.h` (30 lines; its
  comment says the handler *"keeps a frame record on every target, as one compiled with frame pointers
  does"*, and its body is `(void)__builtin_frame_address(0);` then the call), compiled `clang -std=gnu11
  -Wall -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls
  <level> -c` and read with `objdump -d` (`briefs/fp/run.sh`, both images, its objects 22:14:17 and
  22:14:19, `stat`): **x86-64, Debian clang 22.1.8, `-O2`: `push %rax`, `movslq`, `call *`, `mov`, `pop %rax`,
  `ret`, no frame record**; `-O0`: `push %rbp` / `mov %rsp,%rbp`; **`-O2 -fno-omit-frame-pointer`: `push
  %rbp` / `mov %rsp,%rbp`**; aarch64, Debian clang 22.1.8: `stp x29, x30, [sp, #-16]!` / `mov x29, sp` at
  `-O2` with and without the word. **The other two handlers have the same shape** (the critic's item 3,
  `critic/fp/run.sh` in `critic-249-1` and `critic-249-2`, carried): x86-64 `-O2` `leaving_handler` `push
  %rax; xor %edi,%edi; call` and `forking_handler` `push %rax; call ...; pop %rax; ret`, both opening `push
  %rbp; mov %rsp,%rbp` with the word and at `-O0`; arm64 `stp x29, x30` / `mov x29, sp` at every level.
  **The CI's clang 21.1.8 is unread** (apt.llvm.org's `clang-21` inside a `--rm` container would read it;
  unrun, a question).
- **The walk through the C library's own frames** (the critic's § C and item 4, carried: `critic/glibc/walk.c`,
  R1's walk modelled in C as `sigguard.c:333-345` does it, built with the `run` form's words at three levels,
  in `critic-249-8`, emulated x86-64, and `critic-249-9`, Linux arm64, both on libc6 2.41-12+deb13u3 and
  Debian clang 22.1.8):

  | image | a handler that calls back: `-O0` | `-O2` | `-O2` with the word | through `exit`'s `atexit` list, any level | through `fork`'s `pthread_atfork` list, any level |
  |---|---|---|---|---|---|
  | x86-64, emulated | found, step 0 | **ended at step 0** | found, step 0 | **ended at step 0** at all three | **ended at step 0** at all three |
  | aarch64 | found, step 0 | found, step 0 | found, step 0 | found, step 2 at all three | found, step 2 at all three |

  So, by that model and unrun with the Heroes cases: **through Debian's x86-64 glibc the walk ends at its
  first step from an `atexit` or a `fork` callback whatever the program's own flags**, the frame above the
  callee being `__run_exit_handlers`' or the fork handlers', whose saved `%rbp` is no frame record. Three
  things follow, each a question to run:
  - **the emulated image is expected to read the `atexit` and `fork` cases red at `-O0` too**, where the
    CI's log reads them green at `-O0`; so it is the instrument for the `calls` case and, for the other
    two, a measurement of Debian's libc rather than of the CI's leg (the compiler-engineer runs those two
    at `-O0` first and names the level that fails);
  - **candidate C reaches the program's own C and the runtime, never the C library's frames between a
    handler and the callback** (`exit`, `fork`, and by the same reasoning `qsort` and any libc function
    that calls back): on a libc that keeps no frame records, C closes the `calls` case and leaves the
    other two; that the CI's Ubuntu libc keeps the chain, since it passes them at `-O0`, is an inference
    (whose policy page is the historian's to read), unrun here: no Ubuntu image of this project is on this
    Mac and none is pulled;
  - **the tree's own `atexit` and `fork` cases would then run the Heroes function inside the signal on a
    Debian x86-64 machine at every level**, 2522's class reached through the C library itself; no CI leg
    runs Debian x86-64. It bears on question 3's class.
- **ASan under emulation, a question** (the critic's item 24): the `run` form judges each case at `-O0`
  (`suite_run.hero:221`), at `-O2` (228) and under `--sanitize` and `--sanitize -O0` (the loop at 238);
  whether ASan runs in the emulated image at all is unread, so a red there is read by its level before it
  is read as the frame records'.
- **Where the word would go**: `selfhost/cli/flags.hero` (584 lines): `flags()` at 133-154 returns 19
  words and none names a frame pointer (`sed -n 133,154p`; `grep -rn -E
  'frame-pointer|omit-frame|unwind-tables' selfhost/ runtime/ tests/harness/` prints only two comments,
  `stack.c:49` and `sigguard.c:51`); `level_words` (214) builds every compile's and the link's words from
  that list, and its comment (204-213) says *"A unit's compile (`compiling.unit_words`), the runtime's
  (`toolchain.runtime_object`) and the link (`link.link_line`) read this one list"*. `frame_words()`
  (`:198-202`) is the precedent of a word asked of the machine (`-fstack-clash-protection`, defect 2253).
  A `--library` folder's archive is built by its author, not by this list: R3's library is that residue,
  and the C library's own frames are the other.

### D, routes nobody listed, and where the briefs searched

Searched with `grep -l -E -i '<term>'` over panels 246 to 248's records, briefs and reports
(`docs/panel/24[678]-*`, `24[678]-briefs/*`, `24[678]-reports/*`): `sa_tramp` 0 files,
`SYSCALL_USER_DISPATCH` 0, `seccomp` 0, `nm -u` 0, *imports an installer* 0, `GC_USESIGRT` 0,
`backtrace\(` 0; and, already present, `SIGRTMIN` 3 files, `libsigchain` 4, `musl` 4, `SIGPWR` 4,
`fno-omit-frame-pointer` 1. Each below is **posed, unbuilt, unrun**, and a seat that finds it a
restatement of a route above says so:

- **D-tramp, Darwin: a trampoline of the runtime's** (248's one-name I-dylib with I's depth, above). The
  kernel's action on Darwin carries the trampoline the C library hands it: `struct __sigaction` holds
  `sa_tramp`, `void (*sa_tramp)(void *, int, int, siginfo_t *, void *)` (`sys/signal.h:277-282` in the SDK),
  and every installer libSystem has ends in libsystem_kernel's `__sigaction` across images (248's
  ffi-pragmatist, `248-reports/ffi-pragmatist.md:77-84`, carried). An interposer of that one name could
  hand the kernel a trampoline of the runtime's that keeps the per-thread depth around the program's own
  handler, leaving the handler, its stack, its mask and its flags as the program set them, and
  `sigaction(sig, NULL, &old)` answering the program's handler. **What `_sigtramp` keeps, which a
  trampoline of the runtime's must keep too** (the critic's item 17): the SDK's stubs export `___sigaction`
  and `___sigreturn` from `libsystem_kernel.tbd` and `__sigtramp` from `libsystem_platform.tbd`, no stub of
  those three names `__in_sigtramp`, and no header under `usr/include` declares `__sigaction`, `__sigreturn`
  or `__in_sigtramp` (`tr ... | grep -x` over the three `.tbd` files and `grep -rn` over `usr/include`, run by
  the coordinator); and `_sigtramp` disassembled (the critic's `lldb --batch` on `/bin/ls`, `HOME` and
  `TMPDIR` in its folder, carried) keeps **a sixth argument** (`mov x19, x5`) and hands it, with the context
  and 30, to the call after the handler, and stores the signal number into libsystem_platform's
  **`__in_sigtramp`** before the handler and zero after. So the runtime's trampoline must reproduce a
  return whose sixth argument no header declares, and replaces a process-wide mark libSystem's own
  trampoline keeps. Its other costs: the I-dylib's price (a dylib beside the program, an undocumented
  `__interpose`, the hardened runtime's Team ID); what it changes in point (4) (its frame where
  `_sigtramp`'s stood, and the exact test's learned address); and the depth question of B'.
- **A sweep instead of an interposer, Darwin** (the critic's item 18): the runtime, in the executable,
  re-installs every signal whose handler is a function through libsystem_kernel's `__sigaction` with its
  own `sa_tramp`, at chosen points (the first crossing, each `hero_callback_of`), with no dylib and no
  definition. Its holes: a handler installed after the last sweep, and one re-installed through libc,
  which passes `_sigtramp` again. On Linux the analogue re-installs a wrapper and needs B''s table.
- **The C library's own mark, Darwin**: `__in_sigtramp` (above) is process-wide, private (in no stub),
  cleared by a nested handler's return and left set by a `siglongjmp` out, by the critic's reading; whether
  it could serve as a mark at all is the question.
- **D-imports, at build: a reading of which bound libraries import an installer.** The census seed above
  is that reading done by hand. A program that hands C a function and binds a library whose objects import
  `sigaction`, `signal` or another installer would be refused or told at build, before any run; panel
  239's member already asks the link who supplies each name the runtime calls (`selfhost/cli/supplied.hero`,
  357 lines, `wc -l`). It is a **new diagnostic** if it lands, so the escape binds it; it is question 3's
  material as much as question 2's. **Its holes, measured today** (the critic's § D, item 8): a library
  loaded by `dlopen` (sdl2-compat loading SDL3, above), an installer reached through `dlsym` (every census
  library on this Mac but ncurses, curl, libssl, json-c and the Go archive imports `dlsym`, the seed
  above), a raw system
  call (Linux libuv, libcrypto and both SDLs import `syscall`), and the program's own units, which are not
  a bound library. **Its nearest refused relative**: panel 247's route A refused at build by the callee's
  name (*"`signal`, `sigset`, `bsd_signal`"*, `docs/panel/247-*.md:36-37`), and 247 refused *"A alone and E
  by name (vetoed by the compiler-engineer and the ffi-pragmatist: their selection is a name list, and
  `wraplocale` dies 5 of 5 in silence through its hole)"* (`:293-294`). D-imports selects by a name list too,
  of a library's imports rather than a call's callee: a seat says whether 247's veto reaches it, and the
  sdl2-compat program is the one to build.
- **A refusal at a library's first install** (under A's or B''s definitions): refuse when a function is
  installed while the program has handed C a function. Its cost is every program that binds a library
  installing at start-up (libuv's, OpenSSL's, SDL's, ncurses' by the seed) and hands C a function.
- **The raw system call, a residue of every interposition**: Linux's `prctl(PR_SET_SYSCALL_USER_DISPATCH)`
  exists in the image's headers (`/usr/include/linux/prctl.h:253`, `#define PR_SET_SYSCALL_USER_DISPATCH
  59`), and **seccomp's filter returning a trap on `rt_sigaction`** is the more general facility
  (searched, `seccomp` 0 files); whether either could see a library's raw `rt_sigaction`, what survives an
  `execve` into a child, at what cost and on which kernels, is a question, not a route.
- **The kernel's shadow stack, Linux**: x86-64's CET shadow stack and arm64's GCS hold every return address
  whatever the frame records; whether any target kernel and CPU enables it for a process whose every object
  is not marked, and whether the delivery's return is on it, is a question for the compiler-engineer
  (Linux's `/proc/self/status` names a thread's shadow-stack state where the kernel supports it; unread
  here).
- **Combinations by platform**: a sound answer may be a different route per platform (C on x86-64 for the
  tree's own C, A or B' on Linux for libraries, D-tramp, the sweep or a told residue on Darwin). A seat that
  proposes one says what each platform closes and leaves.

## Question 3: what closes 2522 if no candidate is sound everywhere

**What binds**: panel 247's R3, *"Never a known cost: it blocks M-core-packages' tag like any `blocking`
defect"* (`docs/panel/247-*.md:191-195`); route K, the residue kept, *"R3 forbids it in its own words;
only the author's change to R3 admits it"* (`248-briefs/00-shared.md:513-515`, carried); 2522's class,
`blocking` (`gh issue view 2522 --json labels`: `kind:defect`, `class:blocking`, `area:runtime`). **So
question 3 is which class of programs the runtime or the compiler refuses or tells, on which platform,
with which words, so that no program of 2522's shape runs on in silence or dies in silence**, and
whether that is a closing of 2522 under R3 or a change to R3, which is the author's, said in those
words. The class now holds a shape the briefs did not name at first: the tree's own `atexit` and `fork`
cases on a Debian x86-64 machine, reached through the C library's frames (§ C above).

**What a program is told today** when R1 sees it: `panic: <module.function> was called from inside a
signal handler: C called it while handling a signal, where neither this runtime's memory nor the C
library's is safe to use.` and the remedy `HERO_SIGNAL_REMEDY` (`sigguard.c:87-91`), which reads
*"... after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C and SIGTERM only make
`hero_net_stop_asked() -> i64` answer 1 ..."*. **Spec line 404 now reads *"Ctrl-C, SIGTERM and
`hero_net_ask_stop() -> ()` only make ..."*** (`sed -n 404p spec/heroes-spec.md`; `git log -1 --
spec/heroes-spec.md`: `cd693eec1`, batch 36, 248's q4m), so the remedy has been false since that commit.
**Defect 2533 tracks it** (https://github.com/heroes-lang/heroes/issues/2533; `gh issue view 2533 --repo
heroes-lang/heroes`: `OPEN`, filed 2026-11-14T21:45:55Z, `kind:defect`, `class:blocking`, `area:runtime`,
M-core-packages, *"the runtime's signal remedy still says Ctrl-C and SIGTERM alone make the stop answer,
which the asked stop has made false"*). Its reach (the critic's item 21, re-run): the macro has **three
users**, `sigguard.c:306`, `alloc.c:555` and `lend.c:194` (`grep -rn HERO_SIGNAL_REMEDY runtime/`, the
definition left out); **ten** golden files pin its words (`grep -rl 'hero_net_stop_asked() -> i64\` answer
1' tests/golden | wc -l`: 10); and panel 247's R9 binds the remedy to name each door *"with the header it
comes from, as R5's sentence does"* (`docs/panel/247-*.md:280-284`). **Its repair is batch 37's lane
b37-runtime's** (the coordinator's word; the lane's worktree stands at `.claude/worktrees/b37-runtime`, its
head `0d3f17ad9`, on defect 2531, by `git -C ... log --oneline -3`). So **every refusal "with R1's words"
the briefs ask for carries words 2533 calls false until it lands, and any new words question 3 proposes
move the same macro, the same ten files and the same spec sentence: 2533's repair and this sitting's words
land in an order the synthesis states.** Any new words question 3 proposes are the escape's.

**The shapes a seat answers, each with what it costs a correct program**: a refusal at build (D-imports,
and its holes); a refusal at a library's first install (§ D); a refusal at run time on the first callback
of a program whose bound library installed a handler (how would the runtime know); a word at build or at
start that names the residue without refusing (which R3 does not admit without the author); and per
platform, Darwin, Linux (the Debian x86-64 shape above included) and Windows, where 247's R10 and 248's
rows say nothing new is needed (above, carried).

## What the coordinator measured (each with its command, above)

The base's head, tree, status and diff against 248's base; its two stages and `sources 2ab7ca0a644692a1`
on this Mac (`briefs/base/`) and in Linux arm64 (`briefs/linux/base-arm64.log`: seed exit 0, stage 2 exit
0, `sources 2ab7ca0a644692a1`, *Debian clang version 22.1.8 (1~deb13u4)*, `docker exit 0`); the emulated
x86-64 image's toolchain and the base there (`briefs/linux/base-amd64.log`); the Windows box's state; the
Docker images; R3's two cases on this Mac (`briefs/r2522/`) and in Linux arm64 with the first import
reading (`briefs/linux/census/`); the kernel's `struct sigaction`, the arm64 image's glibc and its
`SIGRTMIN` macro (two `docker run --rm`, between the `date` readings 22:39:36 and 22:39:52, and between
22:41:12 and 22:43); every Docker image this Mac holds and the one musl image among them (by 22:43); the
CI's job metadata and log (`briefs/ci/`); the case handler's code at three levels on two images
(`briefs/fp/`); glibc's real-time range, the installed and installable libraries and `apt-get`
(`briefs/linux/probe.sh`, `rt.c`); the Darwin SDK's signals; Go's environment and a `c-archive` built inside
the root (`briefs/go/`); Go's and bdwgc's sources read; the tree's bound libraries; the runtime's signal
names, 27 installs and 18 mask calls; the searches for unlisted routes; the inputs' manifest
(`briefs/inputs-manifest.txt`). **On the critic's first pass, from about 23:15**: the one import list and
its two readings (`briefs/import-names.txt`, `briefs/imports-mac.txt`, `briefs/linux/census2/census2.log`),
Debian's bdwgc's signals (`census2/gcsig.c`), sdl2-compat's `otool -L`, `strings` and imports, Darwin's
`.tbd` stubs and headers for the trampoline's names, `man 3 setjmp`, Go 1.27.3's installers, context edits
and forwarding (`grep -n`), the stack guard's chain (`sed -n 668,686p stack.c`), defect 2533 (`gh issue
view`), the remedy's users and golden pins, panel 247's R9 and refused routes and 248's route I (`sed`),
the lane b37-runtime's head, and `storm.py:52`. **Line counts** (`wc -l` at the base):
`runtime/parts/sigguard.c` 364, `sigdoor.c` 137, `stack.c` 1213, `alloc.c` 1548, `thread.c` 144,
`runtime/parts/run.c` 2446, `runtime/platform/hero_net.c` 2202, `selfhost/emit/callback_thunk.hero` 343,
`selfhost/cli/flags.hero` 584, `.claude/rules/c-boundary.md` 398 (panel 239's member at its lines 186-221),
`spec/heroes-spec.md` 492 lines and 29,216 bytes (`wc -l -c`). **The critic's runs this brief carries**,
each named where it is used: the other two handlers' disassembly (item 3), the walk model (item 4 and its
§ C), the CI run at 23:10 (item 5), the flib exit codes (item 6), Go 1.24.4's lines (item 7), the world's
counts (item 13), `_sigtramp`'s disassembly (item 17).

## Found while writing, not this sitting's question

- **The runtime's remedy lags spec line 404**: defect 2533, `blocking`, filed (§ Question 3). Its repair
  and this sitting's words are ordered by the synthesis.

## Questions no one has run, posed as questions

- Whether the three 2494 cases read red in the emulated x86-64 image at `-O2`, the `atexit` and `fork`
  cases at `-O0` too (the critic's walk model), and green with `-fno-omit-frame-pointer` in `flags()`; and
  what the word costs a program and the compiler there.
- **Whether the tree's own `atexit` and `fork` cases run the Heroes function inside the signal on an x86-64
  whose C library keeps no frame records** (Debian's, by the model), at every level, and so what candidate
  C leaves besides libraries built apart: the C library's own frames.
- Whether CI run 43640139573's Linux x86-64 job reads the same three reds at this base; and what the CI's
  clang 21.1.8 emits for the three handlers.
- Whether ASan runs in the emulated image at all.
- Which signal each census library installs, on which thread, with which flags, and whether any calls
  back into the program from a handler; which block, unblock or wait for a signal outside a handler.
- Whether a real-time marker collides with any census entry on glibc (SDL's real-time imports first), and
  with musl's reservation.
- Which classic Darwin signal, if any, no census entry blocks, waits for or installs.
- **Whether each definitions route works when it is reached before the runtime starts**: a Go archive's
  constructor installs its handlers before `main`, through libc's `sigaction` on Linux and libSystem's on
  Darwin.
- **Whether Go's preemption and its panics survive each route and the runtime's own stack guard**, which
  chains a fault not its own to the handler it found (`stack.c:670-685`).
- What a program binding SDL2 on this Mac gets under each route and under D-imports: SDL3, through
  `dlopen`.
- Whether a depth entry holding the wrapper's stack pointer survives `siglongjmp`, `longjmp` and
  `_longjmp` on both POSIX legs.
- Whether a trampoline of the runtime's handed through `__sigaction`'s `sa_tramp` is accepted by Darwin's
  kernel and reached from every image, how it reproduces the sixth argument and `__in_sigtramp`, and what
  it costs.
- What a Go archive's SIGURG preemption does beside the runtime's SIGURG probe at the first callback.
- What any route does to ASan's own handlers under `--sanitize`, where ASan gives each thread its own
  alternate stack (`248-reports/compiler-engineer.md:155-157`, carried).
- How 2522's repair and defect 2533's are ordered (one macro, three users, ten golden files, one spec
  sentence).

Written by the coordinator from 22:08 to 22:44 CET on 2026-11-14 (`date`), and repaired on the
completeness critic's first pass (`249-reports/critic-briefs.md`, 25 items) from about 23:15 to 23:27
CET, the texts before it kept as `*.before-critic.md`; its item 20, the lane, is the coordinator's, put to
the author. No seat had started.
