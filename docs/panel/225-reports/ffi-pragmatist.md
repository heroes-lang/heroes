# Panel 225, ffi-pragmatist

Written 2026-10-18 as the facts were run, in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-ffi-pragmatist` (made with `git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach .claude/worktrees/225-ffi-pragmatist
7b97b9438`, `git log -1` printing `7b97b9438 M-witnessed-seed: closed, ...`). Every number carries
the command that produced it; a claim I did not run says *unrun*. My work files are under
`.claude/worktrees/225-ffi-pragmatist/ffi-work/`; the reproducers were copied there with `cp -p`
from `.claude/worktrees/scratch-clean-list/repro/` and `repro-s/`. Read first: `00-shared.md` with
its corrections, `critic-briefs.md`, my brief; design.md §1.11 (lines 477-583), §1.12, §4.17 (from
2265), §4.19 (from 2493); spec § 13; `.claude/rules/c-boundary.md`.

## 0. My compiler

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-s1`, `/usr/bin/time -p`
  `real 8.09 user 7.95 sys 0.12` (load averages 2.25 2.04 2.05 by `uptime`).
- Stage 2: `./heroes-s1 build selfhost/main.hero -o heroes`, `real 78.89 user 61.04 sys 12.26`
  (other sessions were running: load 3.84 after it, so the duration is not reusable).
  `./heroes --version`: `heroes 0.4.0` / `sources 08c492ac05cbb532`, the brief's key.
  `clang --version`: `Apple clang version 21.0.0 (clang-2100.3.34.2)`, `arm64-apple-darwin27.0.0`.

## 1. The defect reproduced (measure 1)

In `ffi-work/repro/`, `../../heroes build <p>.hero -o <p>`, each exit 1, one diagnostic each:
`d2137` `ffi_parameter_type` on `narrow` (2:5) alone; `d2137-next` `ffi_constant_type` on `K_DIV`
(5:5); `d2136` `ffi_parameter_type` on `out` of `fill` (2:20) alone; `d2136-next`
`ffi_constant_type` on `K_DIV`. `diff d2137.err d2137.emit.err` prints nothing: my stderr is the
coordinator's kept copy byte for byte.

## 2. The instrument: the probe's path, by hand

`ffi-work/probe-by-hand.sh` replicates `value_run`'s path: the unit's text with the reference
table no longer `used` (`value_probe.unit_text`), `clang -E -Dmain=hero_probe_hidden_main
@<unit words>`, the canaries and dispatcher appended as `value_probe.dispatcher` writes them,
`clang -x cpp-output @<unit words> <checks()> <level> -g0 -ffunction-sections -fdata-sections -w
-c`, the driver, a link with the runtime's object alone and `-Wl,-dead_strip`, and runs from
index 0 restarting after each trap, each run `timeout 30` and `head -c 1048576`. The unit words
are the build's own response file (`build/words/words-4e5808a66b72179b.rsp`, 25 words, the same
list the kept probe of `d2136-next` compiled under, `build/value-45fd0ffe54bfa35f/words-*.rsp`).
It runs from the program's directory, since the words carry `-I .` (run elsewhere it fails at
`#include <fill.h>`, measured).

**Calibration**: on `d2136-next`'s own unit (`build/tu-1460a2be262ced8e/d2136next.c`) it reads
`trap 0`, `trap 2`, `finished`, the compiler's own kept verdict
(`build/value-45fd0ffe54bfa35f/verdict.txt`: `trap 0`, `trap 2`).

## 3. Route A: the refused unit asked as it is

**Which word makes `narrow`'s refusal an error.** `d2137`'s refused unit
(`build/tu-22e47be266b6cfde/d2137.c`, 206 lines) compiled under the build's words:
`clang @build/words/words-4e5808a66b72179b.rsp -c d2137.c` prints two errors, both
`[-Werror,-Wshorten-64-to-32]`, at `d2137.hero:2:96` (the narrowing probe, unit line 105) and
`d2137.hero:8:34` (the call `t2 = narrow(t1)`, unit line 155), exit 1. The warning is made an
error three ways at once: `-Werror=shorten-64-to-32` in the words, `#pragma clang diagnostic error
"-Wshorten-64-to-32"` at unit lines 37, 91 and 144, and `"-Wimplicit-int-conversion"` inside the
probe's own push at line 103. **The same command with `-w` after the words: exit 0, an object
written**, no line on stderr. Whether `-w` lifts the word's upgrade and the pragma's alike is
measured apart below.

**The probe's whole path on the refused unit**: `probe-by-hand.sh` on `d2137.c` with accessor
`h_d2137_K_DIV` reads `trap 0`, `trap 2`, `finished` at `-O0`, and the same at `-O2`. So route A,
asked of 2137's reproducer, finds `K_DIV` in the refused build, from the program's own unit, with
no text of its own.

**Word or pragma, separated.** With `-Werror=shorten-64-to-32` removed from the words (24 words
left, `grep -v`), the unit still prints both errors, `[-Werror,-Wshorten-64-to-32]`: the unit's
pragmas alone make them errors. With `-w` added, exit 0; and `-w` before the words instead of
after, exit 0. So `-w` lifts both upgrades wherever it stands.

**A binding refusal no `-w` lifts, and how many classes are which.** Nine programs of my own in
`ffi-work/w/`, each one refused binding beside `K_DIV` over a plain header with no pragma
(`wh.h`) or the golden `ffi-a-construction.h`, built with `../../heroes build <p>.hero -o <p>`:
every one exits 1 with its one diagnostic and **`K_DIV` told in none** (`grep -c K_DIV w*.err`: 0
for all nine), and every one leaves a unit and no object, so each is 2137's shape. Each refused
unit compiled under the build's words, then with `-w`:

| program | told | clang's error in the unit | made an error by | `-w` |
|---|---|---|---|---|
| `d2137` | `ffi_parameter_type`, `i64` against `int` | `-Wshorten-64-to-32` | `-Werror=` word and pragma | **lifts** |
| `w3float` | `ffi_parameter_type`, `f64` against `int` | `-Wfloat-conversion` | pragma | **lifts** |
| `w5sign` | `ffi_parameter_type`, `u32` against `int` | `-Wsign-conversion` | `-Werror=` word and pragma | **lifts** |
| `w8handle` | `ffi_parameter_type`, handle `B` against `struct a *` | `-Wincompatible-pointer-types` (second round's unit) | `-Werror=` word and pragma | **lifts**, on Apple clang 21 |
| `w9lend` | `field_lend_written`, a field of `=` `t` lent to `void *` | `-Wincompatible-pointer-types-discards-qualifiers` | `-Werror=` word and pragma | **lifts** |
| `w1ret` | `ffi_return_type`, `-> i8` against `int` | `static assertion failed` (`HERO_RET_I8`, unit line 76) | an error by C | **stays** |
| `w2unknown` | `ffi_unknown_name`, `narrowz` | `call to undeclared function` `[-Wimplicit-function-declaration]` | clang's default error in C99 and later | **stays** |
| `w4cstr` | `ffi_parameter_type`, `cstr` against `int` | `incompatible pointer to integer conversion` `[-Wint-conversion]` | clang's default error | **stays** |
| `w6unavail` | `ffi_unavailable`, `gone` | `'gone' is unavailable` (7 lines) | an error by C | **stays** |
| `w7union` | `ffi_union_field`, `UT(i: 1, f: 1.5)` | `static assertion failed ... heroes-ffi-union UT i f` | an error by C | **stays** (the critic's U1, again) |

So **5 of 10 classes are lifted and 5 are not**. On the five `-w` lets compile, the probe's whole
path (`probe-by-hand.sh`) reads `trap 0`, `trap 2`, `finished` on each refused unit: route A finds
`K_DIV` in every one. The two default errors are clang's choice, not this compiler's:
`-Wint-conversion` and `-Wimplicit-function-declaration` are warnings clang now maps to errors by
default, which `-w` does not lift. **Which class a binding refusal falls in is therefore a fact
about the clang that compiles it**, and `-Wincompatible-pointer-types` is the live example:
whether another clang maps it to an error by default I did not run (no other clang here; the
brief keeps this seat on this Mac): *unrun*, a question for the Linux legs.

## 4. Route C by hand: a unit of the constants alone

`ffi-work/cut-c.awk` cuts route C's unit out of the program's own unit text, as the brief
describes it: every line up to the end of the `HERO_ENTRY_STOP` block (the header line, the
compiler's own includes, the guard and every group's include in its order, `heroes_standard.h`,
the ABI assertion, the unit's defines), the `#define HERO_RET_*` and `HERO_C_*` lines the
accessors' own assertions use, and each accessor from the `#line` before its signature to its
`}`. Left out: the record, union and return assertions the prelude writes
(`extern_record_assertions`, `union_assertions`, `extern_assertions`, `emit/decls.hero` lines
187-189), the types, the extern probes, the reference table, the prototypes, every body and
`main`. A cut keeps the program's `#line` numbers, so it is the most faithful route C can be; an
emitter writing it would restore to its own line count (only the generated-file lines inside an
accessor would move, never its `return`, which stands at the constant's `.hero` line).

On `d2137`'s unit the cut is 91 lines (from 206). Compiled under the build's own words **with
no `-w`**: 0 errors, for `d2137` and for every program `-w` does not lift (`w1ret`,
`w2unknown`, `w4cstr`, `w6unavail`, 91 lines each; `w7union`, 99), and the probe's path reads
`trap 0`, `trap 2`, `finished` on all six. **Route C reaches the six classes it was run on, the
five hard errors included**, every one standing outside the accessors and outside the headers;
the four other soft classes I did not run through C (P, § 7, ran all ten).

## 5. Route C attacked: does a unit of the constants alone read what the program's unit reads?

`ffi-work/att/`: `att.h` holds eight constants whose value depends on the unit's other text,
each undefined in one reading and defined in another; `cfg.h` is an earlier group's header that
sets `CFG_DIV -1`; `inner.h` is a header `att.h` includes itself. `att.hero` binds `cfg_one` from
`cfg.h`, then from `att.h` the refused `narrow(x: i64)`, `cfn`, and the eight constants.

| index | constant | what it depends on |
|---|---|---|
| 2 | `K_WORD` | `#ifdef _USE_MATH_DEFINES`, a define the program's own words pass |
| 3 | `K_CFG` | `#ifndef CFG_DIV`, set by an earlier group's header (`cfg.h`) |
| 4 | `K_INNER` | `INNER_DIV`, set by a header `att.h` includes |
| 5 | `K_BCP` | `__builtin_constant_p(minus_one())`, a `static inline __attribute__((const))` of the header |
| 6 | `K_OPT` | `#ifdef __OPTIMIZE__` |
| 7 | `K_LINE` | `__LINE__` where it is read |
| 8 | `K_FILE` | `sizeof(__FILE__)` where it is read |
| 9 | `K_CNT` | `__COUNTER__`, which `cfn`, an object-like macro of a bound function, also expands |

`heroes build att.hero`: `ffi_parameter_type` on `narrow` alone, exit 1, at `-O0` and at
`-O2`. Three readings, by `probe-by-hand.sh`, `trap N` per index (0 is the undefined canary):

| reading | `-O0` | `-O2` (built `-O2`, its own words) |
|---|---|---|
| **A**, the refused unit as it is | 2 3 4 7 8 | 2 3 4 5 6 7 8 |
| **C**, the cut of § 4 | 2 3 4 7 8 **9** | 2 3 4 5 6 7 8 **9** |
| **C′**, the cut keeping only the constants' own group's include (`cfg.h` dropped) | 2 4 7 8 9 (**3 missed**) | not run |
| **the compiler's own next build**: `att.hero` with `narrow(x: i32)`, same file name, `../../heroes build -O0/-O2 att.hero` | told `K_WORD K_CFG K_INNER K_LINE K_FILE` = 2 3 4 7 8 (kept verdict `trap 2 3 4 7 8`) | told 2 3 4 5 6 7 8 |

- **Route A reads exactly what the program's next build reads**, at both levels, every shape.
- **Route C reads every shape the program reads but one, and the one it misreads it refuses
  falsely**: `K_CNT`. The expanded `return` lines show why (`grep` of each `unit.i`): the
  program's unit preprocesses it to `return ((-2147483647 - 1) / (5 - 3));`, defined, since `cfn`
  expanded `__COUNTER__` three times before the accessor (its return assertion, its extern probe
  and the reference table, at `0`, `1` and `2` in the unit's `-E` text), and the cut to `return
  ((-2147483647 - 1) / (2 - 3));`, which is `INT_MIN / -1`. Route C would tell `K_CNT` undefined
  beside `narrow`'s refusal; the author corrects `narrow`, and the next build accepts `K_CNT`. A
  false message: the C reading the program does not make, which is this seat's veto line.
- **Route C must keep every include line in its order**: C′ misses `K_CFG`. Any route C that
  rebuilds the header list from the constants' groups instead of keeping the unit's own is wrong
  on `cfgone`'s shape (design.md §4.19's own example of two orders meaning two programs).
- `K_OPT`'s first `-O2` row, preprocessed under the `-O0` build's words, read 6 as defined in
  every reading: the probe preprocesses under the program's words, so a level is the build's,
  and my first `-O2` column was my replica's artifact. The table's column is the build at `-O2`.
- `K_FILE` was first refused `ffi_not_constant` when I named the corrected program
  `att-next.hero`: its accessor's assertion, on the generated file's line, read `__FILE__` as
  `"attnext.c"`, and `sizeof` 10 minus 10 divides by zero. That is the program's own reading of
  its own name, my artifact again; the row above keeps the name `att.hero`.

So route C's unit is not the program's reading in one measured shape. The shape is contrived
(a header counting expansions in a name it lets a program bind), but it is the shape panel 216's
critic named, and the failure is in the direction a false message lies in, not a late one.

## 6. The shipped repair of 2128 tells a false message, by the same mechanism

Route C's false refusal is not new: `constant_accessor.inert` writes a refused constant's
accessor inert **before** preprocessing, so every macro the original accessor expanded is gone
from the copy, `__COUNTER__` included. Run end-to-end with my stage 2, `ffi-work/cnt/`:

- `cnt.h`: `#define K_BAD (__COUNTER__ + 1)` and `#define K_CNT2 ((-2147483647 - 1) /
  (__COUNTER__ - 3))`; `cnt.hero` binds `constant K_BAD: i8` (refused at its accessor: `int` is not
  `i8`) and `constant K_CNT2: i64`.
- `../../heroes build cnt.hero -o cnt`: exit 1, **two** diagnostics, `ffi_constant_type` on
  `K_BAD` (`is int in cnt.h, not i8`, `fix (guess): declare it i32`) and `ffi_constant_type` on
  `K_CNT2` (*the program's own unit ... traps where it reads it*), the second from defect 2128's
  copy (`value_beside.again`).
- The drafted fix, `K_BAD: i32` (`next32/`), and `K_BAD: i64` (`next/`): each `build exit 0`, and
  the binary prints `-1073741821`, which is `3 + INT_MIN / 2`. **`K_CNT2` is defined in the
  program's own reading**; the copy read `__COUNTER__` 2 where the program reads 5, because
  `K_BAD`'s three expansions were written out of it.

So the build tells an author that a constant C defines is undefined, and the author's correction
of the other constant makes it build. By `.claude/rules/verification.md` § Bounded discovery a
false message is `blocking` whatever the work; it is found beside this sitting's question (depth
one: 2128's copy is the mechanism routes B and C generalise), so it is the coordinator's to file,
with `ffi-work/cnt/` as its reproducer. The shape is contrived, and it is the shape any copy cut
before preprocessing has.

## 7. Route P (not in the brief's list): the program's unit, marked, preprocessed whole, cut after

What would have to be true for a route nobody listed: a copy whose every accessor is preprocessed
exactly as the program's unit preprocesses it, yet whose refused lines clang never judges.
`clang -E` passes an unknown pragma through verbatim and still expands what stands between two
of them (`ffi-work/c/pragma-pass.c`, `clang -E -P`: `int b = (1 + 1);` inside the marked lines,
`int c = (2 + 1);` after them). So:

1. the probe's copy is the program's own unit, every line route C leaves out kept but wrapped in
   `#pragma heroes_probe cut` and `#pragma heroes_probe keep`, and each refused constant's
   accessor wrapped whole with its inert twin written after the `keep`
   (`ffi-work/mark-p.awk`; without the markers the copy differs from the unit by the twin's four
   lines alone, `diff`);
2. it is preprocessed under the program's own words as today (`value_run.preprocessed`);
3. the marked lines leave the `-E` text, then the dispatcher is appended and compiled as today
   (`probe-by-hand.sh` with `CUT=1`).

Measured, each against § 5 and § 6's references:

| program | `-fsyntax-only` errors in the marked copy, before the cut | P's reading |
|---|---|---|
| `cnt` (`K_BAD` inert) | 2 | `trap 0`, `finished`: `K_CNT2` defined, **as the program's next build** |
| `att`, `-O0` | 2 | 2 3 4 7 8, **as the next build**, no `K_CNT` |
| `att`, built `-O2` | 2 | 2 3 4 5 6 7 8, **as the next build** |
| `d2137` | 2 | `K_DIV` (`trap 2`) |
| `w1ret` `w2unknown` `w4cstr` `w6unavail` `w7union` | 1, 1, 2, 7, 1 | `K_DIV` in each |
| `w3float` `w5sign` `w8handle` `w9lend` | (not counted) | `K_DIV` in each |

**P reaches all ten classes of § 3 and reads every shape of § 5 and § 6 as the program does.** It
is route C's unit, cut after the preprocessor instead of before it, and route A's reading,
without A's dependence on which warnings a given clang maps to errors.

What P cannot reach, by construction: a refusal inside a header's own text, or one the
preprocessor makes (a missing header, `#error`): the header text is kept, so the probe's compile
or its `-E` fails, and a failure beside a refusal is told by nobody (`value_beside.hero` lines
21-24), as today. That is right: such a header gives no value to read.

One thing every route that keeps a refused accessor alive owes: `whose.with_beside`
(`selfhost/cli/whose.hero` lines 253-262) appends the probe's findings to clang's with no check
by declaration, so a constant clang refused at a place `value_beside.holding` does not locate
would be told twice. Today's inerting is what prevents it; a resolution should drop a beside
diagnostic whose declaration the refusal already names.

## 8. Route P built in my copy, with E (measure 2)

Six modules changed and one added in `.claude/worktrees/225-ffi-pragmatist` (`git diff --stat`:
158 insertions, 69 deletions; `selfhost/emit/probe_cut.hero` new, 75 lines), each formatted with
`heroes fmt --in-place` and the whole compiler checked by `./heroes check selfhost/main.hero`
(clean, `real 6.71`):

- `emit/decls.hero`: `prelude` split into `opening` (the headers, the guard, the unit's defines)
  and `checks` (the record, union and return assertions, the string literals); `prelude` calls
  both, so its callers and its output are unchanged.
- `emit/probe_cut.hero`: the two marks, `#pragma heroes_probe cut` and `#pragma heroes_probe
  keep`, and `cut`, which takes out every line from a `CUT` line to the next `KEEP` line by a
  byte search and slices, never a split into lines (a unit that reads a large header preprocesses
  to megabytes); its test includes a mark's words inside another line, which cut nothing.
- `emit/unit.hero`: `unit(..., inert, cut)`. With `cut`, the `CUT` line after the opening; a
  `KEEP` before each accessor the probe asks and a `CUT` after it; a refused constant's own
  accessor written whole inside the cut, its inert twin after a `KEEP`; a `KEEP` at the end.
  Without `cut`, the text the build compiles, as before.
- `cli/value_run.hero`: `Unit.cut`; `preprocessed` applies `probe_cut.cut` to a cut unit's `-E`
  text, the probe's and the quote's alike.
- `cli/value_beside.hero`: `refused` names the constants clang refused at their accessors and
  no longer gives up where an error stands elsewhere or has no place; `again` always writes the
  cut copy, and stays absent only where an accessor is not located once in the unit.
- `cli/assemble.hero`: every refused unit is asked again, not the first alone (route E).
- `cli/whose.hero`: `with_beside` drops a probe finding at a span the refusal already tells.

`./heroes build selfhost/main.hero -o heroes-p`: `real 37.37`; `heroes-p --version`: `sources
7e12e4f040112477`.

| program | stage 2 (today) | `heroes-p` |
|---|---|---|
| `d2137` | `narrow` alone, exit 1 | **`narrow` and `K_DIV`, exit 1**, `K_DIV`'s message byte for byte as `d2137-next`'s |
| `d2137-next`, `d2136-next` | `K_DIV`, exit 1 | the same |
| `d2136` | `fill`'s `out` alone | **the same**: P and E act on units, 2136 is refused before any (§ 10) |
| `cnt` (§ 6) | `K_BAD` and a **false** `K_CNT2` | **`K_BAD` alone** |
| `att`, `-O0` / `-O2` (§ 5) | `narrow` alone | `narrow` and exactly the next build's constants, `K_CNT` never |
| `w1ret` `w2unknown` `w3float` `w4cstr` `w5sign` `w6unavail` `w7union` `w8handle` `w9lend` | the binding alone | **the binding and `K_DIV`**, every one, the critic's U1 (`w7union`) included |
| critic's S1 (`repro-s/s1`) | `narrow` alone | `narrow` and `K_DIV`, from the second unit (E); `K_NONE`, that unit's own clang refusal, still waits a build: 2143's other half, which no probe tells |
| critic's S2 | `K_NONE`, `K_DIV` | the same; `narrow`, the second unit's own refusal, still waits |

**The gate, every golden case that exercises the probe** (critic's § 3.3), `./heroes-p run
tests/harness/main.hero -- ./heroes-p <form> <filter>`, each exit 0: `unsupported`
`fixedbugs-2072` 1/0, `-2075` 5/0, `-2101` 1/0, `-2104` 1/0, `-2109` 1/0, `-2128` 4/0; `run`
`fixedbugs-2072` 1/0, `-2075` 1/0, `-2101` 1/0, `-2109` 1/0: **17 passed, 0 failed**, the
critic's baseline. Logs `ffi-work/gate-*.log`.

**The program's own unit does not move.** Built in two fresh directories, one per compiler
(`ffi-work/cmpA`, `cmpB`), `cmp` of the units: `d2137next` (12,124 bytes), `d2137` (9,765),
`att` (24,088), `w7union` (11,647), each byte-identical, three of them refused units. `--emit-c`
of `run/fixedbugs-2072-values-c-defines-still-build` (18,168 bytes) and
`run/fixedbugs-2101-...-builds` (15,035), each exit 0 under both, byte-identical.

## 9. The cost (measure 3)

Clang runs counted by a logging shim first on `PATH` (`ffi-work/shim/clang`, one line per run),
each program built in a fresh directory, cold then warm:

| program | stage 2 cold / warm | `heroes-p` cold / warm | probe directories |
|---|---|---|---|
| `plain.hero`, no `extern` | 5 / 2 | 5 / 2 | none |
| `nocon.hero`, `narrow` refused, no constant | 12 / 6 | 12 / 6 | none |
| `d2137.hero` | 13 / 7 | **18 / 8** | 1 (`heroes-p`) |

A program binding no header value builds as before, its probe not run, cold and warm. A refused
build whose refused units bind no asked constant pays nothing. Each refused unit that binds one
pays **five clang runs cold** (named by `diff` of the two logs: the copy's `-E`, its trap-mode
compile, its link, the `-E -dD` that quotes the header's line, and panel 218's C2 preprocess,
under `build/init-<key>`, `value_c2.hero` line 55)
plus the probe's own runs (three here: the canary's trap, `K_DIV`'s, the finish), and **one warm**,
the quote, which `value_ask.defined_text` re-runs whenever a kept verdict refuses (an existing
behaviour of the probe, not this route's). With E it is per refused unit.

## 10. 2136 and route F

`heroes-p` still tells `fill` alone on `d2136`: its refusal is `probing.check_all`'s, before any
unit is written. Why 2136 needs a probe at all is visible in the C: the call crosses as
`(void)fill((void *)&h0_x)` (`d2136next.c` line 147), so clang never sees the pointee at the
unit, which is why the pointee check runs before it.

**F's C side, by hand**: no compiler writes `d2136`'s unit, so I derived it from `d2136-next`'s by
the emitter's two differences (the `fill` probe's `int32_t * a0`, the lent local's `int32_t`),
marked both with `mark-p.awk`, preprocessed both under the build's words and cut: the kept texts
are byte-identical (`cmp`, 1,148 lines without line markers), and the probe reads `trap 2`,
`K_DIV`. P's copy needs no unit to have compiled, so it can stand where F needs it. **F's
compiler side I did not build** (the units written and asked before `bound_macro`, `pragma_ask`,
`check_all`, `lock_told` and `two_ways`): unrun. Its one risk that P does not remove: before the
units compile, no clang verdict on the accessors exists to write a refused constant inert, so a
constant clang would refuse with a warning made an error is told in the probe's words (true,
worded otherwise than the next build's), and one clang would refuse with a hard error makes its
unit's probe fail, its other values then told a build late, as today.

## 11. Beside the sitting, a negative

Whether a header can switch the probe's traps off for every accessor with `#pragma clang
attribute push (__attribute__((no_sanitize("signed-integer-overflow"))), apply_to = function)`
left open (`ffi-work/nosan/`): no. `heroes build nosan.hero` is `ffi_header_refused`, *unterminated
'#pragma clang attribute push' at end of file*, exit 1; a push must close within the header's own
text, which ends before any accessor. One shape searched; a balanced push and pop inside the
header reaches no accessor by construction.

## 12. Route P against real headers

**SQLite, the ladder's own binding** (`ffi-work/sqlite/`, the SDK's `sqlite3.h`):
`examples/sqlite/main.hero` with one refusal planted that `-w` does not lift,
`sqlite3_column_int(...) -> i8` (the return assertion), and a second group binding `K_DIV`:

| program | stage 2 | `heroes-p` |
|---|---|---|
| `planted.hero` | `ffi_return_type` on `sqlite3_column_int` alone, exit 1 | **`ffi_return_type` and `K_DIV`**, exit 1; `SQLITE_OK` and `SQLITE_ROW` not told |
| `corrected.hero` (`-> i64` again) | `K_DIV` alone | `K_DIV` alone: what `heroes-p` told beside the refusal is the next build's reading |
| `examples/sqlite/main.hero`, untouched | (green in the tree) | builds exit 0, runs exit 0, output `diff`-equal to `main.expected` |

**Values C defines, beside a refusal, read as defined** (`ffi-work/defined/`):
`run/fixedbugs-2109-a-value-c-defines-in-an-initialiser-builds.hero`, fifteen constants C
defines, two of them records and one a `__builtin_constant_p` shape, with 2137's `narrow(x: i64)`
added beside them. `heroes-p build -O0` and `-O2`: `narrow` alone, exit 1; both kept verdicts read
`trap 0`, the canary alone. P's copy refuses none of them.

**The verbs** (critic's note): on `d2137.hero`, `heroes-p build --emit-c` (exit 1, 0 bytes on
stdout), `heroes-p run` and `heroes-p test` each tell `ffi_parameter_type` and `ffi_constant_type`,
exit 1, as `build` does.

**The compiler's own tests**: `./heroes-p test selfhost/main.hero`, **1731 tests, all passed**,
exit 0 (`real 149.42 user 107.27 sys 36.05`, other sessions running), the new and changed tests
among them (`grep` of the log: *the copy loses the lines between its marks after
preprocessing...*, *a refused unit's errors are read against the accessors its text holds*).

**Two shapes beside P's own** (`ffi-work/mixed/`): a unit refused at once at an accessor
(`K_SHIFT`, the region's `shift count >= width`) and elsewhere (`narrow(x: i64)`), with its group
written after `main`, so `main`'s body stands before the accessors in the unit (`h_mixed_main` at
unit line 126, the accessors from 141). Stage 2: `K_SHIFT` and `narrow`, exit 1, `K_DIV` and
`K_FLT` waiting a build (today's `refused` gives up on the error elsewhere). `heroes-p`: `K_SHIFT`,
`narrow`, `K_DIV`, `K_FLT`, exit 1, `K_SHIFT` once.

**Every refused golden case, not the 17 alone**: a refusal at a unit that also binds a constant
could gain a diagnostic under P, and those cases live in two forms. `./heroes-p run
tests/harness/main.hero -- ./heroes-p unsupported`: **353 passed, 0 failed**, exit 0, two
Windows-only cases `SKIP` (defect 2053's, by their own note), `real 153.68`; `-- ./heroes-p full`:
**40 passed, 0 failed**, exit 0. No pinned diagnostic moved.

**One cost P pays for nothing**, measured with the shim (`ffi-work/cost/hdr-*`): a unit refused
inside its header (`nosan.h`, § 11) beside `K_DIV`. Stage 2: 33 clang runs cold, 30 warm;
`heroes-p`: 35 and 32, both extra runs the copy's (its `-E` and its compile, which fails on the
header as the unit did and keeps no verdict, so warm pays them again); told the same one
diagnostic. A resolution can skip the copy where clang's own message places an error in a header
file rather than the unit's C or a `.hero` line; I did not build that.

## 13. Verdicts, per route

| route | verdict | the strongest reason it is wrong, measured |
|---|---|---|
| **A**, the refused unit as it is | **object** as the resolution (its *reading* is exact: § 5, equal to the next build at both levels) | reaches **5 of 10** refusal classes (§ 3), and which five is clang's choice of default errors (`-Wint-conversion`, `-Wimplicit-function-declaration` measured default errors here; `-Wincompatible-pointer-types` on the CI's clangs *unrun*), a premise about the world that expires in silence. And A must still write a refused constant inert (else a region refusal is told twice), which today happens before preprocessing: § 6's false message comes with it |
| **B**, the refused binding written inert | **object** | inert at the source is one emission form per class (a narrowing probe, a return assertion, a union assertion, field assertions, a call in a body, a construction, a lend, the reference table), and each takes expansions out of the text: § 6's mechanism. P does B's job for every class at once, after the preprocessor |
| **C**, a unit of the constants alone | **veto**, as a cut made before preprocessing | § 5: it tells `K_CNT` undefined where the program's next build accepts it, a false message, the C reading the program does not make; panel 216's compiler-engineer withdrew F2 on that same condition (`A_COUNTER_MISS`, panel 216 file line 48). And C′ shows its *include lines* must be every one, in order |
| **D**, left as it is | **object** | no wrong binary, but a binding and its constants are written together from one header and each is told a build apart; the only form that lets a tag be placed writes into §4.17 (full sitting); and D leaves § 6's shipped false message where it is |
| **E**, every refused unit | **approve**, with P | none found: S1 measured (`K_DIV` from the second unit). It tells the probe's half of 2143 only; the second unit's own clang refusal is `whose`'s to tell, out of every probe route |
| **F**, the probe before a pre-unit refusal | **approve its C side**, conditionally; its compiler side *unrun* | before the units compile no clang verdict on the accessors exists, so a refused constant is told in the probe's words or makes its unit's probe fail; P's copy needs no unit compiled (§ 10) |
| **P**, the unit marked, preprocessed whole, cut after (this seat's) | **approve: the resolution I would adopt, with E** | none found in § 7 to § 12; its one cost for nothing is the header-refused build's two runs, and its marks rest on `clang -E` passing an unknown pragma as a whole line, measured on Apple clang 21 alone |

## 14. The seat's answer

- **verdict**: approve P with E; **veto** route C as a cut made before preprocessing; object to A,
  B and D as resolutions; approve F's C side on condition.
- **section**: design.md §4.19, *a `constant` is the group's second kind of member, and its value is
  the header's* (panel 038), and §1.11's point 3, *FFI signatures are verified by clang against the
  real header*; spec § 13, *clang checks every result type, constant and record field against that
  header*. design.md does not cover a probe copy's fidelity (`grep -n '__COUNTER__'
  docs/design.md spec/heroes-spec.md` prints nothing); the ruling it rests on is panel 216's R1,
  the program's own unit, chosen over F2 for this very failure.
- **experiment**: `ffi-work/` in my copy. By hand: the refused units under the build's words and
  `-w` (§ 3, ten classes), route C's cut (`cut-c.awk`, clang accepts it with 0 errors for `d2137`
  and the five hard refusals), route P's marks (`mark-p.awk`, clang accepts the cut text for all ten),
  eight dependence shapes (`att.h`) and the shipped copy's counter (`cnt.h`) run end-to-end;
  built: `heroes-p` (P with E), gated by 17 of 17 probe cases, `unsupported` 353/0, `full` 40/0,
  1731 compiler tests, units and `--emit-c` byte-identical, SQLite's real header.
- **argument**: The probe exists so a header's value reads as the program reads it (panel 216's
  R1). A copy cut before the preprocessor reads another text: route C told `K_CNT` undefined where
  the program's next build accepts it, and the shipped copy of 2128 tells `K_CNT2` so today, the
  failure panel 216 withdrew F2 for. Route A reads truly but reaches only the five of ten refusal
  classes `-w` lifts, a set clang's defaults decide. Cut after preprocessing, P keeps every token
  the program's unit expands and lets clang judge only the opening and the accessors: ten classes
  of ten, every attack shape, the unit byte-identical, five clang runs per refused unit binding a
  constant, cold.
- **prediction**: if P lands, the golden cases the landing adds from `d2137.hero` (both
  diagnostics, exit 1) and from `ffi-work/cnt/` (`K_BAD` alone, no `K_CNT2`) read the same on all
  four CI legs, because every leg's `clang -E` passes `#pragma heroes_probe cut` and `keep` as whole
  lines. Instrument: the `unsupported` form on the CI's legs. Scored at the landing push's first CI
  run. Falsified by any leg reading `K_CNT2` or missing `K_DIV`.
- **condition**: the veto on C lifts only by moving its cut after preprocessing, which is P: no
  corpus run can lift it, since the false message is the mechanism's, measured in shipped code.
  My approval of P turns to an objection if a CI leg's clang does not pass the marks (the
  prediction falsified), or if any program is found where P's copy tells a value the corrected
  program's own build accepts. A turns to approve if it writes its refused constants inert after
  preprocessing, at which point it is P with less reach.

## 15. For the coordinator

- **A false message in shipped code, found beside**: defect 2128's copy (`062f8c262`) tells a
  value C defines undefined (§ 6, reproducer `ffi-work/cnt/`, three builds). `blocking` by the
  bounded-discovery rule's own list; filing is yours. P repairs it as a side effect.
- **What P does not do**: tell a second refused unit's own clang refusal (2143's other half,
  `whose`), reach 2136's family without F's compiler side, or skip the copy of a unit refused inside
  its header (two clang runs, § 12). Hardening I did not build: the marks could carry a digest of
  the program's sources, so no header can write their text.
- **The lane**: P writes nothing into design.md §4.17 or spec § 13; no diagnostic class, wording or
  surface moves (the 17 cases and `unsupported`/`full` whole read as before). It changes when a
  value is told and repairs a false message.
- My copy stays at `.claude/worktrees/225-ffi-pragmatist`, its diff uncommitted, `heroes-p` built
  in it; `tests/golden/run/build/` there is a cache my `--emit-c` spot check left.
