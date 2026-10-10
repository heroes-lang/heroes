# Panel 209, the spec-warden's report

Started 2026-10-10 16:48:24 CEST (`date`). Folder:
`.claude/worktrees/scratch-b15/209-spec-warden/`, a detached worktree of the
trunk at `87794631` made by the coordinator; no git command run in it.
Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in
that folder, `real 11.92 user 10.64 sys 0.32` (`/usr/bin/time -p`), binary
dated 16:48. Every `./heroes` below is that one; every `heroes measure` is run
from that folder's root and prices on the vendored tables only, **a lower
bound of unknown sign**; no `--refresh` is run. The anchors for the real row
are the coordinator's: trunk 9847, B1 9844, B2 9865, the round 10021
(`00-shared.md`, its table; unverified by me, a paid run). Time box 45
minutes, to about 17:33; what is not reached is written as unrun.

The ceiling today, by grep (`grep -nE '^### 1\.6|10240' docs/design.md`):
`docs/design.md:253` `### 1.6 The spec budget`, `:255` *must fit in 10240*,
`:278` *Raised again to 10240 by author decision 2026-09-14*. The number
this brief carries agrees with the document today.

Written as it goes; sections appended in the order the work ran.

## 1. The drafts as handed to me, measured in my copy (16:49, `date`)

`./heroes measure <file>` from my worktree's root, vendored tables only:

| file | claude-legacy | cl100k_base | real (coordinator's, unverified by me) |
|---|---|---|---|
| `spec/heroes-spec.md` (trunk) | 7348 | 7479 | 9847 (pinned 2026-10-09) |
| `drafts/spec-B1.md` | 7356 (+8) | 7487 (+8) | 9844 (-3) |
| `drafts/spec-B2.md` | 7372 (+24) | 7503 (+24) | 9865 (+18) |

The tool's own last line on the trunk: *Headroom: 393 against the 10240
ceiling, the FFI floor mortgages 60, so what is measured against the ceiling
is 9907 and the check goes red at 10240.* The vendored and the real rows
disagree in sign on B1, as the brief says; the real row judges.

`diff spec/heroes-spec.md drafts/spec-B1.md`: 8 lines each side (43, 131,
132, 134, 150, 291, 293, 410); B2 adds line 135. The critic's count holds.

**Against the round's 10021**: B1 at the coordinator's -3 reads 10018, B2 at
+18 reads 10039; the ceiling is 10240 and the floor-mortgaged line 10180. No
breach on either draft at the real row, by arithmetic on carried numbers.

## 2. The instruments with B2 in place of the spec, in my copy (question 3)

`cp drafts/spec-B2.md spec/heroes-spec.md`, then `./heroes run
tests/harness/main.hero -- ./heroes spec` and `... grammar`, 16:50 to 16:51
(`drafts/suites-B2.out`); the trunk's text put back afterwards and
`cmp`-identical to the trunk's file.

- **`spec`: 19 passed, 4 failed**: `spec/budget` (7503 against the recorded
  7479), `spec/spendable`, `spec/real` (the pinned `9847 ... 2026-10-09` line
  no longer printed), `spec/ledger` (newest row 7479 against 7503). The
  count and pin rows and no other; **`named`, `rejected`, `inventory`,
  `shape`, `anchors`, `offered` stay green with `@=` in the code spans and
  the production under today's lexer.** The critic's reading holds. No row
  waits on the lexer; the four go green when the real count is pasted into
  `suite_spec.hero`, `selfhost/cli/measure.hero` and the ledger in the
  landing commit (`.claude/rules/spec-shape.md` § How a change is made).
- **`grammar`: 9 passed, 0 failed.** `./heroes grammar` with B2 in place
  prints the new production as the document writes it (its line 64,
  `Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .`), and the
  suite compares the compiler's `keyword` and `binary_op` arms to that
  output: a symbol that is not a binary operator and not a keyword moves no
  row. It does not compare a production to the parser; nothing does
  (spec-shape.md § Where the productions are).

**For R5, a keyword, the same two suites move**: `var` is a word the lexer
REFUSES today, `selfhost/keywords.hero:288`, *`var` is not a word in this
language — declare a mutable with `@`: `v: i64 @ 0`*, and the `rejected`
check reads that arm (`suite_spec.hero:365`, `words_in_arms(... "=> ok(ForeignWord(")`),
so an R5 draft with `var` in a code span is red on `rejected` until the lexer
stops refusing it; and a new keyword enters the `keyword` arms `grammar`
reads. Unrun: the R5 draft through the suites (time box); the reading above
is from the suite's source and the arm it names.

## 3. My drafts, vendored, every one `./heroes measure drafts/<file>` (question 1 and 4)

All made by `drafts/make_drafts.py` from `drafts/spec-trunk.md` (a copy of
the trunk's spec) and `drafts/spec-B1.md`; **vendored, a lower bound of
unknown sign**; the real anchors are the coordinator's B1 9844 and B2 9865.

| draft | route | claude-legacy | cl100k_base | delta vendored (both tables) |
|---|---|---|---|---|
| trunk | R0 | 7348 | 7479 | 0 |
| `R0-b-eq` | R0, § 2 example `b: u8 = 255` | 7348 | 7479 | 0 |
| B1 | R1 | 7356 | 7487 | +8 |
| **`W1`** | R1 lean | 7350 | 7481 | **+2** |
| `W1a` | B1 with § 2 `=` only | 7355 | 7486 | +7 |
| `W1-min` | W1, line 132's comment one word | see below | | |
| B2 | R1b | 7372 | 7503 | +24 |
| `W1b-append` | R1b, B2's shape, `@` argument counted | 7374 | 7505 | +26 |
| **`W1b-merge`** | R1b, merged into the use sentence | 7368 | 7499 | **+20** |
| `W1b-merge2` | R1b, merged, clause worded as the append's | 7368 | 7499 | +20 |
| `R2` | `=@` | 7351 | 7481 | +3 / +2 |
| `R3` | `@@` | 7346 | 7477 | -2 |
| `R4` | `:=` | 7345 | 7477 | -3 / -2 |
| `R5` | `var v = 0`, `@` re-binds | 7349 | 7481 | +1 / +2 |
| `R6` | type mandatory on both | 7322 | 7453 | **-26** |
| `R7` | `@` declares, `@=` re-binds | 7350 | 7481 | +2 |
| `R10` | `v: i64 @= 0`, type kept on a cell | 7360 | 7491 | +12 |

**W1 against B1** (`diff drafts/spec-B1.md drafts/spec-W1.md`: three lines):
line 43 `b: u8 = 255` (1 vendored token under `@=`, and legal today: my
probe `drafts/probes/u8_eq.hero` builds and prints 255); line 131's comment
*mutable cell, type inferred too*; line 132's comment *re-binding; only a
cell takes one* (5 vendored tokens together). The eight lines of B1 are
otherwise W1's, and B1's prose sentence at 134 is the cheapest correct one I
found: the alternatives I tried in the head (*`@=` declares a mutable cell,
which `@` re-binds*) move no word that counts. **So B1 is within 6 vendored
tokens of the cheapest statement of R1 and W1 is that statement**; whether
the 6 survive on the reader's table is the refresh's to say (the two tables
disagreed in sign on B1 itself).

**W1b against B2.** B2's sentence costs 16 vendored over B1; the `@`
argument clause costs 8 more appended (W1b-append 24 over W1) and the merge
into the use sentence brings the whole rule to 18 over W1: *An unused
binding or parameter is a compile error, and so is a cell nothing re-binds;
a read is a use and a write is not, except through an `@` parameter, which
re-binds too.* Merging beats appending by 6 here, as spec-shape.md § Where a
rule lives says it does (panel 122). The clause *which re-binds too* hangs
on *`@` parameter* and can be read of the callee's parameter rather than of
the caller's argument; `W1b-merge2` says it as the append does, *a cell
nothing re-binds, an `@` argument counting*, at the same count, and is the
wording I would land. One home: § 5's use sentence, where non-use already
lives.

**R6 is the cheapest document and the dearest language.** It removes § 10's
*an empty one needs an annotation* (every binding has one) and the words
*type inferred*, and adds a type to the four untyped `=` lines inside
fences (`:98`, `:99`, `:130`, `:226`, the critic's count by
`^[[:space:]]*`), 26 vendored under the trunk. What it costs a program is
the shared brief's `=` inferred column: **10225** lines in `selfhost/`, 833
in `examples/`, 8812 in `tests/` (carried), each gaining `: Type`; and it
needs `_` exempted (`_: i64 = f(x)` is not a sentence anyone should write),
which the draft does by splitting `Simple`'s first alternative. design.md
§1.2's formula counts program tokens, not the prompt's: a document 26 tokens
lighter that makes every program heavier by three tokens a binding is the
trade §1.2 refuses in its own words.

**R7 is R1's price moved onto the majority of lines**: the shared brief's
mutations column reads 7553 against 5926 declarations in `selfhost/`
(carried), and the document itself shows it, three more sites moving (`:294`,
`:301`, `:305`) than under R1. design.md §4.4 `:1114`: *you pay tokens only at
the declaration site*, which is §1.5's rule; R7 pays at the use.

**R10 is the dearest symbol route** at +12 vendored, because its production
holds two shapes, `( [ ":" Type ] "=" | ":" Type "@=" )`, where R1's holds
one. It separates the symbol from the inference for the blind seat's
reading, the critic's reason for listing it, and as a landing it buys the
declare/re-bind distinction at the full annotation cost on every cell.

## 4. The narrower cell and the `@` argument (question 2)

**No sentence is needed for the narrower cell under R1.** § 2 already decides
the cell's type on its own line: *a literal takes the type its context asks
for, otherwise `i64`*, the context being the annotation `[ ":" Type ]` or
nothing; § 3 *No implicit conversions, widths included* then refuses `b @
some_u8` at the mutation, which is the `type_mismatch` the critic's
`cell_narrow.hero` shows today and the same rule that refuses `x = 255`
handed to a `u8` parameter today (the critic's `u8_from_i64_binding.hero`).
A reader who wants a `u8` cell writes `b: u8 @= 255`, the shape § 10 and
§ 13 keep showing on `m` and `x`. What the sentence would buy is R8, a cell
typed from a later line, and that contradicts § 3 and design.md §4.5 `:1146`
*errors stay local*; so the silence is the ruling, not a gap.

**B2 owes the `@` argument clause, and B2 owes more than that**: its own
§ 2 example is a cell nothing re-binds. `b: u8 @= 255`, and the only thing
that follows is `b + 1`. Under B2's sentence as written the document carries
a fragment its rule refuses, in the section before the rule. W1 and W1b
write it `b: u8 = 255`; the harness's copy of the sqlite block
(`suite_special.hero:419`, `db: Db @ nullptr` re-bound only through `@db`)
and the document's own `x: cstr @= s.lease()` then `end_lease(@x)` (`:410`
to `:411`) are the two places the `@` argument clause is load-bearing.

## 5. Two homes and the ceiling

- **No draft states a rule in two homes.** `@=` declares and `@` re-binds
  lives in § 5 alone; § 2, § 10 and § 13 carry examples of the form, not the
  rule. The trunk's fence comment at `:132`, *only a declared `@` name can be
  mutated*, restates the prose beside it and B1 keeps that shape with
  `@=`; W1 shortens it and `W1-min` cuts it to one word (measured below).
  B2's never-re-bound sentence has one home, § 5, and the merged W1b puts it
  in the sentence that already governs non-use. No veto on this ground.
- **No draft breaches the ceiling, against the trunk or the round.** The
  tool's own line: 10240, the FFI floor mortgaging 60, red at 10240 with
  9907 measured against it today. On the coordinator's real rows B1 is 9844
  and B2 9865; on the round's 10021 they read 10018 and 10039 by arithmetic;
  the dearest of my drafts, W1b-append, is 26 vendored over the trunk, and
  even at twice that on the reader's table the round's document reads under
  10080 against a line of 10180. The real count is owed at landing, in the
  commit that pastes it, and nothing I can run reaches it. No veto on this
  ground.

## 6. §1.2's arithmetic and Principle 0's burden (question 5)

**What R1 saves a program**, from the shared brief's two files
(`209-coordinator/tokens/`, vendored, carried): 65322 to 48677 claude-legacy
and 72855 to 56590 cl100k over 5926 declaration lines, **2.8 tokens a
declaration**; `selfhost/` holds 10.6 declarations a file (5926 over 557),
so **about 30 tokens a file**. design.md §1.2 prices one correction
round-trip at 500 to 2000. So R1 pays for itself only if it adds **fewer
than one extra round-trip per 17 to 67 files** and removes none; §1.2's own
sentence, *shorten only when it's free*, is the test.

**Where a round-trip can come from under R1, each probed on today's
compiler** (`drafts/probes/`, `./heroes check`):
- `=` written for `@=`: `not_mutable` today (`eq_then_at.hero`, `typed_eq_then_at.hero`),
  a `guess` fix whose text writes the mandatory-type shape, `v: <type> @
  <value>`; the message and the fix move with R1 (the compiler-engineer's).
  A compile error under every route.
- `@=` written for `=`: **silent under R1** (today `v: i64 @ 0` nobody
  re-binds builds, the coordinator's probe), a compile error under R1b. This
  is the one direction R1b closes and the one that decides between them.
- `@=` on a declared name: `shadowed_binding` today (`redeclare.hero`); unchanged.
- `v @ 0` to declare: `unknown_name` twice today, R9's repair; unchanged by R1.
- **The new class**: a cell born of an empty container, a `fail(` or an
  `ok(`, written without its annotation. Today the cell form forces the
  annotation, so `cannot_infer` never fires on a cell; under R1 it fires on
  **1559 of 5926** such cells in `selfhost/` (26%, the critic's corrected
  lower bound) wherever the writer trusts *type inferred*. § 10 says *an
  empty one needs an annotation* in the prompt; whether a model reads it is
  the blind seat's question and nobody's premise.
- **The class R1 removes**: a cell whose annotation was guessed wrong (`n:
  i32 @ 0` then `n @ len(xs)`), which under R1 is `n @= 0`, an `i64`, and
  right. Nothing counts it today.

**Principle 0** (design.md §1.0 `:112`, CLAUDE.md § 2): the compiler does not
need R1; `selfhost/` compiles itself under R0 today. It enters only on a
measured thesis effect. The instruments that exist today: `heroes mutate`
(metric 3, `selfhost/cli/table.hero:158`) over `docs/metrics/operators.md`,
whose rows 13 and 14 name the mandatory type as the catching rule and whose
table is data; `heroes measure` for the program tokens above; and the blind
seat's six sessions (`209-blind/run.sh`, the critic's reading), the only one
of the three that reads a rewrite rate. **What discharges the burden**: the
blind seat's B2 arm showing no more first-compile errors at binding lines
than its A arm, over programs whose binding lines are shorter; and under
R1b, `forget-at-decl` re-planted as `v @= 0` to `v = 0` with a later `v @ e`
killed 100% (`not_mutable`), `mutate-undeclared` killed 100% (`unknown_name`),
and a new row, `x = e` to `x @= e` with no later write, killed 100% under R1b
and surviving 100% under R1, which is the measured argument for R1b over R1
and the one `docs/measurements/007`'s shape can score. **What falsifies it**:
more binding-line errors in B2's three sessions than in A's three, or a
`cannot_infer` on a cell in any B2 session.

**Corrections to § 3's table, measured after it was written** (`./heroes
measure`, 16:58 and 17:00): `W1-min` reads **7344 / 7475, -4 under the
trunk on both tables**, the only R1 draft below R0 vendored, by cutting line
132's comment to the one word *re-binding*, the clause it drops being a
restatement of the prose one line below (the trunk's doubled shape, § 5
above). `W1b-min`, W1-min with W1b-merge2's sentence, reads **7362 / 7493,
+14** over the trunk. Those two are the drafts I would land for R1 and R1b;
their § 5 is in `drafts/spec-W1b-min.md:129-138`. The round's spec differs
from the trunk's in four hunks, 113-115, 159-160, 277-282 and 348-352
(`diff`, read-only, 17:00), none of them a line the drafts move, so the
drafts apply to the round with the lines after 277 renumbered.

## 7. Verdict per route

- **R0**: approve. Nothing to pay; R9 lands under it as under every route.
- **R1, as B1**: object, provisional. B1 is correct and within 12 vendored of
  the cheapest statement; `W1-min` is that statement, -4 vendored, its
  named removal the restated fence clause and the `@=` on a § 2 example
  that never re-binds. The objection is design.md §1.0's: the compiler does
  not need it and no thesis effect is measured yet; §1.2's arithmetic (30
  tokens a file against 500 to 2000 a round-trip) makes the rewrite rate the
  whole case, and the instrument that reads it is running in this sitting.
- **R1b, as B2**: object as drafted, and the route I would approve over R1
  once the reading is in. B2 refuses its own § 2 example and leaves the `@`
  argument unstated; `W1b-min` repairs both at +14 vendored, one home. It
  is the half §1.2 favours: it turns R1's one silent direction (`@=` for
  `=`) into a compile error, which metric 3 can score.
- **R2 `=@`**: object. Same price as R1 (+3 / +2) and the byte pair already
  lives in the recovery corpus as the planted mutant `named-arg-equals`
  (`plan-singles.jsonl:4460,4552`, `out=@db`): under R2 that mistake lexes
  as a declaration symbol inside a call. `=` first is `=` doing double
  duty, design.md §4.4 `:1087`'s reason against `=`+`var`.
- **R3 `@@`**: object. -2 vendored, and `@@v` is a known mistake with a
  certain fix today (`selfhost/parse/at_prefix.hero:62-63`, goldens
  `fixedbugs-131-a-sigil-*`, `-179-*`); §4.4 `:1093` gives `@` its
  reading, *position, slot*, and a doubled glyph none.
- **R4 `:=`**: object, on design.md §4.4 `:1086`, `::`/`:=` refused as
  confusable, and the document uses `::` at § 9. -3 vendored buys back no
  recorded refusal.
- **R5 a keyword**: object. +1 / +2 vendored; `rejected` is red until the
  lexer stops refusing `var` (`keywords.hero:288`), a word every program
  then loses, and a word and a symbol become two vocabularies for one
  distinction the symbol makes alone. §4.4 `:1087`'s refusal of `=`+`var`
  was of `=` re-binding; R5 keeps `@` for that, so the record should say
  the old reason does not apply as written, and this one does.
- **R6**: object, on §1.2. The cheapest document (-26) and the dearest
  programs: 10225 `=` lines in `selfhost/` alone gain a type (carried),
  and `_` needs an exemption the draft had to write.
- **R7**: object, on §4.4 `:1114` and §1.5: the price moves to the majority
  of lines (7553 mutations against 5926 declarations in `selfhost/`,
  carried), and three more lines of the document move than under R1.
- **R8**: object. The sentence it needs contradicts § 3 *No implicit
  conversions* and design.md §4.5 `:1146` *errors stay local*; no draft.
- **R9**: approve. Zero spec tokens; design.md §4.17's promise; independent
  of the route. Two more diagnostics write the declaration's shape and move
  with whatever lands: `not_mutable`'s `guess` fix (`v: <type> @ <value>`)
  and `var`'s message (`keywords.hero:289`).
- **R10**: object. +12 vendored, two production shapes for one rule; the
  blind seat's separating variant, not a landing.
- **R11 `@v = 0`**: object; occupied by `at_prefix`'s recovery.

## 8. Prediction, condition, veto

**Prediction, three parts, each with its instrument and the point it is
scored:**
1. Tokens: `W1-min` applied to `spec/heroes-spec.md` and refreshed
   (`heroes measure spec/heroes-spec.md --refresh`, the landing commit)
   reads **below B1's 9844**; `W1b-min` reads **below B2's 9865**.
   Falsified by either reading at or above its anchor.
2. Rewrite rate: in the blind seat's six sessions (`209-blind/run.sh`,
   scored at panel 209's synthesis), the three B2 sessions show **no more
   first-compile errors at binding lines** (`not_mutable`, `unknown_name`
   at an `@` line, `shadowed_binding` at a declaration, `cannot_infer` on
   a cell) than the three A sessions. Falsified by one more, or by any
   `cannot_infer` on a cell under B2.
3. Metric 3 (`heroes mutate`, `docs/metrics/operators.md` extended, the
   landing gate): under R1b's compiler `forget-at-decl` re-planted as
   `v @= 0` to `v = 0` with a later `v @ e`, `mutate-undeclared`, and a new
   row `x = e` to `x @= e` with no later write each kill **100%**; under
   R1's compiler the new row **survives 100%**. Falsified by any survivor of
   the first two under either, or by a kill of the third under R1.

**Condition**: R1b becomes approve on prediction 2 holding and prediction 1's
refresh under the ceiling; R1 alone stays object while the third row
survives. The verdict becomes **veto on §1.0** if the resolution lands with
neither the blind reading nor a registered prediction naming it, and **veto
on §1.6** if the landing's refresh reads 10180 or more on the round's base,
which no draft here approaches.

**Veto**: none today. No draft breaches 10240 against the trunk's 9847 or
the round's 10021 on any number I can reach, and no draft states a rule in
two homes; the one draft that contradicts itself, B2 at § 2, is repaired in
`W1b-min` and not vetoed.

Finished Sat Oct 10 17:01:18 CEST 2026 (`date`). Unrun: the R5 draft through `spec` and
`grammar`; the real row of every draft (a paid refresh); the round's
`heroes grammar` with a draft in place.

---

- **verdict**: object, provisional (every count of mine is vendored)
- **section**: design.md §1.0 (the burden), §1.2 (the formula), §1.6 (the ceiling and the payment rule), §4.4 `:1102-1115` (the mandatory type's two reasons)
- **spec_token_delta**: B1 +8 / B2 +24 vendored (real, carried: -3 / +18); my W1-min **-4** vendored, W1b-min **+14** vendored; real rows unrun
- **removal**: the fence comment's restatement at § 5 `:132` and the `@=` on § 2's never-re-bound example, both in W1-min; for R1b's +14, the registered prediction above
- **needed_for_self_hosting**: no
- **argument**: The symbol answers §4.4's two reasons for the mandatory type, locality and the `totl` typo, without the type: `@=` is the declaration's own shape and `@` on an undeclared name stays `unknown_name`. What the type still bought is gone only where the cell's value cannot say it, 26% of `selfhost/`'s cells, each a `cannot_infer` for a writer who trusts *inferred*. At 30 tokens a file against a 500-token round-trip, §1.2 makes the rewrite rate the whole case, and the blind seat is reading it now. B2 as drafted refuses its own § 2 example; W1b-min repairs it, counts the `@` argument, and sits at +14 vendored in one home.
- **prediction**: W1-min refreshes below 9844; B2's three blind sessions show no more binding-line errors than A's; the `x = e` to `x @= e` mutant survives 100% under R1 and 0% under R1b
- **condition**: the blind reading in hand and a refresh under 10180 on the round's base turn R1b to approve; a landing without the reading or a registered prediction naming it turns this to veto on §1.0
