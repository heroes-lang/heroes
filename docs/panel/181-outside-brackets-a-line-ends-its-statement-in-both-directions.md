# Panel 181: outside brackets a line ends its statement, in both directions

2026-09-28, M-agreed-retention, at the trunk `0fc98107`, which the seats copied
and nothing in the sitting changed. **Not a retro-record**: defect 116 was filed
as a compiler admitting a form the ratified design excludes, to be repaired at a
sitting, and the sitting decides the repair. Briefs: `docs/panel/181-briefs/`.
Reports: `docs/panel/181-reports/`. The historian's (no write tool) and the
llm-ergonomist's (the harness refused the seat's file) were written out by the
coordinator from their final messages, verbatim, with a header. The
coordinator's key to the ergonomist's fragments and the scoring of two
predictions are in
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/coordinator-key/key.md`,
which the critic re-ran.

**Four seats and a critic, not five**, on CLAUDE.md § 4's *choosing only the
seats whose input differs* (CL-023): the ffi-pragmatist's input is the C a
binding needs, and no route reaches C. Written so the author can overrule it.
Panel 182 sat in the soundness lane on the same machine at the same time; its
seats copied the same commit, and nothing either sitting read was changed while
the other ran.

**Five errors in the shared brief, all found by seats.**
- *The 61-kind match*: `variant TokenKind` has 69 cases, 17 answering `true` in
  `is_line_ender` and 52 `false`; the 61 is `selfhost/layout.hero:12`'s own
  comment, a premise that expired in the source (critic).
- *19 hits after an `error` token* in one place and 16 in another: 16 at the
  same margin, 3 more before an INDENT, 6 more before end-of-file dedents
  (critic).
- *The compiler implements the clause panel 007 rejected*: it implements panel
  007's RATIFIED mechanism, Go's ender list and *terminator insertion unchanged
  everywhere*, whose consequence at the same margin is the continuation; the
  ratified *"broken inside parentheses or not at all"* never had a mechanism.
  So a refusal is the ratified rule and also amends a ratified mechanism
  sentence (critic).
- *The cascade, not measured*: the measured answer is the opposite of a
  cascade. A malformed token at a line's end makes the parser swallow the next
  line, so a second mistake there is never reported (compiler-engineer, critic).
- *Defect 116's search found only panel 007 and panel 180*: `grep -rli "depth
  zero"` over the same places returns 7 files, among them defect 005's record,
  the class's precedent, in no brief (critic; the spec-warden found 005 on its
  own).

**Two facts the coordinator found after the briefs went out**, handed to the
critic and checked by it: `tests/golden/check/depth-zero-continuation.hero`, a
golden the author ratified on 2026-08-04, whose comment describes the
same-margin reading while its body tests only the deeper line; and
`docs/work/milestones/M-thesis-harness.md:141`, which says the form *"is refused
and now enforced"*, false as measured on four counts (critic, § Records owed).

## The proposal

Defect 116 (`docs/work/DEFECTS.md`): `y = a +` over `1` at the statement's own
margin is `check` 0 and prints 6, the lexer planting no terminator after a token
that cannot end a line and the same margin planting no indent; one level deeper
the line is refused as a block. design.md §4.15 (panel 007, ratified
2026-08-03): *at bracket depth zero every line's indentation is structural: a
long expression is broken inside parentheses or not at all*, and Nim's
trailing-operator rule deferred. Defect 118: `fmt` exits 2 on a `match` arm
continued that way. The sitting chooses whether the compiler refuses or the
design admits, where and how, and what the spec says.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve route (a) final**, the lexer refusal that hands the parser the line the author broke; object to (b), (c), (a) as briefed and P; record P2 as the §1.7 alternative; no veto | §1.7, Part 5, §1.1; §4.15; §4.17 | `layout.hero` 117 to 272 code lines, `state.hero` +7, zero outside the lexer; 241 of 242 probes refused, 235 with one diagnostic; lex +1%, check +0.2% | at the close no module under `parse/`, `check/`, `ir/`, `emit/` nor `cursor.hero`, `grammar_expr.hero`, `parse.hero` grows for the repair; the lexer grows at most 180 code lines; none of the ten `.expected` files of the 20 error-token and `use` hits changes | objects to (a) on one shape whose `certain` fix fails to compile (**met**, below); approves (c) on a measured baseline |
| llm-ergonomist | **approve Variant Y** with two amendments; object to X and to Z; no veto | the thesis, locality | | today's compiler refuses 1, 2, 4, 5, 6, 7, 8, 10 and accepts 3 and 9 (**falsified**: it accepts 1, 4, 5 and 10); at the next harness run Y at most halves X's first-try refusals (unscoreable: metric 2 has 0 tasks) | objects to Y unless a condition can be broken inside parentheses (**met**: `if (a > 0 &&` / `b)` compiles, c01); vetoes a variant that compiles a fragment to another meaning |
| spec-warden | **veto route (c)**; object to Y as worded; **approve (a) or (b) with W1b paid by R** | §1.6, §1.2, Principle 0 | W1b +30 real; R -10; W1b+R **+20 real** (9019), +13/+14 vendored | over the 1164 `.hero` files of `0fc98107`, exactly one `check` exit changes under the landed refusal, `comments107/margin.hero` 0 to 1 (pre-scored **held** by the critic on three prototypes) | on (c), a metric-2 run and the author re-ratifying panel 007's condition; on R, a blind reading showing the clause load-bearing |
| historian | **approve refusal**, route (a) preferred on precedent; object to ratifying today's behaviour and to calling (c) Nim's precedent | precedent | | P2: Guido van Rossum's 2007 typo in Heroes exits 1 with a type diagnostic (**held**, `bad_operand` at the `+`); P1: Nim 2.2 admits the same column after `+` and refuses it after `.` (**held**, run by the critic on Nim 2.2.12) | a language reference documenting today's shape with years of use; a silent misreading reported in occam, Koka, CoffeeScript or Scala 3 |

## What the sitting measured

**The class is wider than the defect, and it is one class.** The
compiler-engineer's map: every token kind that cannot end a line, 43 at depth
zero, in every depth-zero context, 87 shapes and 340 programs; **at the same
margin 82 of 83 compile** and print what their joined line prints; one level
deeper 81 of 82 are refused as a block; shallower 70 of 70 are refused. Beyond
the brief's seventeen: `else` over `if b` read as `else if`, `function` over
`main()`, every declaration keyword split from its name, generics broken after
`<` or `,`, `|` joining patterns and releasers, `.dot =>` over its inline body
(which the parser reads on across a terminator, `grammar_expr.hero:1082`), `:`
in a field, `fail` over its record, `P::` over `x`, a trailing comment after the
operator. The spec-warden found three more at exit 0: a break across a blank
line, a pattern broken after `|`, and `for x in` over `xs`. **The critic found
the entries in the other direction**, which no route priced:

- **a postfix after a block** (filed as **defect 119**): `f = if c` /
  `double` / `else` / `triple` / `(5)` prints 10, `[0]` after a block of arrays
  prints 7, a lone `?` prints 11, and `fmt` exits 2 on all three, today and
  under every priced route, because the separator there is a DEDENT and
  defect 005's `ends_the_expression` guards only a binary operator;
- **a leading spaced `-` at depth zero** (filed as **defect 120**): `total =
  base` / `- fee` is refused with `discarded_value`, whose fix, tagged
  `certain`, writes `_ = - fee`, and applied the program prints 100 where 93
  was meant (reproduced by the coordinator, `check --apply` then `run`);
- **the `error` token's glue**: `a = 0X10` over `b = 1 +* 2` reports one of two
  mistakes, and so does `p: ptr = null` over the same line, `reserved_word`
  being the thesis's own showcase for an imported habit.

The critic's question, which the sitting had not asked, is therefore the one the
resolution answers: **what single rule makes every depth-zero line end its
statement unless a production writes a block there, so the next member of the
class is refused by construction rather than found by `fmt` exiting 2**, as
005, 107, 118 and 119 each were.

**The reader.** The llm-ergonomist, reading only the spec, predicted the refusal
on every fragment the compiler accepts: its blind column matches the three
refusal prototypes 10 of 10 and today's compiler 6 of 10 (critic). A careful
reader of today's spec already reads the refusal; what the spec lacks is a
sentence where the reader looks (the ergonomist: X answers *"only through the
word there"*, and its closing clause *"reads like a licence to break
anywhere"*). The ergonomist also found the one program in the experiment that
compiles to a different meaning, and it is not a layout shape: a reader who
takes § 1's *"no parentheses around conditions"* as a ban hoists a long
condition's clauses into names and loses `&&`'s short circuit, so an index
inside the second clause aborts where the original would not.

**The precedents.** Every indentation-sensitive language the historian found
that continues after a trailing operator (occam 2.1, Koka, CoffeeScript,
Scala 3, and Nim's parser, the last run by the critic) admits the same column
and a deeper one; Python refuses both (PEP 3125, 2007, rejected, Guido van
Rossum: *"as no indent is required on the next line it will accidentally
introduce legal interpretations for certain common typos"*). **Today's shape,
the same margin admitted and a deeper line refused, the historian found in no
language.** Nim's written rule (one level deeper) is not its parser's: P1, run.

**The prices**, on the reader's tokeniser, each reproduced by the critic: today
8999 real; Y 9076; Z 9052; W1b 9029; R (dropping *"no parentheses around
conditions"*) 8989; W1b with R **9019, +20**; the compiler-engineer's route (c)
sentence 9067, its clause 9024.

**The cost in time** of route (a), `/usr/bin/time -p`, alternated, the load
between 2.6 and 3.2: `heroes lex` over the 272 `selfhost/` files concatenated
(74,955 lines) median 1.46 against 1.48 s user, `check selfhost/main.hero` 4.22
against 4.23 (compiler-engineer).

## Disagreements, unsmoothed

1. **Route (a) against P2.** The compiler-engineer recommends (a), 162 code
   lines, and records the strongest objection to it himself: P2, a terminator at
   every depth-zero line end plus one parser repair, is 12 lines and by §1.7's
   own test the simplification. What the 150 lines buy, measured on 242 probes:
   235 single-diagnostic refusals against 127, a `certain` fix on 155 probes
   against none, a message that names the rule against *found the end of the
   line*, and a `check --permissive` control arm that can switch the rule off.
2. **Does the spec owe a sentence?** The compiler-engineer: no, § 0 is true as
   it stands. The spec-warden and the ergonomist: yes, since the refusal is
   inferable and not forced (the ratified golden's own comment described the Go
   reading). The critic measured the careful reader as already right, and left
   the less careful one unmeasured.
3. **R reverses CLAUDE.md § 12's default.** § 12 says the compiler has the bug;
   R deletes the spec clause the compiler does not enforce (c01 to c03 compile,
   `fmt` strips the parentheses). The critic asks the synthesis to say so rather
   than inherit it, and the resolution does, below.
4. **Is the new code a thesis rule?** The compiler-engineer measured yes (in
   its fifth copy, `check --permissive` exits 0 and reads the join); the
   spec-warden inferred no, of route (a) as briefed, which the engineer measured
   right about that variant. The critic: with the code on the list, the control
   arm compiles the continuation §4.15 says the language never had, and panel
   180 left its neighbour `line_end_before_continuation` off.
5. **Route (a)'s `certain` fix on Guido's shape.** `y = a +` over `show(n: a)`:
   joined, `bad_operand`, exit 1 (critic, measured). This meets the
   compiler-engineer's own condition for objecting to (a) as he built it.

## The resolution: `provisional, author ratification pending`

**The rule, one sentence, both directions: outside brackets a line ends its
statement.** A depth-zero line whose last token cannot end a statement, and a
depth-zero line whose first token can only continue the line above it, are one
defect, refused by one diagnostic at one site.

1. **The refusal.** `continuation_outside_brackets`, emitted by the lexer at the
   line break, in the shape of route (a) final: the lexer records how a
   depth-zero line ended, and at the next line with words on it decides from
   the margin and that line's first token. It is refused, with the line end
   named as the cause, when either (i) the line above ended with a token that is
   not a line ender (the 52 `false` kinds of `is_line_ender`, `error` excepted,
   item 4), at the same, a deeper or a shallower margin; or (ii) the new line,
   at the margin of the statement above or of a block that has just closed,
   begins with a token that can only go on with the line above: a binary
   operator, a `-` set apart from its operand (panel 180's criterion), or a
   postfix `(`, `[`, `?`, `::`. **A block head is not a continuation**: `else`
   and `=>` before a deeper line open their bodies, and `:` before a deeper line
   keeps the parser's own `trailing_colon` refusal. A line that begins with a
   declaration or statement keyword (`constant`, `function`, `record`,
   `variant`, `test`, `extern`, `use`, `return`, `break`, `continue`, `assert`,
   `for`, `while`, `else` before anything but `if`) ends the line above, and
   the parser says what that line lacks in its own words. So do two lines that
   each bind or mutate (route (a)'s rule 3). **Neither heuristic ever decides
   whether a program compiles**, only which diagnostic the author reads: every
   branch refuses or leaves the parser exactly today's reading.
2. **One diagnostic per break.** The parser is handed the line the author broke,
   joined, so nothing downstream moves (route (a)'s mechanism). Because no suite
   counts the diagnostics a break costs and route (a) as briefed failed the same
   checks with two or more per break, the landing's `check/` golden holds a
   shape per margin and per branch of item 1 with an `.expected` of exactly one
   line each.
3. **The fix.** *Write the statement on one line*: `certain` where the new line
   cannot stand alone as a statement, so the join is the only repair; **`guess`
   where it can** (a call, Guido's shape, where the join may be the dropped
   operand's wrong repair); `guess` after a shallower line, where the author more
   likely closed the block; none when a comment stands between the two lines.
   Wrapping in parentheses is the second repair where there is an expression to
   wrap, offered by the parser as a `guess`. Measured at the landing: `check
   --apply` on every refused probe, then `check` and `run`; a `certain` fix that
   fails to compile or prints other than the joined control is a defect of the
   landing.
4. **An `error` token ends a depth-zero line**, so a malformed token never hides
   the next line's mistake (measured by the compiler-engineer: no `.expected`
   file moves).
5. **Defect 120's fix class.** A `certain` fix is certain only where it repairs
   the defect it names (`.claude/rules/diagnostics-and-goldens.md`): on a line
   that item 1 (ii) refuses, `discarded_value` is not reached, and `_ =` is
   never offered as `certain` on a line that begins with a spaced `-` or a
   postfix after a finished line. The landing audits the other `certain` sites
   of `discarded_value` for the same shape and records what it read.
6. **The thesis list.** `continuation_outside_brackets` joins
   `diag.is_thesis_rule`, on the ruling of 2026-09-27 (*a rule without which the
   program still has a meaning*): without it the program means the join, and the
   control arm reads exactly what today's compiler reads. **This is the
   author's to overrule**: the critic's point stands, that the control arm then
   compiles a continuation the language never had, which is the point of a
   control arm.
7. **The spec: V3 with R4, the most explicit wording priced, not the
   cheapest.** At the end of § 0's opening paragraph: *"Outside brackets a line
   ends its statement: it may not end where the statement cannot, and the next
   line may not go on with it, so a long expression, a condition included,
   breaks inside parentheses."* And § 1's *"No braces, no semicolons, no
   parentheses around conditions."* becomes *"No braces, no semicolons; a
   condition needs no parentheses."* Priced by the coordinator on the reader's
   tokeniser in a copy of `0fc98107`, 2026-09-28 at 03:36 (`measure --refresh`,
   the spec restored to sha256 prefix `1b56c89f1b68570b`): **9061 real, +62**;
   vendored 6716 and 6839, +44 and +45, under `DELTA_GATE`'s 50; 1179 free
   against the ceiling, 1119 after the FFI floor. The same instrument read V1
   (the spec-warden's W1b, rewrapped) 9028, V1 with R 9018, the condition
   clause with R 9024, V3 with R 9050, so the wrapping moves a count by one at
   most. **Why the longer text**: the author's instruction of this night, meant
   as *always choose the most robust and solid route, even at the cost of the
   spec's tokens*. W1b states one direction in the spec's own phrase; V3 states
   both, the line that cannot end and the line that cannot begin, which is the
   class item 1 refuses and the one defects 119 and 120 belong to; *a condition
   included* is the ergonomist's amendment (b), which W1b left to inference; and
   R4 keeps the true half of § 1's clause, the canonical form, where R deleted
   it. **A second blind reading of V3 with R4** by a fresh llm-ergonomist, on
   fifteen fragments including the shapes of 119 and 120, is the instrument
   that decides whether the text is read as meant; its report is
   `docs/panel/181-reports/llm-ergonomist-second-reading.md`, and a fragment it
   cannot read off the text reopens the wording before the landing, not after.
   **§ 12's default reversed, and why**: design.md §4.15's *"No parens around
   conditions ... `if x > 3`, not `if (x > 3)`. Zero information, two tokens
   saved"* describes `fmt`'s canonical output, which `fmt` enforces and R4
   states. Read as a prohibition in the spec, the old clause forbids the only
   way a long condition can be broken once this refusal lands, and it is what
   sent the ergonomist to the hoisting that loses a short circuit. Refusing
   `if (x)` in the compiler instead would make that silent reordering the only
   route, which § Precedence's rank 3 refuses.
8. **design.md §4.15** records the ruling beneath the ratified bullet: the rule
   in both directions, the code, the three block heads, the `error` token, the
   fix classes, and that *terminators inserted unchanged everywhere* still holds
   with the refusal beside it; the deferral of Nim's rule stands as written.
9. **The formatter.** Under item 1 every shape the map and the critic found at
   `fmt` exit 2 stops parsing, and `fmt` refuses it at exit 1. Defects 116, 118,
   119 and 120 close with the landing, each with a `check/` case named after it;
   `print/bare.hero`'s branch for *a line continued at the statement's own
   margin* loses its only input, and whether it can go is the landing's to
   measure.
10. **Module shape.** "The line left open" is a concern with a name and no
    cycle: a lexer module of its own, `layout.hero` staying near its 117 code
    lines.
11. **The gate**, since this widens a refusal
    (`.claude/rules/verification.md`): `check`, `run`, `emission`,
    `determinism`, `corpus`, `annotations`, `fixes`, `surface`, `canonical`,
    `grammar`, `spec`, `layout`, `order`, `records`, the compiler's tests and
    the net's own, and Linux arm64 and the Windows box before the merge.

**Records owed** by the landing, each a dated correction beneath the text and
never a deletion: `tests/golden/check/depth-zero-continuation.hero`'s header
(its *"there is NO continuation at bracket depth zero"* was false at the same
margin, and its annotation and `.expected` move to the new code);
`docs/work/milestones/M-thesis-harness.md:141` (the form compiled; the design.md
pointer was stale; `ends_the_expression` covered one shape; the repair was
defect 005's `/decide` answer 5a, not panel 095); the headers of
`surface-fixtures/comments107/margin.hero` and `brackets180/shape116.hero`, and
`suite_surface.hero`'s row for the first; `layout.hero:12`'s *61 kinds*, a code
comment and not a record, made true.

**What conservative would have been** (CL-040): P2, a terminator planted at every
depth-zero line end and an arm's inline body refused after a line end, 12 code
lines in `layout.hero` and `grammar_expr.hero`; the parser's existing messages
(*expected an expression, found the end of the line*), no fix, no spec sentence,
and a control arm that cannot switch the rule off; defect 119 and 120 repaired
apart. Not taken: it costs 396 diagnostics on the 242 probes against 249, names
no rule, and leaves the class to be found member by member.

**Not taken, and why.** Route (c) (Nim's rule one level deeper): the
spec-warden's veto, Principle 0 unmet and panel 007's condition unmeetable today
(metric 2 has 0 tasks); it also inverts the author's ratified golden and keeps
defect 118 one level down. Route (b) in the parser: covers one margin and takes
`cursor.hero` past §11's ceiling. Same column or deeper (occam, Koka): admits the
same-margin shape this sitting refuses, the one Guido van Rossum named. Admit it
and let `fmt` print it away: contradicts §4.15 as ratified, keeps 118 and the
`xs.len(` / `)` output. Each is the author's to reopen.

**What the conditions compel.** The ergonomist's two amendments are met: Y's
*unless a block opens below it* is not adopted (V3 names no exception, and the
arm's `=>` is the grammar's), and a condition may be broken inside parentheses
(c01 compiles, V3 says *a condition included*, and R4 replaces the clause that
read as forbidding it). The spec-warden's condition on R, a blind reading
showing the clause load-bearing, is answered by keeping its true half. The
compiler-engineer's objection condition, met by Guido's shape, is answered by
item 3's `guess`. The spec-warden's veto is honoured. The historian's objection
to ratifying today's behaviour is the resolution.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | no module under `parse/`, `check/`, `ir/`, `emit/`, nor `cursor.hero`, `grammar_expr.hero`, `parse.hero`, grows in code lines for the repair; the lexer modules grow at most 180 together over `0fc98107`; none of the ten `.expected` files of the 16 error-token and 4 `use` hits changes. The resolution reaches further than the seat's route (items 1 (ii) and 5), so a growth there is scored as the resolution's and said so | the landing |
| spec-warden | over the 1164 `.hero` files of `0fc98107` as they stand there, exactly one `check` exit changes, `surface-fixtures/comments107/margin.hero`, 0 to 1 (baseline 635 zero, 529 one, `price/exits.tsv`, sha256 prefix `edcb78bf10c1fd0d`) | the landing |
| llm-ergonomist | the compiler refuses 1, 2, 4, 5, 6, 7, 8, 10: **falsified**, scored in the sitting (it accepts 1, 4, 5, 10) | scored |
| llm-ergonomist | at the next harness run Y at most halves X's first-try refusals, silent errors 0 | **unscoreable as registered**: metric 2 has 0 tasks (`harness/tasks/README.md`); not renewed (panel 046 R1) |
| historian | P2, Guido's typo in Heroes exits 1: **held**, scored in the sitting | scored |
| historian | P1, Nim 2.2 admits `let y = a +` / `1` at the same column and refuses `let n = xs.` / `len` there: **held**, run by the critic on Nim 2.2.12 | scored |

## Author's verdict

**RATIFIED 2026-09-28**, in one act with every sitting
`docs/work/DECIDE.md` held, panels 175, 176, 177, 179, 180, 181 and 182,
on the author's instruction of that evening, meant as: *ratify every
decision on the list*. **Recorded as a reading of this file**, CLAUDE.md
§ 4's default, which the author asked on 2026-09-21 to be taken for
granted; not `by delegation`. The recommendations the list carried are
taken with it. Each sentence below was verified against the tree at
`a6eab736` before it was written.

**What the yes settles**: route (a) final as landed with defects 116, 118, 119
and 120, and item 1 (ii) as narrowed at 04:03. **The two points the list put to
the author**, both taken as landed: `continuation_outside_brackets` stays on
the thesis list (`selfhost/diag.hero`), so `check --permissive` reads the join;
and § 12's default stays reversed on § 1's clause, which reads *a condition
needs no parentheses*. **And the third point**, added at defect 126's landing:
a line that holds a whole arm does not stand alone under item 3, so the join
after an arm's split `-` stays `certain`, the recommendation.

**What it does not settle**: defect 129, open at the ratification, is item 3's
own criterion failing on a statement broken at more than one line end, and a
certain join that writes an operator against a literal it takes under no type.
It is repaired under item 3 as ratified, in lane 129, and recorded in its own
entry.

**What this section said while the sitting was open**, kept because a
record is not rewritten:

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 181`. Work
proceeds on the provisional resolution: the landing lane repairs defects 116,
118, 119 and 120 on it, with the spec and design.md amendments in their own
commit citing this file.*

## The second blind reading, 2026-09-28 at 03:45

A fresh llm-ergonomist read the candidate spec with V3 and R4 in place and
nothing else (`docs/panel/181-reports/llm-ergonomist-second-reading.md`, brief
`docs/panel/181-briefs/llm-ergonomist-second-reading.md`). **It approves the
wording unchanged.** It predicts the landed compiler accepts fragments 3, 9 and
12 and refuses the other twelve, the shapes of defects 119 (fragment 10) and
120 (fragment 7) among them, with zero silent divergences, and that its two
written programs, a parenthesised four-call sum and a parenthesised
three-clause condition, compile at the first try. It considered the shorter
*"Outside brackets no line goes on below"* and declined it: the answers do not
change, and the sentence's second half, *the next line may not go on with it*,
is what catches the leading-operator habits of fragments 6 and 7. Without the
sentence its own first draft broke a sum outside brackets and hoisted a long
condition's clauses into names, the reordering that loses `&&`'s short
circuit. One hesitation it names and does not blame on the sentence: fragment
9 (`ys = [1, 2,` / `3]`), where § 0's line ending in `,` and § 10's *"separates
elements by newline across lines and by comma on one"* pull apart, 75 per cent
on accepted. **The wording stands as item 7 gives it**, and the landing lane
was told so at 03:46.

| seat | prediction | checkable at |
|---|---|---|
| llm-ergonomist, second reading | the landed compiler answers the fifteen fragments as the seat did, 15 of 15, or 14 of 15 with fragment 9 the one; no refused fragment accepted with another meaning; both written programs compile | the landing |

## Item 1 (ii) narrowed, 2026-09-28 at 04:03

**The list in item 1 (ii) was wrong about `(` and `[`**, measured by the
landing lane's agent and reproduced by the coordinator: both begin a Primary
(spec § 7), so a line that begins with one after a finished line is a
statement of its own, and a legal one where it means something. On the trunk's
compiler, `y = a` / `(a + b).print()` compiles and prints 7 then 5, and `y = 1`
/ `[1, 2].len().print()` prints 2 then 1; the tree holds one such line,
`tests/golden/surface-fixtures/comments101/parenplace.hero:10`, `(x  # after
the place` / `) @ a`, which the list as written would have refused, moving its
exit and falsifying the spec-warden's prediction. The lane's census of the 1164
files of `0fc98107` found no depth-zero line after a finished line or a closed
block that begins with a binary operator, a `-`, `?`, `::` or `[`, and that one
beginning with `(`.

**The resolution as it lands** (the coordinator's approval of the lane's
reading, 04:00): the lexer refuses, and hands on the join for, a depth-zero line
that begins with a binary operator, a `-` set apart from its operand, `?` or
`::`, at the statement's margin and after a closed block; a line that begins
with `(`, `[` or `.` begins a new statement, and **the parser's postfix loop
stops at every suffix after a block that has just closed** (panel 035's own
rule, *a suffix never reaches across a block that just closed*, made true of
every suffix and not only `.`), so defect 119's `(5)` and `[0]` become lines of
their own, refused as unused values, and a line such as `(5).print()` after a
block runs as the second statement it reads as; `discarded_value`'s `_ =` is a
`guess` on a line that begins with `-`, `.`, `(` or `[`, panel 180's
`begins_a_value` set. Nothing here changes a verdict: the second blind reading
refused fragments 10 and 11 as unused values, which is this reading. The
wording V3 already says it: *the next line may not go on with it*, and a line
that begins a Primary does not.
