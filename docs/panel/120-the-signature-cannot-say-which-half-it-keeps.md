# Panel 120 — a signature cannot say which half it keeps

Date: 2026-09-08. Trigger: **author instruction, mid-step.** Panel 119's
llm-ergonomist reported that what gated all three of its tasks in all five arms
was not the absence of closures but the spec's silence about `filter`'s type; the
coordinator filed it as an item in `docs/work/DECIDE.md`; the author's answer was
that it is opened at once rather than left in a list.

**Four seats, not five.** The ffi-pragmatist was not convened: this question has
no ABI, no C and no `extern`, so its input would not differ from the others'
(CLAUDE.md §4, CL-023's *"choosing only the seats whose input differs"*). Naming
the seat that did not sit is part of the record.

## Proposal (as put to the judges)

`spec/heroes-spec.md:188` names seven functions as *"written in Heroes"* and the
spec states the type of some of them. Write the missing ones. Measured before the
briefs went out, base **3965**:

- **H1** — the five spelled out one by one, appended to § Functions and calls. +86.
- **H2** — the five compressed into one appended sentence. +69.
- **H3** — the four missing ones compressed, appended. **+63**.
- **H4** — refuse: the spec stays silent.

Recommended provisional as put to the judges: **H3**. The evidence replaced the
whole ballot; see *Resolution*.

## The ballot was wrong in four ways, and every one was found by a seat

The option set is itself a measurement (§ RUN IT, CL-057), and this one was
mis-measured four times over:

1. **The spec states TWO of the seven types, not one** — `map` at `:121` and
   `range(from: a, to: b) -> [i64]` at `:145`. The llm-ergonomist's correction,
   confirmed independently by the historian.
2. **`fold`'s argument order is already written**, at `:116-117`, *"folding left
   with the accumulator first"*. Two seats found this separately, and both
   objected to paying for it again. It was a false premise of the brief.
3. **Every draft appended prose to § Functions and calls, seventy lines above
   the place a reader meets the names.** The ergonomist's finding: the spec's own
   habit is to carry the fact beside the name.
4. **And the one that ends the ballot: a signature cannot state the fact that
   matters.** The historian's words: `function filter<A>(xs: [A], f:
   (function(A) -> bool)) -> [A]` is **byte-identical for keep and for reject**.
   So H1, H2 and H3 buy the loud class — arity, return type — and buy **nothing
   at all** for the silent class this sitting was convened about.

## What the budget is, corrected during the sitting

`tests/harness/suite_spec.hero:204` reads `if SPEC_TOKENS + FFI_FLOOR >= CEILING`,
so 4096 is itself red, the spec's own ceiling is **4035**, and the free budget is
**70**, not the 71 the coordinator had written into four records. `heroes
measure`'s printed sentence is exact — *"the check goes red at 4096"* — so this
is the coordinator's subtraction and not defect 017 returning. Corrected under
the original sentences with its date (`docs/measurements/020`).

**A live condition of the author's own decision is already met**, reported by the
spec-warden rather than found later. `vendor/tokenizers/README.md` carries the
2026-08-26 ruling that o200k is not vendored, with two triggers: *"vendor it
before any verdict lands within 10 tokens of a ceiling"* and *"If the headroom
ever falls below the spread, this paragraph is what should be re-read."* The
printed spread is **76**, the usable headroom **70**. The second is true today,
whatever this sitting decides.

## What nobody had run: the compiler calls none of them

`grep -rEn '(^|[^A-Za-z0-9_])(map|filter|fold|find|any|all)\(' selfhost
--include='*.hero'` returns **zero** call sites. The coordinator's first run
returned 14 and every one was `.map(` — the variant case for the map **type** in
the compiler's own type table — or a string inside `library_source.hero`. So the
self-hosted compiler, 190 modules and 55,361 lines, never calls `map`, `filter`,
`fold`, `find`, `any` or `all`: it writes loops. **No option here enters on
compiler-need**; every one must serve the thesis or fail.

## The three facts, run rather than reasoned

```
$ heroes run pol2.hero          # xs = [1, 2, 5, 7] · is_small(n) = n < 3
1, 2                            # filter KEEPS what the predicate accepts
1                               # find gives the FIRST, not any
true                            # find with no match is_err()
$ heroes run code.hero
not_found                       # and that code is named NOWHERE in the spec
```

The compiler seat sharpened the second: `find` gives the first **in array
order**, not the smallest — `[5,4,3,2,1]` returns `4`. And the spec names
`missing_key` for `m[k]` at `:160` and no other code, so a reader writing
`e.code == "no_match"` gets a branch that never fires. **That is silent.**

## Verdicts

| Judge | Verdict | Section | Cost / delta, run | Prediction | Condition |
|---|---|---|---|---|---|
| llm-ergonomist | **object** to the removal; **approve** the replacement; H2 ≡ H3 a **null result** | the thesis; §1.1 | wrote 16 programs over 4 tasks × 4 arms. **H2, H3 and the fuller draft produced byte-identical programs**, and so did the status quo. Silent-capable facts left open: status quo **2**, H2 **2**, H3 **2**, the prose draft **0**. Four tasks needed **five** named helpers that exist only to be passed | n=40/arm: unprompted labels **KEEP ≥85%, DELETE 20-35%**, so first-attempt failures rise **+50 to +65 pp**; end-state after one round trip **0 ± 3 pp**; silent-wrong delta **exactly 0.0**; and the one that matters — **≥40% of DELETE-arm signatures use body names rather than call-site names, against ≤15% in KEEP** | **not a veto, and the boundary is written down**: deleting the sentence while the compiler still enforces is ergonomics; deleting the **check** is soundness and an instant veto. Withdraws if prediction 4 comes in under a 15 pp gap, or if anyone **names a clause that needs 75 tokens and cannot fit in 40** |
| compiler-engineer | **object** — on placement and grammar, not on truth | Part 5 (`design.md:2323`); §4.17 | every semantic clause **true as run**: `filter` keeps (`2,4,6` from `1..6`), `find` gives the first in array order (`4` from `[5,4,3,2,1]`), code `not_found`, all four predicates take exactly `(function(A) -> bool)`, rejecting `i64` and `bool?`; prefix and UFCS both work; `fold`'s accumulator-first and `start`-second both hold. **Zero compiler lines** — not one of Part 5's seven constructs. **Proved the embedded library live by MUTATION**: changed `find`'s code to `PROOF_LIVE`, rebuilt (56.61 s), and the live compiler reported `PROOF_LIVE` where the seed reported `not_found` | scored by `heroes run tests/harness/main.hero -- ./heroes spec`: if the wording's last space-free backticked fragment is `range` **and** no type sits in a space-free code span inside the `Built-ins:` sentence, `spec/offered` and `spec/names` pass with exactly **24** harvested names; with the coordinator's draft verbatim, `spec/offered` fails naming `bool` and the harvest is **23** | approves on either: the exact final wording is run through the spec suite before it lands with 24 names harvested; or the panel rules the harvest rule is what changes, in which case the `range`-last guard is **replaced** and not deleted — `spec_text.hero:98-106` records what deleting it cost last time. Asks that both defects it found be filed this sitting |
| spec-warden | **approve H3's successor paid by a removal**; **veto H1**; object H2, H3, H4 | §1.6 (`design.md:253`), §1.4, §1.2, §1.0 | `cl100k_base` binds every row. **H1 4058/4101 → red**, H2 **4063 → red**, H3 4028/4030 → green but **18 above §1.6's own stated effective bound of 4010**. Priced the mandatory-named bullet at `:108-109` as a removal: **−35**, measured in isolation and in place. Reproduced the coordinator's +30 draft **exactly** | at M-guide-book close, the amended `Built-ins:` sentence needs **no follow-up sentence** about the predicate type, `any`/`all`'s results or `fold`'s type parameters; and `grep` still shows **0** fold sites whose accumulator type differs from the element type (today 13, all `B == A`), so H3's extra 33 tokens would have bought nothing measurable | **veto on H1 unconditional**: red on all three of its drafts. H3 needs o200k vendored first — it leaves 7 against the author's 10-token trigger — while a draft leaving **≥25** clears it. Gave the ledger duty with its numbers rather than its shape |
| historian (advisory) | **approve**, but **not the proposal as costed**; objects specifically to paying for `fold` again | precedent | **not one specification of a statically typed language names a standard function without its type**, across seven checked: C11 N1570's Synopsis prototypes, POSIX Issue 7, Haskell 2010 ch. 20, the SML Basis, Go's spec plus `builtin.go`, the JLS/javadoc, the Rust Reference. **The two that omit types are untyped** — R7RS gives a call template, Lua's manual argument names — so *"short specs drop signatures"* is false: **Wirth's 17-page Oberon report calls itself "intentionally kept concise" and still spends §10.2 on a table of Argument type and Result type.** Size discipline cut features, never their descriptions: R7RS split off a large language rather than shrink them | if the +63 lands as four full signatures, the spec **still** will not answer *"does `filter` keep or drop?"* or *"what code does `find` fail with?"* — testable by grep today. If both stay absent after the spend, 63 of a 71-token budget went to the wrong place | withdraws on: a statically typed language whose spec deliberately omits its standard functions' types, with a recorded reason; a language that ships `filter` with **reject** polarity; the Endrikat ICSE 2014 text, which it could not retrieve and on which it therefore rests nothing; or a grammar production making *"no anonymous functions"* derivable |

## Findings that changed the proposal

**The historian's central finding ends the ballot, and it is one sentence.** A
signature is byte-identical for keep and for reject, so the expensive half of
every draft bought only the loud class. What closes the silent class is **prose**,
and prose is cheap. Every specification it opened states the polarity in words
and none leaves it to the name: Haskell 2010 (*"returns the list of those
elements that satisfy the predicate"*), the Java SE 8 API spec (*"elements … that
match the given predicate"*), and SRFI-1, which pairs `filter` with `remove` and
`partition`. **And the ambiguity is argued on the record**: Ruby Feature #13784,
Michael Gee — *"The word 'filter' implies a separation, but does not convey which
part we are 'keeping' like 'select' and 'reject' do."* Matz approved the alias;
it landed in Ruby 2.6. The seat searched for a language shipping `filter` with
reject polarity and found none — so the behaviour is universal and universally
written down anyway.

**The strongest precedent in the sitting is for the cheapest line.** Wirth's
Oberon report states its own test — *"What remains unsaid is mostly left so
intentionally, either because it is derivable from stated rules of the
language"* — and then obeys it in seventeen pages: §10.1 spells out that a
procedure-typed actual parameter *"cannot be a predefined procedure"* and that a
result type *"can be neither a record nor an array"*. **That first sentence is
the same fact as Heroes' `:116`**, which functions are values, and Wirth writes
the exclusion out loud. Heroes' spec has no grammar production for a lambda, so
*"no anonymous functions"* is **not derivable**, and by Wirth's own criterion it
is stated. Two more: the JLS reserves `const` and `goto` though unused, *"to
produce better error messages if these C++ keywords incorrectly appear in
programs"* — Heroes' thesis with a 1996 date — and the Rust Reference lists its
reserved-but-unused keywords with the same reasoning.

**And the asymmetry that makes it binding here.** Go states a few refusals in its
spec and keeps the bulk in a FAQ with at least fourteen refusal headings. Heroes'
`spec:3` says *"This document is the whole language."* **There is nowhere to
demote a refusal to.**

**The ergonomist overturned the warden's removal, and the argument is not about
tokens.** The mandatory-named bullet is *"the conversion function from the
signatures the spec lists to the calls a reader writes"*: delete it and
`write_file(path: str, text: str)`, a built-in every program uses and whose call
the spec never shows, stops being derivable. Verified by the coordinator — the
positional call is `error[needs_label]`, and `grep -c "write_file(path:"
spec/heroes-spec.md` is **1**, the signature, with no call anywhere. Worse, the
rule governs the **writer**: knowing it, you name parameters for the caller;
not knowing it, you name them for the body, and `relabel_d(a: str, b: str)`
compiles, runs correctly, and is permanently worse at every call site.
**The `certain` fix is what entrenches that**, because it arrives after the
signature exists and repairs the call site, so nothing ever sends the writer back
to rename. And the bullet is the only thing saying where labels **stop**: without
it the safe generalisation is *"label everything"*, and `len(xs: rs)` fails.

**Headroom is worth nothing to a reader**, in that seat's words, and it made the
only argument that could have saved the removal falsifiable: *"I cannot name a
clause that needs 75 and not 40."* Nobody named one. The removal is not taken.

**The compiler seat caught the coordinator's draft turning a green check red**,
and the second half is worse than the first.
`tests/harness/spec_text.hero:107-136` harvests every **space-free backticked
fragment** inside the `Built-ins:` sentence as an offered built-in name. The
draft's `` `bool` `` is one, so `spec/offered` fails — and `filter` and `find`
**disappear from the harness's coverage** because they sat inside multi-word
spans, which are discarded. A check that stops checking without saying so.
`spec_text.hero:98-106` records why that parser is strict: panel 081 spent a
neighbouring sentence as a removal, the parser fell onto a condemned fallback and
read `inf` out of the float sentence. **The reusable constraint**: inside that
sentence a type may appear only in a multi-word code span or unbackticked, and
the last space-free backticked fragment must be `range`.

That constraint then caught the coordinator's **second** draft before the suite
did, on `` `A?` ``, which would have been harvested as a built-in named `A?`.

**A grammar error in the coordinator's draft, and it would have put a false
sentence in the spec.** *"each taking a `(function(A) -> bool)`"* scopes over the
whole list and is false for three of the seven: `map` is `(function(A) -> B)`,
`fold` is `(function(B, A) -> B)`, and `range` takes no function at all.

**The embedded library is live, and it was proved by mutation rather than by
reading.** The compiler seat changed `find`'s code in
`selfhost/library_source.hero` to `PROOF_LIVE`, rebuilt, and the live compiler
reported `PROOF_LIVE` where the seed reported `not_found`.

**Two defects, filed rather than repaired** (`docs/work/DEFECTS.md` 020 and 021).
The library's `extern` **constants** are readable from every program —
`print(HERO_OS_OK)` prints `0` — while its `extern` functions are properly
walled: one match arm at `selfhost/resolve/names.hero:60` groups `.constant_decl`
with kinds that cannot be extern and carries no guard, where `.function_decl`
carries one twice. And a diagnostic shows a reader `#0`: six lines with an
ordinary generic identity function give `found [#0]`, while
`selfhost/check/render.hero:19-23` states in writing that this happens only
*"where a caller outside the checker has no function in hand"*. **021 fires on
the exact misreading of `fold`'s argument order that this sitting's sentence
exists to prevent.**

**One finding met on the way and not asked for.** `xs.map(to_str)` is
`error[builtin_as_value]` — a built-in cannot be a function value, *"several take
any number of arguments … which no single signature can describe"* — and the spec
does not say so. Go's specification says exactly this about its own built-ins,
and ships a second non-compilable file of fake signatures because its spec's form
is not a usable one. The silence is loud, so by this sitting's own rule it is not
a purchase; recorded because it was met while running the polarity test.

## Resolution — PROVISIONAL, author ratification pending

**Nothing on the ballot is adopted. The types are not bought, the prose is, and
the removal is not taken.** Measured by the coordinator with the harness's own
harvest rule checked by hand before each draft, base 3965, ceiling 4035:

**R1 — the absence is stated, +6.** *"no anonymous functions"* joins the
enumeration already at `spec:110-111`. Wirth's criterion decides it: the fact is
not derivable, there is no grammar production for a lambda, and `spec:3` leaves
nowhere to demote a refusal to. **This is also what panel 119's refusal owes the
spec**: that sitting refused closures, its Part 6 row will live in design.md, and
design.md is not what a reader gets.

**R2 — `filter`'s polarity and `find`'s firstness are stated in prose, +14**, in
the `Built-ins:` sentence where a reader meets the names, harness-safe: the
harvest is **byte-identical to today's** — nine space-free spans, `range` last,
no name lost. *"`map` · `filter`, keeping what the function accepts · `fold` ·
`find`, the first it accepts, or an error · `any` · `all` · `range`."* The
pronoun is *the function*, not *it*, on the ergonomist's note that *"keeps what
it accepts"* can bind to `filter` itself.

**R3 — `find`'s failure code is named beside the one code the spec already
names, +10.** `:160` becomes *"`V?` with code `missing_key`, and `find` fails
`not_found`"*. Backticks are safe there because it is outside the harvested
sentence.

**R4 — the type signatures are NOT bought, and not for the budget.** They are
byte-identical for keep and reject, so they buy only the loud class; `any` and
`all` returning `bool` is near-universal and a wrong guess is a type error; and
`fold` is already written at `:116-117`, which two seats found independently and
both objected to paying for twice.

**R5 — the removal is NOT taken.** The mandatory-named bullet stays. It is the
conversion function from a listed signature to a written call, it governs how
parameters are named, and its `certain` fix repairs the wrong end. Nobody could
name a clause needing 75 tokens that does not fit in 40, which was the
ergonomist's own falsifier for its objection.

**R6 — the total is +30, the spec lands at 3995, and 40 tokens stay free.**
Cheaper than every draft on the ballot and it closes more: both silent classes
plus the unnamed failure code, where H3 at +63 closed neither and would have left
7.

**What conservative would have been, so the author can choose it** (CL-040):
**H4, refuse** — the spec stays silent, no ledger row, nothing corrected, 70
tokens free. Two seats argued against it: the ergonomist because an unspecified
built-in routes a writer onto a hand-rolled path carrying guesses of its own, and
the historian because no specification of a typed language does this. It is
cheaper by one ledger row and one commit.

**What a veto would compel.** Nothing here is vetoed. The warden's veto is on H1
alone and H1 is not adopted; the ergonomist's is reserved for deleting the
enforcement rather than the sentence, which R5 does not do; the compiler seat's
objection is discharged by R2's harvest being identical to today's, and its
condition — that the exact final wording be run through the spec suite before it
lands — is met in the amending commit rather than here.

## Author's verdict

**Pending**, queued as `- [ ] **panel 120**` in `docs/work/DECIDE.md`. Work
proceeds on the resolution above; the verdict is appended here when given, with
follow-up work if it is overturned.

What a `yes` would settle: that the spec spends **+30** of its last 70 tokens on
**prose rather than signatures** (R1-R4); that the absence of anonymous functions
is stated rather than inferred, which is panel 119's refusal made visible to a
reader (R1); that `filter`'s polarity and `find`'s firstness and failure code are
worth buying while their types are not (R2, R3, R4); and that the
mandatory-named-argument bullet stays (R5).

What it does **not** settle: whether o200k is vendored, whose own trigger is
already met and which is an author decision of 2026-08-26 rather than this
sitting's; defects 018 to 021, filed and unrepaired, four of which now block this
milestone's tag under the author's rule of 2026-09-08; and the `builtin_as_value`
silence, recorded and not bought.

A `no` on R1 leaves panel 119's refusal of closures invisible to everyone who
reads only the spec, which is everyone the language is for.

## Predictions to score

| Judge | Prediction | Scored at |
|---|---|---|
| historian | after a signature-only spend the spec still answers neither *"does `filter` keep or drop"* nor *"what code does `find` fail with"* — greppable today | this milestone's close, against R2 and R3 |
| historian | no statically typed language's specification deliberately omits its standard functions' types; seven checked | M-deferral-ledger |
| compiler-engineer | with `range` last and no type in a space-free code span, `spec/offered` and `spec/names` pass with exactly 24 harvested names | the amending commit |
| compiler-engineer | after the `check/render.hero` repair, the six-line generic program emits no `#`, and `grep -c '"#" +' selfhost/check/render.hero` goes 1 → 0 | defect 021's repair |
| spec-warden | the amended sentence needs no follow-up clause about the predicate type, `any`/`all`'s results or `fold`'s parameters | M-guide-book close |
| spec-warden | `grep` still shows 0 fold sites whose accumulator type differs from the element type; today 13, all `B == A` | M-guide-book close |
| llm-ergonomist | ≥40% of DELETE-arm signatures use body names rather than call-site names, against ≤15% in KEEP | M-thesis-harness, only if R5 is ever overturned |
| llm-ergonomist | H2 and H3 are indistinguishable — a null result, do not spend a sitting separating them | already scored here |
