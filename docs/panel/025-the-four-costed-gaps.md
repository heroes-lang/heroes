# Panel 025 — the four costed gaps, and the one that was not a gap

**Convened** 2026-08-10, to discharge what panel 024 R4 said was owed: the repair of
panel 023's falsified spec row, plus the three gaps priced with it. **Trigger**
`spec/**` (CLAUDE.md §4).

**Two judges, and the omission is stated.** The llm-ergonomist owns whether a wording
changes what a reader writes; the spec-warden owns what it costs. The
compiler-engineer, ffi-pragmatist and historian were not convened: no form is added,
no C boundary moves, and panel 024's historian had already supplied the precedent
this question needs.

## The candidates, measured before the judges saw them

| | | binding max | Δ |
|---|---|---|---|
| **A** | the spec as it stands | 2231 | +0 |
| **B** | the recursion rule promoted out of the `[T]` table cell, illegal form named | 2278 | **+47** |
| **C** | B, plus the three behaviours the compiler already decides | 2313 | **+82** |
| **D** | C, plus how a variant case is constructed (added after the ergonomist named it) | 2348 | +117 |

The ergonomist received A, B and C **blind**, labelled only by letter, with the task
of writing the same expression evaluator from each and reporting where it stopped
rather than guessed.

## The verdict table

| judge | A | B | C | its own finding |
|---|---|---|---|---|
| llm-ergonomist | **object** | approve | **approve — adopt** | the decisive artifact is a program, not an argument: `clock_at(-60)` prints `-1:0` under A **while the program's own comment says it expects `23:0`** |
| spec-warden | **object** | **approve** | **object** (the slice half only) | could not reproduce panel 024 R4 — and `slice` on `[T]` **never compiles**, in range or out |

## The reversal, and it corrects this panel's own premise

Panel 024 R4 reported that the ergonomist wrote `args: [Expr]` and `lhs: Expr`,
"**both plausible, both compiling**", and ranked the recursion gap **T1** as producing
"a silently different program". The warden ran both. The convener then ran both again:

```
$ heroes check lhs.hero
error[no_size]: `variant Expr` contains itself, so it has no size
exit 1
```

**`lhs: Expr` does not compile.** It is `no_size`, exit 1, with two teaching notes and
a fix naming the repair. So R4's factual claim is false, and its ranking inverted the
real risk order: **T1 is §1.4 working as designed** — one wrong token, a compile error
rather than a wrong answer.

The two judges converge on this without having seen each other. The ergonomist, from
the inside: *"every wrong guess in this family is a **compile error**: the type checker
must compute a size, so it always speaks. So B buys **first-try rate and retries, not
correctness**."* The warden, from the outside: *"zero silent bugs across the experiment
that convened this panel."*

**What that means for the record.** The convener repeated R4's "both compiling" to the
author without verifying it. It was the ergonomist's overclaim in panel 024 and the
convener's failure to run it. B still lands — the argument for it is §1.2's rewrite
rate, one avoided round trip at 500–2000 tokens against +47 — but **it must be recorded
for that reason and not for silent-bug avoidance**, or the project has adopted a change
on evidence that does not exist.

## The gap that is genuinely silent, and it was ranked third

`%` on a negative operand. Verified on all six sign combinations: `-7/3` = −2,
`-7%3` = −1, `7/-3` = −2, `7%-3` = 1, `-7/-3` = 2, `-7%-3` = −1.

The ergonomist's artifact is the whole argument. Same task, same intent:

```
# under A                              # under C
t = offset % 1440                      t: int @ offset % 1440
return Clock(hour: t/60,               if t < 0
             minute: t%60)                  t @ t + 1440
                                       return Clock(hour: t/60, minute: t%60)
```

Under A it prints `-1:0` where its own comment says `23:0`. It compiles, it runs, it is
wrong, and **no instrument in this project fires**. Its note on the mechanism is worse
than a missing rule: A's `%` is **extra-textual** — decided by the reader's prior
language rather than by any line — and A's Python-shaped surface (significant
indentation, `print(...)`, no braces) primes Python's answer of `2` while the language's
arithmetic is C's. *"A worse condition than non-locality, and one I cannot veto, only
measure."*

## The rule this panel actually produced

> **Spend spec tokens where a wrong guess is silent. Spend none where the type checker
> already speaks.**

Ranked by it: C's arithmetic sentence (silent → specified) beats B's by-value rule
(loud retry → specified) beats any sentence about map iteration order (already loud,
needs nothing).

## Q4 was not a gap, and the argument is better than the convener's hint

The convener asked which of the four questions differs in kind, hinting at "an
operation the language does not yet have". The ergonomist sharpened it:

> *"The other three ask about the behaviour of an operation the document **grants**, and
> Q4 asks about an operation the document does not grant. Granting an operation incurs a
> debt: its edges."*

Answering it would be **worse than silence, twice**: it would manufacture the feature by
implication — a reader who is told the iteration order writes `for k in m`, which does
not compile, so the spec would have created a first-try failure out of nothing — and it
would pre-commit the implementation to insertion order, a data-structure choice no user
program can currently observe. The one consequence of ordering that *is* observable is
already specified one bullet up: `==` ignores insertion order.

The warden confirms from the record: panel 022 re-deferred that sentence to the
milestone that lands `for k in m`, re-priced at **+12** for order-plus-what-iteration-
yields. **Nothing lands.**

## The defect found by pricing a sentence

The warden went to verify that "`slice` out of range aborts" was true, and found it
unverifiable for one of the two types the sentence covers:

```
$ heroes check w7.hero     # ys = slice(xs, from: 1, to: 3), xs: [int]
exit 0
$ heroes run w7.hero
internal error: compiling the generated C failed:
  error: passing 'HeroArrayHeader *' to parameter of incompatible type 'HeroStr'
    t9 = hero_str_slice(t6, t7, t8);
exit 2
```

**`slice` on an array never compiles** — in range or out. It type-checks at exit 0 and
then reports exit 2, which CLAUDE.md §7 reserves for "the compiler is wrong", on a
program the author is entitled to write: spec line 148 lists `slice` and does not
restrict it to `str`.

`len` has the same one-built-in-two-operands shape and needed no gate row, because both
halves landed in the same step. `slice`'s did not, and the gate checked the built-in **by
name** — so the array half sailed through a list that was about names. Fixed here: the
row splits by operand, and the array half is refused until `hero_array_slice` exists
(M6, with `sort`, `join`, `chars` and `range`).

Filed as `tests/golden/unsupported/fixedbugs-array-slice-reached-clang.hero`, which
keeps the `str` half in the same file so the refusal is provably about the operand.

**No test in this project looked there.** It was found by a judge checking whether a
sentence it was pricing was true.

## Resolution — provisional, author ratification pending

### R1 · Land B and C's arithmetic. **2231 → 2304, +73 measured.**

The exact diff: the `[T]` cell loses `; how a type contains itself` (**−6**); a rule
lands in the Types list saying a record or variant holds its fields by value and may
contain itself **only** through `[T]` or `{K: V}`, with both forms shown; and the abort
sentence's parenthetical `(integer division truncates)` is replaced by `/` and `%`
truncate toward zero, with `-7 / 3` and `-7 % 3` spelled out.

Two smuggled parentheticals deleted, one rule promoted, one silent gap closed.

**The word doing the work in B is `only`** — the ergonomist stopped there rather than
guessing: *"`only` converts an example into a closed set."* Under A it would have tried
`child: Expr?`, hit the compile error, and retried; under B it never wrote it. And A was
not merely quieter: its `only through [T]` phrasing is **restrictively wrong** about
`{K: V}`, which steered the judge to believe maps were excluded at 60% confidence.

### R2 · Hold C's slice sentence. **+9, and it fails all three teeth.**

No design.md mandate (design.md says out-of-bounds *index*, not slice), a genuinely
open decision (panel 024 T4 lists clamp / abort / `T?`), and behaviour verifiable on
one of the two types it covers. It lands when either `slice` on `[T]` compiles and its
abort is verified on both, or it is scoped to `str` as an explicit decision on panel
021's `to`-excluded model.

### R3 · The category, with teeth — the warden's, adopted

Three of C's additions state behaviour the compiler already implements. Is documenting
it an *addition* under §1.2, or the spec catching up? The warden argued both and picked
the second **narrowly**, and the bound is what makes it safe: the category is
**"discharge of an explicit design.md specification mandate"**, never "documentation of
implemented behaviour" — the second phrasing is an infinitely large drawer, drawn down
one unobjectionable sentence at a time, which is §1.6's own named failure mode. Three
teeth, all checkable by a reader:

1. **Cite a mandate, not a consistency.** The sentence must quote a design.md clause
   saying the thing *must* be specified. §4.14's own heading — *"Arithmetic edge cases,
   all of which must be specified because unspecified means the model invents"* — is
   what admits `%`.
2. **Decide nothing.** If design.md is silent and the panel must choose among plausible
   answers, it is an addition at full burden. This rejects `slice`.
3. **The warden verifies, on every type the sentence covers.** This tooth is what found
   the array-`slice` defect.

### R4 · Nothing on map iteration order

Already loud, already deferred, already priced at +12 for the milestone that lands
`for k in m`.

### R5 · The gap no candidate closed, and it is now costed

**The spec never says how a variant case is constructed.** Declaring is there, matching
is there, building is not — and it hits every AST program. The ergonomist calls it *"the
highest-yield ~15 tokens on the table and no candidate spends them"*; measured as
candidate D it is **+35**, not 15. Not landed here, because this panel's mandate was the
four gaps and a fifth addition would breach the warden's own five-amendment prediction.
Queued with its measurement.

### The three requirements the warden set on the landing, all met

1. **Correct the record.** Done above, and in the DESIGN-LOG: B lands for §1.2's
   rewrite rate, not for silent-bug avoidance.
2. **Re-register the predictions per item.** Panel 024's bundled T1–T4 prediction is
   unfalsifiable and T2 is not shipping. Separate numbers below.
3. **File the array-`slice` bug.** Done, as a named fixed-defect case.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | llm-ergonomist | tree/AST first-try, B−A and C−A: **+5 to +12 points**, and **silent-error delta 0.0** — zero silently-wrong AST programs under any variant | next harness run |
| 2 | llm-ergonomist | negative `%`: under A **≥40%** of attempts silently wrong, under C **≤10%**; silent delta **−30 to −40** | next harness run |
| 3 | llm-ergonomist | **≥50%** of first-try AST attempts under **all three** variants use a variant-construction form that does not parse, the three within 5 points — the gap R5 leaves open | next harness run |
| 4 | spec-warden | with B landed, `no_size` fires in **≤1 of the next 20** fresh AST samples, against a pre-B rate of 5–8 | next harness run |
| 5 | spec-warden | the arithmetic sentence drops `%`-on-negatives silent-wrong output from **≥30% to ≤5%**; the slice sentence would have moved **0 ± 2** | next harness run |
| 6 | spec-warden | the binding count stays **< 2450 through 2026-09-30** (tightened from panel 024's 2600, because +73 spends the slack early), and **the next amendment is net ≤ +25 or carries a measured removal** | that date |
| 7 | spec-warden | if the next three amendments total more than **+75 with zero removals**, the mandate category has become the drawer it must not be, and the delta gate is adopted immediately rather than at architecture's convenience | standing |

## Watch list

- **B is the first amendment in eight to take panel 012's branch 1** — a named removal,
  measured at exactly the −6 that panel 023's clause cost. §1.6 records that the clause
  had produced zero removals in seven amendments; this is the eighth and it subtracts.
- **The raise-independence test**, and it is the warden's answer to its own panel-024
  objection: *a spend that would have been affordable under the previous ceiling is not
  evidence of the raise being spent.* 2304 clears the **old 3000 ceiling by 696**, and
  R4's defect was found by reading the spec rather than by noticing headroom. The 915
  unclaimed tokens of the 4096 raise are **still unclaimed**.
- **`heroes measure` reports two instruments and panel 011's data had o200k highest**
  (2050 vs cl100k's 2048). "Maximum" is a maximum over two of three known tokenisers.
  Immaterial at 1792 headroom; not immaterial if anyone ever cites it as absolute.
- **Two gaps neither candidate touched and both judges named**: `T` → `T?` widening
  (unspecified, used constantly) and `if`-as-an-expression's layout (the spec says it is
  one and shows only the statement form). Both fail loudly; both cost retries.
- **The ergonomist's condition worth watching**: *"if the harness scores 'compiles'
  instead of 'correct output', A ≈ C by construction, my whole margin vanishes, and the
  measurement is the wrong measurement — that result should change the harness, not the
  verdict."*

## DESIGN-LOG

Appended 2026-08-10 — see the lines citing panel 025.
