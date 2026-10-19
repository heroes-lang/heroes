# Panel 225, completeness critic, second pass: what is missing

Written 2026-10-19 as it was run, after both seats' reports (`compiler-engineer.md`, 433 lines,
and `ffi-pragmatist.md`, 448 lines, by `wc -l`), in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-critic` (detached at `7b97b9438`).
The seats' directories were only READ. Every number carries its command; what I did not run says
*unrun*. No verdict is given. **Second pass complete, 2026-10-19 04:59 +0200** (`date`). Below, **CE** is the compiler-engineer's prototype (`heroes-g`,
C-prime with E and F) and **FP** the ffi-pragmatist's (`heroes-p`, P with E); **s2** is my
stage 2 of `7b97b9438`.

## Method, and what makes the two binaries the two diffs

- Each seat's uncommitted diff saved into my directory: `git -C <seat's worktree> diff >
  critic-work/pass2/{ce,fp}.diff` (CE 10 files, +348 -107; FP 6 files, +158 -69, its new
  `selfhost/emit/probe_cut.hero` untracked and copied apart, 75 lines).
- Each seat's binary copied into `critic-work/bin/` (`cp -p`): `heroes-g --version` prints
  `sources 2a4331af563a2a10`, `heroes-p` `sources 7e12e4f040112477`, each its report's key.
  `heroes-g doctor` finds the runtime of my own tree (`runtime_places`: the working directory,
  then every directory above the executable), byte-identical at `7b97b9438` since neither diff
  touches `runtime/`.
- **Each binary is its diff**: with CE's diff applied to my tree (`git apply`, checked first with
  `--check`), the guard accepted `heroes-g` for a run of the net, which it refuses where the
  compiler's sources differ from the tree's; likewise `heroes-p` with FP's diff and its new file
  (`unsupported fixedbugs-2128`: 4 passed, 0 failed). Both diffs reverted after
  (`git checkout -- selfhost`, the new files removed; `git status --short -- selfhost` then prints
  nothing).
- Every program below is mine, under `critic-work/pass2/<dir>/`, built in a fresh `build/` with
  `../../bin/<compiler> build <p>.hero -o <p>`, each binary that built run under `timeout 10`.

## Summary

- **Neither prototype alone covers what the other covers, measured both ways (§ 2)**: a refusal
  in a typedef is answered by FP only (CE keeps the typedefs); a refused constant whose own C type
  is refused is answered by CE only (FP compiles an inert twin that spells that type); a copy that
  cannot compile silences the refused units after it in FP only (CE asks one unit at a time); F
  (2136, L1) is CE's alone.
- **Both cuts can be defeated by a header that writes the mark (§ 3.1)**: each prototype then
  tells a value undefined beside a refusal that the program as written, and the corrected program,
  define (`K_SZ`), meeting **both** seats' own falsifiers; and because CE cuts EVERY probe's text,
  not only the copy's, the same header makes CE fail a correct program at exit 2. CE's residue 4,
  *every such case I can reason ends in a copy that does not compile*, is contradicted by these
  runs. F inherits the attack.
- **Without a forged mark I found no false word**: `clang optimize off` inside a refused value
  changed no verdict (§ 3.2); no `#include` stands inside a cut (§ 3.3); F beside the lock refusal
  tells true words, reworded once (§ 4).
- **The seats hold the resolution to two different falsifiers, and the trunk already meets one of
  them (§ 5.1)**: CE judges a message against *the program as written* (design.md §4.17's
  sentence); FP against *the corrected program's own build*. A value read through `__LINE__` beside
  a refusal is told by s2 and both prototypes, and the correction that deletes the refused line
  builds. That is the question the sitting did not ask (§ 8).
- **Measured for CE where its report says unrun**: the whole `unsupported` form 353 passed, 0
  failed, and `full` 40 passed, 0 failed, on `heroes-g` (§ 6).

## 1. What each prototype changes, tests and leaves out (read from the diffs)

| | CE (`heroes-g`) | FP (`heroes-p`) |
|---|---|---|
| marks | `#pragma hero_probe_cut on` / `off` (`emit/probe_cut.hero` lines 18-22), whole lines, `\r` allowed | `#pragma heroes_probe cut` / `keep` (lines 21-25), whole lines, `\n`-ended |
| what the copy keeps | the opening, **the string-literal table and the typedefs**, the asked accessors (marks around the prelude's three assertion writers, `emit/decls.hero`, and everything after `typeorder.definitions` but the accessors, `emit/unit.hero`) | the opening and the asked accessors only (`CUT` right after `emit_decls.opening`; `KEEP` before each asked accessor, `CUT` after) |
| a refused accessor | written as the program writes it inside a cut stretch, and **left out of `Unit.defined`** (`value_beside.again`'s `asked`); `constant_accessor.inert` deleted | written as the program writes it inside the cut, **its inert twin kept** after a `KEEP` (`return (<type>){0};`); `Unit.defined` whole |
| where the cut is applied | **to every probe's preprocessed text** (`value_run.preprocessed`, `return ok(probe_cut.cut(text.must()))`) | only to a copy (`if one.cut`, a new `Unit.cut` field) |
| E | every refused unit (`assemble.per_module`) | every refused unit |
| one unit at a time | yes (`value_beside.told` loops, so a broken copy leaves the rest asked) | no (`value_ask.told` returns at its first broken unit) |
| F | at `check_all`, `lock_told`, `two_ways`/`defined_twice` (`cli/value_early.hero`, new, 108 lines) | C side by hand only |
| a finding the refusal already tells | not dropped | dropped (`whose.with_beside`) |
| measured by the seat | 17 probe cases, `layout`, 1732 tests, 6 `--emit-c` and 13 units byte-identical, clang runs and durations | 17 probe cases, whole `unsupported` 353/0 and `full` 40/0, 1731 tests, 4 units and 2 `--emit-c` byte-identical, SQLite's header, 2109's fifteen defined constants beside a refusal |

## 2. Where they differ, measured

Each program built by s2, `heroes-g` and `heroes-p`; *told* lists the diagnostics on stderr.

| program (`critic-work/pass2/`) | what it holds | s2 | CE | FP |
|---|---|---|---|---|
| `tdef/tdef.hero` | `record Holder` holding by value `U`, a group record whose tag `un.h` declares a `union`, beside `K_DIV` | `ffi_tag_is_a_union` | the same, **no `K_DIV`** | **`ffi_tag_is_a_union` and `K_DIV`** |
| `twin/twin.hero` | `constant NO_DB: Db` over `record Db tag nosuch`, a tag `tw.h` never declares, beside `K_DIV` | `ffi_unknown_name` | **`ffi_unknown_name` and `K_DIV`** | the same as s2, **no `K_DIV`** |
| `first/first.hero` | the root refused inside its own header (`hbad.h`, an unclosed `#pragma clang attribute push`), and `t4mod/vals` refused at `narrow(x: i64)` with `K_DIV` | `ffi_header_refused`; 0 `value-` entries | **`ffi_header_refused` and `K_DIV`**; 2 `value-` entries | `ffi_header_refused`; **1 `value-` entry**: the second copy never asked |
| `mixed/mixed.hero` (the ffi-pragmatist's, copied) | `K_SHIFT` refused at its accessor, `narrow` elsewhere, `main` before the accessors | `K_SHIFT`, `narrow` | `K_SHIFT` (once), `narrow`, `K_DIV`, `K_FLT` | the same as CE |

Why, read from each copy: CE's copy of `tdef` (`build/value-46f5446335d51d69/tdef.c`) keeps
`typedef struct h_tdef_Holder { struct u f_inner; ...` at lines 87-88, between its `off` at 86 and
its next `on` at 107; FP's copy of `twin` (`build/value-52c830010de64283/twin.c`) keeps
`nosuch * h_twin_NO_DB(void) { ... return (nosuch *){0}; }` at lines 122-125, between `keep` at
120 and `cut` at 126. In `first`, FP's build holds one `value-` directory, the broken root copy;
CE's holds two. `mixed` reads the same on both, so FP's de-duplication is not exercised there,
and I found no shape that exercises it (§ 7).

## 3. Can a header defeat either cut?

### 3.1 A header writing the marks' own text: yes, both, measured

The marks are fixed strings; a header's unknown pragma passes `-E` verbatim (both seats measured
this), and inside the compiler's guard `-Wall` is ignored, so s2 builds such a header in silence.

| program | the header | s2 | CE | FP |
|---|---|---|---|---|
| `forge1/forge1.hero`, correct, nothing refused | the line `#pragma hero_probe_cut on`, then `#define K_OK 5` | exit 0, prints `5` | **exit 2**: *internal error: the probe of the header constants' values in module `forge1` did not compile, where the module's own unit did: ... call to undeclared function 'h_forge1_K_OK'* | exit 0, prints `5` |
| `forge2c/forge2.hero`, `narrow(x: i64)` refused | `#pragma pack(push, 1)`, then CE's mark, then `#define K_SZ ((-2147483647 - 1) / ((int)sizeof(struct { char c; int i; }) - 6))` | `narrow` | **`narrow` and `K_SZ`** (*... traps where it reads it*) | `narrow` |
| `forge2c/forge2-next.hero`, the same corrected (`x: i32`) | the same | exit 0, prints `-1073741821` | **exit 2**, internal error | exit 0, prints `-1073741821` |
| `forge2p/forge2.hero` | the same with FP's mark | `narrow` | `narrow` | **`narrow` and `K_SZ`** |
| `forge2p/forge2-next.hero` | the same | exit 0, `-1073741821` | exit 0, `-1073741821` | exit 0, `-1073741821` |
| `forge3c`, `forge3p` | the mark written by `_Pragma` from a macro (`#define HDR_MARK _Pragma("...")`, then `HDR_MARK`) | as `forge2` | as `forge2c` | as `forge2p` |

How: in the program's own unit the guard's close writes `#pragma pack()` after every header, so
`K_SZ`'s accessor reads `sizeof` 8 and `INT_MIN / 2`, which the corrected program prints
(`-1073741821` is `3 + INT_MIN / 2`). The forged mark opens a cut inside the header that the
copy's own first closing mark ends, so the guard's close falls inside it, `pack(1)` reaches the
accessor, `sizeof` is 5, and the copy reads `INT_MIN / -1`. The message is false of the program as
written and of the corrected program: **each seat's own falsifier is met** (CE §12, *a program ...
whose asked constant the cut copy judges otherwise than the program's own unit as written*; FP §14,
*P's copy tells a value the corrected program's own build accepts*). And since CE cuts every probe's
text, a correct program whose header holds CE's mark is refused at exit 2 (`forge1`). CE's residue
4 reasoned the opposite; FP names the hardening (*a digest of the program's sources*) as unbuilt.
**The shape is contrived** (a header holding the compiler's own mark), and it is the shape the
coordinator asked about; neither prototype refuses it today.

### 3.2 A pragma's state across a cut, without a forged mark: none found

- With plain clang (`critic-work/pass2/pragma/`, `clang -std=gnu11 -c` of `int f(void) { return
  (_Pragma("<p>") 5); }`): `pack(1)`, `STDC FENV_ACCESS ON`, `float_control(precise, off)` and
  `clang fp contract(fast)` are refused inside an expression (*expected expression*), so no
  constant's value can carry them into a cut stretch; `clang optimize off`, `clang diagnostic` and
  `push_macro` compile there. `_Pragma("clang optimize off")` inside one function's `return` makes
  the function after it `optnone` (`clang -std=gnu11 -O2 -S -emit-llvm`: `second` gets attribute
  set `#1`, `noinline nounwind optnone`).
- `optoff/opt.hero`: `K_OFF: i8` (refused at its own accessor) whose value is `(_Pragma("clang
  optimize off") 300)`, then the ffi-pragmatist's `K_BCP` (undefined only where the optimiser
  folds). At `-O2` all three compilers tell `K_OFF` and `K_BCP`; the corrected program
  (`opt-next.hero`, `K_OFF: i32`) is refused for `K_BCP` by all three too, so the message beside
  the refusal is **true**. At `-O0`, `K_OFF` alone, and the corrected program prints `301`. The
  program's own probe text holds `#pragma clang optimize off` six times (probe.i lines 1352-1382)
  before `K_BCP`'s accessor (line 1401, its prototype at 1345); **why the pragma leaves the verdict
  unchanged I did not establish** (a prototype before the pragma is one candidate, unrun).
- After the first mark, every `#pragma` line of the two copies still on disk is a `clang
  diagnostic` one (`awk` of the lines after the first mark, `#pragma` lines other than the marks,
  counted): CE's `tdef` copy 30 `error`, 1 `push`, 1 `pop`; FP's `twin` copy 42, 1 and 1. The
  probe's `-w` silences them. Names the compiler writes whose macros hold a `_Pragma` are refused
  before any unit by `pragma_ask` (defect 570), not run by me.

### 3.3 `#pragma once` and include guards across a cut: none can act

Both are settled during `-E`, before either cut. And in every copy built here no `#include`
stands inside a marked stretch: over 10 copy files (`tdef`, `first`, `mixed` on CE; `twin`,
`first`, `mixed` on FP; `find build -path 'build/value-*' -name '*.c'`), the last `#include` is at
lines 47-51 and the first mark at 59-64, `#include` lines after the first mark: 0 in each.

### 3.4 `_Pragma` inside a macro the cut removes

Measured twice above: as the mark itself (`forge3c`, `forge3p`, a false message) and as
`clang optimize off` in a refused value (`optoff`, no false message).

## 4. Can F tell a false word at the four refusals?

On `heroes-g` (FP has no compiler side for F), each beside L1's lock refusal (`ffi_lock_copied`,
`critic-work/pass2/fword/`, `forgeF/`):

- `fshift.hero` (`K_SHIFT` and `K_DIV`): the lock, `K_SHIFT` and `K_DIV`. **True, reworded**:
  beside the refusal `K_SHIFT` is told *... traps where it reads it* (the probe's words); with the
  lock corrected (`fshift-next.hero`) s2 and CE both tell it *shift count >= width of type* (the
  region's words). Both true; the ffi-pragmatist predicted this (§ 10 of its report).
- `fi8.hero` (`K_I8: i8` over an `int`, and `K_DIV`): the lock alone on all three: the copy holds
  `K_I8`'s refused accessor, which no clang verdict marks before any unit, so it does not compile
  and `K_DIV` waits (CE's residue 3, measured).
- `forgeF/ff.hero` (the lock, beside `forge2c`'s header): **the lock and a false `K_SZ`**; the
  corrected `ff-next.hero` prints `-1073741824` on s2 and fails at exit 2 on CE. F inherits § 3.1.
- **Unrun, a question**: F writes each unit from a round state the round never reached (CE §7:
  *the round's tags, the lend units read so far*). Whether any text before an asked accessor
  depends on that state, and so on a count a `__COUNTER__` reads, I did not establish.

## 5. Contradictions between the seats, and which side is checkable

### 5.1 Against which program is a message judged

CE §12 turns on *a program ... whose asked constant the cut copy judges otherwise than the
program's own unit as written* (design.md §4.17: *every message is true of the program as
written*). FP §14 turns on *any program ... where P's copy tells a value the corrected program's
own build accepts*, and FP §5 used *the compiler's own next build* as its reference column. The
two coincide on every attack either seat ran, and part where a correction moves what a value
reads. **Checkable, and run** (`critic-work/pass2/line/`): `ln.h` holds `#define K_LINE
((-2147483647 - 1) / (__LINE__ - 4))`; `asw.hero` binds `K_NONE` (a name the header lacks) on
line 2 and `K_LINE` on line 3:

| program | s2 | CE | FP |
|---|---|---|---|
| `asw.hero`, as written (`K_LINE` reads `INT_MIN / -1`) | `K_NONE`, `K_LINE` | the same | the same |
| `deleted.hero`, the refused line deleted (`K_LINE` on line 2, `INT_MIN / -2`) | exit 0, prints `1073741824` | the same | the same |
| `renamed.hero`, the refused name changed on its line | `K_LINEX`, `K_LINE` | the same | the same |

So the trunk's own repair of 2128 already tells a value that one correction makes defined and
another does not; FP's falsifier, read literally, is met by s2 and by every probe route
independent of any cut, and *the corrected program* is not one program. CE's falsifier is the
document's. Which one binds the resolution is not a fact either seat can measure.

### 5.2 The other differences, each measured in § 2 or § 3

What the copy keeps (`tdef`: FP answers, CE does not); the refused accessor's form (`twin`: CE
answers, FP does not); where the cut applies (`forge1`: CE exit 2, FP builds); one unit at a time
(`first`: CE answers, FP does not); F (2136, L1: CE only, measured by CE). The seats agree on A
(object as the resolution), on C (veto, as a cut made before preprocessing) and on the class `-w`
lifts (CE §4, FP §3); they differ in strength on B (CE veto, FP object) on the same mechanism.

## 6. Claims asserted and not measured

- **CE residue 4** (*every such case I can reason ends in a copy that does not compile, silence and
  not a false message*): reasoned, and contradicted by `forge2c`, `forge3c`, `forgeF` and `forge1`.
- **CE residue 7** (a pragma's state across a cut): reasoned, unrun by CE; § 3.2 ran one shape.
- **CE §7, F not at `bound_macro` and `pragma_ask`**: reasoned, still unrun.
- **CE §10, the whole forms unrun**: now run on `heroes-g` against a tree carrying CE's diff:
  `./critic-work/bin/heroes-g run tests/harness/main.hero -- ./critic-work/bin/heroes-g unsupported`
  **353 passed, 0 failed** (two Windows-only cases `SKIP`, as on FP's run; `real 133.47`), and
  `... full` **40 passed, 0 failed**, each exit 0. `run`, `emit`, `emission` and `determinism`
  remain unrun for both prototypes.
- **FP §7**, *a constant clang refused at a place `value_beside.holding` does not locate would be
  told twice*: no reproducer; the de-duplication it motivates is exercised by nothing run here or in
  either report (`mixed` tells `K_SHIFT` once on CE too, which has none).
- **FP §13**, *A must still write a refused constant inert (else a region refusal is told twice)*:
  unmeasured.
- **Both**: `clang -E` passing an unknown pragma as a whole line, and `-w` lifting a pragma-made
  error, measured on Apple clang 21 alone; FP's prediction scores it on the CI's legs.

## 7. Routes nobody listed

- **G, a mark no header can write**, which both prototypes need (§ 3.1). Both seats name a key or
  digest in the mark, unbuilt. A second form neither names: the copy knows how many marks it
  wrote, and the probe declines (the copy's silence) wherever the preprocessed text holds any other
  number of mark lines; it catches a literal line and a `_Pragma` alike and needs no secret. Unbuilt.
- **H, the cut confined to the copy**, which FP does and CE does not: the one change that turns
  `forge1`'s exit 2 back into a build.
- **I, the copy's fidelity checked rather than argued**: the refused unit preprocesses (its
  refusal is clang's compile, except a header's own), so its accessors' preprocessed text can be
  compared with the copy's, token for token apart from the generated `#line` numbers, before the
  probe reads the copy; a difference makes the copy silent. One more `-E` per refused unit. It
  would not catch § 3.1 (a forged mark is in both texts), and G would. Unbuilt, unrun.
- **The union**, which neither seat built: FP's cut extent (opening and asked accessors) and its
  cut confined to the copy; CE's refused accessors left out of what the probe calls (no twin), one
  unit at a time, and F at the four refusals; and G. On every program in § 2 to § 4 the union
  answers where either prototype does, by construction from the parts, **unrun as a whole**;
  without G it still tells `forge2`'s false `K_SZ`.

## 8. The question the sitting should have asked

**Of which program is a value told beside a refusal a statement: the program as written, whose
unit clang did not compile, or the program the author builds next?** design.md §4.17 says *as
written*; one seat's falsifier says *next build*; the trunk already tells values that some
corrections make defined (§ 5.1). The answer decides which falsifier binds the resolution, and
whether a reading that moves with the author's correction (`__LINE__`, `__COUNTER__`) is a defect
or a property of telling anything beside a refusal.

And a second, which § 3.1 makes concrete: **may a header's text steer how the compiler reads its
own copy?** Panel 216 ruled that `heroes build` may run a header's code, bounded as R1 bounds it;
the marks are the first place the compiler reads its own control lines back out of a stream a
header writes into, and no sitting has ruled on that.

## 9. State left

My worktree `.claude/worktrees/225-critic` is at `7b97b9438` with no change under `selfhost/`
(both diffs reverted), its untracked `critic-work/` (programs, logs, the two copied binaries and
the two diffs) and `heroes`, `heroes-s1`. Nothing committed; nothing written outside the
repository's root in this pass.
