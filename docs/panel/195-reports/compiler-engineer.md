# Panel 195, compiler-engineer

Started 12:22:42 (`date`). Copy: `<scratchpad>/195-compiler-engineer/`, from
`git archive 96f3a588 | tar -x`, `round-b12/run-4254/seed-new.c` over
`seed/heroes.c` (sha1 2687685540fe...). Instruction counts only, no timing.
Findings are added below as they are run.

## Written 12:31:25 — the copy, and what the code says

Built 12:22-12:24: `heroes-seed` from `seed-new.c` (exit 0), then
`./heroes-seed build selfhost/main.hero -o heroes` ("wrote heroes", exit 0),
kept as `heroes-branch` for the baseline.

Read (line counts by `wc -l` in the copy): `selfhost/ir/lower.hero` 631 (the
constant at :77-97 is `build.begin(... .constant_kind ...)` and `flatten.block`),
`selfhost/emit/container.hero` 464 (`build_array` :57-81, `hero_array_new` +
one `hero_array_push_owned` per element), `selfhost/emit/construct.hero` 180,
`selfhost/emit/body.hero` 357 (`definition` :79, the one place both the fused
unit, `emit.hero:299`, and the per-module unit, `emit/unit.hero:211`, write a
body), `runtime/parts/array.c` 341, `cow.c` 126, `drop.c` 153.

`--dump-ir` of a two-constant program (`w/small.hero`): a constant's IR is ONE
block of `const` instructions, one `construct array(...)`, and the ownership
pass's `load/store/decref/incref/decref_slot` around `$own0`, then
`return`. The reader's IR is `call heroes K()`, owned, released by the caller.
So the read needs no lowering change at all: a constant function that returns a
block whose count never moves is released by the caller exactly as today, and
the release is the guard's no-op. **The route is erased in the emitter; the
checker, the lowering, the ownership pass and the IR do not move.**

Every write the runtime makes to an array header, by `grep -rn refcount runtime`
and `grep -rn 'hero_array_data(\|->len = \|->len +='`: `array.c:57` (new),
`:68` (incref), `:84` (decref), `drop.c:84,92` (the link, after decref
dooms the block), `cow.c:44` (test ==1, then copy), `cow.c:78,83` (test ==1
before an in-place push), `cow.c:120` (after unshare). Every emitted element
write is `hero_array_unshare` or `hero_array_set` first
(`emit/container.hero:148-233`). So with the guard in incref and decref, no
path writes a block whose count is -1: unshare and push_owned see a count that
is not 1 and copy.

## Written 12:34:36 — the baseline, this branch's compiler, the original runtime

`base/`: `HEROES_RUNTIME=runtime.orig ../heroes-branch build <f>.hero [-O2]`, then
`/usr/bin/time -l`, instructions retired, run 12:34:22-12:34:25. `ks.hero` is my
`[str]` twin: 32 string literals, `sum + K[at % 32].len()` a million times;
`locals.hero` binds `k = K` once above the loop. Every binary printed what its
twin printed (4843750 for the ints, 5937500 for the strs).

| program | -O0 | -O2 |
|---|---|---|
| `k` (landed, `f7576a01`) | 4,596,876,585 | 2,577,431,397 |
| `local` (floor) | 116,542,863 | 38,382,216 |
| `ks` (landed) | 8,518,428,608 | 3,389,239,402 |
| `locals` (floor) | 142,543,550 | 43,456,976 |

## Written 12:35:28 — item 2, the trade in hero_array_decref, run

**Every way a count goes negative today** (`grep -rn '{-1' runtime selfhost`,
`grep -rn refcount runtime`): two, both `str`: `HERO_STR_STATIC`
(`heroes_runtime.h:202`) and `hero_empty_block` (`str.c:27`). No array or map
count is set negative anywhere, and the emitter writes no count
(`grep -rn refcount selfhost` finds only `guarded_names`'s word). An array's
count reaches 0 or below only by a release after the last, in freed memory.

**The guard I built** (`runtime/parts/array.c`, +25 lines): `hero_array_counted`
loads the count relaxed; above 0 it is counted, exactly -1 is a constant's
static block and both entry points return, ANY OTHER value below 1 is a
release after the last and panics by name. And `decref`'s `fetch_sub` answer
below 1 (two releases racing for one reference) panics the same way, where
today's `> 1` sent it to the drop list and freed it again.

The critic's `dbl.c` (a block, released twice), on both runtimes, exit codes
by `$?`:

| | 32 elements | 4 elements |
|---|---|---|
| today's runtime | SIGTRAP from malloc, exit 133 | silent, exit 0 (word after free 5) |
| this guard | `panic: an array released after its last reference`, exit 134 | silent, exit 0 (word 5) |
| this guard, `-fsanitize=address,undefined` | `heap-use-after-free`, exit 134 | `heap-use-after-free`, exit 134 |

And the critic's `ro.c` (a `static const` block, count -1, in
`__DATA_CONST`): today SIGBUS exit 138 at both; with the guard `decref` and
`incref` both return, exit 0.

**So the guard makes no memory error silent that is loud today**: on a heap
block it is louder (a name where malloc trapped, the same silence where malloc
let it pass), and ASan still names both, because the guard's own load is the
instrumented read of freed memory. What it does make invisible is a release
too many **of a static block**, which frees nothing and moves no count: it is
not a memory error, it is the class `str` literals have lived under since
`HERO_STR_STATIC`. The emitter bug that would cause it (the caller releasing
a call's owned result once too often) is not constant-specific: the IR read is
`call heroes K()`, the same owned call result every function returns, so every
other call in every `--sanitize` run still witnesses that class.

## Written 12:36:30 — item 1, the route built: a static const block per literal array

**What I built, in the copy** (the stage-one compiler `heroes-stage1` was built
by `heroes-branch` with the header's stamp held at 27, then the stamp set to 28):

- `runtime/heroes_runtime.h`: `HERO_RUNTIME_ABI` 27 -> 28, and three macros
  beside `HeroArrayHeader`: `HERO_ARRAY_STATIC(name, type, elem, n, ...)`
  (`static const struct { HeroArrayHeader h; type b[n]; }`, count -1, and a
  `_Static_assert` that `b` sits at `sizeof(HeroArrayHeader)`, where
  `hero_array_at` reads), `HERO_ARRAY_STATIC_EMPTY(name, elem)` (the header
  alone: C11 has no zero-length array), `HERO_ARRAY_LIT(name)`.
- `runtime/parts/array.c`: the guard of item 2.
- `selfhost/emit/static_constant.hero` (new): walks a constant's IR. Lays it
  out only if it is ONE block of `lit`, `construct` (array, record, case) and
  the ownership pass's `load/store/incref/decref/decref_slot`, returning an
  array; any other op (call, binary, unary, index, funcref, map, ...) or any
  branch leaves the body to the landed route. Hooked at the top of
  `emit/body.hero`'s `definition` (+3 lines), the one place both the fused
  and the per-module units write a body.
- `heroes_guard_open.h`/`_close.h` (+4 pragma lines each),
  `emit/guarded_names.hero` (+4 words: the three macros and the parameter
  `type`), `emit/decls.hero` (the stamp, 2 lines).

Emitted for `w/small.hero` (`heroes-stage1 build --emit-c`):

    HERO_ARRAY_STATIC(hero_constant_h_small_K_4, HeroStr, &hero_desc_str, 3, HERO_STR_LIT(hero_str_6bb5e50a), ...);
    HeroArrayHeader * h_small_K(void) {
        return HERO_ARRAY_LIT(hero_constant_h_small_K_4);
    }

`heroes-stage1 run small.hero` and `--sanitize`: `mars`, `4`, exit 0 both.

**Instructions retired** (`route/`, `heroes-stage1 build [-O2]`, the copy's
runtime with the guard, `/usr/bin/time -l`, 12:35:57-12:36:03), every binary
printing its twin's answer:

| program | -O0 | -O2 | against landed -O0 / -O2 | against floor -O0 / -O2 |
|---|---|---|---|---|
| `k` | 165,809,911 | 56,367,147 | 27.7x / 45.7x fewer | 1.42x / 1.47x |
| `local` | 116,541,100 | 38,394,813 | | |
| `ks` | 191,550,500 | 61,365,850 | 44.5x / 55.2x fewer | 1.34x / 1.41x |
| `locals` | 142,500,041 | 43,383,811 | | |

Against ir12's hand-written static block (CARRIED, 142,668,198 / 52,641,845) the
emitted route reads 23 million more at -O0: the read is still a call and its
owned result still goes through the slot's load, store and `decref`, which
now returns on the guard. Unmeasured which part is which.

## Written 12:40:45 — item 3, the shapes, each run

All in `<copy>/shapes/`, built by `heroes-stage1` (the route) against the copy's
runtime, and every program's output diffed against the same source built by
`heroes-branch` against `runtime.orig` (the landed route): identical in each
case below. "Laid out" is counted by `grep HERO_ARRAY_STATIC` in `--emit-c`.

| shape | what the route does | run |
|---|---|---|
| `[i64]` (`k`), `[str]` (`ks`) | laid out | the tables above |
| `[[i64]]` with an inner `[]` | laid out, inner blocks first, outer holds `HERO_ARRAY_LIT` of each | `laid.hero`, exit 0, same output, `--sanitize` exit 0, stderr empty |
| `[Variant]`, a payload `str`, a payload `[i64]`, a payload-free case | laid out (`aggregate.construct_case`'s compound literal, which clang takes in a static initializer: `cprobe/init.c`, 0 warnings even under `-pedantic`) | `laid.hero`, as above |
| `[Record]` holding a `str` | **refused by the language today**: `error[constant_body]: ... may not contain a call` at the constructor (`heroes check`, exit 1); the route would lay it out through `aggregate.construct` (built, unexercised) | not a program |
| `[f64]`, `[u8]`, `[i8]`, `[bool]`, `INT64_MIN` | laid out | `laid.hero` |
| empty `[]` | `HERO_ARRAY_STATIC_EMPTY` | `laid.hero`: `.len()` 0, a push onto a copy gives 1 and leaves the constant 0 |
| a write to a copy: `xs[0] @ 99`, `ys[2][0] @ 7`, `vs[1] @ ...`, `push`, `@` parameter | the static block is copied first (count not 1), the constant unchanged | `laid.hero`: `99 1`, `7 3`, `1 0`, `4 4` |
| `==`, `sort`, as a map key, `.slice`, `.push` returning new | read-only on the block | `laid.hero`: `true true`, sorted, `1`, `1`, `13` |
| `{str: i64}` map, and one with `BASE + 2` | built at every read (the map's layout is the runtime's hash) | `built.hero`: 42 42, exit 0, `--sanitize` exit 0 |
| `if` body, `f"a{BASE}b"` element | built at every read (a branch; calls) | `built.hero`: 80, a40b |
| a body naming another constant: `[INNER, INNER]`, and `INNER` alone | built at every read (a call), whose call returns `INNER`'s static block | `built.hero`: 2, 1 |
| `[(function(i64) -> i64)]` with `hero_thread_spawn` bound | built at every read: the `funcref` stays in the IR and `twice`/`thrice` keep `hero_thread_guard("built.twice")` (`--emit-c`, lines 552, 577) | `built.hero`: 25 25 |
| an index that aborts on a laid-out constant | aborts at the read | `aborts.hero`: prints 3, then `panic: array index out of range`, exit 134 |
| a computed body that aborts, `[PICK[1], PICK[5]]` | built at every read, aborts at the read (PICK itself laid out) | `aborts2.hero`: `before`, then the panic, exit 134 |
| read by two `hero_thread_spawn` workers and `main` | laid out; the count never moves | `spawnk.hero` (the critic's): 1453125 exit 0; `--sanitize` exit 0; the emitted C under `clang -fsanitize=thread -O1` with the copy's runtime: exit 0, **0** ThreadSanitizer warnings |

## Written 12:44:52 — item 4, the ABI, run both ways

**Yes, the route needs a runtime that skips a static array block**: today's
`incref` and `decref` write the count first and fault (the critic's `ro.c`,
exit 138); this copy's return (`cprobe/ro-new`, exit 0).

**The skew is a compile error both ways, and I moved the stamp to 28.** The
emitted C names three macros only the new header declares, which is the
precedent `hero_thread_guard` set in this same header (`heroes_runtime.h:61-67`,
*declared HERE, under the ABI stamp, on purpose: a runtime that predates the
guard must be a compile error*) and the one defect 245 followed when
`HERO_STR_MAGIC_NUL` moved the stamp (`6b33db23`, by `git log -G'define
HERO_RUNTIME_ABI'`). CL-007 refuses the stamp to *behaviour*; this is a
declaration the generated C uses, which is the stamp's job as written ("a header
from another compiler"). Measured, `clang -fsyntax-only`:

- the route's C (`w/small-route.c`) against `runtime.orig`: `static assertion
  failed due to requirement '27 == 28': heroes_runtime.h is from another compiler`;
- the same C with its stamp line edited back to 27: `unexpected type name
  'HeroStr'`, `call to undeclared function 'HERO_ARRAY_LIT'`. So even a stamp
  left at 27 would not have produced the silent SIGBUS; the bump is what makes
  the message name the cause;
- `heroes-branch` (emits 27) against the new header: `requirement '28 == 27'`
  (12:44, `build selfhost/lexer.hero --emit-c`, exit 2).

**Words added to the runtime's headers**: `HERO_ARRAY_LIT`,
`HERO_ARRAY_STATIC`, `HERO_ARRAY_STATIC_EMPTY`, and the macro parameter
`type`. Every other word the macros write (`name elem n h b offsetof static
const struct HeroArrayHeader`) is already in `NAMES`; `__typeof__`,
`__VA_ARGS__`, `_Static_assert` are reserved and `macro_guard.used` skips
them. So yes, `emit/guarded_names.hero` and both guard files carry the four
(defect 361's test *the list holds every word the runtime's headers write*
would name each one otherwise). The emitter's own text uses them after
`heroes_guard_close.h`, where they mean what the runtime said. The element
member is `b`, the name `HERO_STR_STATIC` already uses, so no `e` enters
(the critic's F6). Cost of the bump outside code: every `tests/emission/*.c`
carries the stamp line, 398 files by `grep -rl 'HERO_RUNTIME_ABI == 27' tests`,
re-blessed by `UPDATE_EMISSION=1` as every stamp move before it was (8 moves
since 2026-09-04 by `git log -G`), and the seed.

## Written 12:53:21 — the strongest case against my own route, and what answers it

1. **A runtime path added later that writes an array without asking its count
   faults with no name.** `runtime/parts/stack.c:437-450` passes a SIGBUS that
   is neither a stack overflow, a null read nor a dead handle on to the default,
   so it dies at exit 138 with stderr empty (the critic's `ro.c` on today's
   runtime). Today no such path exists (item 2's grep), and the alternative,
   `static` instead of `static const`, turns that same missed guard into a
   count moved on a block every thread shares: at 0 `decref` would free memory
   malloc never gave, at 1 `cow.c:44` would write the constant in place and
   the program would print a wrong value at exit 0. A fault beats a wrong
   answer (design.md §1.12), so `const` stands, and the condition below asks
   for the test that keeps the paths honest.
2. **A release too many of a constant's value becomes invisible, even to ASan.**
   Answered in item 2: not a memory error, `str`'s existing class, and the
   emitter path that would cause it is the generic owned-call-result path that
   every other call still exercises under `--sanitize`.
3. **A missing release of a constant's value is no longer a leak.** Same answer:
   the leak check still sees the identical path on every heap result.
4. **240 lines for 8 loop sites, 1 in `selfhost/`.** The cost lands in one new
   emitter module (211 `code_lines`, of which ~45 are its two tests' helpers
   and doc) and 25 runtime lines; nothing in the checker, the lowering, the
   ownership pass, the IR, the verifier or the IR goldens moves, and every read
   of every literal array constant stops allocating, in a loop or not (F8: a
   read in a function called from a loop is as hot, and a text count cannot
   find it).

*Corrected 12:53:35, the "~45" above was not measured.* Measured
(python over the file, the `code_lines` rule): 211 = 187 non-blank lines above
`## Tests` (35 comment lines, 20 `use` lines, 132 of code) + 24 lines of test
helpers outside the two `test` blocks.

## Written 12:55:49 — what the guard costs a program with no constant

`guardcost/shares.hero`: a loop of a million iterations, each a copy of an
array (`incref`), a write to the copy (`unshare`: a block, a copy, a
`decref`), a borrowed call and the copy's release. Emitted once by
`heroes-branch`, compiled against `runtime.orig` and against the copy's runtime
(the stamp line edited to 28, no constant in it, so only the runtime differs).
Seven runs each, instructions retired sorted; **the counts are bimodal by
~72 million on this Mac for the same binary** (malloc's path, by the spread;
unmeasured why), so modes are compared with modes:

| runtime | -O0, lower mode | -O0, upper mode | -O2, lower mode | -O2, upper mode |
|---|---|---|---|---|
| `runtime.orig` | 1,261,420,307 | ~1,334.0M | 772,846,751-774,961,477 | ~845.6M |
| the guard, written inline | 1,297,444,650 | ~1,368.2M | 781,736,294-783,757,696 | ~854.3M |
| | +2.9% | +2.6% | +1.1% | +1.0% |

My first version called a helper from both entry points and cost +7.0% at -O0
on one run (1,262,295,408 -> 1,351,015,827), so the guard is now written out in
`incref` and `decref` (one relaxed load, one compare on the hot path: the same
as ir12's `< 0` line and `str.c`'s). The panic for counts below 1 sits on the
cold side of that one compare, so the louder double release costs nothing over
ir12's guard. Re-run against the inline version: `dbl` 32 elements
`panic: an array released after its last reference` exit 134, 4 elements exit
0, `ro` decref and incref exit 0; `clang -Wall -Wextra -fsyntax-only
runtime/runtime.c`: 0 diagnostics.

## Written 12:56:40 — the reproducers on the inline guard, three runs each

`route/`, rebuilt by `heroes-stage1` against the inline guard, instructions:

| program | -O0 (3 runs) | -O2 (3 runs) |
|---|---|---|
| `k` | 148,654,023 · 147,864,040 · 147,898,528 | 55,449,703 · 54,852,822 · 54,806,724 |
| `ks` | 174,487,251 · 173,910,336 · 173,864,145 | 60,380,030 · 59,828,488 · 59,869,720 |
| `k` landed (base, re-run) | 4,597,113,177 · 4,596,023,179 · 4,596,011,908 | |
| `local` floor (base, re-run) | 116,743,630 · 115,858,217 · 115,869,786 | |

So **`k` reads at 1.28x the floor at -O0 and 1.43x at -O2 (38,382,216); `ks`
at 1.22x (142,543,550) and 1.38x (43,456,976)**, from 39x/67x and 60x/78x. The
inline guard took 18 million off `k`'s -O0 count (165,809,911 with the call).
ir12's hand-written C (CARRIED) read 142,668,198 / 52,641,845.

The compiler's own `--emit-c` of the pristine `96f3a588` `selfhost/main.hero`
(`input/`), `heroes-branch` against `runtime.orig`: **1,153,913,075,602**
instructions, exit 0, 36,930,128 bytes (12:44:24-12:56:04; the machine was
shared with another session's clang, which moves no instruction count). The
route's twin is being built with the inline guard; the call-guard run was
stopped as obsolete.

## Written 12:58:23 — what the other routes would cost, measured where it can be

- **A new IR op** (the critic's route 1 as a lowering of `K[i]`, `K.len()`,
  `for x in K`): an op joins every exhaustive match over `ir.OpKind`; the
  newest op, `decref_slot`, is matched **80 times in 28 files** of `selfhost/`
  (`grep -rn '\.decref_slot' input/selfhost | wc -l`, files by `-l`), among
  them the verifier, the ownership pass, the printer and `emit/inst.hero`. Its
  `--dump-ir` changes, and `tests/golden/ir/` is a hard stop for
  regeneration (CLAUDE.md § Hard stops). And `f(K)` and `x = K` still
  build. Done instead as a peephole in the emitter, it is data flow in a module
  whose own header says it is a printer (`emit.hero:6-9`, "No analysis, no
  optimisation"; that is the module doc, and design.md's nearest sentence is
  §2's line 655, *the entire LLVM optimisation pipeline at -O2 — a welcome side
  effect, not a reason*). Unbuilt by me; the cost is the op count above.
- **Hoisting a read out of a loop**: lands in `ir/flatten.hero`, the knot of 25
  mutually recursive functions (`ir/lower.hero:11-19`), changes the IR of
  every program that reads a constant in a loop (the same goldens), and serves 8
  lexical sites (the critic's F8) and not a read in a called function. Unbuilt.
- **One copy per process, CAS-published, in writable memory**: serves the
  shapes the static route leaves (computed bodies, maps; **zero** of the 66 in the
  tree, by the critic's grep), at the price of a once-protocol, a registry
  released before the leak check, the guard in `map.c` as well, and a failure
  mode that is silent where the static route's is a fault: a missed guard moves
  a count on a writable block every thread shares. Unbuilt.
- **One copy per thread**: CARRIED 195,594,982 at -O0, above this route's
  147.9M, plus a per-thread release at thread exit and before the leak check
  on three platforms. Unbuilt by me.

*Corrected 12:58:33*: the design.md sentence at line 655 is in **§3.1
Backend: C emission (C11)** (by `awk` over the headings), not "§2".

## Written 12:59:41 — the route compiler's own tests, and determinism

- `./heroes-route-inline test selfhost/main.hero` in the copy (12:57:52-12:59:22):
  **1288 tests, all passed**, exit 0, among them my two
  (`"literals are laid out, the inner block first, and the read is the
  address"`, `"a computed body, a branch and a function value are built at
  every read"`), `macro_guard`'s two over the guard files and the runtime's
  words (so the four new words are where defect 361's test wants them), and
  `emit.hero`'s *the same bytes twice*.
- `--emit-c` of `laid`, `built` and `spawnk` by `heroes-stage1` (built by the
  landed compiler) and by `heroes-route-inline` (built by the route):
  byte-identical (`cmp`), and identical on a second emission.
- `heroes-stage1` built `heroes-route-inline` (exit 0), which builds and runs
  every program above: the compiler compiles itself with its own 23 array
  constants (19 files, `grep -rhE '^constant [A-Z_0-9]+: (\[|\{)' input/selfhost`)
  laid out by the route.

## Written 13:08:58 — item 5, the cost to the compiler

**Instructions**, `build selfhost/main.hero --emit-c` over the pristine
`96f3a588` tree (`input/`), `build/` emptied before each run, each compiler
against its own runtime, three runs each, every run exit 0:

| compiler | run 1 | run 2 | run 3 | bytes of C |
|---|---|---|---|---|
| `heroes-branch` (landed, `runtime.orig`) | 1,153,913,075,602 | 1,148,054,169,377 | 1,148,149,513,348 | 36,930,128 |
| `heroes-route-inline` (route + inline guard) | 1,133,768,469,679 | 1,139,877,212,992 | 1,139,992,732,763 | 36,837,196 |

Median against median: **-8,272,300,356, -0.72%**; runs 2 and 3, taken on a
quieter machine back to back, -0.71% each. So the compiler gets faster, not
slower: the guard's +1 to +3% on refcount-heavy paths (the `shares` table) is
more than paid by the compiler's own constants no longer building. **All 23 of
the compiler's array constants are laid out** (`grep -c '^HERO_ARRAY_STATIC'`
and `grep -c 'return HERO_ARRAY_LIT('` in the route's emission: 23 and 23,
against 23 declarations in 19 files). The emitted C is 92,932 bytes shorter,
and stable across runs (`cmp` of run 1 and run 3: identical, both compilers).
The first diff between the two emissions is line 13, the stamp.

**Layout, `code_lines`' unit** (`tests/harness/suite_layout.hero:817`: non-blank
lines outside `test` blocks; replayed by `<scratchpad>/p195-ce-codelines.py`),
before at `96f3a588`, after in the copy, `heroes fmt --in-place` applied:

| file | before | after |
|---|---|---|
| `selfhost/emit/static_constant.hero` (new) | — | 211 (263 lines) |
| `selfhost/emit/body.hero` | 260 | 264 |
| `selfhost/emit/guarded_names.hero` | 133 | 135 |
| `selfhost/emit/decls.hero` | 262 | 262 (the stamp, 2 lines changed) |
| `runtime/heroes_runtime.h` (`wc -l`) | 684 | 709 |
| `runtime/parts/array.c` (`wc -l`) | 341 | 362 |
| `runtime/heroes_guard_open.h` / `_close.h` (`wc -l`) | 632 / 632 | 636 / 636 |

Nothing in `selfhost/check/`, `selfhost/resolve/`, `selfhost/ir/`, the
verifier or `tests/golden/ir/` moves. Outside code: the 398 stamp lines in
`tests/` and the seed.

*Corrected 13:09:11*: `runtime/parts/array.c` is **369** lines after the
inline guard (`wc -l`), not 362; 341 at `96f3a588`. `diff -rq` of the copy
against `96f3a588`: exactly four files under `selfhost/` (`emit/body.hero`,
`emit/decls.hero`, `emit/guarded_names.hero`, the new
`emit/static_constant.hero`) and four under `runtime/` (`heroes_runtime.h`,
`parts/array.c`, the two guard files). The diffs are kept at
`<scratchpad>/p195-ce-array.diff`, `p195-ce-header.diff`, `p195-ce-body.diff`.

## Verdict (written at the time below)

- `verdict`: **approve** the static block, count -1, as built here; **object** to
  every other route on the table; **no veto** (no route I approve adds a core
  construct or breaches the ceiling).
- `section`: design.md §1.7 and Part 5 (erased in the emitter: the checker, the
  lowering, the ownership pass, the IR and its goldens do not move), §1.12 (no
  write reaches read-only memory; a double release is louder than today, not
  quieter), §1.1 (the ceiling: one emitter module).
- `implementation_cost`: `selfhost/emit/static_constant.hero` new, 211
  code_lines (132 of code, 35 comment, 20 `use`, 24 test helpers);
  `emit/body.hero` 260 -> 264; `emit/guarded_names.hero` 133 -> 135;
  `emit/decls.hero` the stamp; `runtime/heroes_runtime.h` 684 -> 709 (three
  macros, ABI 28); `runtime/parts/array.c` 341 -> 369; the guard files +4 each;
  398 stamp lines in `tests/` and the seed regenerated. The compiler's own
  `--emit-c`: -0.72% instructions (median of three).
- `needed_for_self_hosting`: **no** (Principle 0 binds forms; this adds none).
- `argument`: the read is `call heroes K()`, an owned result the caller already
  releases, so returning a block no count moves needs no change above the
  emitter; the guard is one load and one compare, the same hot path as `str`'s.
  Built, it reads 1.28x the hoisted floor at -O0 (39x today), lays out all 23
  of the compiler's own constants, and makes the compiler 0.72% cheaper. A
  release too many of a static block frees nothing: `str`'s existing class,
  witnessed by every other call under `--sanitize`. A heap double release
  becomes a named panic where malloc trapped. Every shape is decided by the IR
  whitelist; computed, map and function bodies keep today's route.
- `prediction`: at the commit that closes defect 382 on this route, `k.hero`
  at -O0 retires at most 1.5x `local.hero` (1.28x here) and the trunk
  compiler's `build selfhost/main.hero --emit-c` retires no more instructions
  than the compiler before it (-0.72% here), by `/usr/bin/time -l`; and
  `emit/static_constant.hero` stays at or under 230 code_lines.
- `condition`: I move to object if (a) Linux arm64 or Windows shows a
  `HERO_ARRAY_STATIC` block written without a fault, or a runtime array path
  writes a header without first testing its count (`grep` at landing); (b) any
  `tests/golden/run` program's output or `--sanitize` verdict differs between
  the landed route and this one; (c) the compiler's `--emit-c` instructions
  rise. The landing owes a run golden that puts every array primitive through a
  laid-out constant (as `shapes/laid.hero` does), plain and `--sanitize`.

### Per route

| route | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| static block, count -1 (ir12), as built here | **approve** | §1.7, Part 5, §1.12 | 211 code_lines (one emitter module) + 4 + 2; runtime +25 header, +28 `array.c`, +8 pragmas; ABI 28; compiler -0.72% | `k` <= 1.5x `local` at -O0 at 382's closing commit; compiler `--emit-c` not up | (a)-(c) above |
| nothing more, `f7576a01` alone | object | §1.1 (the ceiling is no reason to stop: the route above fits under it) | 0 | on any trunk without a further route, `k` stays >= 30x `local` at -O0 | the static route failing on Linux arm64 or Windows |
| static C table, in-place pushes | object | §1.7 | a table writer, and CARRIED 4,832,551,658 > landed 4,601,618,060 | — | a measurement below the landed route |
| static C table, one `memcpy` (new runtime function) | object | §1.7, §4.20 | a runtime entry point; still a block per read (CARRIED 1,734,408,333, 15x the floor); a byte copy is sound only for literal elements, the static route's restriction without its payoff | — | — |
| one copy per thread | object | §1.12, §1.7 | a thread-exit registry on three platforms and a release before the leak check, unbuilt; CARRIED 195,594,982 > 147.9M here | — | a registry built and run on all three platforms |
| static block for scalars and `str` only | object | §1.7 | no saving: `[[i64]]`, `[Variant]` and `[]` lay out through the same 132 lines and ran identically, ASan clean | — | a shape among those failing on another platform |
| the lowering hoists a read out of a loop | object | §1.7, §3.1 (clang's optimiser is "a welcome side effect, not a reason"; design.md does not otherwise forbid a lowering optimisation, said as such) | `ir/flatten.hero`'s knot, `--dump-ir` of every such program (`tests/golden/ir/` is a hard stop), 8 lexical sites served | — | — |
| `K[i]`, `K.len()`, `for x in K` onto a static element array (critic 1) | object | §1.7 | as an IR op: 80 exhaustive matches in 28 files (`.decref_slot`'s count); as an emitter peephole: data flow in a printer; `f(K)` still builds; the gain over this route is 1.28x -> ~1x | — | built, and cheaper than an op |
| one process-wide copy, CAS-published (critic 2) | object | §1.12, §1.7 | once-protocol, exit registry, `map.c` guard; serves 0 of the 66 constants the static route leaves; a missed guard is a silent count on a shared writable block | — | a computed or map constant in the tree that is read hot |
| `static` rather than `static const` (critic 3) | object | §1.12 | none in lines; a missed guard becomes a moved count (free of static memory at 0, an in-place write of the constant at 1) instead of a fault | — | a platform where `static const` with relocations is not read-only |
