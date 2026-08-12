---
name: decide
description: Settle the open DECISIONS the compiler is waiting on, from the decision list (docs/debrief/DECIDE.md) — fast, no teaching. Verify each item against the repository before asking; a settled question put to the author is the one cost this skill cannot pay. Apply every answer in the same session.
---

# /decide [n] — the decisions, and nothing else

Split from `/learn` by author instruction (2026-08-12), and named after the list
it reads — `docs/debrief/DECIDE.md` — because a skill whose name does not match
its list is one more thing to remember. The queue held two kinds of item with
**opposite clocks**: a decision left open goes on shaping
the code by default, an explanation left unread costs nothing until it is wanted.
This takes the first kind only.

The whole session is: **what is still open, what does it block, what is the
answer.** No retrieval, no walkthrough, no drill, no glossary. If the author asks
"why is it like that", answer in two sentences and move on — the long version is
`/learn`'s.

## Procedure

1. **Read `docs/debrief/DECIDE.md`.** The triage is already done — the queue was
   split into three lists on 2026-08-12 — so this step is only checking that
   nothing arrived in the wrong one. An item that asks *what is true* belongs in
   `LEARN.md` however interesting, and one that names the milestone which will do
   it belongs in `SCHEDULED.md`; move it rather than answering it.

2. **Verify before asking. This is the rule the skill exists for.** Run the
   thing. A queue entry is a claim from the day it was written, and entries
   outlive their causes: the map that "cannot be added to" grows, the `%` sign
   the spec now states, the error code the spec now names — four such were found
   in the first session. **Asking the author about a settled question is the one
   cost this skill cannot pay**, so every candidate is checked against the
   repository first and the stale ones are ticked with *what closed them*, not
   asked.

3. **Rank by what it blocks**, never by age. A decision the next milestone reads
   outranks one nothing reads yet, and the blocker goes in the question.

4. **Ask the whole batch at once, in plain text.** The measurement or the
   snippet visible, options lettered, one reply (`1b 2a 3c`). The widget stays
   out: several of these carry a measurement or a code shape, and a decision
   whose evidence is hidden is a guess.

5. **Apply immediately, in the same session.** Spec, design.md or CLAUDE.md
   amendment; DESIGN-LOG line; the queue item ticked **with the verdict written
   into it**; one commit. A decision recorded and not applied is the same open
   question with more paperwork.

Anything the author defers stays open **with its blocker named**, so the next
session ranks it without re-deriving why.

## Do not reach for a panel from here

This skill is fast; `/panel` is not. The first session convened a full five-judge
panel on its first question and cost thirty minutes and four hand-built runtimes
for an answer two judges gave identically — the author stopped it mid-flight.

If an item turns out to need a panel, **say so and stop**: hand the author the
question already instructed — the proposal in ten lines, the measurement, what
each lane would cost — and let them convene it when they have time to wait.
`/panel`'s soundness lane exists for exactly the cheap case, and choosing it is
still their call, not this session's.
