# Panel 079 — the dot that two passes read differently

**Convened** 2026-08-16, M-selfhost-port, on `docs/debrief/DECIDE.md:335` and
`:343` — two diagnostics that name a repair the reader cannot make.
**Two seats reported before the sitting was stopped by the author** —
llm-ergonomist and spec-warden. The compiler-engineer, ffi-pragmatist and
historian were cancelled mid-flight; this file says so rather than presenting a
two-seat result as a full lane's.

**And the sitting found something neither item asked about**, by composing the
two seats' verdicts with a measurement: the checker and the lowering **disagree**
about what a dot means.

## What was asked

**Defect A.** `a.g(50)` on a matched case payload was
`error[unknown_function]: Step.apply has no field g` — about a payload that
declares `g` two lines above. The same shape on a record worked.

**Defect B.** Binding a C struct whose last member is a **flexible array member**
said *"the field disagrees — correct it"*, and no length would be correct.

## The finding neither item contained

The llm-ergonomist made a **collision clause** the condition of its approval: if
a value has both a field `g` and a top-level `g`, that must be a compile error
*"never a silent preference"* — because shape 1 would otherwise manufacture the
one silently-different program in the whole exercise.

The spec-warden, from a disjoint input, quoted the rule that orders it:
**design.md:1534** — *"The resolver, on seeing a dot, **first looks for a
field**; failing that, looks for a free function"* — and challenged the panel:
*"§4.11:1534 already orders that case, so show me the program."*

**The coordinator wrote the program, and the collision was already shipping —
resolving the wrong way.**

```
record Holder { tag: i64, g: (function(i64) -> i64) }
function g(h: Holder, n: i64) -> i64 …
s.g(50)   ->  50007        the free function
          ->  100          what the field would have answered
```

Exit 0, no diagnostic. And it has a second face: make the two result types
differ, and the **checker** types the expression from the field while the
**lowering** calls the function — so the only thing that notices is clang,
*"assigning to `int64_t` from incompatible type `HeroStr`"*, at **exit 2**. **A
type confusion the type system did not catch**, on a program the author is
entitled to write.

Located: `types/ufcs.rs` asks the field first, as design.md states and as its own
doc says *"is not an implementation detail"*. `ir/calls.rs` asked the
**resolver**, which answers `Ref::Top` whenever a free function of that name
exists, field or no field. One rule, two passes, two answers.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **spec-warden** | **approve shape 1 at +0 tokens** · approve B's rewording · **veto any spec sentence for B** | **Ruled defect A a §12 bug rather than an addition**, and quoted the two lines that entail it: design.md **§4.2:845** (*"a variant case **is** a small record, so it is written as one — no special case"*) and **§4.11:1534**. The refusal was one predicate in `types/construct.rs`, while `ir/layout.rs` had carried the `Ty::Case` arm since panel 068. Then ruled the whole of defect B free: a compile diagnostic is spec business only where a program can **read** it (`e.code`) or where the message **is** the feature (`???`) — *"this is neither"* — and the repair B must name is already at `spec:222`, so the message must **point** at it rather than the document restate it (+26 priced and refused). Measured the workaround at **7 tokens**, and said why that is not the cost: the message asserts a declared field is absent, so the reader's first repair edits a correct declaration |
| **llm-ergonomist** (spec-only, blind; contamination disclosed) | **approve shape 1** conditionally · **veto shape 2** · **veto shape 3** | *"The spec teaches what the compiler refuses"* — the document's own `match` example reads a field off a matched payload (`.num n => n.v`), so a reader has **more** textual warrant for the variant form than for the record form, and the variant form is what was refused. Vetoed keeping the refusal because legality would then depend on **how the value was bound** — neither on the line nor in the signature. Wrote three messages in the compiler's voice, and checked its own vocabulary against the document, rejecting *case*, *receiver*, *resolve*, *ambiguous*, *member*, *flexible*, *layout* as words the spec never uses. Asked that one question be **re-run on a fresh seat**, because it destroyed its own copy of the sitting's measurement |

## The resolution — provisional, author ratification pending

1. **Shape 1 lands, and it is a §12 bug fix rather than a language change**:
   `field_of_function_type` gains its `Ty::Case` arm, because design.md says a
   case payload is a record and `ir/layout.rs` already agreed.
2. **The precedence is repaired in the same commit**, because it is the same rule:
   the lowering asks the field first, as `types/ufcs.rs` and design.md:1534 both
   already do. **Swept before landing: 0 of 268 corpus files change meaning**, so
   the repair is free.
3. **Both halves of the rule now live in one file** — `ir/ufcs.rs`, cut on the
   seam `types/calls.rs` was cut on one pass earlier — so the two answers cannot
   drift apart again. That is the §11 obligation the defect earned.
4. **The collision clause is NOT adopted as a new refusal**, and the reason is
   what the measurement changed: the ergonomist asked for it as protection
   against a hazard shape 1 would *introduce*, and the hazard was **already
   there, resolving wrongly**. Repairing the precedence removes it. A record with
   both a field `g` and a function `g` is now unambiguous — the field wins, which
   is what design.md says — and the golden pins both readings side by side.
5. **Defect B: the wording is owed, at zero spec tokens**, and the emitter can
   carry it — `extern_record.rs`'s array assertion is a **tail measurement**, so
   the flexible-array shape is already structurally distinct where the message is
   recovered. Not landed here; it is a string, and the seat that would have
   written the C for it was cancelled.
6. **One question is owed to a fresh seat**: *given the spec and the two tasks,
   which form would you guess the language refuses?* The ergonomist's own copy is
   void by its disclosure, and it registered the prediction that fresh readers do
   **no better than 60 %** and skew toward the record.

**What a veto at ratification would compel**: the repair is one arm in the
checker and one lookup order in the lowering; reverting is a revert, and the
golden that pins it would go with it.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| spec-warden | `SPEC_TOKENS` is unchanged by defect A, and the new golden runs green while `check/ufcs-unknown-function.hero` is untouched | M-selfhost-probe |
| llm-ergonomist | fresh models given the spec plus the variant task write `a.g(50)` directly at **≥70 %** and were refused before this; under the repair the same programs compile. And **≥50 %** of models given the OLD message produce a still-wrong second program — re-declaring, renaming, or moving the field | M-program-corpus |
| llm-ergonomist | the voided question, re-run: fresh readers do **no better than 60 %** at predicting which form is refused, and skew toward the record. **At ≥80 % on the variant it withdraws its shape-2 veto** | M-program-corpus |

## Conditions on the record

- **spec-warden**: flips to object if the repair cannot be made without a spec
  sentence. It could — 3506 is unchanged by this sitting.
- **llm-ergonomist**: withdraws its shape-1 approval and demotes it to object if
  the collision is left as a **silent preference**. Resolution 4 answers that by
  removing the silence rather than by adding a refusal, which is the stronger
  form of what it asked for.

## What the missing three seats would have judged

The compiler-engineer was asked whether shape 1 creates an ambiguity, and the
answer arrived from the measurement instead: it inherits one that already ships.
The ffi-pragmatist held defect B — whether a flexible array member is one shape
or several, and how many real headers have one — and **its panel-071 condition is
therefore still undischarged**. The historian owed the field-versus-free-function
precedent, which Rust, Go, D and Nim have all taken positions on. Defect B waits
for those two; defect A did not need them.

## Author's verdict

*Pending.*
