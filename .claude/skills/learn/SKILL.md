---
name: learn
description: Teach the author one part of the compiler they want to understand, from the learning list (docs/learn/LEARN.md). Long preamble, code on screen, then one very clear question. NEVER convened by the assistant, never blocking, no decisions — it exists only when the author asks for it.
---

# /learn [n] — understanding, only when asked for

The author is learning compilers through this project. This is the only place
that serves that, and it serves **nothing else**: no decisions, no ratifications,
no unblocking. Those live in `/decide`, and mixing them is what this split was
made to end (author instruction 2026-08-12) — a session that alternates *"what
does this teach you"* with *"the compiler is waiting on your answer"* is two
different clocks in one room, and the second always wins.

**Never convened by the assistant.** Not at a milestone close, not when the queue
grows, not as a suggestion at the end of a step. `docs/learn/LEARN.md` is written by `/step`
and read here, and the author decides when. Conversation in Italian, using
`/where`'s canonical analogies; artifacts written here in English.

## The rule this skill is really about

**Long preamble, complete setup, then one very clear question** (author
instruction 2026-08-12, and it overrides the terser habit that came before).

A queue entry is a note written by someone who had the file open. Read out loud
it becomes a riddle: *"which field decides it, and where is it computed?"* is
unanswerable without knowing what "it" is, what the alternatives were, and what
the code looks like. That is not retrieval practice; it is a memory test on
something never learnt.

So each item is set up **in full** before anything is asked:

1. **Where we are.** What part of the compiler this is, what it is for, and what
   the input and output of that stage are — in `/where`'s register, assuming no
   compiler knowledge.
2. **Why it exists at all.** What would go wrong without this piece. The
   *problem* before the solution, always.
3. **The code, on screen**, in a fenced block, trimmed to what the question is
   about and no more. Never "open `x.rs` and look" — if it is worth asking about,
   it is worth pasting.
4. **What was decided and what the alternatives were** — because most questions
   here are really *"why this way and not the obvious way"*, and the obvious way
   has to be visible to be rejected.
5. **Then the question**, one sentence, with lettered options.

The preamble may run twenty lines. That is correct. **The one thing it must never
contain is the answer** — everything needed to *think* is on screen, and nothing
that makes thinking unnecessary.

## Asking

**Plain text, never the question widget** (author instruction 2026-08-04). The
widget hides the code the question is about, and a question whose subject is
invisible is not a question. Code in a fenced block, options as a lettered list,
in the message itself.

**Ask the whole batch at once and stop.** The author answers in one reply
(`1b 2a 3c`). Three to five items is a session; more is a lecture.

## After the answers

- **Right**: say so in one line and move on. No praise, no elaboration.
- **Wrong**: give the answer, then the walkthrough — before → after on a real
  example from this repository, and the commit that made it so.
- **A genuinely new explanation** — one the author needed and the record does not
  hold — is distilled into `docs/glossary/NNN-<concept>.md` (numbered in birth
  order, English, canonical analogies, origin cited, never deleted) and the
  glossary index is updated.
- Tick the items in `LEARN.md` and leave them checked; the list is also the
  record. Commit: `learn: <what was covered>`.

**Lessons stay impersonal — shapes and rules, never scores.** Nothing about how
the author did is written anywhere, in this repository or in a message. What is
written is what the *code* teaches.

## Where the items are

`docs/learn/LEARN.md`, and nowhere else. The queue was split into three lists
on 2026-08-12 because one file held 192 open items of three kinds, and a list you
have to filter before you can read it is a list nobody opens:

- **`LEARN.md`** — what is true. This skill's, and only this skill's.
- **`DECIDE.md`** — what *should* be true. `/decide`'s, because the compiler
  goes on behaving some way while it waits, which is a different urgency from
  wanting to understand.
- **`SCHEDULED.md`** — work with a home, read by `/step` at the milestone that
  names it.

`docs/work/DONE.md` is the record: every ticked item lands there and stays.

**This paragraph named `QUEUE.md` until 2026-09-07 and that file has not existed
since 2026-08-26**, when the lists were renamed. Worse, the reason it gave was
*"the path every commit subject cites"*, which is the exact clause CLAUDE.md § 3
records as **measured false**: 24 subjects of 792, 3.0%, over the 463 commits
before the rename. `/decide` and `/where` were repaired in that same batch and
this file was missed. No instrument caught it for twelve days, because the
dead-citation check requires a `/` in the token and a bare `QUEUE.md` has none
(`docs/contract/case-law.md` CL-031).
