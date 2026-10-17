# Panel 224, compiler-engineer: route E built, and the ceiling of each route

2026-10-17, from 18:07. Lane `lane-land215`, HEAD `3c478500b`. Read in order: `00-shared.md`,
`224-reports/critic-briefs.md` (its findings taken as framing facts), `compiler-engineer.md`.
Everything below was run in my own copies: `$C` is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/224-compiler-engineer` (`git -C <lane>
archive 3c478500b | tar -x -C $C`, then an empty `git init` so the hooks judge it as its own tree),
`$O` is `$C/out` (until 18:27 these outputs were written to the session's scratchpad, outside
the repository's root, against CLAUDE.md's hard stop and the shared brief's rule; I moved them, a
rename on the same volume, `/dev/disk3s5`, while two runs were still writing into them),
and `/c/w/224-compiler-engineer` on the box (the same archive carried by `ssh` as a `.tar.gz`,
`build/`, `archive/` and `site/` left out). No paid run, no credential printed. Nothing written in
the lane but this file. **Status 18:56: complete; every row below was run, and what was not is said *unrun* where it
stands. § 10, appended 19:05 to 19:42 at the coordinator's request, repairs the patch after the
critic's second pass; the box's `run` form, read at 19:57, gives no verdict on the patch: the
box's disk filled under it, my directory holding 12G of the 100G.**

## The verdict

- `verdict`: **approve route E, and adopt it** (built, `$C/selfhost/cli/front_check.hero` and the
  `units.hero` call); **object** to A, B (as 2120's resolution), C, D, E', F and G; **no veto**,
  since no route adds a core construct or breaches the ceiling.
- `section`: design.md §1.1 (simplicity sets the ceiling), §1.7 and Part 5 (no core construct:
  nothing reaches the lexer, checker, IR or emitter); §1.12 (the object keeps its target's
  meaning); §4.17 (nothing promised). **The document does not cover `-fwrapv` or a driver's
  default flags at all** (`grep -n "wrapv\|signed overflow\|C leaves undefined" docs/design.md`
  prints nothing), so the objection to A stands on §1.12 and panel 218's ratified R3, not on a
  sentence about the flag.
- `implementation_cost`: `selfhost/cli/front_check.hero`, new, 63 lines by `wc -l` (about 25 of
  code, a test); `selfhost/cli/units.hero` 599 to 609. One extra clang front-end run per cold
  compile of a unit that holds the region, 0.31-0.35 s on the box's `windows.h` unit, about +0.03
  s on this Mac; nothing warm; no key moves for a program binding no header value.
- `needed_for_self_hosting`: **no** (the compiler binds no header constant C leaves undefined);
  it is a robustness repair of an existing class, not a form.
- `argument`: 2120 is not the platform-independent order: the same `-1 << 2` is a compile-stage
  refusal on four clangs and a probe-stage one on clang 23 with MS compatibility, because the
  driver adds `-fwrapv`. Which stage a mistake belongs to must not depend on the machine. Route E
  asks the front end once more under `-fno-wrapv`, only for units holding the region, and keeps
  the object under the target's words, so panel 218's R3 stands. Measured: 2072 green in one build
  on the box and here; `unsupported` 328/0 on the box, 344/0 here; 1720 own tests; fixpoint
  identical. A gets the same words by moving every key on four platforms and the meaning of every
  header's inline code on one.
- `prediction`: § 9.
- `condition`: § 7, row E: a UCRT or SDK header refused under E and accepted by the base, or E's
  cold cost past 10% of a program's cold build on the box.

## 0. Compilers

| where | command | result |
|---|---|---|
| Mac, stage 1 | `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` | exit 0 |
| Mac, base stage 2 | `./heroes build selfhost/main.hero -o heroes-next; cp heroes-next heroes-base` | exit 0, `sources 5f45d6514b22d650` |
| Mac, route E | `./heroes-base build selfhost/main.hero -o heroes-e` | exit 0, `real 33.34 user 32.60`, `sources 789ddedd621bdb7b` |
| box, stage 1 | `clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes.exe` | exit 0, `sources 5f45d6514b22d650`, `clang version 23.1.1` |
| box, base stage 2 | `./heroes.exe build selfhost/main.hero -o heroes-base.exe` | exit 0, `sources 5f45d6514b22d650` |
| box, route E | `./heroes-base.exe build selfhost/main.hero -o heroes-e.exe` | exit 0, `sources 789ddedd621bdb7b` |
| Mac, route A (for pricing, § 4) | `flags.hero` + `"-fno-wrapv"`, `./heroes-base build selfhost/main.hero -o heroes-a`, `flags.hero` restored | exit 0, `sources 09b5de1b7e6f282b` |

## 1. The patch (route E), as built

Two files, `$C/selfhost/cli/front_check.hero` (new, 63 lines by `wc -l`) and
`$C/selfhost/cli/units.hero` (599 to 609 by `wc -l`). **The patch** is
`$C/out/route-e.patch` (`diff -ruN` against `3c478500b`, `-p1`); `git -C <lane> apply --check
$C/out/route-e.patch` passes, nothing written in the lane.

- **Where it lands**: in `units.tu_object` (`selfhost/cli/units.hero:138` at `3c478500b`, 139 patched), after the cache check
  and after the unit's C is published, before the compile. A unit whose text holds the region's
  first error line (`#pragma clang diagnostic error "-Wshift-count-overflow"`, written by
  `emit/value_check.opened`, `selfhost/emit/value_check.hero:90-95`) is asked once by clang with
  its own words (the same response file) and `-fno-wrapv -fsyntax-only -fno-crash-diagnostics`
  after them, so `-fno-wrapv` wins over the driver's own `-fwrapv` (the critic's: the last word
  wins). A refusal there is returned exactly as a compile's refusal is (`failure_text` + `GENERATED`
  + the C file), so `whose.of_refusal` and `emit/ffi_constant_value.hero` read it unchanged. A pass
  goes on to the compile, under the target's own words: the object keeps the meaning its clang
  gives it, panel 218's R3 untouched.
- **Its key**: `front_check.key_text(tu_text)` is appended to the TU key (`units.hero:144`), `""`
  for a unit with no region, so **a program binding no header integer constant keys every unit as
  before**, and a unit with the region keys `|front:-fno-wrapv -fsyntax-only
  -fno-crash-diagnostics`, so an object compiled by a compiler without the check is never served
  to one with it (the TU key names VERSION, clang's identity, the text, the runtime and the words,
  `units.hero:142-143` at `3c478500b`, and not the Heroes compiler's sources).
- **No surface, no spec token, no design.md sentence, no new diagnostic class**: the words told
  are the region's own, clang's, which `.expected` already pins.

## 2. Measurements on this Mac (Apple clang 21), route E against the base

| what | command (in `$C`, or `$O/shapes`, `$O/time`) | base | route E |
|---|---|---|---|
| the 2072 case | `./heroes-e run tests/harness/main.hero -- ./heroes-e unsupported fixedbugs-2072 > $O/mac-e-2072.txt` | 1 passed, 0 failed (the critic's) | **1 passed, 0 failed** |
| the compiler's own tests | `./heroes-e test selfhost/main.hero > $O/mac-e-selftest.txt` | | **1720 tests, all passed**, exit 0 (the new test among them) |
| `unsupported` whole | `./heroes-e run tests/harness/main.hero -- ./heroes-e unsupported` | | **344 passed, 0 failed** (two `fixedbugs-2053` cases skipped as Windows-only) |
| `layout` | same, `layout` | | **6 passed, 0 failed** (`units.hero` at 609 by `wc -l` stays inside its allowance) |
| `cache`, alone | same, `cache` | | **7 passed, 0 failed** |
| `run` whole | same, `run` (18:18 to 18:44, beside another session's `determinism` run) | | **534 passed, 0 failed** |
| built by the seed's own stage 1 | `./heroes build selfhost/main.hero -o heroes-e1` | | exit 0, `sources 789ddedd621bdb7b`: no new form, so no seed refresh (M-bounded-seed's rhythm) |
| the fixpoint | `./heroes-e1 build selfhost/main.hero --emit-c > out/e-by-e1.c`, the same with `./heroes-e`, `cmp` | | **identical**, 26,249,379 bytes |

**The critic's Mac shapes** (copied to `$O/shapes`, never run in the critic's folder; each built
from an empty `build/` with `heroes-<base|e> build <p>.hero`), with `pair.hero` added, the box's two
values alone (`-1 << 2`, `1 << 31`):

| program | binds | base told | route E told |
|---|---|---|---|
| `both.hero` | `SHIFT_PAST`, `DIV_MIN`, `FCAST` | `SHIFT_PAST` alone, exit 1 | `SHIFT_PAST` alone, exit 1 |
| `probe_only.hero` | `DIV_MIN`, `FCAST` | both, exit 1 | both, exit 1 |
| `mm/` | `SHIFT_PAST` in `main`, `DIV_MIN` in `other` | `SHIFT_PAST` alone | `SHIFT_PAST` alone |
| `missing.hero` | a name the header lacks, `DIV_MIN` | the missing name alone | the missing name alone |
| `pair.hero` | `SHIFT_NEGATIVE`, `SHIFT_INTO_SIGN` | both, region words | both, region words |

So **route E moves nothing on this Mac**, as built to: it answers the `-fwrapv` pair, and the
platform-independent order the critic found (a value only the probe sees waits for every unit's
compile) is untouched (§ 4).

**Build cost** (`/usr/bin/time -p heroes-<c> build <p>.hero`, `rm -rf build` before each cold
pair, base and E interleaved, three repetitions each; the machine was not still, another
session's `race_atomic` holding about 1.5 cores, load 3.65 to 4.63, so the ratio is read: `real`
sits about 0.2 s over `user + sys` on the cold builds that bind a header value, base and E alike,
and level with it on the rest, so I read no row as waiting on the other session; raw rows in
`$O/time/timings.txt`):

| program | base cold `real` / `user` | E cold `real` / `user` | base warm `real` | E warm `real` |
|---|---|---|---|---|
| `none.hero`, binds no header value | 0.29-0.31 / 0.21-0.22 | 0.29 / 0.21-0.22 | 0.09-0.10 | 0.10 |
| `one.hero`, one constant of a 5-line header | 0.81-0.83 / 0.44-0.45 | 0.84-0.91 / 0.45-0.46 | 0.12 | 0.12 |
| `stdio.hero`, `BUFSIZ` of `<stdio.h>` | 0.83-0.86 / 0.46-0.50 | 0.86-0.89 / 0.47-0.50 | 0.12-0.13 | 0.12-0.13 |

**A program binding no header value does not move**, and neither do its keys, measured: in one
`build/` (`out/keys`), `heroes-base build none.hero` then `heroes-e build none.hero` leaves 2
`tu-` directories and 1 runtime object after either, **0 new**; the same for `one.hero` leaves 2
then 3, **1 new**, the region's unit alone, the runtime's object reused. One that binds one pays a second front-end run of the
unit on a cold build, about +0.03 s `real` here, and nothing warm.

## 3. On the box (clang 23.1.1, `x86_64-pc-windows-msvc`)

In `/c/w/224-compiler-engineer`; every run by `ssh -n win '... > <file> 2>&1 < /dev/null'` and the
file read back (two first attempts with the output piped through `tail` inside `ssh` hung with no
`heroes.exe` running on the box and were ended by me; nothing of theirs is used).

| what | command | result |
|---|---|---|
| base stage 2 | `./heroes.exe build selfhost/main.hero -o heroes-base.exe` | `sources 5f45d6514b22d650` |
| **measurement 1**, base | `./heroes-base.exe run tests/harness/main.hero -- ./heroes-base.exe unsupported fixedbugs-2072` | **`0 passed, 1 failed`**, exit 1; the actual output holds four of the six, lacking exactly `SHIFT_NEGATIVE` and `SHIFT_INTO_SIGN` (the critic's reading, reproduced) |
| route E stage 2 | `./heroes-base.exe build selfhost/main.hero -o heroes-e.exe` (the two files carried by `tar`) | exit 0, `sources 789ddedd621bdb7b`, the same key as this Mac's `heroes-e` |
| `unsupported` whole, base | `./heroes-base.exe run tests/harness/main.hero -- ./heroes-base.exe unsupported` (beside route E's run from 18:27, 2 processors) | `327 passed, 1 failed`, the failure `fixedbugs-2072`, 18 skipped by name |
| **the 2072 case, route E** | `./heroes-e.exe run tests/harness/main.hero -- ./heroes-e.exe unsupported fixedbugs-2072` | **`1 passed, 0 failed`**, exit 0: all six told in one build, in the `.expected`'s words |
| `unsupported` whole, route E | `./heroes-e.exe run tests/harness/main.hero -- ./heroes-e.exe unsupported` (beside the base's run, 2 processors) | **`328 passed, 0 failed`**, exit 0, the same 18 skipped by name |
| a `windows.h` constant through route E | `heroes-<base|e>.exe build win.hero` (`constant MAX_PATH: i64` of `windows.h`), then run it | both build, exit 0, both print `260`: E's `-fno-wrapv` front end refuses nothing of `windows.h` for this unit |
| the compiler's own tests, route E, first run | `./heroes-e.exe test selfhost/main.hero` | `1720 tests, 1 failed`: `"the key made in C is the key made in Heroes, for a file of every shape"`, `panic: ... no file at site/public/favicon.ico`. **My copy's fault**: the test reads that file (`selfhost/module/reading.hero:341`) and I left `site/` out of the box's copy. A base run to compare was started and ended by me (`taskkill` by image name; only my directory holds a `heroes-base.exe`, `ls /c/w/*/heroes-base.exe /c/w/*/*/heroes-base.exe`) |
| the same, `site/public/favicon.ico` brought | the same | **`1720 tests, all passed`**, exit 0 |

**The shapes on the box** (`progs/shapes`, each from an empty `build/`, `heroes-<base|e>.exe build
<p>.hero`; output in `$O/box-shapes.txt`):

| program | base told | route E told |
|---|---|---|
| `both.hero` | `SHIFT_PAST` alone, exit 1 | `SHIFT_PAST` alone, exit 1 |
| `probe_only.hero` | `DIV_MIN`, `FCAST` | `DIV_MIN`, `FCAST` |
| `mm/` | `SHIFT_PAST` alone | `SHIFT_PAST` alone |
| `missing.hero` | `ffi_unknown_name` alone | `ffi_unknown_name` alone |
| `pair.hero` (`-1 << 2`, `1 << 31` alone) | both, **in the probe's words** (*its expression holds an operation C leaves undefined ... traps where it reads it*) | both, **in the region's words** (*shifting a negative signed value is undefined*; *signed shift result (0x80000000) sets the sign bit ...*), byte for byte the words this Mac prints (`cmp` of the two `e-pair.out`, 1245 bytes each, identical) |

**Cost on the box.** Git Bash's `time` reads no `user` for a native Windows process, so only
`real` is read (`out/box-timings.txt`, `TIMEFORMAT`, three repetitions, base and E interleaved, the
box quiet of my runs; other seats may have run, unread). The spread is wider than the cost:

| program | base cold `real` | E cold `real` | base warm | E warm |
|---|---|---|---|---|
| `none.hero` | 1.33-1.68 | 1.22-1.36 | 0.41-0.72 | 0.40-0.44 |
| `one.hero` | 2.21-3.09 | 1.97-2.27 | 0.53-0.57 | 0.39-0.50 |
| `win.hero` (`windows.h`) | 13.31-15.95 | 12.45-15.75 | 0.73-0.90 | 0.68-1.00 |

So E's own step was timed alone, on the `windows.h` unit route E compiled
(`progs/time/build/tu-0f334bc20cd4328f/win.c`, its words `build/words/words-5afd024315ae3786.rsp`),
five alternating runs: `clang @words -fno-wrapv -fsyntax-only -fno-crash-diagnostics win.c`
**0.31-0.35 s**, `clang @words -c ... -o x.o` **0.33-0.38 s** (`out/box-front-cost.txt`). **Route
E about doubles the clang time of the one unit that holds the region, on a cold build only: on
`windows.h` about 0.33 s of a 12 to 16 s cold build, 2 to 3%.**

So route E does two things on the box and nothing else measured: the two values are told in the
first build beside the other four, and a program binding them alone reads the same words on the
box as on this Mac. The platform-independent order is the same on both machines, under both
compilers.

## 4. What the routes cost, and where it lands

Every cost below is in `selfhost/cli/` or the harness: **no route touches the lexer, the parser,
the checker, the IR or the emitter's output** except E' and F, which touch the emitter
(`selfhost/emit/value_check.hero`, 123 lines; `emit/constant_accessor.hero`). None adds a core
construct (design.md Part 5): this is build plumbing, and the question is lines, keys and words.

| route | lines and modules | emitted C | cache keys | suites owed | words told for the box's two |
|---|---|---|---|---|---|
| **A** `-fno-wrapv` in `flags()` | 1 word in `selfhost/cli/flags.hero:130-151` (493 lines) | unchanged | **every** key on **every** platform: each unit's (`units.hero:143` names the words), the runtime's (`level_words` opens with `flags()`, `flags.hero:183-194`; `toolchain.hero:214`), every header probe's (`compiling.probe_flags`, `compiling.hero:142-143`), the value probe's (`value_ask.hero:104` names the unit's words) | `run` whole, `cache` alone, `selfhost/**` (`.claude/rules/verification.md`'s map) | the region's, clang's |
| **B** the probe asked where the region refused | unbuilt; by reading, 60 to 100 lines over `assemble.hero` (322), `rounds.hero` (268), `value_ask.hero` (133), `value_told.hero` (352): a region-only refusal read as such; the probe's findings carried in `Round` and told only by the final round, since a refused round runs again when a tag is learned (`rounds.hero:87-108`); a new dedupe, since nothing in the probe path knows which constants the region refused (`value_told.hero:212-214` dedupes C2 against panel 216's probe only) | unchanged | none | `unsupported`, `run`, the compiler's own tests | **the probe's**, which the case's one `.expected` (`suite_golden.hero:303`) does not hold; a skip is by platform (`:304`) and the CI's Windows leg (clang 20.1.8) reads the region's words, so no expectation the harness holds is right on both Windows machines |
| **C** left open | 0 | unchanged | none | none | the probe's, a build later |
| **D** a pragma | none exists in the vocabulary the critic searched (nine spellings, both clangs) | | | | |
| **E** front end under `-fno-wrapv` (built) | `cli/front_check.hero` new, 63 by `wc -l`, about 25 of code; `cli/units.hero` +10 (599 to 609) | unchanged | **only a unit holding the region**, once (`front_check.key_text`) | `unsupported`, `run`, `cache`, `layout`, `selfhost/**` | **the region's**, clang's, as `.expected` pins |
| **E'** the accessors in a unit of their own | unbuilt; the emitter's accessor and region, the unit plan (`assemble.hero`, `units.hero`), a second object per such module and its key; a header's inline code compiled under two meanings in one program | moves | a new unit per such module | `emit`, `emission`, `wholes`, `run`, `unsupported` | the region's |
| **F** the region's six as warnings, read back | unbuilt; `value_check.opened` writes `warning` for `error`, a reader in `tu_object`'s success path or `per_module`, the dedupe B needs | moves (the pragma) | every unit holding the region | `emit`, `emission`, `unsupported`, `run` | **none**: under `-fwrapv` the front end does not say the two at all, as a warning or an error (the critic's § 3), so F alone leaves 2120 open; with the probe after it, the probe's |
| **G** `-fno-ms-compatibility` | 1 word, Windows only | unchanged | every key on Windows | `run`, `cache`, the platform | the region's; and every SDK header read without the compatibility it is written for (unrun by me) |

**Route A's keys, measured on this Mac.** A compiler with `"-fno-wrapv"` appended to `flags()`
(`heroes-a`, built by `heroes-base`, `sources 09b5de1b7e6f282b`; `flags.hero` restored after the
build and `cmp`-checked against its copy): in one `build/` after the base, `none.hero` gains **2
new `tu-` directories of 2 and a second runtime object**, and so does `one.hero`; under route E
the same reads 0 and 1 new, the runtime reused (§ 2). On this Mac A tells `pair.hero` in the same
bytes as E (`cmp` of `a-pair.out` and `e-pair.out`). A compiler with A on the box is **unrun**;
that A tells all six there rests on the critic's reading of the real unit under `-fno-wrapv`.

**Route E's cheaper variant, priced and not taken**: the toolchain already asks `clang -###` with
the build's words once per build (`clang_floor.identity`, `selfhost/cli/clang_floor.hero:104-121`,
from `toolchain.hero:128-129`), so E could run only where that answer holds `"-fwrapv"`, a field of
`Toolchain` and about five lines, at no cost on this Mac or Linux. It is not taken: a `-fwrapv`
in a package's own words (a `.pc`'s `Cflags`, which reach a unit through `unit_words`,
`assemble.hero:130-134`) is not in that answer, and E as built covers it by construction, its
`-fno-wrapv` coming after every word of the unit (the critic measured `-fwrapv -fno-wrapv` telling
all six). An end-to-end package carrying `-fwrapv` is **unrun**.

## 5. Shapes beside route E (CLAUDE.md § RUN IT, attacked at the shapes next to it)

- **A unit with the region and another refusal** (a missing name beside the box's pair): E's
  front end says what the compile's front end would, plus the two: an **inference** from the
  critic's reading that `-fwrapv` takes those two and nothing else from the region's six
  (`ovf.c`), not a measurement over every diagnostic clang has. The reading of the refusal is
  unchanged (`whose.of_refusal`). Measured: `missing.hero` on this Mac reads the same under E.
- **A refused round run again for a learned tag** (`rounds.hero:87-108`): each round's unit has a
  new text, so E asks it again, one front end per round, as the compile does.
- **`--emit-c`**: runs the rounds (defect 298's correction in `.claude/rules/verification.md`),
  so E runs there too.
- **A record constant** (panel 221's warnings beside a static initialiser, `ffi_value.hero:137`)
  holds no region (`value_check.asked` excludes records, `value_check.hero:76-78`), so E does not
  read it; on the box its fields are panel 218's C2 probe's, which compiles with `-fno-wrapv`
  already (`value_run.hero:105-111`). Its instrument is the 2109 case, whose header holds
  `1 << 31` in a record's brace list (`R_FILL`, `tests/golden/unsupported/fixedbugs-2109-folded.h`):
  green on the box under the base and under E, both whole `unsupported` runs (§ 3).
- **What `-fno-wrapv` could refuse that the target's compile accepts**, a header's own code
  outside the region: `out/sa/sa.c`, a `_Static_assert((1 << 31) < 0, "")`, an `enum { E = 1 << 31
  }` and a `static const int k = (-1 << 2)`, `clang -std=gnu11 [-fno-wrapv] -fsyntax-only`: on
  this Mac nothing but one warning (`-Wshift-negative-value`) in either mode, exit 0; on the box
  nothing by default and that one warning under `-fno-wrapv`, exit 0. Outside the region the word
  adds warnings and no error, and E refuses only on clang's error, so these shapes pass. **Unrun**:
  a package whose own words carry `-Werror` (the warning would then be an error under E and not
  under the target's compile), and the UCRT and the SDK at large (the ffi-pragmatist's item 2);
  `windows.h` through E is in § 3.

## 6. What 2120 is, read from the code

The region and the probe are two stages of one build: the region is the unit's compile
(`units.tu_object`), the probe runs only once every unit compiled (`assemble.hero:212-213` returns
before `value_ask.told` at `:223`). design.md §4.17 (lines 2333 to 2345 at `3c478500b`) counts, and
does not promise against, "a mistake ... told only once another is fixed", and says "A later
stage's mistakes wait for the earlier stage's by design". That sentence is written of `heroes
check`'s stages; **the document does not name the build's probe**, so reading it onto the probe is
an analogy, mine. Under it, the critic's Mac shapes (`both`, `mm`, `missing`) are the by-design
order: a probe-stage value waits for a compile-stage refusal, on every target.

**2120 is not that.** `-1 << 2` and `1 << 31` are compile-stage mistakes on this Mac, on Linux and
on the CI's Windows clang 20.1.8, and probe-stage ones on clang 23 with MS compatibility, because
the driver's `-fwrapv` takes them out of the region. **Which stage a mistake belongs to depended on
the machine's clang**, and with it the build it is told in and its words (the box's `pair.hero`,
§ 3). Route E puts them back in the compile stage on every target. It promises nothing about
the order of stages, so it writes nothing into §4.17 or spec § 13, and the lane's condition holds.

## 7. Verdicts, per route

I hold the veto for a core construct or a breach of the ceiling (design.md §1.1, §1.7, Part 5).
**No route adds a core construct** (none reaches the lexer, the checker or the IR), **and none
breaches the ceiling**, so I exercise no veto. Where a route is unsound I object and say what
would make the objection a veto.

| route | verdict | stands on | the condition that would turn me |
|---|---|---|---|
| **A** | **object** | §1.12 and panel 218's R3 (ratified 2026-10-17, "never in the program's"): one word, but it moves every key on all four platforms and, on the one target that differs, takes the defined meaning `-fwrapv` gives a signed overflow away from the runtime's C and every header's inline code (the program's own arithmetic is checked and aborts, CLAUDE.md § 7, so it is the C around it that changes), to get back a diagnostic E gets back without touching the object. The critic measured clang `-O2` folding `(x * 2) / 2` to `x` under `-fno-wrapv` on the box, so the change is not only of words | a measurement that the object compiled under the box's `-fwrapv` is itself wrong for Heroes (a runtime or emitted-C path whose meaning changes under it), which E cannot reach |
| **B** | **object**, as 2120's resolution | §1.1 (the ceiling) and the harness: 60 to 100 lines across four modules, unbuilt, a new dedupe, the probe's findings carried through rounds, and the box's two told in words the case's one `.expected` cannot hold beside the CI's | a built B under 60 lines whose words for the two are the region's on the box; and its **part that probes the units that compiled when another refused** I would approve as the repair of the `mm` shape, filed apart (§ 8) |
| **C** | **object** | design.md §4.17 counts the build it costs; `platform_skips.hero` skips by platform and its own header refuses a skip by machine (the critic's § 2.5); nothing lets an open defect stop counting at a tag | none while E exists at its measured cost |
| **D** | **object**: no spelling exists in the vocabulary searched (the critic's nine, both clangs) | | a pragma or attribute that brings the two warnings back under `-fwrapv`, shown on the box |
| **E** | **approve, and adopt** | §1.1 (73 lines in `selfhost/cli/`, nothing in the core), §1.12 (the object keeps its target's meaning), §4.17 (nothing promised) | the box's whole `unsupported` form, or a header of the UCRT or the SDK, refused under E and accepted by the base (§ 3, § 5's last shape); or a cold-build cost on the box past 10% of a program's cold build |
| **E'** | **object** | §1.1: the emitter, the unit plan and the keys move, for the words E gets with none of that; a header's inline code compiled under two meanings in one program | none needed while E stands |
| **F** | **object**, as 2120's resolution | it does not tell the two at all under `-fwrapv` (the front end does not say them as warnings either) | none for 2120; for the `both` shape it is B's cousin and owes the same dedupe |
| **G** | **object**, and a veto if measured | §1.12: every Windows SDK header read without the MS compatibility it is written for, to move one diagnostic's build. Unrun by me | a veto the moment one UCRT or SDK header is shown to parse differently under it |

## 8. What I would file apart

The critic's platform-independent shapes are real and are not 2120's: `mm/` (a refused unit stops
the probe of every other unit) and `both.hero` (a region refusal stops the probe of its own unit).
They are §4.17's counted cost by the analogy of § 6. **I would file `mm/` as an `adjacent`
defect**, its candidate repair the probe asked of the units that compiled even when another
refused, the findings told once by the final round (`rounds.hero:87-108`), which costs no dedupe
(the refused unit is not probed) and no new words. `both.hero` needs the dedupe and is the larger
of the two. Neither is built here.

## 9. Prediction

**Route E, landed as built, reads the 2072 case `1 passed, 0 failed` on the CI's Windows leg on
the first run whose log prints a clang of version 23 or later, where the base compiler reads it
`0 passed, 1 failed` as the box does today; and `selfhost/cli/front_check.hero` plus the
`units.hero` delta stay under 80 lines by `wc -l` at the batch gate that lands it.** Instrument:
the CI job's log (`clang version` line and `unsupported:` count), `wc -l`. Scored at the first
milestone tag after `windows-latest` moves past clang 20; falsified by a red 2072 there under E,
or by the patch growing past 80 lines.

## 10. The repair asked by the coordinator (19:05 to 19:45), after the critic's second pass

The coordinator adopted E provisionally and asked four things on reading `critic.md`: (1) the
pass's verdict to count only where every error it reports is a region error, at the unit's own C
and naming one of `undefined_words.UNDEFINED`, any other error or a mix leaving the verdict to the
unit's own compile; (2) a compiler test witnessing E on every platform; (3) a test tying
`front_check.holds` to `value_check.opened`'s own output; (4) the critic's `hdr.h` program to build
and print 7 on the box, and the 2072 case to stay 1/0 in one build on the box and this Mac.

**The patch**: `$C/out/route-e2.patch`, against `3c478500b`, `selfhost/` only (`diff -ruN`, `-p1`;
`shasum -a 256` begins `e600280e6c2c81d4`); `git -C <lane> apply --check` passes, nothing written in
the lane. Three files: `selfhost/cli/front_check.hero` (new, 236 lines by `wc -l`, 158 before its
tests), `selfhost/cli/units.hero` (599 to 621), `selfhost/emit/value_check.hero` (123 to 128: the
region's line is now `error_line(flag)`, which `opened` writes and `holds` finds, so the spelling
exists once; the emitted C is the same text).

### (1) as worded and (4) cannot both hold: measured, and how the patch keeps both intents

A first build to the letter of (1) (`heroes-e2`, `sources 6f4bbee933c36e2b`) built the critic's
`main.hero` on the box and printed `7`, and **read the 2072 case `0 passed, 1 failed` on the box**,
the pair missing again (`out/box-e2-checks.txt`). The reason, read on the box from the 2072 unit
the harness compiled (`build/tu-58cd5f2a33902cfc/...c`, `clang @<words> -I
tests/golden/unsupported -fsyntax-only [-fno-wrapv]`): the pass reports the six region errors
**and three more**, the constancy and type `_Static_assert`s for `DIVIDED_BY_ZERO` and
`REMAINDER_BY_ZERO` at the unit's C lines 222, 251 and 252, and the unit's own compile reports
**the same three** under the target's meaning. So under (1) as worded the pass can never give a
verdict on that unit, and the case cannot be 1/0 on the box.

**What the patch does instead** (`heroes-e3`, `sources 5fa340a90f75c5eb`): the pass runs **after**
the unit's compile, and its verdict counts only where **every error it reports that the compile
did not** is a region error: placed (`clang_place.place`, read from the right, so a Windows drive's
colon stays in the path) at the unit's C file or a file one of the unit's own `#line`s names (the
`.hero` the region's `return` stands at), naming one of `UNDEFINED` as an error. An error the
compile did not report that is anything else (a header's own line, an unplaced `clang: error:`, a
`fatal error: too many errors emitted`) means no verdict: the compile's own verdict stands, as in
the base. The compile's own errors are what the base refuses the unit by, so the pass can add
nothing to a refusal but region errors, which is what (1) guards. Where the pass refuses after a
compile that succeeded, its staged object and dependency listing are discarded, never published.
**This departs from (1)'s words**; the coordinator should read it as the seat's proposal, not as
(1) done.

### Results, `heroes-e3` (`$O` = `$C/out`)

| what | where | command | result |
|---|---|---|---|
| built | Mac | `./heroes-base build selfhost/main.hero -o heroes-e3` | exit 0, `sources 5fa340a90f75c5eb` |
| built by the seed's stage 1, fixpoint | Mac | `./heroes build selfhost/main.hero -o heroes-e3s1`; both `--emit-c`, `cmp` | exit 0, same key; **identical**, 26,274,251 bytes |
| built | box | `./heroes-base.exe build selfhost/main.hero -o heroes-e3.exe` | exit 0, `sources 5fa340a90f75c5eb` |
| own tests | Mac | `./heroes-e3 test selfhost/main.hero` | **1723 tests, all passed**, exit 0 (four new, below) |
| `layout` | Mac | harness `layout` | **6 passed, 0 failed** |
| **2072 case** | Mac | harness `unsupported fixedbugs-2072` | **1 passed, 0 failed** |
| **2072 case** | box | the same with `heroes-e3.exe` | **1 passed, 0 failed** (`out/box-e3-checks.txt`) |
| **the critic's `main.hero`** (`hdr.h`: its own `#pragma clang diagnostic error "-Wshift-negative-value"` and `-1 << 2`; binds `K_OK`) | box | `heroes-e3.exe build main.hero`, run | **exit 0, prints `7`**, as the base (the first E: exit 1, `ffi_header_refused` with the false `stdio.h` note) |
| `fl.hero`, `control.hero` | box | the same | exit 0, `1.5`; exit 0, `7` |
| the same three | Mac | `heroes-e3 build` | `main` and `fl` exit 1 `ffi_header_refused`, `control` 7: as the base and the first E; `main`'s message is the base's byte for byte (`cmp main-base.out main-e3.out`) |
| `pair.hero` (`-1 << 2`, `1 << 31` alone) | box | `heroes-e3.exe build pair.hero` | exit 1, both in the region's words, `cmp` equal to the first E's |
| `unsupported` whole | Mac | harness `unsupported` | **344 passed, 0 failed** |
| `unsupported` whole | box | the same | **328 passed, 0 failed**, exit 0 (beside the box's `run`) |
| `run` whole | Mac | harness `run` | **534 passed, 0 failed** (two cases skipped for `sys/prctl.h`, as the harness says) |
| `run` whole | box | the same, started 19:17, ended about 19:57 | `382 passed, 134 failed`, **and no reading of the patch**: the box's disk filled during the run. Of the 134 `FAIL` lines (`grep -c` in `run-e3.txt` on the box), 114 carry *the operating system's own reason is 112* (Windows `ERROR_DISK_FULL`), 19 `LLVM ERROR: IO failure on output stream: no space on device`, 1 *reason is 28* (`ENOSPC`), summing to the 134; they run alphabetically from `fixedbugs-590` to the end and include programs binding no header value (`functions`, `strings`, `while-loop`), which the pass never reads. `df -h /c` after: 100G, 98% used, 2.9G free. **My own directory held 12G of it** (`du -sh /c/w/224-compiler-engineer`, `build/` 12G), regenerable cache I did not delete, since a deletion is asked for. A valid `run` on the box under the patch is **unrun** |
| own tests | box | `heroes-e3.exe test selfhost/main.hero` (beside the box's `run`) | **1723 tests, all passed**, exit 0, the four new among them: the `-fwrapv` witness holds on clang 23 too |

**The four tests** in `front_check.hero`, each run on this Mac in the 1723:
- *the region is found by the lines `emit/value_check.opened` writes, and only it is keyed* (3):
  `opened` writes into a `writer`, `holds` must find what it wrote;
- *under -fwrapv the pass refuses the region's two, and the unit's own front end does not* (2): a
  unit of two accessors in `opened`'s region, `1 << 31` and `-1 << 2`, written to
  `build/selftest-front-check/w.c`; `refused` with `clang -std=gnu11 -fwrapv` is a refusal naming
  `-Wshift-sign-overflow` and `-Wshift-negative-value`, and `clang -std=gnu11 -fwrapv
  -fsyntax-only` of the same file is not. It asks the machine's clang with `-fwrapv` written in, so
  it witnesses E on every platform, not only where the driver adds the word;
- *a header's own error, alone or beside the region's, leaves the verdict to the unit's compile*:
  the critic's `hdr.h` beside a region holding `return 7`, and beside the two, both no verdict;
- *only errors at the unit's own lines naming the region's warnings are the region's*: the pure
  reader, a Windows path with escaped backslashes in its `#line`, a header's line, an error the
  compile also reported (skipped), the same error with nothing new (no verdict), an unplaced
  `clang: error:`, a `fatal error:` line.

### What this leaves (unrun, or not repaired here)

- **The critic's `fl.hero` split stays**: the header's own compile and `header_order`'s trials
  ask under the target's meaning, so on the box a header that makes one of the two warnings an
  error itself is read under `-fwrapv` everywhere but the pass. With the repair, the pass no longer
  refuses it either, so the box reads `hdr.h` the same with E as with the base; the box and this Mac
  still differ on it, as they did before 224. Not repaired here; it is a question for the sitting.
- **The cheaper variant** (the pass only where the build's `clang -###` answer holds `-fwrapv`): my
  § 4 refused it on a premise the critic falsified (a package cannot carry `-fwrapv`,
  `libraries.hero:162`). I withdraw that reason. The variant would change only whether a real build runs the pass;
  the witness test calls the pass directly and holds under either. What running it everywhere
  still buys is that every platform's real builds exercise the same path the box needs; what it
  costs is the measured +0.03 s per cold unit holding the region on this Mac. Unbuilt; I no
  longer object to it.
- `corpus`, `warnings`, `emission` on the box under `heroes-e3`: unrun.
