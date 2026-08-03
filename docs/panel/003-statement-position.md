# Panel 003 — statement-position rule

2026-08-03 · five judges, differentiated inputs (`/panel`) · author's choice:
**A (recommended resolution)**, made before the session · status:
**DECIDED — adopted**.

## Proposal (verbatim, from OPEN-QUESTIONS 003)

> A non-`()` expression in statement position is a compile error; the
> diagnostic dictates the fix (`_ = expr`, or use the value). Zero keywords,
> ~+8 spec tokens; subsumes Nim's `discard`.

Problem: `xs.push(4)` as a statement compiles and silently does nothing
useful (`push` is pure under value semantics, §4.10) — a plausible silent
error in exactly the class the language exists to kill.

## Verdicts

| Judge | Verdict | Key finding | Cost | Flip condition |
|---|---|---|---|---|
| compiler-engineer | APPROVE | lives in the **typer**, not the parser ("non-`()`" is a type judgment); cheapest form: ⇐-check every expression statement against `()` — literally reusing panel 002's machinery; UFCS is a non-issue (already erased by the resolver) | ~20–30 lines in future `types/` + 1 diagnostic with a `certain` fix | turns-to-green data shows `@`-mutating functions that also return values forcing `_ =` noise that measurably lowers first-try rate |
| llm-ergonomist (blind A/B) | APPROVE | wrote the growth code the way a model writes it first (bare `xs.push(4)`): silent no-op printing `0` under the permissive variant, local compile error with dictated fix under the rule — the single highest-frequency LLM habit error under value semantics, converted to a machine-checkable diagnostic | `_ =` on rare legitimate discards; nothing becomes non-local | legitimate-discard sites outnumber silent no-ops and models fail the dictated fix >20% on retry |
| spec-warden | APPROVE (provisional) | +18 words ≈ +24 est. tokens; compiler-need not claimed — the §1.2 thesis derivation is, and accepted: one prevented silent bug outweighs 10× the token cost. Flag: the rule pivots on `()`, which the spec never defines | +24 est. tokens (v1 package; header cut required, see 002) | baseline shows models writing `_ =` reflexively everywhere (rule overfiring); measured budget breach → VETO |
| ffi-pragmatist | APPROVE — **no extern relaxation** | counted real headers: sqlite3.h 152/287 functions return ignorable `int`; but an ignored C return code is C's own classic *silent* bug — relaxing at the boundary would delete the error exactly where §1.11 needs it. Lowering is free: `_ = e` → `(void)e;` (compiled, warning-proof) | emitter lowers `_ =` to `(void)`; diagnostic ships machine-applicable fix | a real binding program where >25% of call statements need `_ =` → then a per-`extern` "discardable" mark in §4.19's annotation vocabulary, never a blanket relaxation |
| historian | APPROVE | hard error is precedented and held: Nim `discard` (static error), Zig `_ =` (kept; only the wording was debated). Go's middle way (calls exempt) spawned the errcheck tool class; Swift shipped silent discards then paid the SE-0047 migration in Swift 3 | Go's exemption cost an ecosystem of external vetting tools | Nim or Zig formally relaxing the error to a warning (found the opposite) |

## Disagreements

None. The ffi-pragmatist pre-answers the likely future pressure point
(C functions with ignorable returns) with a named escape valve — per-`extern`
annotation at M7 if the measured `_ =` rate crosses 25% — so the rule itself
stays intact.

## Decision (author, 2026-08-03)

**A adopted.** Applied to design.md §4.14. Implementation note recorded for
M4: implement as a ⇐-check against `()`, sharing panel 002's expected-type
machinery. Watch item: if the baseline shows confusion about `()`, the spec
type table owes a `()` row. Spec v1 amendment text:

> add "An expression statement whose value is not `()` is a compile error;
> write `_ = expr` to discard."

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | a `discard-result` mutation operator kills at 100% after M4 (0% under `--permissive`) | first mutation drill after M4 |
| llm-ergonomist | ≥50% of first-try array-building completions contain a bare `xs.push(v)`; silent-wrong-output rate drops ≥40 pts vs the permissive variant | next milestone-close harness run |
| spec-warden | ≥1 of the 5 draft baseline tasks yields a bare non-`()` statement that v0 compiles silently | pre-amendment baseline run |
| ffi-pragmatist | the §4.19 SQLite acceptance program needs ≤4 `_ =` markers; the raylib frame loop needs 0 | M7 |
| historian | the Heroes compiler's own source uses the `_ =` escape hatch before the closure compiles (a hard error with no escape never reaches fixpoint) | M8c |
