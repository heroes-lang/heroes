# Panel 000 — simplicity review (verdict: OBJECT)

## The abandonment ranking

| Artifact (revision 1) | P(abandoned in 1 month) | Cheapest surviving version |
|---|---|---|
| 8-section journal per step | 0.9 | Prediction / what-happened, ~10 lines |
| 5-judge panel with 7 structured fields | 0.85 | One adversarial pass with the five questions |
| Cyclone rule as prose + PORT-DEBT ledger | 0.85 | One line + tooling |
| 15 theory notes | 0.8 | Write only when a prediction misses (~4–5) |
| CLAUDE.md, 13 sections | 0.75 | One page |
| Model-in-the-loop metrics 2 & 4 | 0.75 | Metric 1 as a gate; 2 as a one-off experiment |
| `tests/golden/c/` | 0.7 | Delete (churn without signal) |
| ROADMAP checkboxes | 0.7 | Tags already say where you are |
| DESIGN-LOG | 0.2 | Keep as-is |
| Golden `check/` + `run/` | 0.1 | Keep — Part 0 calls them the single most important artifact |

## Core objections

- **25 written verdicts before a single token is lexed** (5 judges × 5 M0
  sessions); a veto that "compels a written answer" is advisory paperwork;
  five agents reading the same document with the same prompt produce
  correlated verdicts — a rubber stamp with a table.
- Six records of one event (journal, DESIGN-LOG, ROADMAP, panel file, commit,
  tag) — keep two (DESIGN-LOG line + commit body).
- The plan violated its own §1.1: the ceiling is the author's *attention*.
- M0 should end with something running, not a directory tree.

## Disposition in revision 2

**Adopted:** journal 8→5 sections; skills 3→2; golden dirs 5→2 (no C goldens,
no lex/parse golden dirs — crate-internal snapshots instead); ROADMAP deleted;
theory notes on-miss (~8 expected, 3 prerequisite); mutation metric mechanised
(no API); tooling replaces prose for the Cyclone rule; M0 compressed to one
day and every spike runs the same day.

**Rejected (author's explicit decisions):** the mandatory panel stays — but
redesigned per the LLM-ergonomics review so sessions are cheaper and verdicts
informative; the LSP stays — off the critical path.

**The one thing to keep**, per this review: `tests/golden/` with the
UPDATE_GOLDEN discipline — the only artifact that fails loudly when neglected.
Kept, with the discipline written into CLAUDE.md rules 9–10.
