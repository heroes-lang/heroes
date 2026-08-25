---
name: step
description: Run one development step of the Heroes compiler autonomously — implement, test, queue what the author owes nobody (/learn) and what the compiler is waiting on (/decide), commit. Chains steps without stopping; use for every implementation step, including in unattended loops.
---

# /step [<milestone> <step>] — the development protocol

The assistant implements; the author's learning is real but **post-hoc**: it
happens in `/learn`, on the author's clock, fed by the queue this skill
writes. Never stop mid-step to ask the author anything. (Learn-first mode —
closed questions before implementing — only when the author explicitly asks
for it before a step; no other ceremony attaches to it.)

## 1. Orient
`git log --oneline -10` · DESIGN-LOG tail · `docs/ROADMAP.md` status · read
`spec/heroes-spec.md` in full. If no step was named, take the next one from
the ROADMAP.

**Opening a milestone also means reading `docs/work/SCHEDULED.md` for the
items that name it.** That list is work with a home — a measurement to run, a
paragraph owed, a row to price — put aside precisely because doing it earlier
would do it against a smaller corpus or a compiler that had not grown into the
question yet. There is no `/scheduled` skill and there should not be one: a third
verb that reads a list `/step` already has to open answers no question the two
existing ones do not (CLAUDE.md §10's stopping rule, applied to the skills). What
the list needs is not a caller of its own, it is *this line* — without it the
file is write-only, which is the failure mode of every list nobody is obliged to
open.

**Every item that gets done in the step is ticked with what closed it and MOVED
to `docs/work/DONE.md` before the commit.** A live list holds open items only;
the record holds everything else. That rule existed in three documents and in no
skill until 2026-08-26, and by then `docs/work/DECIDE.md` held 138 ticked items
and zero open ones while the record had not been written to in eight days. The
same applies to an item this step *finds* rather than finishes: it is filed as
`- [ ]` in the list that matches what it asks — `SCHEDULED.md` if it names a
milestone, `DECIDE.md` if it names a default the compiler is running on,
`docs/learn/LEARN.md` if it only asks what is true — and **never as a bare
bullet or a `## ` section**, which is a notation no count in this project can
see.

## 2. Implement
- If the step touches a panel path (CLAUDE.md § Panel), run `/panel` first —
  asynchronous: adopt the conservative default, queue the ratification.
- Write the code and the golden cases. Mark the milestone's 5 adversarial
  cases `# UNVERIFIED — pending debrief`; label bulk regression cases.
- Run the tests, which since **M-bootstrap-archive** (2026-08-19) means the two
  the one command gives: `heroes test selfhost/main.hero` for the compiler's own,
  and `heroes run tests/harness/main.hero -- <compiler>` for the net — the second
  one carries the double-emit determinism diff, the ASan `run/` goldens, the
  blessed emissions and §11's file ceiling inside it. `cargo test` and `cargo
  clippy` were this line until the archive; the Rust they ran is
  `archive/bootstrap-rs/` and nothing builds it. Naming a suite runs only that
  one, which is what makes a fast loop possible: `-- <compiler> records`.

## 3. On failure
Diagnose and fix autonomously. Record symptom → cause → fix in the milestone
journal, and queue the **raw symptom only** (golden diff, clang error, panic)
plus the fixing commit's hash — so the author can hypothesise in `/learn`
before reading the fix.

## 4. Queue comprehension — per new concept, not per step
When a step introduces a new concept (first tokens, first tree, first types,
first blocks, first C…), append 2–4 closed-form items (a count, a choice
among structures, an output value) to `docs/learn/LEARN.md`:
`- [ ] <origin> | <question / task> | <where to look> | <why it matters>`
Plumbing steps — CLI, harness, refactors, bulk cases — add nothing.

## 5. Close
Every step ends with a commit: `M-<name> step <k>: <what>`. The naming algorithm
for `<name>` is CLAUDE.md §14 — its only home.

Milestone close — the checklist (this is its only copy):
- goldens pass (ASan-clean where applicable); determinism diff empty (from M-scalars-run on);
- journal `docs/journal/NNN-<slug>.md`, 3 sections: **goal** · **what
  surprised** (impersonal — shapes and rules, never scores) · **what broke
  and why**;
- one story beat line in `docs/book/beats.md`;
- a DESIGN-LOG line per decision made;
- **score every prediction whose milestone this is**, and lapse the ones you
  cannot: `grep -n "$(git describe --tags --abbrev=0)\|<this milestone>" docs/work/SCHEDULED.md
  docs/measurements/010-spec-budget-ledger.md docs/panel/*.md`. (It named
  `crates/heroes/src/measure/gate.rs` until 2026-08-23 — doubly dead: the tree is
  archived, and panel 086 moved the ledger's 38 rows out of that doc comment into
  the record above, where `tests/harness/suite_spec.hero` locks them to
  `SPEC_TOKENS`.) A prediction is scored, or
  it is marked `lapsed` in the ledger row that spent it and the clause it bought
  goes back to `docs/work/DECIDE.md` to be re-argued under the removal branch — **never
  renewed with a new milestone name** (panel 046 R2). This bullet exists because
  the mechanism failed without it: panel 036's *"Score at M-ffi-ladder close"*
  survived that milestone's close untouched;
- queue the milestone's `/learn` offers: walkthrough, golden ratification,
  mutation drill, exit-quiz (all optional, author's call);
- append the closing block — the status paragraph and the milestone's chain entry —
  to `docs/journal/NNN-<slug>.md` § *What landed, and what carried forward*, and
  leave `docs/ROADMAP.md` § Status at **≤15 lines**. The ROADMAP says what is
  *next*; a closed milestone's record is its journal. (It reached 935 lines before
  this rule existed, growing ~66 per close, and the reader met 512 lines about the
  past before the first line about the future.)
- update `docs/ROADMAP.md` § Status and § The order; tag `m-<name>`, push
  `--follow-tags`;
- site build log: only when the author asks (`site/README.md`).
