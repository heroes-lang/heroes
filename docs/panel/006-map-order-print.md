# Panel 006 — map iteration order and `print`'s contract

2026-08-03 · five judges, differentiated inputs (`/panel`) · author's choice:
**A (recommended resolution)**, made before the session · status:
**DECIDED — adopted, with the contract completed per the warden's condition**.

## Proposal (verbatim, from OPEN-QUESTIONS 006)

> (a) Insertion order, fixed hash seed. (b) `print` is a compiler-known form
> (not a user-callable variadic function) accepting a comma-separated list of
> `str`/`int`/`f64`/`bool` values, each rendered by its canonical `.str()`;
> no other variadics exist (§4.9 stays true).

## Verdicts

| Judge | Verdict | Key finding | Cost | Flip condition |
|---|---|---|---|---|
| compiler-engineer | APPROVE | (a) cheapest deterministic map in C: dense entries array (which *is* the iteration order) + sparse index; reuses spike 04's header pattern; v1 needs no `remove` → no tombstones; `HeroDesc` gains the `hash` member §4.20 already promises (additive). Fixed seed costs zero lines — with insertion-order iteration the seed is unobservable. (b) files under core construct #7 (primitive ops), not an eighth construct. **Trap named**: the Rust bootstrap iterates `BTreeMap` (sorted) while Heroes maps iterate by insertion — every ordering-sensitive map walk in the Rust compiler must become an explicit sort in the Heroes port or the M8 fixpoint diff breaks; mark those sites as they are written | (a) ~120–200 runtime lines + 15 header lines + 1 descriptor pointer; (b) ~25+25+25 lines typer/emitter/runtime; ≲300 total | (a) self-hosting turns out to need map deletion (runtime >~400 lines); (b) print's typer case >~150 lines |
| llm-ergonomist (blind A/B) | APPROVE | (a) models' spec-silent default IS insertion order (Python/JS prior) — the guarantee matches the prior; sorted order punishes it silently. (b) multi-type print: one completion, done; str-only variant produced three plausible wrong completions, one of which **compiles and prints `{n}` literally**. Two residual gaps flagged: re-inserted key position; argument separator | no locality change (a); print line locally decidable (b) | the mutation drill showing models predominantly assume sorted order; the unstated separator causing more silent diffs than the str-only variant's failures |
| spec-warden | **OBJECT → resolved** | (a) approved as drafted: +7 words ≈ +9 est. tokens to close an explicitly-listed compiler need — cheapest amendment on the table. (b) objection: "rendered canonically" specified **neither separator nor terminator** — 15 tokens spent leaving `print`'s observable output underdetermined, the exact ambiguity goldens and the determinism diff trip on. Stated flip condition: specify both → APPROVE. **The decision below does so; the objection is resolved on its own terms** | (a)+9, (b)+15 (+~4 for the fix) est. tokens; v1 package requires the header cut (see 002) | measured budget breach without a cut → VETO |
| ffi-pragmatist | APPROVE | `s006_map.c` compiles ASan-clean and iterates in insertion order with a **new** map header — spike 04's array header untouched, no ABI break; descriptors are regenerated every compile and never cross the FFI. `s006_print.c`: the contract implies segment printers — `hero_print_str/int/f64/bool` + `hero_print_end`; today's `hero_print_int` owns the newline (`"%lld\n"`) and must lose it. **Flag**: `%.17g` is locale-sensitive and ugly (`0.10000000000000001`); canonical `f64` rendering must be locale-independent and deterministic, decided with this session, or fixpoint and goldens are at risk | 4 print functions + newline relocation; `HeroDesc` +1 field; map header + put/get/iter with fixed FNV seed | a *shared* container header change (rewriting spike 04's frozen shape); f64 rendering requiring a per-platform shim |
| historian | APPROVE | (a) Python dict: 3.6 impl detail → **3.7 language guarantee** (Dec 2017); JS `Map` insertion-order since ES2015; Go's randomization condemns *unspecified* order, not *guaranteed* order (Go randomized because code ossified around accidentally-stable unspecified order); 8+ years of Python/JS guarantee with no ossification record. (b) Pascal's `WriteLn` is the 50-year precedent: compiler magic, impossible to write in Pascal itself; Go's `print`/`println` hedge ("bootstrapping; not guaranteed to stay") is the proven wording template | Go's unspecified-but-stable middle path is the only verified disaster | (a) a verified case of a guaranteed insertion-order map causing Go-style ossification; (b) Go actually removing print/println would strengthen the hedge, not flip |

## Disagreements

One real objection (spec-warden, on (b)'s wording), independently confirmed
by the ergonomist's separator gap and the ffi-pragmatist's terminator finding
— three judges, three different inputs, same hole. Resolved by completing the
contract in the decision (below), which is the warden's stated APPROVE
condition.

## Decision (author, 2026-08-03; contract completed per the judges' conditions)

**A adopted, completed as follows:**

(a) Map iteration follows **insertion order**. Overwriting an existing key
**keeps its position** (the Python-prior answer to the ergonomist's gap).
The hash seed is fixed — with insertion-order iteration it is unobservable;
the implementation simply never randomizes.

(b) `print` is a **compiler-known form**, not a function value: it accepts a
comma-separated list of `str`/`int`/`f64`/`bool` values, renders each by its
canonical `.str()`, writes **no separator between values and exactly one
trailing newline**. Canonical `f64` rendering is deterministic and
locale-independent (exact algorithm fixed at M5b with its goldens). No other
variadics exist.

Engineering rider (compiler-engineer): ordering-sensitive `BTreeMap` walks
in the Rust bootstrap are marked at the write site (`// ORDER:` comment) so
the Heroes port makes each an explicit `sort` — the M8 fixpoint depends on it.

Spec v1 amendment texts:
> add "Iteration over a map follows insertion order; overwriting a key keeps
> its position."
> replace "`print(...)`" in Built-ins with "`print` is a compiler form:
> comma-separated `str`/`int`/`f64`/`bool` values, each rendered canonically,
> no separator, one trailing newline."

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | `runtime/runtime.c` ≤500 lines with the map included; determinism diff empty with map iteration in a golden; at M8 every ordering-sensitive Rust site carries a countable marker and `diff B.c C.c` is empty | M5c, M8 |
| llm-ergonomist | with the order sentence deleted from the prompt (mutation drill), ≥70% of completions assume insertion order; str-only print would have produced ≥3 wrong first completions (≥1 compiling), multi-type ≤1 class (all loud) | next Part-11 run |
| spec-warden | had (b) landed without separator/terminator, ≥1 of the first 10 print goldens would mismatch on them | first milestone where print executes (moot if the completed contract holds) |
| ffi-pragmatist | fixpoint diff empty with maps in the compiler; array header still exactly the four spike-04 fields | M8c |
| historian | the determinism test never fails due to map order; by v1 the spec carries a Go-style hedge on print formatting | continuous; spec freeze |
