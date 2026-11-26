# Panel 261, the compiler-engineer's brief

Read `00-shared.md` first, whole (§ 1 the proposal, § 2 the questions, § 3 the language and the record, § 4 the
compiler's map, § 5 the driving programs, § 6 the sketches, § 7 the routes nobody listed), then the critic's first
reading (`docs/panel/261-reports/critic-briefs.md`), **on which these briefs were repaired** before any seat started. **Your agent file is the
base's** (`.claude/agents/compiler-engineer.md` of your copy); design.md is reached by `grep -n`, never from memory.
Under the author's amendment of 2026-11-22 the most robust route wins at any cost in compiler size; **your veto on
soundness stands**, and what you count is cost to be written, never a reason by itself.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/261-compiler-engineer`, a detached worktree at
the base `df92f9281` (the coordinator makes it; `00-shared.md` names the base). Build there: `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build selfhost/main.hero -o heroes-next`; `./heroes-next
--version` should read `sources 4dcf3a9f20257573`, the repair's stage 2 and stage 3 at the base (`00-shared.md` § 8);
say if it does not. Your edited compiler is
a third binary, `heroes-261`, built from your edited `selfhost/` by `heroes-next`; never the trunk's `./heroes`, never
another seat's. **No paid run, no credential printed, `gh` reads only, nothing outside the repository's root, no
`pkill`, no commit.** Write your report as you go to `docs/panel/261-reports/compiler-engineer.md` in your copy, your
programs under `docs/panel/261-reports/programs/compiler-engineer/`.

## The map (`00-shared.md` § 4 has the table with line counts; re-read each before you cite it)

- **The type's parser**: `selfhost/parse/type.hero` (671 lines): `function_params` (286) reads a parameter type and an
  optional name (`named_parameter`, 335); `lent` is refused at 321 and 325 (`expected_function_type_params_close`, probe
  `p4`); `@` never reaches it (`parse_type` answers `expected_type`, probe `p1`).
- **The type**: `selfhost/check/table.hero` (559), `variant Ty`'s `function_ty` (71: `params`, `labels`, `result`),
  its key (157), `ty_key` (142), `intern` (250), `intern_function` (270), its one constructor (275). `labels` is a
  parallel run beside `params`, one entry per parameter, **the precedent a mode run would follow** (panel 129's names
  in the type; design.md §4.9's defect 026), **relations included**: names have a one-way fit (`check/type_fit.hero`,
  `fits` at 37, its comment 13 to 36) and a generic agreement check (`check/generics.hero:118`, `named` at 281;
  design.md §4.9 lines 1761 to 1763), `00-shared.md` § 1 item 1.
- **Where a mode can be lost** (`00-shared.md` § 2 question 1, the critic's § 2 item 4, re-run by the repair at the
  base): **at least ten sites, not panel 112's two**. Rebuilt from parameters, labels and result:
  `selfhost/ir/mono.hero:299`, `check/generics.hero:326`, `check/type_fit.hero:81`, `check/lower.hero:66`,
  `check/fixed_repairs.hero:61`, `check/function_value.hero:78` and `:240`; related without comparing ids:
  `type_fit.fits` (37) and `generics.bind` (31, its function-type arm 96 to 130); keyed by a rendering: `emit/synth.hero:35` `content_key` over
  `check/render.hero` (229 lines, its `function_ty` arm at 92), and `synth.hero:47` `instance_key`. **Build first the
  case the critic infers and nobody ran**: a generic `(function(T) -> ())` bound by `bind` to a value of
  `(function(@App) -> ())`, which would hide the `@` unless `bind` compares modes. And defect 2650 (two function types
  from two modules, one typedef, clang refusing at exit 2) is the class of R6's silent sites that happened since.
- **The labels run's measured cost**, the nearest precedent to price yours against: `2724a4f9a` (2026-09-11, `git
  show --stat`), **48 files, 16 of `selfhost/`**; its body (`git show -s --format=%B`): *".function_ty is CONSTRUCTED
  at one site and its 192 other mentions are match arms"*, *"generics were free"* (which for a mode is the silence,
  not the saving), and *"The canonical key omits the names' share when a type has none"*, nine of 441 emission traces
  moving by a typedef's name alone: say whether your key keeps a mode-free program's bytes the same way.
- **A name as a value**: `selfhost/check/function_value.hero` (350), `signature_of`; the refusals in
  `selfhost/check/markers.hero` (109): `refused_as_value` (64) and `through_a_value` (107).
- **A call through a value**: `selfhost/check/walk.hero:1296` `indirect_call` (the walk is 2,486 lines; its ceiling is
  `tests/harness/suite_layout.hero`'s `DECIDED`, mirrored by `.claude/hooks/ceiling.py`); a call by name's marks at
  1678.
- **The control arm**: `selfhost/ir/flatten.hero:608` `value_call` is also where `build --permissive` gives a
  missing or extra `@` through a value its meaning (`.claude/rules/verification.md`'s map;
  `tests/golden/control/marker_mismatch-extra-at`, `-missing-at`, `-non-place`, `never_rebound`). **At the base an
  extra `@` through a value exits 2 under `build --permissive`** (`00-shared.md` § 2 question 6, probe `x1`): the
  lowering hands `&h1_x` to a by-value slot.
- **Exclusivity**: `selfhost/resolve/writes.hero` (`refuse_aliased_places`, 260) over
  `selfhost/resolve/aliased_places.hero`'s tree; `lent` beside an `@`, nested included:
  `selfhost/check/read_lends.hero` (223). Both read a callee's **declaration** for its marks (`read_lend.marked_at`,
  `read_lend.lent_at`, `selfhost/ir/flatten.hero:700-724`): a value has none, so where they ask a type instead is your
  route's core.
- **The lowering**: `selfhost/ir/flatten.hero:608` `value_call` lowers every argument as a value (*"no declaration is
  known, so no parameter's mark is read"*); the IR already has `inout_arg(place, read_only)` (`selfhost/ir.hero:57-62`)
  and `Callee.indirect` (79-81); `selfhost/ir/read_only.hero` (85) verifies a read-only slot.
- **The emitter**: `selfhost/emit/types.hero:155` `function_type`, the value's typedef (today `typedef int64_t
  (*h_0fn_32a9823f)(h_f7_App, int64_t);`, `00-shared.md` § 3.4 `f7`); `selfhost/emit/signature.hero` (`one_param`,
  260: an `@` slot is `T *`; `params_as`, 102, the value's convention; a library function's `hero_value_` twin);
  `selfhost/emit/func_ref.hero` (106); `selfhost/emit/ctype.hero:398`; `selfhost/emit/synth.hero:35`
  `content_key`; the callback thunk `selfhost/emit/callback_thunk.hero` (429).
- **Threads**: `selfhost/check/thread_ops.hero` (255), `spawn`'s check.
- **`heroes doc` and its reader**: `selfhost/cli/doc.hero` (330), `doc_shape.hero` (369), `doc_text.hero` (323), and
  `rebel/generators/rebel_index/` (`packages_text.hero`, `faces.hero`), which reads what `heroes doc` prints and broke
  once on a generic function's signature (defect 2761): `00-shared.md` § 2 question 10.
- **The count to beat**: `function_ty` is named on 374 lines in 141 files (`00-shared.md` § 4, the same at the new
  base). A field on the existing case (panel 112's R6: *"2 edits in 1 file"*, carried) against a new case (*"173
  `non_exhaustive` errors across 49 files"* in 2026-09, carried): **count both at the base**, by building them.

## What you build, in your copy, and run

1. **The route you would adopt**, end to end: the grammar (`TypeArg` with `@` and `lent`, names beside them, `@p: T
   lent` refused as § 4's `Param` refuses it, or say why not); the mode in the type and in both keys (`ty_key`,
   `content_key`), with **the test panel 112's R6 owes**: two function types differing only in a mode intern to two
   ids and emit two typedefs; the value (a top-level function with modes, a generic one through `bind_missing`, a
   library function through its twin); the call through a value (its marks, arity, labels, `marker_mismatch`'s new
   reading); exclusivity and the nested-`lent` rule asked of a callee's **type**; the lowering of `@` and `lent`
   arguments through `value_call`; the typedef's pointer slots; the diagnostics of `00-shared.md` § 2 question 6.
2. **The router on it**: sketches (A) and (B) of `00-shared.md` § 6.2 written as programs and **run** (a loop over a
   fixed list of requests made by `http.request(method:, target:)`, no network; or `web.route` over a handler table),
   and one of § 5.3's programs rewritten whole on the route you adopt, reach's `reach_web.hero` the smallest, and
   notes' `http/dispatch.hero` the largest, built and run with its `main.expected` where it has one (sketch A meets
   notes' three groups, their `mount` and `openapi.add_routes`, `notes/http/routes.hero:143-157`: say whether the
   router's index stays aligned with the handlers' push order across `mount`, or build the name-keyed map of
   `00-shared.md` § 7 (k) instead). Sketch (C) needs a generic record: say what it would cost on top (panel 252's R3,
   *"288 incomplete matches in 109 files (the critic's re-count of the compiler-engineer's 290 in 111, on the frozen
   `selfhost/` alone)"*, `docs/panel/252-*.md:567-570`, carried; its seat's first prediction, *"at least 111 files"*,
   `:650`, is the count the sitting corrected), and note that **a generic variant is refused too** (probes `g1`, `g2`:
   `record Stack<T>` and `variant Maybe<T>` are each `expected_end_of_line`).
3. **The cases**, each built under the base's `heroes-next` and under `heroes-261`, its exit and output written:
   - question 2: `app.handler(@app, r)`, `app.routes[i].handler(@app, r)`, `state.router.handlers[i](@state, r)`, and
     the same with the callee bound first, `h = app.handler` then `h(@app, r)`; whether they differ, and whether one
     can write through the lent place while the callee value is read from it (print the place before and after);
   - question 5: a read-only handler in a table of `@` handlers (`never_rebound`, probe `u1`), and each route of
     question 5 built (a conversion from `lent` to `@`, the waiver, two tables), with what each refuses; **the
     no-state handler** beside it (`@_: App` is `never_rebound`, `_: App lent` builds, a named unread slot is
     `unused_binding`: probes `c2` to `c5`, § 7 (i)); **the self-write** that passes `never_rebound` today (`app @ app`,
     `app.n @ app.n`, probes `c1`, `c8`): does your route keep it a write, and should it; **agent's tool table**
     (`rebel/examples/agent/tools/toolbox.hero:94-113`, every handler `Toolbox lent`, one guarded entry) written on
     your route, question 5's driving program; and **`lent` adapted to a by-value type** (§ 7 (m), panel 217's
     spec-warden item 6);
   - the field call against UFCS, probe `f4` (`b.f(10)` takes the field silently today): does a mode change it? **The
     field first is panel 079's ruling and design.md §4.11 line 1949's, with the collision clause refused** (`00-shared.md`
     § 2 question 2), and the spec does not state it: your route stands on a rule a spec reader cannot see;
   - question 2's nested half, **which the base already answers** (probes `q2a`, `q2b`: the callee is read before a
     nested `@` call writes it, prints `10`): start from it, and build `server.handlers[i](@server.state, r)`, the
     callee and the state in disjoint places (`00-shared.md` § 7 (l));
   - an `@` handler stored in a map and called after `.must()`; an array of them grown in a loop; `==` on two;
   - two `@` arguments of a call through a value reaching one place (`aliased_mutable_arguments` today for a call by
     name, design.md §4.8 lines 1587 to 1621), and a `lent` argument beside an `@` of its place, nested included;
   - `spawn(f, x)` and `spawn_sized` with `f` taking `@` or `lent` (question 8; refused today as the value's
     refusal, probes `s1`, `s2`), and a mode-carrying value sent through a mailbox;
   - **the control arm**: `build --permissive` of a call through a value missing its `@` and adding one, on your
     route, and at the base (probe `x1`, exit 2 today), with the meaning `tests/golden/control/` pins for a call by
     name; and whether the `control` and `permissive` forms move;
   - **the routes and shapes of `00-shared.md` § 7 (i) to (p)**, each built or said why not: the constant table
     (`k1`, `k3` run today; `k2`'s list of records is `constant_body`), the name-keyed map, disjoint places, changes
     answered as data, and **the compiler's own knot through a value** (§ 7 (o): panel 109's `@b` walk, panel 121's
     `grammar_expr.hero`, panel 138's sub-parser, Rebel's `interpreter/run/value.hero:8-13`), written on your route as
     the one case that makes the form compiler-need; a function-typed `@` parameter whose type carries modes; a mode
     in a generic parameter's position bound by `bind`; an empty table `[]` typed by context;
   - a generic `function each<S>(@s: S, hs: [(function(@S, i64) -> ())])` instantiated twice;
   - a function value with modes crossing a module (`use`), and one of Rebel's library, whose C takes `const HeroPlace
     *hero_caller` last (`selfhost/emit/signature.hero:326-342`'s test, whose twin already writes a pointer slot for
     an `@` parameter, `hero_value_h_library_dist(int64_t h0_a, int64_t *ph1_b)`), and a large group record through a
     value, which keeps C's convention through a twin (panel 236, `selfhost/emit/signature.hero:99-102`);
   - **one shape beside each that nobody named** (CLAUDE.md § RUN IT: none, one, nested, generic, empty).
4. **What it costs and what it moves**: lines per module (and every `DECIDED` ceiling it crosses, by
   `.claude/hooks/ceiling.py` or `layout` narrowed); the compiler's own tests (`./heroes-261 test selfhost/main.hero`);
   the goldens of `00-shared.md` § 3.3 that flip, each named; **byte-identity**: `./heroes-next build selfhost/main.hero
   --emit-c` against `./heroes-261 build selfhost/main.hero --emit-c`, each with the line `hero_program_sources` taken
   out (`.claude/rules/verification.md` § The formatter's probe, its restatement of 2026-10-17), and the same over a
   handful of `tests/golden/run/` programs that use function values (`grep -l '(function('`), by `cmp`; the seed's
   fixpoint one generation further if your route reaches the seed's own C. Time nothing as a verdict while the machine
   is loaded: `uptime` beside any number you write.
5. **The re-printing tools** (`00-shared.md` § 2 question 10): each round trip run (`heroes fmt` on a file holding the
   form, `--dump-ast`, `heroes probe` over a fixture), or named as owed with the file it lands in.

## What you judge

`verdict` (approve, object, veto) per route of `00-shared.md` § 6.2 and § 7, with `section` (design.md §1.7, Part 5),
`cost` (lines, files, ceilings, goldens), `prediction` (one falsifiable sentence, with the milestone that checks it)
and `condition` (what would change your mind). Name the route you would land, and **build it before you name it** (§ 3c:
*a route that does not build is not adopted*). Say what a seat's soundness veto would compel: a mode that a call through
a value can hide, a place written by two `@` routes at once, a value whose C type differs from the function's.
