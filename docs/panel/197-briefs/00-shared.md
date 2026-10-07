# Panel 197, the shared brief: what debug information clang is asked for

Written by the coordinator on 2026-10-07 from 12:12 by the clock, and
**repaired from 12:34 on the completeness critic's first pass**
(`docs/panel/197-reports/completeness-critic-pass1.md`, its 19 repairs, every
one applied; its fact numbers are cited as *critic N*). The frozen tree is
`lane-panel-197` at `dad2da47` (the trunk as pushed with batch 13, `34f95b71`,
plus one commit touching `.claude/rules/generated-c.md` only). **Lane:
soundness** (`.claude/skills/panel/SKILL.md` § Two lanes): `compiler-engineer`
and `ffi-pragmatist`, then the critic's second pass. The proposal moves no
surface syntax, no diagnostic and no spec token (`grep -n -i
'lldb\|debugger\|line-tables\|debug\|#line\|dwarf\|sanitiz\|backtrace'
spec/heroes-spec.md` prints nothing, critic 2). **A route that needs a new flag
or environment variable is not adopted by this lane**: it is a surface, and a
full sitting would be convened for it (critic § 3.10).

## The proposal

`selfhost/cli/flags.hero`'s `flags()` passes clang sixteen words, the second
`-g` (`function flags()` at line 91 to its `]` at 109; its own test pins
`got.len() == 16` and `got[1] == "-g"` at `:208-211`, critic 5). `flags()`
reaches every compile of a program unit, every header probe
(`compiling.hero:136-137`), the runtime object (`toolchain.hero:281`), the
toolchain probe (`:117`), the link line (`link.hero:134`) and `--emit-c`'s
rounds, and it is inside **the build cache key** (`toolchain.hero:120`), so any
change makes every build directory cold once (critic 4).

Two open improvements ask about it:

- **defect 322** (panel 190's critic, 2026-10-04): at `-O2`, the 400-return
  shape needed 1,695,369,184 bytes of clang's peak footprint under `-g` and
  970,589,672 under `-gline-tables-only` (−42.75%). **This is history**: it was
  read on the trunk's C at `703af779`, before defect 231 closed at `075b425d`;
  today's emission is the one-exit C, and the number is re-measured (critic 6).
  The shape: *a function of N strings, each followed by an `if` that returns
  it* (`docs/panel/190-briefs/00-shared.md:38-40`); its generated files are
  gone.
- **defect 219** (lane depth, 2026-10-03): clang's debug information dies on a
  type chain between 3,000 and 5,000 nested variants on this Mac, by 10,000 in
  the Linux container; *the same clang under `ulimit -s 65520` compiles it*
  (`selfhost/emit/typeorder.hero:122-123`), and **65,520 KB is this Mac's hard
  stack limit** (`ulimit -Hs`). Panel 184's R6 floor of N = 2,000 was measured
  on this Mac and Linux arm64, Windows owed, and **is not a spec sentence**
  (critic 7). Since `6c95f44a` (defect 170) the build says a clang death at
  exit 2 (`selfhost/cli/clang_died.hero`).

**The routes, a list to be widened, not chosen from:**

- **(A)** `-g` stays; 219 is answered by raising clang's stack. Darwin's hard
  limit is the measured value itself; Linux's is unlimited in the container;
  **`clang.exe` on the box reserves 10,000,000 bytes in its own PE header**
  (`llvm-readobj --file-headers`), and `link_flags()`'s `-Wl,/STACK:67108864`
  (`flags.hero:194`) sets the stack of what `heroes` links, never clang's. A
  stack number that differs by platform is what panel 107 refused
  (`flags.hero:150-160`). Critic 24, § 3.6.
- **(B)** `-gline-tables-only` for every build.
- **(C)** `-g` where the build is unoptimised, `-gline-tables-only` at `-O2`.
  The levels today: `heroes build` `-O0` (`verbs.hero:31`), `heroes run` `-O2`
  (`:56`), `heroes test` `-O0` (`:105`); `--sanitize` is orthogonal
  (`compile.hero:63-76`); the runtime object takes the build's level
  (`produce.hero:122`); the harness runs under `heroes run`, so at `-O2`, and
  the `run/` goldens at `-O0`, `-O2` and `--sanitize` (critic 9).
- **(D)** `-gline-tables-only` by default, full `-g` under a flag: a surface,
  not adoptable here (above).
- **(E)** Keep full `-g` and hand LLVM's DWARF writer the type chain from its
  bottom, as `typeorder.hero`'s `in_order` does for clang's type conversion.
  The critic's hand shape (a variant chain shaped like the seed's, 5,000 deep,
  with a file-scope variable of the top type, Apple clang 21, `-O0`): `-g0`
  exit 0; `-g -emit-llvm -S` exit 0; **`-g -c` exit 254, *Illegal
  instruction: 4***; `-gline-tables-only -c` exit 0; without the file-scope
  variable `-g -c` exit 0; with bottom-up file-scope pointer declarations
  before it, `-g -c` exit 0 at **5,000 and 10,000**. Object at 5,000: 2,990,032
  bytes under `-g`, 545,800 under `-gline-tables-only`. **Hand shape only**:
  the real emission and the other platforms are unrun (critic § 2).
- **(F)** Name the debug KIND: `-g` is `-debug-info-kind=standalone` on this
  Mac, `constructor` on Linux arm64 (Debian clang 22.1.8) and `constructor` +
  CodeView on the box (clang 23.1.1) (`clang -### -c -g`). On the hand shape
  `-fno-standalone-debug`, `-gdwarf-4` and `-fno-eliminate-unused-debug-types`
  did not move the 5,000 death; their memory effect at `-O2` is unrun.
- **(G)** A per-unit word the compiler chooses: `-g` by default,
  `-gline-tables-only` for a unit whose deepest by-value chain
  (`cli/deep_types.hero` computes it) or size passes a measured threshold.
- **(H)** Retry on death: a unit clang dies on (`clang_died.died_by`) compiled
  again with `-gline-tables-only`, and the build says so.
- **(I)** The header probes at `-g0` (they never link and are never debugged;
  `compiling.hero:128-131` asserts `-g` changes no predefined macro).
- **(J)** `-g0`, listed to be refused on evidence: no `.hero` breakpoint, no
  ASan file and line.
- **(K)** `-gno-column-info`: a column under `#line` is the C file's column
  printed against a `.hero` line. Unrun.
- **(L)** A debug word through the environment: the stopping rule binds it as
  a flag; not adoptable here.

Not routes, measured: `-g1` and `-gmlt` are aliases of `-gline-tables-only`;
`-gsplit-dwarf` is dropped silently by the Darwin driver. No clang flag that
limits type depth in debug info was found in `clang --help`; `-cc1` and
`-mllvm` options were not searched, so that stays a question.

## What it costs, read and not run

- design.md Part 2, `:630-632`: *No typed variable inspection in v1. Line-level
  debugging works ... But `p x` shows a mangled C temporary, not a Heroes
  value.* What `p x` shows under each route is **unrun**: the compiler-engineer
  measures it.
- design.md `:653-654` (§3.1, *Debug info*, the line mapping), `:3682` (Part 8,
  *The mangled names are what `lldb` shows for variables*) and `:750` (§3.2, QBE
  rejected partly for *no DWARF*) move with any route that removes variable
  DWARF (critic 3).
- **Scheduled work rests on full `-g`** (critic § 3.2): **M-typed-inspection**
  (`docs/ROADMAP.md`, row 69, scheduled by author instruction 2026-09-06)
  measured that *half of it already works* under today's `-g`
  (`docs/roadmap/milestones/M-typed-inspection.md:15-22`), and its falsifier is
  *`-g` deleted, the suite must go red* (`:55`); **M-vscode-extension** (row 73)
  composes `lldb-dap` over the DWARF (`M-vscode-extension.md:16-29`).
- **Nothing in the net runs lldb** (critic § 3.3): `lldb_breaks_on_a_hero_line`
  is archived; the `lines` suite reads `#line` claims with no debugger. A route
  that breaks breakpoints passes every gate; the seats' lldb runs are the only
  instrument, and the landing owes one.
- **On Darwin the DWARF lives in the kept objects** under `build/tu-*/`, the
  binary carrying a debug map, so any debug word outside `flags()` and
  `build_words(level, sanitize)` (`toolchain.hero:134-135`) must enter the
  cache key or a `-g` object is reused for another word (critic § 3.4).
- **The sanitizers' locations** (critic § 3.9, hand C built in one step, so with
  a `.dSYM` the compiler's path does not write): UBSan's line is the same under
  `-g0`, `-gline-tables-only` and `-g`; ASan's frame names file and line under
  line tables and `-g`, none under `-g0`. Through the compiler's own path and on
  Linux, unrun. The run goldens' 21 `!sanitizer:` needles name no file or
  line, so the net would not see a lost location.
- **Defect 335 is open**: stepping out of a `return` lands on the `function`
  line under every route; do not attribute it to a route.
- The CI legs' clangs (Ubuntu clang 18.1.3, Windows clang 20.1.8, the Darwin
  runner's Apple clang) are none of the three measured here: say what the
  adopted route owes them.

## The cases a built route passes before the synthesis

`run/fixedbugs-170-*` and `run/fixedbugs-140-*`, the `lines` suite,
`flags.hero`'s and `compiling.hero`'s tests (the compiler's own tests
narrowed), the `run` suite filtered to the `!sanitizer:` cases, and a hand lldb
run of breakpoint, step and `p` on this Mac.

## The rules every seat works under

- **Your own copy**: `cp -R` the frozen tree
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-197` to
  `<scratchpad>/197-<seat>/tree` (the scratchpad is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`),
  then `rm -rf build` inside the COPY only. Build your own compiler there from
  the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (a
  few seconds; `./heroes build selfhost/main.hero -o heroes-next` about a
  minute; both durations unmeasured today). Never build, run or write in the
  frozen tree, the trunk, a `lane-b14-*` worktree or another seat's copy.
- **The instrument** (critic 17, § 3.1). Thirteen lanes of batch 14 run on
  this Mac, so no duration is valid: counts only. **On this Mac clang runs its
  cc1 as a child process**, so read clang's counters with `timeout N
  /usr/bin/time -l clang -fintegrated-cc1 <words>` (time INSIDE timeout, the
  flag on), never `/usr/bin/time -l` around `timeout` or around `./heroes`:
  on one 10,000-line file the three readings were 16,110,936 instructions
  (timeout's), 136,775,523 (the driver's) and 45,772,156,952 (the compile's).
  A `heroes build` is read by its units' command lines replayed alone, and by
  object sizes under `build/tu-*/`. **In the Linux container there is no
  `/usr/bin/time`**: one clang per `docker run`, and read
  `/sys/fs/cgroup/memory.peak` or python3's
  `resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss`. Read every
  footprint twice: panel 190's critic measured a 2 to 5% spread at 400 and
  about 8% at 800.
- **Room**: this Mac has 25,769,803,776 bytes of RAM beside 13 lanes. **One
  clang over 4 GB at a time for the whole sitting**: before starting one, run
  `pgrep -fl 'clang.*197-'`; if another seat's is running, wait for it. The
  800-return shape (about 8 GB, panel 190's unrun estimate) runs on this Mac
  only; the Docker VM holds 8,215,117,824 bytes and cannot.
- **A run that may not end is bounded** (`.claude/rules/verification.md` § A run
  that may not end is bounded in time and in bytes): build the binary, run it
  under `timeout 600`, output to `/dev/null` or to a file you check, and
  `pgrep -fl` your own processes after. A clang killed by a byte limit dies by
  `SIGXFSZ`, which reads like a clang death: do not use `ulimit -f` on clang.
- **The Windows box** (`ssh win`, Git Bash; read at 12:24 by the critic:
  clang 23.1.1, 2 cores, 10,997,520 KiB free; `lld-link`, `lldb.exe`,
  `lldb-dap.exe`, `llvm-pdbutil.exe`, `llvm-symbolizer.exe` in
  `/c/Program Files/LLVM/bin/`): hand-written C or a single emitted file only,
  one clang at a time, your own folder `/c/w/p197-<seat>-<pid>`, `df -k /c` at
  least 8 GB free first, never remove anything; lane b14-box and lane
  b14-runtime may use it too.
- **Docker `heroes-linux-arm64`**: Debian clang 22.1.8, cc1 in-process,
  `ulimit -s` 8,192 soft and unlimited hard, `nproc` 8.
- **No paid run.** Times written only after `date` is read.
- **Write your report as you go** to `<scratchpad>/197-<seat>/report.md`; the
  coordinator copies it into `docs/panel/197-reports/<seat>.md`. Every number
  with the command that produced it.
