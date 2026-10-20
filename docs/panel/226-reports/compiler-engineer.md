# Panel 226, the compiler-engineer's report

Written as the work went, 2026-10-20, in the lane `thesis` at `a0a8d1249`. Every number below was
measured in my copy, `<scratchpad>/226-compiler-engineer/` (a `git archive` of `a0a8d1249`, 12,801
files by `find . -type f | wc -l`), with the seed-built compiler (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 7.91`, `sources 5f45d6514b22d650`) and judged with stage 2
(`./heroes build selfhost/main.hero -o heroes-next`, `real 83.79 user 63.99 sys 14.30`, `sources
7792ecfe50a531e8`, the tree's key). Line counts are `wc -l` unless they say `code_lines`, which is
`.claude/hooks/ceiling.py`'s `code_lines`, the `layout` suite's unit (`CEILING = 300`, its
`DECIDED` table read from the tree). A row marked **unrun** is a reading and says so. The witness
programs, their binaries and the run log are in `<scratchpad>/226-ce-witness/` (`witness-log.txt`).

- `verdict`: **object**. No veto: the proposal adds no core construct and the prototype breaches no
  ceiling. The objection is that the table the proposal promises ("each of the 27 dropped rules
  given a runtime meaning") is not a filter for seven of them: measured, five programs that
  `check --permissive` passes die at `build --permissive` with exit 2, two more run with a meaning
  their own message denies, and the abort the control arm already produces says *compiler bug*.
  Each repair is a second reading inside the resolver or the lowering, at a module with zero
  ceiling headroom, and Part 11's text does not say whether it buys them or pins a refusal.
- `section`: design.md §1.7 ("anything in the core must be implemented in the type checker *and*
  the lowering *and* the backend"; "does it ... remove a special case from the compiler?"), §1.1
  (the ceiling), Part 5 (the seven constructs), Part 11 lines 4351 to 4367 (the control arm's
  paragraph as it stands), §1.12 (a permissive binary must not corrupt memory: none did).

## `implementation_cost`

### Where `build` meets the checker, and what the flag is

`heroes build`, `run` and `test` reach the frontend through one function, `compile.to_unit`
(`selfhost/cli/compile.hero:160-317`, the module 430 lines, **290 `code_lines` against the ceiling
of 300** before the patch): parse, `stage(parsed.diagnostics, s)`; modules; resolve; `checker.check`;
`lower.lower`; `verify`; `mono`; `place_store`; `own`; `emit`. Every `stage` call
(`compile.hero:90-107`) renders what it is handed and stops at the first stage that speaks; there is
no filter. `check` has one: `kept_of` (`selfhost/cli/check.hero:281-291`) drops `diag.is_thesis_rule`
under `--permissive`, and `run` swaps `parsed.diagnostics` for `parsed.control` (`check.hero:40-41`,
`selfhost/parse.hero:32-39`). The flag today is one row of `check_flags` (`selfhost/cli/table.hero:94`);
`build_flags`, `run_flags` and `test_flags` (lines 98 to 121) do not carry it. So `build --permissive`
is a flag in `.claude/rules/cli-surface.md`'s sense (line 24: "a flag if it changes how one question
is answered about the same input").

**The prototype, in my copy** (originals kept in `<scratchpad>/226-ce-orig/`; `diff -u` per file,
lines added and removed; `code_lines` after):

| piece | module | added / removed | `code_lines` before → after |
|---|---|---|---|
| `--permissive` in `build_flags`, `run_flags`, `test_flags` | `selfhost/cli/table.hero` | +3 / −0 | 202 → 205 |
| `diag.kept(diagnostics, permissive)`, the filter beside `is_thesis_rule` | `selfhost/diag.hero` | +10 / −0 | 218 → 228 |
| `Options.permissive: bool`; `stage(…, permissive)` filtering through `diag.kept`; `parsed.control` in `to_unit`; 7 `stage` calls widened | `selfhost/cli/compile.hero` | +25 / −15 | **290 → 297 of 300** |
| `permissive:` at every `Options(` construction (13: `grep -rn "compile\.Options(\|[^a-z_]Options(" selfhost --include='*.hero' \| grep -v "record Options" \| wc -l`) | `verbs.hero` 4, `compile.hero` 3, `produce.hero` 2, `header_ask.hero` 3, `deep_types.hero` 1 | +8/−4, (in the row above), +4/−2, +6/−3, +2/−1 | 177 → 180; 295 → 295; 110 → 113; 216 → 217 |
| `compile.stage(…, permissive: false)` at its 14 external callers (FFI and round diagnostics, never a thesis rule) | `whose.hero` 8, `rounds.hero` 4, `artifact.hero` 1, `value_ask.hero` 1 | +13/−8, +6/−4, +1/−1, +1/−1 | 249, 295, 147, 136 |
| **total** | **11 files** | **+79 / −39** | no module over its ceiling |

Type-checked whole by the seed-built compiler: `./heroes check selfhost/main.hero`, exit 0,
`real 6.35`. **The cache needs no new key field**: a unit's object is keyed on the TU's own emitted
text with the compile words (`selfhost/cli/units.hero:118-145`, the `digest.digest(...)` at line 142),
so a permissive build whose C differs is keyed apart by construction and one whose C is identical
shares the object, which is correct.

### The 27 rows

Sites are `grep -rn --include='*.hero' -E 'code: "<name>"' selfhost/ | grep -v '^selfhost/diag.hero'`.
*Measured* is what `heroes-next-perm` (stage 2 from the patched tree, `sources 291f5502424ce964`) did
with a witness program under `check` (strict), `check --permissive`, `build --permissive -o`, and the
binary run (`HEROES_RUNTIME` pointed at my copy's `runtime/`, so the witnesses' `build/` is their
own). *Meaning* is the runtime meaning I would pin, with the permissive language whose rule it is.
*Lines* is what that meaning costs beyond the filter above.

| # | code | emitted at | measured (witness → `check --permissive` → `build --permissive` → run) | meaning to pin | lines, module |
|---|---|---|---|---|---|
| 1 | `unused_binding` | `rename_fit.hero:365`, `resolve/errors.hero:185`, `module/diagnostics.hero:64` | w21: exit 0 → exit 0 → `1`, exit 0 | nothing happens (C, Python) | 0 |
| 2 | `never_rebound` | `resolve/errors.hero:342,359`, `resolve/written_nowhere.hero:79,96` | w19: exit 0 → exit 0 → `1` | the caller's place comes back as lent (a C pointer never written) | 0 |
| 3 | `cell_redeclared` | `resolve/meant.hero:186`, called at `:131` | w18 `v: i64 @= 1; v @= 2; print(v)`: exit 0 → exit 0 → **`1`**. The resolver counts the write and records the re-binding (`meant.hero:131-133`); **the lowering performs no store**, and the message's own sentence (*a cell is re-bound with `@`: write `v @ …`*) is not what the binary does w28 typed `v: i64 @= 2` after `v: i64 @= 1`: strict says `never_rebound` and `shadowed_binding`, → exit 0 → **`1`**, the same lost write | `x @= e` on a cell is `x @ e` (Python re-binding): **today a silent no-op** | ~4 in `ir/flatten.hero`'s `.declare` statement, reading `r.rebinds` (**1150 against DECIDED 1150**) |
| 4 | `declared_place` | `cell_errors.hero:63,78` | w22 `p.x @= 2`: exit 0 → exit 0 → `2` | the write (`cell_errors.hero:72-74` reads it so) | 0 |
| 5 | `hole_without_f` | `plain_holes.hero:158` | w16: exit 0 → exit 0 → `n is {n}` | prints its braces as text (Python without `f`) | 0 |
| 6 | `shadowed_binding` | `resolve/errors.hero:142,149`, `module/diagnostics.hero:39,52` | w09 immutable `x = 1; if true: x = 2; print(x); print(x)`: exit 0 → exit 0 → **`1` `1`**: `state.declare` returns `fail` and binds nothing (`resolve/state.hero:310-311`), so **the outer wins**. w24 cell `v: i64 @= 1; if true: v = 2; print(v)`: → **`1`**: the resolver reads it as the write (`meant.hero:158`, *nothing declared*) and **the lowering performs no store** | the brief's *inner wins* (every permissive language) is neither; today reads *outer wins* for an immutable and *a silent no-op* for a cell | ~4 in `resolve/state.hero` (**329 against DECIDED 330**) for *inner wins*; the cell's store as row 3 |
| 7 | `builtin_name_taken` | `resolve/errors.hero:156` | w08 top-level `function print(n: i64) -> i64; main: print(5)`: exit 0 → exit 0 → `5`: the built-in wins (`resolve/top.hero:91-94` keeps the library's entry); w26 local `range = 3; print(range)`: exit 0 → exit 0 → `3`: the local wins (`state.hero:336-337`, *reported and then BINDS*) | top-level: the built-in; local: the local | 0 |
| 8 | `discarded_value` | `discard_errors.hero:111` | w12 `name()` of type `str` on its own line: exit 0 → exit 0 → `1` | the value falls (C) | 0 |
| 9 | `discarded_failure` | `discard_errors.hero:259,290` | w20 `_ = risky(0 - 1)`: exit 0 → exit 0 → `1` | the error is dropped (Go's `_ = err`) | 0 |
| 10 | `bound_unit` | `discard_errors.hero:303` | w02 `x = greet()` (greet returns `()`): strict says `unused_binding` first; → exit 0 → **exit 2**: `internal error: monomorphisation produced an ill-formed program … slot `x` carries a type the checker never resolved, and no diagnostic says why … the verifier refused` | `x` holds `()` (Python's `None`), or a refusal under `--permissive` too; **today an internal error after a clean `check --permissive`** | a `()` slot through `mono`/the emitter (**unrun** how many), or 1 line moving the code to panel 192's third category |
| 11 | `wildcard_on_variant` | `data_errors.hero:259` | w14: exit 0 → exit 0 → `other` (`ir/flatten.hero:1003-1010` fills every unclaimed case on `.wildcard`) | catch-all (Rust's `_`) | 0 |
| 12 | `marker_mismatch` | `data_errors.hero:155,215`, `resolve/built_marks.hero:258` | w03 `bump(v)` to `@n`: exit 0 → **exit 2**, clang: `incompatible integer to pointer conversion passing 'int64_t' to parameter of type 'int64_t *'`. w04 `show(@v)` to `n`: exit 0 → **exit 2**, clang: `incompatible pointer to integer conversion`. The lowering reads the CALL SITE's marker (`ir/flatten.hero:613`, `if arg.mutable`) | the parameter decides, the call site's marker says nothing (C++ `T&`, Pascal `var`); a non-place to an `@` parameter must stay refused (C++ refuses binding a temporary to `T&`), **and today it is not**: w27 `bump(1)` to `@n` says `marker_mismatch` alone under strict, exit 0 under `--permissive`, and **exit 2** at clang, since `not_a_place` is asked only where an `@` is written (`check/markers.hero:5-13`) | ~8 in `flatten.lowered_from` (**1150/1150**) plus the flag reaching `lower()` (`ir/lower.hero:46`: a parameter, or a field of `state.Checked`), plus ~3 in `check/markers.hero` asking `not_a_place` of an unmarked argument to an `@` parameter under the flag |
| 13 | `ufcs_on_mutable` | `data_errors.hero:167` | w05 `v.bump()` with `bump(@n)`: exit 0 → **exit 2**, the verifier: `an instruction carries a type the checker never resolved` (`with_receiver`, `flatten.hero:642-647`, hands the receiver as a value) | the receiver's place is lent, as `bump(@v)` (Swift's `mutating`, a C++ non-const method) | ~4 in `with_receiver`, same module and flag as row 12 |
| 14 | `declaration_in_arm` | `cursor.hero:521` (a test), `parse/arm_body.hero:50` | w06 `.red => k = 1` in statement position: exit 0 → exit 0 → `3` (`cursor.error` of a thesis code is no refusal, `cursor.hero:516-521`) | the arm is a block of one statement, its name visible nowhere after (Rust's `=> { let x = 1; }`); in value position the arm is `()` and a type error, not a thesis rule | 0 |
| 15 | `missing_label` | `label_errors.hero:122,197` | not fired by my witness (w13 fired `needs_label`); by reading, labels are checked at a position and dropped by the lowering (`flatten.hero:495-498`) | read by position (C) | 0 |
| 16 | `needs_label` | `label_errors.hero:113` | w13 `pair(1, 2)`: exit 0 → exit 0 → `12` | by position | 0 |
| 17 | `wrong_label` | `label_errors.hero:44,211,224,232` | w13 `pair(b: 1, a: 2)`: → `12`: by position, the labels ignored | by position | 0 |
| 18 | `declared_twice` | `resolve/errors.hero:135`, called at `resolve/top.hero:103` | w07 two `function f`: exit 0 → **exit 2**, clang: `redefinition of 'h_w07declaredtwice_f'`. The resolver keeps the first (`top.hero:100-104`); the lowering still emits the second | the first wins (no permissive precedent refuses less: C refuses a redefinition, JavaScript takes the last); or a refusal under `--permissive` too | ~3 in `ir/lower.hero`'s walk of decls (skip a decl `resolved.top_in` does not name), or 1 line to the third category |
| 19 | `machine_locked_path` | `head_package.hero:67`, `head_names.hero:101,106` | not exercised: an `extern` group's header or package name; **never fires in a graded program**, R8's task rules forbid `extern` | the toolchain's | 0 |
| 20 | `spaced_minus_element` | `parse/arm_line.hero:59`, `parse/list_line.hero:116` | w15 `[3 / - 2]` over two lines: exit 0 → exit 0 → `2` (two elements) | unary minus, a negative element | 0 |
| 21 | `continuation_outside_brackets` | `join_fix.hero:39`, `sign_above.hero:368`, `open_line.hero:261,313`, `parse.hero:364` | my witness (w23) hit `unexpected_block`, not this rule: **not exercised**; by design `parsed.control` is the parse of the join (`parse.hero:32-39`, defect 131) | the join, as `check --permissive` reads it | 0 |
| 22 | `unreachable_statement` | `flow_errors.hero:138` | w10 `return 1; print(9)`: exit 0 → exit 0 → `1` (`build.terminate` declines on an ended block, `ir/build.hero:184-186`) | dead code, never run (C) | 0 |
| 23 | `undefined_header_name` | `head_names.hero:276` | not exercised, as row 19 | the toolchain's | 0 |
| 24 | `unshowable_name` | `head_names.hero:96` | not exercised, as row 19 | the toolchain's | 0 |
| 25 | `package_comparison` | `head_package.hero:76` | not exercised, as row 19 | the toolchain's | 0 |
| 26 | `endless_recursion` | `flow_errors.hero:272`, `check/endless_cycle.hero:332` | w17: exit 0 → exit 0 → exit 134, `panic: stack exhausted in w17endlessrecursion.down` (`runtime/parts/stack.c`) | runs until the stack is exhausted; **loud**, R3's second class, never silent | 0 |
| 27 | `non_exhaustive` | `data_errors.hero:271,278`, called at `check/reach.hero:222,234,238` | w01 (the brief's case with its `type_mismatch` arm corrected to `q.s * q.s`, since the golden as written carries a non-thesis error at line 20 and no permissive build can accept it): exit 0 → exit 0 → `4`, then at `.dot` **exit 134, `panic: entered unreachable code — this is a compiler bug, please report it`**. w11 `match n` on `i64` with no `_` (`needs_wildcard`, the same code): `zero`, then at 7 the same abort and message. `ir/flatten.hero:1017-1036` builds an orphan block `match: unclaimed` ending `.unreachable`, which `emit/term.hero` writes as `hero_unreachable()`; under strict that block never exists, so **changing its terminator moves no strict emission** | aborts at the unnamed case with a message of its own (below) | a third `AbortWhy` case, `ir.hero:114-119` (**315 against DECIDED 315**), its arms in `ir/print.hero` (467/470), `emit/inst.hero:302` (327/350), `ir/flatten.hero` (1150/1150, +~4, the orphan's terminator), a writer in `emit/abort.hero` (95, +~8), `hero_panic_unmatched` in `runtime/parts/panic.c` (+4): **~25 lines, 6 files, 3 at their DECIDED row** |

**Read together**: 23 codes exercised through 27 valid witnesses (rows 15, 19, 21, 23, 24, 25 by
reading). **Seventeen build and run as their sentence says.** **Six programs pass
`check --permissive` and die at `build --permissive` with exit 2**: rows 10, 12 (three shapes), 13,
18, three of them clang's refusal of the emitted C and three the IR verifier's; §1.12 held, nothing reached a
running binary with a wrong pointer. **Three run with a meaning their own message denies**: rows 3
(two shapes) and 6's cell, where the resolver reads a write and the lowering stores nothing. **One reads against
every permissive language**: row 6's immutable, the outer binding winning. **Two abort with a false
message**: row 27's two shapes. An exit 2 after a clean `check --permissive` is CLAUDE.md's
`blocking` class ("an exit 2 where the author can be told"), and in the harness it is R8's
`build_failed → excluded`: the control arm would exclude exactly the samples where the `@` rule
bit, which are the pairs the primary is built from.

**The message I propose** for row 27, modelled on `hero_panic_must`'s shape
(`runtime/parts/failure.c:188-198`, `panic: .must() on an error: <code>: <msg>`):
`panic: this match has no arm for .dot`: the case's name as the program spells it, so the
author who reads `panic:` sees what the strict compiler would have named in `non_exhaustive`'s
own words (`this match does not cover .dot`); for `needs_wildcard`'s `i64`/`str` chain, where no
case has a name: `panic: this match has no arm for 7`, the value through `to_str`, or `no arm for
this value` where the type has no rendering. `hero_unreachable`'s *this is a compiler bug, please
report it* (`runtime/parts/panic.c:45-47`) is false here and must not be said: a permissive program
reaching an unnamed case is the program's mistake, the one the thesis measures.

**Sentences for the three codes the brief named**:

- `marker_mismatch`: *the parameter's declaration decides; an `@` written or omitted at the call
  changes nothing; an argument that is not a place cannot be handed to an `@` parameter and is
  refused* (`not_a_place`, not a thesis rule, which today is asked only where an `@` is written, so
  the sentence costs the checker a line too). It CAN be given a meaning, and the meaning costs
  lowering and checker lines, not a filter: row 12, measured exit 2 today in all three shapes.
- `ufcs_on_mutable`: *`x.f()` where `f`'s first parameter is `@` lends `x`'s place, as `f(@x)`
  does; a receiver that is not a place stays refused.* Row 13, the same lines, measured exit 2.
- `declaration_in_arm`: *the arm's body is a block of one statement; the name it binds is visible
  nowhere after it; in value position the arm is `()`.* Row 14, zero lines, measured `3`.

### The prototype's numbers (question 2)

- Diff: **+79 / −39 lines over 11 files** (`diff -u` against the saved originals, lines opening
  `+`/`-` and not `++`/`--`); the flag's own plumbing is 38 of the added lines (`table` 3, `diag`
  10, `compile` 25), the rest the record's field at 13 constructions and `permissive: false` at 14
  callers of `compile.stage` outside `compile.hero`.
- `heroes-next-perm build w01-non_exhaustive.hero --permissive -o …`: exit 0, `wrote …`; the binary
  printed `4` and at `area(.dot)` **exited 134** with `panic: entered unreachable code — this is a
  compiler bug, please report it` on stderr: the abort the brief predicted, at the unnamed case,
  with the message a permissive abort must not say.
- The compiler's own tests, `test selfhost/main.hero`, before (`heroes-next`) and after
  (`heroes-next-perm`): before `1738 tests, all passed` (exit 0), after `1738 tests, all passed` (exit 0): the flag's plumbing moved no test, and no test was added for it, which a landing owes (`compile.hero`'s own `test` block constructs `Options` three times and never with `permissive: true`).

### The seed (question 3)

Stage 2 from the patched `selfhost/` with the seed-built compiler: `./heroes build
selfhost/main.hero -o heroes-next-perm`, **exit 0**, `real 33.49 user 32.69 sys 0.67` (warm cache; a
script of milliseconds wrote the witness files beside it, so the duration is indicative, the ratio
clean), `--version` saying `sources 291f5502424ce964`, and `build --help`, `run --help`,
`test --help` each printing the flag once. **No seed refresh is forced**: the flag is a row of the
argv table and a field of a record, both forms the seed's language already has
(`.claude/rules/verification.md` § The batch: a NEW FORM enters `selfhost/` only after the refresh
whose seed knows it; a row and a field are not one). Row 27's third `AbortWhy` case is a variant
case, not a form, so it forces none either. **The fixpoint one generation further holds**: stage 3 built by `heroes-next-perm` from the
patched `selfhost/` (`build … -o heroes-s3`, exit 0, from the witness directory with
`HEROES_RUNTIME` set so its `build/` was its own), and `cmp` of the two whole emissions,
`heroes-next-perm build selfhost/main.hero --emit-c` against `heroes-s3 build … --emit-c`:
**identical, 27,890,840 bytes**, both stderrs empty, `heroes-s3 --version` saying `sources
291f5502424ce964`, the same key as stage 2. So the flag's plumbing is a fixpoint of itself and
the seed's refresh is owed only by M-bounded-seed's rhythm, not by this change.

### The grader's spawns (question 4)

- `check --json` (unpatched `heroes-next`) writes schema 2 to stderr:
  `{"schema": 2, "kind": "diagnostics", "read": [{file, sha256}], "diagnostics": [{code, col,
  file, fixes, line, message, notes}]}`. **The `code` list is enough to bucket a refusal**: w01 gave
  `['non_exhaustive']`, w13 `['needs_label', 'needs_label', 'wrong_label', 'wrong_label']`; and
  since `spoken` speaks one stage (`check.hero:265-275`), the list is one stage's, which the bucket
  may read as *refused at parse / names / types* from the first code's stage.
- **A `???` program**: `check --json` **exit 0**, stderr `"diagnostics": []` (189 bytes), the hole
  as TEXT on stdout (81 bytes: `hole at w25-hole.hero:2:15 / expected type: i64 / in scope: …`);
  the JSON names no hole (the one `hole` in stderr is my file's name). `build`: **exit 1**,
  stderr `no binary: 1 hole in w25-hole.hero`, stdout the same text, no file written. **So R5's
  `hole` bucket cannot be read from the JSON alone**: a `check --json` with an empty list AND a
  non-empty stdout is the hole, or `build`'s exit 1 with `no binary:`. The grader's spawn should
  read both streams, or run `build` and bucket `no binary:` before `error[`.
- `heroes test` has **no per-test time limit**, by reading: `selfhost/cli/verbs.hero:144-157` runs
  each test through `process.run(program, words, out: "", err: "")`, and `selfhost/cli/process.hero:109`
  `function run(program: str, words: [str], out: str, err: str) -> Ran` takes no seconds. The
  harness's `hero_run_limit(seconds)` is set in the parent (`tests/harness/shell.hero:285`) and
  binds the one child `hero_run_go` starts, `heroes test`: at the limit `kill(-child, SIGTERM)` goes
  to that child's GROUP (`runtime/parts/run.c:1556`), and the test binaries run in groups of their
  own (`run.c:238-250`, *every child gets a process group of its own*), so the group kill does not
  reach them; **what reaches them is the forwarding handler** (`run.c:244-253`): a `heroes` waiting
  on a child catches SIGTERM, sends it to every child's group, waits `HERO_RUN_GRACE_MS` 2000 ms,
  SIGKILLs what stands, then dies of the signal; the watchdog's own grace is 5000 ms (`run.c:290`),
  longer on purpose, so the chain completes. By one hop of forwarding, within 2 s: **yes**, by
  reading (**unrun**: a sleeping test under a limited parent). A test binary that itself spawned a
  grandchild is reached only if it is itself a Heroes program waiting on it.

### `run_bounded` and the second root (question 5)

`spawned(scratch, program, words, in_path, write_limit)` (`tests/harness/shell.hero:239-300`) already
takes the input path and the byte cap and sets `hero_run_limit(seconds: WATCHDOG_SECONDS)`
unconditionally at line 285, under the sentence *every call is guarded, and there is no way to ask
for less*; `WATCHDOG_SECONDS` is 600 at line 751, asserted at 1287. The coordinator's
`run_bounded(scratch, program, words, seconds, bytes, stdin_path)` with `spawned` taking the
seconds and `run`, `run_fed`, `run_limited`, `attempted` passing the constant costs about ten lines
in a 2,234-line module that no `DECIDED` row governs (`ceiling.py`'s `DECIDED_ROW` matches
`selfhost/…` alone), and a test of one second's wall. What it costs beyond lines, and what it misses:

- **the bound is `seconds + 5` at worst**, not `seconds`: the child's grace before SIGKILL is
  `HERO_RUN_WATCHDOG_GRACE_MS` 5000 (`run.c:290`); a grader giving a model's program 10 s should
  write 15 in its budget, and the design should say so;
- **the byte cap is POSIX's alone**: `can_limit_writes()` (`shell.hero:171-172`) is false on
  Windows, so `run_bounded` there has a time bound and no byte bound; the grader must refuse to
  start where `can_limit_writes()` is false, or the sentence *bounded in time and in bytes* is
  untrue on one platform;
- the zero bound refused: right, and it should refuse `seconds > WATCHDOG_SECONDS` too, or the
  constant stops being the ceiling it is asserted to be;
- `hero_run_limit` and `hero_run_limit_writes` are process-global and taken by the next
  `hero_run_go` (`shell.hero:285-290`); the harness is single-threaded here, so the shape holds, and
  `hero_run_reset()` at line 282 keeps a refused spawn from leaving a cap behind;
- `stdin_path` is `in_path` already (`shell.hero:297`): the hidden cases' input files cost nothing new;
- what no bound here covers is a program that detaches a grandchild: Darwin's sentinel
  (`run.c:276-280`, defect 465) and Linux's parent-death signal (defect 437) are the runtime's
  answer, and R11's sandbox the second wall.

The second root: `ISSUE_PROGRAM` (`tests/harness/main.hero:645`) and its test (lines 777 to 805)
hard-code `issues` in `reached`, `blocks` and `strays`. A `PROGRAMS: [str]` makes the one test a
loop of about eight lines, and loses what the net's output is for: a loop's first failing root
stops the test and names neither. **One test per root is the shape**, since `FAIL <title>` is the
line a reader gets; a helper `root_tests(root: str)` of about 25 lines and two tests of three lines
each, so the third root costs three.

### Core or sugar, and the ceiling (question 6)

`build --permissive` is **a flag the frontend erases**: a filter (`diag.kept`) and a swap of two
parse outputs inside `compile.to_unit`, no construct, nothing new for the checker, the lowering or
the backend to handle for 17 of the 23 codes exercised. For seven codes the promised meaning is
**not a filter**: a second reading of an existing construct inside the resolver (which binding
wins, rows 3 and 6), inside the lowering (which side's marker decides, rows 12 and 13; a rebinding's
store, rows 3 and 6; a second declaration skipped, row 18; a `()` slot, row 10), or a new abort
kind through four IR and emitter modules and the runtime (row 27). None is a Part 5 row and none
is a new construct; §1.7's question, *does it add a special case to the compiler?*, answers seven,
each small, each under a flag the compiler's own build never takes. The ceiling: the filter alone
puts `compile.hero` at **297 of 300**; the seven meanings land on `ir/flatten.hero` (1150/1150),
`ir.hero` (315/315) and `resolve/state.hero` (329/330), so they move three `DECIDED` rows or open a
module. **Still one person's compiler**: Pascal-P4's ~4,000 lines is not this project's scale
(`selfhost/` is 176,098 lines in 678 modules, `find selfhost -name '*.hero' | xargs cat | wc -l`),
and the proposal adds under 150 to it; the ceiling that binds here is the per-module one.

- `needed_for_self_hosting`: **no**. The compiler compiles itself under every thesis rule; nothing
  in the closure list needs a permissive binary. The flag enters by Principle 0's second door, a
  measured Part 11 effect, which is what the control arm is for.

- `argument`: `build --permissive` is a flag, not a construct: +79/−39 lines over 11 files,
  `compile.hero` at 297 of 300, no seed refresh, stage 2 green. The objection is the table the
  proposal promises. Measured on 27 witnesses with stage 2: 17 codes build and run as pinned;
  `non_exhaustive` aborts saying *this is a compiler bug*; six programs `check --permissive`
  passes die at `build --permissive` with exit 2 (`marker_mismatch` in three shapes,
  `ufcs_on_mutable`, `declared_twice`, `bound_unit`); three more run with a meaning their own
  message denies (`cell_redeclared` twice, a cell's shadow: the write never happens). Those are resolver and lowering
  readings, not filters, each at a module with zero ceiling headroom. Part 11 must say which it
  buys and which it refuses.

- `prediction`: if `build --permissive` lands as the proposal reads it, a filter and a pinned
  table with no lowering-side meaning for `marker_mismatch`, `ufcs_on_mutable`, `declared_twice`
  and `bound_unit`, then at M-thesis-harness's first held-out run the control arm's `build_failed`
  bucket (exit 2 after a clean `check --permissive`) is non-empty, and every member carries one of
  those four codes under strict `check --json`. Falsified if the bucket is empty over the 40 tasks
  × 3 samples, or holds a program none of the four names. Checkable from the run's provenance
  records at that milestone.

- `condition`: I move to **approve** on either of two pieces of evidence. (a) The 27 witnesses
  under a landed `build --permissive`, each one either building and running as its pinned sentence
  says, or refused at `check --permissive` with exit 1 by a code the table names, **never exit 2**;
  and `compile.hero` under 300 or split at a named seam, the `DECIDED` rows it moves written in
  the same commit. (b) The sitting rules the four exit-2 codes into panel 192's third category, kept
  under `--permissive`, with Part 11's paragraph saying so and `is_thesis_rule`'s list losing them,
  and `drop-case`'s control-arm column re-measured after.
