---
name: step
description: Run one development step of the Heroes compiler autonomously — implement, test, queue comprehension items for /debrief, commit. Chains steps without stopping; use for every implementation step, including in unattended loops.
---

# /step [<milestone.step>] — the development protocol

The assistant implements; the author's learning is real but **post-hoc**: it
happens in `/debrief`, on the author's clock, fed by the queue this skill
writes. Never stop mid-step to ask the author anything. (Learn-first mode —
closed questions before implementing — only when the author explicitly asks
for it before a step; no other ceremony attaches to it.)

## 1. Orient
`git log --oneline -10` · DESIGN-LOG tail · `docs/ROADMAP.md` status · read
`spec/heroes-spec.md` in full. If no step was named, take the next one from
the ROADMAP.

## 2. Implement
- If the step touches a panel path (CLAUDE.md rule 4), run `/panel` first —
  asynchronous: adopt the conservative default, queue the ratification.
- Write the code and the golden cases. Mark the milestone's 5 adversarial
  cases `# UNVERIFIED — pending debrief`; label bulk regression cases.
- Run `cargo test` and `cargo clippy`; from M5a also the double-emit
  determinism diff and the ASan `run/` goldens.

## 3. On failure
Diagnose and fix autonomously. Record symptom → cause → fix in the milestone
journal, and queue the **raw symptom only** (golden diff, clang error, panic)
plus the fixing commit's hash — so the author can hypothesise in debrief
before reading the fix.

## 4. Queue comprehension — per new concept, not per step
When a step introduces a new concept (first tokens, first tree, first types,
first blocks, first C…), append 2–4 closed-form items (a count, a choice
among structures, an output value) to `docs/debrief/QUEUE.md`:
`- [ ] <origin> | <question / task> | <where to look> | <why it matters>`
Plumbing steps — CLI, harness, refactors, bulk cases — add nothing.

## 5. Close
Every step ends with a commit: `M<n> step <k>: <what>`.

Milestone close — the checklist (this is its only copy):
- goldens pass (ASan-clean where applicable); determinism diff empty (M5a+);
- journal `docs/journal/NNN-<slug>.md`, 3 sections: **goal** · **what
  surprised** (impersonal — shapes and rules, never scores) · **what broke
  and why**;
- one story beat line in `docs/book/beats.md`;
- a DESIGN-LOG line per decision made;
- queue the milestone's debrief offers: walkthrough, golden ratification,
  mutation drill, exit-quiz (all optional, author's call);
- update `docs/ROADMAP.md` status; tag `m<n>`, push `--follow-tags`;
- site build log: only when the author asks (`site/README.md`).
