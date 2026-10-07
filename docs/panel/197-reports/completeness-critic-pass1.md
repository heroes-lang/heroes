# Panel 197, completeness critic, first pass: the briefs against the frozen tree

Written from 12:13 to 12:33 by the clock (`date` read at 12:13:06, 12:15:58, 12:18:18,
12:24:30, 12:29:50, 12:31:43). Frozen tree `lane-panel-197` at `dad2da47`; `T` below is
that path. Everything here was read-only on the tree and the trunk. The clang probes
ran on hand-written C in `<scratchpad>/197-critic/depth/` (generators in `../gen/`),
each under `timeout 120`; two read-only commands went to the Windows box (`ssh win`)
and three throwaway `docker run --rm heroes-linux-arm64` read its clang. No paid run.
No verdict on the proposal.

## 1. Every framing fact, with its status

### 00-shared.md

| # | Fact as the brief gives it | Status | Command / what is true |
|---|---|---|---|
| 1 | Tree `dad2da47` = the trunk as pushed with batch 13 plus one rule commit | **verified** | `git -C T log --oneline -3`; `origin/main` is `34f95b71`; `git show --stat dad2da47` touches `.claude/rules/generated-c.md` only (2+, 1-) |
| 2 | `grep -n -i 'lldb\|debugger\|line-tables' spec/heroes-spec.md` prints nothing | **verified** (exit 1, re-run 12:14) | Also empty: `grep -n -i 'debug\|#line\|dwarf\|sanitiz\|backtrace\|line number'` on the spec. The spec states no depth floor either (`grep -n -i 'floor\|deep'` prints only `:280` *Recursion too deep aborts*) |
| 3 | The proposal "moves one sentence of design.md Part 2" | **incomplete** | Under (B)/(D) at least three more design.md sentences change meaning: `:653-654` (§3.1 *Debug info*, unchanged only if line tables stay), **`:3682`** (Part 8 item 12, *"The mangled names are what `lldb` shows for variables"*: false under (B)), and `:750` (§3.2, QBE rejected partly for *"no DWARF"*). `grep -n -i 'lldb\|debugger\|DWARF\|debug info' docs/design.md` |
| 4 | `-g` is "a flag every build passes" | **verified, and understated** | `flags()` also reaches every header probe (`compiling.hero:136-137`, `probe_flags = without_diagnostics(flags())`, which keeps `-g` by its own comment `:128-131`), the runtime object (`toolchain.hero:281`), the toolchain identity probe (`toolchain.hero:117`), the link line (`link.hero:134`), and `--emit-c`'s rounds (verification.md, defect 298). It is also in **the build cache key**: `compiler_key = digest(flags().join(" ") + ...)` (`toolchain.hero:120`), so any change to `flags()` makes every build directory cold once |
| 5 | `flags()` passes sixteen words, the second `-g`; `function flags()` at :91, `]` at :109; `awk 'NR>=93 && NR<=108' ... \| grep -c '"'` = 16 | **verified** | `sed -n 91,109p selfhost/cli/flags.hero`; the awk command prints 16. The file's own test pins it: `assert got.len() == 16`, `assert got[1] == "-g"` (`flags.hero:208-211`), so every route that moves `-g` edits that test |
| 6 | Defect 322: 400-return shape at `-O2`, 1,695,369,184 under `-g`, 970,589,672 under `-gline-tables-only` (−42.7%), 800 unrun, carried | **verified as the card's text; the C it describes no longer exists** | Card `issues/2026-10/04/2026-10-04-0037-defect-322-*.md` and `docs/panel/190-reports/completeness-critic.md` § 6 give both numbers. Computed −42.75%. They were read on **the trunk's C at `703af779`, before panel 190's one-exit landing**: defect 231 closed at `075b425d` (2026-10-04, `git log -1 075b425d`). Today's 400 C is the post-repair emission (closest to that table's *A-star* row: 1,985,595,312 / 913,409,056). The brief should say the card's number describes no current emission |
| 7 | Defect 219: dies between 3,000 and 5,000 nested variants on this Mac, by 10,000 in the Linux container; 64 MB stack compiled 10,000; past panel 184's R6 floor of 2,000; since `6c95f44a` exit 2 via `clang_died.hero` | **verified as the card's text**, two refinements | Card `issues/2026-10/03/2026-10-03-0004-defect-219-*.md`; `git log -1 6c95f44a` (2026-10-03 12:28, *Defect 170: clang is handed a chain of structs from its bottom past 32...*); `clang_died.hero` exists (115 lines). (a) **How the stack was raised is in the tree**: `selfhost/emit/typeorder.hero:122-123`, *"the same clang under `ulimit -s 65520` compiles it"*, and **65,520 KB is this Mac's hard limit** (`ulimit -Hs` = 65520), so (A) on Darwin has no margin above the measured value. (b) **R6's floor is not a spec sentence**: N = 2,000 measured on this Mac and Linux arm64, Windows owed, *"the sentence is not written in the spec"* (`issues/2026-10/03/2026-10-03-1130-panel-184-r5-landed-*.md:65-70`) |
| 8 | The route list (A) to (D), "to be widened" | **incomplete**: see § 2 | |
| 9 | (C): find levels with `git grep -n '"-O' -- selfhost/cli/` | **verified as a command; the coordinator could have stated the answer** | `heroes build` defaults `-O0` (`verbs.hero:31`, `table.hero:105`), `heroes run` **`-O2`** (`verbs.hero:56`, `table.hero:114`), `heroes test` `-O0` (`verbs.hero:105`, `table.hero:120`); `--sanitize` is orthogonal to the level (`compile.hero:63-76`); the runtime object takes the build's level (`produce.hero:122`). The harness itself runs under `heroes run`, so at `-O2`; `run/` goldens run at `-O0`, `-O2` and `--sanitize` (`suite_run.hero:157-166`). CLAUDE.md's `./heroes build selfhost/main.hero` is therefore **`-O0`** |
| 10 | (D) is bound by `.claude/rules/cli-surface.md`'s stopping rule | **verified** | `cli-surface.md:18-20`: a flag enters only if the fixpoint, the golden harness or Part 11's harness must type it, or it has a measured Part 11 effect. Nothing in the tree types a debug-level flag today (`--no-line` is the recorded refusal, `:61`) |
| 11 | design.md Part 2 `:630-632` quote | **verified** | `sed -n 630,632p docs/design.md`; Part 2 is `:614-635` (`grep -n '^## Part' docs/design.md`) |
| 12 | design.md §3.1 `:653` promises the line mapping only | **verified** | `sed -n 653,654p`: *"`#line` directives map generated C back to `.hero` lines; lldb breaks on and steps through the author's source."* |
| 13 | "Under (B), `p x` would show nothing" | **an inference, written as a fact** | Unrun. It is what compiler-engineer item 4 measures; the brief should say *unrun* |
| 14 | ASan/UBSan file and line under `-gline-tables-only`, and the Windows format, are unrun | **verified as unrun**; part of it now read | On the box, clang 23.1.1's `-g` is **`-gcodeview -debug-info-kind=constructor`, cc1 in-process** (`ssh win '"/c/Program Files/LLVM/bin/clang" -### -c -g -x c /dev/null'`), and this Mac's driver, asked for `--target=x86_64-pc-windows-msvc -fuse-ld=lld`, puts `"-debug"` on the link line (so a `.pdb`), an inference for the box until run there. CodeView, not DWARF, is the answer to *DWARF or CodeView* |
| 15 | Seed builds in "about 5 s"; `heroes build selfhost/main.hero` "about a minute" | **carried, no command** | Not run by the critic (read-only). The skill's last reading is 3.34 s and 60.97 s, 2026-09-24 (`SKILL.md:118-124`). Durations are invalid today anyway (fact 16), so write *a few seconds* and *about a minute, unmeasured today* |
| 16 | Thirteen lanes of batch 14 run on this Mac | **partly verified** | `git worktree list` shows exactly 13 `lane-b14-*` worktrees at `dad2da47`; `uptime` read load 4.00 at 12:16; at 12:31 `pgrep -fl clang` showed lane-b14-parse and lane-b14-ir compiling. *Run at the same time* is a process fact, not a count |
| 17 | `/usr/bin/time -l`'s *instructions retired*, *peak memory footprint*, *maximum resident set size* are the instrument | **verified to print; FALSE as an instrument of clang on this Mac without a word the brief omits** | See § 3.1. On this Mac **clang runs cc1 out of process**, so `/usr/bin/time -l clang ...` counts the driver; under `timeout` outside it counts `timeout`; on `./heroes build` it counts `heroes`; and the Linux container has no `/usr/bin/time` |
| 18 | verification.md § *A run that may not end* | **verified** | The section is *A run that may not end is bounded in time and in bytes*. The brief bounds bytes with `ulimit -f 400000` where the rule names `/dev/null` or `head -c`: a bound all the same, but a clang that passes it dies by `SIGXFSZ`, which reads like a clang death |
| 19 | Windows box: `ssh win`, Git Bash, clang 23.1.1, lld-link, 2 cores, about 11 GB free, shared with two lanes | **verified except the sharing** | `ssh win 'clang --version; nproc; df -k /c; which lld-link'`: clang 23.1.1, 2, 10,997,520 KiB free (90% used), `/c/Program Files/LLVM/bin/lld-link`. Sharing: newest folder in `/c/w` is `b14-box-37776`; `tasklist` showed no clang or heroes at 12:24 |
| 20 | Docker `heroes-linux-arm64` is free | **verified** | `docker ps` empty at 12:25; image 2 weeks old. **Facts the brief should add**: Debian clang **22.1.8**, cc1 **in-process**, `-g` = **`-debug-info-kind=constructor`**, `ulimit -s` 8192 soft / unlimited hard, `nproc` 8, and the Docker VM holds **8,215,117,824 bytes** (`docker info`), so an 800-return compile (about 8 GB, panel 190's unrun estimate) cannot run there |
| 21 | Report path `docs/panel/197-reports/<seat>.md` in the copy, plus `<scratchpad>/197-<seat>/report.md` | not checkable (future files) | |

### compiler-engineer.md

| # | Fact | Status | Command / what is true |
|---|---|---|---|
| 22 | Panel 190's critic report § 6 holds the shape | **verified, with a gap** | § 6 is *A route nobody listed: line tables without variable locations* (`:200-230`); **the shape itself is in panel 190's shared brief**: *"a function of N strings, each followed by an `if` that returns it"* (`docs/panel/190-briefs/00-shared.md:38-40`). Its files, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/190-critic/pass2/`, **do not exist in this session's scratchpad** (`ls`: no such file) |
| 23 | Defect 170's cases; `git log --format='%h %s' -1 6c95f44a`; `run/fixedbugs-140-*` | **verified** | Three `fixedbugs-140-*` and two `fixedbugs-170-*` cases in `tests/golden/run/` (`ls tests/golden/run/ \| grep fixedbugs-1[47]0`). The brief names 140's and not 170's paths: `fixedbugs-170-variants-32-deep-build`, `fixedbugs-170-variants-and-options-100-deep-build` |
| 24 | "the Windows link flag the compiler already passes for its own binary, `flags.hero` from line 113 ... to about line 170" | **line numbers verified; the framing misleads** | `:113` is the sentence, `link_flags()` is `:168-194`, `-Wl,/STACK:67108864` at `:194`. It is passed on **every binary `heroes` links on Windows** (programs and compiler alike) and sets **that binary's** stack. **It cannot change clang's**: `clang.exe`'s reserve is **`SizeOfStackReserve: 10000000`** in its own PE header (`ssh win '"/c/Program Files/LLVM/bin/llvm-readobj" --file-headers ".../clang.exe"'`), and its cc1 runs in-process. Panel 107's refusal of `-Wl,-z,stacksize=` and of a created main thread (`flags.hero:150-160`) is the precedent for a non-uniform stack number |
| 25 | Item 3, "the compiler's own build under each route: instructions retired and binary size" | **unmeasurable as written** | `/usr/bin/time -l ./heroes build ...` counts `heroes`, not the clang children it spawns (§ 3.1). Also: binary size on Darwin does not carry the DWARF, which stays in the kept `.o` (`ecbf182f` body, 2026-08-16: *"binary size unchanged ... the DWARF is in the object"*); object sizes under `build/tu-*/` are the measure |
| 26 | Item 5, which builds run at which level | **answerable now**, fact 9 | |

### ffi-pragmatist.md

| # | Fact | Status | Command / what is true |
|---|---|---|---|
| 27 | Sanitizer words: "read `selfhost/cli/` for the words" | **verified** | `-fsanitize=address,undefined` (`flags.hero:200`), on the compile and the link line (`link.hero:136`); not a leak detector on Darwin arm64 (`flags.hero:196-198`); LeakSanitizer exists on Linux only (`platforms.md`) |
| 28 | Item 3: "if the runtime prints a backtrace anywhere" | **answerable now** | `hero_panic` prints `panic: <msg>` and aborts, no location, no backtrace (`runtime/parts/panic.c:39-43`); the one abort that names a function is the stack guard, by **`dladdr`** on POSIX (`runtime/parts/stack.c:371-395`) and nothing on Windows (`stack.c:725-735`: *"`SymFromAddr` needs dbghelp ... and a PDB beside the binary"*). **And `flags.hero:174` ties that naming to `-g`**: *"Darwin names the symbol either way (`-g` on every build, nothing strips)"*. `dladdr` reads the symbol table, so it should survive any debug word, but that is an inference the comment contradicts and a seat should run |
| 29 | Item 4: "`where lldb windbg cdb`", running one only if installed | **answered** | `/c/Program Files/LLVM/bin/` holds **`lldb.exe`, `lldb-dap.exe`, `llvm-pdbutil.exe`, `llvm-symbolizer.exe`**; `windbg` and `cdb` are not on PATH. The CI recorded `lldb.exe` exiting `0xC0000135` (`.github/workflows/ci.yml:381-397`); whether the box's does is the question. `llvm-pdbutil` reads the `.pdb` with no debugger at all |
| 30 | Item 5: the 400 shape's footprint on Linux arm64 | **no instrument named for that platform** | The container has no `/usr/bin/time`, no `perf`, no `valgrind`; it has `python3`, `perl` and `/sys/fs/cgroup/memory.peak` (cgroup v2) (`docker run --rm heroes-linux-arm64 bash -c '...'`) |

## 2. Routes the list missed

Asked of each: what would have to be true for it to exist. Probes are the critic's,
on hand-written C, Apple clang 21.0.0, `-O0 -fno-crash-diagnostics`, `ulimit -s` 8176.

**(E) Keep full `-g` and hand LLVM's DWARF writer the type chain from its bottom, as
`typeorder.hero`'s `in_order` already does for clang's type conversion.** For this to
exist, the death under `-g` must be a recursion whose order the emitted C decides.
Measured on a hand-written variant chain shaped like the seed's (tag enum, case struct,
variant struct with an anonymous union `as`; `gen/variants.py`), 5,000 deep, with a
typeorder-shaped touch function and a file-scope variable of the top type:

| 5,000 variants | `-g0` | `-g` `-emit-llvm -S` | `-g` `-c` | `-gline-tables-only` `-c` |
|---|---|---|---|---|
| exit | 0 | **0** | **254, *Illegal instruction: 4*** | 0 |

So on this shape **the front end survives `-g` and the backend dies**. Without the
file-scope variable (`v5000nog.c`) `-g -c` exits 0. With **bottom-up file-scope pointer
declarations** before it (`__attribute__((used)) static R<i> *const hero_dbg_<i> = 0;`
and the same for each case struct, `v5000g.c`) `-g -c` exits 0 at **5,000 and at
10,000**, where without them it dies at both. Object at 5,000: 2,990,032 bytes under
`-g`, 545,800 under `-gline-tables-only`. **Hand shape only**: whether the real emission
of defect 170's program meets its deep type in a global, a parameter or something else,
and whether Linux and Windows agree, is unrun.

**(F) Name the debug kind, not only the level.** `-g` is not one thing across the legs,
measured with `clang -### -c -g`: this Mac **`-debug-info-kind=standalone`**, Linux arm64
(Debian 22.1.8) **`constructor`**, the Windows box (23.1.1) **`constructor` + CodeView**.
`-g -fno-standalone-debug` gives the Mac `constructor`. On the hand shape it did not move
the 5,000 death (exit 254), nor did `-gdwarf-4` or `-fno-eliminate-unused-debug-types`
(`-debug-info-kind=unused-types`). Their effect on memory at `-O2` is unrun.

**(G) A per-unit word chosen by the compiler**, `-g` by default and `-gline-tables-only`
for a unit whose deepest by-value chain (`cli/deep_types.hero` already computes it) or
whose size passes a measured threshold. It exists if clang's death and cost are
predictable from what the emitter knows; `IN_ORDER_FROM` is the precedent of the shape.

**(H) Retry on death**: a unit clang dies on (`clang_died.died_by`) is compiled again with
`-gline-tables-only`, and the build says so. It exists because the build already tells a
death from a refusal; its cost is one second compile only where clang died.

**(I) Probes at `-g0`.** Header probes never link and are never debugged, yet compile
under `-g` (`probe_flags`). Exists if no probe answer depends on `-g`
(`compiling.hero:128-131` asserts `-g` changes no predefined macro).

**(J) The floor, `-g0`**, to be refused on evidence rather than left unlisted: no line
table, so no `.hero` breakpoint and no ASan file and line.

**(K) `-gno-column-info`.** A column under `#line` is the C file's column printed against
a `.hero` line; dropping it may be both smaller and truer. Unrun.

**(L) A debug word through the environment** (`HEROES_...`), the third input class
`cli-surface.md` admits; the same stopping rule would bind it.

**Not routes, measured**: `-g1` and `-gmlt` are aliases of `-gline-tables-only` (same
`-debug-info-kind=line-tables-only`). `-gsplit-dwarf` is dropped silently by the Darwin
driver (no `-split-dwarf*` in `-###`) and honoured by Linux's, and moves DWARF out of
the object rather than out of clang's memory (that last half unrun).
`-gline-directives-only` is a distinct kind with no lldb use known (unrun). No clang
flag that limits type depth in debug info was found: `clang --help | grep -i -E
'debug|^ *-g' | grep -i -E 'depth|limit|level|max'` names only `-fno-standalone-debug`
(*"Limit debug information produced to reduce size"*). `-cc1` and `-mllvm` options were
not searched, so this stays a question rather than a premise.

## 3. Questions the sitting should ask and does not

**3.1 The instrument measures the wrong process unless the brief says how.** Measured on
one 10,000-line file (`v5000nog.c`, `-O0 -g -c`):

| invocation | instructions retired | peak footprint | max RSS |
|---|---|---|---|
| `/usr/bin/time -l timeout 120 clang ...` | 16,110,936 (timeout's) | 1,081,656 (timeout's) | 132,382,720 |
| `timeout 120 /usr/bin/time -l clang ...` | 136,775,523 (the driver's) | 2,588,984 (the driver's) | 130,957,312 |
| `timeout 120 /usr/bin/time -l clang -fintegrated-cc1 ...` | **45,772,156,952** | **115,622,320** | 130,023,424 |

`clang -### -c t.c` prints no `(in-process)` on this Mac and does with
`-fintegrated-cc1`; the live lanes' builds at 12:31 show a separate `clang -cc1` child
(`pgrep -fl clang`). Panel 190's footprint numbers were taken with
`-fintegrated-cc1` (its critic § 2). Max RSS follows the child either way. Linux's and
Windows' clangs run cc1 in-process by default. So: every Mac counter needs
`-fintegrated-cc1` on a clang run alone; `./heroes build` can only be read by max RSS or
by the per-unit command lines; footprint varies 2-5% at 400 and about 8% at 800 on one C
(panel 190's critic § 2), so each figure is read twice.

**3.2 Which scheduled work rests on full `-g`?** `docs/ROADMAP.md:163`, **M-typed-inspection**
(row 69, scheduled by author instruction 2026-09-06): its page measured that *"half of
it already works"* under today's `-g` (locals by the author's spelling, a `str`'s text, a
record's fields, `docs/roadmap/milestones/M-typed-inspection.md:15-22`), and its first
step is an lldb suite whose falsifier is *"`-g` deleted, the suite must go red"* (`:55`).
**M-vscode-extension** (row 73) composes `lldb-dap` over the DWARF and shows lldb's
variables pane (`M-vscode-extension.md:16-29`). **M-panic-location** (row 68) passes
`__FILE__`/`__LINE__` and does not need DWARF (`M-panic-location.md:14-17`). Any route that
removes variable DWARF from a build an author debugs takes the measured half of row 69
away; the brief's *What it costs* names none of this.

**3.3 Nothing in the net executes lldb.** `git grep lldb -- tests/harness selfhost` finds
comments only; `lldb_breaks_on_a_hero_line` is in `archive/bootstrap-rs/` since
2026-08-19 (`suite_records.hero:373-376`). The `lines` suite reads `#line` claims with no
debugger. So a route that breaks breakpoints passes every gate; the seats' lldb runs are
the only instrument, and the landing owes one.

**3.4 On Darwin the DWARF lives in the kept object**, and the binary carries a debug map
(`ecbf182f`'s body; `issues/2026-08/25/2026-08-25-0418-*.md`). So lldb resolves through
`build/tu-*/<unit>.o`, keyed by `compiler_key` and `build_words`. Any route whose debug
word lives outside `flags()` and `build_words(level, sanitize)` (`toolchain.hero:134-135`),
such as (D)'s flag or (G)'s per-unit choice, must enter the key, or a `-g` object is
reused for a line-tables build and the reverse.

**3.5 Where does the `-g` death happen on the real emission**, front end or backend, and
through which declaration? § 2 (E) shows the backend on a hand shape. If it is the same on
the emitted C, (E) competes with every flag route; if not, (E) falls.

**3.6 Is (A) possible on every platform?** Darwin's hard stack limit is 65,520 KB, the very
value measured; Linux's is unlimited in the container; Windows' clang.exe reserve is fixed
at 10,000,000 bytes in its PE header. A per-platform number is what panel 107 refused.

**3.7 Defect 335 is open** (`issues/2026-10/04/2026-10-04-0048-defect-335-*.md`): stepping out of
a `return` lands on the `function` line. The compiler-engineer's lldb probe (*a `return`
inside a loop*) will meet it under every route; the brief should say so, or a seat
attributes it to a route.

**3.8 The CI legs' clangs** (Ubuntu clang 18.1.3, Windows clang 20.1.8, the Darwin runner's
Apple clang, which died at 1,000 before typeorder's help, defect 155) are none of the
three the seats measure. Does the adopted route owe them a case (the `fixedbugs-170-*`
and `-140-*` goldens run there), and at what depth?

**3.9 Where the sanitizers' locations come from.** Measured at 12:33 on a hand-written
`san/ub.c` (signed overflow in a `static inline`, then a heap use-after-free), built in
ONE step with `-O0 -fsanitize=address,undefined`, so the driver wrote a `.dSYM`, which
the compiler's compile-then-link path does not:

| debug word | UBSan line | ASan frame `#0` |
|---|---|---|
| `-g0` | `ub.c:5:50: runtime error: signed integer overflow` | `main+0x1e8 (ub-g0:arm64+...)`, no file or line |
| `-gline-tables-only` | the same | `main ub.c:13` |
| `-g` | the same | `main ub.c:13` |

So UBSan's location does not depend on the debug word and ASan's needs line tables.
Through the compiler's own path (objects kept under `build/`, no `.dSYM`, a debug map)
and on Linux, unrun. A planted bug must sit in code compiled
under the unit's flags (the header's `static inline`), or a separately built C library
carries its own flags and the route is not what is measured. The run goldens' 21
`!sanitizer:` needles (`git grep -h '!sanitizer:' -- tests/golden | sort | uniq -c`: 17
*on unknown address*, 3 *bad-free*, 1 *double-free*) name no file or line, so the net
would not see a lost location.

**3.10 Which design.md sentences, and which lane.** (D) adds a flag to `table.hero`, which
the site's `claims.ts` reads (verification.md's map) and the stopping rule may refuse. If
the sitting adopts a new flag, does the soundness lane still hold (SKILL.md: *"When in doubt
take the full panel"*)?

**3.11 Collisions with batch 14.** `lane-b14-cli` and `lane-b14-emit` exist; at 12:16 none of
the b14 worktrees had a dirty `flags.hero`, `units.hero`, `toolchain.hero`, `compiling.hero`,
`link.hero`, `clang_died.hero`, `stack.c`, `design.md` or `generated-c.md` (`git status
--short` in each). Whether their assigned defects touch those files is unasked.

**3.12 The machine's room.** This Mac has 25,769,803,776 bytes of RAM and 8 cores beside
13 lanes; 140 GB free on the data volume. *One clang over 4 GB at a time* is per seat;
two seats can each hold one. The 800 shape (about 8 GB, unrun) needs a sitting-wide slot.

## 4. Repairs recommended to the briefs

1. **00-shared, instrument line** (fact 17, § 3.1): replace with *On this Mac clang runs
   cc1 as a child process: read clang's counters with `timeout N /usr/bin/time -l clang
   -fintegrated-cc1 <words>` (time inside timeout, the flag on), never `/usr/bin/time -l`
   around `timeout` or around `./heroes`. A `heroes build` is read by max RSS and by its
   units' command lines replayed alone. In the Linux container there is no `/usr/bin/time`:
   use one clang per `docker run` and read `/sys/fs/cgroup/memory.peak`, or python3's
   `resource.getrusage(RUSAGE_CHILDREN).ru_maxrss`. Read every footprint twice (panel 190's
   critic § 2: 2-5% spread at 400, about 8% at 800).*
2. **00-shared, defect 322 bullet** (fact 6): add *read on the trunk's C at `703af779`,
   before defect 231 closed at `075b425d`; today's emission is the one-exit C, so the
   number is history and is re-measured*.
3. **00-shared, defect 219 bullet** (fact 7): add *raised by `ulimit -s 65520`
   (`typeorder.hero:122-123`), which is this Mac's hard limit*; *R6's N = 2,000 is measured
   on this Mac and Linux arm64 and is not a spec sentence*.
4. **00-shared, route list**: add (E) to (L) of § 2 with the critic's hand-shape table for
   (E), and state (C)'s levels (fact 9) instead of a command to find them.
5. **00-shared, What it costs**: add design.md `:3682` and `:750`; mark *"`p x` would show
   nothing"* as unrun; add M-typed-inspection, M-vscode-extension and that no suite runs lldb
   (§ 3.2, 3.3); add the kind per platform (standalone / constructor / CodeView).
6. **00-shared, Windows**: replace *about 11 GB free* with the reading and time; add that
   `lldb.exe`, `lldb-dap.exe`, `llvm-pdbutil.exe`, `llvm-symbolizer.exe` are installed and
   `clang.exe`'s stack reserve is 10,000,000 bytes.
7. **00-shared, Docker**: add clang 22.1.8, cc1 in-process, `-g` = constructor, stack
   8,192 KB / unlimited, VM 8,215,117,824 bytes (no 800 shape there).
8. **00-shared, seed timing**: *a few seconds; about a minute; durations unmeasured today*.
9. **compiler-engineer item 1**: give the shape's definition from
   `docs/panel/190-briefs/00-shared.md:38-40` and say its files are gone; ask it to measure
   today's emission and, as a control, to say how far it is from the card's.
10. **compiler-engineer item 2**: name `fixedbugs-170-*`; ask which phase dies
    (`-emit-llvm` against `-c`) and on which declaration, and to try (E) on the real
    emission at 3,000, 5,000 and 10,000; replace *the Windows link flag ... for its own
    binary* with *clang.exe's own reserve (10,000,000, PE header); `link_flags()` sets the
    stack of what heroes links, never clang's*.
11. **compiler-engineer item 3**: measure the units, not `heroes`, and object sizes under
    `build/tu-*/`, since Darwin's binary carries no DWARF.
12. **compiler-engineer item 4**: warn of defect 335; add *does `b file.hero:N` still
    resolve, and does the caret's column mean anything* (K).
13. **compiler-engineer, new item**: where the debug word enters the cache key under each
    route (§ 3.4), and the `flags.hero` tests a route must change.
14. **ffi-pragmatist item 2**: the planted bug in the header's `static inline`, not a
    separate library; record that the `!sanitizer:` needles name no location.
15. **ffi-pragmatist item 3**: name the stack guard's `in <function>` line and
    `flags.hero:174`'s claim about `-g`, on Darwin and Linux, under each route.
16. **ffi-pragmatist item 4**: lldb.exe is installed: run it, and read the `.pdb` with
    `llvm-pdbutil dump -l -symbols` under each route whatever lldb does.
17. **ffi-pragmatist item 5**: name the Linux memory instrument (repair 1).
18. **Both briefs**: name the cases a built route must pass before the synthesis (SKILL.md
    § 3c): `run/fixedbugs-170-*` and `-140-*`, the `lines` suite, `flags.hero`'s and
    `compiling.hero`'s tests, `run` filtered to `!sanitizer:` cases, and a hand lldb run.
19. **00-shared, room**: one clang over 4 GB at a time for the whole sitting, not per seat.

Nit: −42.7% is −42.75% computed, −42.8% rounded (`python3 -c
"print((970589672-1695369184)/1695369184*100)"`).
