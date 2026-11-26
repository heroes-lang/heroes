# Panel 261, the compiler-engineer's report (started 2026-11-26 10:18, written as it goes, last line 11:31)

Copy: `.claude/worktrees/261-compiler-engineer`, detached at `df92f9281` (`git log -1 --format='%H %ci %s'`, 10:19).
Scratch (build logs, long outputs): `ce-scratch/` in the copy, untracked. Programs kept:
`docs/panel/261-reports/programs/compiler-engineer/`.

## 0. Compilers

- seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0 (`ce-scratch/seed.log`; `real 21.08 user
  19.01` at load 19.6 to 29.8, `uptime` 10:19, so not a duration).
- stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0, `real 393.77 user 235.36 sys 57.31` at load 27 to
  37 (`uptime` 10:27; waiting, not a duration). **`./heroes-next --version` reads `heroes 0.6.0`, `sources
  4dcf3a9f20257573`**, the repair's key (`00-shared.md` § 8), so the base's compiler is the one the briefs measured.

## 1. The route built (heroes-261), and where each line landed

The whole change is `programs/compiler-engineer/route-v4.diff` (1,303 lines, `git diff selfhost` plus the new file):
the source of `heroes-261-v4` (`sources 1283d3066f568534`) and, after it was built, the R6 unit test in
`check/table.hero`, which `heroes-261-v4 test selfhost/main.hero` ran.

**A mode run on the existing case, never a new `Ty` case** (panel 112's R6 shape, the labels run's precedent,
`2724a4f9a`): `variant Ty`'s `function_ty` gains `modes: Params`, a run of `"@"`, `"lent"` or `""` in a new arena of the
table (`Types.modes`, `Types.mode_runs`, written by `type_runs.name_run` as the names are), **stored EMPTY where every
position is by value** (`table.intern_function`), and keyed by `mode_key`, which is `""` for an empty run and `:d<start>:
<len>` otherwise. So a type with no mode keeps its key, its id and its C typedef's name, as `label_key` promised for
names. `intern_function` takes `modes:` beside `labels:`, and **the language's own rule then made every call name both**
(`needs_label`, two `[str]` parameters): 31 call sites, 21 of them tests, each now writes `labels:` and `modes:`, so no
rebuild site can drop a mode positionally in silence.

What each pass does (files edited, all in the copy; `git diff --stat` at the end of § 4):

- **parser** (`parse/type.hero`): `function_params` reads `@` before a position (a name may follow, `@app: App`) and
  `lent` after its type (`mode_after`); `@T lent` is refused with `lent_shape`, the code `@c: T lent` already earns on a
  signature (`check/read_lends.declared`, panel 217's R1). The AST's `function_type` gains `modes: [str]`
  (`ast.hero`).
- **the type** (`check/table.hero`): the run, its key, `modes_of` (a full-length list, `""` where the run is empty),
  `lends_through_a_type`.
- **the seven rebuild sites** of the critic's § 2 item 4 each copy the modes: `ir/mono.hero:299` (monomorphisation),
  `check/generics.hero` `substitute`, `check/type_fit.hero` `fits`, `check/lower.hero` (a written type),
  `check/fixed_repairs.hero` `growable_twin`, `check/function_value.hero` `signature_of` (the declaration's modes:
  `@` where `mutable`, `lent` where `read_lend.read_only`, so panel 171's `lent` on an extern's `cstr` stays by value)
  and `instance_type` (a generic instance keeps its signature's modes).
- **the two relations**: `generics.bind` refuses a function type whose modes disagree with the parameter's
  (`mismatch_shape`, *before* the positions are bound, so `T` is never bound through a moded position);
  `type_fit.fits` re-interns with the value's own modes, so a mode is never a blank to fill.
- **the value**: `markers.refused_as_value` admits every function of the program; an `extern` keeps both refusals
  (`mutable_parameter_as_value`, `lent_parameter_as_value`), its `@` being a cell C writes: the ffi seat's question.
- **the call through a value** (`check/walk.hero` `indirect_call`, `check/markers.hero` `through_a_value`): each
  argument's `@` is held to its position's mode as a call by name holds it to the declaration's, by the same
  diagnostic, `data_errors.marker_mismatch(name: <the callee as written>, wants:, blocked:)`; a missing `@` where the
  place can take it gets the `certain` fix, since the type gives one reading. A string's byte is refused as an `@`
  argument there too (`access.refuse_str_element`, defect 440's rule).
- **exclusivity**: two `@` arguments were already the resolver's, by the call site's marks
  (`resolve/writes.refuse_aliased_places`, every call); the `lent` half (`check/read_lends.hero`) now asks a callee's
  **type** where the call names no declaration (`lent_of`: a `.call`'s callee type, a field call's field type through
  `name_types.field_of_function_type`).
- **the lowering** (`ir/flatten.hero` `value_call`): the modes are read from `build.value_ty(b, target)`, the callee
  value's own type, and each argument lowered by `value_argument`: `@` the caller's place (`inout.argument`), `lent` in
  place (`lent_argument`), else the value, **an extra `@` included** (Part 11's *"says nothing: the value is passed"*,
  which closes probe `x1`'s exit 2, § 3).
- **the emitter** (`emit/types.hero` `function_type`, `emit/typeorder.hero`): an `@` or `lent` position is `T *`,
  the pointer `signature.one_param` already gives a function's own `@` slot and a `lent` one (`ir/lower.hero:201`
  lowers `lent` as `mutable: true, read_only: true`), so the value's typedef and the function's C type agree.
- **the renderer** (`check/render.hero`, `moded`): `@` before, `lent` after, so `content_key` and `instance_key`
  (`emit/synth.hero:35`, `:47`), which hash this rendering, carry the mode: **panel 112's R6 silent site closed by
  construction**, and every message that shows a type shows its modes.
- **the formatter** (`print/types.hero`): the same spelling back.
- **threads** (`check/thread_ops.hero`, `thread_errors.hero`): a body whose parameter is `@` or `lent` is
  `thread_body`, *"threads share nothing but a mailbox"*.

**Two builds.** `heroes-261-v1` (`sources 7f9360a71ea8d240`): the above, a mode pure identity. `heroes-261-v2`: the
above **plus one widening**, `type_fit.lent_where_at`: a `lent` position where `@` is asked fits, read as `@` (both are
the caller's place as one `T *`; the call site writes `@` and is held to `@`'s exclusivity; the callee only reads), and
`bind` agrees with it. Nothing else fits: `@` where `lent` or a copy is asked, and `lent` where a copy is asked, stay
`type_mismatch`. This is question 5's conversion, the names' one-way fit in another dimension (§ 3, `q5_*`).

## 2. What a mode through a value can hide: three holes found by building, and the repair each needs

Run with `ce-scratch/run_cases.sh <compiler> <tag> <cases>` (check, then `build` and the binary under `timeout 30`,
output bounded; outputs `ce-scratch/out-<tag>/`). Every case is a program under
`docs/panel/261-reports/programs/compiler-engineer/`, its expectation in its header.

**The route as the brief's § 1 states it, built without the two repairs below, is unsound, measured** (`heroes-261-v2`,
`sources 742779af14ef4557`):

- **`w5_heap_through_value`**: `h(@p, "x")` through a value of `(function(@Bag, str) -> ())`, `p` a by-value
  parameter whose record holds an array: **checks, builds, and the binary aborts, exit 134, *"panic: an array
  released after its last reference — a compiler bug, please report it"***. It is the class M-robustness-guards step 1
  closed for a call by name (`resolve/writes.hero:16-28`: *"accepted and corrupted the heap — exit 133 or exit 134"*),
  reopened through a value. `w1_at_on_binding`: the same `@` on a `=` binding, a by-value parameter and a loop
  variable, accepted and each written (prints `1`, `2`, `6`, exit 0).
- **`m06_alias`, `w4_alias_field`**: `h(a: @app, b: @app)` through a local, a field and an element, **accepted**,
  the callee handed one place under two names (prints `2`; `4`). Panel 010's refusal, reopened.
- **Why**: `resolve/built_marks.hero:101-105` (base) **sets aside every `@` of a call through a value**, *"A function
  value: no function type carries an `@` (defect 398)"*, so neither `writes.inout_root` (the rule of the left of `@`)
  nor `writes.refuse_aliased_places` (panel 010) is asked of it. The premise died with the proposal, and the comment
  goes on reading as correct: `.claude/rules/module-shape.md`'s *"a premise about the world expires silently"*, found
  by running the shape, never by reading the critic's ten sites, none of which is this one. **An eleventh site, in the
  resolver, which no brief names.**
- **First repair, `heroes-261-v3` (`sources 606afc9b03e4742c`), and why it was wrong**: `built_marks.of_a_call` kept
  every mark of a call through a value, so both rules were asked in the resolver. It closed `w1`, `w4`, `w5` and `m06`,
  and **reopened defect 431's class**: the resolver cannot read the type, so an EXTRA `@` through a by-value value on
  a `=` binding is told `not_mutable` (the wrong repair; the right one removes the `@`), and a resolver refusal stops the
  checker, so `tests/golden/check/fixedbugs-431-an-at-argument-a-call-takes-none-for` went from its 9 pinned
  `marker_mismatch` lines to **2 `not_mutable` lines, 7 messages about calls BY NAME lost** (`check --brief`, `diff`
  against the base's, `ce-scratch/g-fixedbugs-431-*.v3`). Measured, then undone.
- **The repair kept, `heroes-261-v4`**: the resolver sets the marks aside as at the base (its comment's premise
  rewritten), and **the checker, which reads the mode, asks both rules of each `@` the type takes**:
  `markers.through_a_value` asks the rule of the left of `@` (`markers.blocked`, the words the resolver's `not_mutable`
  uses, code `not_mutable`), and `read_lends.aliased_through_a_value` asks panel 010's with the resolver's own steps
  (`aliased_places`, `writes.place_path`'s step rule via `read_lends.path_of`) and message
  (`writes.aliased_mutable_arguments`). This also covers **a twelfth site**: a field call where a top-level function of
  the same name also stands (`f4`'s shape, panel 079's field first), whose marks the resolver read from THAT function
  (`w3_field_over_function`: under v2, `h.g(@b, 10)` on a `=` binding `b` printed `20`, `20`).
- **And one message for one mistake**: a cell handed bare to an `@` position through a value (`m04_missing_at`) got,
  under v1 and v2, `never_rebound` beside `marker_mismatch`, **two `certain` fixes** (`=` and `@`) that applied together
  write a program that does not check (`app = …` then `h(@app)`, `not_mutable`). By name the base says one
  (`m04b_missing_at_by_name`: `marker_mismatch` alone), because `resolve/meant.meant_by_signature` reads the
  declaration. Repair: `resolve/value_calls.hero` (new, 55 lines) leaves such a cell's verdict to the checker
  (`Local.deferred`, defect 630's machinery), and `markers.through_a_value` keeps it where the type asks `@`
  (`kept_cells.keep_root`).

**What a soundness veto compels, by these measurements**: no landing of the form without the resolver's repair and
its cases (`w1`, `w4`, `w5`, `m06`), and the `w5` program in `tests/golden/run/` as a `fixedbugs`-style case, since
nothing else in the net reaches a heap write through a value.

## 3. The cases, base against the route (`heroes-261-v4`, `sources 1283d3066f568534`)

Every line is `ce-scratch/run_cases.sh` (`check`; where it checks, `build` and the binary under `timeout 30`); base is
`heroes-next` (`ce-scratch/base-cases.txt`), v1 to v4 the four builds (`ce-scratch/v<k>-cases.txt`). At the base every
case that writes a mode in a type is `expected_type` (`@`) or `expected_function_type_params_close` (`lent`), exit 1.

| case | what it asks | v4 |
|---|---|---|
| `m01_at_value` | `h: (function(@App, i64) -> i64) = bump`, `h(@app, 2)` | `2`, `5`, `5`, exit 0 |
| `m02_lent_value` | `(function(Config lent) -> i64)`, annotated and inferred | `7`, `7` |
| `m03_three_types` | `@`, `lent`, by value: three types | `type_mismatch` ×2 (`@` where a copy is asked; a copy where `lent` is); `lent` where `@` is asked fits (v1: ×3) |
| `m04_missing_at` | `h(app)`, `h` taking `@App` | `marker_mismatch` alone, fix `certain` `@` (v1, v2: plus `never_rebound` with a certain `=`) |
| `m05_extra_at` (probe `x1`) | `fs[0](@x)`, by-value type | `marker_mismatch`, *"`fs[0]` does not change this argument"*; **`build --permissive`: `6`, `3`, exit 0** (base: exit 2, clang) |
| `c1_control_missing` | `h(app)` and `h(App(n: 7))` under `build --permissive` | `1`, `2`: the type decides, the place written, a value held in a slot |
| `m06_alias`, `w4_alias_field` | two `@` through a local, a field, an element | `aliased_mutable_arguments` (v1, v2: accepted, `2`, `4`) |
| `m07_lent_beside_at` | `h(a: @app, b: app)` with `b` `lent`; `l(app, touch(@app))` nested | `aliased_mutable_arguments` ×2 (since v1) |
| `m08_generic_hides` | the critic's case: `apply<T>(f: (function(T) -> ()), x: T)` given `bump(@a: App)` | `type_mismatch`, *"expected `(function(a type parameter) -> ())`, found `(function(@App) -> ())`"*: the `@` is not hidden; the message does not say that the mode is the difference (owed, § 6) |
| `m09_generic_each` | `each<S>(@s: S, hs: [(function(@S, i64) -> ())], k)` at `App` and `Counter` | `3`, `10` |
| `q2_field_callee`, `f4_field_first` | a record holding, as a field, a handler taking that record by `@` | **build exit 2** (clang: *"unknown type name"*): the typedef cycle of § 3.1 below, **a defect of the base** |
| `q2_routes_index` | `app.routes[0].handler(@app, 1)`, `state.router.handlers[0](@state, 1)`, each handler rewriting the table it was read from | `1`, `2`, `101`, `3`: the callee is the copy read before the lend (the base's `q2a` order); the write lands for the next call |
| `q2_disjoint` | `server.handlers[i](@server.state, r)` | `5`, `7` |
| `q3_containers` | a map's `.must()`, a list grown in a loop, `==` ×2, `[]` by context, a `T?` | `1`, `3`, `6`, `true`, `false`, `0`, `7` |
| `q5_uniform_table` | `health(@app)` that only reads, `logout(@_: App)` | `never_rebound` ×2, unchanged: the signature is held honest |
| `q5_fit_table` | the same table, `health(app: App lent)`, `logout(_: App lent)` | `2`, `3`, `1`, `2`: one uniform `@` table, every signature true |
| `q5_two_tables` | readers and writers in two tables | `2`, `3`, `1` |
| `q5_lent_to_at` | `lent` where `@` is asked; `lent` where a copy is asked (§ 7 (m)) | the first fits; the second `type_mismatch` |
| `q5_self_write` | `app @ app` in an `@` handler | `2`, `3`: still a write, as at the base (`c1`) |
| `q7_nested_type` | `run_twice(@h: (function(@App) -> ()), @a: App)` swapping `h`; `call_moded<T>(f: (function(@T) -> ()), @x: T)` | `3`, `0` |
| `q8_spawn` | `spawn(bump, x)`, `spawn(get, Cfg(n: 4))` | `thread_body` ×2, *"whose parameter is the caller's place — threads share nothing but a mailbox"* |
| `q8_mailbox` | a `(function(@App, i64) -> ())` sent through a mailbox, called on the other thread's own place | `11`: a function value is code, no place, and crosses as any copy |
| `k_constant_table` | `constant WRITERS: [(function(@App) -> ())]`, `constant READERS: {str: (function(App lent) -> i64)}` | `1`, `2`, `3` |
| `w1_at_on_binding`, `w5_heap_through_value` | `@` through a value on a `=` binding, a by-value parameter, a loop variable | `not_mutable` (v2: ran; `w5` **exit 134**, heap) |
| `w3_field_over_function` | a field call over a same-named top-level function, `@` on a `=` binding | `not_mutable` (v2: `20`, `20`) |
| `lib_value/` | Rebel's `web.add(@router, method:, pattern:, name:)` standing as a value of `(function(@web.Router, method: str, pattern: str, name: str) -> ()?)`, handed to the program's function and called twice | `2`, `not_found`, `b`, exit 0: the library's `hero_value_` twin (no `HeroPlace`) takes the `@` slot as the pointer its signature test already writes |
| `r6_three_typedefs` | panel 112's R6: `@`, `lent` and by value | three typedefs, `h_0fn_5ada5d8c` and `h_0fn_58ef5c0d` with `h_r6threetypedefs_App *`, `h_0fn_68e3c5c3` by value (`--emit-c`); runs `1`, `1`, `1`; and a unit test in `check/table.hero` (*"a mode is part of a function type's identity, and a type with none keeps its key"*) |
| `knot/` | § 7 (o): a block walker handing itself to an item walker in a module that does not `use` it, `@b` threaded | `1 [ 2 [ 3 ] 4 ] 5`, `0`, `2` |

**The five check goldens that name the retired refusals, under v4** (`check --brief`, `diff` against the base's output,
which equals each `.expected`): `fixedbugs-398-a-function-with-an-at-parameter-has-no-value` moves from 12 lines to 7
(the values it pinned as refused are now legal where their context's type carries the mode; the rest become
`type_mismatch` or `cannot_infer`); `fixedbugs-398-an-at-through-a-function-value-marks-nothing`, `-412-` and `-431-`
keep every line and code and change one phrase, *"a function value"* becoming the callee as written (`` `g` ``,
`` `h.f` ``, `` `pick()` ``); `lent-a-function-value-a-marked-call-and-lent-beside-at` moves one line,
`lent_parameter_as_value` becoming `type_mismatch`. The `.hero` of the first and the last hold programs whose meaning
changes and are owed a rewrite, not a re-bless. **Retired codes**: `mutable_parameter_as_value` and
`lent_parameter_as_value` survive on `extern` alone; `data_errors.marker_through_a_value` has no caller.

### 3.1 A defect of the base the router's natural shape meets

`base_self_field.hero`, no mode at all: `record Box { n: i64, f: (function(Box, i64) -> i64) }`, `b.f(b, 10)`. Under
`heroes-next`: check exit 0, **build exit 2**, *"baseselffield.c:18:56: error: unexpected type name 'int64_t': expected
identifier"*: the function typedef is ordered before the struct it names, since `emit/type_needs.hero` counts a function
type's parameters as by-value containment (a cycle the checker does not refuse, panel 023 refusing only by-value
cycles). With a mode the same cycle is `(function(@Box, i64) -> i64)`, `q2_field_callee` and `f4_field_first`, exit 2
on every build. Not filed (no GitHub write is mine); searched `gh issue list --search` for *"record holding function
value itself typedef"* and *"unknown type name"*, open and closed, nothing on point. Class: blocking (an exit 2). The
repair is the emitter's (a forward `typedef struct h_X h_X;` before a function typedef that names `h_X`, which C
accepts for a parameter in a declarator); unbuilt here.

## 4. What it costs, where it lands, what it moves

**Lines** (the copy at v4 plus the R6 test, `git diff --numstat selfhost` and `wc -l` of the new file): **25 files of
`selfhost/`, 24 changed and 1 new (`resolve/value_calls.hero`, 63 lines); 603 lines added, 76 removed.** In the unit
`tests/harness/suite_layout.hero` counts (code lines outside `test` blocks, `.claude/hooks/ceiling.py`'s mirror,
`ce-scratch/ceilings.txt`): **7,661 to 8,096, +435**, by pass: parser and AST +29 (`parse/type.hero` +26,
`ast.hero` +3); the type and its relations +108 (`check/table.hero` +46, `type_fit` +25, `generics` +12,
`function_value` +17, `fixed_repairs` +6, `render` +9, `lower` 0, `ir/mono` +6); the call through a value and its
rules +194 (`read_lends` +90, `markers` +31, `kept_cells` +11, `walk` +5, `resolve/value_calls` +54,
`resolve/built_marks` +2, `resolve.hero` +2); threads +24 (`thread_ops` +11, `thread_errors` +13); lowering +52
(`ir/flatten.hero`); emitter +9 (`emit/types` +8, `emit/typeorder` +1); formatter +5 (`print/types`). **Core, not
sugar**: every pass from the parser to the emitter learns it (design.md §1.7: *"this determines the size of your
compiler"*; Part 5's table has no row a mode in a type could be erased into).

**Ceilings crossed: 7 of the 25 modules** pass their `DECIDED` row or the 300 default (`ceiling.decided`):
`ast.hero` 553 of 550, `check/generics.hero` 311 of 300, `check/table.hero` 414 of 368, `check/walk.hero` 1,882 of
1,877, `ir/flatten.hero` 1,297 of 1,250, `ir/mono.hero` 390 of 384, `parse/type.hero` 326 of 300. Each is a cut or a
decided row the landing owes; `layout` narrowed to them is unrun.

**Against the precedent and the counts the brief carried**: the labels run `2724a4f9a` touched 16 files of
`selfhost/`; this touches 25. Panel 112's R6 *"2 edits in 1 file"* (carried, for a `const` qualifier) is not what a
mode costs: the field is 2 edits, and the relations, rebuilds, rules, lowering and emitter are the other 23 files. A
new `Ty` case instead of a field (`function_ty` named on 374 lines in 141 files, `00-shared.md` § 4) was not built:
the field needed no match arm anywhere, since every arm binds the case's payload whole, which is the measured reason to
prefer it.

**Byte identity, the promise `label_key` made and `mode_key` keeps** (`ce-scratch/identity_all.sh`: `--emit-c` by
`heroes-next` and by `heroes-261-v4`, the `hero_program_sources` line taken out of each, `cmp`): **every program of
`tests/golden/run/` that writes `(function(`, `grep -l` 249 of them**, result in § 4.1 when the run ends.

## 5. The tools that re-print a program (`.claude/rules/diagnostics-and-goldens.md` § A new surface form)

- **`heroes fmt`** (`print/types.hero`, +5): `heroes-261-v4 fmt --in-place` over the 39 `.hero` files kept here, then
  `fmt | cmp` of each: **0 not canonical**; the 17 the first pass moved moved for spacing before a `#~` and long
  literals, none for a mode. Owed: the formatter's self-check (`cli/syntax_cmds.hero`) handed a pair that drops a mode,
  unrun.
- **`--dump-ast`** (`heroes-261-v4 parse --dump-ast`): prints `bind table: [(function(@App, i64) -> i64)] = …` and
  `bind f: (function(Config lent) -> i64) = total`, the same renderer.
- **`heroes doc`** (`heroes-261-v4 doc knot/item.hero`): prints `function walk(@b: Builder, it: Item, block:
  (function(@Builder, [Item]) -> ()))` and the call `item.walk(@b, it, block)`. Its reader
  `rebel/generators/rebel_index/` (defect 2761's) was not run on a moded face: unrun.
- **The diagnostics that quote a program**: `marker_mismatch`'s `certain` `@` through a value, applied
  (`check --apply --in-place` on a copy of `m04_missing_at`): the program then checks, exit 0; on `c1`'s
  `h(App(n: 7))`, no place, no fix is offered and the note says why, as by name.
- **`heroes probe`'s reader** (`probe/reader.hero`), **`heroes mutate`**, **the editor's grammar and the site's
  highlighter**, **`rebel_index`**: unrun. `@` and `lent` are tokens `fmt` neither adds nor drops, so the reader's
  count should not move (an inference); `mutate/handles.hero:122` matches the node by kind and compiles unchanged.
- **`heroes grammar`** prints the spec's productions (`cli/grammar.hero:138`), so it moves with the warden's text and
  holds no copy of `TypeArg`.

## 6. Question 6: the diagnostics, as built

- **A value whose modes differ from the type asked**: `type_mismatch`, both types printed with their modes
  (`m03`, `q5_lent_to_at`, the 398 golden: *"expected `(function(i64) -> ())`, found `(function(@i64) -> ())`"*).
  No fix: which side is meant is the author's. **Owed**: a note naming the position and the two modes; `m08`'s
  *"expected `(function(a type parameter) -> ())`"* does not say the `@` is why, and a reader who does not see the
  mark reads it as a type error.
- **`lent` where `@` is asked**: fits (v2 on), no diagnostic, the one widening (§ 1). `@` where `lent` or a copy is
  asked and `lent` where a copy is asked stay `type_mismatch`.
- **A call through a value missing its `@`**: `marker_mismatch`, the by-name words with the callee as written (*"`h`
  changes this argument, so the call site writes `@` too"*), and **a `certain` fix where the place can take it**: the
  type gives one reading, as a declaration does, unlike `aliased_mutable_arguments`' two (design.md §4.8, panel 110).
  Applied by `check --apply --in-place` on `m04`, the program checks, exit 0. One message, after § 2's repair.
- **Adding one**: `marker_mismatch`, *"`fs[0]` does not change this argument — the `@` says it does"*. The retired
  *"a function value does not change this argument"* (`data_errors.marker_through_a_value`) has no caller.
- **An `@` the type takes before a non-place or a non-cell**: `not_mutable`, the resolver's words through
  `markers.blocked`. **Two `@` reaching one place**, and **`lent` beside an `@` of its place**:
  `aliased_mutable_arguments`, the resolver's and panel 217's messages.
- **Retired codes**: `mutable_parameter_as_value` and `lent_parameter_as_value` stay for an `extern` alone (the ffi
  seat's question); for a function of the program they never fire.
- **The control arm** (`build --permissive`): **the type decides, as the declaration does by name**: a missing `@`
  hands the place (`c1`: `1`, `2`), a value that is no place goes through a held slot, an extra `@` *"says nothing:
  the value is passed"* (`m05` = probe `x1`: **`6`, `3`, exit 0, where the base exits 2**). The four
  `tests/golden/control/` cases pin calls by name and do not move by construction (the lowering by name is untouched,
  `lowered_from`); a case per meaning through a value is owed in that folder. Whether the `control` and `permissive`
  forms move as wholes: unrun (no harness run was made with this compiler).

## 7. The router on the route, and every route of § 6.2 and § 7

Each program below is under `programs/compiler-engineer/<dir>/` with a `heroes.toml` that takes Rebel's library from
the copy's own `rebel/library/0.1.0/files` (reach's list of packages); a fixed list of requests made by
`http.request(method:, target:)`, no network, each answered through `web.route` and a call through a value.

- **(k) the name-keyed map beside the router, `router_k/`** (`App.handlers: {str: (function(@App, web.Found,
  http.Request) -> http.Response?)}`, each route bound at its `add` by `on(@app, …, by: note_page)`, the dispatch three
  lines): under v4, **its 8 lines `cmp`-equal to `main.expected`**, exit 0. Writer `@`, reader `lent`, no-state
  `_: App lent`, all in one `@` table by the fit. No library change. **Approve.**
- **(B) the library's generic faces, `router_b/`** (`faces.on<S>` and `faces.dispatch<S>`, written in a module of the
  program as `web` would hold them; the state `S` a `Notes` record, the table the program's): under v3 and v4, **8
  lines `cmp`-equal to `main.expected`**, exit 0. `bind` meets `lent` where `@S` is asked and agrees by the
  fit. Cost in the library: 2 public functions in `web` (88 today, 90 the fence, `rebel_faces.hero:45-46`), so the
  fence holds at exactly 90 with `on` and `dispatch`; a face per method (`get`, `post`, …) crosses it. **Approve**, with
  (k) the program's table and no handler held by the library.
- **(A) the program's list indexed by the router's own index, `router_a/`**: a group made, a route added to it, a route
  added to the router, then the group mounted, the handlers pushed beside each `add` (the order notes writes,
  `notes/http/routes.hero:143-157`, one route moved): **`/g/a -> b`, `/b -> a`, exit 0, a silent misroute** under v4.
  `web.Found` has no index (`web.hero:301-311`), so the program reads it as `web.route` does, the route's position in
  `router.routes`, which `mount` orders by the mount and the program's list by the push. The coupling of two orders is
  the defect; notes' own order happens to align today, and `openapi.add_routes` adds routes the program never pushes a
  handler for. **Object**: (k) does the same without the coupling and without a library field.
- **(C) `Router<S>`, a generic record**: not built. Panel 252's R3 counted *"288 incomplete matches in 109 files"*
  (carried, `docs/panel/252-*.md:567-570`) for records with type parameters, and a generic variant is refused too
  (probe `g2`). (B) and (k) answer the author's `web.get(@r, "/notes/:slug", note_page)` with the table the program's,
  one argument more. **Object** on cost: nothing (C) does that (B) does not, measured on the same requests.
- **(a) a `handler` record kind**: a new kind the parser, the checker, the lowering and the emitter learn, where a
  function type with modes is the whole of what it holds. **Object** (Part 5: a core construct with no work of its own).
- **(e) a variant of routes matched exhaustively**: kept where the set is closed (gateway's `Kind`). A table loses the
  exhaustiveness: a name bound to no function is `not_found` at run time in `router_k`'s `answer` (`h =
  app.handlers[found.name]?`). Complement, not rival.
- **(f) by value** builds at the base (the clerk's blind references); **(g)** the owner thread is the gateway's and
  untouched; **(j) the constant table** builds (`k_constant_table`: `1`, `2`, `3`); **(l) disjoint places**
  (`q2_disjoint`: `5`, `7`); **(o) the compiler's knot** (`knot/`: runs).
- **(h) `lent` coerced to `@`**: built (v2 on), sound by the C type and the call site's `@` rule; **approve**. The
  `never_rebound` waiver instead: **object**, since it makes a signature that says `@` and writes nothing legal
  wherever the function is also used as a value, and the rule would then depend on uses far from the signature.
- **(i) the no-state handler**: `_: App lent` in an `@` table, by the fit (`q5_fit_table`: `1`).
- **(m) `lent` adapted to a by-value type**: refused (`q5_lent_to_at`): the value's C slot is `T *` and the type's is
  `T`, so it needs a thunk per adapted function, not a fit. **Object**.
- **(p) a callback C runs on another thread**: an `extern` keeps both refusals in this route, so the 18 thread bodies
  (`ground/callbacks.txt`) are unchanged; a mode in an `extern` callback's type is the ffi seat's.
- **A route nobody listed, built**: **the handler bound in a record of the program's own** (`q2_routes_index`'s
  `Route { name, handler: (function(@App, i64) -> i64) }` in `App.routes`): per-route data beside the handler
  (registry's scope, chat's limiter, agent's guard) without a second map, which builds and runs (`1`, `2`, `101`,
  `3`), **through a list**; as a **direct** field of the state's own record it meets § 3.1's typedef cycle, exit 2.

### 4.1 Byte identity, measured

**248 of the 249 programs of `tests/golden/run/` that write `(function(` emit the same C under `heroes-261-v4` as under
`heroes-next`**, the provenance line aside (`ce-scratch/identity.txt`, `cmp` each). **One differs**:
`fixedbugs-417-every-function-value-is-named-before-c-is-written`, 38,238 bytes either way, **two typedefs in another
order** (`typedef HeroStr (*h_0fn_1fb9aa18)(HeroStr, int64_t);` before the two `h_0opt_` structs at the base, after
`h_0fn_7b615d8f` under v4): the same declarations, a function type's id interned later. Each compiler is deterministic
on it (two emissions each, `cmp`); v1, v2 and v3 differ the same way, so the core change moved it; reduced to
`ce-scratch/order/b.hero` (a generic `down<A>` standing as a value of its own instance inside itself, called at `i64`
and at `str`), whose `--dump-ir` is identical under both, so the order moves after the lowering (monomorphisation or the
emitter's seeds). **Cause not found in this sitting**: a landing owes it, since `tests/emission` blesses that file's C
byte for byte, and *"a mode-free program keeps its bytes"* is the promise `mode_key` exists to keep. **A hypothesis,
unrun**: every direct call records its callee's signature (`walk.named_call`, *"the callee is a value in its own
right"*), and the embedded library's two externs with `@status: i64` (`library_source.hero:153`, `:163`) now intern a
moded signature no program had, which shifts later type ids, and an order read off a map keyed by ids would move; the
test is `declared_modes` answering `""` on an `extern` (which no value carries anyway) and this file's `cmp` again.

**The compiler's own C**: `heroes-next build selfhost/main.hero --emit-c` against `heroes-261-v4 build
selfhost/main.hero --emit-c`, both over the copy's edited `selfhost/` (which writes no mode), the provenance line
aside: **40,972,079 bytes each, `cmp` silent** (`ce-scratch/ident/self.txt`). So the seed's own C does not move, and
no seed refresh is owed by the route until `selfhost/` writes the form.

## 8. Principle 0: is the form the compiler's own need?

`knot/` builds and runs the shape panels 109, 121 and 138 measured as blocked (a walk that threads `@b` handing
itself as a value to a module that does not `use` it): `1 [ 2 [ 3 ] 4 ] 5`, `0`, `2`. **But that need was met another
way since**: panel 252's R1 (2026-11-19) makes a ring of modules build (`.claude/rules/module-shape.md`: *"Since panel
252's R1 a ring of modules builds, so the seam exists"*; whether this route made a ring, `check/walk.hero` now using
`check/read_lends`, is unasked: `heroes-next check selfhost/main.hero` exits 0 either way). So the knot is cut by a
`use`, not by a value, and **`needed_for_self_hosting: no`**: the form enters under the author's
amendment of 2026-11-22, on the router's measured need, not under Principle 0.

## 9. The driving programs rewritten on the route

- **reach's `reach_web.hero`, rewritten whole, `programs/compiler-engineer/reach_web/`**: `App` gains `answers: {str:
  (function(@App, web.Found, http.Request) -> http.Response)}`, built by `new_app()` (`"home"` and `"user"` bound to
  `as_app`, a `lent` reader, `"visit"` to `visit`, an `@` writer); `handle` becomes a lookup and a call through the
  value, its `match found.name` gone. `heroes-261-v4 check`: exit 0. `heroes-261-v4 test` (which runs every test of
  every Rebel module the file reaches, by its header's design): **its own four tests passed first** (*"app: a program
  with no state served on the loopback"*, *"serve_conn: a program's state counted across its requests"*, the `100
  Continue` and `431` one, the reverse proxy's), **then 2,829 tests of the library, 0 failed, when `timeout 600` cut the
  run** at load 33 to 40 (`ce-scratch/reach.test`, exit 124): incomplete as a whole-library run, complete for the four
  tests the rewrite touches. Built and run, the binary prints `text/html; charset=utf-8`, its `main`'s line, exit 0. No
  binary of mine was left running (`pgrep -fl build/`, `lsof -d cwd`: the two left were
  other lanes').
- **agent's toolbox, `programs/compiler-engineer/agent/`** (the whole program copied, `tools/toolbox.hero` alone
  changed): `run`'s six `if name ==` become a lookup in `answers(box: Toolbox lent) -> {str: (function(Toolbox lent,
  json.Value) -> outcomes.Outcome)}`, `run_command` bound only where some command is allowed; five adapters written
  (`read_lines`, `list_files`, `count_tokens`, `query_json`, `run_command`; `read_file` is a built-in's name and was
  refused, `builtin_name_taken`). `heroes-261-v4 check`: exit 0. **It costs lines**: 178 to 195 (`wc -l`), and the
  tool's name is still written twice (`specs` and `answers`), as it was (`specs` and `run`): on agent the form buys
  a table of `lent` functions where an `if` chain stood and nothing a reader or the checker can see more. The
  alternative that writes each name once, the answer in `Spec`, meets `tools/executor.hero:38`, which builds a `Spec`
  for a remote server's tool that has no local answer. **Built and run as its `main.args` say (`demo --work
  build/agent/demo`, from `ce-scratch/agent-run/`, `timeout 120`): 9,219 bytes, `cmp`-equal to `main.expected`**, every
  tool call of the scripted session answered through the table.
- **notes' dispatch** (`http/dispatch.hero`, 107 lines) **was not rewritten** in this sitting: `router_a/` measures the
  index route's coupling on notes' own order of `add`, group and `mount`, and `router_k/` the route that does not meet
  it.

## 10. What stays unrun

- **No harness run**: no form of the net (`check`, `run`, `emission`, `control`, `permissive`, `full`, `fixes`,
  `annotations`, `layout`, `probe`, `rebel`) was run with `heroes-261-v4`; the goldens' movement in § 3 is five `check
  --brief` runs and a `diff`, and the 73 files naming `marker_mismatch` (`00-shared.md` § 3.3) were not walked.
- **The compiler's own tests** (`heroes-261-v4 test selfhost/main.hero`): **2,325 tests, 1 failed**, *"the list holds
  every word the runtime's headers write"* (`emit/macro_guard.hero:301`), which reads `runtime/heroes_runtime.h` and
  `runtime/hero_os.h` against `guarded_names`, neither touched here. **The base compiler fails the same one test over
  the same sources** (`heroes-next test selfhost/main.hero`: *"2325 tests, 1 failed"*, the same name,
  `ce-scratch/selftest-next.log`), so it is not the route's; a fact of the copy at `df92f9281` (the runtime's headers
  against the list), unfiled by me.
- **The seed's fixpoint** one generation further: not run; the route reaches no emitted C of the compiler (§ 4.1).
- **The cause of `fixedbugs-417`'s moved typedef** (§ 4.1), a hypothesis named.
- **The forward-typedef repair** of § 3.1 (a record holding a handler of itself): unbuilt.
- **A mode mismatch's note** naming the position (§ 6): unbuilt.
- **`heroes probe`, `heroes mutate`, the formatter's self-check, the editor's grammar, the site's highlighter,
  `rebel_index`**: unrun (§ 5).
- **The ceilings' cuts** for the seven modules over theirs (§ 4): not made; `layout` narrowed: unrun.
- **notes' and auth's, registry's, searchd's and chat's dispatch rewritten**, gateway's, certs', worker's, ingest's,
  backup's and monitor's: not done in this sitting; reach and agent were (§ 9).
- **A large group record through a value** (panel 236's twin) and **a self-call through a value** (panel 199) with a
  mode: not built.
- **Timings**: none taken as a verdict; every build ran at load 9 to 40 (`uptime` beside each), the clock forbidden.
- **The C boundary**: an `extern` keeps both refusals in this route; a mode in a callback's type is the ffi seat's.

## 11. Verdict

- `verdict`: **object** on the proposal as the brief states it, **with a soundness veto on any landing without § 2's
  checker-side rules**; **approve** routes (k) and (B) on the route I built (`heroes-261-v4`), with the `lent`-to-`@`
  fit (h); **object** to (A), (C), (a), (m) and the `never_rebound` waiver.
- `section`: design.md §1.7 (core plus elaboration, *"this determines the size of your compiler"*) and Part 5 (the
  core/sugar table, which has no row a mode could be erased into); the veto on §4.8 (*"`@` parameters never alias, so a
  direct-pointer lowering and `restrict` stay legal"*, lines 1620 and 1621) and §1.12 (robustness: no memory
  corruption), both measured broken through a value by the route as mapped.
- `implementation_cost`: **+435 code lines** (`suite_layout`'s unit; 603 added, 76 removed raw) in **25 modules of
  `selfhost/`** (24 changed, `resolve/value_calls.hero` new): parser and AST 29, the type and its relations 108, the
  call through a value and its rules 194, threads 24, lowering 52 (`ir/flatten.hero`), emitter 9, formatter 5; **7
  modules over their `DECIDED` ceiling** (`ast`, `check/generics`, `check/table`, `check/walk`, `ir/flatten`,
  `ir/mono`, `parse/type`). Owed beyond the prototype: § 3.1's typedef repair, § 6's note, § 4.1's moved emission, the
  golden cases (`w1`, `w4`, `w5`, `m04`, `m06`, `c1`, `m05`'s control arm), five `check` goldens moved, the spec's
  text.
- `needed_for_self_hosting`: **no** (§ 8: panel 252's R1 rings cut the knot a value would have).
- `argument`: The form is core: every pass from the parser to the emitter learns it, 25 modules and +435 lines. Built
  as the brief maps it, ten sites, it is unsound: the resolver sets every `@` through a value aside
  (`resolve/built_marks.hero:101-105`), so an `@` on a by-value parameter aborts the heap (`w5`, exit 134) and two `@`
  alias one place (`m06`), at sites no brief named. With both rules asked in the checker (v4) every shape tried holds,
  248 of 249 emissions and the compiler's own 40,972,079 bytes stay identical, and the name-keyed router and the
  generic faces run Rebel's `web` to their expected output. I object rather than approve on seven ceilings crossed, a
  base defect (exit 2) the router's direct-field shape meets, and one emission moved unexplained.
- `prediction`: **the landing commit of this form adds at least 450 code lines to `selfhost/` in `suite_layout`'s
  count (test blocks excluded) across at least 25 modules**, checkable at that commit in M-rebel-library by `git diff
  --numstat` read through `ceiling.code_lines`; fewer means a site of § 1 or § 2 was left out.
- `condition`: I approve the route when (1) the landed compiler refuses `w1`, `w4`, `w5` and `m06` and runs `m01`,
  `q5_fit_table` and `router_k` as here, each a golden case; (2) `q2_field_callee` builds (§ 3.1's repair); (3)
  `fixedbugs-417`'s emission is byte-identical or its cause is named in the landing's record. The veto lifts with (1)
  alone; evidence that a mode is lost at a thirteenth site (bind_missing is safe only because `fits` follows it,
  `function_value.instance`) would reinstate it.
