# 000 — A session record is not a glossary entry

**Origin.** 2026-08-10 · reasoning session · the author opened a chat to
reason about the project rather than advance it: «uso questa chat per capire
… posso farti delle domande per ragionare sul progetto senza mai modificare i
file. Al limite salviamo le sintesi di queste chat in una cartella del
progetto.» Read: `CLAUDE.md` §1, §4, §11, §14 · `docs/glossary/README.md` ·
`docs/debrief/QUEUE.md` · `docs/panel/OPEN-QUESTIONS.md` ·
`docs/book/README.md` · `.claude/skills/{where,debrief}/SKILL.md` ·
`DESIGN-LOG.md`. No file of code, spec or design modified.

## The question

Two questions, and only the second one was hard.

1. Can a chat be used to reason about the project without modifying anything?
2. Where do the syntheses go?

## What the artifacts say

**The project had already answered this once, for a different remainder.**
`DESIGN-LOG.md`, 2026-08-03: `docs/glossary/` was created on author
instruction because "good explanations were evaporating in chat; first entry:
000-basic-block". So the failure mode is on the record, and so is the fix — for
*explanations*.

**Three homes already exist, and each is deliberately lossy.**

- `docs/glossary/NNN-<concept>.md` — the distilled explanation, one concept per
  file, born (per its README) either from "a debrief gap" or from "a `/where`
  pillola". It is the closest existing thing to a session record, and it keeps
  the concept while discarding the session: an entry opens with `**Origin.**`
  naming the date and the friction, and then talks about the concept only.
- `docs/debrief/QUEUE.md` — questions are *pre-written* here and ticked off;
  "the queue is also the record". The answers and the discussion are not stored.
- `docs/panel/OPEN-QUESTIONS.md` — open sessions plus a watch list of
  "conditions and gaps on the record". Author decisions, not reasoning.

**One more place stores the author's own words.** `docs/book/README.md`
licenses, for `beats.md`, "what the author actually asked, verbatim when it
matters (Italian quotes are welcome inside beats: they are quoted speech, not
artifacts)". This is the only existing exemption from CLAUDE.md §11, and it is
narrow: quoted speech.

**So the remainder is identifiable.** What none of the three holds is the
*path*: which files were opened, in what order, and what the answer turned out
to depend on — plus what was still standing when the chat ended.

**Two constraints bind any conversation record.**

- `/debrief` SKILL.md: "Lessons stay impersonal — shapes and rules, never
  scores; personal performance is never written anywhere." A record of a
  session in which someone was learning is the most likely place in the repo to
  break this rule, so it is written first, not last.
- CLAUDE.md §4: a design change needs `/panel` + a `docs/panel/NNN-*.md` + a
  DESIGN-LOG line. A directory that accumulated conclusions about the language
  would be a way to amend the design without any of that. Hence the hard rule:
  a reasoning note never amends `design.md`, and it does not pre-empt a verdict.

**Discoverability turned out to decide the shape.** There is no
`docs/README.md` and no index for `journal/`, `panel/`, `measurements/` or
`debrief/`. `docs/measurements/002-metric-3.md` and most of `docs/panel/` are
reachable only by filename convention or by a citation from `QUEUE.md` or
`DESIGN-LOG.md`'s panel column. A new kind of document is therefore invisible
unless it carries its own `README.md` index **or** is cited from one of those
two files.

## What was settled

Four choices, author instruction, 2026-08-10:

| Question | Choice |
|---|---|
| where | a new directory, `docs/reasoning/`, over routing into the three existing homes |
| when | a note at the end of every qualifying chat, automatically |
| language | English, per §11, with the `beats.md` carve-out for the quoted Italian question |
| commit | one commit per note, pushed |

And three rules that follow from the artifacts rather than from the choices:

1. **The division of competence is the README's first section**, not a footnote.
   `docs/glossary/README.md` says "one home for every concept note" and has
   already absorbed one directory (`docs/theory/`, 2026-08-03) to keep that
   true. A note therefore **cites** a glossary entry and never restates it.
2. **Impersonal, inherited verbatim from `/debrief`.**
3. **A text brought in from elsewhere is audited, not just filed** — what the
   document already says, what is new, what is *dangling*, what precedent still
   needs a source. This rule was not designed; it was extracted from the
   defect class this project keeps catching in its own documents (a spec that
   briefed models into a reserved-word error for four milestones; "five
   documents that had stopped being true" at panel 019).

**Why this is not a panel path.** CLAUDE.md §4 lists what needs the panel —
`spec/**`, design.md Parts 1–11, surface syntax or semantics, a diagnostic
class, architecture — and then says the teaching process "is amended by author
instruction, no panel". A documentation duty is teaching process.

**A concern was raised and overruled, and it is on the record because the
reasoning matters more than the outcome**: a fourth directory sits close to
the glossary's "one home" rule. The author chose the directory anyway. The rule
is preserved by division of competence rather than by refusing the directory —
the note holds the session, the glossary holds the concept, and the note cites.

## What stayed open

- **Whether a pure orientation should produce a note.** "Automatically at the
  end of every chat" needs a definition a future session can apply, so the
  README supplies one: at least one question asked, no code/spec/design file
  modified, and `/step`, `/panel`, `/debrief` excluded because each already has
  its artifact. The assumption written in — that a `/where` orientation
  produces no note, having no resolved question to re-read — is one line in the
  README if the author wants it the other way.
- **Whether `beats.md` should take a line per practice change.** CLAUDE.md §14
  ties the story beat to the milestone, not the session; one was appended here
  because the birth of the glossary is already part of the project's story and
  this has the same shape. Strikeable, one line.
