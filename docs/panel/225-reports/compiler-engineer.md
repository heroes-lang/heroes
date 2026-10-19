# Panel 225, compiler-engineer

Written 2026-10-18 as the facts were run, in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-compiler-engineer` (made with `git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach .claude/worktrees/225-compiler-engineer
7b97b9438`, `git log -1 --oneline` printing `7b97b9438 M-witnessed-seed: closed, ...`), with my own
stage 2. Every number carries the command that produced it; a claim I did not run says *unrun*.
Read first: `00-shared.md` (with its corrections), `225-reports/critic-briefs.md`,
`225-briefs/compiler-engineer.md`. **In progress**; the verdict section is written last.

**Complete, 2026-10-19 04:36 +0200** (`date`). The sections below are the log in the order the
facts were run; the verdicts are § 11, the condition and the prediction § 12.

## Summary

- **Adopted, built: C-prime**, a route the brief does not list. Every refused unit is asked again
  (E) and, at four of the six refusals made before any unit, every module's unit (F): the
  program's own unit, written with a pragma marking every stretch no asked accessor needs,
  preprocessed **whole** under the program's own words, and only then cut. Every expansion reads as
  the program's unit reads it (`__COUNTER__`, `__LINE__`, `__FILE__`, `__OPTIMIZE__`, a `#define`),
  and nothing an author's binding can make clang refuse is left to compile. It reads `d2137.hero`
  in one build (`narrow` and `K_DIV`, exit 1), and **the same mechanism reaches `d2136.hero`**
  (`fill` and `K_DIV`), U1, L1 and S1's second unit; the critic's 17 cases 17 of 17, `layout` 6/0,
  the compiler's own tests 1732 passed. No build's C moves (six programs' `--emit-c` and 13 units
  byte-identical); a build not refused pays nothing (same clang runs; the warm self-build 34.9 s
  either way); a refused build that binds a header value pays 5 clang runs and about 0.6 s cold, 1
  run and 0.04 s warm, per unit asked. 10 files, +348 -107.
  **Corrected 2026-10-19 (the union step)**: not every expansion. The probe's preprocess adds one
  word to the unit's own, `-D main=hero_probe_hidden_main` (`cli/value_run.preprocessed`), so a
  header testing `main` reads otherwise under every probe, a compiled unit's and a copy's alike:
  defect 2146, found by the coordinator, to be repaired in the lane. What holds is that the copy
  reads every expansion as the probe of a unit the build compiled reads it.
- **Veto on B and C** (soundness, design.md §4.17): both leave text out *before* preprocessing,
  and a bound function behind a macro that counts makes them tell `K_CTR` as undefined where the
  program's own unit reads 0 (§ 6). **Object** to A as the whole resolution (it reaches only a
  warning promoted to an error) and to D (the tag).
- **FOUND, for the coordinator to file `blocking`**: the landed repair of defect 2128 has the same
  hole; its inert accessor makes `inert.hero` tell `K_CTR` falsely on today's compiler (§ 9). The
  adopted route repairs it and retires `emit/constant_accessor.inert`.

## 1. My compiler

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-s1`, `/usr/bin/time -p`
  `real 7.59 user 7.50 sys 0.12`.
- Stage 2: running at the time of writing; its `--version` goes here.
  (Completed: § 1b below.)

## 2. The modules read, `wc -l` at `7b97b9438`

`wc -l` in my copy: `selfhost/cli/value_beside.hero` 248, `value_ask.hero` 151, `value_run.hero`
280, `assemble.hero` 287, `rounds.hero` 291, `selfhost/emit/constant_accessor.hero` 153,
`selfhost/cli/probing.hero` 274; `selfhost/emit/value_probe.hero` 316, `selfhost/emit/unit.hero`
217, `selfhost/emit/members.hero` 320, `selfhost/cli/whose.hero` 275, `selfhost/cli/value_told.hero`
352, `selfhost/cli/value_c2.hero` 205, `selfhost/cli/settled.hero` 82, `selfhost/cli/units.hero` 621.

## 1b. Stage 2 (completed)

`./heroes-s1 build selfhost/main.hero -o heroes`: `real 79.00 user 61.08 sys 12.26`
(`/usr/bin/time -p`; load average 2.71 by `uptime` right after, so not a duration to reuse).
`./heroes --version`: `heroes 0.4.0` / `sources 08c492ac05cbb532`, the brief's key.
`clang --version`: `Apple clang version 21.0.0 (clang-2100.3.34.2)`, `arm64-apple-darwin27.0.0`.

## 3. The defect, reproduced in my copy

Reproducers copied (`cp -p`, sources only, no `build/`) into `ce-work/repro/` and `ce-work/repro-s/`.
`../../heroes build <p>.hero -o <p>` in `ce-work/repro/`:

| program | exit | stderr |
|---|---|---|
| `d2137.hero` | 1 | `error[ffi_parameter_type]` for `x` of `narrow` alone; `diff` against the coordinator's `d2137.emit.err` prints nothing |
| `d2137-next.hero` | 1 | `error[ffi_constant_type]` for `K_DIV` alone |
| `d2136.hero` | 1 | `error[ffi_parameter_type]` for `out` of `fill` alone |
| `d2136-next.hero` | 1 | `error[ffi_constant_type]` for `K_DIV` alone |

The refused unit stays on disk, `build/tu-7690bfb33193103e/d2137.c` (206 lines by `wc -l`), with the
build's words in `build/words/words-2c0e949969d09fd4.rsp`.

## 4. Route A's open question, measured by hand

- `clang @build/words/words-2c0e949969d09fd4.rsp -c build/tu-7690bfb33193103e/d2137.c -o /dev/null`:
  exit 1, two errors, both `[-Werror,-Wshorten-64-to-32]`, at `d2137.hero:2:96` (the extern probe
  `hero_ffi_probe_h_d2137_narrow`, unit line 105, under the unit's own `#pragma clang diagnostic
  error "-Wimplicit-int-conversion"` block, lines 99 to 107) and `d2137.hero:8:34` (`t2 =
  narrow(t1)` in `main`'s body). The critic's reading, reproduced.
- **The same command with `-w` after the words: exit 0.** And with the probe's own words after them
  (`-fno-sanitize=all -fsanitize=<the four> -fsanitize-trap=<the four> -fno-wrapv -O0 -g0
  -ffunction-sections -fdata-sections -w -c`): exit 0. So **`narrow`'s refusal is a warning `-w`
  lifts**, the unit's `#pragma clang diagnostic error` included: route A's question, answered yes
  for d2137.
- **What `-w` lifts on this clang, and what it does not** (`ce-work/hand/w.c`, `clang -std=gnu11
  -c` with and without `-w`): it lifts `-Wincompatible-pointer-types` (a warning here by default;
  also made an error by `-Werror=incompatible-pointer-types` and by `#pragma clang diagnostic
  error`, `ce-work/hand/p.c`, exit 0 under `-w`). It does **not** lift the three warnings this
  clang makes errors by default, `-Wint-conversion`, `-Wincompatible-function-pointer-types`,
  `-Wimplicit-function-declaration`, nor the hard errors, `use of undeclared identifier` and
  `static assertion failed`. So route A's reach is the class of a warning promoted to an error by a
  word or a pragma, and no more: U1's assertion, a function name the header lacks and an integer
  passed for a pointer stay outside it.
- **A pragma line survives `clang -E` verbatim and unexpanded** (`ce-work/hand/p.c`: `#define
  hero_cut EXPANDED_IF_YOU_SEE_THIS`, then `#pragma hero_cut on 7f3a`; `clang -std=gnu11 -Wall -E
  p.c` prints `#pragma hero_cut on 7f3a` as written, and no warning on stderr). And a `__COUNTER__`
  expanded between two such lines is counted before anything is cut: `int g(void) { return K_C; }`
  after the region reads `(1)` in the `-E` output. **This is the fact the route I adopt rests on**
  (§ 6): text removed *after* preprocessing leaves every later expansion as the program's unit
  read it; text removed *before* preprocessing does not.

## 5. Step 1 built: route A generalised, with E (every refused unit)

**What changed** (my copy, `git diff --stat`: `selfhost/cli/assemble.hero` 24 lines touched,
`selfhost/cli/value_beside.hero` 65, 49 insertions and 40 deletions together, tests included):

- `value_beside.refused` collects the constants whose accessors hold an error clang reported and
  **no longer gives up** where an error stands elsewhere or has no place; it fails only where the
  accessors cannot be located (`fail(code: "unlocated")`), and then the unit is not asked, as
  before. `again` returns the copy even where no accessor holds an error: then the copy **is** the
  refused unit's own text, and the probe's existing `-w` decides whether it compiles. Its test now
  holds the new rule (an error elsewhere keeps the accessor's own error held; an error elsewhere
  alone holds none).
- `value_beside.told` asks one unit at a time, so a copy the probe cannot answer no longer silences
  the units after it (`value_ask.told` returns at its first `broken`).
- `assemble.per_module` keeps **every** refused unit (`refused_at`, `refused_texts`, the words in
  `refusals`, same order) and asks each again: the critic's E, the probe half of defect 2143.

`./heroes check selfhost/main.hero`: clean, `real 6.74`. Built with my stage 2: `./heroes build
selfhost/main.hero -o heroes-ae`, `real 36.97 user 36.06 sys 0.68`, `sources 6886f58d435317b1`.

**Read, each in a fresh directory (`rm -rf build`), `../../heroes-ae build <p>.hero -o <p>`:**

| program | today (stage 2) | A+E |
|---|---|---|
| `d2137.hero` | `ffi_parameter_type` alone, exit 1 | **`ffi_parameter_type` (`narrow`) and `ffi_constant_type` (`K_DIV`), exit 1** |
| `d2136.hero` | `ffi_parameter_type` alone | the same: not reached (refused before any unit) |
| S1 (`shapes/s1.hero`) | `narrow` alone (critic) | `narrow` and `K_DIV`; `K_NONE`, the second unit's own refusal, still waits (2143's other half) |
| S2 | `K_NONE` and `K_DIV`, not `narrow` (critic) | the same |
| U1 (`pre/u1/u1.hero`) | `ffi_union_field` alone | the same: not reached, the static assertion stands under `-w` |
| L1 (`pre/l1/l1.hero`) | `ffi_lock_copied` alone | the same: not reached (refused before any unit) |

**The gate, the critic's 17 cases** (`./heroes run tests/harness/main.hero -- ./heroes-ae <form>
<filter>`, one log each under `ce-work/gate-ae/`): `unsupported` 2072 1/0, 2075 5/0, 2101 1/0, 2104
1/0, 2109 1/0, 2128 4/0; `run` 2072, 2075, 2101, 2109 1/0 each; every run exit 0. **17 of 17.**

## 6. Step 2 built: the cut after preprocessing (C-prime), and the attack that sinks B and C

**A route the brief does not list, C-prime: route C's content, cut after `-E` instead of written
before it.** The copy of a refused unit is the program's own unit, written whole, with a pragma
line opening and closing every stretch no constant's accessor needs (the prelude's three assertion
writers, then everything after the typedefs except the accessors: the extern probes, the reference
table, the per-type prototypes, the descriptors, the prototypes, the callback table, the padding
tables, every body, the per-type bodies and the shim). The probe preprocesses that copy under the
program's own words exactly as it preprocesses a compiled unit, and **only then** cuts the marked
stretches from the `.i` (`selfhost/emit/probe_cut.hero`, new, 85 lines with its three tests).
Every expansion the program's unit makes is made, so `__COUNTER__`, `__LINE__`, `__FILE__`,
`__OPTIMIZE__` and a `#define` inside a stretch read as the program reads them; and nothing an
author's binding can make clang refuse is left to compile.
**Corrected 2026-10-19 (the union step)**: every expansion but the one word the probe's preprocess
adds, `-D main=hero_probe_hidden_main`, which every probe carries (defect 2146). The marks are written by
`writer.cut_open` and `cut_close`, which write nothing unless `writer.cuts` is set, and only
`value_beside.again` sets it (`unit.unit`'s new `marked`): **a unit the build compiles carries no
mark** (`grep -c hero_probe_cut build/tu-*/*.c` prints `0` for both units of U1's build), so no
build's emitted C moves and no key of a compiled unit moves.

Built: `./heroes build selfhost/main.hero -o heroes-cut`, `real 40.06 user 38.18 sys 1.50`,
`sources 505d21923f4146fe`; `./heroes check selfhost/main.hero` clean (`real 6.67`).

| program | A+E | C-prime (A+E plus the cut) |
|---|---|---|
| `d2137.hero` | `narrow` + `K_DIV` | `narrow` + `K_DIV` |
| `d2136.hero` | `fill` alone | `fill` alone (refused before any unit) |
| S1 | `narrow` + `K_DIV` | the same |
| S2 | `K_NONE` + `K_DIV` | the same |
| **U1** | `ffi_union_field` alone | **`ffi_union_field` + `K_DIV`**, exit 1 |
| L1 | `ffi_lock_copied` alone | the same (refused before any unit) |

U1's probe directory (`build/value-64493f1a3ba88c31/`): the copy `u1.c` holds three marked
stretches (lines 68 to 98, 107 to 159, 190 to 261); the compiled `probe.i` (1,397 lines) holds no
mark, no union assertion and no body, only the header, the runtime, the ABI stamp, the typedefs,
`K_DIV`'s accessor and the dispatcher; `verdict.txt` reads `trap 0` (the canary) and `trap 2`
(`K_DIV`).

### The attack: a bound function behind a macro that counts (`ce-work/ctr/`)

`ctr.h`: `static inline int narrow_impl(int x)`, `#define narrow (*(__COUNTER__, &narrow_impl))`,
`#define K_CTR (__COUNTER__ < 3 ? (-2147483647 - 1) / -1 : 0)`, `#define K_DIV ((-2147483647 - 1) /
-1)`. In the program's own unit `narrow` expands three times before `K_CTR`'s accessor (the result
assertion at unit line 75, the extern probe at 104, the reference table at 110; `grep -n` of
`build/tu-2649d1e805fe4e95/ctr.c`), and the accessor's two assertions take 3 and 4, so its `return`
reads 5: `K_CTR` is 0, a value C defines.

- **Ground truth, my unmodified stage 2**: `ctr-only.hero` (`narrow(x: i32)`, `K_CTR` alone) builds,
  exit 0, and its binary prints `3` (`timeout 10 ./ctr-only`); `ctr-next.hero` (`narrow(x: i32)`,
  `K_CTR`, `K_DIV`) is refused for `K_DIV` alone, exit 1. `ctr.hero` (`narrow(x: i64)`): `narrow`
  alone, exit 1.
- **A+E** on `ctr.hero`: `narrow` and `K_DIV`, exit 1. **C-prime**: `narrow` and `K_DIV`, exit 1.
  Both right.
- **The B/C model** (`heroes-before`, `sources 10475bdb17f39e64`: the same marks, cut from the
  unit's text BEFORE `-E`, every line starting with `#` kept, so the defines and `#line`s stand and
  only the binding's code and the bodies go, which is what route B, *the binding's code and its
  calls made to read nothing of the header*, and route C, *its include lines and defines, and the
  constants' accessors*, both remove): **`narrow`, `K_CTR` and `K_DIV`**, exit 1. **The `K_CTR`
  line is false**: it says *the program's own unit ... traps where it reads it*, and the program's
  own unit reads 0. A false message beside the author's own refusal, the `blocking` class, on the
  shape panel 216 sank F2 with (`A_COUNTER_MISS`). The model was built and the file restored at
  once (`cmp` of `selfhost/cli/value_run.hero` against the kept cut version printed nothing).
- A first, cruder model that also cut the prelude's `#define HERO_RET_*` before `-E` told `narrow`
  alone: its own copy no longer compiled. That is the second way a cut before preprocessing goes
  wrong, silence, and it is not counted against B or C, which keep the defines.

## 7. Step 3 built: F, the probe beside four of the six refusals made before any unit

`selfhost/cli/value_early.hero` (new, 108 lines by `wc -l`): at a refusal of `rounds.round` made
before any unit, each module whose unit defines an accessor the probe asks has its unit written as
the round would write it (`members.retold` with the round's tags, the lend units read so far and
the bound macros kept), **marked** as the copy beside a unit's refusal is, and asked. `rounds.hero`
(+37 lines, -2): `check_all`'s refusal fills `beside`, which `run` already tells (line 125); the lock
refusal and the two-ways refusal stage the probe's findings in one list with their own (`joined`).

**Not wired, on purpose: `bound_macro` and `pragma_ask`.** Each refuses because the unit's own
reading of a name cannot be trusted (a bound name another group's header gives a meaning; a name
the compiler writes that a header defines as a macro holding a `_Pragma`). A value read through
that unit could be told under a meaning the author did not bind, so a word there could be false;
the value waits the build it waits today. This is reasoned from the two modules' own comments and
**unrun**: I built no program that refuses at either beside an undefined value.

Built: `./heroes build selfhost/main.hero -o heroes-f`, `real 36.88 user 36.41 sys 0.50`,
`sources 396a4d6062463979`. Each program in a fresh directory:

| program | today (stage 2) | full route (A+E, the cut, F) |
|---|---|---|
| `d2137.hero` | `narrow` | `narrow` + `K_DIV` |
| **`d2136.hero`** | `fill` | **`fill` + `K_DIV`** (two diagnostics, one blank line between, one staging) |
| S1 | `narrow` | `narrow` + `K_DIV` |
| S2 | `K_NONE` + `K_DIV` | the same |
| U1 | union | union + `K_DIV` |
| **L1** | lock | **lock + `K_DIV`** |
| `ctr.hero` | `narrow` | `narrow` + `K_DIV`, no word on `K_CTR` |

**Gates on the full route's compiler:** the critic's 17 cases, 17 of 17 (`ce-work/gate-f/`, every
run exit 0); `./heroes run tests/harness/main.hero -- ./heroes-f layout`: `6 passed, 0 failed`;
`./heroes-f test selfhost/main.hero`: **`1733 tests, all passed`**, exit 0. Unrun: the whole
`run`, `unsupported`, `emit`, `emission` and `determinism` forms, the net, the census, the CI's
legs (section 9 says which are owed and why).

## 8. The cost, counted

**Clang runs per build** (a wrapper first on `PATH`, `ce-work/countpath/clang`, logging each run,
since `cli/process.hero` reaches clang through `execvp`; each program in a fresh copy, a cold build
then a warm one; `ce-work/count/`):

| program | today cold / warm | full route cold / warm |
|---|---|---|
| `plain.hero` (`print(1)`, no header) | 5 / 2 | 5 / 2 |
| `run/fixedbugs-2072-values-c-defines-still-build` (header values, builds) | 12 / 3 | 12 / 3 |
| `d2137.hero` (refused in its unit) | 13 / 7 | 18 / 8 |
| `d2136.hero` (refused before any unit) | 10 / 5 | 15 / 6 |
| U1 (refused in its unit, a static assertion) | 16 / 7 | 21 / 8 |
| U1 under route A alone (`heroes-ae`) | 16 / 7 | **18 / 9** |

- **A build that is not refused pays nothing**: the same clang runs, cold and warm.
- **A refused build that binds a header value pays 5 clang runs cold and 1 warm, per unit asked**:
  the copy preprocessed (`-E`), the probe compiled (`-x cpp-output`), linked with `driver.c`, the
  copy preprocessed again with its `#define`s kept (`-dD`, for quoting the header's line, only where
  the verdict refuses), and C2's `-ast-dump=json`; plus the probe binary's runs, one per trap and one
  that finishes (3 for d2137: the undefined canary, `K_DIV`, the end). Warm, the verdict is kept by
  its key and only the `-dD` preprocess runs again, which `value_ask.defined_text` does today for
  every refusing verdict (`d2137-next.hero`'s warm build pays it now). With E, per refused unit
  that defines an asked accessor; with F, per module that does.
- **Route A alone pays for nothing where its copy cannot compile**: on U1, 2 runs cold and 2 again
  on **every** warm build, because a probe that cannot compile is kept under no key; the cut answers
  U1 for 5 cold and 1 warm.
- **What moves in the emitted C and the keys**: nothing a build compiles. `grep -c hero_probe_cut`
  is 0 on every `build/tu-*/*.c` of U1's build; the fused path's writer never sets `cuts`. Only the
  copy's text moves, and with it only the copy's own `value-` and `init-` keys.

**Lines** (`git diff --stat` in my copy, tests included): 9 files, **327 insertions, 48 deletions**:
`cli/value_early.hero` +108 (new), `emit/probe_cut.hero` +85 (new, 3 tests), `cli/value_beside.hero`
+36 -30, `cli/rounds.hero` +37 -2, `emit/unit.hero` +19 -3, `emit/writer.hero` +17 -1,
`cli/assemble.hero` +16 -11, `cli/value_run.hero` +5 -1, `emit/decls.hero` +4. By the `layout`
suite's own rule (`code_lines`: test blocks and blank lines out, comments in): `rounds.hero` 258 to
292, `emit/decls.hero` 296 to 300, `emit/writer.hero` 284 to 298, `emit/unit.hero` 197 to 211,
`assemble.hero` 250 to 254, `value_run.hero` 232 to 235, `value_beside.hero` 162 to 164; the two new
files 57 and 101. **Three files land at the threshold**: `layout` passes, and the next change to
`rounds.hero`, `decls.hero` or `writer.hero` owes a split.

## 9. Step 4 built: a refused accessor is cut too, and the landed 2128 writer is retired

**FOUND: the landed repair of defect 2128 tells a false message, measured on my unmodified stage
2.** Its copy writes a refused constant's accessor inert (`emit/constant_accessor.inert`) in the
unit's text, **before** preprocessing, which is route B's and C's hole on a smaller stretch.
`ce-work/inert/bad.h`: `#define K_BAD (__COUNTER__ * 0 + (1 << 40))` (the region refuses it at its
own accessor) and `#define K_CTR (__COUNTER__ < 3 ? (-2147483647 - 1) / -1 : 0)`; `inert.hero`
binds both. The program's own unit, preprocessed under the build's own words (`clang
@build/words/words-2c0e949969d09fd4.rsp -E build/tu-55ba060e5921fb27/inert.c`), returns
`(2 * 0 + (1 << 40))` for `K_BAD` and **`(5 < 3 ? (-2147483647 - 1) / -1 : 0)`** for `K_CTR`, which
is 0. `./heroes build inert.hero`: `K_BAD` **and `K_CTR`**, exit 1, the `K_CTR` message saying *the
program's own unit ... traps where it reads it*. **False of the program as written**, which
design.md §4.17 calls *a defect that blocks*. Its two corrections, run: dropping `K_BAD`'s binding
(`inert-only.hero`) makes `K_CTR` undefined (refused, exit 1, true there); keeping it with a value C
defines (`inert-fixed.hero`, `1 << 20`) builds, exit 0, and prints `1048576`, so `K_CTR` is 0 there.
The inert copy's word is a guess about a program nobody wrote. The same preprocessing of `ctr.hero`'s
own unit returns `(5 < 3 ? ...)` for `K_CTR` too, which is the measurement behind § 6's `K_CTR` line.
**Not filed: the coordinator's to file**, `blocking` by § Bounded discovery (*a false message*),
with `ce-work/inert/` as its reproducer.

**The repair, built**: a refused constant's accessor is written as the program writes it, **inside**
a marked stretch, so it is preprocessed with the rest and cut after; the copy's `Unit.defined`
leaves it out, so the dispatcher never calls it. `constant_accessor.inert` and its test are deleted
(-42 lines); `unit.unit`'s `inert` becomes `cut`. The probe's copy is then the program's own unit,
preprocessed whole, every time. **Corrected 2026-10-19 (the union step)**: preprocessed under the probe's
words, one more than the unit's (defect 2146).

Built: `./heroes build selfhost/main.hero -o heroes-g`, `real 39.09 user 38.26 sys 0.65`, `sources
2a4331af563a2a10`. **This is the route I adopt.** Every program in a fresh directory:

| program | today (stage 2) | **adopted route** |
|---|---|---|
| `d2137.hero` | `narrow` | `narrow` + `K_DIV` |
| `d2136.hero` | `fill` | `fill` + `K_DIV` |
| S1 | `narrow` | `narrow` + `K_DIV` (`K_NONE`, 2143's other half, waits) |
| S2 | `K_NONE` + `K_DIV` | the same |
| U1 | union | union + `K_DIV` |
| L1 | lock | lock + `K_DIV` |
| `ctr.hero` | `narrow` | `narrow` + `K_DIV`, no `K_CTR` |
| `inert.hero` | `K_BAD` + **`K_CTR` (false)** | **`K_BAD` alone** |
| `d2137-next`, `d2136-next`, `ctr-next` | `K_DIV` | the same |
| `ctr-only`, `inert-fixed` | exit 0 | exit 0 |
| `inert-only` | `K_CTR` | the same |

**Gates on `heroes-g`**: the critic's 17 cases, **17 of 17** (`ce-work/gate-g/`); `layout` `6
passed, 0 failed`; `./heroes-g test selfhost/main.hero`: **`1732 tests, all passed`**, exit 0 (one
fewer than step 3's 1733: the retired writer's own test). `./heroes check selfhost/main.hero` clean.

**No build's C moves, byte for byte** (`ce-work/same/`, six programs that build, among them two
modules in `s1-next.hero`, built by stage 2 and by `heroes-g`): all six exit 0 with either; `heroes
build <p> --emit-c` identical by `cmp` for all six (17,290 to 18,168 bytes); the 13 per-module units
each build wrote (`build/tu-*/*.c`), compared by SHA-256 as a multiset since a unit's directory is
keyed by the compiler, **the same 13 contents**; `hero_probe_cut` appears in none.

**Clang runs, recounted on `heroes-g`** (`ce-work/count/g-*`): `plain` 5/2 and the 2072 case 12/3,
unchanged; d2137 18/8, d2136 15/6, U1 21/8 against today's 13/7, 10/5, 16/7; `inert.hero` **16/9
against today's 17/10**, one fewer, since its copy no longer refuses `K_CTR` and quotes nothing.

**Durations** (`/usr/bin/time -p`, three runs each, compilers alternating, load average 1.7 to 2.2,
`ce-work/count/timing.txt`): `plain` 0.26 s cold and 0.09 s warm with either compiler; d2137 0.41 to
**0.95-1.02** s cold, 0.18 to 0.22 s warm; d2136 0.35 to **0.88-0.96** s cold, 0.14-0.15 to
0.18-0.19 s warm. The compiler building itself, warm (each compiler warmed once first): today 34.98,
34.98, 34.97 s real; adopted 34.91, 34.95, 34.87 s, `user` within 0.3 s of `real` in every run.
**No build that is not refused is slower; a refused build that binds a header value pays about
0.6 s cold and 0.04 s warm per unit asked.** How many units: the compiler's own build asks the
probe of 3 modules (`cli/process`, `cli/runtime_places`, `module/reading`: the `.c` names in its
`build/value-*` directories).

**Lines of the adopted route** (`git diff --shortstat`, tests and comments included): **10 files,
348 insertions, 107 deletions, net +241**; the code alone (comment, blank and test lines out of the
diff): +215 -68.
**Corrected 2026-10-19, the same hour**: that count took comment and blank lines out of the diff and
left the test lines in; it is not *the code alone*.
 New: `emit/probe_cut.hero` 85 (35 lines of code), `cli/value_early.hero` 108 (76,
most of them one argument per line). Scale: `selfhost/` is 674 files and 175,346 lines at
`7b97b9438`, `selfhost/cli/` 123 files and 32,989, the value-probe family (`cli/value_*`,
`emit/value_*`, `constant_accessor`, `record_constant`) 11 files and 2,447 lines; this adds about a
tenth to the family and 0.14% to the compiler. `layout`'s count: `rounds.hero` 293, `decls.hero`
300, `writer.hero` 298, each passing and each at the threshold.

## 10. What the adopted route owes before it lands (`.claude/rules/verification.md`'s map, read 2026-10-19)

- **Its cases**, each annotated in its source: `unsupported/` cases for d2137, d2136, U1, L1, S1's
  second unit, and the two attacks, `ctr` (the guard that a copy is never cut before preprocessing)
  and `inert` (2128's false message); their `.expected` files are new, never regenerated.
- `selfhost/**`: `canonical`, `layout` (run, 6/0), `order`, `probe`, `records`, `seed`, `spec`, the
  compiler's own tests (run, 1732). `selfhost/emit/**`: `emission`, `emit`, `determinism`, `wholes`,
  `descriptors`, `lldb`. Measured here only the byte identity on six programs and 13 units; the
  suites themselves are **unrun**.
- **The whole `unsupported` and `run` forms**, because the route changes which messages a refused
  build prints: any golden program refused with a header value bound beside its refusal now tells
  that value too, and its `.expected` must say so or the case shows the route wrong. **Unrun.**
- The CI's legs after the push: the probe's words, the cut and the marks are clang's, and Linux and
  Windows are **unrun**. `-w` lifting a pragma-made error and `-E` writing an unknown pragma back
  verbatim are measured on Apple clang 21 only.

## 11. Verdicts

| route | verdict | why, in one line |
|---|---|---|
| **A**, the refused unit as it is | **object** as the resolution | exact (the program's own text) but reaches only a warning promoted to an error: U1 and every hard error stay a build late, and a copy that cannot compile costs 2 clang runs on every warm build for no answer (U1: 18/9 against 16/7). Its principle is kept whole in C-prime, which answers wherever A does. |
| **B**, the binding written inert | **veto** (soundness) | removes the binding's text before preprocessing, so a `__COUNTER__` the binding expands moves: measured false `K_CTR` message (§ 6), against §4.17; and it needs an inert form in every emitter module that writes header-reading text, adding special cases where §1.7 asks to remove them. |
| **C**, a unit of the constants alone, written | **veto** (soundness) | the same measured false message: a probe text of its own, panel 216's F and F2 again. |
| **C-prime**, C cut after preprocessing | **adopt** (built) | the program's own unit preprocessed whole, then cut: every reading exact (but defect 2146's word: corrected 2026-10-19), every error outside an accessor gone, one mechanism for every refused unit and four pre-unit refusals, and the 2128 inert writer retired. |
| **D**, left as it is | **object** | 0 lines, and the tag: 2137 counts `blocking` at the next tag, and the form of D that lets one be placed writes §4.17, a full sitting. A measured repair exists at +241 lines. |
| **E**, every refused unit | **approve** (built) | the probe half of 2143 for 16 lines in `assemble.hero`; the second unit's own refusal is 2143's other half, a change to `whose.of_refusal`'s input, and not here. |
| **F**, beside a pre-unit refusal | **approve** at `check_all`, `lock_told`, `two_ways`/`defined_twice` (built: 2136, L1); **object** at `bound_macro` and `pragma_ask` | each of those two refuses because the unit's reading of a name cannot be trusted; reasoned, **unrun**. |

**The veto, and what it stands on.** My seat's charter vetoes a core construct or a breach of the
ceiling, and no route here adds a construct the checker, the lowering and the backend must all
handle: every route is the driver's and the emitter's, erased before any program runs. The veto on
B and C stands on the sitting's soundness rule (CLAUDE.md § 4, *seats veto on soundness*) and on
design.md §4.17, panel 187's sentence: *every message is true of the program as written ... a
message that breaks either is a defect that blocks*. The `K_CTR` message is measured false of the
program as written. **The veto compels a written answer, not a halt**: B or C answered by
preprocessing the whole unit before leaving anything out is C-prime.

**The lane's condition holds**: no route I adopt writes into design.md §4.17 or spec § 13. Every
message the adopted route adds is `ffi_constant_type`'s existing words, true of the program's own
unit as written, the unit the build tried to compile; what it changes is when it is told.

**Residue, said rather than hidden**: (1) S1's `K_NONE`, a second refused unit's own refusal, still
waits (2143's other half). (2) Beside `bound_macro` and `pragma_ask` the value waits a build (above).
(3) Before any unit there is no clang message to find a refused accessor in, so a unit whose own
accessor holds a hard error (a name the header lacks) does not compile even cut, and its values
wait, as today. (4) A header can write the mark itself (`_Pragma("hero_probe_cut on")`); every such
case I can reason ends in a copy that does not compile, silence and not a false message, and a
per-copy key in the mark would close it for about 5 lines, **unbuilt**. **Corrected 2026-10-19 (the union
step)**: false. The critic's second pass ran it (`critic.md` § 3.1): a header holding this mark
made `heroes-g` tell `K_SZ` falsely beside a refusal (`forge2c`, `forge3c`, `forgeF`) and refuse a
correct program at exit 2 (`forge1`), because the cut ran on every probe's text. The union
(`compiler-engineer-union.md`) confines the cut to a copy and keys and matches the marks. (5) The marks shift the
copy's generated `#line` numbers; a value is always expanded under its constant's own `.hero` line
(`constant_accessor.hero` line 75, `record_constant.hero` line 121), so the shift can make an
accessor's assertion differ only toward a copy that does not compile. (6) Three files land at
`layout`'s threshold. (7) Reasoned and **unrun**: a cut stretch holding a pragma that changes the
state of the rest of the unit (`pack`, `clang attribute push`); in a build that reaches its units
`pragma_ask` has refused every name the compiler writes whose macro holds a `_Pragma`, and the
compiler's own pragmas inside cut stretches are diagnostic ones in the units I read.

## 12. Condition and prediction

**Condition.** I turn from adopt to object on C-prime if a program is shown whose asked constant
the cut copy judges otherwise than the program's own unit as written, the refusal left in place,
by any run (a pragma's state across a cut, residue 7, is where I would look first); or if the
whole `unsupported`, `run` or `emission` form moves anything but the new cases. I withdraw the veto
on B and C if either is restated to preprocess the whole unit before leaving anything out (which is
C-prime), or if the author rules that a value told beside a refusal is a statement about the
corrected program rather than the program as written, a ruling that writes §4.17 and convenes the
sitting in full.

**Prediction.** Landed as built here, the `emission` suite moves **0** blessed files and `emit`
**0** goldens, and each of the seven new `unsupported` cases (d2137, d2136, U1, L1, S1, `ctr`,
`inert`) tells its value in the same build as its refusal, `ctr` telling no `K_CTR` and `inert`
telling `K_BAD` alone. Instrument: the landing lane's gate, `./heroes run tests/harness/main.hero
-- <compiler> emission`, `emit` and `unsupported`. Scored at that gate, before `m-witnessed-seed`
is placed.

**First pass complete, 2026-10-19.** My copy stays at
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-compiler-engineer` for the coordinator
to remove: its diff is the adopted route (`heroes-g`), with `heroes-ae`, `heroes-cut`, `heroes-f`
and the two `heroes-before` models built beside it, every program and log under `ce-work/`.
