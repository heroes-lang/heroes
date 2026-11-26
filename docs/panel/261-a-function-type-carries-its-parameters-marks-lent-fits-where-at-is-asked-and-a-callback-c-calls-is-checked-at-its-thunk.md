# Panel 261: a function type carries its parameters' marks; `lent` fits where `@` is asked; and a callback C calls is checked at its thunk

**Resolution adopted: provisional — author ratification pending.** Full panel: the compiler-engineer, the ffi-pragmatist,
the spec-warden, the historian, the completeness critic twice (on the briefs, then on the reports); the llm-ergonomist
registered and not run, its paid run authorised by the author (*ok a 20 dollars*, 2026-11-26) with a ceiling of 20.00
USD (`261-briefs/llm-ergonomist.md`), started by the coordinator after this synthesis. Convened 2026-11-26 on **the
author's proposal**, relayed by the coordinator and meant as: *I want the language change now, in the most robust and
complete way, because as it is it is too limited; study a solution that stands, then make every change to Rebel's
examples and library*; and widened at about 13:01 the same day by **the author's addition**, meant as: *before the
milestone's tag I want web middleware handled the way Go's Echo or Go's standard library do it, and used in the
examples* (§ The author's addition, below). Briefs: `docs/panel/261-briefs/` (written at `07e747bc6`, repaired from
the critic's first reading and moved to the base `df92f9281`); reports: `docs/panel/261-reports/`, the seats' programs
under `programs/` (the critic's sources and text outputs copied by this synthesis from
`.claude/worktrees/261-critic-scratch/r2/`, 510 files, no binary, no `build/`; the synthesis's own under
`programs/synthesis/`). **Every cell below is at `df92f9281`** unless it names `main` (`a60dfb77f`, 15 commits on, none
touching the route's 25 files, the spec or design.md: the critic's `git diff --stat`). The synthesis's runs are this
Mac's, 2026-11-26 from about 12:48, at load 6 to 10 (`uptime`), with four compilers copied into
`.claude/worktrees/panel261-scratch/synth/bin/`: **base** (the critic's stage 2, `sources 4dcf3a9f20257573`, the
repair's key), **route** (the critic's build of `programs/compiler-engineer/route-v4.diff`, `sources
7cc4a75a5a7c6bf1`, which answered 46 of 46 cases as the seat's `heroes-261-v4` did, `critic-reports.md` § 8.1),
**main** (the trunk's stage 3 at `a60dfb77f`, `.claude/worktrees/gate-push4/heroes-next`, `sources
6e60cb9f65cc1298`) and **h417** (the route with one change, § R8, `sources 239afb8a2ad36e7e`); and the seat's own
`heroes-261-v4` (`sources 1283d3066f568534`) by its absolute path, never rebuilt. Programs and outputs:
`programs/synthesis/`, its runner `runp.sh` (`check`, then `run` under `timeout 60`, each exit printed).

## The proposal, as put to the seats

`00-shared.md` § 1, in four items: **a function type carries each parameter's mode**, `@` before a type and `lent`
after it, and a mode is part of the type's identity, `(function(@App) -> ())`, `(function(App lent) -> ())` and
`(function(App) -> ())` being three types; **a top-level function with `@` or `lent` parameters is a value**, spec § 9
line 321 losing *"but not one with an `@` or `lent` parameter"*; **a call through a value writes the modes as a call by
name does**, `handler(@app, r)`, under § 9's exclusivity (lines 314 to 319); and **the sitting decides** what a mode
means in an `extern` callback's type (§ 13), in `spawn(f, x)` (§ 11), a mode mismatch's diagnostic and fix, and how
Rebel's router binds a route to its handler (`web.get(@r, "/notes/:slug", note_page)`). Today `p1`
(`h: (function(@App, i64) -> i64) = bump`) is `expected_type` at the `@`, `p2` `mutable_parameter_as_value`, `p3`
`lent_parameter_as_value`, `p4` `expected_function_type_params_close` (`00-shared.md` § 3.3), and `web.hero:19-23` says
why Rebel's programs dispatch by hand: *"a function value takes no `@` parameter (spec § 9), so no handler a router
held could carry the program's state"*.

## What the sitting found before judging, and beside it

- **The route as the brief mapped it is unsound, and the hole was at a site no brief named.**
  `resolve/built_marks.hero:101-105` sets every `@` of a call through a value aside (*"no function type carries an `@`
  (defect 398)"*), so neither the rule of the left of `@` nor panel 010's two-`@` rule is asked of it. Built without a
  repair (`heroes-261-v2`), `w5_heap_through_value` (`h(@p, "x")`, `p` a by-value parameter holding an array) **checks,
  builds and aborts, exit 134, *"panic: an array released after its last reference — a compiler bug"***; `m06_alias`
  and `w4_alias_field` hand one place to two `@` positions and run (the compiler-engineer, § 2). The critic's ten sites
  (`critic-briefs.md` § 2 item 4) were a reading of `intern_function`'s callers, which drop a **type**; this one drops a
  **mark**, and the field call over a same-named function (`w3_field_over_function`) is a twelfth.
- **The compiler-engineer's built route lets C call an `@` function through the base's thunk.** Its *"an `extern` keeps
  both refusals"* is true of an `extern` function used as a value and not of an `extern`'s callback parameter: the
  critic's `ffi/shapes/v4_shapes.hero`, the ffi-pragmatist's shapes written as the proposal lets a program write them,
  **builds under `heroes-261-v4` and prints `40040`, `4702111234474983745`, `42` at exit 0**, and under `--sanitize`
  stops at **`heap-use-after-free ... WRITE of size 8 ... in hand_over shapes.h:34`**; the same on Linux x86-64 and
  Linux arm64 (`critic-reports.md` § 1, § 8.2, § 8.3). The emitted thunk forwards `ph0_h` unchecked
  (`programs/critic/ffi/shapes/v4_shapes.c:325-327`). That is the landing the ffi-pragmatist vetoes.
- **A literal's element type is its first element's, so the fit depends on the order of a table's entries**, and it
  did before this sitting, for names. The critic's `t6_map_literal_order` (`{"GET": get, "SET": set}` refused,
  `type_mismatch`; `{"SET": set, "GET": get}` accepted) and `t1_literal_order` under the route. **The synthesis's
  shape beside it, at the base and on `main`, with no mode**: `probe/names_order_a.hero` (a value typed with names first,
  then a function whose value names none) checks and prints `4`, `2`; `names_order_b.hero` (the same two, the other
  order, in a map) is **`type_mismatch`: *"expected `(function(i64, str) -> i64)`, found `(function(n: i64, s: str) ->
  i64)`"***, exit 1, on base, main and route alike. And an `if`'s branches and a `match`'s arms take no fit at all, in
  either order: `branch_names.hero` refuses all three (base, route), `branch_modes.hero` refuses each mixed branch and
  arm under the route (`type_mismatch` ×3, then `marker_mismatch` ×2 from the types fixed so).
- **The names' fit runs the other way from what the brief handed every seat.** A value whose type leaves a position
  unnamed fits a type that names it, never the reverse: the warden's `w4` (`double(n: i64)` where `(function(x: i64) ->
  i64)` is asked) prints `8`, its `w5` (a value typed `(function(x: i64) -> i64)` where `(function(i64) -> i64)` is
  asked) is `type_mismatch`; the critic's `n1_names` and `n2_names` the same under the base and the route;
  `check/type_fit.hero:13-22`: *"a name that is not there fits whatever the expected type calls that position"*. The
  error was the critic's first reading's, from feature 1133's body, carried by the repair into `00-shared.md` § 1 item
  1; it did not mislead the route (the `lent`-to-`@` fit runs in the names' real direction), and feature 1133's sentence
  is false at the base (the critic).
- **An `extern` that takes a callback, called through a function value, hands C the raw function, not its thunk.** The
  synthesis's `programs/synthesis/ext/callback_through_value.hero` (the ffi-pragmatist's pool allocator, C's fresh node
  at an address the program gave back): `hand_fresh(peek)` by name prints `11`; `f = hand_fresh` then `f(peek)`
  **aborts, exit 134, *"a C handle reached the argument `n` of `node_peek` at an address a call marked `consumes` or
  `transfers` ended ... a copy made before the ending call"*, false**, on base, main and route. The emitted C
  (`ext/ecv.c:259-260` against `ext/ecb.c:256`): through the value `t4 = t2(t3)` with `t3` the function itself; by name
  `call_val((h_0fn_...)hero_callback_of((void (*)(void))t1))`. So every duty this sitting puts in the thunk could be
  stepped around today; drafted as a defect (§ Found beside).
- **`fixedbugs-417`'s moved typedefs have a cause, measured.** The compiler-engineer found the one mode-free emission
  of 249 that moved and named a hypothesis (the embedded library's two `extern`s with `@status: i64` now intern a
  marked signature, shifting later type ids). The synthesis built it: the route with `declared_modes` answering by
  value for an `extern` (`programs/synthesis/h417-declared-modes.diff`, 8 lines of `check/function_value.hero`), stage 2
  from the route compiler, `sources 239afb8a2ad36e7e`; `--emit-c` of the case, the `hero_program_sources` line taken
  out, **`cmp` equal to the base's**, where the route's differs at line 85; and 14 cases (`m01`, `m03` to `m06`, `m08`,
  `q5_fit_table`, `w1`, `w5`, `r6`, `q8_spawn`, the critic's `t6` and `s2b`, and `extern_at_value`) print the same
  under h417 as under the route, `check` and `run` (`probe/h417cases/`, unkept, outputs compared by `md5`).
- **No closure and no generic record are needed for the router or for middleware**, measured (§ The author's addition):
  the compiler-engineer's `router_k` and `router_b` print their `main.expected` under the route, and the synthesis's
  `programs/synthesis/mw/` runs a chain of three middleware kinds, the state reached through `@`, under both route
  compilers.
- **The compiler's own knot is cut today by a ring, not by a value.** The critic built panel 109's shape as two modules
  that `use` each other and thread `@b`, at the base: `1 [ 2 [ 3 ] 4 ] 5`, exit 0 (`programs/critic/mine/ring/`). So
  **`needed_for_self_hosting: no`**, measured on the shape; the form enters under the author's amendment of 2026-11-22
  (CLAUDE.md § Precedence), not under Principle 0.
- **Rust, Swift and Nim refuse the fit, and none of them needs it.** The critic ran each on this Mac: a read-only
  function where a read-write function type is asked is refused (`swiftc` 6.4.2 *"cannot convert"*, `rustc` 1.100.0
  `E0308` *"types differ in mutability"*, `nim` 2.4.2 *"type mismatch"*). The synthesis ran the shape beside it, a
  read-write parameter the function only reads, in a table of read-write functions (`programs/synthesis/prec/`):
  **`rustc -W unused` exit 0, `swiftc -swift-version 6` exit 0, `nim c` exit 0, each printing `3 6` with no
  diagnostic**. Heroes refuses that function, `never_rebound` (`u1`, `00-shared.md` § 3.4). So in those three a
  read-only handler stands in a writers' table by being declared a writer; here it cannot, which is the measured reason
  the fit is needed here and was not there.
- **`use` cannot name a function**: `function use(n: i64)` is `error[expected_name]` under the route
  (`probe/use_name.hero.txt`), so Echo's `e.Use` has no spelling `web.use`.

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** to the proposal as briefed, **soundness veto** on any landing without the checker's two rules through a value; **approve** (k) and (B) with the fit (h); object to (A), (C), (a), (m) and the `never_rebound` waiver | design.md §1.7, Part 5 (core: every pass learns it); the veto §4.8 lines 1620 and 1621 and §1.12 | **+435 code lines** (603 added, 76 removed) in **25 modules** (24 changed, `resolve/value_calls.hero` new); **7 modules past their ceiling** (`ast` 553 of 550, `check/generics` 311 of 300, `check/table` 414 of 368, `check/walk` 1,882 of 1,877, `ir/flatten` 1,297 of 1,250, `ir/mono` 390 of 384, `parse/type` 326 of 300) | the landing commit adds **at least 450 code lines to `selfhost/` in at least 25 modules**, read by `ceiling.code_lines` | approves when the landed compiler refuses `w1`, `w4`, `w5`, `m06` and runs `m01`, `q5_fit_table`, `router_k` as goldens, `q2_field_callee` builds, and `fixedbugs-417` is byte-identical or its cause named; the veto lifts with the first |
| ffi-pragmatist | **object** to the proposal as briefed, **conditional veto** on a landing that lets C call an `@` or `lent` function through the base's thunk; **approve** a mode in an `extern` callback's type, meaningful, on its five conditions and route F; `spawn` with a mode refused by a code of its own | design.md §1.11, §4.19, Part 8 item 19 (its counts stale), §4.8 lines 1620 and 1621, §1.12 | the thunk's duties, one compare per slot per call C makes; `const` for a lent group record; diagnostics' text | **nftw binds with no shim and no self-write** on a landing with route F, three legs; `sqlite3_exec`'s row callback still needs Part 8's shim; of `sqlite3.h` 3.54.0's 20 API-parameter callback types **17** pass clang on this Mac (14 on Linux), **0 of 92** vtable types | approves on (a) handles cleared and checked, (b) a null slot refused naming callback and slot, (c) `@` slots compared or §4.8 restated, (d) `const` for a lent group record in typedef, thunk and function or twin, (e) route F or another rule for a read-only callback with no self-write, each a `run` case on three legs |
| spec-warden | **approve `spec-w1-fit`, provisional**; **object** to `spec-w3-waiver` (§1.3) and to `spec-w2-identity` as the adopted text; W1c only where the C of a callback's mark is built | design.md §1.6, §1.2, §1.3; CLAUDE.md § Precedence as amended 2026-11-22 | W1 **+149** cl100k_base (10,245 to 10,394), a lower bound, vendored only; real about +186 to +203, an inference | under W1 at most 1 of 10 t2 and t3 readers refused for a mode (D3), at least 4 of 5 right on t1 and t2 (D1); W1's real delta +186 to +203 | withdraws if the fit is applied at naming only and not in `fits` and `bind`, if `app.handler(@app, r)` is refused, if D3 fails, or if W1 reads past +250 real |
| historian (advisory) | **approve** | precedent | none | none | turns to object on a language that refused procedures with by-reference modes as values and kept it, a bug class traced to a call-site mark through a value, a router that handed stored handlers an exclusive state and withdrew it, or a language that removed modes from its function types |

The completeness critic gives no verdict; its second reading (`critic-reports.md`) ran the compiler-engineer's route
on this Mac, Linux x86-64 and Linux arm64, rebuilt it from the published diff (46 of 46 cases alike), ran the
ffi-pragmatist's shapes on it, three shapes of its own beside question 2, and the historian's precedents on three
compilers. The llm-ergonomist's reading is registered (`261-briefs/blind/registration.md`), not run.

## The resolution adopted (provisional)

**R1, the form: a mark in a function type, part of the type, and every function of the program a value of its
signature's type** (built, `heroes-261-v4`). `TypeArg = "@" [ ident ":" ] Type | [ ident ":" ] Type [ "lent" ]`, `Param`'s
shape with the name optional, so `@T lent` is refused by the production and by the checker, `lent_shape` (*"`lent`
after a position marked `@` — an `@` position already hands the caller's place"*, `probe/lent_shape_type.hero.txt`),
and a name after the mark, `(function(a: @App) -> ())`, is `expected_type` (the critic's `t3_name_then_at`). **A mark
run on the existing case, never a new `Ty` case** (panel 112's R6 shape; the labels run's precedent, `2724a4f9a`):
`function_ty` gains `modes`, stored **empty** where every position is by value and keyed `mode_key`, `""` for an empty
run, so a mark-free type keeps its key, its id and its C typedef's name; `intern_function` takes `modes:` beside
`labels:`, and the language's own `needs_label` then made every one of its 31 calls name both. The seven rebuild sites
copy the marks; `generics.bind` refuses a marked mismatch before any position binds (`m08`: *"expected `(function(a type
parameter) -> ())`, found `(function(@App) -> ())`"*, the `@` not hidden); `type_fit.fits` re-interns with the value's
own marks; `check/render.hero` prints them, so `content_key` and `instance_key` carry them: **panel 112's R6 silent site
closed by construction**, `r6_three_typedefs` emitting three typedefs for `@`, `lent` and by value, and a unit test in
`check/table.hero`. A top-level function of the program, a generic instance and a library function through its
`hero_value_` twin (`lib_value`: `web.add` as a value, `2`, `not_found`, `b`) stand as values; a call through a value
writes `@` where its type does (`m01`: `2`, `5`, `5`; `m02`: `7`, `7`; `q3_containers`: a map's `.must()`, a list grown
in a loop, `==` twice, `[]` by context, a `T?`). **The callee read out of the place its own call lends is the copy
taken before the lend**, the base's order for a nested `@` (`q2a`, `q2b`) kept: `q2_routes_index` `1`, `2`, `101`, `3`;
the critic's `s1_map_callee` (the map emptied under the running callee, `--sanitize` clean), `s2`, `s2b`, `s3` (a
generic, a module crossed, the callee a call's answer) and `t2_call_result`; Rust's Reference states it (*"Expressions
taking multiple operands are evaluated left to right as written in the source code"*, its *Evaluation order of
operands*, listing *"Call expression"*; the critic, the 1.90.0 toolchain's offline copy), and Swift 6.4.2 and Rust
1.100.0 run the same shape, `app.h(&app, 2)` and `(app.h)(&mut app, 2)`, printing `3`, `9`, `10` (the critic). **Needed
for self-hosting: no** (the critic's ring, above).

**R2, the checker asks both rules of each `@` the type takes** (built, `heroes-261-v4`), the soundness veto's
condition. The resolver keeps its set-aside of a call through a value (`resolve/built_marks.hero:101-105`), its premise
rewritten, because the resolver cannot read a type: **v3 asked both rules there** and reopened defect 431's class, an
extra `@` through a by-value value told `not_mutable` and a resolver refusal stopping the checker, so
`tests/golden/check/fixedbugs-431-an-at-argument-a-call-takes-none-for` went from its **9 pinned `marker_mismatch`
lines to 2 `not_mutable` lines, 7 messages about calls by name lost** (the compiler-engineer's `check --brief` and
`diff`; the critic re-ran the five goldens, 12 to 7, 3, 5, 9, 3 lines). So **the checker, which reads the mark, asks
them**: `markers.through_a_value` the rule of the left of `@` (`not_mutable`, the resolver's own words through
`markers.blocked`), `read_lends.aliased_through_a_value` panel 010's rule with the resolver's steps and message
(`aliased_mutable_arguments`), and the nested-`lent` rule of a callee's **type** where the call names no declaration;
`resolve/value_calls.hero` (new) leaves a bare cell's verdict to the checker, so a missing `@` through a value is one
message (`m04`: `marker_mismatch` alone with a `certain` `@`, where v1 and v2 printed `never_rebound` beside it with two
`certain` fixes that applied together do not check). **Kept as goldens, each refused**: `w1_at_on_binding` (an `@` on
a `=` binding, a by-value parameter, a loop variable: three `not_mutable`), **`w5_heap_through_value` as a `run`-form
case** (`not_mutable`; nothing else in the net reaches a heap write through a value), `w4_alias_field` and `m06_alias`
(`aliased_mutable_arguments`), `m08_generic_hides` (`type_mismatch`), `m07_lent_beside_at` (two), and
`w3_field_over_function`; defect 431's nine lines kept, the four 398/412/431 goldens changing one phrase, *"a function
value"* becoming the callee as written. Under `build --permissive` the route keeps `w5` `not_mutable`, exit 1 (the
critic), so the heap class stays closed in the control arm too.

**R3, a read-only function in a writers' table: `lent` fits where `@` is asked, and a table's type never depends on the
order its entries are written in.**
- **The fit (h)** (built since v2): a `lent` position where a type asks `@` fits, read as `@`; the call site writes `@`
  and is judged by `@`'s exclusivity (the critic's `s2b`: a reader in an `@` slot called `app.writers[1](@app,
  bump(@app))`, judged as `@`, prints `2`, `3`, `n 3`); nothing else fits: `@` where `lent` or a copy is asked, and
  `lent` where a copy is asked, stay `type_mismatch` (`m03`, `q5_lent_to_at`). Sound by the C type: a `lent` slot and an
  `@` slot of a program record are one `T *` (the warden's `w6`, `--emit-c` lines 112 and 113). **A no-state member is
  `_: App lent`** (`q5_fit_table`: `2`, `3`, `1`, `2`). Measured on the programs, the fit is what keeps one table:
  notes' page dispatch (`http/dispatch.hero:39-107`, read) has **21 arms: 13 write the state** (12 hand `@app`, `METRICS`
  writes `@app.registry` inline), **3 hand a copy of `app`, 3 read a field of it inline** (`STATIC`, `READY`, the site's
  routes) **and 2 take none** (`LOGOUT`, `HEALTH`), so under identity 8 of 21 would stand in a second table or answer
  the state back by value; `router_k`, `reach_web` and `mw` each mix writers and readers in one `@` table under the fit.
- **Adopted unbuilt, the shapes beside the literal's order** (the critic's item 2, widened by the synthesis's probes):
  where the elements of a list or a map, the branches of an `if` or the arms of a `match` are function values, they
  take **the type each of them fits, in any order**: a position where one writes `@` and another `lent` is `@`, a
  position one names and another leaves unnamed takes the name, and anything else is refused as today. The join is
  commutative, so `t6`, `t1` and `names_order_b` check in both orders, `branch_names` and `branch_modes` check, and a
  table of readers alone stays a `lent` table into which a writer is refused (`probe/push_order.hero`: `r @= [peek]`
  then `r @ r.push(bump)` is `type_mismatch` under the route, rightly, since `r[0](app, 1)` is called with no `@`).
  **Cases owed**: those five, each in both orders.
- **The names' fit, stated the right way round in the spec** (the critic's item 7): a function whose parameters share
  no type names none, so it fits a type naming them; a value whose type names its positions does not fit a type naming
  none (`w4`, `w5`; `n1`, `n2`). The join needs the sentence, which the warden's W1 did not write (its silence was on
  the lenient side; the join makes it a rule a reader must predict).
- **Adopted unbuilt, a write of a place to itself is no write** (`x @ x` not counting for `never_rebound`, the warden's
  +7): with the fit, no read-only member needs a self-write, so the one use left for `app @ app` is a signature that
  says `@` and writes nothing, the class `never_rebound` closed. **Census at `a60dfb77f`**: `perl` over the 9,590 tracked
  `.hero` files for a line `<place> @ <the same place>` finds **5** (`programs/synthesis/selfwrite-census.txt`: four
  `run` goldens testing a self-assignment's reference counts, `adversarial-str-self-assign`, `records-own-strings`,
  `adversarial-aggregate-overwrite`, `fixedbugs-field-path-store-leaked`, and Rebel's `json/jq/internal/regexes.hero:50`,
  an arm doing nothing), and each of the five places is written elsewhere in its file (`grep`), so the rule refuses
  none of them: read, not run by a compiler.

**R4, a mark in an `extern` callback's type is meaningful, and C reaches a marked function only through a thunk that
carries five duties** (adopted unbuilt; the ffi-pragmatist's five conditions and the critic's item 1, **the veto
lifting on them**). A landing that lets a program write a mark in a callback's type and does not carry all of them
refuses the mark in the checker instead, so **the route's unchecked thunk is never shipped**.
- **(a) Handles**: where an `@` or `lent` slot reaches a handle, the thunk marks it lent on entry
  (`hero_handle_lent(*p)`) and asks it alive after the call (`hero_handle_alive(*p, ...)`): without the first a correct
  program is refused with a false message (`reuse`: exit 134 on the three legs, `11` with the clear), without the second
  C writes into freed memory at exit 0 (`take_copy`, `4702111234474983745`, `heap-use-after-free` under `--sanitize`,
  three legs; the checked thunk's panic instead). This makes § 13's handle sentences true through a slot; it adds none.
- **(b) A null slot** aborts, naming the callback and the slot, where today's panic blames the program for C's null
  (`set_five`, rtree's null `pRes`: exit 134, *"Two things put a null where C read it ... This runtime raised neither"*,
  three legs).
- **(c) Overlap**: C handing places that overlap to two marked parameters, one of them `@`, aborts, naming the callback
  and both: § 9's exclusivity with C the caller, by each slot's address and size, so design.md §4.8 lines 1620 and 1621
  (*"`@` parameters never alias"*) hold for a callback too. The ffi-pragmatist named the narrower compare, two `@`
  slots of one type, or the invariant restated without callbacks; **the synthesis takes the wider check**, since a
  record and its own field, or an `@` beside a `lent` of its place, alias as surely (`twice`: `40040` where the model
  reads `4`).
- **(d) `const` for a `lent` slot**: a callback type's `lent` slot is C's pointer to const, in the typedef and the
  thunk, a group record's and a number's alike; and a function's own `lent` group record is `const T *` (route B), which
  § 13 already says of an `extern`'s parameter (*"a group's record that is no handle, declared `lent` without `@`, is a
  pointer to const"*). Measured by hand-edited emission compiled with the compiler's own words: variant B accepted,
  nftw walking the tree on the three legs; A, C and D refused at the probe. **A `lent` number in a callback is `const
  T *`**, one reading where the route wrote a third (a non-`const` pointer, which binds only C's `int64_t *`, the
  critic's `lent_num_mut` `42` and `lent_num_ptr` refused).
- **(e) Route F**: where a function's own slot and the type's differ by `const` (a `lent` group record or number bound
  where the type asks `@`, as nftw's `struct FTW *` and glibc's `struct dl_phdr_info *` ask a reader), C is handed a
  **twin** with the type's own signature calling the function, so a read-only callback over a non-`const` C pointer is
  written `lent`, with no self-write (variant F: accepted, both assertions holding, `--sanitize` exit 0, three legs).
- **(f) Every call C makes reaches the thunk**, an `extern` called through a function value included: its value hands
  each function argument through `hero_callback_of`, as the call by name does (the synthesis's finding above, drafted
  as a defect, since it stands on `main` without the form).
- What it unlocks, the ffi-pragmatist's census held to clang on three of twenty: nftw, `dl_iterate_phdr`'s fields,
  rtree's query info and geometry out-parameter; `sqlite3.h`'s API callback types binding 17 of 20 on this Mac (15
  today), 14 on Linux (12 today), `sqlite3_exec`'s row callback still Part 8's shim (`char **` has no spelling), 0 of the
  92 vtable types. **No binding in the tree needs it** (18 function-typed `extern` parameters, every one a thread body
  with no mark, `ground/callbacks.txt`), so its cost and its gain are the bindings a program will write.

**R5, a function of an `extern` group with an `@` parameter, or a group record `lent`, is no value, refused in true
words** (adopted unbuilt, the critic's item 4). Under the route, `extern_at_value.hero` (`bump_it(@p: i64)` bound to
`(function(@i64) -> i64)`) is `mutable_parameter_as_value` *"... no function type carries an `@` (§ 9)"*, and
`extern_lent_record_value.hero` (`sum_pt(p: Pt lent)`) is `lent_parameter_as_value` *"no function type carries `lent`
(§ 9)"*, on base, main and route: **false under R1**. The refusal stands and its words change: an `extern`'s `@` is C's
out-parameter and its `lent` C's pointer to const or a lend C does not keep (§ 13), and a parameter may carry
`counted_by`, `owned`, `acquires`, `consumes`, `transfers`, `retains` or `borrows` (`CParam`, spec § 13's grammar),
none of which a function type carries, so a call through such a value would skip what the call by name does; the
message says so and points to a function of the program calling it by name, which is a value. An `extern` with no mark
stays a value (`extern_plain_value.hero`: `42` on base, main and route); one whose `cstr` is `lent` stands as a value
today with the mark dropped from its type (`extern_lent_value.hero`: the value accepted, its call with `.cstr()`
refused `lend_needs_a_header`), which R5 leaves as it is. **The spec says the exception** (R9).

**R6, `spawn` and `spawn_sized` refuse a marked function by a code of their own** (built): `thread_body`, *"a thread
runs `f(x)` on a copy of `x`, and the `f` given has type `(function(@i64) -> i64)`, whose parameter is the caller's
place — threads share nothing but a mailbox"* (`q8_spawn`, the critic's `t8_spawn_sized`), the value's own refusal
having retired. The runtime hands a body a `HeroCross` copy (`runtime/parts/spawn.c:693`, the ffi-pragmatist), so an
`@` body would write a copy nobody reads; at the C door clang refuses an `@` body against `hero_thread_spawn_sized`
(`threads/thread_door.c`). **A marked function value sent through a mailbox is legal** (`q8_mailbox`: `11`): a function
value is code and carries no place.

**R7, the diagnostics, and the control arm** (built, but where marked). A value whose marks differ from the type asked:
`type_mismatch`, both types printed with their marks, no fix; **owed** (the compiler-engineer's § 6): a note naming the
position and the two marks, since `m08`'s message does not say the `@` is the difference. A missing `@` through a value:
`marker_mismatch`, the by-name words with the callee as written (*"`h` changes this argument, so the call site writes
`@` too"*), its fix `certain` where the place can take it (the type gives one reading, unlike
`aliased_mutable_arguments`' two, panel 110), applied by `check --apply --in-place` on `m04`, which then checks; an extra
one: *"`fs[0]` does not change this argument — the `@` says it does"*. `not_mutable`, `aliased_mutable_arguments`,
`lent_shape`, `thread_body` as above. **The sentences made false, owed in the landing's commit**:
`selfhost/data_errors.hero:180`, `:194` and `:220` (*"no function type carries"*, the last with no caller under the
route), R5's two messages, `ffi_callback_type`'s second note (*"a pointee the header marks `const` has no spelling here
unless it is `const char *`"*, `emit/ffi_narrowed.hero`), false the day `lent` spells `const T *`; `never_rebound`'s
first `guess` fix in a callback (`at: Ftw lent`), refused by clang at the base, becomes right under R3 and R4(e).
**The control arm** (`build --permissive`): the type decides, as the declaration does by name: a missing `@` hands the
place (`c1_control_missing`: `1`, `2`), an extra `@` *"says nothing: the value is passed"* (`m05_extra_at` = the
repair's `x1`: **`6`, `3`, exit 0, where the base and `main` exit 2**); a case per meaning through a value owed in
`tests/golden/control/`; whether the `control` and `permissive` forms move as wholes: unrun.

**R8, the emitted C, and the bytes of every program that does not use the form.** An `@` or `lent` position of a
value's typedef is `T *`, the pointer `signature.one_param` gives a function's own slot, so value and function agree
(`r6`; the ffi-pragmatist's hand-written `values/`: `9 9`, `14 9`, `--sanitize` clean, three legs). **Byte identity,
measured**: 248 of the 249 `run` goldens that write `(function(` (the compiler-engineer); `tests/golden/emit`, `ir` and
`fixedbugs` whole and every fifth mode-free `run` program, **248 of 248** (212 emitting C, 36 nothing under either);
**55 of 55** `examples/*/main.hero`; the compiler's own C, **40,972,079 bytes each, `cmp` silent** (the critic, the
compiler-engineer). **`fixedbugs-417`**: moved under the route, its cause found (§ found before judging) and **adopted
unbuilt into the landing: an `extern`'s signature interns no marked type** (the 8-line `declared_modes` change), which
R5 makes harmless since no `extern` with a mark stands as a value; unrun on the other 248 and the 55 under h417 (an
inference: the change touches `extern` signatures alone). **Owed beside it**: the forward `typedef struct h_X h_X;`
before a function typedef naming `h_X`, without which a state record holding its own handler directly
(`q2_field_callee`, `f4_field_first`) exits 2 on every build, a defect of the base (drafted). Route B's `const` (R4(d))
moves **3 `run` goldens** by the ffi-pragmatist's classification (`census/lent_group.py`), unrun as emission. The seed's
own C does not move until `selfhost/` writes the form, so no refresh is owed by the route.

**R9, the spec's sentence**, the warden's `spec-w1-fit` rewritten to say what R1 to R6 do
(`261-reports/synthesis-adopted.txt` and `.diff`, built by `programs/synthesis/spec/make_adopted.py`, every anchor
asserted once): § 3's `TypeArg`; § 5's *"an `@` argument counting and `x @ x` not"*; § 9's field first; § 9's values
bullet; § 11's `spawn`; § 13's callback. The values bullet, in place of line 321's two lines:

    - Top-level functions are values, typed with the marks their signature writes,
      and a call through a value marks its arguments as its type does:
      `xs.fold(0, add)`; `advance` above is a `(function(@Lex) -> ())`, called
      `f(@l)`, and `peek` a `(function(Lex lent) -> ())`, called `g(l)`. The marks
      are part of the type, but `lent` fits where a type asks `@`: `peek`, and
      `skip(_: Lex lent)`, are values of `advance`'s type too; and a function whose
      parameters share no type names none, so it fits a type naming them. Where the
      elements of a list or a map, the branches of an `if` or the arms of a `match`
      are such values, they take the type each of them fits, in any order. A
      function of an `extern` group is no value where a parameter is `@`, or a
      record `lent`: those marks are C's (section 13).

and § 13's callback sentence: *"A callback is a **parameter**, never a result or a signal handler, nor called by one;
its parameters follow the same rule, an `@` one the place C hands it and a `lent` one that place through a pointer to
const, a number's too, and a null place, or two that overlap where one is `@`, aborts, naming the callback; `()` is
`void`"*. **Its count: cl100k_base 10,245 to 10,555 (+310), claude-legacy 10,067 to 10,366 (+299), a lower bound,
vendored only** (`heroes measure` with the base's stage 2 from a tree of the base's `vendor/`, no `--refresh`, none
authorised); the pieces alone (`pieces.py`): `TypeArg` +13, `x @ x` +7, the field first +19, **the values bullet +206**
(the warden's +100 and three sentences), `spawn` +12, **the callback +53** (the warden's +5). **Real, about +388 to
+422, an inference** from the ledger's ratio of 1.25 to 1.36 the warden read: 13,827 to 13,861 against the 16,384
ceiling, the FFI floor's 60 inside. Past `DELTA_GATE`: the landing commit's body names the payment, the registered
predictions. **Run on this text**: `spec` **32 passed, 4 failed** (`budget`, `spendable`, `real`, `ledger`, the checks a
landing refreshes, as on every warden text), `grammar` **12 passed, 0 failed** (`programs/synthesis/spec/suite-*-adopted.txt`).
**Against the route, clause by clause** (the critic's § 5 table, carried to the adopted text):

| clause | the resolution | the route as built |
|---|---|---|
| § 3 `TypeArg` | R1 | parses both, refuses `@T lent` (`lent_shape`) and a name before the `@` |
| § 5 *"and `x @ x` not"* | R3 | `q5_self_write` `2`, `3`: still a write; **amended by R3** |
| § 9 the field first | panel 079's ruling, design.md §4.11 line 1949 | `f4` `20` as at the base; `w3` closes the marks' half |
| *"typed with the marks ... a call through a value marks its arguments as its type does"* | R1, R2 | `marker_mismatch`, `not_mutable` (`m04`, `m05`, `w1`, `w5`) |
| *"The marks are part of the type, but `lent` fits where a type asks `@`"* | R3 | three typedefs (`r6`), `type_mismatch` between them (`m03`), the fit by binding, `bind` and a written table |
| *"a function whose parameters share no type names none, so it fits a type naming them"* | the base's fit, stated | the base's (`n1`, `n2`) |
| *"Where the elements ... in any order"* | R3, unbuilt | the first element's type: `t6`, `t1`, `names_order_b`; branches refuse both orders; **amended by R3** |
| *"A function of an `extern` group is no value where ..."* | R5 | refused, with a false message; **amended by R5** |
| § 11 *"`f` taking `x` with neither mark"* | R6 | `thread_body` |
| § 13 *"an `@` one the place C hands it ... aborts, naming the callback"* | R4, unbuilt | accepted and built with the base's thunk: `40040`, C writing freed memory at exit 0, nftw refused (non-`const` `lent` slot); **amended by R4** |

**design.md moves with it** (its words are the landing's): §4.13 lines 2072 to 2078, §4.8 line 1559 (a function with
`lent` used as a value among `lent`'s refusals) and lines 1620 and 1621 (true at the boundary by R4(c)), Part 8 item 19
lines 4606 to 4608 (the fifth cause's code, `expected_type` at the base, not `expected_function_type_params_close`) and
4634 to 4640 (*"a qualifier field on the function type"*, made a fact by R1's mark run), and its counts, restated by
the ffi-pragmatist's census (112 callback types, 20 of them API parameters; Part 8's 106 and 153 not reproduced, the
method that produced them not in the record).

**R10, the tools that re-print a program.** Built and run: `heroes fmt` (`print/types.hero`, +5: 39 files, 0 not
canonical), `--dump-ast` and `heroes doc` (*"`function walk(@b: Builder, it: Item, block: (function(@Builder, [Item])
-> ()))`"*). `heroes grammar` prints the spec's production and moves with R9 (`grammar` 12 passed). **Owed, unrun**: the
formatter's self-check handed a pair that drops a mark, `heroes probe`'s reader, `heroes mutate`, the editor's grammar,
the site's highlighter, and `rebel/generators/rebel_index/`, whose reader of `heroes doc` broke once on a generic
signature (defect 2761) and will read marked faces.

**R11, the router and the middleware chain in `web`**, the author's addition (its own heading below): route (k), the
answers bound by name in the program's own table, and the faces of sketch (B), generic functions over that table, with
the chain's faces beside them; nothing of the program's held by the library, no generic record, no closure.

**R12, what the landing owes the tree** (CLAUDE.md § 2: nothing kept for an older spelling). The compiler: R1, R2, R6
as built (**at least 450 code lines in at least 25 modules**, the compiler-engineer's prediction, and its **seven
ceilings**, each a cut or a decided row), and, unbuilt, R3's join and self-write rule, R4's five duties and (f), R5's
words, R7's note, R8's `declared_modes` change and forward typedef; each with its cases: `w1`, `w4`, `w5` (a `run`
case), `m06`, `m08`, `m04`, `m01`, `q5_fit_table`, `r6`, the five order cases of R3 in both orders, the self-write,
R4's shapes as `run` cases on the three legs (`take_copy`, `reuse`, `pass_back`, `twice`, the null, nftw with route F,
`dl_iterate_phdr`, rtree's geometry callback), the `extern` refusals of R5, `thread_body` twice, the control arm's two
meanings through a value, `router_k` and `mw`. The goldens: the five `check` goldens moved (the first and the last
rewritten, their programs' meaning changed), the census of the **73** `tests/golden` files naming `marker_mismatch`, the
`control`, `permissive` and `full` forms, then the whole net, the net's own tests, Rebel's pipeline and the latency
instrument's `compare`. The sentences of R7 and design.md's of R9; Rebel's own (`web.hero:21`, `:585`, `:1362`;
`interpreter/run/value.hero:11`; `notes/app/state.hero:1-5` and `registry/server/serving.hero:5-8` resting on `web`'s
opening comment; the warden's § 4.5). **And the rewrites the author asked for**: `web`'s router faces and chain (R11),
and **notes, auth, registry, searchd, chat, reach, gateway and agent** rewritten on them, each built and run with its
`main.expected` where it has one: notes' 21 page arms and its two API answers (`api/notes_api.hero:73`,
`api/accounts_api.hero:71`), auth's 21 arms, registry's 14 and its per-route scope and limiter (route data beside the
handler: a step of the route's own, R11), searchd's 10, chat's split dispatch with its two places (`open_socket(@srv,
@reader, ...)`: the handler's second place as an argument of its own), reach's (rewritten by the compiler-engineer,
checks under the route), gateway's stand-ins' hand-matched paths (its own `match routed.route.kind` over a closed
`variant Kind` stays, (e)), agent's toolbox (rewritten by the compiler-engineer, 178 to 195 lines, its output
`cmp`-equal to `main.expected`). **On Linux every one of them that reaches Rebel's `math` is refused before the form
is written** (§ Found beside), so the landing's Linux legs wait on that defect.

## The author's addition: middleware as Echo and Go's own library write it, with no closure

**The shape asked for**: a step every route runs (Echo's `e.Use`), a group's (a path prefix, `g.Use`) and one route's
own; a step that answers before the handler (auth refusing with 401) and one that wraps it (a logger reading the
response and the time); the order they run in; and the program's state reached through `@`. Echo writes a middleware as
`func(next echo.HandlerFunc) echo.HandlerFunc`, a function returning a new function that holds `next` (the historian
verified Echo's `HandlerFunc`, `type HandlerFunc func(c Context) error`, v4.13.4, and not its middleware type, which no
seat read: **unverified by this sitting**).

**Built, `programs/synthesis/mw/`** (`main.hero`, 176 lines; `chain.hero`, the faces as `web` would hold them, 64
lines; both canonical by the route's `fmt`). **The `next` is data**: `chain.Next` holds the route found, the names of
the steps its chain runs in order, and the index of the next; a step continues by calling `chain.proceed`, a generic
function (`proceed<S>(@state: S, steps: {str: (function(@S, Next, http.Request) -> http.Response?)}, answers: {str:
(function(@S, web.Found, http.Request) -> http.Response?)}, n: Next, r: http.Request) -> http.Response?`), which calls
the step `n` names through a value, `@state` lent all the way down, or past the last the route's answer. **The router
keeps names, never a function**: `chain.Steps` holds every route's steps, each group's by the group's name (`web.Found`
already carries `groups`, outermost first), and each route's own by its name; the program binds each step's name to its
function in its own table, as route (k) binds each answer. The program's one line that continues a chain,
`go_on(@app, n, r)`, reads its two tables by value from the place it lends on (`chain.proceed(@app, steps:
app.step_of, answers: app.answer_of, n: n, r: r)`), which § 9's exclusivity allows (no other `@` argument reaches the
place). `logger` (every route) marks the time, calls on, reads the status and the elapsed time and appends a line to
`app.log`; `auth` (the group `admin`, prefix `/admin`) answers **401** without calling on unless the request carries
`Authorization: Bearer letmein`, counting the refusal in `app.refused`; `count` (the route `save`'s own) counts the
writes around the answer. The answers mix a writer (`save_note`, `@`) and readers (`home`, `note_page`, `stats`,
`lent`) in one `@` table by R3's fit. Six requests made by `http.request`, no network.

**Run**: under the route compiler (`sources 7cc4a75a5a7c6bf1`), `check` exit 0, `build` exit 0, the binary three times,
exit 0 each, `cmp`-equal; under **`heroes-261-v4`** by its absolute path (`sources 1283d3066f568534`), `check` 0,
`build` 0, **three runs each `cmp`-equal to `main.expected`** (`mw/v4.log`); under the base, `expected_type` at the
first `@` in a type. `main.expected`:

    GET / -> 200 home, notes 0 | logger> <logger
    POST /notes/a -> 201 stored a | logger> count> save <count <logger
    GET /notes/a -> 200 alpha | logger> <logger
    GET /admin/stats -> 401  | logger> auth> <auth <logger
    GET /admin/stats -> 200 notes 1 writes 1 refused 1 | logger> auth> <auth <logger
    GET /nowhere -> not_found
    GET / 200 timed true
    POST /notes/a 201 timed true
    GET /notes/a 200 timed true
    GET /admin/stats 401 timed true
    GET /admin/stats 200 timed true

So **the order**: every route's step first, then each group's, outermost first, then the route's own, then the
answer, each step's after-part in the reverse order (`logger> count> save <count <logger`); **auth answers before the
handler** (`401`, the trace never reaching `stats`, `refused 1` kept in the state); **the logger wraps** (it reads the
`401` and the `200` and the elapsed time, `timed true` being `nanoseconds >= 0`, the clock's reading itself not
printed so the output is fixed); **the state is reached through `@`** in every step and the writer, and read `lent` by
the readers (`stats` prints `notes 1 writes 1 refused 1`). A route nobody binds (`/nowhere`) is the router's
`not_found` before any chain starts. **Unrun**: Linux (the program reaches `math` through `web` and would meet the
`nextafterf` defect there, an inference); a step that is itself generic; a chain whose step changes which route
answers.

**The shapes that would need a form the language lacks, each ruled:**

| shape | the form | the resolution | measured |
|---|---|---|---|
| Echo's `func(next HandlerFunc) HandlerFunc`, a step returning a new function that holds `next` | a closure | **refused, routed around**: design.md Part 6 line 3644 (panel 119), and the ffi-pragmatist objects at the boundary (a capturing value is no longer the eight bytes of a C function pointer, `emit/ctype.hero`'s `function_ty` arm); the `next` passed as data does what it did | `probe/echo_style.hero.txt`, a function declared inside a function: `expected_expression` at the inner `function`, base and route; `mw/` runs without one |
| `web.Router<S>` holding `[(function(@S, ...))]`, the author's sketch (C) | a generic record | **not adopted, routed around**: the router keeps names, the program's record holds the functions, the library's faces are generic functions; panel 252's R3 (*"waits"*) stands, with no measured need against it | `router_k`, `router_b` (the compiler-engineer) and `mw` run with none; its cost carried, *"288 incomplete matches in 109 files"* (`docs/panel/252-*.md:567-570`) |
| a function returning a function | none lacking: a function may return a top-level function value | **available**; what Echo's needs is a function returning one that holds its argument, which is the closure above | `probe/return_function.hero` (`pick(true)(3)`, `pick(false)(3)`: `9 6`, base); the critic's `t2_call_result` (`first(app)(@app, 3)`: `5`) and `s3` (a marked function answered by a call), route |
| `web.use`, Echo's name | `use` is a keyword | **routed around**: the face takes another name (`add_step` in the sketch) | `probe/use_name.hero.txt`: `expected_name` |

**The faces the landing owes in `web`** (sketched, unbuilt in the library; built as `mw/chain.hero` in a module of the
program): `record Next` (the route found, the chain's names, the next index); `add_step(@router, step:)`, the same
face on a router `new_router` made (every route's) and on one `new_group` made (that group's), the group's steps kept
on the group router and travelling with `mount` as its routes do; a route's own steps, a field of `Route` or
`add_route_step(@router, route:, step:)`; `web.Found` gaining the chain's names in order; and the generic
`proceed<S>(@state: S, steps: ..., answers: ..., n: Next, r: http.Request) -> http.Response?`, with sketch (B)'s
`on<S>` and `dispatch<S>`. **Cost**: `web` stands at 88 public functions against the `rebel` suite's fence of 90
(`tests/harness/rebel_faces.hero:45-46`): (B)'s two and the chain's three or four pass it, so the chain is a face of its
own (`web/chain`, as decision 2802 split `web/proxy`) or a decision on the fence; unrun against the `rebel` suite. And
**feature 2658's four by-value steps** (`before_with<S>`, `after_with<S>`, `require_basic_with<S>`,
`require_bearer_with<S>`, `web.hero:626` to `:740`) and `limit_rate`, *"a step that changes state ... stays a call the
program makes"*, are the chain's to absorb, since a step may now change the state through `@`.

**Listed among the landing's debts (R12): the router's middleware chain and its use in the examples**: `web`'s faces
above, each of the eight examples the author named using the chain where it filters or wraps today (auth's and notes'
sign-in checks, registry's scope per writing route and its rate limiter, every server's request log), and the cases:
`mw` as a `run` case with its `main.expected`, a group's step travelling through `mount`, a step answering before the
handler, a wrapping step reading the response.

## Not adopted

- **Identity alone, no fit** (`spec-w2-identity`, +117 vendored): every read-only member of a writers' table is a
  compile error (`u1`, `q5_uniform_table`: `never_rebound` twice), and the ways out are a self-write (which R3 now
  refuses), a second table (`q5_two_tables`: `2`, `3`, `1`), or the state answered back by value; 8 of notes' 21 page
  arms would leave its table. On the C side, all three real bindings hand a non-`const` pointer that a read-only
  callback only reads (nftw's `struct FTW *`, glibc's `struct dl_phdr_info *`, `sqlite3_rtree_dbl *`), which identity
  binds only with `@` and a self-write. The warden predicts D3 failing under it (4 of 10, 1 to 7). Recorded as the
  conservative side of R3, below.
- **`never_rebound` waived for a function used as a value** (`spec-w3-waiver`, +126): whether a signature's `@` must
  write would be decided by a use elsewhere in the program, design.md §1.3's refused category (the warden, lines 213
  and 214), and a signature saying `@` and writing nothing becomes legal again (the compiler-engineer).
- **(m), a `lent` function where a by-value type is asked**: refused by the route (`q5_lent_to_at`), the value's C slot
  `T *` and the type's `T`. Swift 6.4.2 admits that direction (`borrowing` where a plain type is asked, `m_dir.swift`,
  `4`, the critic) and refuses (h); its twin captures nothing (the critic), so it is no closure, and nobody built it.
  Not adopted for want of a built route and a measured need; a sitting of its own if a program asks.
- **The fit read only at naming** (the warden's condition): the route applies it in `fits` and `bind` too (`s3`,
  `router_b`).
- **Sketch (A), the handlers indexed by the router's own index**: a silent misroute, measured (`router_a`: `/g/a -> b`,
  `/b -> a`, exit 0), since `mount` orders the router's routes and the program pushes its handlers in another order.
- **Sketch (C), `Router<S>`; (a), a `handler` record kind**: no need measured (R11), and (a) a core construct with no
  work of its own (Part 5, the compiler-engineer).
- **The resolver asking both rules (v3)**: loses 7 of defect 431's 9 pinned messages (R2).
- **The route exactly as built**: short at the C boundary (R4), at the literal's order (R3), at the `extern`'s message
  (R5) and at `fixedbugs-417`'s bytes (R8).
- **An `extern` with a mark made a value through a twin**: its C words (`counted_by`, `owned`, `acquires`, `consumes`,
  `transfers`, `retains`, `borrows`) have no place in a function type, so the twin would carry a type that drops what
  the call by name does; unbuilt, and R5's refusal costs one function of the program.
- **The ffi-pragmatist's narrower overlap check** (two `@` slots of one type) and **§4.8 restated without callbacks**:
  R4(c) takes the check that makes the invariant true.

## Disagreements, stated plainly

1. **The C boundary.** The compiler-engineer: *"an `extern` keeps both refusals"*, a mark in a callback's type *"the
   ffi seat's"*; its route builds one with the base's thunk. The ffi-pragmatist vetoes exactly that landing; the critic
   measured it on three platforms. **R4 adopts the ffi-pragmatist's conditions, widened at (c), with (f) added**; the
   compiler-engineer's built route is not the landing at the boundary.
2. **The fit (h) against the precedents.** Swift, Rust and Nim refuse it, measured; C# 12 admits it with a warning
   (the historian, the proposal's page). The synthesis adopts it, robust and complete, on the measurement that those
   three accept a writer that only reads and Heroes does not (`never_rebound`), so their refusal costs them nothing and
   Heroes' would cost a second table or a self-write. **Conservative** would be identity (W2) with two tables.
3. **Which way the names' fit runs.** The brief (the critic's first reading, the repair) said the reverse of the
   compiler; the warden found it and the critic withdrew it. R3 and R9 state the compiler's direction. Feature 1133's
   sentence is false at the base, owed a correction underneath (the coordinator's).
4. **(m).** Refused (the compiler-engineer), no objection (the ffi-pragmatist), naming only (the warden); Swift admits
   it; nobody built it. Not adopted (above).
5. **W1 against the resolution.** The warden approved W1; the critic found it false against the route at § 13 and at
   the `extern`'s value, and blind to the literal's order. The adopted text is W1 rewritten (R9); W1 is the rival.
6. **`needed_for_self_hosting`.** Both seats said *no* on a rule file's sentence; the critic built the ring. Agreed,
   now measured.
7. **The spec's size.** Under §1.2 alone the warden calls W1's addition *"a problem I state rather than solve"*; the
   adopted text is twice W1's (+310 against +149, vendored). Robustness (rank 3) and the author's amendment of
   2026-11-22 admit it; no warden veto holds against the most robust resolution, and none was cast.
8. **The registration's arms.** The coordinator's brief to the synthesis named two arms, `adopted` and `rival`;
   `llm-ergonomist.md` registers `now` and `adopted` and prices a third column past the ceiling. The synthesis kept the
   registered design and registered the rival's text and hash, not run (`registration.md` § What the synthesis added).

## What a veto would compel

**No veto is exercised against the resolution.** The compiler-engineer's soundness veto compels the checker's two rules
through a value with `w1`, `w4`, `w5` and `m06` refused: **R2, built**. The ffi-pragmatist's conditional veto compels
that C never call a marked function through the base's thunk: **R4, adopted unbuilt, and until its five duties and (f)
land with their cases on three legs, the checker refuses a mark in an `extern` callback's type**, so the vetoed landing
cannot ship. The spec-warden holds no veto against the most robust resolution and reports its cost (+310 vendored).

## The conservative resolution

**Identity, no fit, and no mark at the C boundary** (the warden's W2 with W1's § 13): every marked function a value,
a read-only member of a writers' table refused (`never_rebound`) and written into a second table or answered by value;
a table literal whose entries differ in a mark refused in both orders (as an `if`'s branches are today), its type
written; a mark in an `extern` callback's type refused in the checker, which costs no binding in the tree (18
function-typed `extern` parameters, all thread bodies with no mark) and forgoes nftw, `dl_iterate_phdr`'s fields and
rtree's out-parameter (sqlite3's API callback types binding 15 of 20 on this Mac, 12 on Linux, against 17 and 14); an
`extern` with a mark refused in true words as R5 does. **Robustness takes R3 and R4 instead**: identity turns a
read-only handler into a self-write or a second table, and a C binding over a non-`const` pointer into a hand-written C
shim, the one place left where nothing checks the C; R4 moves that C into a thunk the compiler writes and checks.

## The critic's list of what must not be asserted, held line by line

From `critic-reports.md` § 9 and § What the synthesis must settle:

- *"The route refuses every call that hides an `@`"*: asserted from Heroes only; from C, the route does not (R4 adopts
  the repair, unbuilt).
- *"An `extern` keeps both refusals"*: not asserted of a callback parameter; asserted of an `extern` function as a value
  (R5), with the message's falsehood named.
- *"W1 says what the route does"*: not asserted; the table in R9 names every clause where the route and the text part.
- *The names' fit as the brief stated it*: not asserted; stated the compiler's way round, with its source.
- *"`needed_for_self_hosting: no` rests on a ring built"*: asserted, on the critic's ring at the base, not on the
  compiler's own knots, which stay uncut.
- *"The fence holds at exactly 90"*: not asserted; arithmetic on 88, and the chain's faces pass it (R11), unrun against
  the `rebel` suite.
- *The `control`, `permissive`, `check`, `run` and `emission` forms under the route*: not asserted; unrun by every seat,
  the critic and the synthesis.
- *The ffi-pragmatist's "3 `run` goldens"*: asserted as a classification, unrun as emission.
- *A real count*: none; every count here is vendored, a lower bound.
- *"A mode-free program keeps its bytes"*: asserted with its exception named and its cause found (R8), the fix unrun
  over the other 248 and 55.
- *"Route (k) runs on Linux"*: not asserted; `router_k` is refused on both Linux legs by a defect of the base.
- *"The fit has precedent"*: asserted as C# 12's alone, with its warning, beside three measured refusals.
- *A prior-art line from the historian's commentary*: the four lines the critic named (the forum summary, axum's *every
  example* and *So*, ISO 7185's item b)) are left out of the decision's `## Prior art`; and Rust is cited on question 2's
  order from its Reference, which the historian's page did not say and the critic's local copy does.
- *The blind run's spend or a D-rule outcome*: not asserted; registered only.

## Found beside, for the coordinator

- **An `extern` taking a callback, called through a value, skips the thunk** (`blocking`, the synthesis): a correct
  program refused at exit 134 with a false message on base, `main` and route. Draft:
  `.claude/worktrees/rebel-scratch/coord/issues/d-p261-callback-value.md`.
- **`x1`, `build --permissive` exit 2** (`blocking`, the repair's probe): re-run on `a60dfb77f` by the synthesis, the
  same exit and clang line. R2's lowering closes it at the landing (`m05`). Draft: `d-p261-x1.md`.
- **The typedef cycle, `base_self_field`** (`blocking`, the compiler-engineer): `check` 0, `build` exit 2, re-run on
  `a60dfb77f`, the same. Draft: `d-p261-self-field.md`.
- **Rebel's `math` refused on Linux, `ffi_unpaired_release` on `nextafterf`** (`blocking`, the critic's `leg/baseleg.sh`,
  Linux arm64 at the base): **on `a60dfb77f` unrun** (a container, not seconds); no commit since the base touches the
  modules that raise it or Rebel's `math` (`git diff --stat`), an inference that it still happens; the Rebel pipeline's
  failed run at `4a5e585a9` names no `nextafterf` (`grep -c` 0 in its failed logs), so whether the CI meets it is a
  question. Draft: `d-p261-math-linux.md`.
- **`sqlite3_update_hook`'s `long long` on Linux** (`blocking`, the ffi-pragmatist): **on `a60dfb77f` unrun**; no
  commit since the base touches `selfhost/emit`. Draft: `d-p261-long-long.md`.
- **The literal's and the branches' order, for names, at the base** (`names_order_b`, `branch_names`, base and `main`):
  closed by R3's join at the landing; unfiled, the coordinator's to file or to carry as the landing's case.
- **Feature 1133's sentence** (*"an expected type carrying no names accepts a value whose type does"*) is false at the
  base: a correction underneath the closed issue is owed (the critic), the coordinator's.
- **The base's own tests read `1 failed`** (*"the list holds every word the runtime's headers write"*): repaired on
  `main` by `1ac3f57f1` (the critic).
- **A file outside the repository's root**: the historian reports its fetch tool kept Wirth's Oberon-07 PDF in the
  tool's own results folder; the path is unread by every seat. The author is owed that sentence (CLAUDE.md § Hard
  stops). The critic's own slip, `/tmp/critic-unused`, was removed at once (its § 9).
- **Seat programs that do not parse under the trunk's compiler**: the marked programs under `programs/` are `.hero`
  files the commit hook (`.claude/hooks/staged.py`) refuses to stage, the form not being in the trunk's compiler; they
  travel as the sitting's evidence archive or as `.hero.txt`, the coordinator's choice.

## What is open and unrun

- **The landing itself**: R3's join and self-write rule, R4's duties and (f), R5's words, R7's note, R8's two changes,
  R11's faces in `web`, the eight examples rewritten, and every case and golden of R12.
- **No harness form** ran under the route but the five `check` goldens' `check --brief`; the 73 `marker_mismatch` files
  unwalked.
- **Linux and Windows for `mw`**; Windows for every cell (no box); nftw and `dl_iterate_phdr` do not exist there.
- **The real count** of the adopted text and of every warden text (`--refresh` unauthorised).
- **The dangling run** of an `@` place C owns, kept by a third library past the callback (the ffi-pragmatist's
  `keeps_body`): today's class, wider reach, unrun.
- **A large group record through a value with a mark** (panel 236's twin) and **a self-call through a value with a
  mark** (panel 199): unbuilt.
- **(n)**, the state lent and the changes answered as data: built by nobody.
- **The latency instrument's `compare`** of the route against the base: unrun; every build here ran at load 6 to 40.
- **The blind reading**: registered; its `adopted` grades wait for the landed compiler (`registration.md`).

## Predictions to score

The seats' (the verdicts' table), at the landing in M-rebel-library: the compiler-engineer's **at least 450 code lines
in at least 25 modules**; the ffi-pragmatist's nftw with no shim and no self-write on three legs, `sqlite3_exec`'s shim
kept, 17 of 20 (14 on Linux) and 0 of 92; the warden's D1, D2, D3, D4 and spend for W1, read for both arms by
`registration.md` § What the synthesis added, and its W1 real count +186 to +203. **The synthesis's own**: the adopted
text reads **13,827 to 13,861** real on `measure --refresh` (13,439 plus the inferred +388 to +422); **R3 and R4 are
withdrawn** by one correct program among `mw`, `router_k`, `router_b`, `q5_fit_table`, the five order cases in both
orders, nftw with route F, `dl_iterate_phdr` with a `lent` reader, and rtree's geometry callback, refused by the landed
compiler on this Mac or in Linux arm64 (`math` repaired), **or by one program the landed build accepts whose run
writes freed memory, aborts with a false message, or hands one place to two `@` parameters** among R4's shapes, on any
of the three legs (`--sanitize` where it reads memory).

## The blind reading, run 2026-11-26

Added the same afternoon, after this sitting's commit (`e74a3ed7f`): the registered reading ran on the author's
authorisation (*ok a 20 dollars*), its report `261-reports/llm-ergonomist-blind.md`, the sessions' answers
`261-reports/llm-ergonomist-blind-answers.tar.xz` (text only), the runner as it was adapted and as it ran, with its two
repairs after sessions had run, `261-reports/registration-runner-as-run.md`. **31 sessions, all counted, 8.78 USD of
the 20.00** (8.771908 by the 31 sessions' own JSON and 0.003128 recorded by one session cut mid-run and rerun; the
runner's own count, which takes that cut session's empty file at 0.50, is 9.27); no session voided, every one `success`,
`is_error` false, `completed`. **By the registration's own rules the resolution stands as adopted**: D1 holds (`adopted`:
t1 5 of 5, t2 5 of 5, and t3 5 of 5), D2 (no program wrong under either arm, 0 against 0), D3 (0 of the 10 `adopted` t2
and t3 programs refused for a mode, every read-only function in an `@` table written `lent`), D4 (0 lost updates under
each arm, and no program anywhere writes a place to itself), and D5's counts (15 choice points about a mode under `now`,
6 citing one section; 29 under `adopted`, 7 citing one section). **What it measures is the form each reader chose, not a
difference in correctness, which none of the 30 got wrong**: all 15 `now` readers passed the state by value and answered
the new state, all 15 `adopted` readers wrote `@` for changing functions and `lent` for read-only ones. The warden's
directions and ranges hold, the point predictions low because the warden expected readers to fail. **What it found that
it was not built to measure**: the base compiler already runs `route.handler(...)` as a field call (design.md §4.11),
which spec § 9's text reads as `f(x, y)`; all 5 `now` readers of t1 avoided that call on that reading, and under
`adopted` 3 of 5 wrote it directly: the field-first sentence the spec-warden priced at +19 tokens is owed whatever is
adopted. **The run's own limits, in its report**: the `adopted` judge is the route compiler (`sources
7cc4a75a5a7c6bf1`, with the seat's `heroes-261-v4` beside it), no landed build existing yet, and the landed compiler's
answer on the 15 `adopted` programs is owed; the verdict session told the arms apart from the marks in the programs, so
the label blinding failed at that session, and the scorer read the key before grading; the 14:06 restart cut one
session four seconds in, its rerun counted and its set-aside file's spend kept in the ceiling's sum.

## Author's verdict

**Pending.** The decision's draft is `docs/panel/261-reports/decision-draft.md`, filed with this sitting as `panel 261`.

**Ratified by the author's reading, 2026-11-26, at about 14:06**, in conversation (the answer *1a* to the
coordinator's recommendation, meant as: *ratify R1 to R12 as adopted, the fit included*; decision issue 2827).
Recorded with it, so the reading is of what was adopted: R3's fit, a `lent` function where `@` is asked, which Swift,
Rust and Nim refuse on the author's Mac and the historian found only in C# 12, with a warning; the alternative the
author may still turn is the conservative resolution above, identity with no fit. A mark in an `extern` callback's
type stays refused by the checker until the thunk carries the ffi-pragmatist's five duties (R4). The sitting's blind
reading, authorised up to 20.00 USD, runs on the landed route and is scored beneath with its date. The landing is
lane land261, then the router and the middleware chain (R11, R12) and the rewrite of the examples; the author asked
for web middleware before the milestone's tag, which is where R11 and R12 stand.
