# Panel 159 — the document was not the only thing that was behind

2026-09-16, at M-check-completeness. Five seats and the completeness critic.
**Resolution provisional — ratified the same day by the author's standing
delegation, recorded at the foot.**

Convened on three rules a reader of `spec/heroes-spec.md` had to guess at, filed
by panel 126's llm-ergonomist out of three tasks written twice each: is `sort`
ascending, is `xs[i] @ v` accepted, may `main` be `-> ()?`. The shared brief
answered all three from the compiler and concluded, in its own words, *"**So none
of the three is a compiler defect.** All three are silences."*

**That sentence is the sitting's premise and it is false twice.** Two of the
three silences sit on top of compiler defects, one of which no seat found. And
the sentence two seats independently drafted for the third is itself false.

## The three, as put, and the verdict table

| seat | verdict | veto |
|---|---|---|
| **compiler-engineer** | object — two of the three are not silences of the same kind | not cast; **would** veto accepting `main() -> ()?` without a specified exit code |
| **llm-ergonomist** | approve, three sentences | not cast — none of the three is non-local |
| **spec-warden** | object (provisional: every delta is vendored) | not cast; nothing approaches the ceiling |
| **ffi-pragmatist** | object on R3's framing | not cast; nothing touches the C ABI |
| **historian** | approve (advisory) | none |

## What was measured, and it moved every question

**R1 is not an unknown fact. It is a fact published to the wrong reader.**
`runtime/heroes_runtime.h:420` reads `/* sort(xs) — a NEW array, STABLE,
ascending.`, and `runtime/parts/sort.c:10` says it again. The critic found both;
the compiler-engineer had walked past `:420` while reporting the rot at
`:421-422`. **So the project has already promised ascending and stable — to a C
author including the header, and not to the Heroes author reading the one
document we tell them is the whole language.** That asymmetry, not the token
price, is the argument the sitting never heard.

**And *ascending* alone does not state R1.** The spec-warden ran
`print("a" < "b")`: exit **1**, `error[bad_operand]: '<' takes any integer or a
float, found 'str'`. `spec:172` is the rule — `< <= > >=`: a number only. So the
document defines **no ordering relation on `str` at all**, while its only `sort`
example sorts `str` (`spec:288-289`). A reader told *ascending* is told the
direction of a relation the document does not have.

**Byte order holds above ASCII**, which was the warden's own UNRUN and the critic
closed it: `sort(["é", "z"])` → **`z,é`**, exit 0. `é` is `C3 A9`, `z` is `0x7A`.
Bytes, not code points and not collation — so *a `str` by bytes* is safe to write.

**Stability is observable, so the silence about it is not a ruling.** The
llm-ergonomist held that equal elements are structurally identical and no program
can tell. Measured: `sort([0.0, -0.0])` → `0.0 -0.0` and `sort([-0.0, 0.0])` →
`-0.0 0.0`. The order is the input's, `runtime/parts/sort.c:47-48` commits to it
in prose, and the header publishes it.

**R2's form is core, not an accident of the grammar.** A dedicated place-path
walk (`selfhost/emit/container.hero`, 393 lines, `:150-234`), a runtime part that
exists only for it (`runtime/parts/cow.c`, 126 lines), a unit test, a fixedbugs
note from 2026-08-17 and `design.md:1343-1349`.

**R3's refusal covers one third of what a sentence about `main` would claim.**
`selfhost/check/decls.hero:81` guards `result != unit_ty`, so `-> ()?` and
`-> i64` are refused and **`-> ()` passes by construction**: `function main() ->
()` is check 0, run 0. Parameters are not guarded at all.

## THE HINGE: two seats drafted the same false sentence, from opposite inputs

The llm-ergonomist proposed *"`function main()`, that signature and no other"*.
The spec-warden priced *", that signature exactly"* at **+4**. Both are **false**,
and the critic ran the program that shows it. Landing either would have put a
claim in the one document a reader is told to trust that the compiler
contradicts at exit 0 — **manufacturing a defect that does not exist**, which is
the inverse of every failure this project has catalogued.

Only the compiler-engineer found it, and its correction is the one wording on the
table that survives: the true sentence is the diagnostic's own. `main`
**produces nothing**.

**The sitting has no mechanism that would have caught this.** Two of five seats,
reading different inputs, proposed the same false clause, and the first place
their wording met a compiler was the critic.

## THE STRUCTURAL GAP, and it is not a seat's failure

The llm-ergonomist's R2 sentence merges into `spec:283-285`, whose remainder
reads *"reached through a field or an index it copies the whole array"*. Measured
on the emitted C, exit 0 throughout:

| program | emitted |
|---|---|
| `b.xs[1] @ 99` through a field | `hero_array_set(&(h0_b.f_xs), …)` — **no copy** |
| `b.xs @ b.xs.push(9)` through a field | `hero_array_push(…)` — a copy |
| `g[0] @ g[0].push(9)` through an index | `hero_array_push(…)` then `hero_array_set(…)` — a copy |

The copy rule is true for `push` and **false for `xs[i] @ v`**. The cheapest
draft on the table would have published a false performance rule.

**And that seat could not have known.** Its brief forbids it the repository —
that is the point of the seat, and it is why its verdicts carry information. So
this is a gap in the sitting's DESIGN: a seat that proposes exact wording from
the document alone will sometimes propose a sentence the compiler falsifies, and
nothing routes its drafts back through a seat that can compile them. They meet
for the first time in the synthesis, by which point a resolution has been
adopted. **Recorded as owed by the process, not by the seat.**

## The prices, all vendored and all a LOWER BOUND

`ANTHROPIC_API_KEY` is unset on this machine, so `heroes measure
spec/heroes-spec.md --refresh` exits **2** and the binding `claude-opus-5` number
could not be taken. Base **5989** `cl100k_base` / **5863** legacy / **7974** real,
the last cached 2026-09-15. **The vendored-to-real gap on this document is 1985
tokens**, so every delta below understates by about a third and no verdict rests
on one. Author instruction of this day: *always measure with the real*, now
`.claude/rules/spec-shape.md` § How a change to the document is made.

| draft | vendored delta |
|---|---|
| R1 complete, merged into § 11's `sort` parenthesis | **+11** |
| R1 one word (`ascending`) merged | +2 |
| R2 GENERAL, merged into § 5's `@` rule | **+9** |
| R2 partial (`xs[i] @ v` alone), merged into § 10 | +10 |
| R3 merged | +4 |
| § 3's `and only a group's record may hold one`, duplicated by § 13 | **−14** |

**For R2 the COMPLETE rule is cheaper than the partial one, +9 against +10.**
Naming the general form costs less than naming one of its three instances, and it
carries no *expressio unius* damage: prose for the element instance alone
restates a third of a rule stated once in the notation and makes the other two
look excluded.

**Panel 122 holds a second time**: merged +16 against free-standing +32.

## THE ROUTE NOBODY LISTED

**Define `<` on `str`.** `spec:183` already says *"ordering one aborts, in
`< <= > >=` and in `sort`"*, so if `<` worked on `str` the document would hand the
reader the ordering relation for free and R1's spec cost would fall toward zero.
It is a language change, it is panel business, and it appears in no brief and no
report. **Not taken here** — it is a new sitting, not a clause of this one — and
filed so it is refused on the record rather than by omission.

## The resolution adopted

**R1. The document states the direction with the COMPLETE sentence, merged into
§ 11's existing `sort` parenthesis, and the two repository copies are corrected
in the same act.** `spec:294` already carries the type set correctly and
generically — *"`sort` (a number, `str` or `bool`, never a type parameter)"* — so
the sentence names the RELATION per kind and never the type list: *a number by
value, a `str` by bytes, `false` before `true`*. **+11 vendored.** A thirteenth
integer width lands under *a number by value* without touching it; a type list
would rot, and the proof is that the same list has rotted in three places.

The header and `sort.c` corrections cost **zero** spec tokens and remove a
falsehood a binding author reads today. **They are a second witness of defect
051** — a comment asserting an invariant the compiler falsifies — and are owed
there, at the class and not at the witness.

**What conservative would have been**: the one word, **+2**. Refused because the
warden measured that it names a relation the document does not define.

**R2. The GENERAL rule, merged into § 5's `@` sentence, at +9 — and it lands with
defect 052's repair or not at all.** *`@` declares a mutable cell and re-binds
it, or a field or element inside one.* It is the only sentence on the table that
also covers `f(@xs[i])`, the out-parameter into an array element, which the
grammar derives, the compiler accepts at exit 0 and the document mentions
**zero** times (`grep -nE '@[a-z_][a-z0-9_]*\['` → 0 hits).

**Not merged into `spec:283-285`**, measured false for this form above.

**The ffi-pragmatist's attribution is corrected and its finding is strengthened.**
That seat demonstrated `@dbs[i]` and `dbs[0] @ d` with real `sqlite3.h` bindings
and concluded R2 is FFI documentation. The critic ran the same form with **no
`extern` anywhere** — `bump(@xs[1])` and `bump(@xs[i])`, check 0, run 0 — so the
form is plain Heroes and is undocumented on **every** boundary, not the C one.
The seat found it at the boundary because that is where it was told to look. Its
conclusion stands and its home moves: § 5, which is exactly where the spec-warden
priced it. **Two seats reached the same sentence from opposite ends and neither
noticed.**

**R3. The sentence is the diagnostic's — `main` PRODUCES NOTHING — and it waits
on defect 053.** Never *that signature exactly*, which two seats proposed and
which `function main() -> ()` falsifies at exit 0. And the sentence is held back
until the compiler checks what it would claim: `function main(n: i64)` is check
**0** and run **2**, `internal error: compiling the generated C failed: too few
arguments to function call`. A sentence about `main`'s signature that the
compiler does not check on parameters, and contradicts in clang's voice, is worse
than the silence it replaces.

**R4. Merged, never appended, and the homes are settled here because no seat
agreed on them.** R1 → § 11's `sort` parenthesis. R2 → § 5's `@` rule. R3 → § 1's
first bullet. Three seats named three different homes for R1 and three for R2;
`.claude/rules/spec-shape.md`'s one-rule-one-home is the tie-break, and it puts
each rule in the section of the operation it governs.

**R5. The payment is § 3's `and only a group's record may hold one`, −14**, a
true duplicate of § 13, which is `cstr`'s home. § 3 keeps its own half. With R1
and R2 landing and R3 held, the document **shrinks by 6 vendored tokens while
saying two things more**.

**R6. Three defects are booked.**

- **052**, the `str` element store, found by the compiler-engineer on exactly
  R2's form. `s[0] @ 65` is check 0 and run 134 saying *this is a compiler bug*,
  against `spec § 3`'s own *immutable UTF-8 string*. The critic widened it from
  one shape to three and corrected the repair's home: the guard the seat priced
  sits in the EMITTER, which runs after `check` has passed the program, so the
  `check` golden its prediction registers cannot exist until the refusal is in
  the CHECKER.
- **053**, `main`'s parameters, found by the critic and by no seat.
- **051 gains a second witness**, `runtime/heroes_runtime.h:422` and
  `runtime/parts/sort.c:10`, both still naming three orderable types where the
  dispatch names twelve.

**R7. What R2 teaches sends a reader to an edge the document has never stated.**
The next program after an element write is a swap, and the llm-ergonomist's own
task 2 was one. Measured: `shift(a: @xs[0], b: @xs[1])` is check 0, run 0; with
variable indices it is check 1, `error[aliased_mutable_arguments]`. The rule is
*decimal-digit literals only*, its single home is `design.md:1343-1349`, and the
spec's entire word on aliasing is `:63`. **Filed, not written here**: it is a
second sentence with its own price, and R2's +9 does not cover it.

## Predictions to score

| seat | prediction | scored by |
|---|---|---|
| compiler-engineer | a `check` golden for `s[0] @ 65` needs a NEW checker refusal, under 40 lines | at defect 052's close; the critic already falsified its FIRST home — the emitter guard leaves `check` at 0 |
| spec-warden | with a +0 resolution, `measure --refresh` reads 7974 / 5989 unchanged | at the repair's close, and only with the key present |
| llm-ergonomist | on a sort-with-tie-break task, ≥1 in 3 recoveries is silently wrong on ties today | M-thesis-harness |
| llm-ergonomist | ≥1 in 4 first attempts at an in-place swap avoids element assignment | M-thesis-harness |
| llm-ergonomist | ≥1 in 3 first attempts at a fallible `main` writes `-> ()?` | checkable today by compiling four lines |
| ffi-pragmatist | 19 of 20 binding modules never call `exit` | already corrected by the critic from 18 of 20 — the seat's census counted `examples/ledger/main.hero`, which holds no `extern` |
| historian | direction was never a live ambiguity in any language it searched — stability was, twice | the sitting must say whether R1 buys insurance or closes a documented class |

## Process, recorded against this sitting

**No seat was killed by the watchdog**, the fourth sitting in a row under the
write-your-report-first rule.

**Nine claims in the coordinator's briefs were corrected by seats and the
critic**, after seven at panels 155-158. The eighth: the sitting's prescribed
method, `heroes measure <draft>`, reaches only a lower bound, because `--refresh`
refuses every path but the two judged documents (spec-warden). **The ninth is a
rule, not a number**: `.claude/rules/verification.md` § What gates what routes a
`spec/heroes-spec.md` change to `spec` and `special`, and **`suite_grammar.hero:307`
reads the document too** — which is load-bearing here, because R2 amends § 5,
the section holding the `Place` production that `grammar` cross-checks against
`heroes grammar`. A sitting that landed this and ran two of the three suites
would have shipped it ungated, and the rules file's own warning applies word for
word: *a suite name that selects nothing is a green run that tested nothing.*

Two more the critic ran down. The shared brief dated a **cached** number to the
sitting's own day — *"measured 2026-09-16"* against a tool that stamps
**2026-09-15** — which is CL-017. And the compiler-engineer's *"twelve
comparators, each written `return a < b ? …`"* is **eleven plus one delegation**:
`hero_cmp_str` calls `hero_str_cmp`, and the delegated one is `str`, the exact
type R1's sentence is about.

**The critic checked the llm-ergonomist's two asserted program outputs by
compiling them, and both are right byte for byte.** That seat has no instrument
and its brief forbids it one; its factual record this sitting is clean, and it is
recorded here because a seat that cannot measure is the easiest to doubt.

## Author's verdict

**RATIFIED 2026-09-16, as adopted — BY DELEGATION AND NOT BY READING**, under the
author's standing instruction of that day. The yes is the assistant's judgement
under an authority the author handed over, on a sitting the author has not read;
recording it otherwise would credit them with a reading that did not happen.

**What the yes settles.** That R1 lands complete rather than as one word, and
that the header and `sort.c` are corrected with it at zero spec cost. That R2
lands as the general rule in § 5, not as its element instance in § 10, and not at
all until defect 052 is repaired. That R3 is held until defect 053 is repaired,
and that when it lands it says what the diagnostic says. That the `-14` in § 3
pays for the sitting.

**What it does not settle.** Every delta above is vendored and is a lower bound;
the binding number could not be taken on this machine. Defining `<` on `str` is
refused here only in the sense of not being taken — it is a sitting of its own.
And the structural gap this sitting found in its own procedure, that no seat
which can compile ever reads the drafting seat's wording, is named and not fixed.
