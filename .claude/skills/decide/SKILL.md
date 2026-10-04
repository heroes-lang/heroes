---
name: decide
description: Settle the open DECISIONS the compiler is waiting on, the open decision issues of issues/ — fast, no teaching. Verify each item against the repository before asking; a settled question put to the author is the one cost this skill cannot pay. Apply every answer in the same session.
---

# /decide [n] — the decisions, and nothing else

Split from `/learn` by author instruction (2026-08-12), and named after the list
it reads, the open `decision` issues (`docs/work/DECIDE.md` until 2026-10-04),
because a skill whose name does not match its list is one more thing to
remember. The queue held two kinds of item with
**opposite clocks**: a decision left open goes on shaping
the code by default, an explanation left unread costs nothing until it is wanted.
This takes the first kind only.

The whole session is: **what is still open, what does it block, what is the
answer.** No retrieval, no walkthrough, no drill, no glossary. If the author asks
"why is it like that", answer in two sentences and move on — the long version is
`/learn`'s.

## Procedure

1. **Read the open decisions**: `grep -rl "^kind: decision" issues/ | xargs grep -l "^- \[ \] "`.
   This step only checks that nothing arrived as the wrong kind. An item that
   asks *what is true* is a `learn` issue however interesting; one that names
   the milestone which will do it is that milestone's `feature` or `task`; one
   that describes a compiler **defect**, a crash or a wrong answer at exit 0, is
   a `defect` and not a decision at all. Correct its card's `kind` rather than
   answering it: the card is metadata, corrected where it stands, and the file
   does not move (`.claude/rules/records.md` § The issues).

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
   amendment; a `decision` issue for the decision taken, closed the day it is
   filed; the open item **ticked where it stands, with the verdict written
   under it** and its card's `commit` set to `self`; one commit. A decision
   recorded and not applied is the same open question with more paperwork.

   **Ticking is not tidying, it is this step.** Before the lists were files the
   rule was to move a ticked item into the record, written in three places and
   performed by none: by 2026-08-26 `DECIDE.md` held **138 ticked items and zero
   open ones**, 391 KB, under eighteen headings still titled `## Open`. Since
   2026-10-04 nothing moves, and an issue whose box is ticked is closed by that
   alone (CL-080).

6. **One notation, and an issue holds only its item.** `- [ ]` open, `- [x]`
   closed. No `## ` sections, no free prose at column zero, no
   `~~strikethrough~~`. Nine live FFI findings once sat under two panel
   headings as bare bullets no count could see, and when they were finally read
   **five were already closed** and two were measurably false.

   **The shape**, the same for every kind since 2026-09-07: under the card, one
   line of three fields, `- [ ] **<origin>** | <the question, in one line> |
   <where to look>`, and under it an optional body indented four spaces,
   opening with `**Origin:**` and its date. **An indented body is not the prose
   this rule forbids** — every count reads the `- [ ] ` line.

   **The first field is what the instrument reads**: a sitting's ratification
   opens `**panel NNN**`, padded, because `records/verdicts` scans the item
   LINE for it; put the number in a body and every pending panel reports as
   unqueued, silently. `records/cards` and `records/lists` are the executors,
   `tests/harness/suite_records.hero`'s, and the rules are
   `.claude/rules/records.md` § The issues.

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
