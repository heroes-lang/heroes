# Panel 261, the shared brief: a function type carries its parameters' modes

**A full panel** (`.claude/skills/panel/SKILL.md` § Two lanes): `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
`historian`, and `llm-ergonomist` **registered** (its paid run **authorised by the author on 2026-11-26**, their words
*ok a 20 dollars*, a ceiling of **20.00 USD** for the whole run of 31 sessions, and **started by the coordinator after the
synthesis**: `llm-ergonomist.md`), and the completeness critic, who read these briefs FIRST, before any seat starts
(§ 3c). The change is to surface syntax, to what the checker does at a call, to a diagnostic class and to the spec's
§ 3 and § 9 (CLAUDE.md § 4), so the soundness lane would lose half of it.

**Repaired before any seat starts**, 2026-11-26 from 09:44, from the critic's first reading
(`docs/panel/261-reports/critic-briefs.md`, its nine repairs and its § 2 and § 3), and **moved onto a new base** (below):
every number, line and probe the move could change was re-run at the new base by the repair, with a compiler it built
from the seed in `.claude/worktrees/panel261-repair-scratch/` (§ 8). A sentence the repair changed says so where the
change matters; the critic's findings are credited to the critic.

**The author's instruction, 2026-11-26** (relayed by the coordinator; meant as: *I want the language change now, in the
most robust and complete way, because as it is it is too limited; study a solution that stands, then make every change
to Rebel's examples and library*). Under CLAUDE.md § Precedence as the author amended it on 2026-11-22 (*"the most
robust solution now always wins, a new form included and at the cost of the spec's size: Principle 0, the spec's budget
(design.md §1.6) and the spec-warden's vetoes yield to it"*), **the budget and Principle 0 are costs to be measured and
written, never a veto**; a seat's soundness veto stands as ever. **The coordinator's addition at 00:59**: the driving
programs of the sitting are **every** Rebel example that serves HTTP, with the router or without it, gateway first
(§ 5).

**The base is `main` at `df92f9281`** (`df92f9281a89c83252a076b0dffed2ee2b32dec9`, 2026-11-26 09:40:59 +0100, the
merge of lane-land260-split that closes panel 260's landing, steps 197a to 197e; `git log -1 --format='%H %ci %s'
df92f9281`, the repair, 09:45), unless the coordinator's prompt names another. **The clerk wrote these briefs at the first
base `07e747bc6`** (`07e747bc66bcd49add47b2ba8011194969df61f2`, 2026-11-26 00:52:57 +0100), and the repair moved the
worktree `.claude/worktrees/panel261` onto `df92f9281` with `git checkout --detach` (the briefs are untracked and
travelled with it). **What the move changed, by `git diff --stat 07e747bc6 df92f9281`**: 184 files; the spec by two
lines replaced in § 13 (the platform layer's sentence and the `Extern` production gaining `[ "layer" ]`), its line
count unchanged, 516, so **every spec line cited below stands where it stood**; `docs/design.md` by four lines added at
its line 501, so **every design.md line from 501 on moved by +4** and is cited below as it now reads; 85 files of
`selfhost/`, `runtime/` and `rebel/library/`, of which `selfhost/ast.hero`, `print/fmt.hero` and `print/dump.hero` are
the ones this brief cites (re-counted in § 4); `rebel/examples/`, `rebel/generators/`, `web.hero` and `http.hero`
unchanged (`git diff --stat` of each, empty), so § 5's counts stand, and the census re-run reads byte for byte the
clerk's (`diff`). **`main` has moved since** to `c6f3184e5` (lane-abort-trace's step 196a and lane-land260-split's
step 197f: five files of `selfhost/cli/` and `selfhost/parse/group_head.hero`, none cited here; `git diff --stat
df92f9281 main -- spec docs/design.md rebel selfhost runtime`, 10:13); the base stays `df92f9281` unless panel
259's landing merges, which the coordinator's brief to the repair named as the one reason to move again.

The repair's compilers and probes are in `.claude/worktrees/panel261-repair-scratch/` (`tree/` is `git archive
df92f9281` whole, extracted there; `build.log`; `facts/` the commands' outputs; `probe/`, `critic-probes/`, `refs/`).
**Every number, path, line and count below was produced by the command named beside it, run at the base named**: by
the clerk at `07e747bc6` from 00:57 where nothing the move touched could change it, or by the repair at `df92f9281`
from 09:45 where it could, or is marked *carried from <file>* where nobody re-ran it, or *unrun*. The machine read a load
of 93 to 150 the whole clerk's night and 11.9 to 17.5 during the repair (`uptime`), so **no duration in this brief is a
measurement** (CLAUDE.md § Verification: the ratio). Scripts and outputs are in `ground/`.

**No seat or critic starts a paid run, a `claude -p` session, an API call or a cloud run its brief does not name; none
prints a credential** (`gh auth status` masks it, `gh auth token` never); `gh` reads only; no file outside the
repository's root (no `/tmp`); no `pkill`; no commit, push or merge. Each seat works in its own detached worktree,
`.claude/worktrees/261-<seat>` (made by the coordinator), builds its own compiler there from the seed and stage 2
(§ 8), and never reads, builds or runs inside another seat's directory.

## 1. The proposal, in ten lines

1. **A function type carries each parameter's mode**: spec § 3's `TypeArg` admits `@` before a type and `lent` after
   it, so `(function(@App, http.Request) -> http.Response?)` and `(function(Config lent) -> i64)` are types, and a
   mode is part of the type's identity: `(function(@App) -> ())`, `(function(App lent) -> ())` and `(function(App) ->
   ())` are three types. **The precedent is a parameter's name, and it is identity plus two relations, not identity
   alone** (the critic's § 2 item 3, re-read by the repair): § 9 lines 305 to 309 make the names part of the type; a
   **one-way fit** lets an expected type that carries no run of names accept a value whose type has one
   (`selfhost/check/type_fit.hero:13-36`, `fits` at 37: *"It is a widening, and the word is used on purpose"*; feature
   1133, `gh issue view 1133`: *"an expected type carrying none accepting a value whose type does"*); and **a generic
   path compares the names on its own** (`selfhost/check/generics.hero:118`, `bind`'s call of `named`, defined at :281; design.md
   §4.9 lines 1761 to 1763, `error[callback_name]`). The spec states the identity and **not the fit** (`grep -n` of
   *carrying no*, *names nothing*, *carries none* in the spec: nothing). Whether a mode has a fit of its own (a `lent`
   function where an `@` type is asked, question 5) and a generic check of its own (question 1) is the sitting's.
2. **A top-level function with `@` or `lent` parameters is a value** of the type its signature writes; § 9 line 321
   loses *"but not one with an `@` or `lent` parameter"*.
3. **A call through a value writes the modes as a call by name does**: `@` before each `@` argument, nothing before a
   `lent` one, `handler(@app, r)`, under the exclusivity § 9 already states (lines 314 to 319): no other `@` argument
   of the call may reach the place, nor, for `lent`, one nested in another argument.
4. **The sitting decides** what a mode means, or is refused, in an `extern` callback's type (§ 13), in `spawn(f, x)`
   (§ 11), and a mode mismatch's diagnostic and fix; and Rebel's router binds a route to its handler,
   `web.get(@r, "/notes/:slug", note_page)` with `note_page` taking `@app` (§ 6).

## 2. The questions the seats measure, never assume

Each is a question; a seat that answers one says by which command.

1. **Grammar and identity.** Where do `@` and `lent` stand in `TypeArg` beside a name (`(function(@app: App) -> ())`,
   `(function(c: Config lent) -> i64)`), and does `Param`'s rule *"`@p: T lent` is refused"* (§ 4 line 164, panel 217's
   R1) carry over? Does a mode enter the interned type's key (`selfhost/check/table.hero:142`, `ty_key`) and the C
   typedef's key (`selfhost/emit/synth.hero:35`, `content_key`), the two silent sites panel 112's R6 names (§ 3.5)?
   **They are not the only places a mode can be lost** (the critic's § 2 item 4, re-run by the repair at the base:
   `grep -rn 'intern_function(' selfhost`, code lines outside test blocks). A function type is **rebuilt from its
   parameters, labels and result**, where a mode the rebuild does not copy is dropped, at `selfhost/ir/mono.hero:299`
   (monomorphisation, sketch B's core), `check/generics.hero:326` (substitution), `check/type_fit.hero:81` (`fits`
   re-interns its filled blanks), `check/lower.hero:66` (a written type), `check/fixed_repairs.hero:61`,
   `check/function_value.hero:78` and `:240` (a name as a value); and **two places relate function types without
   comparing ids**: `type_fit.fits` (37) and `generics.bind` (31; its function-type arm `check/generics.hero:96-130`, parameter by parameter, then
   the labels at 118), where a generic `(function(T) -> ())` bound to a value of `(function(@App) -> ())` would hide
   the `@` unless `bind` compares modes (**the critic's inference from `bind`'s code, unrun**: no compiler with modes
   exists to run it on; **the case to build first**). And `content_key` (`synth.hero:35-42`) hashes
   `render.render_ty`'s **rendering** (`check/render.hero`, 229 lines, its `function_ty` arm at 92), so a mode the
   renderer does not print is a mode the typedef's name does not carry; `instance_key` (`synth.hero:47`) keys a generic
   instance the same way. **The class R6 warned of has happened since, in another dimension**: defect 2650 (closed
   2026-11-23, `gh issue view 2650`), *"A function type and a generic instance over two types of one name from two
   modules ... get one C name, and clang refuses the program"*. **The labels run's measured cost** is the nearest
   precedent (`git show --stat 2724a4f9a`, 2026-09-11: 48 files, 16 of `selfhost/`; its body, `git show -s`:
   *".function_ty is CONSTRUCTED at one site and its 192 other mentions are match arms"*, one site still today,
   `table.hero:275`; *"generics were free: check/generics.hero's bind descends a function type structurally and never
   compares two whole interned ids"*, which for a mode is the silence, not the saving; *"The canonical key omits the
   names' share when a type has none, so a type nobody named keeps the C typedef name it always had"*, nine of 441
   emission traces moving by a typedef's name alone: a mode keyed the same way is what keeps a mode-free program's
   bytes).
2. **A function value kept inside the value lent with `@`.** `app.handler(@app, r)`, `app.routes[i].handler(@app, r)`,
   `state.router.handlers[i](@state, r)`: the callee is read out of the place the call lends. Is that an alias the
   exclusivity rule must see, a copy taken before the lend (§ 7 line 241: *"An expression's parts are evaluated left to
   right as written"*), or a third reading? What does a reader predict? **The base already answers the nested half**
   (the critic's § 6 probes, re-run by the repair under its stage 2, `ground/probe/q2a.hero` and `q2b.hero`): `b.f(swap(@b))`,
   where `swap` writes `b.f` to `triple` and answers 5, prints **10**, the callee `double` read before the nested
   call made it `triple`, and `fs[0](swap(@fs))` the same, exit 0 each; the emitted C loads the callee into a
   temporary before the arguments (`q2b`'s `--emit-c`, line 134: `t5 = ...[t4]; t6 = h_q2b_swap(&h0_fs); t7 =
   t5(t6);`, `ground/probe/q2b.c`). So *"a copy taken before the lend"* is what the base does for a nested `@` today,
   and a seat may start from it. And `app.handler(...)` is a field call or a UFCS call: § 3.4's probe `f4` shows the field
   winning **silently** over a top-level function of the same name today. **That is a ruling, not an accident, and
   the spec does not state it** (the critic's § 2 item 5): spec § 9 line 310 says *"`x.f(y)` is sugar for `f(x, y)`
   (UFCS), except a thread's `wait` and a mailbox's `send`"*, by which `f4` would print 13; it prints 20 because
   design.md §4.11 line 1949 (*"The resolver, on seeing a dot, first looks for a field; failing that, looks for a free
   function"*) and **panel 079** (`docs/panel/079-the-dot-that-two-passes-read-differently.md:66-84`, ratified
   2026-08-24) ruled the field first and **refused the collision clause** (*"The collision clause is NOT adopted as a
   new refusal"*, its item 4). `grep -n -i` of *field first*, *looks for a field*, *field wins* in the spec: nothing.
   So a reader of the spec alone, the blind seat's included, predicts 13 for `f4`, and `app.handler(@app, r)` stands
   on a rule the spec does not say.
3. **Arrays, maps and records of such values**: `[(function(@S, Request) -> Response?)]`, `{str: (function(@S, [str])
   -> Reply)}`, a record field; a record holding values over its own type (§ 3.4's `f6` builds today without modes);
   `==` on them (§ 7 lines 235 and 236: *"a function value compare as an address"*; probe `e1` prints `false`, `true`); a map's `missing_key`.
4. **Generics.** `function route<S>(@r: Router, pattern: str, h: (function(@S, Request) -> Response?))` is a generic
   function, which the language has. **`Router<S>` is not a form**: records take no type parameter (spec § 9 line 323,
   *"Generics: on functions only"*; design.md §4.12 line 2057; **the parser refuses `record Stack<T>` and `variant
   Maybe<T>` alike**, `error[expected_end_of_line]` at the `<`, exit 1 each (probes `g1` and `g2`, the critic's, re-run
   by the repair), so no declared type of a library can carry `S`), and **panel 252's R3 ruled that a generic record
   waits** (`docs/panel/252-*.md:565-579`, ratified 2026-11-19 by issue 2599, three days before the author's amendment
   of 2026-11-22). So how does a library's router hold handlers whose state type is the program's? § 6 gives three
   sketches; which builds, and which is robust?
5. **Is `lent` in a function type needed, or only `@`?** Two facts pull: a C callback receiving `const struct T *` has
   no spelling (panel 217's ffi-pragmatist, § 3.5, carried); and **a read-only handler in a table of `@` handlers is
   refused today by `never_rebound`** (§ 3.4's probe `u1`: *"`app` is an `@` parameter and nothing writes it"*), so a
   uniform `(function(@App, Request) -> Response)` table refuses every handler that only reads the state. Is a `lent`
   function usable where an `@` type is asked (a conversion, **the shape the names' one-way fit already has**, § 1
   item 1), is `never_rebound` waived for a function used as a value of such a type, or does the program write two
   tables? Measure it on the routers of § 5, whose handlers mix the two. **A uniform `@` table has three kinds of
   member, not two** (the critic's § 6, re-run by the repair): a handler that takes **no state** (notes' `logout()`,
   auth's `login_page(r)`) must still take the state's slot, and `@_: App` is refused by `never_rebound` in its own
   words (*"`_` binds nothing, so an `@` on it marks a parameter nothing can write"*, probe `c2`, exit 1), `_: App lent`
   is accepted (`c3`, exit 0, prints `1`), and a named slot never read is `unused_binding` (`c4` with `lent`, `c5` by
   value, exit 1 each). **And a write that changes nothing passes `never_rebound` today**: `app @ app` as an `@`
   function's first line checks and runs (`c1`, prints `4`, exit 0), and so do `app.n @ app.n` (`c8`) and a cell's `x
   @ x` (`c7`, prints `3`): a workaround a reader can write, which no ruling the critic or the repair found names
   (`grep -n -i 'itself\|self-assign'` of panels 209 and 210, the critic: nothing). **The driving program of this
   question is agent** (§ 5.4): its tool table, `agent/tools/toolbox.hero:94-113`, is the one table in Rebel's examples
   whose every handler only reads (`run(box: Toolbox lent, name: str, input: json.Value)`), and one of its entries
   carries data beside the handler (`name == "run_command" && box.allowed.len() > 0`).
6. **A mode mismatch's diagnostic**: a function with `@` where a plain type is asked, the converse, a `lent` one where
   `@` is asked, and a call through a value missing its `@` or adding one. Its class and code (which of today's four
   codes retire, § 3.3), its words, and whether the fix is `certain` (`.claude/rules/diagnostics-and-goldens.md` § Errors
   are a deliverable): panel 110 made `aliased_mutable_arguments`'s fix a `guess` because two readings diverge
   (design.md §4.8 lines 1591 to 1599); does a missing `@` through a value have one reading? **And what the control
   arm gives it** (the critic's § 2 item 6): Part 11's control arm, `build --permissive` (panel 226's R1), RUNS a
   program the strict build refuses, with a meaning pinned per code, and `tests/golden/control/` (58 files, `ls | wc
   -l`) pins `marker_mismatch-extra-at`, `-missing-at`, `-non-place` and `never_rebound` (`.hero` and `.expected`
   each): a missing `@` *"is the caller's place all the same: the declaration decides, as C++'s `T&` and Pascal's
   `var` do"* (`marker_mismatch-missing-at.hero:4`), and an extra one *"says nothing: the value is passed"*
   (`marker_mismatch-extra-at.hero:4`). A call through a value has no declaration; under the proposal its
   **type** decides, and `selfhost/ir/flatten.hero:608` `value_call` is where the control arm's meaning of a missing or
   extra `@` through a value is written (`.claude/rules/verification.md`'s map: *"the control arm's meanings live in
   `ir/flatten.hero`"*). **Measured by the repair at the base, and a defect today**: probe `x1`, `fs = [double]`, `x @=
   3`, `fs[0](@x)`, is `marker_mismatch` under `check` (exit 1), accepted by `check --permissive` (exit 0), and **`build
   --permissive` exits 2**: *"internal error: compiling the generated C failed ... incompatible pointer to integer
   conversion passing 'int64_t *' ... to parameter of type 'int64_t'"*, the lowering handing `&h1_x` to a by-value
   slot where the pinned meaning is *"the value is passed"* (`ground/probe/x1.permissive.out`). Not filed by the repair (no GitHub write is its to make): the coordinator
   files it, `blocking` by its class (an exit 2). What the control arm owes a mode through a value is part of this
   question.
7. **The C boundary** (the ffi-pragmatist's question, design.md §4.19 and Part 8 item 19): an `extern` callback
   parameter whose type carries `@` or `lent`: refused, or `struct T *` and `const struct T *` (Part 8 item 19, carried:
   102 of 153 unspellable callback parameters in `sqlite3.h` are a pointer to a header's struct, +64 callbacks; 26 an
   out-parameter)? **Part 8 item 19's code for the out-parameter is stale** (the critic's § 2 item 7, re-run by the
   repair): design.md lines 4606 to 4608 say `(function(i32, @out: i32) -> i32)` is
   `expected_function_type_params_close`; under the base's stage 2 it is **`error[expected_type]: expected a type,
   found `@``** at 1:34, exit 1 (probe `o1`), the code of `p1`; `expected_function_type_params_close` is `lent`'s
   refusal (`p4`), not `@`'s. What does `@` promise there that C can break (design.md §4.8 lines 1620 and 1621:
   *"`@` parameters never alias, so a direct-pointer lowering and `restrict` stay legal"*), and what does the thunk
   (`selfhost/emit/callback_thunk.hero`, 429 lines; `runtime/parts/callback.c`, 111 lines) owe it? **No binding in the
   tree needs this spelling today** (the critic's § 5, re-run by the repair): every function-typed parameter inside a
   `group` or `extern` block of `rebel/library`, `selfhost/`, `examples/` and `rebel/examples/` is a thread body, **18**
   (`awk` over the files `grep -rlE '^(group|extern) '` names, each `(function(` line inside a block; 2 in Rebel's
   library, `threads/hero_os.hero:22` and `:37`, 16 in `selfhost/` and `examples/`, none in `rebel/examples/`), every
   one `(function(i64) -> i64)`, `(function(x: i64) -> i64)` or `(function(str) -> str)`, the `body` of a
   `hero_thread_spawn` function. So the C half rests on Part 8's `sqlite3.h` counts (carried), panel 217's example and
   the bindings the ffi-pragmatist writes, a lower bound of that search; and since those 18 are functions C calls on a
   thread of its own, a mode admitted in an `extern` callback's type meets question 8 there (the critic's reading of
   the declarations, unrun).
8. **Threads**: `spawn(f, x)` and `spawn_sized` with an `f` whose parameter is `@` or `lent` (§ 11 line 383: *"threads
   share nothing but a mailbox"*). Refused, by which code; or a hand-over (`spawn(f, @x)`, which panel 254's
   spec-warden probed: *"cannot complete a hand-over (`mutable_parameter_as_value`, `consumed_borrowed_handle`)"*,
   carried from `docs/panel/254-*.md:157`)? Today, at the base: `spawn(bump, @x)` with `bump(@n: i64)` is
   `mutable_parameter_as_value` and `spawn(get, Cfg(n: 4))` with `get(c: Cfg lent)` is `lent_parameter_as_value`, exit 1
   each (probes `s1` and `s2`, the critic's, re-run by the repair): the refusal is the value's, not `spawn`'s. A
   mode-carrying value **sent through a mailbox** is the shape beside it (the critic's § 6, unbuilt).
9. **The emitted C**: a value's typedef with a pointer slot (today `typedef int64_t (*h_0fn_32a9823f)(h_f7_App,
   int64_t);` beside `int64_t h_f7_bump(h_f7_App *ph0_a, int64_t h1_x);`, § 3.4's `f7.c`); a `lent` slot's `const`;
   a function of the library standing as a value (its `hero_value_` twin dropping `const HeroPlace *hero_caller`,
   `selfhost/emit/signature.hero:326-342`'s test, which **already writes a pointer slot in a value's twin**: from a
   library function with `@b`, `hero_value_h_library_dist(int64_t h0_a, int64_t *ph1_b)`, line 342); **two value-call
   conventions a mode slot meets** (the critic's § 3): a large group record through a function value keeps C's
   convention through a twin (panel 236's R3, `signature.hero:99-102`, `large_value.hero:82` `value_large`), and a
   self-call through a function value (panel 199); and **byte-identity of every emission that does not use the form**
   (the seed's fixpoint, `tests/emission`, `tests/golden/emit`).
10. **Every tool that re-prints a program** (`.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in
    every tool that reads the language, lines 96 to 157): the formatter, the dump printers, the formatter's
    self-check, `heroes mutate`, `heroes probe`'s reader, the fixes that quote a program, `heroes measure`, `heroes
    grammar` (it prints the spec's productions, `selfhost/cli/grammar.hero:138`), the editor's grammar and the site's
    highlighter; **and `heroes doc`** (the critic's § 2 item 8: `selfhost/cli/doc.hero` 330 lines, `doc_shape.hero`
    369, `doc_text.hero` 323, `wc -l`; `selfhost/cli/table.hero:195`, its summary: *"each declaration's signature ...
    the call a program writes"*) **and the generator that reads what it prints**: `rebel/examples/PACKAGES.md` line 3,
    *"Every word of it is read from what `heroes doc` prints of each face"*, written by `rebel/generators/rebel_index/`
    (`packages_text.hero`, `faces.hero`), whose reader of `heroes doc` already broke once on a generic function's
    signature (defect 2761, `gh issue view 2761`: *"its reader of `heroes doc` takes a generic function's obligations
    for the end of the function"*). A face whose handler type carries a mode is printed by both.
11. **The spec's real cost** (§ 3.6), and where each sentence lives (`.claude/rules/spec-shape.md` § Where a rule lives).
12. **The record**: did any sitting refuse this, and why (§ 3.5)? Is panel 217's refusal of the `lent` half a ruling
    or an absence? **The record holds a measured answer to hand over** (the critic's § 3): panel 217's R2 lists *"a
    function with a `lent` parameter used as a value"* among *"the refusals that are part of the form"*
    (`docs/panel/217-*.md:119-123`), **with its alternative written in the sitting's record**: its spec-warden's item
    6 (`docs/panel/217-reports/spec-warden.md:71-76`), *"an adapter passing the argument's address is sound; if the
    prototype refuses instead, the text owes that clause"*. So a ruling, with its rejected alternative named. And does
    panel 109's measured need (a mutually recursive walk that threads `@b`), **with panel 121's R5 and panel 138's
    sub-parser beside it** (§ 3.5), count as the compiler's own?
13. **The route nobody listed** (§ 7): a handler record kind, a state the router owns, closures, generic records, a
    variant of routes matched exhaustively, the state passed and answered by value, the gateway's owner thread, and
    the eight routes and shapes the critic's § 6 added (§ 7 (i) to (p)); say why each is or is not the robust one, and
    name one this list lacks.
14. **What the landing owes the tree**: every Rebel example of § 5 rewritten, and every library face that takes a
    handler (§ 5.1), with nothing kept for an older spelling (CLAUDE.md § 2: *before `1.0.0`, no form enters to keep an
    older program compiling*). **Two costs no brief named before the repair** (the critic's § 3): `web` stands at **88
    public functions** at the base (`grep -c '^function [a-z]' rebel/library/0.1.0/files/web.hero`) against the `rebel`
    suite's fence of **90** (`tests/harness/rebel_faces.hero:45-46`, `SIZE_MOST`; decision 2802, closed 2026-11-24,
    which split `web/proxy` out to bring `web` from 93 to 88), so sketch B's `get`, `dispatch` and `serve_with`, or a
    face per method, cross it; and **feature 2658** (closed 2026-11-23) delivered `before_with<S>`, `after_with<S>`,
    `require_basic_with<S>` and `require_bearer_with<S>` (`web.hero:626`, `:647`, `:693`, `:740`), the by-value route (f)
    already taken in the library, saying *"a step that changes state, `limit_rate`'s limiter, stays a call the program
    makes"*: those four faces are this question's census too, with the faces that take a function besides `web` and
    `http` (`openapi.add_routes`, `jsonrpc`/`mcp`, `graphql`, `grpc`, `jobs`, `ui`; § 5.1).

## 3. The language today

### 3.1 The spec at the base `df92f9281` (516 lines, 35,246 bytes, `wc -l -c spec/heroes-spec.md`; sha256 `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f`, `shasum -a 256`; the repair, 09:46)

At the first base it read 516 lines, 34,828 bytes, sha256 `aaf23b9e...6f3`; panel 260's step 197d replaced lines 401
and 508 (§ 13's platform-layer sentence, `Extern`'s production), so no cited line moved. The sentences the proposal
moves (`sed -n` of each range, re-run by the repair at the base, each as quoted):

- line 109, § 3's table: *"`(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory"*;
- lines 123 to 128, § 3's productions: `Prefix = ... | "(" "function" TypeArgs "->" Type ")" ...`, `TypeArgs = "(" [
  TypeArg { "," TypeArg } ] ")" .`, `TypeArg = Type | ident ":" Type .`;
- line 164, § 4: `Param = "@" ident ":" Type | ident ":" Type [ "lent" ] .`;
- line 178, § 5: *"... and so is a cell nothing re-binds or an `@` parameter nothing writes or ends (section 13), an `@`
  argument counting"*;
- lines 305 to 309, § 9: a pair sharing a type is named at the call *"and a function type names the same two,
  `(function(from: str, to: str) -> ())`. The names are part of the type ... a call through a value names each in its
  own position"*;
- lines 310 and 320, § 9: UFCS, *"UFCS does not apply when the first parameter is `@`"*;
- lines 314 to 319, § 9: *"Mutable parameters are marked `@` in the signature and at the call site ... a read-only one
  is marked `lent`, in the signature alone ... Either way the callee works on the caller's place itself, which no other
  `@` argument of the call may reach, nor, for `lent`, one nested in another argument."*;
- **line 321**, § 9: *"Top-level functions are values, but not one with an `@` or `lent` parameter: `xs.fold(0, add)`."*;
- lines 323 to 329, § 9: generics *"on functions only"*;
- line 334, § 9: `Arg = [ ident ":" ] [ "@" ] Expression .` (already admits `@` at any call);
- line 383, § 11: `spawn(f, x)` *"runs `f(x)` on a thread of its own"*; *"threads share nothing but a mailbox"*;
- lines 427 and 428, § 13: *"A callback is a **parameter**, never a result or a signal handler, nor called by one; its
  parameters follow the same rule and `()` is `void`"*.

### 3.2 design.md (5,436 lines at the base, `wc -l`, 5,432 at the first; reached by `grep -n`, every line below re-located by the repair)

- **§4.13 Functions as values**, lines 2062 to 2121: *"Functions that capture nothing are values"* (2069); **2072 to
  2078**: *"But not one with an `@` or a `lent` parameter (the `lent` half panel 217's R1, landed 2026-10-18 at
  M-readonly-lend; the `@` half `error[mutable_parameter_as_value]`, which the specification left to its function
  type's production, `TypeArg`, until that landing): no function type carries either mode, so a call through a value,
  `f(x)`, would hide the `@` the call owes, or take a copy of what `lent` reads in place."* **The proposal answers that
  reason**: the type carries the mode, and the call through the value writes the `@`.
- **§4.8 Mutable parameters**, lines 1521 to 1631: copy in, copy out as the model, the place itself as what C sees
  (1535 to 1547); `lent` (1549 to 1568); exclusivity and *"comparing places step by step is a complete alias test"*
  (1587 to 1621; the `guess` 1591 to 1599; *"`@` parameters never alias"* 1620 and 1621).
- **§4.9**, lines 1761 to 1763: a generic path checks that *"the names of the function handed over must AGREE with
  the type's names for those positions"*, `error[callback_name]` (§ 1 item 1).
- **§4.11 UFCS**, line 1949: *"The resolver, on seeing a dot, first looks for a field; failing that, looks for a free
  function"*, panel 079's ruling, which the spec does not state (§ 2 question 2).
- **§4.12 Generics**, line 2057: *"Generics on functions only, not on types."*
- **Part 6**, line 3643 (traits refused, panel 137) and **line 3644 (closures that capture, refused on cost, panel
  119)**: *"the **capture-free** narrowing is named there as the form that returns"*.
- **Part 8 item 19**, lines 4582 to 4640 (panel 112): the callback vocabulary; its *"fifth cause"*, 4606 to 4608, names
  the out-parameter's code as `expected_function_type_params_close`, **stale at the base** (`expected_type`, § 2
  question 7); **its last lines, 4634 to 4640**: *"What would make this row wrong (§12): a binding ... that needs a
  Heroes function value in a callback slot this vocabulary cannot spell ... the route it must take is then the one
  panel 112 R6 records, a qualifier field on the function type that already exists (**2 edits in 1 file**, measured)
  rather than a new `Ty` case."* (carried: the counts in that item are panel 112's, 2026-09-05, never re-run here).

### 3.3 What the compiler does today, run (the clerk with the seed's compiler at `07e747bc6`, 00:59; **re-run by the repair with its stage 2 at `df92f9281`**, `sh ground/probes.sh ./heroes-next <ground/probe> <out>` from its tree's root, 09:58: the same first line and exit for each, paths aside, `diff` against the clerk's `*.next.out`; files and outputs, `*.repair.out`, in `ground/probe/`)

| probe | what it writes | answer |
|---|---|---|
| `p1` | `h: (function(@App, i64) -> i64) = bump` | **`error[expected_type]: expected a type, found `@``** at 10:18, exit 1 (the coordinator's measurement, re-run) |
| `p2` | `fs = [bump]`, `function bump(@a: App, n: i64) -> i64` | **`error[mutable_parameter_as_value]`**: *"`bump` changes its argument `a` through `@`, and a function value cannot: no function type carries an `@` (§ 9) ..."*, with a note offering *"a function without `@` that returns the new value"*, exit 1 |
| `p3` | `fs = [get]`, `function get(c: Cfg lent) -> i64` | **`error[lent_parameter_as_value]`**: *"... no function type carries `lent` (§ 9), so `get` is only called by name"*, exit 1 |
| `p4` | `f: (function(Cfg lent) -> i64)` | **`error[expected_function_type_params_close]`**: *"expected `)`, or `,` and another parameter type, found a name (`lent`)"*, exit 1 |
| `o1` (the critic's) | `f: (function(i32, @out: i32) -> i32)`, design.md Part 8 item 19's out-parameter | **`error[expected_type]`** at 1:34, the `@`, exit 1: `p1`'s code, not the `expected_function_type_params_close` Part 8 item 19 carries (§ 2 question 7) |
| `x1` (the repair's) | `fs = [double]`, `x @= 3`, `fs[0](@x)` | **`error[marker_mismatch]`**, exit 1; `check --permissive` exit 0; **`build --permissive` exit 2**, clang refusing the lowering's `&h1_x` to a by-value slot (§ 2 question 6; `x1.permissive.out`) |

And a fourth code, never reached by `p1` to `p4`: an `@` at a call through a value is **`marker_mismatch`**, *"a
function value does not change this argument — the `@` says it does"* (`selfhost/data_errors.hero:213-221`,
`marker_through_a_value`, asked at `selfhost/check/walk.hero:1323` through `selfhost/check/markers.hero:107`; probe `x1`).
**The goldens that pin them** (the repair's count, which corrects the clerk's 11, the critic's § 2 item 9: `grep -rlE
'mutable_parameter_as_value|lent_parameter_as_value|no function type carries|a function value does not change this
argument' tests/golden | wc -l`, **10 files**): `check/fixedbugs-398-a-function-with-an-at-parameter-has-no-value`
(`.hero`, `.expected`), `check/fixedbugs-398-an-at-through-a-function-value-marks-nothing.expected`,
`check/fixedbugs-412-an-at-argument-to-a-function-without-one.expected`,
`check/fixedbugs-431-an-at-argument-a-call-takes-none-for.expected`,
`check/lent-a-function-value-a-marked-call-and-lent-beside-at` (`.hero`, `.expected`),
`surface-fixtures/value398/main.hero`, and **two** frozen files of `tests/golden/recovery/corpus/`,
`selfhost/data_errors.hero.frozen` and `tests/harness/suite_surface.hero.frozen` (the clerk's *"three"* names a file
neither the critic's command nor the repair's finds). **Beside them, the wider census a landing owes**: `marker_mismatch`
alone is on **73 files** of `tests/golden` (`git grep -l marker_mismatch HEAD -- tests/golden | wc -l`, the same at
both bases; the critic's *"89"* is not reproduced by the repair's command, nor by `grep -rl` in the critic's own copy,
73), almost all of them calls by name: a landing that changes its reading through a value owes the census of those;
**the control arm's four** under `tests/golden/control/` (`marker_mismatch-extra-at`, `-missing-at`, `-non-place`,
`never_rebound`, § 2 question 6), judged by the `control` form; the `full` form's seven files naming
`marker_mismatch`, `mutable_parameter_as_value` or `lent_parameter_as_value` (`grep -rlE` over `tests/golden/full`; two
more name `never_rebound`); and the `permissive` form (44 files), whose folder names none of those three codes
(`grep -rlE`: nothing) and `never_rebound` in two cases of defect 614 (a wildcard cell), but which a landing that gives
a mode a meaning under `--permissive` reaches. **Which of them a landing flips is a seat's count, unrun.**

### 3.4 What works today beside it, run (the clerk with the seed's compiler at `07e747bc6`; **re-run by the repair with its stage 2 at `df92f9281`**, `ground/probes.sh`, 09:58: every line and exit below as written, `f1` to `f6` `diff`-equal to the clerk's `f-next.out`)

| probe | what it writes | prints |
|---|---|---|
| `f1` | a record field `f: (function(i64) -> i64)`, called `b.f(b.n)` | `6`, exit 0 |
| `f2` | `g = b.f`, `g(b.n)`; `fs = [double, double]`, `fs[1](4)` | `6`, `8`, exit 0 |
| `f3` | `m: {str: (function(i64) -> i64)}`, `m["d"].must()(5)` | `10`, exit 0 |
| `f4` | the field `f` AND a top-level `function f(b: Box, x: i64)`, called `b.f(10)` | **`20`**, exit 0: the field's `double`, not `f`'s `b.n + x` (13); **no diagnostic** |
| `f5` | `a.n @ a.f(a.n)` inside `function bump(@a: App)` | `6`, exit 0 |
| `f6` | `record App` holding `handlers: [(function(App, i64) -> i64)]`, called `a.handlers[0](a, 4)` | `7`, exit 0 |
| `u1` | `function health(@app: App, x: i64) -> i64` that only reads `app` | **`error[never_rebound]`**, two `guess` fixes: `app: App lent`, or remove the `@` |
| `f7` | `--emit-c` of a value of `get(a: App, x: i64)` beside `bump(@a: App, x: i64)` | `typedef int64_t (*h_0fn_32a9823f)(h_f7_App, int64_t);` (line 57), `int64_t h_f7_bump(h_f7_App *ph0_a, int64_t h1_x);` (113), and the value built as `t3 = h_f7_get;` (133); `ground/probe/f7.c`, 12,005 bytes |

**`f1` to `f7` read the same under stage 2** (the clerk's `heroes-next run` and `--emit-c` at the first base, written
by 01:16:20, the mtime of `f-next.out`: the same lines, exit 0 each; `ground/probe/f-next.out`, `f7-next.c`, whose
lines 57, 113 and 133 are the three above), **and under the repair's stage 2 at the base** (`f7`'s C 13,409 bytes,
`diff`-equal to `f7-next.c` but for the one line `hero_program_sources`, whose key moves with any byte of the runtime).

**The critic's probes beside them** (`docs/panel/261-reports/critic-briefs.md` § 3 and § 6, copied into `ground/probe/`
under short names and **re-run by the repair** at the base, `*.repair.out`):

| probe | what it writes | answer |
|---|---|---|
| `q2a` | `b.f(swap(@b))`, `swap` writing `b.f` to `triple` and answering 5, then `b.f(1)` | `10`, `3`, exit 0: the callee read before the nested call (§ 2 question 2) |
| `q2b` | `fs[0](swap(@fs))`, then `fs[0](1)` | `10`, `3`, exit 0; its C (`q2b.c:134`) loads the callee before the call that writes it |
| `c1` | `app @ app` as the first line of `health(@app: App, x: i64)` | checks, prints `4`, exit 0: a write that changes nothing passes `never_rebound` |
| `c8` | `app.n @ app.n` in the same | `4`, exit 0 |
| `c7` | a cell's `x @ x` | `3`, exit 0 |
| `c2` | `logout(@_: App, x: i64)` | **`error[never_rebound]`**: *"`_` binds nothing, so an `@` on it marks a parameter nothing can write"*, a `guess` fix, exit 1 |
| `c3` | `logout(_: App lent, x: i64)` | `1`, exit 0 |
| `c4`, `c5` | `logout(app: App lent, ...)`, `logout(app: App, ...)`, `app` unread | **`error[unused_binding]`**, exit 1 each |
| `c6` | `logout(_: App, x: i64)` as a value, `fs[0](a, 1)` | `1`, exit 0 |
| `e1` | `a = [double, triple]`, `a[0] == a[1]`, `a[0] == double` | `false`, `true`, exit 0 |
| `k1` | `constant TABLE: [(function(i64) -> i64)]` holding `[double]`, `TABLE[0](4)` | `8`, exit 0 |
| `k3` | `constant TABLE: {str: (function(i64) -> i64)}` holding `{"d": double, "t": triple}`, `TABLE["t"].must()(4)` | `12`, exit 0 |
| `k2` | `constant ROUTES: [Route]` of records holding handlers | **`error[constant_body]`**: *"`constant ROUTES`'s body may not contain a call"*, exit 1 |
| `g1`, `g2` | `record Stack<T>`, `variant Maybe<T>` (kept as `.hero.txt`, they do not parse) | **`error[expected_end_of_line]`** at the `<`, exit 1 each (§ 2 question 4) |
| `s1`, `s2` | `spawn(bump, @x)` with `bump(@n: i64)`; `spawn(get, Cfg(n: 4))` with `get(c: Cfg lent)` | `mutable_parameter_as_value`, `lent_parameter_as_value`, exit 1 each (§ 2 question 8) |

### 3.5 The record: who weighed this before (`grep -l` over `docs/panel/*.md` for `mutable_parameter_as_value` → 210, 254; `TypeArg` → 254; *function type carries* → 103, 129, neither about modes; `gh issue list --search` for *mutable_parameter_as_value*, *lent_parameter_as_value*, *function type carries*, *handler state router*, *function value @ parameter*: one issue about the modes, 1734, defect 398; **widened by the critic's first reading**, `grep -lE` over `docs/panel/*.md` for *through a function value*, *function type.\*`@`*, *used as a value*, *marker_mismatch*, *generic record*, and `gh issue list --search` for *function value lent*, *Router*, *closure*, *generic record*, *never_rebound function value*: the entries marked **(the critic)** below, each re-read by the repair at the lines named)

- **Panel 109, 2026-09-04** (`docs/panel/109-*.md:378-386`, read): the IR lowering's knot of 25 functions and 990 code
  lines could leave one module only by handing the recursion over as a function value, *"and this walk threads its
  builder as `@b`: `(function(@Box) -> ())` is `error[expected_type]` ... run on this compiler, so there is no such
  value to hand over"*. **A need of the compiler's own, measured, and not ruled on.**
- **Panel 112, 2026-09-05** (`docs/panel/112-const-is-the-wrong-noun.md:293-299`, read): **R6** records *"the `quals`
  shape ... as the ONLY admissible route if the const half is ever closed"*: no new `Ty` variant (a new one cost *"173
  `non_exhaustive` errors across 49 files"* then), **2 edits in 1 file**, and *"two silent sites that are owed tests
  when it lands — `check/table.hero`'s `ty_key` and `emit/synth.content_key`, where two callback types differing only
  in `const` would intern to one TyId with no diagnostic"*. Both functions stand today (`grep -n`:
  `selfhost/check/table.hero:142`, `selfhost/emit/synth.hero:35`); `table.hero:157` keys a function type by its
  params, labels and result, **no mode**.
- **Panel 079, convened 2026-08-16, ratified 2026-08-24** (the critic; `docs/panel/079-the-dot-that-two-passes-read-differently.md:66-84`):
  **the field first** at a dot, as design.md §4.11 line 1949 says, *"Swept before landing: 0 of 268 corpus files change
  meaning"*; and *"The collision clause is NOT adopted as a new refusal"* (item 4): a record with a field `g` and a
  function `g` is unambiguous, the field winning. The spec does not state it (§ 2 question 2, probe `f4`).
- **Panel 119, 2026-09-08**: closures that capture refused (design.md line 3644, § 3.2).
- **Panel 121, 2026-09-08, R5** (the critic; `docs/panel/121-the-brace-was-already-taken.md:283-289`):
  `selfhost/grammar_expr.hero` had no legal home because *"the escape hatch is shut because a function type cannot
  declare an `@` parameter"*: **a second measured need of the compiler's own** beside panel 109's.
- **Panels 128 to 130 and feature 1133, 2026-09-11** (the critic; `docs/panel/129-the-names-inside-the-type.md`,
  `130-the-name-a-generic-callback-could-not-say.md`, `gh issue view 1133`): names in the function type, with **a
  one-way fit** and **a generic path comparing the names** (§ 1 item 1), the labels run of `2724a4f9a` (§ 2
  question 1): the precedent a mode run follows, relations included.
- **Panel 138, 2026-09-13** (the critic; `docs/panel/138-the-item-named-a-parser-that-never-existed-and-the-route-was-already-in-the-grammar.md:42-48`):
  the sub-parser taking `@p: Parse` *"is not passable"*, and *"it needs a second feature — `@` in function types — that
  Part 7 does not list and no sitting has proposed"*: **the record saying, that day, that nobody had proposed the
  form**; its historian's OCaml precedent at line 58.
- **Defect 398** (issue 1734, filed 2026-10-06 by panel 194's compiler-engineer, closed 2026-10-07 at `9aeac48e`):
  `_ = bump` with `bump(@n: i64)` built a program clang refused at exit 2; the repair, *"Spec § 9 decided it: no
  function type carries an `@`"*, added `mutable_parameter_as_value` and `marker_mismatch` through a value. **A repair
  of an exit 2, not a sitting that weighed the form.**
- **Panel 199, 2026-10-07** (the critic): a self-call through a function value, one of the value-call conventions a
  mode slot meets (§ 2 question 9).
- **Panel 210** (`docs/panel/210-*.md:36-38`): cites `mutable_parameter_as_value` as a fact (*"A function with an `@`
  parameter cannot be a value"*), weighing nothing.
- **Panel 217, 2026-10-16** (R1 and R2, `docs/panel/217-*.md:113-126`): *"a function with a `lent` parameter used as a
  value"* is one of **R2's refusals that are part of the form**; its compiler-engineer: *"F1 refuses a function with a
  lent parameter as a value (`check/markers.hero`, +17), since no function type carries the mode"*
  (`docs/panel/217-reports/compiler-engineer.md:177-179`); its spec-warden: *"if the prototype refuses instead, the
  text owes that clause"* (`spec-warden.md:71-76`), **whose whole item 6 is the alternative the sitting did not take**
  (the critic): *"an adapter passing the argument's address is sound"* for a `lent` function used where a by-value
  type is asked; its ffi-pragmatist: a C callback receiving `const struct pt *` *"has
  no spelling today"*, and the C an F1 reaching a function type's parameter would emit *"passes to that callback type
  and prints `42`"* (`ffi-pragmatist.md:188-196`). **A ruling, with its rejected alternative written in the record**
  (question 12).
- **Panel 226, 2026-10-20** (the critic): Part 11's control arm, `build --permissive`, and its pinned meanings of
  `marker_mismatch` and `never_rebound` (`tests/golden/control/`; § 2 question 6).
- **Panel 236, 2026-11-08** (the critic): a large group record through a function value keeps C's convention through
  a twin (`selfhost/emit/signature.hero:99-102`, `large_value.hero:82`; § 2 question 9).
- **Panel 252, 2026-11-19** (R3, `docs/panel/252-*.md:565-579`): records with type parameters refused *"on Principle
  0's burden"*, *"What robust would have been: Q3-R (+86 real)"*; its cost **as R3 itself states it**, *"288
  incomplete matches in 109 files (the critic's re-count of the compiler-engineer's 290 in 111, on the frozen
  `selfhost/` alone)"* (`:567-570`), the seat's first prediction (*"touches at least 111 files of `selfhost/`"*,
  `:650`) being the count the sitting corrected (the critic's § 2 item 2).
- **Panel 254** (`docs/panel/254-*.md:157`): the `spawn(f, @x)` probe of question 8.
- **Defect 2650** (the critic; closed 2026-11-23, `gh issue view 2650`): two function types from two modules got one
  C typedef and clang refused a program `heroes check` accepted: panel 112's R6 class, happened in another dimension
  (§ 2 question 1).
- **Feature 2658** (the critic; closed 2026-11-23): `web`'s steps reach the server's state by value,
  `before_with<S>` and three siblings, **route (f) already taken in the library** (§ 2 question 14).
- **Decision 2802** (the critic; closed 2026-11-24): `web` at 88 public functions against the `rebel` suite's 90
  (§ 2 question 14).
- **CLAUDE.md § Precedence, amended 2026-11-22**: Principle 0 and the budget yield to the most robust solution, a
  new form included. Every sitting above sat before it.

### 3.6 The spec's count, at the base `df92f9281` (the repair, 09:52 to 09:53)

The seed's compiler (`./heroes measure spec/heroes-spec.md` from the repair's tree's root, `sources 05186f2b38e77a97`):
claude-legacy **10,067**, cl100k_base **10,245**, then **`STALE: the recorded count is for 4dced6767460e590 and this
file is 24de425e6c28e69b`**: the seed (unchanged by the move, `git diff --stat 07e747bc6 df92f9281 -- seed`, empty)
predates the spec's last two changes, step 192c (`168d3d6c7`, panel 259) and step 197d (`770648d1b`, 2026-11-26
09:39:27, panel 260; `git log -1 -- spec/heroes-spec.md`), whose pin is `selfhost/measure/pinned.hero:55` and `:64`,
`13439` and `"24de425e6c28e69b"`. So the real row is read with **stage 2** (§ 8), not the seed. **Stage 2**
(`heroes-next`, `sources 4dcf3a9f20257573`, the same command, no `--refresh`): claude-legacy **10,067**, cl100k_base
**10,245**, **real 13,439** (`claude-opus-5`, the pin of 2026-11-26), *"Headroom: 2945 against the 16384 ceiling —
but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 13499"*. **At the first
base** the clerk read 9,966, 10,144 and real **13,309**, headroom 3,075 (stage 2 `sources 30e08049445a2331`): the
move added **130 real tokens**, panel 260's two lines, none of them in a sentence this proposal moves. **A candidate's
real count needs `--refresh`**, one request to the count-tokens endpoint each: `spec-warden.md` says when.

**The four refusals of § 3.3 and `u1` read the same under the repair's stage 2** (`heroes-next check`, 09:53, exit 1
each, the same output but for the file's path, `diff`; outputs `ground/probe/*.repair.out` beside the clerk's
`*.next.out`).

## 4. The compiler today: where the form would land (line counts by `wc -l` and every pointer by `sed -n`, re-run by the repair at `df92f9281`; two counts moved, `print/fmt.hero` and `print/dump.hero`, and the AST node's lines)

| what | where | lines |
|---|---|---|
| the type's parser: `unit_or_function` (194), `function_parts` (226), `function_params` (286), `named_parameter` (335); the refusal of `lent` at 321 and 325 | `selfhost/parse/type.hero` | 671 |
| the AST node, `function_type` with `params`, `labels`, `result` | `selfhost/ast.hero:339-353` (335-360 at the first base) | — |
| the checker's type, `function_ty` with `params`, `labels`, `result` (no mode); `ty_key` (142), `intern` (250), the `function_ty` arm of the key (157) | `selfhost/check/table.hero` | 559 |
| a name standing as a value: `signature_of` and the generic instance (defect 402); **it rebuilds the type** at 78 and 240 (`intern_function`) | `selfhost/check/function_value.hero` | 350 |
| **where a function type is rebuilt from its parameters, labels and result** (the critic's § 2 item 4; `grep -rn 'intern_function(' selfhost`, code lines): monomorphisation (`mono.hero:299`), substitution (`generics.hero:326`), a written type (`lower.hero:66`), a growable twin (`fixed_repairs.hero:61`); the one constructor of the case, `table.hero:275` (`intern_function`, 270) | `selfhost/ir/mono.hero`, `check/generics.hero`, `check/lower.hero`, `check/fixed_repairs.hero` | 553, 441, 148, 148 |
| **where two function types are related without comparing ids**: `fits` (37; its comment 13 to 36, *"It is a widening"*; the re-intern at 81), `bind` (31; its function-type arm 96 to 130, parameter by parameter; `named` called at 118, defined at 281) | `selfhost/check/type_fit.hero`, `check/generics.hero` | 169, 441 |
| **the C typedef's name and a generic instance's**: `content_key` (35, hashing `render.render_ty`'s rendering), `instance_key` (47); the renderer's `function_ty` arm (92) | `selfhost/emit/synth.hero`, `selfhost/check/render.hero` | 308, 229 |
| the refusals: `blocked` (35), `refused_as_value` (64, the `@` and `lent` halves), `through_a_value` (107) | `selfhost/check/markers.hero` | 109 |
| a call through a value, `indirect_call` (1296), the arity, the labels, `markers.through_a_value` (1323); a call by name's `marker_mismatch` (1678) | `selfhost/check/walk.hero` | 2,486 |
| two `@` arguments reaching one place | `selfhost/resolve/writes.hero` (`refuse_aliased_places` 260), `selfhost/resolve/aliased_places.hero` | 331, 223 |
| an `@` beside a `lent` of its place, nested included | `selfhost/check/read_lends.hero` | 223 |
| `spawn`'s check | `selfhost/check/thread_ops.hero` | 255 |
| the lowering of a call through a value, `value_call` (608, *"no declaration is known, so no parameter's mark is read"*), `lower_args` (645), `lent_argument` (726) | `selfhost/ir/flatten.hero` | 1,423 |
| the IR's `Arg` (`value_arg`, `inout_arg` with `read_only`) and `Callee.indirect` | `selfhost/ir.hero:57-81` | — |
| the read-only slot's verifier | `selfhost/ir/read_only.hero` | 85 |
| a value's C typedef, `function_type` (155) | `selfhost/emit/types.hero` | 331 |
| a signature: an `@` slot is a pointer (`one_param`, 260), the library's caller place (`stop_place`), a value's twin (`params_as`, 102, `value: true`) | `selfhost/emit/signature.hero` | 350 |
| a function named as a value | `selfhost/emit/func_ref.hero` | 106 |
| the C type of a type, `function_ty` arm (398) | `selfhost/emit/ctype.hero` | 528 |
| the thunk C is handed for a callback | `selfhost/emit/callback_thunk.hero`, `callback_guard.hero` | 429, 207 |
| the callbacks C is inside, counted | `runtime/parts/callback.c` | 111 |
| a function type printed back | `selfhost/print/types.hero`, `print/fmt.hero`, `print/dump.hero`, `probe/reader.hero` | 89, **1,948**, **285**, 408 (1,953 and 273 at the first base) |
| a module's reference, each signature printed as a program writes it (§ 2 question 10) | `selfhost/cli/doc.hero`, `doc_shape.hero`, `doc_text.hero` | 330, 369, 323 |
| a large group record through a value, C's convention (panel 236) | `selfhost/emit/large_value.hero` (`value_large`, 82) | 242 |

`function_ty` is named on **374 lines in 141 files** of `selfhost/` (`grep -rn 'function_ty' selfhost | grep -v
'\.hero:[0-9]*: *#' | wc -l`; `grep -rl ... | wc -l`, comments included in the file count): what a field on the
existing case touches, against a new case, is the compiler-engineer's count (the same two counts at the new base).
**The labels run is the measured precedent** (§ 2 question 1): `2724a4f9a`, 48 files, 16 of `selfhost/`.

## 5. The driving programs: every Rebel program that serves HTTP

### 5.1 The library

**`rebel/library/0.1.0/files/web.hero`** (4,269 lines) is the router: `record Route` (125: method, pattern, **name**,
groups), `record Router` (147), `add(@router, method, pattern, name)` (223), `add_any` (242), `mount` (256), `record
Found` (301: name, pattern, groups, params, **no index**), `route(router, r) -> Found?` (324, the index `at` read at
331 and not answered); `web/internal/routing.hero` (347 lines), `tree.hero` (160), `patterns.hero` (268). **Its module
comment says why the program dispatches** (`web.hero:19-23`, `sed -n`): *"The language has no closure, and a function
value takes no `@` parameter (spec § 9), so no handler a router held could carry the program's state as Echo's
closures carry it. So the router finds and the program calls: it writes `match found.name` and calls its own
function, handing it the state it holds by `@`."* And *"A program with state writes the loop itself"* (`:28-35`),
since `serve` (1394), `serve_tls`
(1408), `serve_stream` (1424) and `serve_until_stopped` (2116) take `handler: (function(Router, http.Request) ->
http.Response)`.

**`http.hero`'s `serve_with<S>`** (2349) and its siblings `serve_tls_with` (2390), `serve_stream_with` (2438),
`serve_h2_with` (3022), `exchange_with` (3494), `serve_until_stopped<S>` (3555), `serve_stream_until_stopped` (3621)
take `state: S` by value and `handler: (function(S, Request) -> Response)`; its comment (`http.hero:2339-2341`): *"read
and never changed: each call receives its own copy ... and a call that changes its copy changes nothing the next one
sees"*. A function-typed parameter appears on **293 lines in 79 files** of the library (`grep -rn '(function('
rebel/library/0.1.0/files --include='*.hero' | grep -v ':\s*#' | wc -l`; `grep -rln ... | wc -l`; the same at the base): which of them want
a mode is question 14's census, unrun.

**The same refusal shapes other faces and programs.** `grep -rn 'takes no `@`\|no function type carries\|function
value takes no\|no closure' rebel/ --include='*.hero' --include='*.md'` prints **22 lines** (the clerk before 01:21; the repair at the base, the same), among them:
`web.hero:582-585` (a middleware step *"that changes [the program's own value], as `limit_rate` changes its limiter, is
called by the program itself, since a function value takes no `@` parameter"*) and `:1361-1362`; `graphql.hero:12`
(*"The executor finds, the program answers. The language has no closure"*), `grpc.hero:18`, `jobs.hero:19`, `ui.hero:15`
(*"No callbacks: the program asks"*), `jsonrpc.hero:8` (*"A server finds, the program calls"*); `notes/app/state.hero:2`,
`notes/http/pipeline.hero:6`, `kanban/store/changes.hero:7`; and **`interpreter/run/value.hero:8-13`**: *"the evaluator
and the statement runner need each other ... one of them takes the other as a **function value**, and a function value
takes no `@` parameter (spec § 9), so the state crosses that boundary by value"*, the shape of panel 109's knot in a
program (§ 3.5). Which of these the proposal changes, and which stay (a closure is still refused), is question 14's.

**The faces that take a function today besides `web`'s router and `http`'s servers** (the critic's § 6, the files
named re-read by the repair): `web`'s four stateful steps of feature 2658, `before_with<S>` (`web.hero:626`),
`after_with<S>` (647), `require_basic_with<S>` (693) and `require_bearer_with<S>` (740), each taking `state: S` by
value; `openapi.add_routes` (notes' API routes are named by its document, `notes/http/routes.hero:155`); `jsonrpc`
and `mcp` (agent's server, § 5.4); `graphql`, `grpc`, `jobs` and `ui` (kanban, explorer), whose module comments are
among the 22 lines above. **The `rebel` suite's fence** stands at 90 public functions a face (`SIZE_MOST`,
`tests/harness/rebel_faces.hero:45-46`) and `web` at 88 (question 14).

### 5.2 The census (`ground/census.sh`, its output `ground/census.txt`)

The examples that `use <rebel>/web` or `<rebel>/http`: 15 (`agent auth backup certs chat gateway ingest iothub monitor
notes reach registry searchd server worker`). The router's users are six. **Its `add` column counts every `add(@`
call, routes and other kinds alike, and is not a route count**; the route counts below are each program's own `grep`.

### 5.3 The router's users: how each dispatches today, and what the proposal writes

| program | routes (`path:lines`, count by `grep -c`) | dispatch today (`path:line`) | the handlers' shapes |
|---|---|---|---|
| **notes** (49 files, 8,704 lines) | `http/routes.hero:126-158` (`router_of`), **22** `add(@` lines (`grep -c '^    add(@'`, lines 128 to 152), a local `add` wrapping `web.add` (166), three groups (`account`, `admin`, `api`), the API's routes added by `openapi.add_routes` (155) and its site's by `openapi.add_site_routes` (157) | **35 `if name ==` lines** (`grep -rn 'if name ==' --include='*.hero'`, by file): `http/dispatch.hero:35` `page`, **20**; `api/notes_api.hero:73` `answer`, **8**; `api/accounts_api.hero:71` `answer`, **3**; `main.hero:108-114`, **4**, the command line's subcommands (one `code = if name ==`, three `else if name ==`), not HTTP. **The HTTP dispatch is 31 lines** (20 + 8 + 3; the critic's § 2, re-run by the repair). (The coordinator's count at 00:59 read 34, which no command the critic or the repair ran reproduces; the clerk's command reads 35.) `http/dispatch.hero` (107 lines, `wc -l`) holds **26** comparisons `name == routes.` (`grep -o | wc -l`), the 20 `if` lines, a second name on one of them, and 5 in `is_negotiated` (24-25) | `page(@app, v, found, r) -> replies.Reply?` calls handlers taking **`@app`** (`pages_notes.home(@app, v:, r:)`), **`app` by copy** (`note_new(app, ...)`, `register_form(app, v:)`), **no state** (`logout()`), and **extra parameters from the route** (`slug`, `tag`, `heading`, `name`) |
| **auth** (40, 7,322) | `server/routes.hero:23-45`, **22** `web.add` | **a `match name` on a string**, `server/routes.hero:75-97`, 21 arms for the 22 names (one arm joining two) and `_ => ok(http.respond_status(404))` (97); `answer(@st, rt, r)` at 55 routes and calls it | `@st` (`accounts_api.register(@st, r:)`), `st` by copy (`oauth_api.metadata(st)`, `userinfo(st, r:)`), none (`login_page(r)`); results `http.Response?` and `http.Response` mixed, each arm wrapping its own in `ok(...)` |
| **registry** (39, 6,660) | `server/api.hero:52-72`, **18** `web.add` calls (55 to 72; a bare `grep -c 'web\.add'` reads 19, line 49 being a comment) | `match found.name`, `server/api.hero:105-120`, 14 arms and `_ => guarded(...)`; **`guarded` (137) matches the name again for a scope** (`:138`) **and three `if found.name ==`** (`:147`, `:154`, `:157`), a publication rate-limited first | `@state` mostly, `state` by copy (`reads.health(state)`, `reads.suggest(state, r:)`); **route data beside the handler**: a scope per writing route, a limiter for one. (Registry's two `if name ==` lines, which the coordinator's count at 00:59 read as dispatch, are not: `packages/names.hero:81`, `packages/unpack.hero:179`.) |
| **searchd** (34, 6,795) | `server/service.hero:32-43`, **10** `web.add` | `match found.name`, `server/service.hero:65-76`, 10 arms and `_ => web.not_routed`; `answer(s: Service, r)` (58) is `http.serve_until_stopped`'s handler (`app/commands.hero:98`), **state by value** | `ui.home(s.engine)`, `api.search(s.engine, r:)`: the engine **by copy**, never written |
| **chat** (37, 8,275) | `server/api.hero:45-53`, **5** `web.add` | **split in two**: `server/serving.hero:144-155`, three `if found.name ==` (socket, register, login), then `api.answer` (`server/api.hero:100-106`), `match found.name`, 2 arms and `_` | **`open_socket(@srv, set: set, @reader, r: r)`: two `@` places**, the server and the connection's reader; `begin_sign_in(@srv, stream:, r:, begun: api.begin_register(@srv, r:))` |
| **reach** (213, 6,015; probes of the library) | `reach_web.hero:30-33`, **3** `web.add` | `app(router, r)` (`:38-52`), `match found.name`, 2 arms and `_` (48); `handle(@state, found, r)` (`:56-61`), `match found.name`, 1 arm and `_ => app(state.router, r)` (61); `serve_conn(@state, stream)` (`:66`), the loop of a program with state; `reach_openapi.hero:66-75`, `openapi.is_site_route(found.name)` | the library's two shapes, stateless and stateful, side by side |

**What the proposal writes for each** is § 6's sketch applied: each route bound at its `add` to the function that
answers it, the dispatch chain deleted, and the handler's shape made uniform. Three facts each rewrite meets, all
measured above: the handlers take the state three ways (`@`, a copy, none), so a uniform `@` type meets
`never_rebound` (question 5); some take values from the route (`slug`, `name`, `version`, `id`, `digest`), which a
uniform type hands as the `Found`; and registry and chat carry per-route data (a scope, a limiter) or a second place
(`@reader`), which a uniform handler type must carry or the route must hold.

### 5.4 The programs that serve without the router (the coordinator's list, gateway first)

| program | how it finds the answer (`path:line`) | what the proposal could write |
|---|---|---|
| **gateway** (36 files, 7,291 lines) | **Routes from a file, not from code**: `route/routes.hero:75` `route(table: Table lent, host, target)`, the longest prefix of `host + path` that ends a segment, in a prefix tree (`containers/trie`, `table_of` 46, `longest` 102), host `*` the fallback; **the answer chosen by a `match` on a variant**, `route/pipeline.hero:148` `dispatched`, `match routed.route.kind` over `config/settings.hero:83` `variant Kind` (`proxy`, `grpc`, `transcode` with `method` and `field`, `graphql`, `metrics`), and again for gRPC calls at `listen/calls.hero:38`; the upstream found by name, a linear scan (`config/settings.hero:198`, `core/state.hero:104`); the requests reach the state through **one owner thread** that `match`es a `variant Ask` (`core/owner.hero:35`, `:122`), workers asking by mailbox; handlers take `@gw`, and gRPC's `relay.relay(@gw, @h2, ...)` **two places** | the variant `match` is exhaustive today (a new `Kind` that no arm answers is a compile error); a table of function values per kind is not. **Whether the gateway's own dispatch should move at all is the sitting's question** (§ 7 (e)). Its stand-in upstreams hand-match paths: `standins/catalog.hero:139-160` (`if path == "/v1/items" && r.method == http.METHOD_GET`, a `has_prefix` of `/v1/items/`, `/v1/orders` POST), `standins/reviews.hero:122` (`/graphql` alone): **router users under the proposal** |
| **certs** (37, 7,273) | `challenges/responder.hero:76-90`, `answer(folder, r)` under `http.serve_until_stopped` (`:40`), a method check then a `has_prefix` of the well-known path; `serve/https.hero:136`, `answer(p: Page, r)`, `if r.url.path != "/"`, under `serve_tls_with` (`:85`); its stand-in CA, `standins/ca.hero:184-195` `route(@s, a, r)`, `if target == "/roots/0" && r.method == http.METHOD_GET`, `/manage/clock` POST, then `acme_route` (`:200` on), seven more hand-matched paths under a method split (`/directory`, `/acme/new-nonce`, a `has_prefix` of `/acme/renewal-info/`, `/acme/issuer/1`, `/acme/new-account`, `/acme/new-order`, `/acme/revoke-cert`) and a split of the rest by `/` (`sed -n 200,240p`) | the CA's nine paths and its split a router with `@s` handlers; the two one-path answers stay as they are |
| **worker** (49, 9,665) | `ops/dashboard.hero:121`, `if r.url.path == "/metrics"`; its stand-in collector, `standins/collector/server.hero:131` `answer_of(@c, r)`, three paths checked by `!=` then `if path == "/v1/traces"` / `else if "/v1/metrics"`; **its Redis stand-in's command table**, `standins/redis/server.hero:234`, `match name` over `HELLO`, `AUTH`, `QUIT`, `SHUTDOWN`, `SELECT`, `CLIENT`, `INFO`, `STANDIN.CLOCK` and then `scripting.execute(@s.ks, @s.scripts, ...)` or `commands.execute(@s.ks, ...)` (`standins/redis/commands.hero:56`, `match name` again at 63) — handlers taking **`@session`, `@s.ks` and `@s.scripts`**; the command line, `cli/command_line.hero:194` | a router for the collector; **a table of command handlers** for the Redis stand-in (the llm task t2's shape) |
| **ingest** (65, 14,424) | `ops/exporter.hero:84`, `if r.url.path == "/metrics"`; its S3 stand-in, `standins/s3_service.hero:110` `handled(@store, r)`, the path split by `/` (`:124`), then `match r.method` (`:146`) over `PUT`, `GET`, `HEAD`, `DELETE` | a router of two patterns, `/:bucket` and `/:bucket/*key`, handlers per method |
| **backup** (46, 8,379) | its S3 stand-in, `standins/s3_server.hero:108` `handled(@b, r)`, the same split (`:125`) then `s3_bucket.hero:88` `object_request(@b, r, key, now)`, `match r.method`; the command line, `cli/commands.hero:158-169`: **`if name == "demo"` at 158, then a `match name` at 160 to 169, eight arms by name and `_ => upkeep.copy(sub)`**, so `copy` answers every name the eight do not (the critic's § 2 item 1, `sed -n 150,175p`, re-run by the repair; the clerk's draft read *"`match name` over 8 subcommands and `_`"*) | the same router as ingest's; a command table, whose `_` arm is today's silent default |
| **agent** (42, 5,876; the critic's § 3, re-read by the repair) | **dispatches by name, and it is the one read-only table in Rebel's examples**: `tools/toolbox.hero:94-113`, `run(box: Toolbox lent, name: str, input: json.Value) -> outcomes.Outcome`, six `if name ==` for the six MCP tools, one guarded (`name == "run_command" && box.allowed.len() > 0`), every handler reading parts of `box`, then `outcomes.refused(...)` (113); served over standard streams by `mcp/server.hero:106` `serve_stdio`, a `jsonrpc`/`mcp` face (`jsonrpc.hero:8`, *"A server finds, the program calls"*); its stand-ins dispatch HTTP by path (`standins/openai_api.hero:138`, `:158`; `claude_api.hero:118`); its command line by six `if name ==` (`app/commands.hero:65-80`) | **question 5's driving program**: a table with no `@` at all, every member `Toolbox lent`, and per-entry data beside the handler (the guard): the `lent` half's case |
| **monitor** (41, 8,349) | `standins/collector.hero:141-199`, `answer_of(@c: Collector, r)`: a method check, a stop path (150), three OTLP paths (154, then 185 and 188), the shape of worker's collector | a router of four routes with `@c` handlers |
| **iothub** (41, 10,001) | one path, `feed/server.hero:135`, `/feed` upgraded to a WebSocket; its two `match name` (`config/registry.hero:119`, `:142`) parse a name into a variant, not dispatch | nothing to bind; read, not a driver |
| **server** (1, 313; in the census, named by neither list before the repair) | `server/main.hero:249-253`, `answer(request)`, one path `/hello`, no state, under `http.serve_conn` (239) | nothing to bind |

`agent`, `iothub`, `monitor` and `server` also `use <rebel>/http` (`ground/census.txt`) and were not named by the
coordinator; the clerk left the first three *unread*, and **the critic read them** (`grep -rnE` of `serve`, `listen`,
`if name ==`, `path ==`, `match name` in each, then `sed -n`), the repair re-reading each line named in the four rows
above. The census's `if name ==` and `path ==` columns (16 and 7, 2 and 3, 3 and 6, 0 and 2) count lines, not
dispatches.

## 6. The proposal as a spec diff, and the router's sketch

### 6.1 A first diff, for the seats to price and amend (not a candidate of the warden's; the warden writes its own)

```
-| `(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory |
+| `(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory; `@A` and `A lent` as in a signature |

-    TypeArg  = Type | ident ":" Type .
+    TypeArg  = "@" [ ident ":" ] Type | [ ident ":" ] Type [ "lent" ] .

-- Top-level functions are values, but not one with an `@` or `lent` parameter:
-  `xs.fold(0, add)`.
+- Top-level functions are values, typed by their signature with each mode:
+  `xs.fold(0, add)`; `h: (function(@App, Request) -> Response) = page`, called `h(@app, r)`.
```

Whether § 9 lines 314 to 319 need a word for a call through a value, what § 11 line 383 says of `spawn`, and what § 13
line 427 says of a callback are the warden's texts.

### 6.2 The router: three sketches, none compiled (the form does not parse today, § 3.3), each for the seats to build

**(A) The program keeps the table** (no generic record; the library changes by one field):

```
record App
    router: web.Router
    handlers: [(function(@App, web.Found, http.Request) -> http.Response?)]
    notes: {str: Note}

function routes(@app: App) -> ()?
    web.add(@app.router, method: http.METHOD_GET, pattern: "/notes/:slug", name: "note")?
    app.handlers @ app.handlers.push(note_page)
    return ok()

function answer(@app: App, r: http.Request) -> http.Response?
    found = web.route(app.router, r)?
    h = app.handlers[found.index]
    return h(@app, found, r)
```

(`web.Found` has no `index` today, `web.hero:301-311`; the router's own index is `routing.route_of`'s answer, read at
`:331` and dropped.) **Unbuilt beneath it** (the critic's § 3): sketch A indexes `app.handlers` by the router's own
index, while notes adds its routes into three groups, `mount`s them and lets `openapi.add_routes` add its own
(`notes/http/routes.hero:143-157`, `add_routes` at 155): whether the router's index stays aligned with a push order
across `mount` is a question for the seat that builds A; a name-keyed map (§ 7 (k)) does not meet it.

**(B) A generic face over the program's table**, the library holding no handler:

```
function get<S>(@r: web.Router, @table: [(function(@S, web.Found, http.Request) -> http.Response?)], pattern: str, h: (function(@S, web.Found, http.Request) -> http.Response?)) -> ()?
function dispatch<S>(r: web.Router, table: [(function(@S, web.Found, http.Request) -> http.Response?)], @state: S, req: http.Request) -> http.Response?
function serve_with<S>(s: http.Server, l: net.Listener, @state: S, handler: (function(@S, http.Request) -> http.Response)) -> ()?
```

**(C) The author's sketch, `web.get(@r, "/notes/:slug", note_page)`**, which needs `Router<S>` holding
`[(function(@S, Found, Request) -> Response?)]`: **a generic record, which panel 252's R3 left waiting** (question 4).

## 7. The routes nobody listed (each a question; say why it is or is not the robust one, and name one missing)

- **(a) A `handler` record kind**: a declared kind whose values are function values with their modes, nothing else.
- **(b) A state the router owns**: `web.serve_with<S>(@state: S, ...)` and the handler table in the library's generic
  functions (sketch B), the program's state never in a record the library declares.
- **(c) Closures**: refused, design.md Part 6 line 3644 (panel 119); its falsifier names *"a program on the §1.0 closure
  list, or a measured Part 11 effect, that a named top-level function cannot express"*.
- **(d) Generic records** (sketch C): panel 252's R3, *"waits"*, refused on Principle 0 before the 2026-11-22 amendment.
- **(e) A variant of routes, matched exhaustively**: the router answers a case of the program's own variant, and a
  `match` with no `_` makes a route nobody answers a compile error; it is the gateway's own form (`match
  routed.route.kind`), and today's name dispatch has a `_` arm in every `match` above that lets a misspelled name
  through (auth `server/routes.hero:97`, registry `server/api.hero:120`, searchd `server/service.hero:76`, chat
  `server/api.hero:106`, reach `reach_web.hero:48` and `:61`; `grep -n '_ *=>'` of each file, read against each range).
  Whether a misspelled name in an arm reaches the `_` silently is **unrun**: a seat's probe.
- **(f) The state passed and answered by value**: `(function(S, Request) -> Answer)` where `Answer` holds the new `S`;
  `http.serve_with`'s comment already names the copy it pays (§ 5.1). **It builds today**: the clerk wrote the blind
  seat's three tasks this way (`blind/reference-t1/` to `-t3/`: a router of four routes over a notes map, a table of six
  commands, a turnstile's four transitions, each bound in one table), and stage 2 checks, formats and runs each, exit
  0, printing its `expected.txt` (`blind/reference-results.txt`; the clerk's stage 2 at the first base, and the
  repair's at the base, three runs each, nine `cmp`-equal). What it costs (a copy of the state per call where a
  handler changes it, a change lost where a handler forgets to answer it back) is a seat's measurement, unrun.
- **(g) The gateway's owner thread**: the state on one thread, the handlers asking by mailbox (`core/owner.hero`).
- **(h) `lent` coerced to `@`**, or `never_rebound` waived for a function used as a value (question 5).

**Added by the critic's first reading** (its § 6; each probe named was re-run by the repair, each *unbuilt* is so):

- **(i) A handler that takes no state** (notes' `logout()`, auth's `login_page(r)`) in a table of `@` handlers: `@_:
  App` is `never_rebound`, `_: App lent` builds, a named unread slot is `unused_binding` (probes `c2` to `c5`,
  question 5): a uniform table has three kinds of member.
- **(j) The table as a top-level `constant`**: `constant TABLE: {str: (function(i64) -> i64)}` holding `{"d": double,
  "t": triple}` runs, printing `12` (`k3`), and a `[double]` list too (`k1`, `8`); a list of records holding handlers
  is refused, `constant_body` (`k2`), whose note says a constant's body is re-evaluated on every read, a cost per
  lookup. A command table can be one constant, listed once and never built at run time.
- **(k) A name-keyed map beside the router**: `{str: (function(@App, web.Found, http.Request) -> http.Response?)}`
  keyed by `found.name` needs no field in `web.Found` (sketch A needs `index`); unbuilt.
- **(l) The callee and the state in disjoint places**: `server.handlers[i](@server.state, r)` reads the callee from a
  field the call does not lend, so question 2 does not arise; unbuilt.
- **(m) `lent` adapted to a by-value type** (panel 217's spec-warden, item 6, *"an adapter passing the argument's
  address is sound"*): a third conversion beside question 5's `lent` to `@`; unbuilt.
- **(n) The state lent read-only and the changes answered as data**: each handler takes `App lent` and answers a list
  of a variant of changes that one function applies by `@`; distinct from (f), which answers the whole state;
  unbuilt.
- **(o) The compiler's own knot through a value**: panel 109's `@b` walk, panel 121's grammar module, panel 138's
  sub-parser, and Rebel's `interpreter/run/value.hero:8-13`. No seat was asked to build one; it is the only route by
  which the form is compiler-need (CLAUDE.md § 2), and the compiler-engineer is now asked to (`compiler-engineer.md`).
- **(p) A callback C runs on another thread**: the 18 thread bodies of question 7 are function values C calls on a
  thread of its own; a mode admitted in an `extern` callback's type and handed to `hero_thread_spawn` would hand one
  thread's place to another (the critic's reading of those declarations, unrun), so questions 7 and 8 are one question
  there.

**Shapes no brief named before the repair** (the critic's § 6, each unbuilt): a mode-carrying value sent through a
mailbox; a function-typed `@` parameter whose type carries modes, `(function(@(function(@App) -> ())) -> ())`; a mode
in a generic parameter's position, `(function(@T) -> ())` bound by `bind` (question 1's first case); the `T?` of such
a value called after `.must()` (`f3`'s shape with modes); an empty table `[]` typed by context; `==` on two such values
(`e1` today: `false`, `true`); and the library faces that take a function besides `web` and `http` (§ 5.1).

**The program of § 5 the briefs read least, before the repair, was agent** (§ 5.4): question 5's driving program with
no `@` at all, and the `lent` half's.

## 8. What each seat runs, and where

- **Its copy**: `.claude/worktrees/261-<seat>` (made by the coordinator, detached at the base). **Build there**:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then **stage 2**, `./heroes build selfhost/main.hero -o
  heroes-next`, and judge with `heroes-next` (`.claude/rules/verification.md` § The compiler that judges is a build
  artifact: the seed's binary is older than `selfhost/`, § 3.6's STALE line is the proof). The clerk's seed build read
  `real 54.67 user 20.93` at load 93 and its stage 2 `real 543.27 user 232.08 sys 59.28` at load 120 to 150
  (`/usr/bin/time -p`) at the first base; **the repair's, at the base `df92f9281`** (`panel261-repair-scratch/build.log`,
  09:45 to 09:56, load 13 to 18): seed `real 29.23 user 21.70`, stage 2 `real 342.63 user 227.43 sys 63.09`, stage 3
  (stage 2 building `selfhost/` again) `real 295.99 user 187.67 sys 40.06`, every one waiting, so none is a duration.
  **Stage 2's and stage 3's `--version` both read `sources 4dcf3a9f20257573`** at the base (the first base's stage 2
  read `30e08049445a2331`), which a seat's own stage 2 at the base should read too; say if it does not.
- **A route that does not build is not adopted** (§ 3c): the compiler-engineer builds the route it would adopt in its
  copy and runs the router sketch on it; the ffi-pragmatist builds the C of a callback with an `@` slot.
- **A program that may not end** (a server) is built with `heroes build` and its binary run under `timeout`, its output
  bounded (`.claude/rules/verification.md` § A run that may not end).
- **Reports** go to `docs/panel/261-reports/<seat>.md` in the seat's copy, written as it goes, and are copied to the
  trunk's same path by the coordinator; programs a seat keeps go to `docs/panel/261-reports/programs/<seat>/`.

## 9. The ground (`ls ground/`)

`census.sh` and `census.txt` (§ 5.2; re-run by the repair at the base, byte for byte the same, `diff`);
`callbacks.sh` and `callbacks.txt` (§ 2 question 7, the 18 function-typed parameters inside `group` and `extern`
blocks, the repair); `probes.sh` (the repair: every probe below re-run from a copy's root, `sh ground/probes.sh
<compiler> <this folder's probe/> <an output folder>`); `probe/` the clerk's probes `p1` to `p4`, `f1` to `f7`, `u1`
and `f7.c` (§ 3.3, § 3.4) and their outputs under the clerk's stage 2 at the first base (`*.next.out`, `f-next.out`,
`f7-next.c`); **the critic's probes**, copied under short names, `q2a`, `q2b`, `c1` to `c8`, `e1`, `g1`, `g2`, `k1` to
`k3`, `s1`, `s2`, `o1`, and **the repair's** `x1`; and **every probe's output under the repair's stage 2 at the base**
(`*.repair.out`, `x1.permissive.out`, `q2b.c`). **`p1`, `p4`, `o1`, `g1` and `g2` are kept as `.hero.txt`**: they do
not parse, by design, and a commit's hook refuses a `.hero` file that does not parse (`.claude/hooks/staged.py`);
`probes.sh` copies each back to `.hero` in its output folder to run it. Every other `.hero` here and under `blind/` is
canonical (`heroes-next fmt <file> | cmp - <file>`, the clerk at 01:22, the repair at 09:59 with its own stage 2). The
blind seat's tasks, references, registration and its runner's registration are `blind/`.
