# Panel 000 — adversarial review of the bootstrap plan (revision 1 → revision 2)

Date: 2026-08-03. Five independent reviewers, differentiated by lens, each
instructed to find the strongest reason the plan was wrong. (This file is the
consolidated record; the five full reports were collapsed into it on
2026-08-03 — process simplification — and live in git history.)

| Reviewer | Verdict | Single most important change demanded |
|---|---|---|
| Language design | **object** | Replace implicit `T`→`T?` promotion with explicit `ok(x)`; drop Principle 0's destructive second question |
| Compiler engineering | approve-with-changes | Add the ownership + type-descriptor passes between lowering and emission; split M5 around them |
| Simplicity | **object** | Cut ceremony; first milestone must end with tokens on screen, not a directory tree |
| LLM ergonomics | **object** | Land spec + harness in M0 with a `--permissive` control arm; grade on tests passing; take the pre-amendment baseline |
| Didactics | approve-with-changes | Make the author's prediction uncued and falsifiable (separate file, committed first) |

## What each review found (distilled; every fix was adopted in revision 2)

**Language design (object).** Principle 0's second question ("can the
compiler be written without it? → reject") would have deleted the thesis
features (`???`, same-typed-argument rule, rich errors, the `_` ban) —
dropped; self-hosting is necessary-not-sufficient. Implicit `T`→`T?`
promotion reopened Part 6's permanent rejection of implicit conversions and
was needed at six sites, not two; the root bug was `fail`'s uninferable type
→ `ok(x)`/`fail` both checked against the expected type (decided, panel 002).
`discard` cut in favour of the statement-position rule (panel 003). Closure
list gaps found: `if`/`else`, map iteration order, `sort`, `join`/`Builder`,
`()` as a type, `print`'s variadic contract, error accumulation via
`@diags: [Diagnostic]`. Plus five design.md inconsistencies fed to the
errata/panel queue.

**Compiler engineering (approve-with-changes).** Revision 1 had no
implementation for §4.3/§4.10 (structural `==`, value-semantics copy/drop):
→ the type-descriptor pass (`h_T_copy/drop/eq/hash` per reachable type),
container ABI decided in M0 by hand-written spike 04. Refcounting was placed
"in the runtime", but the runtime cannot know where a scope ends → the
ownership pass in lowering, visible `incref/decref/cow_check` in `--dump-ir`,
cleanup-label chains on every exit edge, ASan goldens before arrays.
Fixpoint corrected: three compilations, `diff B.c C.c` on generated C.
Decl-ordering pass + namespaced mangler added; "-O2 exploits UB" answered
with the `-Werror` set + `hero_unreachable()`. On QBE→C: the switch stands,
but on §1.11/§4.19 grounds only; QBE scheduled post-fixpoint.

**Simplicity (object).** "25 written verdicts before a single token is
lexed"; six records of one event; the ceiling is the author's *attention*.
Adopted: journal 8→5 sections, skills 3→2, golden dirs 5→2, ROADMAP deleted,
theory notes on-miss, mutation metric mechanised, tooling over prose for the
Cyclone rule, M0 compressed to one day. **The abandonment forecast** (kept
because it keeps scoring): P(abandoned in 1 month) — 8-section journal 0.9 ·
5-judge panel 0.85 · Cyclone-as-prose 0.85 · 15 theory notes 0.8 · CLAUDE.md
13 sections 0.75 · model-in-the-loop metrics 0.75 · C goldens 0.7 · ROADMAP
checkboxes 0.7 · DESIGN-LOG 0.2 · golden check/run 0.1. Scorecard so far:
the prediction protocol was rewritten three times in one day and retired on
day two (2026-08-03, process inversion); CLAUDE.md was cut to pointers the
same day. The reviewer's one keep — `tests/golden/` with the UPDATE_GOLDEN
discipline — is CLAUDE.md rule 9.

**LLM ergonomics (object).** The thesis metric couldn't test the thesis: no
comparison arm, graded on "passes the checker" → `heroes check --permissive`
as the within-subject control, tests-pass grading from M6, spec v0 + baseline
in M0 (the pre-amendment number is unrecoverable). Panel verdicts carried no
information (correlated hats, ~100% objection base rate, confabulated
numbers) → differentiated *inputs*: the ergonomist reads only the spec, the
engineer must cite files, the warden gets measured counts, the historian gets
web access or is cut; every verdict carries a falsifiable prediction, scored.
Mandatory fixes were dangerous → `Fix` tagged `certain | guess`. `???` vs
unused-variable collision → holes suppress unused-binding errors; suggestions
capped at 5, deterministic. Measurement methodology fixed (20×5 Wilson,
frozen hashed prompts, provenance stamps, non-Anthropic robustness check).

**Didactics (approve-with-changes).** The prediction was cued (recognition,
not recall) → uncued, separate file, committed first. Nobody made the author
produce anything → one author-written function per milestone + author-first
diagnosis. M3 was four lessons and the cliff → M3a–d. Weeks-wide spike loops
→ same-day hand-written expected outputs. Golden rubber-stamping → 5
adversarial author cases per milestone. Three prerequisite concepts named
(bidirectional ⇐/⇒ before M3, core-vs-sugar before M4, basic blocks in M0).
*(2026-08-03, process inversion: the prediction gate and author-first stops
were later retired; the retrieval-first principle survives inside `/debrief`,
and the prerequisite exercises live in `docs/debrief/QUEUE.md`.)*

## Disposition

All five verdicts were folded into **revision 2** of the plan (approved by
the author 2026-08-03), with two rejections recorded:

- **Cutting the mandatory panel** (simplicity reviewer): REJECTED — author's
  explicit decision. Adopted instead: the LLM-ergonomics redesign
  (differentiated inputs, falsifiable predictions, path-based triggers) and a
  large ceremony reduction.
- **Cutting the LSP** (simplicity + didactics reviewers): REJECTED — author's
  explicit decision. Moved off the critical path; blocks nothing.

The plan's milestone chain now lives in-repo at `docs/ROADMAP.md`
(reintroduced 2026-08-03 for autonomous runs, reversing revision 2's
"ROADMAP deleted" bookkeeping); its content is otherwise reflected in
CLAUDE.md, the skills, and this docs tree.
