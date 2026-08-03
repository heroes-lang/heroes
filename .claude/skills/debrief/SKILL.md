---
name: debrief
description: Process the comprehension queue (docs/debrief/QUEUE.md) with the author — retrieval-first walkthroughs, failure diagnosis, golden and panel ratifications, drills. The author decides when and how much; nothing here ever blocks development.
---

# /debrief [n] — comprehension, on the author's clock

Development never waits for understanding; this is where understanding
catches up. Conversation in Italian, using the `/where` canonical analogies;
artifacts written here (glossary entries, queue updates) in English.

## Procedure

1. Read `docs/debrief/QUEUE.md`. Group open items by milestone; propose a
   session of 3–7 items, oldest first unless the author picks.
2. Per item, **retrieval before explanation** — the question comes first,
   the walkthrough after:
   - **Concept** — show the input, ask the closed question(s), then walk
     through what was built (before → after on a real repo example).
   - **Failure** — show the raw symptom only; the author hypothesises in one
     sentence (free-form or offered options); then show the fixing commit
     and explain the gap.
   - **Ratification** — golden cases: the author says briefly what each
     guards against → drop its `# UNVERIFIED — pending debrief` marker.
     Panel provisional defaults: the author confirms or overturns → append
     the verdict to the panel file (+ DESIGN-LOG line and follow-up work if
     overturned).
   - **Drill** (milestone offers, on request) — mutation drill: inject one
     bug into committed compiler code, the author diagnoses from the failing
     golden alone. Exit-quiz: the author re-implements one small function on
     a throwaway branch and diffs it.
3. Any gap that needed a genuinely new explanation → distill it into
   `docs/glossary/NNN-<concept>.md` (numbered in birth order, English,
   canonical analogies, origin cited, never deleted) and update the
   glossary index.
4. Tick items off in the queue (leave them checked — the queue is also the
   record). Commit: `debrief: <what was covered>`.

Lessons stay impersonal — shapes and rules, never scores; personal
performance is never written anywhere.
