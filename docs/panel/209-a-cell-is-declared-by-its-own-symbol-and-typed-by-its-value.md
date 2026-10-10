# Panel 209: a cell is declared by its own symbol, `@=`, and typed by its value as a binding is

Convened 2026-10-10 by the author's request, after a conversation in which
they asked why a mutable declaration carries a type and an immutable one
does not, and proposed the form below; **the author's framing, binding on
every seat: the language is alpha, the last release is v0.2.0 (`git tag
--list 'v*'`), nothing is beta, everything may break, and the migration's
cost is work to be measured, never a reason against.** Full panel: four
seats as subagents, each in a detached worktree of the trunk frozen at
**`87794631`** (`git status` clean but another session's panel 208
folders), the blind seat as **24 fresh `claude -p` sessions**, the
completeness critic before the seats and after them. Briefs written from
16:07 to 16:30, the critic's first pass 16:32 to 16:43 (twelve corrections
and four routes, applied before any seat started), the seats launched at
16:48 and back by 17:17, the blind seat's four waves 16:48 to 16:54, the
critic's second pass 17:18 to 17:30, this synthesis from 17:33, every time
read from `date`. Briefs in `209-briefs/` (the blind seat's in
`209-briefs/blind/`), reports in `209-reports/`, the blind seat's 24 reports
and its scoring beside them.

## The proposal, verbatim (the shared brief's)

Today (spec § 5):

```
x = 5              # immutable binding, type inferred
v: i64 @ 0         # mutable declaration — the type is REQUIRED
v @ v + 1          # mutation; only a declared @ name can be mutated
```
    Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .

The type on a mutable declaration is the mark that tells a declaration from
a mutation, since both lines carry `@` (design.md §4.4 `:1102-1115`). The
proposal gives the declaration a symbol of its own, so the type becomes
optional on both forms under one rule. Draft B1:

```
x = 5              # immutable binding, type inferred
v @= 0             # mutable cell, type inferred the same way
v @ v + 1          # mutation; only a cell declared with @= can be mutated
```
`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it,
or a field or element inside one.

    Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .

B2 is B1 plus *A cell nothing re-binds is a compile error: write `=`.* The
routes: **R0** status quo; **R1** `@=` (B1); **R1b** R1 and the
never-re-bound rule (B2); **R2** `=@`; **R3** `@@`; **R4** `:=`; **R5** a
keyword; **R6** the type mandatory on both; **R7** the inverse; and from the
critic's first pass **R8** the type from all writes, **R9** keep R0 and
repair the `v @ 0` message, **R10** `v: i64 @= 0`, **R11** `@v = 0`.

## What the sitting measured

| fact | reading | by |
|---|---|---|
| `@` declarations / mutations / `=` inferred / `=` annotated in `selfhost/` (557 files), by the shared brief's anchored patterns | 5926 / 7553 / 10225 / 847 | coordinator, re-run by the critic |
| `.hero` files holding a `@` declaration | 1671 of 3229 | coordinator |
| `selfhost/` declarations inside `test` blocks | 1190 of 5926 | critic |
| the spec, real tokens: trunk / B1 / B2 / W1-min / W1b-min / round b18 | 9847 / 9844 / 9865 / **9827** / **9850** / 10021 | coordinator, `--refresh` |
| the 5926 declaration lines of `selfhost/`, vendored tokens, as written and as `@=` | 65322 and 48677 (claude-legacy) | coordinator |
| a cell nothing re-binds, today | accepted, exit 0 | coordinator |
| `v @= 0`, today | `expected_expression` at the `=`: `@` before `=` legal nowhere | coordinator, critic |
| an inner block re-declaring an outer name, `@` or `=` | `shadowed_binding` | coordinator, critic on `heroes-r1b` |
| the rule's birth | `57d8f27f`, 2026-08-03, the first commit; no sitting had it on the table | coordinator |
| **R1 built** (token, parser, resolver, checker, formatter, dump) | 180 diff lines over 19 files, +30 by `wc -l`; `check` and `build` green; build time unchanged (ratio 1.09 before, 1.07 after, under the lanes' load) | compiler-engineer |
| **R1b built** (the unused sweep extended, `cannot_infer` on a `nullptr` birth, `bump(@=n)` one message with a certain fix) | 249 diff lines over 23 files, +99 | compiler-engineer |
| `layout` under R1 / R1b | `check/walk.hero` +5 / +10 over 1870, `grammar_expr.hero` +8 over 1085, `selfhost/parse/` +15 over 8699 under R1b; each with a seam the language allows | compiler-engineer |
| the silent `fmt` migration | does not exist: the comment guard refuses 499 of 557 `selfhost/` files, 90 of 120 `examples/`, 683 of 1697 `tests/` | compiler-engineer |
| what R1b refuses in the tree as it stands | 151 cells in `selfhost/` (120 in `test` blocks), 19 in `examples/`, 5 in the harness, **58 in `tests/golden/run/`** (28 files): 233 so far, the other golden forms uncounted; the 16 read are every one a `=` in disguise | compiler-engineer, critic |
| what keeps its annotation under R1, on a stripped copy of `selfhost/` | 1657 of 5926: 1579 born `[]`, `{}`, `fail(`, `ok(`, `nullptr` or a multi-line array; 55 case names; 8 arrays; **15 width cells** told at a distance with no fix; 4269 (72%) may drop it; stripping also unreads 10 imports | compiler-engineer |
| the compiler's own tests under R1b | 1542, 5 failed: three expectation texts pinning the old form, R1b firing beside an `@` the resolver refused (three lines), one enclosing-fix case unread | compiler-engineer |
| the C boundary | no `extern` line in 697 files holds an `=`; the emitted C of a cell is its type's, never its spelling's (three bindings built and run); of 165 cells in the 21 FFI example files, 137 drop a restated type, 22 keep it by `cannot_infer`, 6 keep it unasked | ffi-pragmatist |
| `nullptr` today | infers `ptr`, never `cannot_infer`: under R1 alone `db @= nullptr` is refused at `sqlite3_open(…, @db)` with `type_mismatch` and no word of the declaration (the engineer's p13 under `heroes-r1`); under `heroes-r1b` `cannot_infer` at the declaration with a `guess` fix naming the annotation | ffi-pragmatist, compiler-engineer |
| B2 against its own document | `b: u8 @= 255` in § 2, re-bound by nothing, is refused by B2's sentence (`never_rebound` 2:5 on `heroes-r1b`) | spec-warden, critic |
| the `spec` suite with B2 in place / the `grammar` suite | 19 passed and 4 failed (count and pin rows only) / **9 passed, 0 failed** | spec-warden, critic |
| **the blind seat**, 3 tasks × 2 variants × 4 readings | **4 of 4 in every cell**: under B `@=` found and applied, the annotation kept on the empty map and declined where the value typed the cell, `=` kept where nothing re-binds; under A the `: T @` form every time; t3 read right under both; no reader wrote `@=` for a mutation (0 of 12); estimate 5.69 USD at list price on the subscription | coordinator's scoring, critic |
| the t3-B program under `heroes-r1b`, byte for byte | `unknown_name` 4:9 with the certain fix and `never_rebound` 8:5 on `limit`, **nothing at line 2**: two messages for two real mistakes, not the cascade the four t3-B readers predicted | critic |
| the blind seat's two B programs under `heroes-r1b` | exit 0, print `abcd` and `2`, as the A programs do under the trunk | compiler-engineer, coordinator |

## The verdict table

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve R1 with R1b and R9, landed as R10 first**; object R0, R2 to R7, R11; **veto R8** | §1.1, §1.7, §4.4 `:1102-1115`, §4.5 `:1144`, §4.17 | core, not sugar: about a hundred lines in the lexer's table, the parser, the resolver, the checker and two printers; three ceilings by 5 to 15 lines, each with a seam; one new class of distant error, 15 width cells; `needed_for_self_hosting: no` | at the landing's gate `layout` reads 0 failed with the two knots at or under their ceilings and `selfhost/parse/` at or under 8699, through one leaf module under `selfhost/check/` of at most 40 lines and `lent_with_eq` outside `parse/` or a DECIDED row; the census over the migrated tree refuses 0 `never_rebound` after the rewrites to `=` (its count of 55 + 8 + 15 is on the stripped copy, the critic notes, and the landing strips no annotation) | object if the leaf helper cannot bring `check/walk.hero` under 1870; if `heroes check --apply` does not migrate the 1671 files in one pass; or if the narrower-born class passes about one cell in a hundred over the whole tree; R1b to object if any refused cell is not a `=` in disguise (a lane reads all; it read 16); R8's veto lifted by a route that types a cell from its writes with no second pass and no silent widening |
| ffi-pragmatist | R0 approve; **R1 object, not veto**; R1b object as drafted; R2 to R7 object; R8 object more firmly; **R9 approve; R10 approve**; R11 refuse. **No veto** | §1.11, §4.19's first sentence, §4.17 | the boundary is the same under every route; what moves is where a binding's error lands | the R1 prototype without a `nullptr` rule prints `type_mismatch: expected Db, found ptr` at the `@db` argument and no `cannot_infer` at the declaration (**true under `heroes-r1`, the engineer's p13; discharged under `heroes-r1b`**); after migration `grep` reads 6 and 26 annotated `@= nullptr` cells and 0 bare | R1 to approve when a bare `nullptr` birth under `@=` is `cannot_infer` at its line with a `guess` fix naming the annotation, and `(@=x` is one message with a `certain` fix (**both built in step 1b**); R1b needs its `@`-argument clause (**the sweep counts it already, `at_arg_only.hero` exit 0; the text owes the words**) |
| spec-warden | R0 approve; R1 object provisional (W1-min is the text); **R1b the route it approves once the blind reading is in** (W1b-min); R2 to R8, R10, R11 object; **R9 approve**. **No veto** | §1.0, §1.2, §1.6 (ceiling 10240), §4.4 | W1-min 4 under the trunk vendored, **9827 real**; W1b-min +14 vendored, **9850 real**, the rule merged into § 5's use sentence, § 2's example `b: u8 = 255`; R1 saves about 2.8 tokens a declaration against 500 to 2000 a round-trip | (1) W1-min below 9844 and W1b-min below 9865 (**true, 9827 and 9850**); (2) B's blind sessions no more binding-line errors than A's (**true, 0 and 0, and no `cannot_infer` on a cell under B2**); (3) under R1b's compiler the operators `forget-at-decl` re-planted, `mutate-undeclared` and a new row `x = e` to `x @= e` kill 100%, the new row surviving 100% under R1 alone (the landing's gate; the rows are owed first, the critic notes) | veto on §1.0 absent the blind reading (in); veto on §1.6 at 10180 or more on the round's base (no draft approaches) |
| historian (advisory) | object R0, R2, R3, R4, R6, R7, R8, R11; approve R1 conditional, **R1b the strongest historically**, R5 precedent-rich, R9 as a floor, R10 as the measurement arm | the historical appendix | Go issue 377 (2009, open; *we simply disagree*, 2025-02-04): the `:=` hole is shadowing; Oberon's `:=` assignment only; Turbo Pascal's typed constants the one shipped type-as-mutability mark, repaired by `{$J}`; **Zig 0.12.0 (2024-04-20) makes a never-mutated local a compile error and found three bugs in its own tree**; PEP 465's `@=`; Ritchie's `=+` *a mistake, repaired in 1976*, R2's shape; Hylo's `var x` then `&x = 2`, R5's shape | (1) B2 over `selfhost/` flags more than zero cells (**true, 151**); (2) at least one B2 session writes `v @= <expr with v>` as an update (**false, 0 of 12**: the hazard it weighed against R1 did not show); (3) the inverse operator over the 839 planted declarations survives under R1 and is caught under B2 off the write path (the landing's gate, rows owed) | R1 and R1b unconditional on the tree refusing an inner block declaring an outer name (**true, `shadowed_binding`**); R5 overtakes R1b on prediction 2 (**it did not**); R0 recovers on a `{$J+}` default shown wanted |
| llm-ergonomist (24 blind sessions, `llm-ergonomist-scoring.md`) | 4 of 4 in every cell under both variants | the specification alone | twelve sessions saw the project's git status in their context (nothing naming the rules or the question), twelve re-run as their own empty repositories saw only that and the folder's path; all 24 concordant; no `context` answer voids a reading (the critic's reading of all 24) | the prediction the blind seat scored is the other seats': found and applied 4 of 4, read wrong 0 of 12, the annotation kept 4 of 4 where the literal was empty | a program longer than nine lines, a cell born narrower than a later write and the `.case` birth (55 in `selfhost/`, `cannot_infer` under R1) were not among its tasks |
| completeness critic | pass 1: twelve corrections to the briefs and routes R8 to R11; pass 2: the contradictions above settled on `heroes-r1b`, two routes nobody listed, the commit shape | | | | |

## Disagreements, stated plainly

- **The engineer against the four t3-B readers** on the typo's cascade: the
  readers predicted two messages (lines 4 and 2); the engineer's sweep
  suppresses the second where the name was offered as a repair; the critic
  ran the readers' program byte for byte on `heroes-r1b` and the engineer is
  right. Both are kept: the readers' prediction is what a reader of B2's text
  expects, so the text or the message should say that a cell whose only
  re-binding is a near-miss is not told twice.
- **The warden against the other three on R10**: the warden objects to R10
  as the landed language (two production shapes for one rule, +12 vendored);
  the engineer, the ffi seat and the historian approve it as the **first
  commit**, which isolates the symbol's effect and keeps every width and
  handle on its line while the inference lands apart. No conflict once the
  record says which: R10 is a commit, not the resolution.
- **The ffi seat against the engineer on `@` arguments under R1b**: the ffi
  seat required a clause; the engineer showed the sweep already counts an
  `@` argument as a write (`writes.inout_root`), and the critic's probe
  confirms it. The clause is still owed in the spec's text (W1b-min has it:
  *an `@` argument counting*).
- **Three denominators for *what keeps the annotation***: the shared brief's
  1559 (four classes), the engineer's 1657 (the same plus `nullptr`,
  multi-line arrays, 55 case names, 8 arrays, 15 width cells, on a stripped
  copy run through `check`), the ffi seat's 22 + 6 of 165 FFI cells. The
  critic reproduced all three; the engineer's is the measured keep count for
  `selfhost/`.
- **The engineer's census prediction** counts an annotation-stripping
  migration; the landing it proposes keeps every annotation. The critic
  registers it as a count on the stripped copy, not a gate number.
- **R5 against R1b**: the historian finds R5 (a keyword at birth, `@` at
  every write) the precedent-rich shape and R1b the strongest; its tie-break
  was the blind seat, which showed the `op=` hazard 0 of 12 times. The
  warden and the engineer object to R5 on cost (a reserved word in 3229
  files, six keyword tables, the `rejected` check red until the lexer moves).

## The resolution, provisional — author ratification pending

Adopted by CLAUDE.md § 4's rule, the most robust and complete, never the
cheapest: **R1b with every condition the seats built, landed in the order
the engineer measured, R9 first and R10 before R1.** In full:

- **R1** `@=` declares a mutable cell and `@` re-binds it (a name, a field
  or an element inside one); the type is optional on both `=` and `@=` and
  demanded by `cannot_infer` where the value cannot say it: an empty
  container, `fail(`, `ok(`, a case name, and **a bare `nullptr`**, told at
  the declaration with a `guess` fix naming the annotation and the cell
  poisoned so the call below says nothing more (the engineer's one line
  owed). `v: Type @ value` is refused with a `certain` fix writing `@=`
  (`parse/annotation.hero` beside `no_binding_symbol`).
- **R1b** a cell nothing re-binds is a compile error, `never_rebound`, with a
  `certain` fix writing `=` and keeping the annotation; a re-binding is a
  write by `@` to the name, a field or an element, or through an `@`
  argument; the rule is silent where the name was offered as a repair
  (`suggested`) and where the resolver already refused its only write; a
  cell never read is told `unused_binding` and not this.
- **R9** the `unknown_name` of a mutation's place carries, beside the
  rename, a `guess` fix writing the declaration (`totl @= …`), and the
  diagnostics that write the declaration's shape move with it
  (`not_mutable`'s fix, `keywords.hero`'s `var` and `let` messages, the
  `cannot_infer` fix text gaining a `what` so it writes `@=` for a cell).
- **The width class** (a cell born of a literal and later written a width
  type, 15 sites in 4 files of `selfhost/`): the `type_mismatch` at the
  mutation gains a note naming the cell's declaration line and a `guess` fix
  annotating it; and **the `op=` and inner-block pair** (`v @= v + 1` or
  `v @= 1` in an inner block on a declared `v`): one message with a `certain`
  fix `v @ v + 1`, in place of today's prototype's `shadowed_binding` plus
  `never_rebound`.
- **`(@=x`**: one message with a `certain` fix `write @x` (built,
  `at_prefix.lent_with_eq`).
- **The spec's text is W1b-min** (`209-spec-warden/drafts/spec-W1b-min.md`,
  **9850 real** by the coordinator's refresh, +3 on the trunk): B1's lines,
  the never-re-bound rule merged into § 5's use sentence (*and so is a cell
  nothing re-binds, an `@` argument counting*), § 2's example `b: u8 = 255`,
  the fence comment *re-binding*; the production
  `Binding = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`. design.md
  §4.4's *Why the type is mandatory* is rewritten to say what tells the two
  lines apart now and why the unused-rule half of its reason had already
  moved (spec § 5 `:138`, 2026-08-04).
- **Refused on the seats' measured grounds**: R0 (the one precedent is a
  wart, and the never-re-bound gap is open under it); R2 (`=@` is a planted
  mutant one keystroke from `name: @x` at 500 sites, Ritchie's `=+`); R3
  (`@@v` is a recovered mistake); R4 (`:=` refused at §4.4, two readings of
  one colon); R5 (a reserved word for a distinction the symbol makes alone;
  recorded as the precedent-rich route the author may choose); R6 (10225
  lines pay for nothing); R7 (reopens §4.4's typo hole); **R8 vetoed** by
  the engineer (a flow-sensitive join in a knot past its ceiling, against
  §4.5); R11 (occupied by `at_prefix`'s recovery). **R10 is the first
  commit** of the landing and not the language.

**What conservative would have been**, so the author can choose it: **R0
with R9 and the never-re-bound rule** (the rule is an extension of the
unused sweep and needs no new symbol; it closes the one silent gap the
sitting found, 233 cells so far) — no migration, no token, no ceiling moves,
and the asymmetry kept. What it gives up is measured: one rule for two
forms, about a quarter of the tokens on 5926 + 777 + 2019 declaration lines
(2.8 a declaration by the warden's arithmetic), and the reader's one
question answered by the symbol instead of by §4.4's paragraph. The blind
seat read both forms right 4 of 4, so the symbol's case rests on §1.2's
tokens and §1.5's uniformity, not on a mistake it alone catches.

**The landing, as the seats bound it** (a milestone the author schedules,
its name two words naming the deliverable, the inferred cell; the sitting
names no id, since a name enters `docs/roadmap/names.md` with its row): commit 1 R9 and the never-re-bound rule **with its rewrites to `=`
in the same commit** (151 + 19 + 5 + 58 and the other golden forms, each
read as a `=` in disguise; `heroes test selfhost/main.hero` and the net do
not compile between), the `@`-argument shape counted; commit 2 the `@=`
token, the printers and the 1671-file rewrite **in one commit** through
`heroes check --apply`'s certain fix (the formatter's comment guard refuses
the silent route, and `canonical` is red between the token and the tree),
with the 23 `.expected`, 32 `.applied`/`.fixed`, the harness's cells, the
three ceilings (one leaf module under `selfhost/check/`, `lent_with_eq`
outside `selfhost/parse/` or a DECIDED row saying why the budget moves,
`layout` 0 failed), the five own-test shapes, the 10 unread imports; commit
3 the optional type and the inference, with the `nullptr` rule, the width
note and fix, the `cannot_infer` `what`; then the spec's W1b-min with its
`--refresh` and the four pasted rows, design.md §4.4, `operators.md`'s rows
13 and 14 rewritten and the two new rows (the inverse operator and `x = e`
to `x @= e`), the 839 mutants re-planted and panel 187's R2 after the push,
`site/src`'s 25 lines (a push publishes), the TextMate grammar and
`highlight.ts`, the 132 `@` strings of `seed/heroes.c`'s messages moving
with their sources, the census over every golden tree, the seed at the
batch's close. **Two decision issues** leave this sitting: this
ratification, and the critic's **`@` parameter the callee never writes**,
accepted today and under the prototype, the same class one level up.

## What a veto would compel

No seat vetoed the adopted route. The engineer's veto binds R8, which is not
adopted. If the author overturns R1b, the fallback that every seat approved
is R9 with the never-re-bound rule under today's syntax; if they overturn
the rule as well, R9 alone, at zero spec tokens.

## Predictions to score

| whose | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | `layout` 0 failed with `check/walk.hero` at or under 1870, `grammar_expr.hero` at or under 1085, `selfhost/parse/` at or under 8699, through one leaf module of at most 40 lines; the census refuses 0 `never_rebound` after the rewrites | `heroes run tests/harness/main.hero -- ./heroes layout`; the census | the landing's gate |
| compiler-engineer | `heroes check --apply` migrates the 1671 files in one pass, a second pass moving none | `suite_fixes`' own assertion | commit 2 |
| spec-warden (3) | under R1b's compiler `forget-at-decl` re-planted, `mutate-undeclared` and the new row `x = e` to `x @= e` kill 100%; the new row survives 100% under R1 alone | `heroes mutate`, `operators.md` extended first | the landing's gate |
| historian (3) | the inverse operator (`@=` where `@` was meant) over the 839 planted declarations survives under R1 and is caught under B2 where the original cell is then never re-bound | `operators.md`, panel 187's R2 | after the push |
| ffi-pragmatist | after migration `grep` reads 6 and 26 annotated `@= nullptr` cells in `examples/` and `tests/` and 0 bare | `grep` | commit 3 |
| the sitting | over the whole tree the narrower-born class stays under one cell in a hundred (15 of 5926 in `selfhost/`) | the migrated tree's `check` | commit 3 |

Scored inside the sitting: the warden's (1) and (2) true (9827, 9850; 0 and
0); the historian's (1) true (151) and (2) false (0 of 12); the ffi seat's
true under `heroes-r1` and discharged under `heroes-r1b`.

## What the sitting leaves on disk

The seats' worktrees under `.claude/worktrees/scratch-b15/209-<seat>/`,
ignored by git: the engineer's holds step 1b built and formatted, four
compilers (`heroes-seed`, `heroes-pristine`, `heroes-r1`, `heroes-r1b`), its
`209-notes/` with the pristine, migrated and stripped copies and fourteen
probes; the ffi seat's `ffi-probes/`; the warden's `drafts/`; the critic's
probes; the coordinator's `209-coordinator/` with the measure folder, the
probes and the token files; the blind seat's 24 folders. The landing lane
starts from the engineer's worktree and removes the rest.

## Author's verdict

Pending. The decision issue is
`issues/2026-10/10/2026-10-10-1736-panel-209-ratify-amend-or-overturn-a-cell-declared-by-its-own-symbol.md`.

**Ratified, 2026-10-10 17:57** (the line above stood from 17:33 to 17:57): the
author, in conversation, meant as *all right, then I ratify the panel; and
then on to the push*, after weighing on the human side that the small
asymmetry at the declaration is the price for the mutation standing out,
and after asking the coordinator's reading as a model, which agreed for its
own reasons (the common act keeps the common symbol `=`, the rare act the
rare one, and the line a reader must find in a block is the rare one; `:=`
carries two priors that contradict each other, Go's and Pascal's, where
`@=` carries one weak one that 0 of 12 blind readers followed). A variant
the author raised in the same conversation, **`:=` for the once-bound name,
`@=` for the cell and bare `=` for the mutation**, was weighed and set aside
unmeasured: every confusion among its three forms is still a compile error
(a name declared twice is shadowing, `=` on a non-cell is `not_mutable`, a
bare `=` on an undeclared name is `unknown_name`), but it moves the loud
glyph from the mutation to the birth against §4.4's reason, gives the most
habitual symbol to the act a reader must find, and rewrites every binding
line of the tree rather than the cells'. Recorded as a reading (CLAUDE.md
§ 4). The critic's `@`-parameter question stays its own open decision; the
landing is a milestone the author schedules.

## Scored at the landing, 2026-10-10

Scored by the landing lane at the milestone's close, each by the command its row names (the journal is `docs/records/journal/062-inferred-cell.md`):

- **compiler-engineer, the three ceilings and the census**: `layout` 0 failed at every step; `check/walk.hero` at 1870 and `grammar_expr.hero` at 1084 by the suite's unit, both held; **`selfhost/parse/` at 8735, not 8699**, raised with its reason beside the row (the prediction's *or a DECIDED row saying why the budget moves*), and the leaf outside `parse/` is `cell_errors.hero`, 46 lines; the census refused 0 `never_rebound` after the rewrites: **true in three of four parts, the budget false as a number and met as the row allowed**.
- **compiler-engineer, one pass of `check --apply`**: the tree moved in one pass over every root, by hand at the two cases where `--apply` stops at the lexer (a bracket left open), and a second pass over the examples was owed by a restore from HEAD, not by the fix: **true of the fix, false of the pass as a single command**; `suite_fixes` 965 and 0.
- **ffi-pragmatist**: after migration `grep` reads **6 annotated `@= nullptr` cells in `examples/` and 27 in `tests/`** (28 with the landing's own golden) **and 0 bare** in the programs that build; the one bare cell is the golden that refuses it. **True**, 26 having become 27 by a case landed between the sitting and the migration.
- **the sitting, the narrower-born class**: every annotation kept, so the migrated tree's `check` refuses 0 width mismatches; the class's count under inference is the sitting's 15 of 5,926 by construction, nothing having been stripped. **True as the landing was built, not as the stripped copy was measured.**
- **spec-warden (3) and historian (3)**, the kill rates of `inverse-at`, `bind-as-cell`, `forget-at-decl` and `mutate-undeclared`: panel 187's R2 after the push, as the chain says; **pending, not lapsed**.
