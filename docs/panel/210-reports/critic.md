# Panel 210, completeness critic (one pass, after the reports)

No verdict of my own. Read: the four briefs, the three reports, CLAUDE.md
§ RUN IT and § Precedence, `.claude/rules/diagnostics-and-goldens.md`, spec
§ 3, § 5, § 9 and § 13 of the frozen tree, the compiler-engineer's
`route.diff` and census, the spec-warden's census and probes. Built in
`.claude/worktrees/scratch-b15/210-critic/`: `tree/heroes` from the seed at
`1dd890751` (02:51 to 02:52 by `date`), and `tree-route/heroes-route`, the
same tree with the engineer's `route.diff` applied by `patch -p1` (clean, ten
files) and built by `tree/heroes` (02:53 to 02:55). Probes in `probes/`, each
run with `heroes check` (and `run` where said) by the two compilers. Started
02:51. Written as I went.

## 1. The two corrected rules do not count the same writes

The engineer's eight (report § 2): `@` to the name, a field, an element, an
`@` argument, a `.ptr()` lend, an argument to a `consumes` or `transfers`
parameter, an `@` the checker will add or refuse (`meant_write` widened from
`.cell` to every mutable local, so `bump(k)` for `@c` is a write), and the
UFCS receiver of an `@` first parameter. The spec-warden's corrected rule
(report § 4 and § 9): the brief's four, the extern out-parameter and
`end_lease(@x)` as `@` arguments, `consumes`, `transfers`, a lend C writes
through, **and silence beside a told dropped value of the parameter's own
type**. So the engineer has two writes the spec-warden lacks, and the
spec-warden has one suppression the engineer lacks.

**On correct programs they agree; on refused programs they differ, and one
of the two predictions must fall.** Measured with `heroes check --brief`,
codes counted by `grep -oE '\[[a-z_]+\]' | sort | uniq -c`, trunk against
`heroes-route`:

| case | trunk | route (engineer's eight) |
|---|---|---|
| `fixedbugs-135-a-label-the-order-written-may-not-mean` | 2 `marker_mismatch`, 9 `missing_label`, 18 `needs_label`, 1 `wrong_label` | the same, **0 `never_rebound`** |
| `fixedbugs-135-a-dropped-value-of-a-place-s-own-type` | 15 `discarded_value` | **8 `never_rebound`, 0 `discarded_value`** |
| `fixedbugs-260-each-at-argument-against-the-earlier-ones-at-once` | 5 `aliased_mutable_arguments`, 1 `i` | the same plus 6 `never_rebound` |

The label case's two sites (`:76`, `markers_both_left_out(@x, @y)` calling
`swap(x, y)`) are exactly the difference between the two censuses: the
spec-warden's `tmp/census.u` lists `:76:33` and `:76:42`, the engineer's
`census/result4.txt` reads that file `1 1 0`. So:

- **The spec-warden's P1 (*exactly 20* at `1dd890751`) is false against the
  rule the engineer built**: the route on the frozen tree would read 3 in
  `selfhost/` + 1 example + 14 check sites = **18** (the 14 and the 1 from
  `result4.txt`; the 3 are the engineer's and the spec-warden's own count on
  the unedited `selfhost/`). P1 counts the two `marker_mismatch` sites the
  route spares on purpose.
- **P1 is also false against the spec-warden's own approved route**: that
  route is *silent beside a told dropped value*, and the 8
  `fixedbugs-135-a-dropped-value` sites P1 counts are exactly the parameters
  whose only write is a dropped value of their own type (the engineer's § 3,
  and the route's 8 `never_rebound` against the trunk's 15 `discarded_value`
  above). Her rule as she approves it reads 12 (no meant writes) or 10 (with
  them), never 20.
- **The suppression she requires is unbuildable where the rule is built.**
  The rule lives in the resolver's sweep (`selfhost/resolve/unused_sweep.hero`);
  the dropped value is the checker's verdict (`discarded_value`, which needs a
  type), and on the route the resolver's message is the only one the case
  gets (the table's second row). Neither report prices *silent beside a told
  dropped value*: it needs the rule moved after the checker, or a syntactic
  stand-in in the resolver (section 6 below builds one).

What the synthesis must choose, then, is one write list, and score the
prediction that matches it; the engineer's 15-site prediction is the one
consistent with `route.diff` as it stands.

## 2. A write both seats missed: the handle ended through UFCS

Every one of the 13 FFI sites both seats read calls the consuming function by
name (`node_free(n: n)`, `db_close(db: db.handle)`). Spec § 9 makes
`n.node_free()` the same call (`x.f(y)` is sugar for `f(x, y)`, and UFCS is
barred only when the first parameter is `@`, which `node_free(n: Node
consumes)`'s is not). Probes `probes/c04*` and `c05*`, the header being
`tests/golden/run/handle-a-helper-that-ends-its-parameter-is-the-runtime-s.h`
copied as `node.h`:

| probe | trunk `check` | route `check` |
|---|---|---|
| `c05` `finish(@n)` body `node_free(n: n)` | 0 | 0 |
| `c05b` the same, `@` removed at parameter, call and cell | 1 `consumed_borrowed_handle` | (not run: the route changes nothing there) |
| **`c04` `finish(@n)` body `n.node_free()`** | 0 | **1 `never_rebound`, fix (guess) *remove the `@`*** |
| **`c04b` `c04` with the `@` removed** | **0** | **0** |
| `c04c` `c04b` plus `node_peek(n: b)` after `finish(b)` | 0 | (not run) |

`c04c` built by `tree/heroes build -o c04c.bin` and run as
`timeout 10 ./c04c.bin | head -c 2000`: prints `before: 2`, then `panic: a C
handle reached the argument n of node_peek at an address a call marked
consumes or transfers ended`, exit 134.

Three findings, the first inside the question, the second beside it, the
third the second's neighbour:

- **Inside: the built rule refuses a correct program, and its fix leads to a
  wrong one that compiles.** `c04` is § 13's own shape (*mark the parameter
  `@` and the value does not survive the call*); the route says the `@` is
  written by nothing, because `meant.ended_by_signature` walks `given`
  (`m.args`) from `first_position(receiver)` and never the receiver itself
  (`route.diff`, `built_marks.hero` hunk 1). The guess applied gives `c04b`,
  which both compilers accept. This is the engineer's own veto ground
  (§1.12: *a fix must not lead a correct program into a refusal or a false
  message*) arriving at the eighth write's neighbour, the shape beside his
  p22 (CL-061: the shapes beside the repair).
- **Beside: the trunk misses § 13's refusal in the UFCS form.**
  `consuming.refuse_borrowed` has one caller, `selfhost/check/walk.hero:1202`,
  on the named-callee path (`named_call`); the method path does not reach it
  (read, `grep -rn refuse_borrowed selfhost/`). So `c04b` and `c04c` are
  accepted at `1dd890751`, a borrowed handle ended, which § 13 calls an
  error; the runtime catches the later use (`c04c`, exit 134), so nothing is
  corrupted. By `.claude/rules/verification.md` § Bounded discovery that is *a
  wrong one accepted*, `blocking`, and it is a defect to file whatever panel
  210 decides.
- **`transfers` has the same gap, measured** (`probes/tr.h`, `h_into(child: H
  transfers h_close, parent: H)`): `c07` `attached(@h: H, parent: H)` body
  `h.h_into(parent: parent)`, trunk 0, route 1 `never_rebound` with the
  guess; `c07b`, its `@` removed, trunk 0 and route 0; `c07c`, the same
  written `h_into(child: h, parent: parent)`, trunk and route 1
  `consumed_borrowed_handle`.

The two blind spots agree, which is why neither seat saw it: the resolver's
new count and the checker's old refusal both read only the named form. The
engineer's route decides *which calls end a handle* a second time, in
`resolve/meant.hero`, beside the checker's `check/consuming.hero`; two
places answering one question is how they drift, and here they drifted
together.

## 3. Which spec sentence is true of the built rule

Measured with `tree/heroes measure` (vendored, no `--refresh`), the frozen
spec at legacy 7502, cl100k 7636:

| text at § 5 `:138` | legacy | cl100k | command |
|---|---|---|---|
| engineer's, *a cell or `@` parameter nothing re-binds* | 7507 (+5) | 7641 (+5) | `measure drafts/E5.md` (mine, the engineer's words exactly) |
| spec-warden's D5, *a cell or an `@` parameter nothing re-binds* | 7508 (+6) | 7642 (+6) | `measure ../210-spec-warden/drafts/D5.md` |
| spec-warden's D2c, *... or an `@` parameter nothing writes or ends (section 13), an `@` argument counting* | 7515 (+13) | 7650 (+14) | `measure ../210-spec-warden/drafts/D2c.md` |

Held against what `heroes-route` does:

- **The engineer's +5 text is false of his own rule on the 13 FFI sites.**
  § 5 defines re-binding (`:134-135`: *`@` re-binds it, or a field or element
  inside one*); a `consumes` call is not one. A reader of the +5 sentence
  concludes `finish(@n)` / `node_free(n: n)` is an error; the route accepts it
  (`c05`, exit 0), and § 13 requires it. The engineer handed this question to
  the spec-warden (his § 6); her D1 row answers it for the same reason.
- **D2c is true of the built rule on every census shape**: a lend C writes
  through is a write (§ 13, *C writes back through the lend*), a `consumes` or
  `transfers` argument *ends* (§ 13's verbs), the field case
  (`db_close(db: db.handle)`) by the same reading as *a field or element
  inside one*. The two meant writes (`marker_mismatch`, `ufcs_on_mutable`)
  fire only on programs already refused, so no sentence owes them.
- **D2c is false of the built rule in two places, both the rule's fault, not
  the text's**: `c04` (the UFCS end, section 2), which D2c spares and the
  route refuses; and `function f(@_: i64)`, which D2c calls an error and the
  route accepts (section 4). With the receiver counted and `@_` refused, D2c
  is the true sentence; the +5 sentence is false either way.

## 4. `function f(@_: i64)`: accepted, and it keeps the caller's cell alive

`probes/c02_at_wildcard.hero`, `function f(@_: i64) -> i64` returning 1,
`main` holding `k @= 1` and printing `f(@k)`: trunk `check` exit 0, route
`check` exit 0, trunk `run` prints `1`, exit 0. `c03` (`f(_: i64)`, no `@`)
exit 0 too, as § 5 allows. So the engineer's p15 gap is real on the frozen
tree, and it carries p20's second effect for free: `k` is a cell nothing
re-binds, spared from panel 209's `never_rebound` by *an `@` argument
counting*, the argument going to a parameter that binds nothing. It is defect
608's shape one level up (`_ @= e`,
`issues/2026-10/11/2026-10-11-0018-defect-608-the-wildcard-with-the-cell-symbol-is-accepted.md`),
which the trunk files as `blocking`. The spec-warden's report does not name
it (`grep -n "@_\|wildcard\|608"` over it is empty); her probe rule reads
`resolved.Local`s and the wildcard declares none (`resolve/state.declare`'s
silent `wildcard` answer, the cause defect 608 names), so her rule misses it
too. Unbuilt in both seats; the engineer's *about 10 lines* is an estimate,
**unrun**.

## 5. Panel 209's certain fix leading away from the repair: real at `1dd890751`

`probes/c01_cell_dropped_push.hero` (`xs: [i64] @= [1]`, `xs.push(4)`,
`print(xs.len())`), on `tree/heroes`, the trunk compiler:

1. `check`: one message, `never_rebound` at 2:5, **fix (certain): write `=`**.
2. `check --apply --in-place`, then `check`: `xs: [i64] = [1]`, and now
   `discarded_value` at 3:5, fixes (guess) *assign it back to `xs`* and
   *discard it explicitly*. A second and third `--apply` change nothing (no
   oscillation: both guesses).
3. The first guess written (`c01b`): `not_mutable` at 3:5, fix (guess)
   *declare `xs` as a cell*, the `@=` step 1 removed.
4. The second guess written (`c01d`, `_ = xs.push(4)` on the `=` binding):
   `run` prints **1**, exit 0. The repair the author meant (`c01c`, `@=` kept
   and `xs @ xs.push(4)`) prints **2**.

So the engineer's side finding stands on the frozen tree as he wrote it. What
it is, by the rules: the certain fix does repair what its diagnostic names (a
cell nothing re-binds), and no applied certain fix compiles to a wrong
program, since `discarded_value` still refuses the drop; so it is not *a
certain fix that writes a program meaning something else*. It is one mistake
told by the wrong rule first (§4.17), whose certain fix removes the very `@=`
the true repair needs, a round trip of three messages, and a path through
the second guess to a program that loses the element. Panel 209's R1b spares
a cell *where the resolver already refused its only write*; a drop is refused
by the checker, so R1b's carve-out does not reach it. Searched: `grep -rn
"\.push(\|discarded_value"` over the 209 record, its briefs and its reports is
empty, and over `issues/` the files naming `never_rebound` and `push` or
`discarded` or `dropped` are one, defect 608, which is about `_ @= e`. So the
question is whether any sitting saw it, and the searches say none did. **It should be filed
whatever 210 decides**, `adjacent` by the bounded-discovery list (*a mistake
told only after the first is fixed*), and the route below closes it.

## 6. A route nobody listed: a value line rooted at the local is a write meant

Both the masking the spec-warden wants silenced (fixedbugs-135) and section
5's trap have one cause: the resolver counts writes before the checker knows
which lines are dropped writes. The engineer already has the mechanism for
the other refused shapes (`meant_write` for `marker_mismatch` and
`ufcs_on_mutable`). Built here as a probe, not a proposal:
`tree-alt/heroes-alt` is `tree-route` plus one call in
`resolve/walk.hero`'s `.expr_stmt` arm and `meant.dropped_on` (11 lines with
its comment) counting the receiver of a method-call line as a meant write,
+14/-1 against `tree-route` (`210-critic/alt.diff`; built 02:57 to 02:59).
Results, `check`:

| probe | trunk | route | alt |
|---|---|---|---|
| `c01` cell, dropped push | `never_rebound`, certain `=` | the same | **`discarded_value` alone, first guess *assign it back*, which compiles (`c01c` prints 2)** |
| `c06` `@xs` param, dropped push (the engineer's p24) | 0 | `never_rebound`, guess *remove the `@`* | **`discarded_value` alone** |
| `c04` the UFCS end, and `c07` the UFCS transfer | 0 | `never_rebound` (false) | 0 (right, for the wrong reason: a method line, not the end) |
| `c09` the UFCS end inside a binding, `rc = h.h_close_rc()` (`probes/tr2.h`) | 0 | `never_rebound` (false) | `never_rebound` (false): so `ended_by_signature` must read the receiver whatever this route does |
| `p12_shadow_copy` | 0 | `never_rebound`, guess | the same |
| `fixedbugs-135-a-dropped-value` | 15 `discarded_value` | 8 `never_rebound` | **5 `never_rebound`**: the call form `push(b.items, 4)`, the chain `xs.push(4).push(5)`, the operator `total + x` are not method lines |

So the method form alone closes section 5 for cells and parameters both and
3 of the 8 maskings; the call, chain and operator roots would need the same
count at the call's first argument, the chain's root and an operand, still
syntactic, **unbuilt**. What it can cost is a false negative: a `()` method
line on a needless cell or `@` parameter (`log.show()`) would spare it.

**Census, route against alt**, `census/one.sh` over the engineer's
`census/files.txt` (`selfhost/main.hero` and the 2034 `.hero` files of
`examples/` and `tests/`, 2035 roots), each root checked `--brief` by both,
`xargs -P 8`, 02:59:45 to 03:00:18: **the two differ in one root of 2035**,
`fixedbugs-135-a-dropped-value` (8 against 5). No correct program and no cell
of panel 209's rule loses a refusal; `never_rebound` lines total 34 against
31. Per root on the route over the frozen tree, `@`-parameter messages,
unique: `selfhost/main.hero` 3, `sqlite.hero` 1, fixedbugs-135 8,
fixedbugs-260 6, so **18**, the number section 1 uses.

Priced as a probe: +14/-1 in two files beside `route.diff`, one build.
Not run: the compiler's own tests and the `check` form on `tree-alt`, its
cost in instructions, and the call, chain and operator roots.

## 7. Claims asserted and not measured, and what I could settle

| claim | seat | status now |
|---|---|---|
| a function with an `@` parameter is never a value, so GNAT's 2007 and clippy's #11182 class cannot arise | historian asks, engineer reads `data_errors.hero:172-186` | **settled**: the spec-warden's `p13_value.hero` on `tree/heroes`, `mutable_parameter_as_value`, exit 1 |
| `retains`, `borrows` on the parameter | spec-warden, unrun | **settled by reading**: the checker's `consuming_positions` (`check/consuming.hero:67`) takes `p.consumes \|\| !p.transfers.is_err()`, exactly `ended_by_signature`'s test; `retains` adds a reference and ends nothing (§ 13), so the checker never requires its `@` and the rule owes it nothing. A probe of it, unrun |
| the route's compiler obeys its own rule | engineer, by the 3 edits | **settled**: `tree-route/heroes-route check selfhost/main.hero` exit 0, 0 errors; the seed and fixpoint stay unrun |
| removing the three `selfhost/` `@`s costs no copies (Wirth's question) | historian asks | answered by the engineer's control, not framed as the answer: `ctl/heroes-ctl` retires the trunk's instructions on `check selfhost/main.hero` (84.58 to 84.63 G against 84.55 to 84.63 G). Not re-run here: the machine carries other seats |
| *+0.39%* on `check selfhost/main.hero` | engineer | his three pairs; not re-run (load) |
| the `certain` multi-site fix, *+60 to +90 lines* | engineer | an estimate, **unrun**, and both seats object to the route it prices |
| the resolver's error stops the checker | engineer | measured for `discarded_value` (0 of 15 on fixedbugs-135); not for every checker rule: on the route fixedbugs-260 reads its 5 `aliased_mutable_arguments` beside the 6 `never_rebound` |
| *both repairs are offered and both are `guess`* | spec-warden's approved route | **false of the built rule**: on `p12_shadow_copy` the route gives one fix, *remove the `@`*, and a message that prescribes it (*remove the `@` here and at every call*); applied (`p12_fixed_by_removal`), `check` exit 0 and `run` prints **0**; the write-back (`p12_written_back`, `l @ m`) prints **1**. Her P2 (*one diagnostic, at the parameter, no certain fix*) scores true; the message is what it leaves out |
| P3, D2c's real delta *+9 to +20* | spec-warden | a paid run, unrun, as the brief requires |
| ReSharper's aliasing reason | historian | unverified (HTTP 403), and § 3 removes it anyway |

## 8. Framing facts a seat found false

- The issue's write list (*`@` to the name, a field or an element, or
  through an `@` argument*): incomplete, 13 correct programs refused (both
  seats, measured), and section 2 adds the UFCS end the corrected list still
  misses.
- *`certain` where every caller is in the compiled file set*: `check --apply`
  writes the root file alone (engineer, `cli/certain.hero:43-60`), Heroes has
  no `pub`, and the removal keeps p12's bug (spec-warden). Both object.
- *A method with `@` self*: none exists (§ 9). *A `for` over it*: a read.
- *Does § 5 already cover it?*: § 5 derives acceptance (*a read is a use*);
  the rule needs a word (both).
- The historian's *the issue's own two carve-outs, the C boundary and the
  exported function*: the second has no Heroes form; what stands in for it is
  any module another program `use`s (engineer § 4.2).

## 9. The question the sitting should have asked

**Which repair does the message lead to on the one shape that is a bug?**
The sitting asked what counts as a write and when the fix is `certain`. Its
only thesis instance is p12 (a copy mutated and never written back: a silent
wrong value today, prints 0, spec-warden). Of the 18 sites the route finds
in the tree, the 4 in shipped code are needless `@`s and none a wrong value
(the engineer's § 3, the spec-warden's § 8: *0 of the 33 sites is a forgotten
write-back*); the 14 are golden fixtures of other rules, and 8 of them
(fixedbugs-135) are a dropped write, the `@` right and the line wrong, where
the route's guess is the opposite of the repair (the engineer's § 3). So the
rule's thesis value is p12's shape, and on p12 the built rule's message and
its one fix lead back to the wrong value (section 7, the *both repairs* row). An LLM that reads the message
and does what it says writes `p12_fixed_by_removal`. The spec-warden's
*both repairs offered* is the answer, and it is not in `route.diff`: the
message needs a second clause (*or write `l` where the body meant to*) and a
second `guess`. Its frequency stays the Principle 0 premise nobody measured
(no blind reader tonight; `docs/metrics/operators.md` has no row deleting an
`@` parameter's write, its table read lines 11 to 27).

A second question beside it: both `never_rebound`s, the cell's and the
parameter's, are decided in the resolver before the checker knows which
lines are dropped writes; section 5's trap and fixedbugs-135's masking are
one defect. Whether the count belongs after the checker, or the syntactic
stand-in of section 6 suffices, decides the repair of both, and no seat
asked it of the cell rule that already shipped.

## 10. What would change the resolution, most important first

1. **The built rule refuses `finish(@n)` / `n.node_free()`**, a program § 13
   requires, and its guess leads to one the trunk wrongly accepts (section
   2). The landed rule must count the UFCS receiver of a `consumes` or
   `transfers` parameter, which is the engineer's veto condition applied to
   the shape beside p22; and the trunk's `consumed_borrowed_handle` misses
   the same form, a `blocking` defect to file independently.
2. **The message on p12 prescribes the repair that keeps the bug.** Two
   repairs, both `guess`, as the spec-warden approves; `route.diff` offers
   one.
3. **The spec text**: the engineer's +5 sentence is false of his own rule on
   the 13 consuming helpers; D2c (+13 legacy, +14 cl100k) is the true one
   once the receiver is counted and `@_` is refused. `@_` itself is accepted
   today (section 4) and belongs in the same lane, as defect 608's twin.
4. **The two write lists and the two predictions disagree** on refused
   programs (section 1): the spec-warden's P1 reads 20 where the route reads
   18 at `1dd890751`, and her own silence beside a dropped value would read
   12 or 10. The synthesis picks one list and scores the matching
   prediction; section 6's line-rooted meant write, which moved no correct
   program among the 2035 roots, also closes panel 209's certain-`=` trap
   (section 5), which should be filed whatever 210 decides.

Finished 03:04 by `date`. Everything I built stays in
`.claude/worktrees/scratch-b15/210-critic/` (`tree/`, `tree-route/`,
`tree-alt/`, `alt.diff`, `probes/`, `census/`, `drafts/E5.md`); no file
written elsewhere but this report, no paid run, no commit.

