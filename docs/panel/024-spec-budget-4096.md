# Panel 024 — the spec budget rises to 4096 (retro-record)

**Convened** 2026-08-10, after the decision. **Trigger** design.md §1.6, Part 1
(CLAUDE.md §4). **Status** the author decided; this records consequences and real
objections, and does not stage dissent.

**Three judges, and the omission is deliberate.** The compiler-engineer and the
ffi-pragmatist were not convened: a budget number touches neither implementation
cost nor the C boundary, and inventing input for them would have produced
correlated opinion, which is the one thing this panel's design exists to avoid.

**Two attempts.** The first launch of all three failed on a session limit; the
retries produced the verdicts below. Recorded because the record should show that
the judges ran, not that they were paraphrased.

## The decision, verbatim

> Raise the spec budget's hard ceiling to **4096 tokens**.

History of the number: an **unmeasured 1500**, then a **measured 2000** (panel
009), then **3000** (panel 012), now **4096**. The document measures **2231**.

## The verdict table

| judge | verdict | its own finding | condition |
|---|---|---|---|
| spec-warden | **object** (recorded, not blocking — at 2231 there is no breach, so no veto is available) | reconstructed the growth curve: **7 amendments, +183 tokens (+8.9%), zero removals**, while the ceiling moved **+173%**. 4096 leaves **915 tokens with no named claimant** | drop the objection if the closure items arrive with per-item quotes summing past 3100, or if a **delta gate** lands |
| llm-ergonomist | **approve** the ceiling, **object** to spending more than ~300 of the 1865 new tokens | **no measurable effect** at either size; the error predictor is **chain length, not token count** | withdraw if a ~3900-token spec whose room goes to *worked examples* beats a ~2500-token one by ≥4 points |
| historian | **approve** (advisory) | **nobody has published a measurement of spec length against LLM code correctness** — the absence is the finding | a documented case of a size budget raised three times that still functioned |

## Where the three converged, without being able to see each other

**The kernel's resolution is the soft/hard split, arrived at twice.** The warden
defended keeping the soft line at 2000 from first principles: *"a soft line that
moves whenever the document approaches it is a thermometer, not a thermostat."*
The historian, looking at precedent, found the same shape already shipped — Linux
commit `bdc48fa11e46` deprecated checkpatch's 80-column warning and raised its
default to 100, while `Documentation/process/coding-style` **still says 80**.
Torvalds on the commit: *"staying within 80 columns is certainly still
_preferred_. But it's not the hard limit that the checkpatch warnings imply."*
**The limit survived as a preference and was replaced as a gate. Two numbers,
deliberately.** That is exactly soft 2000 / hard 4096, and it is the strongest
argument in this panel for the shape the change already took.

**Examples beat prose, and two judges got there from opposite directions.** The
historian found the only directly relevant published result, and it cuts against
this project's central instrument: Aycock et al. (ICLR 2025, arXiv 2409.19151),
asking whether an LLM can learn a low-resource language from one grammar book,
found *"almost all improvements stem from the book's **parallel examples** rather
than its **grammatical explanations**"*. The ergonomist, reading only the spec,
made worked examples the single condition under which it would withdraw its
objection to spending — because its own worst wrong guess came *from* an example
(line 97's untyped `s.children`) and its hardest blocked line came from the
**absence** of one. Neither could see the other's input.

**Length itself has a measured cost, and it is not about the window.** Levy,
Jacoby & Goldberg (ACL 2024, "Same Task, More Tokens") padded the same task and
found reasoning degrades *"well before reaching the models' maximum input-length
capacity"* — **and that it degrades even when the padding is duplicated relevant
text**. The historian calls this the strongest published justification for having
a budget at all. The ergonomist reached the same conclusion mechanistically
without the citation: *"chain length, not token count, is my error predictor…
doubling the document doubles the average distance between any two lines that must
be read together. So the size ceiling is safe and the size increase is not free —
for a reason that has nothing to do with attention over 4096 tokens."*

## Resolution — the decision stands; three things are recorded against it

### R1 · The soft line stays at 2000, and that is the load-bearing half

Not a fraction of the ceiling. It is the point where growth stops being free — an
addition owes a named removal or a pre-registered falsifiable prediction (panel
012). Rescaling 2:3 would give ~2730 and put the spec, at 2231, *under* it,
granting a blanket exemption to the next ~500 tokens: **16–22 amendments at the
observed mean of +30**. §1.6 names that failure in its own words. Landed before
the judges reported and endorsed by the warden on measurement.

**Honest counter-evidence on the mechanism, from the same measurement:** the
"named removal" clause has produced **zero removals in seven amendments**. It
damps growth; it does not subtract.

### R2 · The veto moves out of reach, and two checks are owed

At the observed mean of +30 tokens per amendment, 4096 binds after **~61 more
amendments** where 3000 would have bound after ~25. A threshold that fires after
five dozen panels is not an instrument. Both replacements are **level-independent
and enforceable at 2231**, and both are queued rather than adopted here because
they are architecture (CLAUDE.md §4):

- a **delta gate** — fail when a commit's spec delta exceeds ~+50 tokens without
  the panel document naming a removal or registering a prediction. Survives raise
  number five, which a level gate does not.
- a **mortgage ledger** — quote each §1.0 closure item separately, so that
  2231 + Σquotes clearing the ceiling is arithmetic a reader can check.

The historian's PEP 8 case is why these matter more than the number: 79 characters
is intact in the document and irrelevant in practice, because Black's 88 decides.
*"If the budget is not measured by `heroes` itself on every run, the practical
budget becomes whatever the author's editor tolerates."*

### R3 · What the raise is not backed by

The mortgage argument justifies about **3181** (2231 + 950), which clears 3000 by
181 — a precise reason to raise. 4096 leaves **915 tokens with no named
claimant, 41% of the current spec** — and the 950 figure was itself derived *from*
the old headroom (3000 − 2048 = 952) rather than costed per item.

The warden refused the convener's framing of the strongest objection, and the
refusal is worth more than the objection: *"a fourth raise in one week is evidence
the number tracks the document rather than constraining it — I do **not** believe
it in that form. The data refutes it. The document has been flat at ~2200 since
08-04 and never came within 700 tokens of the old ceiling, so no raise was
pressure relief. The truth is less flattering: the number tracks neither the
document nor a costed need. It is unanchored in both directions, and 4096 is the
shape of a context window, not of a language."*

The historian confirms the last clause and sources it: **no published guidance
ties a document's or a prompt's size to a power of two** for any reason beyond
convention. NVIDIA's alignment guidance governs GEMM tiling and says nothing about
prompts. *"4096 is a convention — a legible number that rhymes with context
windows. That is a perfectly good reason to pick it; it just should not be
recorded as engineering-derived."*

### R4 · **Panel 023's own resolution is falsified, and by its own reasoning**

This is the finding this panel most owes forward. Panel 023 priced six spec
wordings for the `no_size` rule and landed the cheapest, +6 tokens, appended to
the `[T]` row: `dynamic array, indices from 0; how a type contains itself`. The
argument for it was that a diagnostic can only teach at the moment you are wrong
and can never tell you something is *allowed*, so the spec should buy the licence
and nothing more. The convener called it "the cheapest and only true one".

It is true and it is insufficient. The ergonomist, reading the amended spec,
ranks that clause **T1 — the worst guess in the language**:

> *"Silence made me stop and ask. An implication made me guess **confidently**, in
> a parenthetical clause I read in under a second, and produce Variant A without
> noticing Variant B existed until I re-read. **Rules smuggled into semicolon
> clauses are the highest-risk text in this document.**"*

It wrote two expression evaluators — `args: [Expr]` and `lhs: Expr` — both
plausible, both compiling, and nothing in the document forcing either. Its note
that the language's own reason to exist is writing compilers, and every AST is
this choice, is what makes it the worst one.

**So the +6 form converted silence into ambiguity, which is worse than silence**,
and the panel-023 claim "the cheapest candidate was also the only true one" was
half right: true, and too compressed to function. Recorded as a falsified claim of
the convener's rather than as a footnote. The repair belongs to its own panel,
because it is a spec change; the queue carries it.

### R5 · Where the room should go, if it goes anywhere

The ergonomist's ranked list is the only costed proposal on the table: ~250–300
tokens closing four gaps captures ~80% of the available gain, and the remaining
1600 have a cost-per-point at least **8× worse**. The four, ranked by how badly a
wrong guess turns out:

| | gap | wrong guess produces |
|---|---|---|
| T1 | direct type recursion legal or not (R4 above) | **a silently different program** |
| T2 | a map's iteration order, and whether it yields keys or pairs | silently different output |
| T3 | the sign of `%` on negatives — `-7 % 3` is `-1` or `2`? | **silently wrong arithmetic** |
| T4 | `slice` out of range: clamp, abort, or `T?` | silent if it clamps |

Two more the ergonomist reframed rather than repeated. Its earlier `T?`-as-option
finding is **downgraded by its own author**: it never produced a wrong program,
and the real gap under that heading is **`T` → `T?` widening**, unspecified and
used constantly. And one nobody had reported: **the spec never shows how to
*construct* a variant case** — declaring and matching are both there, building is
not.

## Predictions to score

| # | judge | prediction | checkable |
|---|---|---|---|
| 1 | spec-warden | with SOFT held at 2000, the next five amendments total **≤ +150**; had it rescaled to ~2730, **≥ +250** | after five amendments |
| 2 | spec-warden | `modules` alone measures **< 240** spec tokens when it lands — i.e. the ~950 mortgage is inflated ≥2× and 3000 would never have bound | M8a |
| 3 | spec-warden | the binding count stays **< 2600 through 2026-09-30** | that date |
| 4 | llm-ergonomist | +700 tokens of **new rules**: first-try rate drops 3–8 points corpus-wide, and tasks using none of the new forms still lose ≥2 | next harness run |
| 5 | llm-ergonomist | +300 tokens closing T1–T4, placed adjacent to what they fix: **+8 to +15 points** on recursive-variant and map tasks, ~0 elsewhere | next harness run |
| 6 | llm-ergonomist | the marginal 400 tokens beyond T1–T4 buy **0 to +2** — unmeasurable, therefore unspendable | next harness run |
| 7 | historian | no counterexample exists: a size budget raised three or more times that still functions as a forcing function | standing |

## Watch list

- **The calibration set is not what constrains this project.** Verified page
  counts: Oberon's report is **16 pages** (the 1990 revision — Oberon-07 is 17),
  and **R7RS-small is 88**. "Small" in a spec's name describes the language, not
  the document. Heroes at 2231 tokens is not in the same universe, so §1.6's
  historical calibration is encouragement rather than a constraint. Word and token
  counts for those documents are **unmeasured** — the historian has no counter in
  its role and says so; anyone comparing must run the same tokeniser.
- **The argument for keeping a hard cap at all is Scheme's.** The 2009 Steering
  Committee split the language in two: the bounded small report shipped in 2013 at
  88 pages, and the unbounded large one did not — Cowan resigned as chair in
  August 2023. Thirteen years on, the capped deliverable exists and the uncapped
  one does not.
- **A methodology note worth keeping**, because it silently inflated a
  measurement by 50% before the warden caught it: `git show <rev>:<path>` is
  mangled in this sandbox and yields commit text plus diff. Read historical blobs
  with `git ls-tree`.
- **The reader's real tokeniser is unpublished**, so the ~2% spread applies to the
  ceiling: the effective bound is **4010–4096**.
- **Part 11 would be measuring something unmeasured.** The historian found no
  paper, benchmark or report varying a language specification's length in context
  and measuring correctness of programs in that language. That is either an
  opportunity or a warning, and it is the reason predictions 4–6 are worth
  scoring rather than assuming.

## DESIGN-LOG

Appended 2026-08-10 — see the lines citing panel 024.

## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The author's instruction was a blanket one — *"ratifica anche tutto
quello che c'è da ratificare"* — given after reading the session summary, not a
clause-by-clause review of this file. It is recorded that way on purpose: this
project's own rule is that a record must not say more than what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here had been
load-bearing since the day it landed, so this changes the record's status rather
than the compiler's behaviour.

What it does **not** settle: anything this file keys to a measurement that has not
been taken. Those stay open on their own terms, listed in `docs/debrief/QUEUE.md`,
and a blanket yes cannot make a number arrive.
