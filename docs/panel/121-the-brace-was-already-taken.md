# Panel 121 — the brace was already taken

Sitting of 2026-09-08, M-interpolation-verdict. **Full panel, five seats**, and
the lane was never in question: the form has surface, spec tokens and at least
one diagnostic class.

**This sitting did not rule on whether.** The author decided on 2026-09-08 that
string interpolation ENTERS, over the coordinator's own reading of the numbers,
and raised the spec ceiling to fund it: *"da programmatore non vorrei rifiutare
questa cosa, al costo di alzare il limite a 5000 token"*, then, after the
costed counter-proposal, *"confermo 4224"*. What the seats were asked is the
SHAPE. Two of them vetoed anyway, on grounds the ballot had not contemplated,
and the resolution below is not the proposal that went in.

**The inputs, both measured before the sitting rather than during it**:
`docs/measurements/021-what-would-stand-inside-a-hole.md` (the corpus) and
`docs/measurements/022-the-narrow-rule-costs-more-than-the-wide-one.md` (the
prices).

## Proposal (as put to the judges)

Into `spec/heroes-spec.md` § Strings, arrays, maps, immediately before the line
beginning "`+` on `str` copies both sides":

```
A string literal may hold a **hole**, `{n}`, which writes that value as
text exactly as `to_str` does: `"line {n}: {word}"`. Any expression may
stand in a hole, and `{{` and `}}` write one brace.
```

Six open questions went with it: what may stand in a hole (the coordinator
recommending **any expression** on coverage and on cost); the spelling and its
escape; the compiler cost of the wide rule; what `print` becomes; whether `???`
is legal in a hole; and the diagnostic class a malformed hole needs.

## The one answer nobody put in the ballot, and five seats reached it separately

**A brace may not be active inside an ordinary string literal.** The proposal
made it active in every literal, and that is the half that did not survive.

- The **compiler-engineer** measured **211** brace-bearing literals in **39** of
  `selfhost/`'s 190 modules, nearly all of them diagnostic text naming a map
  type (`{str: i64}`, `{f64: T}`). Each becomes a parse error, so **the compiler
  stops compiling itself**: a Principle 0 regression, not a price. Its veto
  lifts the moment the spelling is gated.
- The **historian** could source **no brace-delimited-hole language that makes a
  bare `{` active in an ordinary, unprefixed literal** — Python, C#, .NET, Rust,
  Nim, Hare, Luau, D and WG21's live paper all gate it behind a prefix or a new
  delimiter, and the unprefixed languages gate on a sigil rarer in text than a
  brace. Luau's RFC says why in one line: *"Because we care about backward
  compatibility, we need some new syntax in order to not change the meaning of
  existing strings."* The seat named its own condition for withdrawing the point
  — a measured count of brace-bearing literals in `selfhost/`, which it could
  not run — and the count came in at 211, which is not small.
- The **spec-warden** counted **186** on non-comment lines in 39 files, **59** of
  them a lone `"{"`, and found the hazard nobody had: **the old seed's lexer
  reads `{{` as two literal braces**, so doubling the braces in `selfhost/`
  before the compiler understands doubling silently corrupts every diagnostic
  that goes through the seed. A two-stage bootstrap, designed before the first
  line is written.
- The **llm-ergonomist**, reading only the spec, found the lone brace to be
  *"silence, and the worst one"*, and the only place in the proposal where a
  wrong guess yields a silently different program.
- The **ffi-pragmatist** did not reach the question and is not counted here.

## Two vetoes, and what each one rests on

**The engineer vetoed the spelling, not the feature** (design.md §1.7, Part 5;
CLAUDE.md §2). Its route analysis killed two of the three architectures the
brief offered, and both by measurement:

- **The lexer re-entering the parser is dead, and not on cost.**
  `selfhost/lexer.hero:66-72` offsets tokens and diagnostics only into
  compilation coordinates, so an expression built during lexing keeps a
  file-local span and **every hole in every module after the first gets a wrong
  caret, silently**. `selfhost/lexer.hero:12-14` states the Cyclone invariant it
  breaks.
- **The parser desugaring into a `+` chain is self-defeating, and CLAUDE.md §9
  is what kills it.** `selfhost/print/fmt.hero:1266` prints a literal by its
  source span but a `.binary` node structurally, so `heroes fmt` would rewrite
  `"a{x}b"` back into the `+` chain and `tests/harness/suite_canonical.hero`
  would then forbid the form in every file this project calls source: **the
  feature could never appear in its own tree.** The repair is one node carrying
  the whole literal's span, desugared in lowering, where §1.7 puts sugar anyway
  — and then **the formatter costs zero lines**.

**The warden vetoed on Principle 0's burden of proof**, not on the budget, and
was explicit that the budget veto is not engaged. Its argument is the sharpest
thing said against the form all day: *"1461 concatenations are 1461 sites of
code that COMPILES — a frequency count of correct use is evidence against a
rewrite-rate problem, not for one."* The warrant available is a measured
design.md Part 11 effect, and no reader has been put in front of either form.

**The warden's own steelman turned out to be worse than it thought, and the
coordinator ran it.** `"count " + n` is `error[mixed_arithmetic]` today. The
`fix (guess)` offers `to_f64(x)` and `to_i64(x)`; **following it reproduces the
identical diagnostic** with `str` and `f64`, and the repair that works,
`n.to_str()`, is never named. Filed as defect **023**. So the mistake
interpolation prevents is one the compiler currently mis-advises in a loop,
which is evidence for the clause under §1.2 that nobody had.

**The warden's veto condition cannot be run today, and that is a finding about
the condition.** It asks for a count of how often `error[mixed_arithmetic]`
fires with a `str` operand over real transcripts. No transcript corpus exists:
that count is metric 2, which has never run, and
`docs/measurements/007-two-predictions-collected.md` records four predictions
that named it as **lapsed**. Panel 046 R1 forbids naming an instrument that does
not exist as *payment*; a veto condition is not payment, but the asymmetry is
recorded rather than used.

## What the wide rule survived, and what it cost

The ergonomist vetoed **any expression** on locality: under it a hole could hold
a `?`, which is a return, and `if` and `match`, which are expressions with
significant indentation, so *"the line no longer determines the enclosing
function's shape"*.

**Half of that premise is false against the compiler, and the spec is why the
seat believed it.** Run 2026-09-08: a raw newline in a string literal gives
`error[unterminated_string]: this string never closes — strings are
single-line, `"` to `"``. The spec never says so. It says *"strings any but a
raw carriage return"* (`spec:18-19`), and three lines below a sentence about
strings it says *"Multi-line literals separate elements by newline"*, which is
about container literals. **The compiler is precise and the specification is
mute, which is the third instance of that shape this milestone has met** — the
other two are the printed float and the undefined `abort`, both already bought.
This one produced a veto.

**The surviving half is the quote, and the historian brought the answer.** WG21
P3412R3, string interpolation for C++: *"it turns out that it is not very hard
to implement a partial parser inside the lexer just to determine where an
expression ends"*, with the rule — *"To detect the end of the expression-field
just scan for the first `}` or `:` pp-token, skipping over nested parenthesis,
curly brace and square bracket pairs as well as nested string and character
literals."* Heroes has no ternary (`spec:174`) and no `:` sub-syntax in a hole,
so it pays less than C++ does.

**And the quote is not hypothetical.** The ffi seat hand-read every `print(`
line in the 20 files holding an `extern` and found **5 sites in 3 files** that
need a `"` inside a hole — `examples/sqlite/main.hero:87,88`,
`examples/curl/main.hero:49,67`, `examples/sdl/main.hero:56` — and those three
files are rungs **3, 4 and 5** of §4.19's own FFI acceptance ladder. Its
corpus-wide instrument flags 239 of 1319 sites, 18.1%, and the seat labels that
an upper bound with known false positives rather than a measurement, which is
how it should be read.

**Precedent says narrowing does not close the question, it queues it.** Rust is
the only language that narrowed on purpose: RFC 2795 restricts capture to
identifiers and states the reason, *"users will inevitably ask 'why is my
particular expression not accepted?'. This could lead to feature creep"*.
Shipped 1.58.0, 13 January 2022. RFC PR 3626, which asks for **one dot**
(`{self.x}`), opened 6 May 2024 and is still open with `disposition-merge` as of
7 June 2026. Python went the other way: PEP 498's restrictions were lexical
only, and PEP 701 lifted them in 3.12 because they *"serve no purpose from a
language user perspective"*.

## The trade, verified at the source

design.md:1422 records that `print`'s variadic form *"was bought by trading away
string interpolation"*, with Pascal's `WriteLn` as the fifty-year precedent. The
historian checked it against ISO/IEC 7185:1990's collected syntax and every part
holds: `writeln-parameter-list` is its own production inside
`procedure-statement`, so `writeln` is **variadic in the grammar and not by any
general mechanism**, and `formal-parameter-list` has no variadic form, so no
user-declared procedure can have that shape. A precise match for a language
whose spec says *"no user variadics"* (`spec:112`).

**With a coda the record did not have: Wirth abandoned it.** The Oberon report's
`FormalParameters` has no variadic form and the grammar has no `write`
production; its examples pass the field width as an ordinary second argument.
Pascal had the compiler-known variadic print, Oberon dropped it, and neither
ever had interpolation.

**And the trade cannot be bought back from its own proceeds.** `print` narrowed
to one value returns **-3** tokens, and the warden priced collecting it at
**245 `print(` sites in `selfhost/`** plus their goldens. Both seats that looked
at it called the direction legitimate and the price absurd. `print` is unchanged
by this resolution.

## What the C boundary said, and the defect it found

The ffi seat did not veto: **the ABI does not move.** Every renderer a hole can
reach is already declared at 21, and `hero_str_concat` with them; a 7-hole
literal needs 5 renderers and 13 concats and no undeclared symbol. Ownership
holds too, and by construction rather than by luck: a hole's temporary lives to
function exit, strictly longer than `spec:253`'s *"for that call"*, so `.cstr()`
on a hole is sound. 200 SQLite inserts building their SQL from hole-shaped
concatenations ran ASan and UBSan clean. Non-renderable types are already
refused — `ptr`, `cstr`, a group's `record` and a `str?` all give
`error[bad_operand]` — so a hole introduces no second rendering rule and no new
promise about a C value.

**It also found that `hero_str_join` has been at ABI 21 all along**, described in
`runtime/heroes_runtime.h` as *"one allocation, design.md:1318's answer to
O(n²) concatenation"*, and that the `+`-chain lowering ignores it. An entry
point this project has already paid for.

**And it found a memory-corruption class that predates the proposal.**
Confirmed by the coordinator, 2026-09-08, at 16 lines:

```
function from_literal() -> cstr
    return "static-and-immortal".cstr()      # sound: static storage, immortal

function from_built(n: i64) -> cstr
    return ("heap-and-doomed-" + n.to_str()).cstr()   # dangling
```

Builds at **exit 0 with zero diagnostics**. `strlen` of the first is 19, correct;
of the second **0**, three runs of three, where 17 is the answer. Under
`--sanitize`: `AddressSanitizer: heap-use-after-free`, READ of size 18, freed by
`hero_release_block` at `alloc.c:151`. Filed as defect **022**. Interpolation
does not cause it and makes the sound and the dangling spelling **one brace
apart**, which is the seat's whole point.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto** on the spelling, not the feature | §1.7, Part 5; CLAUDE.md §2 | ~270-300 code lines; the AST case is 59 exhaustive sites in 18 modules for a net **+3**; backend **zero**; six ceilings breached, `grammar_expr.hero` at **1002 of 1002** with no legal home | at M-interpolation close the `DECIDED` list holds a number strictly greater than 1002 for `selfhost/grammar_expr.hero` | a prefix or a sigil, and zero edits to existing literals; **or** a measured migration of the 211; **or** `grammar_expr.hero` split first |
| llm-ergonomist | **veto** on *any expression* as written | locality | prefers a name and its `.field` steps; found 10 silences, 2 of them able to change a program's meaning quietly | ≥6/10 fresh models emit undoubled lone braces and 0/10 remark the paragraph does not rule on one | a demonstration that no admissible hole content can contain `"`, `(`, `[` or `?`, and that a literal cannot span lines |
| spec-warden | **veto** on Principle 0; object on the raise arithmetic; **budget veto not engaged** | §1.6, §1.2, §1.0 | the ballot is the dearest of four wordings for one rule; noun dropped saves 8; removal branch unavailable at -3 against 245 sites | at close, `heroes measure` reads ≤ 4047 and no follow-up sentence is needed | count how often `error[mixed_arithmetic]` fires with a `str` operand |
| ffi-pragmatist | **object**, no veto | §1.11, §4.19, `spec:253` | ABI **unmoved**; a `str` hole under a type-blind desugar costs one extra runtime call and one owner slot, 16 own tokens to 20 | `--sanitize` reports heap-use-after-free on the 16-line case on every seed up to this milestone's close unless the escaping `cstr` is refused | the desugar specified to run after the checker; the escaping `cstr` filed or refused; the quote rule stated and the 5 FFI sites compiled |
| historian | **approve** the hole rule; object to the unprefixed brace | precedent | 25 sourced precedents; 5 claims it could not source, named | `"{{{n}}}"` and a `}` inside a nested literal are the two inputs that break a naive scanner | a brace-hole language where a bare `{` is active in an ordinary literal, shipped for years |

## Disagreements, unsmoothed

**The ergonomist and the ffi seat want opposite things, and both are right about
their own corpus.** The narrow rule the ergonomist asks for cannot write 5 of
the sites on §4.19's FFI ladder, because they need a quote inside a hole. The
wide rule the ffi ladder needs is the one the ergonomist vetoes. Nothing
reconciles them at the level of the rule; what reconciles them is the scanner,
which is why P3412R3's recipe is adopted as normative text rather than left to
the implementation.

**The engineer prefers `\(n)` and the resolution does not take it.** Its case is
real and measured: `\(` is `error[unknown_escape]` today with **0** occurrences,
so it costs zero literals, it lands in `selfhost/escape.hero` which has 188 free
lines, and the coordinator's own measurement puts it **24 tokens cheaper** than
the prefix (+94 against +118, both deciding every open case). It is not taken
because `docs/panel/008-escape-sequences.md` R3 rules that a new escape
**reconvenes that panel**, and because this item's only available warrant is the
thesis, where `{}` inside a prefixed literal is the shape a reader has met most.
Recorded so the author can choose it: it is a live branch, not a rejected one.

**The coordinator was wrong twice and both were caught by seats.** The
recommendation put to the panel was *any expression* on two measurements, and it
omitted the axis that decides it: whether the literal is gated. And the ceiling
arithmetic given to the author said 87 free is *"one more than the 78-token
spread"*; it is **nine** more, which the warden caught, and the costed ceiling
for the resolution adopted below is **4253** rather than 4224.

## Resolution — PROVISIONAL, author ratification pending

The most robust and complete resolution, not the cheapest and not a compromise
(CLAUDE.md §4).

**R1. A hole admits any expression**, and the spec states the scan: it ends at
the `}` that closes it, nested brackets and nested literals skipped. Coverage
100% of 2536 holes against 48.7% of chains for a bare name; the enumeration of
admitted forms costs more tokens than the permission; precedent is Python, C#,
Swift, Ruby, JavaScript, Nim, D and WG21; and Rust's narrowing has been unable
to widen by one dot in four years.

**R2. The brace is active only inside a gated literal.** Unanimous, on the
Principle 0 regression: 211 literals in 39 of 190 modules stop parsing and the
compiler stops compiling itself. This also erases the two-stage bootstrap hazard
and the 312-literal migration entirely, and answers the lone-brace silence,
because a plain literal is unchanged.

**R3. The gate is an `f` before the opening quote**: `f"line {n}: {word}"`. The
cheaper branch, `\(e)` at −24 tokens, is recorded above and is the author's to
take; it needs panel 008 reconvened first.

**R4. One AST node carrying the whole literal's span, desugared in lowering.**
Not in the parser, which would let the formatter erase the form from its own
tree; not with a payload on `.str_lit`, which would leave 59 walks treating an
interpolated string as a childless leaf with no compile error, names in holes
unresolved and a call inside a hole absent from the call graph. Lowering is
after the checker, which is what the ffi seat requires so that a `str` hole
emits no `hero_str_identity`.

**R5. The form does not land until `selfhost/grammar_expr.hero` has a legal
home.** It is at 1002 of a decided 1002; a new module that calls `parse_expr`
and is called by `primary` closes a `use` cycle, and the escape hatch is shut
because a function type cannot declare an `@` parameter. Either the `DECIDED`
number rises with a written reason, or the file splits in its own milestone
first. This is panel 119's *"no legal home for the parser"* arriving nineteen
hours later against a different feature.

**R6. The two silences already accepted land with it**: one definition of
*abort* at `spec:71` (+12, covering all eight sites) and the printed float's
round-trip sentence (+9).

**R7. The ceiling costed for R1 to R6 is 4253**, not 4224: the spec reaches
**4113**, the FFI floor mortgages 60, the check fires at `>=`, and the spread
between the two vendored instruments is **79** at that size. Under 4224 the
resolution has 50 free against a 79-token spread, which is inside the noise of
its own instrument.

**R8. Defects 022 and 023 are filed, and 022 blocks the tag.** Robustness is
rank 3 in CLAUDE.md § Precedence and a milestone is tagged only over a clean
list. The escaping `cstr` is a silent wrong answer with memory corruption; it
predates this sitting and it is this milestone's to own.

**R9. Principle 0's burden is NOT met, and the record says so rather than
pretending.** No reader has been put in front of either form; metric 2 has never
run; its home is M-thesis-harness. The author's decision to land the form stands
over the warden's veto, which is the author's to make (CLAUDE.md §3), and what
would have met the burden is written here so that a later sitting can collect
it. Defect 023 is evidence the mistake is real; it is not the measured Part 11
effect §2 asks for.

**What a veto would compel if the author declined to override it**: the form
waits for metric 2 at M-thesis-harness, the two accepted sentences land alone at
+21 inside today's 4096, and design.md Part 7 item 7 gains a dated return
condition naming metric 2 as its instrument.

**R10. This milestone closes with the verdict and not with the form.**
`M-interpolation-verdict` was named for a ruling on purpose, so that it could
close with a refusal; the ruling is now given and the id may not claim the
implementation. The spec clause, design.md §1.6, the ledger row, the pins, this
file and the two defects close it. The implementation takes a new id the author
places (`.claude/rules/records.md`).

## Author's verdict

*Pending.*

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | `tests/harness/suite_layout.hero`'s `DECIDED` holds a number strictly greater than 1002 for `selfhost/grammar_expr.hero`; falsified if the form ships with 1002 and `layout` green | the milestone that ships the form |
| llm-ergonomist | ≥6/10 fresh models emit undoubled lone braces under the ungated proposal, 0/10 remark on the silence; with the lone-brace sentence, doubling rises to ≥8/10 | M-thesis-harness, metric 2 |
| spec-warden | at close, `heroes measure spec/heroes-spec.md` reads ≤ 4047 and no second sentence about holes is needed; falsified by one further spec sentence, one surviving unescaped brace, or a red `spec/spendable` | the milestone that ships the form |
| ffi-pragmatist | `heroes build --sanitize` reports heap-use-after-free on the 16-line `cstr` case, and the plain build prints an empty line at exit 0, on every seed up to this milestone's close, unless the escaping `cstr` is refused | this milestone's close |
| historian | `"{{{n}}}"` and a hole containing a `}` inside a nested literal land in `docs/work/DEFECTS.md` within the milestone that ships the form, unless the spec states the brace-run resolution and the skipping scan | the milestone that ships the form |
