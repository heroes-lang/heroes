---
name: decide
description: Settle the open DECISIONS the compiler is waiting on, from the decision list (docs/work/DECIDE.md) — fast, no teaching. Verify each item against the repository before asking; a settled question put to the author is the one cost this skill cannot pay. Apply every answer in the same session.
---

# /decide [n] — the decisions, and nothing else

Split from `/learn` by author instruction (2026-08-12), and named after the list
it reads — `docs/work/DECIDE.md` — because a skill whose name does not match
its list is one more thing to remember. The queue held two kinds of item with
**opposite clocks**: a decision left open goes on shaping
the code by default, an explanation left unread costs nothing until it is wanted.
This takes the first kind only.

The whole session is: **what is still open, what does it block, what is the
answer.** No retrieval, no walkthrough, no drill, no glossary. If the author asks
"why is it like that", answer in two sentences and move on — the long version is
`/learn`'s.

## Procedure

1. **Read `docs/work/DECIDE.md`.** The triage is already done — the queue was
   split into three lists on 2026-08-12, and a fourth for what is broken arrived
   on 2026-09-03 — so this step is only checking that nothing arrived in the
   wrong one. An item that asks *what is true* belongs in `LEARN.md` however
   interesting; one that names the milestone which will do it belongs in
   `SCHEDULED.md`; one that describes a compiler **defect**, a crash or a wrong
   answer at exit 0, belongs in `DEFECTS.md` and is not a decision at all. Move
   it rather than answering it.

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
   amendment; a `docs/log/` entry; the item ticked **with the verdict written
   into it** and **cut from `docs/work/DECIDE.md`, pasted at the end of
   `docs/work/DONE.md`**; one commit. A decision recorded and not applied is the
   same open question with more paperwork.

   **The move is not tidying, it is this step.** The rule was written in three
   places — CLAUDE.md §3, `DESIGN-LOG:240`, the record's own preamble — and this
   line did not say to do it, so nobody did: by 2026-08-26 `DECIDE.md` held **138
   ticked items and zero open ones**, 391 KB, under eighteen headings still
   titled `## Open`, while the record had gone untouched for eight days. A list
   that keeps its own dead is not a list of what is owed.

6. **One notation, and the file is only items.** `- [ ]` in the live lists,
   `- [x]` in the record. No `## ` sections, no free prose paragraphs, no
   `~~strikethrough~~`. Nine live FFI findings sat under two panel headings in
   `DECIDE.md` as bare bullets no count could see, and when they were finally
   read **five were already closed** and two were measurably false.

   **The shape, since 2026-09-07 and by author instruction — the same in all
   three live lists.** One line per item, three fields:
   `- [ ] **<origin>** | <the question, in one line> | <where to look>`, and
   under it an optional body indented four spaces, opening with `**Origin:**`
   and its date. The items live between two lines of asterisks so that a glance
   says whether anything is owed. **An indented body is not the prose this rule
   forbids** — every count in this project reads the `- [ ] ` line, and a body
   under one is invisible to none of them; what is forbidden is prose at column
   zero, which is how `DEFECTS.md` came to carry five paragraphs about defects
   already in the record. Nothing lives outside the two banners.

   **The first field is what the instrument reads, which is why it differs by
   file**: the sitting in `DECIDE.md` (`panel NNN`, padded — `records/lists`'
   neighbour `queued` scans the item LINE for it), the milestone in
   `SCHEDULED.md`, the defect number in `DEFECTS.md`. Put a sitting's number in
   a body and every pending panel reports as unqueued, silently.

   `tests/harness/suite_records.hero`'s **`records/lists`** is the executor:
   no `- [x]` in a work list, no `- [ ]` in the record, nothing outside the
   banners, and the count in the banner equal to the items counted. It exists
   because this rule was stated in four documents and performed by none.
   `docs/learn/LEARN.md` takes the same shape with one rule fewer — a ticked
   question stays in it, because that list is also its own record — so the check
   reads it with `ticked_allowed` and its banner counts the open questions.

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
