# Panel 005 — Principle 0 formal adoption (§1.0)

2026-08-03 · five judges, differentiated inputs (`/panel`) · author's choice:
**A (recommended resolution)**, made before the session · status:
**DECIDED — ratified**.

## Proposal (verbatim, from OPEN-QUESTIONS 005)

> Ratify §1.0 as written; audit the closure list at the M6 checkpoint.

§1.0's content (already in design.md, adopted with the plan): the stopping
rule — v1 is done when the language compiles itself — and the burden of
proof — a form enters v1 if the compiler needs it (closure list) OR it
provably serves the thesis. The open half was the closure-list audit timing.

## Verdicts

| Judge | Verdict | Key finding | Cost | Flip condition |
|---|---|---|---|---|
| compiler-engineer | APPROVE | deferral to M6 is cheap insurance, not orphan risk: build steps 1–12 implement the seven core constructs needed for *any* v1; the genuinely cuttable items (file I/O, `args()`, modules, library tier) sit at steps 13–15, after the audit point. Demand: the audit must be **mechanical** — grep the appendix + one representative pass for forms actually used, not opinion | 0 lines; the audit is a grep + a DESIGN-LOG entry | any build step before M6 implementing a closure-list form with lowering/emitter cost solely because it is listed → audit moves to M4 |
| llm-ergonomist | ABSTAIN | no experiment possible: ratification changes no spec line a model reads; locality delta exactly zero | — | re-enters only if the audit adds/removes a spec form (as its own blind pair) |
| spec-warden | APPROVE (not provisional — 0 tokens) | ratifying its own mandate. Demand: the M6 audit must audit **spec coverage**, not just the list — modules, file I/O, `args()`, `exit` are on the closure list but absent from spec v0 (~40–80 est. tokens of mortgaged debt); the audit must arrive with named cuts, not just additions | 0 now; 40–80 est. tokens deferred to the modules/FFI amendments | the M6 audit scoped as list-only → OBJECT |
| ffi-pragmatist | APPROVE | `s005_selfhost_io.c`: the *entire* self-hosting FFI surface (read/write file via libc, `args()` from argv, `exit`) compiles today against the frozen `str` contract — an M6 audit before an M7 FFI cannot be invalidated by layout drift. Gap: closure-list I/O items have **no tier assignment** (§4.20 Tier 1 vs §1.11 Tier 2); the audit must pick one | none now; at M6, if Tier 1: ~4 declarations in `heroes_runtime.h` | a closure-list item needing a new *crossing* type not expressible as `ptr`/`cstr`/scalars (show the clang error) |
| historian | APPROVE (honesty clause) | **no verified precedent exists** for self-hosting as a feature freeze: Rust self-hosted 2011 then churned four years to 1.0; Go froze first (compat promise 2012), self-hosted later (1.5, 2015); Zig self-hosted mid-churn. Principle 0 inverts every verified precedent — deliberately. Pascal (self-hosted mid-1970) is the nearest ancestor in spirit | history's cost ran the other way: languages without a scope rule treated self-hosting as a starting gun | a verified language that declared self-hosting its completion criterion and held the line (none found — cite in DESIGN-LOG if ever found) |

## Disagreements

None on ratification; three riders attached to the M6 audit (all adopted, see
decision). The historian's honesty clause stands in the record: this is a
deliberate departure, not a precedented pattern.

## Decision (author, 2026-08-03)

**A adopted — §1.0 ratified.** The M6 closure-list audit is scoped with the
judges' riders: (1) mechanical — grep of the appendix plus a representative
compiler pass, not opinion; (2) covers **spec coverage**, arriving with named
cuts, not just named additions; (3) assigns a runtime tier to `file I/O`,
`args()`, `exit(code)`. Applied to design.md §1.0. No spec text.

Side finding entered into the record: CLAUDE.md rule 6's "Nim's compiler is
~150k lines" could not be verified by the historian — demoted to an explicit
assumption in CLAUDE.md.

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| compiler-engineer | every form removed at the M6 audit has zero lines in lowering/emitter at that commit; the list changes by ≤3 items | M6 |
| spec-warden | the post-modules spec (v2) cannot pass 1500 without cutting ≥40 est. tokens of v1 text | modules-milestone panel |
| ffi-pragmatist | zero edits to the frozen header field lists (`HeroArrayHeader` 4 fields, `HeroStr` 3) between M5c and M8c; the audit adds declarations only | M6 vs M8c diff of `heroes_runtime.h` |
| historian | post-fixpoint scope pressure exceeds pre-fixpoint: within two milestones after M8c, more add-a-form proposals arrive than in all of M0–M8 | M8c + 2 milestones |
