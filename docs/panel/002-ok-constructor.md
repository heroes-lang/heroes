# Panel 002 — `ok(x)` constructor and `fail`'s typing

2026-08-03 · five judges, differentiated inputs (`/panel`) · author's choice:
**A (recommended resolution)**, made before the session; judges instructed to
record real objections, not staged dissent · status: **DECIDED — adopted**.

## Proposal (verbatim, from OPEN-QUESTIONS 002)

> Add `ok(x)` as `fail`'s symmetric twin; both `ok` and `fail` are checked
> *against the expected type* (bidirectional ⇐ mode). ~+8 spec tokens, zero
> new vocabulary (`ok`/`err` arm names already exist). Avoids the `T??`
> ambiguity implicit promotion would create under monomorphisation. The
> acceptance program changes at the six sites.

Rejected alternative: implicit `T` → `T?` promotion (Part 6 rejects implicit
conversions permanently).

## Verdicts

| Judge | Verdict | Key finding | Cost | Flip condition |
|---|---|---|---|---|
| compiler-engineer | APPROVE | ⇐ machinery is already owed to §4.5 (empty literals, leading-dot variants, `return`, `???`) — `ok`/`fail` are two more check-mode cases; sugar, not core | ~40–60 lines confined to future `types/`; zero in lowering/emitter | an `ok` node surviving past the frontend, or >150 lines |
| llm-ergonomist (blind A/B) | APPROVE | wrote `parse_int` under both variants: explicit `ok` never required a guess; under implicit promotion the `T??` lift level produced two silently different programs that both type-check — would have VETOED the implicit variant | one token per success return; transient loud `Ok`/bare-return errors with certain fixes | first-try compile-success drops >20 pts on plain fallible tasks |
| spec-warden | APPROVE (provisional) | measured +12 words ≈ +16 est. tokens; burden of proof met (compiler-need: six+ appendix sites) | part of v1 package: 1559 est. tokens, over budget **unless** the 53-word spec header comment is cut in the same commit (→ ~1492) | measured v1 (real tokenizer) > 1500 without a compensating cut → VETO |
| ffi-pragmatist | APPROVE | `s002_fallible.c`: hand-lowered `ok(x)` and the implicit alternative emit **byte-identical C** — the rule is frontend-only; no ABI surface | zero runtime/binding cost; one `ok(...)` per FFI wrapper success path | `ok(x)` forced to allocate/box (layout diverging from `T`) → VETO |
| historian | APPROVE | Rust: explicit `Ok`/`Err` since 1.0 (2015-05-15), Ok-wrapping refused twice (RFC 2107, try-fn pre-RFC). Swift's implicit promotion is the live counter-precedent: SE-0230 (accepted 2018-10) had to flatten nested optionals five versions in | implicit path cost Swift a compatibility-breaking patch; explicit path cost Rust only keystrokes | a language that shipped implicit promotion and kept it without flattening patches (none found) |

## Disagreements

None on substance. The warden's approval is provisional and package-scoped
(see budget condition above, shared by 003/006).

## Decision (author, 2026-08-03)

**A adopted.** `ok(x)` is `fail`'s symmetric twin; both are checked against
the expected type. Applied to design.md §4.6 and to the appendix acceptance
program — **eleven** sites, not six: the six originally marked plus five more
found when applying the rule exhaustively (`term`/`expression`'s variant
returns, `sum_of`/`product_of`'s `return tot`, `evaluate`'s `.num` arm).
Spec v0 stays frozen; the v1 amendment text is recorded here:

> replace "Construct an error with `fail(code, msg)`;" with "Construct a
> success with `ok(x)`, an error with `fail(code, msg)`; both take their
> type from the expected `T?`."

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | `ok`/`fail` handling ≤60 lines confined to `types/`; zero hits in lowering/emitter beyond the generic variant path | M4 |
| llm-ergonomist | on a nested-fallible (`T??`) task, implicit-variant completions err on the level ≥30% of the time; explicit-variant: 0% (all such confusions are compile errors) | Part-11 harness run with a nested-fallible task |
| spec-warden | ≥1 in 5 baseline tasks touching `T?` produces a `T`-typed expression where `T?` is expected under v0 | pre-amendment baseline run |
| ffi-pragmatist | SQLite binding wrappers need exactly one `ok(...)` per success path, zero shim lines attributable to fallible construction | M7 |
| historian | Heroes never needs a flattening or promotion-depth rule; no `T??` diagnostic/spec clause ever proposed | M8c |
