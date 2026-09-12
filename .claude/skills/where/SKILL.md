---
name: where
description: Orient the author — explain in very simple ITALIAN where the Heroes project stands, what is being built right now and why, assuming zero compiler knowledge. Use whenever the author asks where the project stands, what is being built, or for an explanation (they ask in Italian: "a che punto siamo", "cosa stiamo facendo", "spiegami"), or returns after a break.
---

# /where — orient the author, gently

The author knows programming languages well but is **learning compiler
construction from zero through this project**. This skill produces a status
report that is also a small lesson. It must never make them feel behind.

## Output language: ITALIAN. Everything the author reads from this skill is Italian.
The words to say it in are in **`italian-vocabulary.md`, beside this file** —
the section headings, the pipeline drawing and the fixed analogies. That file
is the only Italian one here, and CLAUDE.md §11 names it as part of its second
declared exception: a translation that is a deliverable. This file, like every
other rule in the repository, is English.

Keep technical identifiers as they are (`lexer/`, `heroes check`, file names),
but **every technical term gets a plain-Italian explanation the first time it
appears — no exceptions, take nothing for granted.**

## Step 1 — Gather the facts (do not guess)

```
git log --oneline -15
git tag --list --sort=creatordate
tail -8 DESIGN-LOG.md
sed -n '/^## Where we are/,/^## Verify/p' docs/ROADMAP.md        # where we are
sed -n '/^## The chain/,/^## The milestones/p' docs/ROADMAP.md   # what is next, in order
                                  # (this said `head -40 … § Status + § The order` until
                                  #  2026-08-26: two headings renamed on 2026-08-25 and a
                                  #  line count that stopped reaching the table it named.
                                  #  A pattern range cannot rot the same way.)
cat docs/work/DECIDE.md           # open decisions = what the compiler is waiting on
cat docs/work/DEFECTS.md          # what is BROKEN right now
                                  # (missing from this block until 2026-09-07, four days
                                  #  after the author instituted the list: no skill read
                                  #  it, so an open defect was invisible in exactly the
                                  #  report the author asks for. CL-044.)
cat docs/work/milestones/*.md          # open work, each item inside its own milestone's file
                                  # (this said `QUEUE.md` until 2026-08-26 — the RECORD,
                                  #  which holds only closed items and had held zero open
                                  #  ones since the 2026-08-12 split. The skill reported a
                                  #  finished archive as the author's outstanding work.)
./heroes run tests/harness/main.hero -- ./heroes 2>&1 | tail -2   # green or red, one line
                                  # (`cargo test` until M-bootstrap-archive; the
                                  #  Rust it ran is archive/bootstrap-rs/ now)
```

Determine: current milestone (from tags + commit messages), what the last
3–5 commits actually did, what is waiting on the author.

## Step 2 — Emit the report, in this exact structure

### 1. The map — always the same drawing
Show the full pipeline with today's position marked, under the heading and with
the drawing that `italian-vocabulary.md` fixes, adding today's markers
(✅ done · 🔨 in progress · ⬜ not started). The drawing never changes shape
between sessions; only the markers move.

### 2. Where we are — 3–6 sentences
What milestone we are in, what the last commits did, whether tests are green —
in everyday words. Numbers and file names welcome, jargon translated.

### 3. What we are building right now, explained simply
The heart of the skill: explain the CURRENT stage as if to a smart friend who
has never heard the word *compiler*. Rules:
- Start from what a compiler even is if relevant, using the sentence
  `italian-vocabulary.md` keeps for it.
- Use ONE tiny concrete example from the repo (3–6 lines of `.hero` or of a
  spike) and show what THIS stage does to it, before → after.
- Use the canonical analogies from that file — always the same ones, so the
  author's mental model accumulates instead of resetting.

### 4. Your turn — the author's pending items (never blocking)
Summarize the **open** items of `docs/work/DECIDE.md` (what the compiler is
waiting on), `docs/work/DEFECTS.md` (what is broken) and
`docs/work/SCHEDULED.md (retired 2026-09-12)` (work with a milestone), each with the file path and
why it is worth their time — the vocabulary file keeps the sentence for the
commonest reason, that guessing a cause before reading the fix is where the
value is. Make clear nothing is waiting on them to proceed.

**A defect is said in plain words and without cushioning**: what a program does
wrong today, on the author's own line, not the module it lives in. It goes first
when there is one, because a broken compiler outranks a pending decision, and
saying so with force is CLAUDE.md § 11's *alive rather than flat*.

**Two records this step must not read.** `docs/done/` is the record: it is
all closed, so summarising it reports finished work as owed. `docs/work/learn/LEARN.md`
is comprehension and is **never** offered here — it exists when the author asks
for it (`/learn`'s own rule), and listing it turns an offer into a debt.

### 5. Today's pill — one micro-lesson
ONE concept (3–5 sentences max), tied to the current stage, with its analogy.
End with a question the author can answer mentally to check they got it.
If the concept has a `docs/ref/glossary/` entry, link it; if the pill resolves
a fresh gap, distill it into a new glossary entry afterwards (a gap resolved
is an artifact earned — see `/learn`).

## Canonical analogies

They live in **`italian-vocabulary.md`**, beside this file, with the section
headings and the pipeline drawing. Read it before writing the report: the whole
point is that the same term gets the same words every time, so the author's
picture accumulates instead of resetting. A term with no row there gets a fresh
plain-Italian gloss, and the row is added afterwards.

## Tone rules
- Never English jargon without immediate Italian gloss.
- Short sentences. No walls of text: the five sections, nothing more.
- Honest about what's broken or pending; never euphemistic, never alarmist.
- If the author seems lost across sessions, suggest re-reading ONE journal
  entry, not five documents.
