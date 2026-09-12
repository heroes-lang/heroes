# Panel 133 — the half that can be derived, and the half that cannot

Convened 2026-09-12. Trigger: `spec/**`, design.md Part 7 item 16, and the
architecture of the verification net. Full lane, five seats.
Status: **ratified as adopted, 2026-09-12** (§ Author's verdict).

## The proposal, verbatim

> A new file `spec/grammar.md` stating every syntactic form of Heroes once, in
> Wirth's EBNF (the Oberon report's notation). It is explicitly NOT the prompt:
> `spec/heroes-spec.md` stays the whole language as a model receives it. The
> grammar file spends no token of §1.6's budget and takes a ceiling and a pin of
> its own, following `spec/reserved-words.md`'s precedent exactly. It is kept
> honest by four new checks in a new `tests/harness/suite_grammar.hero`:
> terminals against `selfhost/keywords.hero` and `selfhost/token.hero`,
> precedence against `binary_op`'s power table in `selfhost/grammar_expr.hero`,
> production names against the parser's entry functions in both directions, and
> every fenced example compiled. This reopens the EBNF appendix refused by
> author decision 2026-08-12 (design.md Part 7 item 16), on the ground that the
> refusal's two arguments have different fates: "spec tokens" dissolves with a
> separate out-of-budget file, "a reader would then have to reconcile with the
> prose" does not and must be closed by instruments.

The author chose all four stated purposes on 2026-09-12: closing the spec's
silences, a reference for humans and tools, a Part 11 prompt experiment, and
understanding the parser. A draft was written from the parser the same day and
measured before the briefs went out.

## The sitting's premise was false, and the seat that reads only the spec found it

The proposal's headline purpose was closing the silence design.md Part 7 item 16
names: *"construction stays unspecified, so a program written from the spec alone
cannot build a variant."*

**That stopped being true on 2026-09-11.** `spec/heroes-spec.md` § 9 reads:

> Record construction is a call with field names, always mandatory:
> `Point(x: 3, y: 4)`, and a variant's case `.num(v: 7)` or `.plus`.

`git log -S` on that sentence returns `d245239a`, 2026-09-11, *"The
specification takes the shape of a Report"* — panel 126's re-shaping, one day
before this sitting. The llm-ergonomist, given only the spec, wrote
`return .circle(r: 3)` in Phase A with high confidence and cited that clause,
before it was allowed to open the grammar.

So design.md Part 7 item 16 carries a falsified cost sentence, and item 16's
deferral of symmetric variant syntax to v2 now rests on one argument fewer than
it was written with. That is a defect this sitting found and it lands whatever
the author decides here (§ Defects found beside the sitting).

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| spec-warden | **veto** | §1.6 first sentence | spec unchanged; the draft is **+2903** vendored, not 0 | at the second close after landing, the draft reads ≥3053 cl100k and ≥8 normative sentences absent from the spec (today 5) | a fifth check, `grammar/prose`: every sentence outside a production, a fence and § Notation appears verbatim in the spec, or does not exist |
| compiler-engineer | object | §1.7, Part 7 item 16 | 0 compiler lines · ~645 harness lines · **0.06 s** measured | `suite_grammar.hero` ships over 550 lines with ≥70 exception rows; under 40 rows means the reverse direction was dropped | the exception table gets a length assert and a written reason per row, on `suite_layout.hero`'s ratchet model, or the reverse direction goes |
| llm-ergonomist | object on the draft, approve the experiment | the objective | 0 of 3 programs changed | spec+grammar beats spec alone by at most +1 task in 10, and by 0 on these three; silent-divergence delta exactly 0 | four defects in the productions fixed, the keyword count listed, `ExternFunction` loses `[ Generics ]` |
| ffi-pragmatist | object | §4.19, §1.11 | 0 bytes across the boundary; 4 bindings built and run at exit 0 | `grammar/productions` passes green over `[ Generics ]` forever, because `parse/tails.hero` reads generics before the extern branch | the newline rule moves into § Notation; FFI examples are **built**, not checked |
| historian | approve with conditions (advisory) | precedent | — | across the next three milestones changing a surface form, at least one check goes red and the **grammar** is what is found wrong; three greens mean it is decoration | the grammar is generated or checked **from the prose**, never hand-copied beside it |

## The convergence, and it is the sitting's finding

Five seats, five different inputs, and every one of them falsified **the same
half of the document**: the hand-written productions.

- The **ergonomist**, which never saw the compiler, found four productions that
  are wrong: `Statement` cannot derive the document's own `return match s`
  (an `if` or `match` ends with a DEDENT, not the NEWLINE the production
  requires); `PatternLiteral = Unary` contradicts its own prose two lines below;
  the arm body admits the declaration the prose forbids; `ExternFunction`
  invents generics. It also found that `Shape.circle(r: 3)` is grammatical
  through the UFCS suffix, so the grammar cannot refuse the most plausible wrong
  construction a model brings from Rust or Swift.
- The **ffi-pragmatist**, which compiled real bindings, found the productions
  **narrower than the language in one place and wider in another**: a multi-line
  `extern` parameter list is accepted by the compiler (verified here, exit 0) and
  underivable from `ExternParams`, while `[ Generics ]` is derivable and dead,
  because `selfhost/check/ffi.hero` refuses `.generic` at every crossing.
- The **compiler-engineer** costed the check meant to guard them: 74 productions
  against 98 parser functions, 37 productions unmatched and 63 functions unnamed
  after a generous matcher, so roughly **82 hand-written rows to judge 74
  productions**. And `suite_layout.hero`'s `DECIDED` does not license that: a
  `DECIDED` row carries a number, ratchets, and has its length asserted, where a
  name-only row is a mute button.
- The **spec-warden** found that 1322 of the draft's 2903 tokens are prose, and
  that **five language rules live in it and nowhere in the spec** — left
  associativity, the trailing comma, `T??`, absolute paths in a group head, and
  `use` path adjacency. Verified here: `grep -ci` on the spec returns 0 for
  `associat`, `trailing comma`, `adjacent` and `T??`.
- The **historian** found that the arrangement has been tried and is dead
  everywhere: Rust removed `src/doc/grammar.md` (PR #64896, merged 2019-10-01)
  and archived `wg-grammar` on 2024-04-08; **Zig's `zig-spec`, which is this
  proposal feature for feature** — a hand-written recursive-descent parser, a
  separate non-normative grammar, and a mechanical check tying them — was
  archived 2025-11-23, three years after its checker correctly reported six real
  divergences that nobody repaired.

**Each of the four checks the proposal offered would have been green over at
least one of those defects.** The productions are the half nothing can judge,
and the draft said so about itself in its own header.

## What the historian found that cuts the other way

Two findings weaken the 2026-08-12 refusal rather than the proposal.

**Wirth shipped the grammar twice in every report Heroes claims descent from.**
The Oberon report is 17 pages with the grammar inline beside each construct
*and* collected again in `Appendix: The Syntax of Oberon`; the Pascal Revised
Report does the same and the second copy is in a different notation, railroad
diagrams. So *"the prose plus a compiler diagnostic beats a second
description"* is a departure from Wirth, not an application of him, and should
be recorded as chosen.

**Panel 024 has been reading Aycock et al. backwards.** The paper (arXiv
2409.19151, ICLR 2025) does say *"almost all improvements stem from the book's
parallel examples rather than its grammatical explanations"* — of **translation**.
On grammaticality judgment and glossing, the tasks nearest to writing a
language, its best arm was a **compact structured description plus parallel
examples**, beating examples alone by up to 3% and reaching 46.1% morpheme
accuracy, 25 points over the no-examples arm. Its own conclusion: *"LLMs can
exploit grammar for relevant linguistic tasks — if provided in a useful form —
but not for translation."* An EBNF plus compiled examples is structurally that
best arm, not the arm the paper condemns.

## The disagreement the sitting does not resolve

The historian's failure mode is *"the checks will work, report truthfully, and
be ignored, because the grammar is in a file no milestone is obliged to open."*
That is exactly what happened to Zig.

**Heroes has the thing Zig lacked**, and the synthesis records it rather than
smoothing it: `check_parser.sh` was a script nobody had to run, where a Heroes
suite is inside a net that gates every sub-step and every push
(`.claude/rules/verification.md`). A red `grammar/*` check stops work. That is a
material difference and it is the strongest argument for the conservative
resolution below.

Against it: a red check whose repair is *"edit the prose in the other file"*
gets repaired by editing the prose in the other file, and the historian's point
is about where the cheapest repair lands, not about whether the check fires.
The sitting cannot settle which force wins without running it, and says so.

## The resolution adopted, provisionally

**The document is split by derivability, and the half that cannot be derived
does not ship by hand.**

**R1 — `heroes grammar`, a subcommand, prints the derivable half.** The keyword
table, the token kinds with their spellings, and the nine precedence levels with
both the operator's spelling **and** its `ast.BinaryOp` name, all read from the
compiler's own tables at run time. This is output, so there is nothing to
diverge, no pin, no exception table, and no §1.6 question — panel 035 D's rule
that compiler output can never be in a prompt is what makes it free. One command
(CLAUDE.md §10), never a second binary and never a script.

**R1 is strictly stronger than the checks it replaces.** `grammar/powers` as
proposed would have passed panel 067's defect: the draft names `%` and level 9
and never names `.rem`, so `.percent => .add` is invisible to it (verified:
`BinaryOp`, `.rem`, `.add`, `.mul` all occur zero times in the draft). Printing
the AST operator name closes the class the joined table exists to prevent.

**R2 — the productions are authored inline in `spec/heroes-spec.md`**, in
Wirth's notation, beside the prose of the construct each governs, and
`spec/grammar.md` becomes the **collected view, generated** by the same
subcommand and never hand-edited. This is Wirth's own arrangement, it is what
the Rust Reference does today with `mdbook-spec`, and it is the only arrangement
in the historian's whole survey that is still alive. It satisfies the warden's
veto condition by construction, because a generated file asserts nothing the
spec does not, and it satisfies the historian's condition, because the
productions then live in the file every milestone already opens.

**R3 — R2 does not fit today, and that is a measurement rather than a
judgment.** The draft's productions alone measure **1253** vendored, and
`heroes measure` states the vendored maximum is *"a lower bound, not the
reader's tokeniser"*. The spec's real headroom is **368** (5716 real + 60 FFI
floor against 6144). So the productions do not fit under any ratio, and R2
requires either a production set trimmed to fit or a raise of §1.6's ceiling.
**A ceiling is the author's, and it is queued as `panel 133`.**

**R4 — until R3 is answered, nothing hand-written ships.** R1 lands alone. This
is the operative instruction of the sitting: the sitting found five independent
falsifications of hand-writing productions in a separate file and zero seats
defending it, so the provisional default is not to do it.

## What the conservative resolution would have been, so the author can choose it

Ship the hand-written `spec/grammar.md` as proposed, with **five** checks rather
than four and all eleven seat conditions met: the warden's `grammar/prose` and
its pin pair, the engineer's length-asserted exception table with a reason per
row, the ffi-pragmatist's newline rule in § Notation and `build` rather than
`check` for FFI examples, and the ergonomist's four production repairs plus the
keyword list.

It is cheaper: no spec token, no ceiling decision, no subcommand. It is what
Zig built, and Zig's checker worked, reported truthfully for three years, and
was archived with its divergences unrepaired. The sitting takes robust over
conservative per CLAUDE.md §4 and records this so the author can take it back.

## What a veto would compel

The warden's veto stands against the proposal **as briefed** and is withdrawn by
R2, which removes the hand-written prose entirely rather than checking it. If
the author takes the conservative resolution instead, the veto is withdrawn only
by `grammar/prose` being built and the five orphan rules being moved into the
spec under §1.6's payment rule or deleted.

No seat vetoed R1.

## Predictions to score

| # | seat | prediction | instrument | checkable at |
|---|---|---|---|---|
| 1 | warden | the hand-written file reads ≥3053 cl100k and carries ≥8 sentences absent from the spec | `heroes measure`, grep of the spec | second close after landing, **only if the conservative resolution is taken** |
| 2 | engineer | `suite_grammar.hero` over 550 lines with ≥70 exception rows | `wc -l`, grep of the table constants | the milestone that lands it |
| 3 | ergonomist | spec+grammar beats spec alone by at most +1 task in 10; silent-divergence delta exactly 0 | a 10-task suite of the A1/A2/A3 shape | the Part 11 experiment |
| 4 | ergonomist | ≥1 in 10 FFI-and-pattern attempts emits a form the productions admit and the checker refuses | the same suite | the same |
| 5 | ffi | `grammar/productions` passes green over `[ Generics ]` forever | write the suite, observe green, then `heroes check` a generic extern → exit 1 | the milestone that lands it |
| 6 | ffi | `examples/ledger/db/sqlite.hero` is derivable line for line from these productions | `bind4.hero`, already built at exit 0 | now |
| 7 | historian | across the next three surface-changing milestones, ≥1 check goes red and the grammar is what is wrong | `docs/records/log/` entries | third such milestone |

## Defects found beside the sitting

Three, none of which depends on how the author rules.

1. **`heroes measure` issues a false ruling on every file with no registered
   ceiling.** Verified here: `./heroes measure design.md` exits **1** with
   *"BREACH: 64490 over the 6144-token ceiling (design.md §1.6)"*, while
   design.md §1.6 line 385 reads *"The spec has a budget; the compiler does
   not"*. The same path printed *"Headroom: 3241"* for the draft. Root cause in
   `selfhost/cli/measure.hero`: the default path judges the **vendored** maximum
   against `CEILING`, which has been a **real** claude-opus-5 number since
   2026-09-09, so the ceiling and the scale are both wrong. The repair sentence
   already exists in the same file, in `run_refresh`: *"no ceiling judges it and
   no check keeps it honest (panel 123 R5)"*. A file with no registered ceiling
   should print its counts, say that, and exit 0.
2. **design.md Part 7 item 16's declared cost is falsified** since 2026-09-11
   (§ The sitting's premise was false). Corrected underneath with its date, never
   deleted.
3. **Panel 024's reading of Aycock et al. is backwards** (§ What the historian
   found that cuts the other way). Corrected underneath with its date.

## What the seats could not source or could not run

- The draft's **real** token count does not exist and cannot today:
  `measure --refresh` refuses every file but the spec, exit 2. Every figure for
  the draft in this file is vendored and is a lower bound.
- The historian marked unverified: the Oberon report's word count, the 812-line
  figure for Rust's deleted grammar, whether Ada's Annex P is *mechanically*
  generated, whether the Rust Reference's grammar is normative or informative,
  R7RS §7.3's opening text, and any tree-sitter divergence with a measured cost.
  The Swift `SummaryOfTheGrammar.md` maintenance comment is verified secondhand
  only, from a named forum participant, 2024-08-21.
- The engineer's 645 harness lines are an estimate from the neighbouring suites.
  Its 0.06 s is measured with `/usr/bin/time -p`.

## Process note

The llm-ergonomist reported that two files under `.claude/rules/` were pushed
into its context by the harness without its asking, one of them carrying a prior
ruling on this exact proposal. It excluded both and used no sentence from
either. Its isolation was broken by the tooling rather than by the seat, and it
offered to be re-run in a clean process. Recorded here because a seat whose
whole value is its isolation is worth nothing if the breach goes unwritten.

## Author's verdict

**Ratified as adopted, 2026-09-12, and the queued question answered the same
day.** The author raised §1.6's ceiling from 6144 to **8192** so the productions
could go inside the document, which is R2 taken and R3 answered in the direction
the sitting could not take for itself. The conservative resolution recorded above
was not taken.

A standing instruction came with it, and it now sits in `CLAUDE.md § Precedence`
as the third consequence: **take the most robust and production-ready
resolution, never the easiest, the compromise, or the cheapest in tokens.** § 4
had said it of a panel's resolution since 2026-09-07; it binds every choice now.

**What the ratification cost, measured rather than predicted.** The productions
took the spec from 5716 to **7531** real tokens, digest `0b96e29b3ca14666`, so
661 are free and 601 net of the FFI floor. The real delta is **1.60** times the
vendored one where the whole document sits at 1.26: a grammar is dense
punctuation and the reader's tokeniser splits it finer than either vendored
table. R3's own arithmetic, which reasoned from the vendored lower bound, would
have been 384 tokens low.

**Two of the seats' conditions were resolved against them, and both are named
rather than quietly dropped.** The ffi-pragmatist asked that `[ Generics ]` be
kept in `ExternFunction` with a sentence explaining that it is dead; it was
dropped instead, because a production nothing can use is not part of the
language and documenting one invites a model to write it. The engineer's
condition on the production check's exception table was resolved by not building
that check at all: `suite_grammar.hero` judges the terminals and the powers,
which are data, and nothing judges a production against the parser.

## Predictions scored at M-stated-grammar's close, 2026-09-12

**2, the engineer's: FALSIFIED, and its own escape clause is what came true.**
`wc -l tests/harness/suite_grammar.hero` reads **491** against the predicted
"over 550", and the exception rows number **0** against "at least 70". The
prediction carried its own reading of a low number — *"if it ships with under 40
rows, it shipped because the reverse direction was dropped, not because the
parser matched the grammar"* — and that is exactly what happened: the
production-versus-parser check was not built at all. The seat was wrong about
the number and right about what a wrong number would mean.

**6, the ffi-pragmatist's: CONFIRMED, by enumeration and not by derivation.**
Every declaration shape in `examples/ledger/db/sqlite.hero` (354 lines) appears
in the productions: `constant` and `function` members, four `@` out-parameters,
and `owned` on a result. Its multi-line signatures are derivable because the
notation paragraph states once that a NEWLINE inside brackets never ends a
statement — the repair this seat's own finding bought. **Stated rather than
overclaimed**: nothing parses the EBNF, so this was scored by enumerating the
file's forms against the productions by hand, not by deriving it.

**1, the warden's: VOID.** It was conditional on the conservative resolution
being taken, and the author took the adopted one.

**5, the ffi-pragmatist's: VOID, and the void is the seat's win.** It predicted
that `grammar/productions` would pass green over `[ Generics ]` in
`ExternFunction` forever. That production was dropped on the seat's own evidence,
so the prediction has no subject. A prediction whose subject is removed BECAUSE
of it is not a miss.

**3, 4 and 7 remain open.** The two ergonomist predictions need the Part 11
experiment, whose item now lives in `docs/work/milestones/M-thesis-harness.md`;
the historian's needs three more surface-changing milestones.
