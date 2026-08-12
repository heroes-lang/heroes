---
name: debrief
description: Process the debrief queue (docs/debrief/QUEUE.md) with the author. Two modes: `/debrief doubts` settles open DECISIONS only, fast, no teaching; plain `/debrief` is the comprehension session — retrieval-first walkthroughs, failure diagnosis, ratifications, drills. The author decides when and how much; nothing here ever blocks development.
---

# /debrief [n] · /debrief doubts — on the author's clock

**Two modes, and the argument goes to `doubts`** (author instruction
2026-08-12). The queue holds two kinds of item that had been walked at one
speed: *decisions* the compiler is waiting on, and *comprehension* the author
owes nobody. They have opposite clocks. A decision left open goes on shaping the
code by default; an explanation left unread costs nothing until the author wants
it.

## Mode: `doubts` — decisions only, no teaching

The whole session is: what is still open, what does it block, what is the answer.
No retrieval, no walkthrough, no drill, no glossary.

1. **Triage the queue for decisions.** An item is a decision if it asks *what
   should be true* — `Decide:`, `Undecided:`, an open question, a panel default
   awaiting a verdict. An item that asks *what is true* is comprehension and is
   out of scope here, however interesting.
2. **Verify before asking, and this is the rule the mode exists for.** Run the
   thing. A queue entry is a claim from the day it was written, and several have
   outlived their cause — the map that "cannot be added to" grows, the `%` sign
   the spec now states, the error code the spec now names. **Asking the author
   about a settled question is the one cost this mode cannot pay**, so every item
   is checked against the repository first and the stale ones are ticked with
   what closed them, not asked.
3. **Rank by what it blocks**, not by age: a decision the next milestone reads is
   ahead of one nothing reads yet. Say the blocker in the question.
4. **Ask the whole batch at once, in plain text**, per the rule below — the
   measurement or the snippet visible, options lettered, one reply.
5. **Apply immediately**: spec/design.md/CLAUDE.md amendment, DESIGN-LOG line,
   the queue item ticked with the verdict, one commit. A decision recorded and
   not applied is the same open question with more paperwork.

Anything the author defers stays open **with its blocker named**, so the next
session ranks it without re-deriving why.

## Mode: plain `/debrief [n]` — comprehension, on the author's clock

Development never waits for understanding; this is where understanding
catches up. Conversation in Italian, using the `/where` canonical analogies;
artifacts written here (glossary entries, queue updates) in English.

**Ask in plain text, never in the question widget** (author instruction
2026-08-04). Put the code snippet in a fenced block and the options in a
lettered list, in the message itself — the widget hides the very code the
question is about, and a question whose subject is invisible is not
retrieval practice. Ask the whole batch at once and stop; the author
answers in one reply ("1b 2a 3a 4c"). The widget stays fine for decisions
(panel ratifications, picking an approach), never for questions about code.

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
