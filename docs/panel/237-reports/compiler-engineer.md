# Panel 237, the compiler-engineer's report

Written 2026-11-08 as I went, in worktree `.claude/worktrees/237-compiler-engineer/`, detached at
`163912031` (`git rev-parse --short HEAD`). No paid run, no credential, no `gh` write, no file
outside this worktree; scratch programs under its `build/ce/`, which `.gitignore` ignores.

## The compiler

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: exit 0.
- `/usr/bin/time -l ./heroes build selfhost/main.hero -o heroes-next`, in its own command: exit 0,
  `real 354.42` against `user 141.50` (the machine at load 22, `uptime`), 701,621,619,195
  instructions retired by the compiler's own process. `./heroes-next --version` prints `sources
  0d7a6741345899d8`, the brief's.

## The instrument, first: `/usr/bin/time -l` counts one process

The brief asks every cost in *instructions retired* from `/usr/bin/time -l`. That count is the
process `time` starts and **none of its children**, measured here:

```
/usr/bin/time -l sh -c 'true; true'                          -> 22,511,732 / 22,692,703 / 24,050,474
/usr/bin/time -l sh -c 'true; clang -fintegrated-cc1 -c t.c' -> 24,465,998 / 24,742,128 / 24,472,567
/usr/bin/time -l sh -c '<four such clangs>'                  -> 29,532,589 / 29,911,176 / 29,953,902
/usr/bin/time -l clang -fintegrated-cc1 -c t.c               -> 100,823,836   (one clang alone)
```

Four clangs add about 5 M to the shell's count where one clang alone is about 100 M: what is
counted is the shell's own work of spawning them. And Apple clang 21.0.0 here runs cc1 as a
**separate process** (`clang -### -c t.c` prints no `(in-process)` job), so even `time -l clang
...` counts the driver alone unless `-fintegrated-cc1` is passed (75,540,823 with
`-fno-integrated-cc1` against 100,823,836 with it, on a one-line `t.c`). **So `time -l heroes
build` cannot see the refusal's cost**, which is one more clang process: the measurement below
counts the compiler's own instructions with `time -l`, and each clang it starts through a
counting `clang` placed first on `PATH` (the runtime spawns `clang` by `execvp`,
`runtime/parts/run.c:1083`), every clang run with `-fintegrated-cc1` so its cc1 is in its count.

## Where the landed work lives, measured

`git show --numstat --format= 96d42c250 -- <area>`: `selfhost/` +1,004 / -198, `runtime/` +183 /
-2. Of `selfhost/`'s, `emit/c_words.hero` (+274) is readers moved out of `emit/macro_guard.hero`
(+155 / -169), not new behaviour. Code lines (tests, comments and blanks taken out, the
`code_of` filter of `.claude/rules/verification.md` plus `NF > 0`):

| file | `wc -l` | code |
|---|---|---|
| `selfhost/cli/shadowed.hero` (the refusal) | 345 | 136 |
| `selfhost/cli/package_macros.hero` (item 2) | 101 | 30 |
| `selfhost/emit/guarded_names.hero` (+63 in the landing) | 273 | 189 |
| `selfhost/emit/c_words.hero` (moved) | 274 | 160 |
| `runtime/heroes_reserved_open.h` / `_close.h` (non-comment lines) | 79 / 79 | 57 / 57 |

One call site: `selfhost/cli/produce.hero:212`, `shadowed.checked(...)` on the build path, before
`probing.planned`. The whole compiler for scale: 747 tracked `selfhost/*.hero`, 193,347 lines
(`git ls-files 'selfhost/*.hero' | xargs cat | wc -l`); `selfhost/cli/` 36,658.

**Core or sugar.** Neither: items 1 and 2 add no construct the lexer, parser, checker, lowering or
emitter handle. They live wholly in the driver (`selfhost/cli/`), run once a build, and change no
emitted byte of a program they accept (the landing's 733 re-blessed emissions moved only the
`hero_program_sources` line, its body says). Item 3 changes no compiler file. So the ceiling
question is the driver's, not the language's (design.md §1.7, Part 5).

## The refusal and the environment, reproduced with this compiler

In `build/ce/refuse/`, the brief's overflow program (its sum `x + args().len()`) beside a
`stdint.h` of `#include_next <stdint.h>`: `heroes-next build main.hero` prints
`error[shadowed_header]` at `main.hero:1:1`, its excerpt quoting `1 | function main()` with a caret
under `f`, a line that has nothing to do with the cause (question 4's business, noted here).

In `build/ce/boolenv/`, `b = true; print(b)` and an `envdir/stdbool.h` of `#include_next
<stdbool.h>`, `#undef true`, `#define true 0`: `CPATH=$PWD/envdir heroes-next build` exit 0, the
program prints `false`, exit 0. The critic's finding 7 reproduces.

**A shape beside it nobody listed: an empty `CPATH` element is the current directory.** clang's
`<...>` search, `clang -v -E -x c /dev/null` (Apple clang 21.0.0):

| environment | first folders printed |
|---|---|
| none | clang's resource directory, the SDK's `usr/include`, ... |
| `CPATH=env/a` | `env/a`, then clang's own |
| `C_INCLUDE_PATH=env/b` | `env/b`, then clang's own |
| `CPATH=env/a C_INCLUDE_PATH=env/b` | `env/a`, `env/b`, then clang's own |
| `-I inc` and `CPATH=env/a` | `inc`, `env/a`, then clang's own |
| `CPATH=` (set, empty) | **`.`**, then clang's own |
| `CPATH=:` | **`.`** |
| `CPATH=env/a:` (a trailing colon) | `env/a`, **`.`** |
| `C_INCLUDE_PATH=` (set, empty) | clang's own alone |
| `CPLUS_INCLUDE_PATH=env/a`, `OBJC_INCLUDE_PATH=env/a` | clang's own alone (not read for C) |

So in `build/ce/cwdroot/`, a `stdbool.h` defining `true` as 0 at the current directory and
`sub/main.hero` printing `true`, built from `cwdroot` as `heroes-next build sub/main.hero`:

```
no CPATH                 build exit 0, prints true
CPATH= (empty)           build exit 0, prints false, exit 0
CPATH=/nonexistent:      build exit 0, prints false, exit 0
```

`export CPATH=/x:$CPATH` with `CPATH` unset writes exactly the trailing colon. The door is reached
by an accident of a shell profile, not only by a folder somebody named on purpose; and it means
**"clear `CPATH`" cannot be done by setting it empty** (`hero_env_put` is `setenv`, and the runtime
has no unset, `grep -n unsetenv runtime/parts/*.c` prints nothing): on this clang an empty `CPATH`
adds `.`.

## Item 3: what the refusal refuses among the tracked programs

- **The opening's listing**, made with the refusal's own kept files (`build/shadowed/opening-*.c`
  and `words-*.rsp` of a build in `build/ce/boolenv/`: `clang @words.rsp -M -MT dep opening.c`):
  38 headers outside `runtime/`, **36 names** once taken relative to their system folder
  (`Availability.h` ... `float.h`, `math.h`, `stdbool.h`, `stddef.h`, `stdint.h`, `sys/cdefs.h` and
  the `_types/` and `sys/_types/` files they reach). `stdio.h`, `string.h` and `stdlib.h` are not
  among them: the unit's opening does not read them on this Mac.
- **The tracked tree**: `git ls-files`, each path matched against the 36 (equal, or ending in
  `/<name>`): **0**, `runtime/` included.
- **Every `examples/*/main.hero`** (55, `ls`), built with `heroes-next build <main> -o ...`,
  three at a time: **55 exit 0**, `shadowed_header` in **0** outputs (`grep -l`). `raylib` and
  `sdl` name a package and build here.
- **The `run` goldens**: 668 `tests/golden/run/*.hero` and 2 folder cases (`ls`). Why a subset
  suffices: the refusal's verdict is a function of the folders a build names and its words, and
  659 of the 668 share one source folder, `tests/golden/run/`, and name no package (`grep -l
  'package "'`: 9 do). That folder holds 271 `.h` files, none bearing one of the 36 names (the
  `git ls-files` match above). So the subset is one package-free case (`ffi-libm.hero`), the 9 that
  name a package and the 2 folder cases, each built with `heroes-next build <case> -o ...`: **12
  exit 0**, `shadowed_header` in **0** outputs. The landing's own gate ran `run` whole at 660
  passed, 0 failed with the refusal in place (`git log -1 96d42c250`), which I did not re-run.

**Refused today among tracked programs: none.** The refusal costs no correct program of this tree.

## Item 5: a golden case annotated in its source, built

The `unsupported` form reads folder cases today: `tests/harness/cases.hero:83-85` walks every
directory ending `.case` (`CASE`, line 90) under the form's directory, whatever the form, and
`shape_unsupported` (`tests/harness/suite_golden.hero:198-210`, `verb: "build"`, `want_code: 1`)
names `tests/golden/unsupported`. Folder cases exist today only under `check/` (`ls -d
tests/golden/*/*.case`: 10, all `check/`). And a diagnostic at `1:1` can be marked: a line that
opens with `#~` is a mark row (`tests/harness/mark_readers.hero:55`).

Built in this worktree (not committed):
`tests/golden/unsupported/fixedbugs-2333-a-file-beside-the-program-named-like-a-library-header-is-refused.case/`
holding `main.hero` (line 1 `#~ shadowed_header`, then the overflow program), `stdint.h`
(`#include_next <stdint.h>`) and `main.expected` (what `build` printed, exit 1):

```
./heroes-next run tests/harness/main.hero -- ./heroes-next unsupported a-file-beside-the-program
  unsupported (only a-file-beside-the-program): 1 passed, 0 failed          exit 0
./heroes-next run tests/harness/main.hero -- ./heroes-next annotations a-file-beside-the-program
  annotations (only a-file-beside-the-program): 1 passed, 0 failed          exit 0
```

So question 6 needs no new form: one folder case, three files, and the rule *every diagnostic
annotated in its source* (CLAUDE.md § 9) is met at `1:1` by a mark row. The excerpt then quotes
the mark row itself (`1 | #~ shadowed_header`), which says more than `function main()` does but is
still not the cause. Its `.expected` holds the case's path three times, so a rename of the case
moves the expectation (as every `unsupported` expectation's `at` line does).

## The variant compiler the prototypes and the cost were measured with

A copy of `selfhost/` at `build/ce/variant/selfhost/`, four edits, each anchor asserted to match
once, `heroes-next check` exit 0, `heroes-next build ... -o heroes-p237` exit 0, `--version`
`sources 4439be3603d45fe0`:

- `cli/produce.hero:212`: `shadowed.checked` called unless `P237_SHADOW=off` (+6 / -1 by `diff`);
- `cli/compiling.hero`, `search_words`: the `--include` folders and the source's folder passed as
  `-I`, or `-idirafter` or `-iquote` by `P237_ORDER` (+16 / -2); the packages' words untouched;
- `emit/c_text.hero`, `header_lines`: under `P237_ORDER=iquote` a group's header written
  `#include "<h>"`, the compiler's own three (`emit/externs.hero:86-102`) still angled (+6 / -1,
  its `use cli/process` included): `--emit-c` of the `own` program below writes `#include "mine2333.h"` at
  line 22, where the landed compiler writes `#include <mine2333.h>`;
- `cli/shadowed.hero`, `searched_first`: under `P237_ENV=count`, the folders of `CPATH` and then
  `C_INCLUDE_PATH`, in the order clang prints them after every `-I`, counted as named, an empty
  `CPATH` element as `.` (clang's reading, above), an empty `C_INCLUDE_PATH` one as nothing (+25
  with the splitting helper).

## Item 1: what the refusal costs, in instructions

Two programs in `build/ce/`: `two/` (`function main()` / `print(1)`) and `group/` (one `extern
"math.h" link "m"` group binding `sqrt`, printing `sqrt(x: 2.0)`). Warm: each built once in each
arm before counting. The arms are one binary, `heroes-p237`, with the refusal's call made
(`on`) or not (`off`), so nothing but the call differs.

**The compiler's own process** (`/usr/bin/time -l`), 12 alternated pairs, the first discarded:

| program | `on` median | `off` median | median of the 11 pair differences | pairs `on` > `off` |
|---|---|---|---|---|
| two-line | 726,195,444 | 640,213,860 | **+84,965,204** (+13.4 %) | 11 of 11 |
| one group | 883,384,937 | 796,672,349 | **+86,199,601** (+10.9 %) | 11 of 11 |

**The clang processes it starts**, which that count cannot see: `PATH=build/ce/bin:$PATH`, where
`bin/clang` writes each run's words and `time -l` to a file of its own and runs `/usr/bin/clang
-fintegrated-cc1 "$@"` (a `-###` run passed as written). Five warm builds per arm:

| program | clang runs `on` / `off` | clang instructions, median `on` / `off` | difference |
|---|---|---|---|
| two-line | 5 / 5 / 5 / 5 / 5 against 4 / 4 / 4 / 4 / 4 | 923,559,564 / 764,782,677 | **+158,776,887** |
| one group | 7 each against 6 each | 1,198,071,949 / 1,040,680,040 | **+157,391,909** |

The run it adds is one more `-M` (by words: `-###` 1, `-M` 3 against 2 for the two-line program;
`-M` 5 against 4 with the group). **Total, a warm build: about +244 M instructions, +17.4 % of the
two-line program's 1,405 M and +13.3 % of the one-group program's 1,837 M.** Two cautions on the
figures: `-fintegrated-cc1` saves the cc1 process a real build spawns, so the clang side is if
anything low; and the +85 M in the compiler's own process, a third of the cost, I did not profile
(a `fork` of a large process is counted in the parent, and is my guess, unrun).

What the lane reported in seconds (0.17 s to 0.20 s, `git log -1 96d42c250`) is the same order: one
process. **It is a fixed cost a build, so it vanishes on a large program**: `heroes-next check
selfhost/main.hero` costs 102.3 G instructions (three runs, 102,279,566,342 to 102,338,164,821), and
a warm build of the compiler does at least that front end's work (the latency ledger's premise, *the
frontend 6.9 of the loop's 37.7*, `.claude/rules/verification.md` § The latency ceiling), so the
refusal is **at most 0.24 %** of it, under the latency rule's floor of effect, 0.5 %. On a two-line
program it is 17 %.

**The redundancy, which is the cost's real story.** In the `off` arm the two `-M` runs are
`opening.of_runtime` (`selfhost/cli/toolchain.hero:276`) and `compiling.unit_reach`
(`selfhost/cli/compiling.hero:248`, defect 2289), which preprocesses each distinct unit opening
under the unit's own words on every build. For a program with no group that opening reads the same
36 library headers the refusal lists again, under the same words less the `-W` ones
(`compiling.probe_words`, line 156). The refusal's `-v` is what the existing run lacks (it discards
stderr, `opening.hero:172`). So a route exists that folds the question into the run every build
already makes, at zero extra processes; **I did not build it** (its order differs: `unit_reach`
runs inside the rounds, after the probes, where the refusal stands before them), so by the panel's
own rule it is a direction for a later lane, not a route this sitting can adopt.

## Item 2: the other routes, built and run

Seven programs in `build/ce/routes/`, each built by `heroes-p237` under six arms and run as `./bin
one` (`build/ce/matrix.sh`, `bash`; `timeout 20`, `head -c 4096`):

- `beside`: `special`'s program (an `i64` sum past the top), a `stdint.h` beside it reading the real
  one and defining `__builtin_add_overflow` to report nothing;
- `included`: the same program, the `math.h` in an `--include` folder;
- `own`: `extern "mine2333.h"`, the header in an `--include` folder (`special`'s third);
- `boolbeside`: `b = true; print(b)`, a `stdbool.h` beside it defining `true` as 0;
- `utilown`: the program's own `util.h` (`#define HERO_UTIL 7`), bound by a group (`util.h` is
  also a header of this Mac's SDK);
- `sibling`: the program's own `money.h`, which includes its own `<money_types.h>` in angle
  brackets, as a library's header includes a sibling;
- `stdiogroup`: `extern "stdio.h"` binding `SEEK_END`, beside a `stdio.h` that reads the real one
  and redefines `SEEK_END` as 99.

| program | R0, landed | R0 with the call off | R1 `-idirafter` | R1 + refusal | R2 `-iquote` + quoted group include | R2 + refusal |
|---|---|---|---|---|---|---|
| `beside` | exit 1 `shadowed_header` | builds, `panic: integer overflow` | panics | panics | panics | panics |
| `included` | exit 1 `shadowed_header` | builds, panics | panics | panics | panics | panics |
| `own` | 2333 | 2333 | 2333 | 2333 | 2333 | 2333 |
| `boolbeside` | exit 1 `shadowed_header` | **`false`, exit 0** | `true` | `true` | `true` | `true` |
| `utilown` | 7 | 7 | **exit 1 `ffi_unknown_name`** | **exit 1** | 7 | 7 |
| `sibling` | 125000 | 125000 | 125000 | 125000 | **exit 1 `ffi_missing_header`** | **exit 1** |
| `stdiogroup` | 99 | 99 | 2 | 2 | 99 | 99 |

What the table says:

1. **`special`'s two programs no longer witness the class.** With the refusal's call taken out,
   `beside` and `included` build and panic as the language promises: the reserved pair
   (`runtime/heroes_reserved_close.h`) gives `__builtin_add_overflow` back. Today the witness is
   `boolbeside`, `true` printed `false` at exit 0. `special`'s check would stay green if the
   refusal were deleted and the stand-in were harmless, so it holds the refusal's message, not the
   class.
2. **Each reorder closes the class for the folders it moves** (`boolbeside` prints `true` under
   R1 and R2) **and refuses a correct program the refusal builds.** R1 lets the platform win every
   name: `utilown` is refused with a **false message**, *`util.h` declares no `HERO_UTIL`*, when the program's `util.h` declares it; clang read the
   SDK's. On this Mac's SDK `util.h`, `err.h`, `cache.h`, `term.h`, `search.h`, `memory.h`,
   `zlib.h`, `sqlite3.h` and `readline/readline.h` are platform headers (3,410 names under the SDK's
   `usr/include`, clang's resource directory and the toolchain's `usr/include`, `find`). R2 serves
   no angle include from the program's folder, so `sibling` is refused, `money_types.h is not on
   this machine's include path`, though it is beside `money.h`.
3. **`stdiogroup` is the shape beyond the 36 names**: a group naming `stdio.h` beside a `stdio.h`
   of the program's. The refusal does not see it (`stdio.h` is not in the opening's listing) and
   R2 does not either: 99. R1 reads the platform's: 2. Whether 99 is a wrong value is the sitting's
   reading of *the header named* (§4.19): the program named `stdio.h`, and the program's own folder
   holds one. It is not a defect of the refusal's cause, and I file nothing.
4. **R2 reopens a ratified measurement.** `selfhost/emit/decls.hero:78` keeps the angled form because *panel 036 measured what the quoted form costs: a decoy sqlite3.h
   beside the generated unit is read with zero diagnostics*. A quoted include searches the unit's own
   directory first (`build/tu-<key>/` today), so R2 needs panel 036 asked again, and it moves every
   emission holding a group's include (the `emission` and `emit` goldens), which R1 and the refusal
   do not.

Neither reorder adds a construct; each is a driver change (+16 / -2 in `cli/compiling.hero`, and
+6 / -1 in `emit/c_text.hero` for R2). **None of the three routes is cheaper in kind than another; the refusal is the
only one of the three that refuses no correct program in this matrix, and the only one that moves
no emission.**

## Item 2, the environment: what it does to every clang, and what each answer costs

What the compiler starts is `clang` by `execvp` with the environment it inherited
(`runtime/parts/run.c:1083`), so every variable below reaches every clang it runs: the runtime's
object, each unit, each probe, the refusal's own listing. Measured on Apple clang 21.0.0:

- `CPATH` and `C_INCLUDE_PATH`: their folders are searched for `<...>` after every `-I` and before
  clang's own (the table above), so a file there stands in exactly as one in a named folder does;
  an empty `CPATH` element is `.`. `CPLUS_INCLUDE_PATH` and `OBJC_INCLUDE_PATH` are not read for C
  (clang's search printed unchanged).
- `CCC_OVERRIDE_OPTIONS`: edits every clang's command line. **A leading `#` makes it silent and
  still applies the edit**: `CCC_OVERRIDE_OPTIONS='#+-DP237X=5' clang -E` prints `int seen = 5`
  with an empty stderr; through `heroes-next`, `#+-D__builtin_add_overflow(a,b,r)=...` builds the
  overflow program at exit 0 with **nothing on stderr** and the program prints
  `-9223372036854775808` at exit 0 (`build/ce/cccsilent/`). So the `### Adding argument` lines the
  critic saw are not a tell. The value `#` alone applies nothing and prints nothing (`seen = 0`,
  empty stderr), and the same build with `CCC_OVERRIDE_OPTIONS='#'` panics as it should, exit 134;
  an empty value prints `### CCC_OVERRIDE_OPTIONS: ` on every clang's stderr.

The three answers, priced:

| answer | `CPATH`, `C_INCLUDE_PATH` | `CCC_OVERRIDE_OPTIONS` |
|---|---|---|
| clear for clang | **not available**: the runtime has `setenv` alone (`hero_env_put`, `runtime/parts/os.c:906`), and an empty `CPATH` adds `.`; and it contradicts design.md §4.19 (*the search path is the machine's own `CPATH`*, panel 055) and the compiler's own note, `selfhost/head_names.hero:222`, *set `CPATH` to the directory holding it*: every program following that advice would stop finding its header | **available**: `hero_env_put("CCC_OVERRIDE_OPTIONS", "#")` once, before the first clang, the shape `selfhost/fetch/transport.hero:160-161` already uses for git; about 15 lines with its binding and comment, no process |
| count in the refusal | **built and run** (`P237_ENV=count`, +25 lines): closes all three doors and keeps the program that finds its own header through `CPATH`, below; the note's words then owe a fourth origin | not a folder; nothing to count |
| refuse a build where set | refuses every program following §4.19's own route | refuses a debugging knob; one `getenv` |

`P237_ENV=count`, with the refusal on:

```
boolenv,  CPATH=envdir           landed: build 0, prints false      counted: exit 1 shadowed_header
boolenv,  C_INCLUDE_PATH=envdir  landed: build 0, prints false      counted: exit 1 shadowed_header
cwdroot,  CPATH= (empty)         landed: build 0, prints false      counted: exit 1 shadowed_header
envown,   CPATH=hdr (own header) landed: build 0, prints 2333       counted: build 0, prints 2333
```

The counted refusal's note is then false: in `cwdroot` it says *move it out of `.`: the folder is
this program's own, one `--include` names, or one a package's `pkg-config` answers*, where `.` is
the current directory an empty `CPATH` element named and the program's folder is `sub/`. So the
real change carries each folder's origin to the note, which the prototype does not (an estimate of
15 more lines, unbuilt). And Windows separates `CPATH` with `;` (unrun here: no Windows run in this
seat).

## Item 4: `check` and the refusal

- `check` runs no clang today, measured: `heroes-next check --json main.hero` in the refused folder
  under the counting `clang` logged **0** runs, exit 0, `"notes": []`, `"diagnostics": []`.
- `check` does not take the flag that names one of the three folder kinds: `heroes-next check
  main.hero --include inc` exits 2, *`check` does not accept `--include`*. Nor does it ask
  `pkg-config`: `grep -c clang selfhost/cli/check.hero` prints 0 (run here, as feature 974's last
  re-check found), and its imports, `check.hero:12-33`, name no `cli/toolchain` and no
  `cli/process`.
- `check` costs **76.3 M** instructions on the two-line program (six runs, 76,208,152 to
  76,583,631). The refusal costs about **244 M** (item 1). Asked inside `check`, it would make the
  two-line `check` about **4.2 times** its cost today, before the `--include` flag and the
  `pkg-config` run it would also need. Panel 082's R2 refused `mono` inside `check` at 1.3 to 2.2
  times on its cost (`docs/panel/082-the-note-that-told-the-author-not-to-try.md:110-115`).
- **The line is principled, and it is already written down as broken.** Panel 082's R6, ratified,
  is *check accepts implies build succeeds*; feature 974 is that invariant's open breach at the C
  boundary, and this refusal is a second instance of the same breach, not a new kind. What would
  answer it without clang is a sentence, not a check: panel 235's R6 already puts a folder fact into
  `check --json`'s `"notes"` (`selfhost/cli/unverified.hero:7-14`), but the refusal's list of 36
  names is a fact clang gives, so a clang-free note could only say *the C boundary was not asked*,
  on every program. My cost reading: keep `check` clang-free, and state in spec § 1's verbs that the
  C boundary's refusals (`ffi_*`, `shadowed_header`) are `build`'s, as feature 974 says of
  bindings.

## Item 6: the shapes beside it, depth one

With `heroes-next` (the landed compiler), in `build/ce/shapes/`, each a program printing `true`
unless said:

| shape | result |
|---|---|
| `sys/cdefs.h` in a subfolder of the program's | exit 1, one `shadowed_header` naming `<sys/cdefs.h>` |
| `stdbool.h` beside the program as a **symlink** to a file elsewhere | exit 1, one `shadowed_header` |
| a folder named **twice** (`--include inc --include inc`) holding `stdbool.h` | exit 1, **one** diagnostic |
| a header reached only under a switch: a group's `sw.h` defining `__STDC_WANT_LIB_EXT1__`, and an `#error` file at `sys/_types/_rsize_t.h` | build 0, prints `true` and `3`: **the file was never read** (that switch does not reach past `stddef.h`'s guard here), so this settles that switch and not the shape |
| **a library header reached only through a group's header**: `extern "stdio.h"` binding `BUFSIZ`, beside a `_stdio.h` of the program's (`#include_next <_stdio.h>`, `#undef BUFSIZ`, `#define BUFSIZ 7`) | **build 0, prints 7, exit 0**; this SDK's `_stdio.h:203` says 1024 |

**The last is a real defect of the refusal's own cause**, beside it at depth one: a file of a named
folder read in the C library's place, here `_stdio.h`, which the program never wrote and which the
SDK's `stdio.h:61` includes. The refusal lists only the opening a program with no group reads
(`shadowed.hero:19-36`, *the lines a unit opens with where its program binds nothing*), so any
library header that only a group's header reaches is outside it, switch or no switch. Its
reproducer is `build/ce/shapes/viagroup/` (two files above). It reaches a wrong FFI constant, not
a §1.12 abort, because the guard and the reserved pair give back what the unit's own C takes; I put
it to the coordinator to file, `blocking` by `.claude/rules/verification.md` § Bounded discovery's
*a wrong value*. A route to it that I did **not** build: list the unit's own opening with its groups
(which `compiling.unit_reach` already preprocesses every build) with `-H`, which prints who
included each file, and refuse a named folder's file only where a platform header or the runtime's
included it; `-M` alone cannot tell `_stdio.h` (the SDK's `stdio.h` included it) from a group's own
`money.h` (the unit included it).

## Question 2 on Linux and Windows

- **Linux**, measured in the `heroes-linux-arm64:latest` container (Debian clang 22.1.8), the
  runtime mounted read-only and the opening listed with `-M` under the unit's words: **38 names**,
  among them `features.h`, `features-time64.h`, `stdc-predef.h`, `gnu/stubs.h`, `sys/cdefs.h` and
  twenty `bits/*.h`. Tracked files bearing one: **0** (`git ls-files` against the list).
  `features.h` is a name a program could give a header of its own. But a program holding it in a
  named folder is **already broken without the refusal**: glibc's `<stdint.h>` includes
  `<features.h>` by angle, so the program's file is read in glibc's place on every unit. So the
  refusal never refuses a program whose build did not already read the file in the library's place;
  what it can refuse is a deliberate stand-in (a gnulib-style replacement header), which is the
  thing it exists to refuse.
- **Windows**: unrun in this seat. The opening there reads the UCRT's and MSVC's headers
  (`shadowed.hero:284-286` names the four common to every platform and says the Windows C library
  has no `sys/cdefs.h`); which other names, and whether one is a name a program would use, is a
  question for the box.

## The other questions, from the cost side only

- **Question 3, the class.** One code is cheapest and true: the environment's folders are the same
  cause (a folder searched before the system's), so they belong to `shadowed_header` with a fourth
  origin in its note, not a code of their own. A package's `-D` of a kept name stays `ffi_package`:
  it is refused where the package's answer is read (`cli/package_words.hero`), a different place
  and a different repair (the `.pc`, not a file). `CCC_OVERRIDE_OPTIONS` needs no code if it is
  set to `#` for clang; it is the debugging knob of the tool under the compiler, not a program's.
- **Question 7, item 3.** No compiler file moves under any arm: it is spec text, and the
  `special` and `spec` suites read it (`.claude/rules/verification.md`'s map, `spec/heroes-spec.md`
  row). What arm C moves in those suites is the critic's measurement, not re-run by this seat.
- **Question 8, design.md §4.19's text**, the cost side of it: whatever §4.19 says about the
  environment has to keep its own sentence *the search path is the machine's own `CPATH`* true, so
  *counted as a folder this build names* fits and *cleared* or *refused* does not.

## Does it fit one person

Yes, as driver code. The landing is one module of 214 code lines by the layout suite's count
(`shadowed.hero`), one of 101 lines (`package_macros.hero`), two runtime headers of 57 code lines
each, and one call; nothing in the lexer, the parser, the checker, the descriptors, ownership or
the emitter. The tree it lands in is 193,347 lines of `selfhost/`, far past Pascal-P4's 4,000, so
§1.1's ceiling is read here as the marginal cost: one module a reader can hold, under its file
ceiling (214 of 300). The pressure point is not this module but its call site: **`produce.hero`
reads 300 code lines against a ceiling of 300** (`suite_layout.hero:45`), so any further line
there, a switch, an environment rule, moves code out first; and `toolchain.hero` reads 298, so the
`CCC_OVERRIDE_OPTIONS` line needs a small module of its own.

## Verdict

- **verdict**: **object** (no veto: no core construct, no ceiling breached).
- **section**: design.md §1.12 (*a boundary that is complete and defended*; *the compiler and the
  runtime check rather than assume*), with §4.19's *the search path is the machine's own `CPATH`*;
  the no-veto rests on §1.7 and Part 5 (nothing enters the type checker, the lowering or the
  backend).
- **implementation_cost**: landed, `selfhost/` +1,004 / -198 and `runtime/` +183 / -2 (`git show
  --numstat --format= 96d42c250`), all in `selfhost/cli/` and `selfhost/emit/`'s name lists and the
  runtime's two headers: `cli/shadowed.hero` 345 lines (214 by the layout count, 136 of code alone),
  `cli/package_macros.hero` 101, `emit/guarded_names.hero` +63, `emit/c_words.hero` 274 moved, not
  new. At run time **one clang process a build**: +85.0 M instructions in the compiler's process and
  +158.8 M in clang, about **+244 M, +17.4 % of a warm two-line build and +13.3 % with one group,
  at most 0.24 % of a warm build of the compiler**. My conditions: the environment counted, 25
  lines built and about 15 of words unbuilt, in `cli/shadowed.hero`; `CCC_OVERRIDE_OPTIONS` set to
  `#`, about 15 lines in a new `cli/` module; no added process. R1 would be +16 / -2 in
  `cli/compiling.hero`; R2 those and +6 / -1 in `emit/c_text.hero`, plus every emission with a
  group re-blessed.
- **needed_for_self_hosting**: no (Principle 0's other ticket: robustness, §1.12, a wrong value at
  exit 0).
- **argument** (119 words): Neither item adds a core construct: both live in `selfhost/cli/` and
  change no emitted byte of an accepted program, and item 3 moves no compiler file, so no veto. I
  object on §1.12: the refusal states a class, a file read in the C library's place, and closes two
  of its doors. Measured with the landed compiler, each at exit 0: `CPATH` or `C_INCLUDE_PATH`
  naming a `stdbool.h` prints `false`; an empty `CPATH` searches the current directory, same
  result; `CCC_OVERRIDE_OPTIONS='#+-D...'` gives a wrong sum and prints nothing; a group binding
  `stdio.h` reads the program's `_stdio.h`, `BUFSIZ` 7. The reorders are worse: `-idirafter`
  refuses a program's own `util.h` with a false message, `-iquote` a sibling include. Keep the
  refusal; widen it.
- **prediction**: if the resolution adopts the conditions below, at M-package-manager's close
  `selfhost/cli/shadowed.hero` reads **at most 260 code lines** by `tests/harness/suite_layout.hero`'s
  `code_lines` (214 at `163912031`), and a warm build of `build/ce/two/main.hero` under the counting
  `clang` (`build/ce/bin/clang`) starts **exactly 5 clangs**, as it does today: the environment and
  `CCC_OVERRIDE_OPTIONS` close in the driver, in lines, with no process added. Falsified by either
  number.
- **condition**: I move to **approve** when the resolution (a) counts the folders of `CPATH` and
  `C_INCLUDE_PATH` as named, an empty `CPATH` element as the current directory, with a note that
  names the variable; (b) sets `CCC_OVERRIDE_OPTIONS` to `#` for every clang the compiler starts;
  (c) sends the `_stdio.h` shape (`build/ce/shapes/viagroup/`) to be filed as a defect with its
  reproducer; and (d) holds the refusal with a folder case whose stand-in is harmful (`true` as 0),
  since `special`'s two programs panic correctly with the refusal taken out and so pin its message,
  not its class. I move toward a reorder route only on a measured correct program the refusal
  refuses and R1 or R2 builds: none in this tree (0 of its files bear one of the 36 macOS or 38
  Linux names), none in the matrix.

## What I left, and where

Nothing committed; nothing written outside this worktree. In it, untracked (`git status --short`): `build-stage2.log`, the stage-2 build's `time -l`;
the variant compiler `heroes-p237` and its sources `build/ce/variant/selfhost/` (four edits, above); the counting clang
`build/ce/bin/clang` and its logs `build/ce/logs/`; the matrix `build/ce/matrix.sh` and
`build/ce/routes/`; the shapes `build/ce/shapes/`; the environment programs `build/ce/boolenv/`,
`cwdroot/`, `envown/`, `cccsilent/`; the folder case
`tests/golden/unsupported/fixedbugs-2333-a-file-beside-the-program-named-like-a-library-header-is-refused.case/`
(untracked). No paid run was made or is proposed. The harness refused one of my runs, rightly: a
harness judged by `heroes-p237`, a compiler built from other sources than the tree's
(`.claude/hooks/provenance.py`), so 2333's seven cases under the routes were run by hand
(`build/ce/cases2333.sh`), each compared byte for byte with its `.expected`.

## Routes built for 2343 and 2344

Asked by the coordinator after the critic's second pass (`237-reports/critic-reports.md`,
findings 3 to 6, and its *must not adopt* items 2 to 4), on defects 2343 and 2344 (`gh issue view`).
Written 2026-11-08 in the same worktree, nothing committed. The critic's `viaown` and `cipempty` and
the ffi-pragmatist's readline program (`237-ffi-pragmatist/build/ffi/rlh/rel/main.hero`) were read
there and their sources copied into `build/ce/m2/`; nothing was built or written in another seat's
worktree.

### The second variant compiler, `heroes-p237b`

A copy of the first variant (`build/ce/variant/selfhost/`, so `P237_SHADOW`, `P237_ORDER` and the
count rule `P237_ENV=count` stay for comparison) at `build/ce/variant2/selfhost/`, with two routes
behind two switches. `heroes-next check` of it exits 0.

**Defect 2344, `P237_ENV=asked`: read the `-###` answer the build already has** (the critic's
route, finding 3).

- `cli/toolchain.hero`: the `Toolchain` record keeps the answer's text, `asked_answer`, which
  `toolchain_at` already reads once a build from `clang_floor.identity` (`clang -### <the
  compiler's own words> -c runtime/runtime.c`) and until now kept only as a digest (+4 / -1 by
  `diff`).
- `cli/clang_added.hero`, new (164 lines, 100 of code without comments and its test): the words of
  the answer's `"-cc1"` job, unquoted as clang writes them; the folders it searches for `<...>`
  ahead of the platform's (`-I`, joined or apart, `-isystem`, `-c-isystem`), every one of which the
  compiler did not pass, since the `-###` question carries no `-I`; clang's own folders (every
  `-internal-isystem` and `-internal-externc-isystem`); and every `-D` or `-U` of a name the guard
  keeps that is not the compiler's own switch (`package_macros.refused`, item 2's rule). Measured
  first: a plain answer holds exactly `-D _USE_MATH_DEFINES` and `-D _CRT_SECURE_NO_WARNINGS` on
  this Mac and in the Linux container (Debian clang 22.1.8), so any other `-D` or `-U` came from
  outside the compiler.
- `cli/shadowed.hero`'s `told`: each such folder is counted among the named folders, unless it is
  also one of clang's own (a tie goes to the platform, the landed rule: `CPATH` naming the SDK's
  own folder must build); each kept `-D` or `-U` is refused, code `shadowed_macro` (the prototype's
  name; question 3 is the sitting's), its note naming the variable. The variable is named by
  matching the folder against the variables' values (`.` against an empty element), for the words
  only: the verdict is clang's answer, whoever added the word. About 28 code lines.
- Every other word of the answer goes through untouched: nothing reads it.

**Defect 2343, `P237_GROUPS=on`: who included each file, and whether the platform has one of that
name.**

- The refusal's one `clang -v -M` run gains `-H` (clang prints each file it enters with its depth
  in dots, the same paths as `-M`; measured in one run with `-v -M`) and `-MG` (a group's missing
  header does not cost the listing), and it lists the runtime's opening **followed by** the
  program's groups inside the guard (`order_trial.opening([])`, then
  `macro_guard.around(c_text.includes(order_trial.theirs(headers)))`), so a library header the
  runtime reads is entered first beneath the runtime, and one only a group reaches is entered
  beneath that group. `cli/produce.hero` hands it the program's group headers
  (`emit_externs.headers`, +1 / -1).
- The rule, for each file of a named folder in that tree: **the unit's own include** (depth 1, the
  header a group names) **is the program's**, never refused; a nested file is refused where **the
  header that included it is not a named folder's** (the platform's or the runtime's: `_stdio.h`
  beneath the SDK's `stdio.h`, the landed 36 beneath the runtime), or where **a platform folder
  holds a file of the same name** (`process.path_exists`: `stdio.h` beneath the program's own
  `lib.h`). A file `-M` lists and `-H` does not (found only by `__has_include`) keeps the landed
  rule. About 89 code lines (`grouped_text`, `Entry`, `entries_of`, `tree_shadows`, `includer`,
  `twin`, `finder`, the reader in the message).
- The message names the header that included the file where it is not the runtime's: *which
  `lib.h` includes*, not *which the C this compiler writes reads*.

Built three times, each `heroes-next check` exit 0 and `heroes-next build ... -o heroes-p237b`
exit 0: the routes as described; then `-include` and `-imacros` added to the `-###` route, after
the first matrix found that door open in every arm (below); then the quoted-sibling exemption added
to the tree route, after a case of my own found it refusing a correct program (below). **Every
figure below is the third build's**, `heroes-p237b --version`: `sources 222d4bf371b8e243`.

Two more facts measured on the way:

- `-MG` keeps the one run going where a group's header is missing: `clang -v -H -M -MG` over a file
  including `<p237_missing.h>` exits 0 and lists the name bare (the route skips a path where no file
  stands, `process.path_exists`); without `-MG`, exit 1.
- **The Linux container reads the environment differently from this Mac, which is the case for
  reading clang rather than re-deriving it.** Debian clang 22.1.8's `-###` (`docker run
  heroes-linux-arm64:latest`, the runtime mounted read-only): `CPATH=/e` is `-I/e` and
  `C_INCLUDE_PATH=:` is two `-c-isystem .`, as on this Mac, and `CCC_OVERRIDE_OPTIONS` reaches it as
  `-D P=5` and `-include /f.h`; but **an empty `CPATH` adds nothing there**, where Apple clang
  21.0.0 adds `-I.`. Its plain answer holds no `-I/usr/local/include` (that folder is an
  `-internal-isystem` there). The route reads each clang's own answer; the count rule would have had
  to be right about both.

### Defect 2344: the environment matrix

`build/ce/envmatrix.sh`, each case built and run under three arms: **landed** (`heroes-next`),
**count** (`heroes-p237b`, `P237_ENV=count`, my first variant's rule) and **asked**
(`P237_ENV=asked`, the `-###` route). Programs in `build/ce/m2/`; `bool` prints `true`, `ccc` is
the overflow program run as `./p one`. A folder holds `stdbool.h` defining `true` as 0;
`force.h` defines `__STDBOOL_H`, `bool`, `true` and `false` (as 0).

| case | landed | count | asked |
|---|---|---|---|
| `CPATH=<folder>` | 0, **`false`** | exit 1 `shadowed_header` | exit 1 `shadowed_header` |
| `C_INCLUDE_PATH=<folder>` | 0, **`false`** | exit 1 | exit 1 |
| `CPATH=''`, the critic's `cipempty` | 0, **`false`** | exit 1 | exit 1 |
| `C_INCLUDE_PATH=':'`, `cipempty` | 0, **`false`** | 0, **`false`** | exit 1 `shadowed_header` |
| `C_INCLUDE_PATH='/x:'`, `cipempty` | 0, **`false`** | 0, **`false`** | exit 1 `shadowed_header` |
| `CCC_OVERRIDE_OPTIONS='#+-D__builtin_add_overflow(...)'` | 0, **`-9223372036854775808`** | 0, **wrong sum** | exit 1 `shadowed_macro` |
| the same without the `#` | 0, **wrong sum** | 0, **wrong sum** | exit 1 `shadowed_macro` |
| `'#+-Dtrue=0'` | 0, `true` | 0, `true` | exit 1 `shadowed_macro` |
| `'#+-I<folder>'` | 0, **`false`** | 0, **`false`** | exit 1 `shadowed_header` |
| `'#+-include<force.h>'` | 0, **`false`** | 0, **`false`** | exit 1 `shadowed_header` |
| `'#+-imacros<force.h>'` | 0, **`false`** | 0, **`false`** | exit 1 `shadowed_header` |
| `'#+-fuse-ld=lld'`, `lld@22`'s `bin` first on `PATH` (measurement 055's use) | exit 2, *linking failed* | exit 2, the same | exit 2, the same |
| `'#+-fuse-ld=ld'` | 0, `true` | 0, `true` | 0, `true` |
| `'#+-Wl,-dead_strip'` | 0, `true` | 0, `true` | 0, `true` |
| `envown`: `CPATH=hdr`, the program's own `mine2333.h` | 0, 2333 | 0, 2333 | 0, 2333 |
| the ffi-pragmatist's readline binding, `CPATH=/opt/homebrew/opt/readline/include` | 0, **2051** | 0, 2051 | 0, 2051 |
| `CPATH=<the SDK's usr/include>` (a platform folder named) | 0, `true` | 0, `true` | 0, `true` |
| nothing set | 0, `true` | 0, `true` | 0, `true` |

- **The `-###` route closes every case of the critic's list** (its *must not adopt* item 2) and
  three it does not list (`-I`, `-include`, `-imacros` through `CCC_OVERRIDE_OPTIONS`), refuses
  none of the four correct programs, and passes every other edit untouched.
- **Measurement 055's `-fuse-ld=lld` cannot link on this Mac whatever the compiler does**: the
  keg-only `lld@22` (22.1.8) says *could not load TAPI file ... libSystem.tbd: malformed file*, and
  plain `clang t.o -o t` under the same override fails the same way. So the row says what can be
  said: every arm gives the same exit 2 with lld's own words, the route adds no refusal, and the
  two linker edits this Mac can link (`-fuse-ld=ld`, `-Wl,-dead_strip`) build and run in every arm.
- **`'#+-Dtrue=0'` is refused although harmless today** (the guard gives `true` back, so the landed
  compiler prints `true`): the route refuses a `-D` of a kept name whatever it does, as item 2's
  rule refuses a package's. Strict, and the same line as item 2.
- **The count rule leaves seven cases at a wrong value at exit 0**: the critic's two
  `C_INCLUDE_PATH` empty elements (finding 4, reproduced) and the five `CCC_OVERRIDE_OPTIONS` cases
  that change a value (the `-D` with and without `#`, `-I`, `-include`, `-imacros`), since it reads
  only two variables; three of the seven are on the critic's list.
- The words, as the route prints them (`cipempty`, `C_INCLUDE_PATH=':'`): *`stdbool.h` stands in
  for the C library's `<stdbool.h>` ...* and the note *rename the file, or move it out of `.`:
  `C_INCLUDE_PATH` names that folder for every clang this build runs, an empty element of it naming
  `.`, the folder clang runs in*; for the `-D`, *`-D__builtin_add_overflow(a,b,r)=...` reaches every
  clang this build runs through `CCC_OVERRIDE_OPTIONS`, and `__builtin_add_overflow` is a name the
  C this compiler writes*, note *take `-D__builtin_add_overflow` out of `CCC_OVERRIDE_OPTIONS`, or
  build with `CCC_OVERRIDE_OPTIONS` unset*. The variable is named by matching the folder against the
  variables' values; a folder no variable holds is named *clang's own default search* (Apple
  clang's `-I/usr/local/include`, absent here, so how a file found there is told is **unrun**, as
  the critic said).

**Correction to my condition (b) above, 2026-11-08**: *sets `CCC_OVERRIDE_OPTIONS` to `#` for every
clang* is withdrawn. The critic's finding 5 is right that it silently drops an edit this repository
has used, and the `-###` route measured here tells instead, passes measurement 055's edit, and
closes the `-I`, `-include` and `-imacros` doors the `#` would also have closed only by discarding
every edit.

### Defect 2343: the group matrix

`build/ce/groupmatrix.sh`, three arms: **landed**, **groups** (`P237_GROUPS=on`), **both**
(`P237_GROUPS=on P237_ENV=asked`).

| case | landed | groups | both |
|---|---|---|---|
| `viagroup` (`_stdio.h` beside a group binding `stdio.h`'s `BUFSIZ`) | 0, **7** | exit 1 `shadowed_header` | exit 1 |
| the critic's `viaown`, evil (`lib.h` includes `<stdio.h>`, the program's `stdio.h` says 7) | 0, **7 14** | exit 1 `shadowed_header` | exit 1 |
| `viaown`, clean | 0, 1024 2048 | 0, 1024 2048 | 0, 1024 2048 |
| `utilown` (the program's own `util.h`, an SDK name, named by its group) | 0, 7 | 0, 7 | 0, 7 |
| `stdiogroup` (a group naming `stdio.h`, the program's `stdio.h` says 99) | 0, 99 | 0, 99 | 0, 99 |
| `sibling` (`money.h` includes its own `<money_types.h>`) | 0, 125000 | 0, 125000 | 0, 125000 |
| `sibutil-quoted` (`money.h` includes its own `"util.h"`) | 0, 125000 | 0, 125000 | 0, 125000 |
| `sibutil-angled` (`money.h` includes its own `<util.h>`) | 0, 125000 | **exit 1 `shadowed_header`** | exit 1 |
| `boolgroup` (a group naming `stdbool.h`, the program's `stdbool.h` says `true` is 0) | exit 1 | exit 1 | exit 1 |
| `beside`, `included`, `boolbeside`, `sub/sys/cdefs.h`, a symlink, a folder named twice | exit 1 each | exit 1 each | exit 1 each |
| `own` (`--include` folder, `mine2333.h`) | 0, 2333 | 0, 2333 | 0, 2333 |
| readline through `CPATH` | 0, 2051 | 0, 2051 | 0, 2051 |
| `group` (`math.h`, `sqrt`) | 0, 1.4142135623730951 | the same | the same |

- **The tree route closes both wrong values** (`viagroup` beneath the SDK's `stdio.h`, `viaown`
  beneath the program's own `lib.h` by the platform's twin) **and builds every correct program of
  the four** (`viaown` clean, `utilown`, `stdiogroup`, and `sibling`), with every landed refusal
  kept. The messages: *`./_stdio.h` stands in for the C library's `<_stdio.h>`, which
  `.../MacOSX.sdk/usr/include/stdio.h` includes*; *`./stdio.h` stands in for the C library's
  `<stdio.h>`, which `./lib.h` includes*.
- **`stdiogroup` prints 99 under this route, a ruling and not a measurement**: the file the group
  names, read for the group's own include, is the program's header, as `utilown`'s is; a rule that
  refused `stdiogroup` and built `utilown` would need a list of which names are the C library's, a
  premise about the world. And a group naming a header the runtime itself reads (`boolgroup`) is
  refused, because the runtime enters that name first.
- **What the route refuses that may be correct: `sibutil-angled`.** A program's own `money.h`
  writing `#include <util.h>` and meaning its own `util.h` is the same shape as the critic's
  `viaown` (`#include <stdio.h>`, answered from the program's folder), and no rule here tells them
  apart without that list of names. The quoted form, `#include "util.h"`, was refused by the first
  build of the route; C's rule (a quoted include asks the includer's folder first) makes it the
  header's own sibling, so the third build exempts a file found beside a named folder's includer
  that names it in quotes (`quoted_sibling`, read from the includer's text), and it builds.
- **The words differ in one case**: where the includer is the runtime's own header, the route says
  *which `runtime/heroes_standard.h` includes* (my folder case's `.expected`, its one differing
  line) where the landed sentence says *which the C this compiler writes reads*. A landing keeps the
  landed sentence for the runtime's includer.

### No correct program refused, with both routes on

`build/ce/sweep2.sh`, `P237_ENV=asked P237_GROUPS=on`: every `examples/*/main.hero` (55) and the 12
`run` goldens of item 3's subset (the 9 naming a package among them): **67 of 67 exit 0**,
`shadowed_header` or `shadowed_macro` in 0 outputs. 2333's seven cases (`build/ce/cases2333b.sh`):
the four `run` cases the same as their `.expected`, the three `unsupported` cases exit 1 and the
same; my folder case exit 1, differing in the includer's words alone (above).

### The cost, children counted

`build/ce/cost2.sh`, and for the count rule an inline loop of the same shape writing
`build/ce/cost3.tsv`: the four arms of one binary, warm; the
compiler's own instructions over 8 alternated rounds, the first discarded (7 kept), and the clangs
it starts through the counting `clang`, 3 builds per arm; medians, range in brackets for the own
process.

| program | arm | clang runs | own process | clangs | total | vs base |
|---|---|---|---|---|---|---|
| two-line | base | 5 | 723,754,558 | 898,117,678 | 1,621,872,236 | |
| | asked (2344) | 5 | +19,557,246 | +10,219,696 | 1,651,649,178 | **+1.84 %** |
| | groups (2343) | 5 | +144,240,822 | +6,667,219 | 1,772,780,277 | **+9.30 %** |
| | both | 5 | +174,039,626 | +6,211,830 | 1,802,123,692 | +11.11 % |
| one group, `math.h` | base | 7 | 871,460,584 | 1,174,565,676 | 2,046,026,260 | |
| | asked | 7 | +20,891,365 | +768,771 | 2,067,686,396 | +1.06 % |
| | groups | 7 | +158,577,794 | +9,315,721 | 2,213,919,775 | +8.21 % |
| | both | 7 | +189,626,526 | +5,364,293 | 2,241,017,079 | +9.53 % |
| one group, `stdio.h` | base | 7 | 870,231,365 | 1,190,479,054 | 2,060,710,419 | |
| | asked | 7 | +20,537,421 | +3,351,700 | 2,084,599,540 | +1.16 % |
| | groups | 7 | +259,218,865 | +9,716,148 | 2,329,645,432 | +13.05 % |
| | both | 7 | +297,889,935 | +22,501,449 | 2,381,101,803 | +15.55 % |

And the count rule against the same base, two-line and one group: own process -892,734 and
-200,485, clangs +5,465,970 and +119,137, 5 and 7 runs: nothing measurable.

- **Neither route adds a process**: 5 and 7 clang runs in every arm.
- **The `-###` route costs about +20 M in the compiler's own process** (reading the answer it
  already holds); its clang-side differences, +0.8 M to +10.2 M, lie inside the spread of identical
  builds (the base arm's own clang range on the two-line program is 895,084,124 to 907,355,335).
- **The tree route costs +144 M to +259 M in the compiler's own process**, growing with the headers
  a group reads, and about +7 M to +10 M in clang for `-H`. That is my prototype's walk of the tree
  (each entry's folder found against every searched directory, twice for a nested one, and the
  `-v`/`-H` text read byte by byte), **unprofiled and unoptimised**: an inference, not measured,
  is that a landing that finds each directory once per path, or asks `compiling.unit_reach`'s run
  instead (my report's item 1), costs far less. As built, it is the route's measured price.

### Size, and where it lands

| route | code lines (comments and tests out) | where |
|---|---|---|
| 2344, `-###` | about **147**: `cli/clang_added.hero` 108 (new; 177 lines with its comments and test, 133 by the layout rule), `shadowed.hero` about 38 (the block in `told`, `forced_diagnostic`, `macro_diagnostic`, the origin in the note), `toolchain.hero` 1 field | `selfhost/cli/` |
| 2343, tree | about **110** in `shadowed.hero` (`grouped_text`, `Entry`, `entries_of`, `tree_shadows`, `quoted_sibling`, `folder_of`, `includer`, `twin`, `finder`, the `-H -MG` words, the reader in the message) and 1 argument in `produce.hero` | `selfhost/cli/` |
| count rule, for comparison | 19 (25 with comments) | `shadowed.hero` |

Neither touches the lexer, the checker, the descriptors, ownership or the emitter: both are driver
code, no construct, no veto. But the variant's `shadowed.hero` stands at **409 lines by the layout
suite's rule against its ceiling of 300**, `toolchain.hero` at 301 (298 landed) and `produce.hero` at
304 (300 landed; the ceiling is `suite_layout.hero:45`), so a landing puts the tree route in a module of its own
beside `clang_added.hero` and moves a line out of each of the other two first.

### Summary

- **Defect 2344**: the **`-###` route** (`P237_ENV=asked`) closes every case: the critic's six
  wrong values (`CPATH=dir`, `C_INCLUDE_PATH=dir`, `CPATH=''`, `C_INCLUDE_PATH=':'` and `'/x:'`, a
  kept `-D` through `CCC_OVERRIDE_OPTIONS` under `#`), the same `-D` without `#`, and `-I`,
  `-include`, `-imacros` through the same variable, refusing none of `envown` (2333), the readline binding (2051), `CPATH` naming the
  SDK's folder, or the linker edits (`-fuse-ld=lld` gives the same lld failure in every arm, as
  plain clang does on this SDK; `-fuse-ld=ld` and `-Wl,-dead_strip` link). About **147 code
  lines** in `selfhost/cli/`, **no added process, about +20 M instructions, +1.1 % to +1.8 %** of a
  warm build. It fails no case run; it refuses `-Dtrue=0`, harmless today, by item 2's own line.
  The count rule (19 lines, no measurable cost) leaves seven of them at a wrong value, three of the
  critic's list among them.
- **Defect 2343**: the **tree route** (`P237_GROUPS=on`) closes `_stdio.h` and `viaown`, builds
  `utilown` (7), `viaown` clean (1024 2048) and `stdiogroup` (99, by the ruling that a group's named
  header is the program's), and keeps every landed refusal; 67 of 67 correct programs build with
  both routes on. About **110 code lines**, **no added process, +144 M to +259 M instructions, +8.2 %
  to +13.0 %** of a warm build as built (unoptimised). **The case it fails, if it is correct:
  `sibutil-angled`**, a program's header including its own `<util.h>` by angle brackets, refused
  because the SDK has a `util.h`; it is `viaown`'s shape, and telling them apart needs a list of the
  C library's header names, which no route here keeps. `stdiogroup`'s 99 is the sitting's ruling to
  confirm.

Built in `build/ce/variant2/selfhost/` (binary `heroes-p237b`; the first two builds kept as
`heroes-p237b-first` and `heroes-p237b-second`), cases in `build/ce/m2/`, scripts
`build/ce/envmatrix.sh`, `groupmatrix.sh`, `sweep2.sh`, `cases2333b.sh`, `cost2.sh`; nothing
committed, nothing written outside this worktree, no paid run.
