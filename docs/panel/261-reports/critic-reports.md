# Panel 261, the completeness critic: second reading, the reports

Written 2026-11-26 from 11:30 by the completeness critic, a fresh agent, after the four seats reported and before the
synthesis. My copy is the detached worktree `.claude/worktrees/261-critic` at `df92f9281` (`git log --oneline -1`,
11:30); my scratch is `.claude/worktrees/261-critic-scratch/r2/`. I read `261-briefs/` as repaired, my first reading
(`critic-briefs.md`), the four reports and the programs the compiler-engineer and the ffi-pragmatist kept. Every
sentence below was settled by the command beside it, run by me in this session, or says *unrun* or *a question*. I give
no verdict on any route. The machine read load 7 to 21 and 5.2 of 6.1 GB of swap in use at 11:30 (`uptime`, `sysctl
vm.swapusage`), so no time below is a duration.

## 0. My compilers

- **The base**: in my copy, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` then `./heroes build
  selfhost/main.hero -o heroes-next`, 11:31 to 11:36, exit 0 each; `./heroes-next --version` reads **`sources
  4dcf3a9f20257573`**, the repair's and the three seats' key (`r2/build.log`).
- **The route**: `git archive df92f9281` extracted into `r2/route/`, `route-v4.diff` applied by `patch -p1` (25 files,
  `r2/patch.log`), the seed built there and stage 2 built from the patched `selfhost/` (`r2/route-build.log`). **A
  trap for whoever re-runs this**: my first attempt applied the diff with `git apply` from inside `r2/route/`, which
  is inside the main checkout's tree; `git apply` there resolves the patch's paths against the main checkout's root and
  **ignores them all silently, exit 0** (git's documented behaviour for paths outside the current directory), so the
  first "route" built was the base. Caught by `cmp` of `selfhost/check/walk.hero` against the base (same) and a missing
  `resolve/value_calls.hero`; the main checkout was not touched (`git -C <main> status --short -- selfhost`, empty).
- **The seat's own binary** `heroes-261-v4` is run by its absolute path, never rebuilt in its copy.
- **My route build is not byte for byte the seat's binary**: `./heroes-route --version` reads **`sources
  7cc4a75a5a7c6bf1`**, `heroes-261-v4` reads `1283d3066f568534`. The compiler-engineer says why (`compiler-engineer.md`
  § 1): the diff is v4's source **plus** the R6 unit test added after v4 was built, and the key covers every byte of
  the sources. That the difference is a `test` block alone is the seat's word, not my measurement (I may not read its
  copy); § 8.1 compares the two binaries' answers case by case instead.
- **The base's own tests are red at `df92f9281`, and not because of the route**: `./heroes-next test
  selfhost/main.hero --only "the list holds every word the runtime"` in my unedited copy: **`1 test, 1 failed`**,
  *"left: `x y`"* (`r2/selftest-only.log`), the words of `hero_f64_rem(double x, double y)` that step 191a
  (`4b897f078`, 01:22) wrote into `runtime/heroes_runtime.h`. So the compiler-engineer's *"2325 tests, 1 failed"* on
  both compilers is the base's. **Repaired on `main` since** by step 191c (`1ac3f57f1`, 10:53, *"the runtime's
  remainder is declared with its parameters unnamed"*; `git log df92f9281..main`). `main` stands at `a60dfb77f`, 15
  commits past the base, and **none of the route's 25 files moved** (`git diff --stat df92f9281 main --` the 25 paths:
  empty), nor the spec or design.md.

## 1. The finding that decides the most: the route the compiler-engineer would adopt meets the ffi-pragmatist's veto

The compiler-engineer writes that in its route *"an `extern` keeps both refusals"* and that *"a mode in an `extern`
callback's type is the ffi seat's"* (`compiler-engineer.md` § 1, § 7 (p), § 10). The first is true of an extern
function **used as a value**; it is not true of an extern's **callback parameter**. I ran the ffi-pragmatist's own
shapes, written as the proposal lets a program write them (no hand-edited C), under the seat's built compiler
`heroes-261-v4` (`sources 1283d3066f568534`), by its absolute path, in `261-critic-scratch/r2/ffi/shapes/`:

- `v4_shapes.hero` (canonical by `fmt | cmp`): `extern "shapes.h"` declaring `twice(cb: (function(@a: i64, @b: i64)
  -> i64))`, `hand_over(cb: (function(@h: Node, k: i32) -> i32), k: i32)` and `fill_big(cb: (function(@b: Big) ->
  i32))`, handed the program's `both`, `take_copy` and `grow` (the ffi-pragmatist's bodies). **`check` exit 0, `build`
  exit 0, the binary prints `40040`, `4702111234474983745`, `42`, exit 0.** Under `--sanitize`: exit 134,
  **`heap-use-after-free ... WRITE of size 8 ... in hand_over shapes.h:34`**, the block freed by `node_free`
  (`v4_shapes.san.run.out`). The emitted thunk (`v4_shapes.c:325-327`) forwards `ph0_h` unchecked:
  `int32_t hero_result = h_v4shapes_take_copy(ph0_h, h1_k);`.
- So **`heroes-261-v4` is the landing the ffi-pragmatist vetoes** (*"I veto any landing that lets C call an `@` or
  `lent` function through the thunk the base writes"*, `ffi-pragmatist.md` § 9), measured on a compiler and not on
  edited C: C writes 64 bytes into freed memory at exit 0, and `twice` hands one pointer to two `@` slots (`40040`,
  design.md §4.8 lines 1620 and 1621's invariant broken for a callback). The two seats' verdicts are not reconciled
  by either report: the compiler-engineer approves (k) and (B) *"on the route I built"*, the ffi-pragmatist vetoes
  that route's C boundary. **The checkable side is the ffi-pragmatist's**, by the run above.
- And **the spec-warden's adopted text says the opposite of what the built route does at § 13**: `spec-w1-fit`
  writes *"its parameters follow the same rule, with neither mark"* (a mode in a callback type refused), while
  `heroes-261-v4` parses, checks and builds one. Only `spec-w1c` (*"an `@` or `lent` one the place C hands it"*)
  describes it, and the warden approves W1c *"only where the ffi-pragmatist builds the C of a callback's mark"*,
  which the ffi-pragmatist built by hand with four duties the route does not carry.
- **nftw itself** (`ffi/nftw/v4_fit.hero`, the callback's last slot `at: Ftw lent` handed where the type asks `@at`, the
  fit; and `v4_selfwrite.hero`, `@at` with `at @ at`): `check` exit 0 each, `build` exit 1 each,
  **`error[ffi_callback_type]`** at the extern, *"clang read the header, and it says `int (*)(const char *, const
  struct stat *, int, struct FTW *)`"*: v4 writes a `lent` group record's slot without `const` (the ffi-pragmatist's
  variant C), so nftw does not bind on the route, and its second note (*"a pointee the header marks `const` has no
  spelling here unless it is `const char *`"*) is printed by a compiler where `lent` is meant to be that spelling. The
  unmodified `nftw_proposal.hero` (an `@at` only read) is `never_rebound`, exit 1, as the ffi-pragmatist predicted.
- **My own build of the route answers the same** (`heroes-route`, `sources 7cc4a75a5a7c6bf1`): `v4_shapes.hero` builds
  and prints `40040`, `4702111234474983745`, `42`, exit 0; `v4_fit` and `v4_selfwrite` are `ffi_callback_type`.
- **What the synthesis must choose between**, each measurable: refuse a mode in an `extern` callback's type in the
  checker (W1's § 13 as written, the ffi-pragmatist's prediction (1) then failing: nftw keeps its shim), or carry the
  four duties of `ffi-pragmatist.md` § 7 in the thunk with a `run` case on three legs each (W1c), the `const` of a
  `lent` group record included. **The route as built does neither**, and no seat's report says so.

## 2. The repair of the briefs, checked against my first reading's nine items

Every number the repair says it re-ran at `df92f9281` I re-ran in my copy (11:33 to 11:45): the spec's 516 lines, 35,246
bytes and sha256 `3df14254...d12d93f`; design.md's 5,436 lines and each relocated line (1521, 1549, 1559, 1591, 1620,
1631, 1761, 1949, 2057, 2062, 2069, 2072, 2078, 3643, 3644, 4582, 4606, 4608, 4634, 4640, `sed -n`, each the text the
brief gives it); `generics.hero` 31, 118, 281, `type_fit.hero` 37 and 81, `render.hero` 229 lines and its arm at 92,
`print/fmt.hero` 1,948 and `dump.hero` 285 (`wc -l`); `tests/golden/control` 58 entries and `permissive` 44 (`ls | wc
-l`); the `full` form's 7 and 2 (`grep -rlE`); `web.hero`'s four `*_with` at 626, 647, 693 and 740, its 88 public
functions and `SIZE_MOST` 90. **All hold.** Item by item:

1. **The rebuild and relation sites**: carried into § 2 question 1 and § 4. Held, and **not enough**: the
   compiler-engineer found an eleventh and a twelfth site by building (`resolve/built_marks.hero:101-105`, every `@`
   through a value set aside before any rule; a field call over a same-named function), neither of which my reading
   named. My list was a reading of `intern_function(`'s callers, so it could not see a site that drops a **mark**
   rather than a **type**; the repair carried my list's vocabulary, and the soundness hole sat outside it.
2. **The names' analogy**: **the repair is wrong, and the error is mine.** My first reading's § 2 item 3 said, from
   feature 1133's body, that the fit lets *"an expected type that carries no run of names accept a value whose type
   has one"*; the repair copied it into `00-shared.md` § 1 item 1 and question 5 (*"the shape the names' one-way fit
   already has"*). The spec-warden found it runs **the other way** (`spec-warden.md` § 2), and I re-ran it: `n1_names`
   (`double(n: i64)` handed where `(function(x: i64) -> i64)` is asked) prints `8`, exit 0; `n2_names` (a value typed
   `(function(x: i64) -> i64)` handed where `(function(i64) -> i64)` is asked) is **`error[type_mismatch]`**, exit 1,
   under my base stage 2 and under `heroes-261-v4` alike (`r2/mine/`). `type_fit.hero:13-22` says so in words: *"a
   name that is not there fits whatever the expected type calls that position"*, the blank on the **value's** side.
   So the precedent the brief handed every seat for question 5 was inverted. It did not mislead the route: the
   lent-to-`@` fit the compiler-engineer built runs in the names' real direction (the value claims less than the slot
   grants). It does mislead the record: feature 1133's sentence is false at the base and nobody has filed that.
3. **The blind registration**: every one of my seven items is applied (`never_rebound` read by its message, `:75` and
   `:92` counting, `errors.hero:340` and `:357` not; the self-write row; the `now` arm graded by the base's stage 2, the
   landed answer beside it; rule (3)'s key-field walk named for t1, t2 and t3; D3 on t2 and t3 with t1 reported and
   why; every printed code; `brief-session.md` and `registration-runner.md` now written). What it still lacks is § 5
   below: **the codes the built route actually gives a mode mistake are not the ones the registration counts.**
4. **Panel 079 and the field first**: in § 2 question 2, § 3.2 and § 3.5. Held; the spec-warden prices the sentence
   (+19 vendored) and every text of its carries it.
5. **The control arm**: in § 3.3 and question 6, with the repair's own probe `x1` (an exit 2 at the base). Held; the
   compiler-engineer built `c1` and `m05` under `build --permissive`. Whether the repair's `x1` was filed: `gh issue
   list --state all --search` for *permissive by-value slot*, *incompatible pointer to integer conversion*, *build
   --permissive marker_mismatch*, and for the compiler-engineer's typedef cycle *function typedef ordered before
   struct*, *record holding a function value of itself* (11:47): nothing on point (2151 is the control arm's landing,
   1986 a `@_` defect, the rest older C-boundary defects). **A question, not a finding of absence: both exit 2s of the
   base read as unfiled** by these words.
6. **The sittings and issues**: all ten entries are in § 3.5 at the lines named. Held.
7. **agent, monitor, iothub, server**: in § 5.4. Held; the compiler-engineer rewrote agent's table.
8. **§ 6's routes into § 7**: (i) to (p). Held; the compiler-engineer built (i), (j), (k), (l), (m) and (o), and (n),
   the changes answered as data, is **built by nobody** (`grep -n "(n)"` of the four reports: the ffi-pragmatist's
   *"no objection"* alone).
9. **The small corrections**: question 7's code (held), backup's range (held), panel 252's count (held), notes' 31
   (held), `heroes doc` and `rebel_index` (held), the historian's glob (`historian.md` brief: I did not re-read it
   whole; the historian cites no panel file, so the glob no longer matters). **My golden count**: my first reading
   said `marker_mismatch` is on *89* files of `tests/golden`; the repair could not reproduce it, and neither can I:
   `grep -rl marker_mismatch tests/golden | wc -l` and `git grep -l marker_mismatch HEAD -- tests/golden | wc -l`
   both print **73** in my copy at the base. My 89 is withdrawn; the repair's 73 and its 10 for the three codes
   (`grep -rlE ... | wc -l`, 10) hold.

## 3. Three shapes of my own beside question 2, under the base and under the route

Each in `r2/mine/`, canonical by `heroes-261-v4 fmt | cmp` (the base cannot parse them), run by `heroes-261-v4` and by
my `heroes-route`; at the base each is refused where the type writes its mode (`expected_type` at the `@`,
`expected_function_type_params_close` at a `lent`, exit 1 each), so beside each I ran its nearest mode-free twin or
its call by name under both compilers.

| shape | what it writes | base | route (v4) |
|---|---|---|---|
| `s1_map_callee` (a map, a `.must()`, the storage freed under the running callee) | `app.hs["k"].must()(@app, 5)`, the handler writing `a.hs @ {}` first, then `app.hs["k"]` read again, then refilled with `triple` and called | twin `b1_map_callee_base`, `app.hs["k"].must()(wipe(@app))`: `10`, `gone`, exit 0 | **`10`, `gone`, `12`, exit 0; under `--sanitize` the same, exit 0**: the callee is copied out of the map before the lend, so emptying the map under it frees nothing it runs from |
| `s2_lent_callee_nested` (the exclusivity sentence's nested-`lent` clause, the callee from the place) | `app.readers[0](app, bump(@app))`, the reader typed `(function(App lent, i64) -> i64)` | by name, `peek(app, bump(@app))` (`b2_by_name`): **`aliased_mutable_arguments`**, the nested-`lent` message, exit 1 | **the same code and message**, at the same column, exit 1 |
| `s2b_at_callee_nested` (the same through `@`, and a reader bound in the `@` table by the fit) | `app.writers[0](@app, bump(@app))` and `app.writers[1](@app, bump(@app))`, `writers[1]` being `peek(a: App lent, ...)` | by name, `add(@app, bump(@app))` (`b2b_by_name`): `2`, exit 0 | **`2`, `3`, `n 3`, exit 0**: the reader in the `@` slot is judged as `@`, so the nested `@` is legal and the reader sees the place after it, as the spec-warden reasons (`spec-warden.md` § 4.1) |
| `s3/` (a generic function, a module crossed, the callee a call's answer on the place the next call lends) | `drive<S>(@s: S, pick: (function(S lent) -> (function(@S, i64) -> i64)), x)`: `h = pick(s)` then `h(@s, x)`, `S` a `machine.Machine` of another module whose step rewrites the list it was read from | refused at the `lent`, exit 1 | **`5`, `15`, `steps 1`, exit 0**: `bind` met a `lent` and an `@` at a type parameter's positions and a moded function type as a function type's result |
| `t2_call_result` (beside `s3`, no generic) | `first(app)(@app, 3)`, a call's answer called at once | refused | `5`, exit 0 |

So on every shape I tried, **a callee read out of the place its call lends is the copy taken before the lend**, as
the base's `q2a` and `q2b` already read for a nested `@`, and the exclusivity rule is asked of the callee's type
exactly as of a declaration (`s2` against `b2`). None of these is a hole. What the route does **not** answer
predictably is § 5's literal order.

## 4. Does the route refuse every call that hides an `@`, every alias the rule names, and no correct program?

- **Every call that hides an `@`, from Heroes**: yes on every shape I or the compiler-engineer built: a missing `@`
  through a value (`m04`, `marker_mismatch`, one message), a generic parameter (`m08`, `type_mismatch`), a moded
  type at a type parameter's positions (`s3`, bound, not hidden), a reader in a writer's slot (judged as `@`, `s2b`),
  the names' fit (it re-interns with the value's own modes, `compiler-engineer.md` § 1), an extern function as a
  value (still refused, § 5), an `@` at a call through a `lent` value (`t9_lent_value_marked`, `marker_mismatch` as
  by name), `spawn_sized` and a moded value handed to `spawn` (`t8_spawn_sized`, `thread_body` twice), and the
  control arm: the seat's `w5` (an `@` through a value on a by-value parameter holding an array) under `build
  --permissive` on my route is still `not_mutable`, exit 1, so the heap class stays closed there too. **From C, no**: a C caller is handed a Heroes `@` function through an `extern`
  callback type and the thunk forwards its slots unchecked (§ 1).
- **Every alias the exclusivity rule names**: from Heroes, yes: two `@` through a local, a field, an element (`m06`,
  `w4`), a `lent` beside an `@` of its place and nested (`m07`, my `s2` against `b2`), each `aliased_mutable_arguments`
  as by name. **From C, no**: `twice` hands one pointer to `both(@a, @b)`, `40040` (§ 1), which design.md §4.8 lines
  1620 and 1621 say cannot happen.
- **No correct program refused**: `router_k` and `router_b` print their `main.expected` byte for byte under `heroes-261-v4`
  on this Mac (`r2/leg/mac-v4/ce.router_k.out`, `ce.router_b.out`, `cmp` each), `knot` prints `1 [ 2 [ 3 ] 4 ] 5`, `0`,
  `2`; the rewritten `reach_web.hero` checks under my `heroes-route`, exit 0 (`r2/leg/reach.check.out`). **But one
  correct-looking program is refused by a rule nobody states**: a table literal with no type written whose first entry
  is a reader (§ 5, `t6`). Whether it is *correct* is the text's to say; as W1 reads, a reader predicts it accepted.
- **On every platform it claims**: the compiler-engineer claims this Mac alone. Linux x86-64 is § 8.

## 5. The spec-warden's text against the built route, clause by clause, and the registration it is scored by

The warden's four diffs each apply to my copy's spec with `patch` and give its text byte for byte (`cmp`, `r2/spec/`).
Each clause of `spec-w1-fit.diff` (the text the warden would adopt) against what `heroes-261-v4` and my `heroes-route`
do, both compilers giving the same answer on every probe below (`r2/mine/`, `r2/mine/ext/`):

| W1's clause | the route | agree? |
|---|---|---|
| § 3 `TypeArg = "@" [ ident ":" ] Type \| [ ident ":" ] Type [ "lent" ]` | parses both, refuses `@T lent` (`lent_shape`, the compiler-engineer's § 1); the name after the mark, `(function(a: @App) -> ())` (`t3_name_then_at`), is `expected_type` at the `@`, as the production says | yes |
| § 9 line 310, the field first at a dot | unchanged from the base (`f4` prints 20; the compiler-engineer's `w3` closes the marks' half) | yes |
| § 9, *"Top-level functions are values, typed with the marks their signature writes"* | **not for a function of an `extern` group**: `extern_at_value.hero`, `bump_it(@p: i64)` bound to `(function(@i64) -> i64)`, is **`error[mutable_parameter_as_value]`**, exit 1, and its message says *"no function type carries an `@` (§ 9)"*, **false** under the route and under W1 | **no**: the text owes the extern's exception (or the route owes its value), and the message is a false one (design.md §4.17), `blocking` by `.claude/rules/verification.md` § Bounded discovery |
| *"a call through a value marks its arguments as its type does"* | `marker_mismatch` for a missing and an extra `@` (`m04`, `m05`), `not_mutable` for an `@` on what the type's `@` cannot take (`w1`, `w5`) | yes |
| *"The marks are part of the type"* | three typedefs for `@`, `lent` and by value (`r6`); `type_mismatch` between them (`m03`) | yes |
| *"but `lent` fits where a type asks `@`"*, `peek` and `skip(_: Lex lent)` values of `advance`'s type | by a binding (`t5_equality`: `w: (function(@App) -> ()) = peek` accepted, `w == bump` prints `false`), through `bind` (`s3`, the compiler-engineer's `router_b`), in a written table (`q5_fit_table`, `t1b_literal_typed`) | yes, **but order-dependent in an unwritten literal**, below |
| § 11 *"`f` taking `x` with neither mark"* | `thread_body` for an `@` or a `lent` body (`q8_spawn`) | yes, under a new code |
| § 13 *"its parameters follow the same rule, with neither mark"* | **a mode in an `extern` callback's type is accepted and built** (§ 1 above: `v4_shapes.hero` runs, C writing into freed memory at exit 0) | **no**: W1 refuses what the route builds; W1c describes it, without the four duties the ffi-pragmatist names |
| (silent) a `lent` number in a callback's type | `ext/lent_num_*.hero`: `n: i64 lent` binds **only to C's non-`const` `int64_t *`** (`call_mut`, prints `42`); against a header's by-value `int64_t` and against `const int64_t *` it is `ffi_callback_type`, exit 1, the second with the note *"a pointee the header marks `const` has no spelling here"* | the ffi-pragmatist's *"two readings; the spec owes one"*: **the route chose a third** (a pointer, not `const`), which no read-only C callback writes |

**A literal's element type is its first element's, so the fit depends on the order of a table's entries**:
`t6_map_literal_order.hero`, `{"SET": set, "GET": get}` (writer first) is accepted and `{"GET": get, "SET": set}`
(reader first) is **`error[type_mismatch]`: expected `(function(Store lent, i64) -> i64)`, found `(function(@Store,
i64) -> i64)`**, exit 1; the same for a list, `[peek, bump]` refused where `[bump, peek]` is accepted
(`t1_literal_order`), and both accepted once the table's type is written (`t1b_literal_typed`: `2`, `2`). `grep -n -i`
of *first element*, *same type*, *literal* in the spec at the base: no sentence says how a literal's element type is
fixed. **This is the blind run's t2 exactly**: a table of seven commands, three of them read-only by the task; a
reader who writes the map literal with `GET` before `SET` and no type is refused under W1, by a rule neither W1 nor
the base spec states. A route nobody listed is the join: a literal whose elements differ only in `lent` against `@`
at a position takes the `@` type, which makes the order irrelevant (unbuilt).

**The registration does not count the codes the route gives a mode mistake** (`blind/registration.md` § The grades,
*refused-for-mode*): it counts `never_rebound` by two messages, `mutable_parameter_as_value`,
`lent_parameter_as_value`, `marker_mismatch`, `expected_type` at an `@`, `expected_function_type_params_close` at a
`lent`, *"and the landed route's new codes"*. The route's answers to a mode mistake, measured on the seat's cases
under v4 (`r2/leg/mac-v4/`): **`type_mismatch`** for a value of the wrong mode (`m03`, and the literal order above),
**`not_mutable`** for an `@` the type takes on a non-cell (`w1`, `w5`), **`aliased_mutable_arguments`** through a
value (`m06`, `s2`), **`thread_body`** for `spawn` (`q8_spawn`), `lent_shape` for `@T lent` in a type. Only
`thread_body` is new. `type_mismatch`, `not_mutable` and `aliased_mutable_arguments` are old codes with other
messages, so they count only if the synthesis lists them **with the message that makes them a mode's** (for
`type_mismatch`: the two types printed differ in a mark alone). The spec-warden's own expected refusal under W1, *"a
by-value `s: Store` in an `@` table, not `never_rebound"`*, is a `type_mismatch` on the route: **as registered, the
warden's predicted D3 refusal would not be counted, and D3 would pass on it**. The literal-order refusal is the same
code.

**The warden's predictions do read the registration's rules otherwise**: D3 over t2 and t3 with t1 reported, the
self-write row, D1's *right* as compiles, prints-right and bound together, `now` from the base. Two things they do not
say: which text they are scored against if the synthesis adopts W1c or amends W1 (registration § How a prediction is
read: *"its text of the same route, every line the arm's diff has and the text does not named"*), and what they
predict for the literal order, which the warden had no compiler to see.

## 6. The historian: does every `verified` line quote its page, and what a compiler on this Mac says

I have no web under my rules (no download), so I held the historian's quotations to the copies of the pages that
stand on this Mac, and ran the precedents it could only read on the compilers installed here:

- **Quotations held to a local copy of the page**: Go 1.27.4's `$GOROOT/src/net/http/server.go:2354` *"The
  HandlerFunc type is an adapter to allow the use of"* and `:2360` *"ServeHTTP calls f(w, r)."*; the Rust Reference of
  the 1.90.0 toolchain's offline docs (`~/.rustup/toolchains/1.90.0-aarch64-apple-darwin/share/doc/rust/html/reference/`,
  tags stripped): *"Function pointers can be created via a coercion from both function items and non-capturing,
  non-async closures."* and the subtyping paragraph, word for word. Swift's, C#'s, Ada's, Nim's, D's, Pascal's,
  Oberon's, Modula-3's, Hylo's, axum's, libevent's and the posts' are not on this Mac: **unchecked by me**.
- **A negative the Reference contradicts**: the historian writes *"The Reference's call-expression page does not
  state whether the function operand is evaluated before the arguments"*. True of that page; but the same Reference's
  *Expressions* page, § *Evaluation order of operands*, lists *"Call expression"* among the expressions of which it
  says *"Expressions taking multiple operands are evaluated left to right as written in the source code."*, and the call-expression page names the
  function operand first (*"an expression, called the function operand, followed by a parenthesized ... list ...
  called the argument operands"*), the local copy above. **So Rust rules on question 2's order: the callee first**,
  the precedent the brief's § 7 line 241 reading and the route both take.
- **Lines inside a `verified` section that are not the page's words**: § 1's forum line, *"the second half is the
  fetcher's summary of the post"* (the historian says so itself, and `.claude/rules/records.md` § And the record says
  whose idea it was asks the page's own words, 33 of 90 such lines having said more than the page in 2026-10-11's
  audit); § 9's *"every example derives `Clone` on the state"* (a count over a page, not a sentence of it) and *"So
  axum's generic router hands a handler a value"* (an inference); § 8's ISO 7185 item b), paraphrased beside item a)'s
  quotation. A `## Prior art` drawn from this report should take none of the four as written.
- **Run on this Mac, where the historian could only read** (`r2/prec/`):
  - **Swift 6.4.2** (`swiftc -swift-version 6`): `q2.swift`, a struct holding `h: (inout App, Int) -> Int` and `hs`
    of the same, called **`app.h(&app, 2)`** with `add` rewriting `a.h`: **compiles and prints `3`, `9`, `10`**: the
    callee is read before the in-out access, and Swift's exclusivity enforcement accepts it, statically and at run
    time. `modes.swift`: `bump(inout App)` where `(App) -> Void` is asked, **`error: cannot convert`**; **`peek(_ a:
    borrowing App)` where `(inout App) -> Void` is asked, `error: cannot convert`**; `plain(_ a: App)` where
    `(borrowing App) -> Void` is asked, accepted; and `m_dir.swift`, **`peek(_ a: borrowing App)` where `(App) -> Void`
    is asked, accepted and run, `4`**: route (m)'s own direction. So Swift refuses the very fit (h) the route adopts
    and admits the one (m) the route refuses. The compiler-engineer refuses (m) because the value's C slot is `T *` and
    the type's `T`, *"so it needs a thunk per adapted function"*; that thunk captures nothing (it takes `T`, lends its
    own copy, calls one named function), a static twin like the ffi-pragmatist's route F, not a closure: the
    spec-warden's closure argument (`spec-warden.md` § 4.2) is about a **by-value** function in an `@` slot, not (m).
  - **Rust 1.100.0**: `q2.rs`, `struct App { h: fn(&mut App, i64) -> i64, hs: Vec<...> }`, **`(app.h)(&mut app, 2)`**
    and `(app.hs[0])(&mut app, 1)`: **compiles, prints `3`, `9`, `10`**. `fit.rs`, `let f: fn(&mut App) -> i64 =
    peek` with `peek(a: &App)`: **`error[E0308]: mismatched types ... types differ in mutability`**.
  - **Nim 2.4.2**: `modes.nim`, `type H = proc (x: var int)`: `bump(x: var int)` accepted, `peek(x: int)` **`Error:
    type mismatch ... expected 'H = proc (x: var int)'`**. The historian's *"`var` in a procedural type's identity
    **unverified**"* is settled by a run: it is part of the identity.
  - So **three compilers on this Mac accept question 2's shape as the route reads it** (the callee a copy taken
    first), and **all three refuse a read-only function where a read-write function type is asked**; the historian's
    one precedent for (h) is C# 12's, *"with a warning and gated on LangVersion"*, which no compiler here can run.
    The route's fit has a precedent of one, with a warning, against three measured refusals.
- **A hard stop touched by a tool**: the historian reports that fetching Wirth's Oberon-07 PDF *"made the fetch tool
  keep the PDF's bytes in its own results folder outside the repository"*. CLAUDE.md § Hard stops: *no file is written
  outside this repository's root*. The seat says it requested nothing; the file stands somewhere outside the root, and
  the author is owed that sentence and the path (unread by me; I did not look outside the root).

## 7. Do the emissions of programs that do not use the form keep their bytes?

The compiler-engineer measured the 249 programs of `tests/golden/run/` that **write** `(function(` and the compiler's
own C; nobody measured the programs that write none, which are the ones the promise is about. `r2/ident/one.sh`:
`--emit-c` by my base stage 2 and by my `heroes-route`, the line `hero_program_sources(void) {` taken out of each,
`cmp`:

- **`tests/golden/emit`, `ir` and `fixedbugs` whole, and every fifth program of `tests/golden/run/` that writes no
  `(function(`** (`grep -L | awk 'NR % 5 == 0'`, 164 of them): **248 of 248 the same**; 212 emitted C, 36 emitted
  nothing under either compiler (both empty, the same; programs whose build needs what `--emit-c` from their
  folder does not give, unread by me which).
- **Every `examples/*/main.hero`** (55, 12 of them writing `(function(`): **55 of 55 the same**, none empty.
- **`fixedbugs-417-every-function-value-is-named-before-c-is-written`**, the one the compiler-engineer found moved:
  **moved under my build too**, 37,949 bytes either way, `typedef HeroStr (*h_0fn_1fb9aa18)(HeroStr, int64_t);` at
  line 85 at the base and line 101 under the route. Its cause is the seat's hypothesis, unrun by anyone; `tests/emission`
  blesses that file's C, so the `emission` suite reads red on the landing until the cause is named or the bless is
  re-made, and *"a mode-free program keeps its bytes"* is then a promise with one known exception.
- Not run: the `emission` suite itself under the route (the instrument that judges blessed bytes), the other four
  fifths of the mode-free `run` programs, and any emission on Linux.

## 8. The compiler-engineer's cases on this Mac, and on Linux

**On this Mac** (`r2/leg/cases.sh`, one driver for every compiler: `check`, then `build` and the binary under
`timeout 60`, output bounded; the two control-arm cases also `build --permissive`), over a copy of the seat's
programs, my shapes and `v4_shapes.hero`, 46 programs:

- **`heroes-261-v4`** (`r2/leg/mac-v4/`, summarised by `r2/leg/summ.sh`): **every verdict is the seat's § 3 table**:
  `m01` `2 5 5`; `m02` `7 7`; `m03` two `type_mismatch`; `m04` one `marker_mismatch`; `m05` `marker_mismatch`, and
  under `--permissive` `6 3`; `c1` under `--permissive` `1 2`; `m06`, `w4` `aliased_mutable_arguments`; `m07` two;
  `m08` `type_mismatch`; `m09` `3 10`; `q2_routes_index` `1 2 101 3`; `q2_disjoint` `5 7`; `q2_field_callee`,
  `f4_field_first` and `base_self_field` build exit 2; `q3` `1 3 6 true false 0 7`; `q5_fit_table` `2 3 1 2`;
  `q5_uniform_table` two `never_rebound`; `q5_self_write` `2 3`; `q7` `3 0`; `q8_spawn` two `thread_body`;
  `q8_mailbox` `11`; `k_constant_table` `1 2 3`; `r6` `1 1 1`; `w1` three `not_mutable`, `w3`, `w5` one each; `knot`
  `1 [ 2 [ 3 ] 4 ] 5`, `0`, `2`; `lib_value` `2`, `not_found`, `b`; **`router_k` and `router_b` `cmp`-equal to their
  `main.expected`**; **`router_a` `/g/a -> b`, `/b -> a`**, the silent misroute the seat reports.
- **The base's stage 2** (`r2/leg/mac-base/`): every program writing a mode refused at the parse (`expected_type`,
  `expected_function_type_params_close`); `m05` under `--permissive` **exit 2** (the repair's `x1`); `base_self_field`
  build exit 2; `m08` `mutable_parameter_as_value`; `q8_spawn` the two `*_as_value` codes.
- **My `heroes-route`** (`r2/leg/mac-route/`): § 8.1 below.

### 8.1 My build of the route against the seat's binary, this Mac

`sh r2/leg/compare.sh r2/leg/mac-v4 r2/leg/mac-route` (each case's codes, exits and printed lines, platform-free):
**46 of 46 the same** (`router_b` re-run alone at 12:18 after I stopped the first pass at 12:03 to free the driver
for the Linux leg; its 8 lines `cmp`-equal to `main.expected`). So `route-v4.diff` as published reproduces
`heroes-261-v4`'s answers, the R6 test block aside.

### 8.2 Linux x86-64

`heroes-linux:latest` without `--platform` (`uname -m`: `x86_64`, under emulation), Debian clang 22.1.8, one container
`p261-critic`, `--rm`, 10:37:51 to 11:07:19 UTC for the seed and stage 2 (`r2/leg/x86out/machine.txt`), over a tar of
`r2/route/`'s sources and the programs (`r2/leg/route-leg.tar`, `leg.sh`). The seed, then the route's stage 2 from the
patched `selfhost/`: **`sources 7cc4a75a5a7c6bf1`, my Mac route's key** (`x86out/stage2.log`). Then the same driver,
ordered for Linux (the programs needing no library first, `router_k` last; `router_b`, `router_a` and `lib_value`
left to this Mac, a library build under emulation costing tens of minutes each):

- **Every verdict the same as `heroes-261-v4`'s on this Mac** (`compare.sh x86out/cases mac-v4`): **42 of the 43
  shared cases the same**, every one of the seat's top-level cases, both control-arm runs, `knot`, my shapes and
  `v4_shapes`. **The 43rd, `router_k`, does not build on Linux x86-64**: `check` exit 0, `build` exit 1, one
  **`error[ffi_unpaired_release]`**: *"`nextafterf`, bound here, is declared by a header of `.`, a folder this build
  names, and defined by a library no folder this build names holds"*, at Rebel's `math/hero_math.hero:57`, *"the
  declaration clang read is in `scratch space>`"*, *"the definition the link took is in
  `/lib/x86_64-linux-gnu/libm.so.6"`* (`x86out/cases/ce.router_k.out`). The route's diff touches no FFI, link or
  library module (its 25 paths, `grep '^+++'`), so this read as a fact of the base, an inference **settled on arm64**
  (§ 8.3: the base's own stage 2 refuses reach's mode-free program the same way; on x86-64 the base is unrun). `gh issue list --state all --search`
  for *ffi_unpaired_release nextafterf*, *nextafterf*, *unpaired release libm*: nothing. So **route (k), approved by
  two seats, cannot be shown to run on Linux** until the base's refusal is repaired, and every program reaching Rebel's
  `math` meets it.
- **`v4_shapes.hero` on Linux x86-64: `40040`, `4702111234474983745`, `42`, exit 0**: C's bytes read back from freed
  memory, as on this Mac (§ 1). The ffi-pragmatist's veto condition holds on two platforms of a compiler, not of
  edited C.
- **The two Linux bindings written as the route lets a program write them** (`x86out/extra/`): **`dl_iterate_phdr`
  with a `lent` reader handed where the callback's type asks `@info`: builds and prints `0`, exit 0** (glibc's `struct
  dl_phdr_info *` is not `const`, so the route's non-`const` `lent` slot matches it); **nftw: `ffi_callback_type`,
  build exit 1**, *"clang read the header, and it says `int (*)(const char *, const struct stat *, int, struct FTW
  *)`"*, as on this Mac. So the fit binds the one header that omits `const` and refuses the one that writes it, which
  is the ffi-pragmatist's measurement inverted into the route's spelling.
- `m05` under `--permissive` `6`, `3`; `base_self_field`, which writes no mode, build exit 2 under the route there too
  (the base was not built on this leg, so its own exit 2 on Linux is unrun).

### 8.3 Linux arm64

`heroes-linux-arm64:latest`, `aarch64`, Debian clang 22.1.8, the same tar and driver, one container `p261-critic`
after the x86-64 one was gone, 11:28:36 to 11:31:58 UTC for the seed and stage 2 (`r2/leg/arm64out/`): **`sources
7cc4a75a5a7c6bf1` again**. **Every verdict as on x86-64**: 42 of the 43 shared cases the same as `heroes-261-v4` on
this Mac; **`v4_shapes.hero` `40040`, `4702111234474983745`, `42`, exit 0**; `dl_iterate_phdr` with the fit `0`;
nftw `ffi_callback_type`; and **`router_k` `build` exit 1, the same `ffi_unpaired_release` on `nextafterf`** at
`math/hero_math.hero:57`. So the C-boundary finding of § 1 stands on **three platforms of a compiler**, and route
(k) builds on neither Linux leg.

**And the Linux refusal is the base's**: a third container (`r2/leg/arm64base/`, 11:35:56 to 11:42:35 UTC), `git
archive df92f9281` of the compiler's sources, Rebel's library and `rebel/examples/reach`, the seed and the **base's**
stage 2 (`sources 4dcf3a9f20257573`), then reach's own `reach_web.hero`, which writes no mode: **`build` exit 1, the
same `error[ffi_unpaired_release]` on `nextafterf`**. So at `df92f9281` a Rebel program reaching `math` does not
build on Linux arm64, route or no route; x86-64 under the base is unrun, and whether `main` (`a60dfb77f`) still
refuses it is unrun (`git log df92f9281..main` subjects name no repair of it: *unpaired*, *libm*, *math*,
*nextafter*, *scratch*, none). Unfiled by the words of § 8.2. `reach_web.hero` takes only `http`, `io`, `net`, `web`
and `web/proxy` and reaches `math` through them (its `use` lines; `router_k` adds `bytes`, which `use`s `math`), so the
HTTP examples this sitting's landing rewrites meet it on Linux, **an inference for the fifteen**, each settled by its
own build there.

## 9. What is missing: routes nobody listed, claims not measured, contradictions between seats

**Routes nobody listed or built**:

- **The literal's join** (§ 5): a list or map literal whose entries differ only in `lent` against `@` at a position
  takes the `@` type, so the order of a table's entries stops deciding whether it checks. Unbuilt.
- **A mode in an `extern` callback's type refused by the checker**, W1's § 13 as written: argued by the spec-warden,
  built by nobody; the route as built does the opposite (§ 1).
- **(m) through a static twin**: Swift does it (§ 6); the compiler-engineer objects on cost, the ffi-pragmatist does not
  object, the spec-warden leaves it to naming. Nobody built the twin, so the three verdicts rest on no run.
- **(n), the state lent and the changes answered as data**: built by no seat (§ 2 item 8).
- **The self-write refused** (`x @ x` counting as no write, the warden's +7): no seat built it, and the route keeps
  `app @ app` a write (`q5_self_write`: `2`, `3`). The registration reports it; no ruling says whether it is a write.

**Claims asserted and not measured**, each with the command that would settle it:

- **`needed_for_self_hosting: no`** (the compiler-engineer and the spec-warden) rested on a rule file's sentence
  (*"Since panel 252's R1 a ring of modules builds"*), not on a ring built. **Settled now at the base**:
  `r2/mine/ring/`, panel 109's shape as two modules that `use` each other and thread `@b` through their mutual
  recursion, `heroes-next run main.hero`: **`1 [ 2 [ 3 ] 4 ] 5`, exit 0**. So the knot is cut by a ring today, and the
  form is not the compiler's need, measured on the shape (not on the compiler's own knots, which stay uncut).
- **The library's cost of route (B)**: *"the fence holds at exactly 90 with `on` and `dispatch`"* is arithmetic on 88;
  `faces.on` and `faces.dispatch` live in a module of the program, not in `web.hero`, so the `rebel` suite never read
  them (`heroes run tests/harness/main.hero -- <route> rebel` over a `web.hero` holding them: unrun).
- **The `control`, `permissive`, `check`, `run` and `emission` forms under the route**: unrun by every seat and by me;
  the five `check` goldens' movement is five `check --brief` runs, which I re-ran under my build with the same line
  counts (12 to 7, 3, 5, 9, 3; `r2/goldens/`).
- **The ffi-pragmatist's *"3 `run` goldens"*** moved by `const` on a lent group record: a classification script over
  the sources, no emission run, no compiler writing `const` existing.
- **The spec's real count**: every text's `real` is an inference (ratio 1.25 to 1.36); `--refresh` unrun, unauthorised.
- **The cause of `fixedbugs-417`'s move**: a hypothesis (§ 7).

**Contradictions between seats, and the checkable side**:

- **The compiler-engineer's *"an `extern` keeps both refusals"* against the ffi-pragmatist's veto**: checkable, and
  checked (§ 1): the route lets C call an `@` function through the unchanged thunk.
- **The brief's names' fit against the spec-warden's**: the warden's side, run (§ 2 item 2).
- **(m)**: refused (compiler-engineer), no objection (ffi-pragmatist), naming only (spec-warden); Swift admits it and
  refuses (h) (§ 6). Nobody built (m), so no side is checkable by a Heroes run yet.
- **What `lent` spells in a callback**: the ffi-pragmatist's `const T *` for a group record (its route B or F) against
  the route's non-`const` `T *` (§ 5, the nftw refusal); checkable by the ffi-pragmatist's variants, whose C the route
  does not emit.

**My own slip, reported**: at 11:50 one command of mine redirected `heroes check`'s output to `/tmp/critic-unused`
(empty, 0 bytes, `ls -la`), a file outside the repository's root, against CLAUDE.md § Hard stops and my brief. I
removed it at once (`rm -f`, `ls` then *"No such file or directory"*) and redid the check into `r2/goldens/`.

## What the synthesis must settle

Most important first; each with the command that settled it or would. Paths are under
`.claude/worktrees/261-critic-scratch/r2/`; `<route>` is `heroes-261-v4` or my `route/heroes-route`, which answer alike.

1. **The route lets C call an `@` function through the unchanged thunk, so the compiler-engineer's approved route is
   the ffi-pragmatist's vetoed landing.** `v4_shapes.hero` builds and runs at exit 0 with C writing into freed memory
   (`--sanitize`: `heap-use-after-free`) and one pointer in two `@` slots (`40040`), on this Mac, Linux x86-64 and
   Linux arm64 (§ 8.2, § 8.3). Choose: refuse a mode in an
   `extern` callback's type in the checker (W1's § 13, then a `check` case), or carry `ffi-pragmatist.md` § 7's four
   duties with a `run` case each on three legs (W1c). *Settled by*: `cd ffi/shapes && <route> build v4_shapes.hero
   --sanitize -o s && ./s`; the landed compiler must exit 1 at `check`, or 134 with the checked thunk's panic.
2. **The fit depends on a literal's entry order**, unstated anywhere: `{"GET": get, "SET": set}` refused,
   `{"SET": set, "GET": get}` accepted, both accepted with the type written. It is the blind run's t2. Choose the
   literal's join (unbuilt), or a spec sentence, before the arm is written. *Settled by*: `<route> run
   mine/t6_map_literal_order.hero` (exit 1), `mine/t1b_literal_typed.hero` (`2`, `2`).
3. **The registration must list the codes the route gives a mode mistake, by message**, before any session:
   `type_mismatch` where the two types differ in a mark alone, `not_mutable` and `aliased_mutable_arguments` through a
   value, `thread_body`, `lent_shape`; else the warden's own expected refusal and the literal-order refusal are
   uncounted and D3 is flattered. *Settled by*: the grades' rule read against `leg/mac-v4/ce.m03_three_types.out`,
   `ce.w1_at_on_binding.out`, `ce.m06_alias.out`, `ce.q8_spawn.out`.
4. **An `extern` function with a mark is still no value, under a false message**: *"no function type carries an `@`
   (§ 9)"*, while W1 says *"Top-level functions are values, typed with the marks"*. Settle the extern's rule, then its
   text and its message (design.md §4.17). *Settled by*: `cd mine/ext && <route> run extern_at_value.hero`.
5. **What `lent` spells in a callback type**: the route writes a non-`const` `T *` for a group record and for a
   number, so nftw does not bind (`ffi_callback_type`, this Mac and x86-64), `dl_iterate_phdr` binds because glibc
   omits the `const`, and `i64 lent` binds only C's non-`const` `int64_t *`; the
   ffi-pragmatist's `const T *` (route B, 3 `run` goldens by its script) or a twin (route F) is unbuilt. Settle it with
   item 1. *Settled by*: `cd ffi/nftw && <route> build v4_fit.hero`; `cd mine/ext && <route> run lent_num_mut.hero`
   (`42`) and `lent_num_ptr.hero` (exit 1).
6. **The fit (h) and its refusal of (m), against the precedents run here**: Swift 6.4.2, Rust 1.100.0 and Nim 2.4.2
   refuse a read-only function where a read-write type is asked; Swift admits (m)'s direction, which the route refuses;
   the historian's one precedent for (h) is C# 12's, with a warning. The ratification text should say so; (m) as a
   static twin was built by nobody. *Settled by*: `cd prec && swiftc -swift-version 6 modes.swift`, `swiftc m_dir.swift`,
   `rustc fit.rs`, `nim c modes.nim`.
7. **The brief's names' fit was inverted, by my first reading, and the repair carried it**; feature 1133's sentence is
   false at the base. The synthesis's record must not repeat it, and 1133 is owed a correction underneath. *Settled
   by*: `../../261-critic/heroes-next run mine/n1_names.hero` (`8`) and `mine/n2_names.hero` (`type_mismatch`).
8. **`fixedbugs-417`'s emission moves under the route, reproduced by my build**: the compiler-engineer's condition (3),
   cause unrun; every other program I emitted keeps its bytes (248 and 55). *Settled by*: `sh ident/one.sh
   <base> <route> tests/golden/run/fixedbugs-417-every-function-value-is-named-before-c-is-written.hero <out>`
   (`DIFF`); then the `emission` suite under the landed compiler.
9. **Four base defects sit under the sitting**: the repair's `x1` (`build --permissive` exit 2) and the
   compiler-engineer's typedef cycle (`base_self_field.hero`, exit 2), each `blocking` and read as unfiled by the
   words of § 2 item 5; the base's own tests red on `x y`, repaired on `main` by `1ac3f57f1`; and **on Linux a Rebel
   program reaching `math` does not build** (`ffi_unpaired_release` on libm's `nextafterf`, Rebel's
   `math/hero_math.hero:57`), `router_k` under the route on x86-64 and arm64 and reach's mode-free `reach_web.hero`
   under the base's own stage 2 on arm64 alike (§ 8.2, § 8.3), unfiled by the words searched, so the Rebel HTTP
   examples the landing rewrites read red on Linux before the form is written (an inference beyond reach and
   `router_k`). The landing's lane starts from `main`,
   and the last two are settled before its gate. *Settled by*: `cd mine && ../../../261-critic/heroes-next build
   --permissive x1.hero` (exit 2, *"incompatible pointer to integer conversion"*) and `... build base_self_field.hero`
   (exit 2), both run by me at 11:58; `heroes-next test selfhost/main.hero --only "the list holds every word"`; `sh
   leg/baseleg.sh` in `heroes-linux-arm64:latest` (`leg/arm64base/reach_web.out`), and the same at `main` and on
   x86-64 (unrun).
10. **The historian's record**: four lines inside `verified` sections are not the page's words (the forum summary,
    axum's *every example* and *So*, ISO 7185's item b); the Rust Reference does rule on question 2's order
    (*left to right as written*, the callee first); and a PDF stands outside the repository's root by the fetch tool,
    which the author is owed. *Settled by*: `sed -e 's/<[^>]*>//g'
    ~/.rustup/toolchains/1.90.0-aarch64-apple-darwin/share/doc/rust/html/reference/expressions.html | grep -n -A20
    'Evaluation order of operands'`.
