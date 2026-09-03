# docs/reasoning — the record of a reasoning session

A **reasoning session** is a conversation in which the author asks questions to
understand the project and **no file of code, spec or design is modified**. One
file per session, historicized: notes accumulate for the life of the project
and are **never deleted** (history lives in git).

Why the directory exists: `DESIGN-LOG.md` records that `docs/glossary/` was
born on author instruction because "good explanations were evaporating in
chat". That move saved the *explanation*. It does not save the *path through
the artifacts* that produced it, or what the session left open — a glossary
entry is a concept, not a session. This directory holds that remainder.

## Division of competence — what goes where

This is the first rule because it is the one a new directory is most likely to
break. `docs/glossary/README.md` says "one home for every concept note", and
it means it: it already absorbed the old `docs/theory/`.

| What the session produced | Where it goes |
|---|---|
| the reusable explanation of a concept | `docs/glossary/NNN-<concept>.md` — a note here **cites** it, never restates it |
| a question worth re-doing as retrieval practice | `docs/learn/LEARN.md`, in its pipe format |
| a question the compiler is waiting on, or a panel to convene | `docs/work/DECIDE.md`, same format |
| work that already has a milestone to do it in | `docs/work/SCHEDULED.md`, same format |
| a gap or a condition on the record | the watch list in `docs/panel/OPEN-QUESTIONS.md` |
| a **change** to the language | **never here** → `/panel` (CLAUDE.md §4) |
| a decision | `DESIGN-LOG.md`, one line (CLAUDE.md §14) |
| the question, the path between the artifacts, what stayed open | `docs/reasoning/NNN-<topic>.md` — the only thing none of the above holds |

## Rules

- **Impersonal.** A note records the question and the answer, **never** what
  the author did or did not know. `/learn` states the rule these notes
  inherit: "Lessons stay impersonal — shapes and rules, never scores; personal
  performance is never written anywhere."
- **English, like every artifact** (CLAUDE.md §11); the conversation stays
  Italian — the same split `/learn` already declares. **One carve-out**: the
  author's question may be quoted verbatim in Italian, on the precedent
  `docs/book/README.md` sets for `beats.md` — "Italian quotes are welcome
  inside beats: they are quoted speech, not artifacts". Quoted speech, not an
  artifact; nothing else in the file is Italian.
- **The session is read-only.** The note, its index line, and the hand-offs in
  the table above are everything the session writes. Nothing under `crates/`,
  `spec/`, `design.md`, `tests/`.
- **Every asserted design rule cites its section**, reached by grep and not
  from memory (CLAUDE.md §1). A claim that cannot be cited is written as a
  conjecture and labelled one, or not written.
- **Never a design change** (CLAUDE.md §4). If a session concludes the design
  should change, the note says so and hands it to the panel; it does not
  amend `design.md`, and it does not pre-empt the verdict.
- **A text the session brings in is audited, not just filed.** When a note
  incorporates prose written elsewhere, it states what the document already
  says (with line-level citations), what is new, what is *dangling* — a claim
  citing a rule the document does not contain — and what precedent still needs
  a source. Unsourced precedent is inadmissible; that is the historian's
  standing rule in `/panel`, and it does not relax outside the panel.
- One session per file, `NNN-<topic>.md`, numbered in birth order, so the
  numbers are the reading order. The index below stays current.
- **Short**: one screen per question, ~200 lines — CLAUDE.md §11's
  short-files rule, at the scale the glossary entries already sit at (75–111).

## What is and is not a reasoning session

A session qualifies when the author asked at least one question about the
project and no code, spec or design file was modified. A `/step`, `/panel`,
`/learn` or `/decide` session does **not** qualify: each already has its
artifact — the journal, the panel file, the ticked list — and recording it
twice is noise.

**One exception, and it is bounded by the reason for the rule** (author decision
2026-08-15, from `docs/reasoning/004`): a session that *reasons its way to a
panel* may leave a note **when the note holds what the panel file does not** —
the path to the question, the inventory it was measured against, the tension that
produced the proposal. It must hold **neither the proposal nor the verdict**,
because those are the panel file's and duplicating them is exactly the noise the
rule forbids. The test is mechanical and reads the two artifacts, not the
session's intent: if the note can be deleted without losing anything the panel
file does not already say, it should not have been written.

That is why the exception is a sentence rather than a licence. The rule's reason
was never *"one session, one artifact"* — it was *"do not record the same thing
twice"*, and 004 is the case where the two artifacts differ.
A pure orientation (`/where`, "a che punto siamo?") produces no note: there is
no resolved question to re-read.

## Template

```
# NNN — <topic>

**Origin.** <date> · reasoning session · <what the author wanted to
understand>. Read: design.md §… · spec § … · <files consulted, none modified>.

## The question
## What the artifacts say        (every claim citing file + section)
## What was settled
## What stayed open              → where it was handed off
```

## Index

- [000-reasoning-sessions.md](000-reasoning-sessions.md) — the session that
  made this directory; why a session record is not a glossary entry, and the
  four choices that fixed the shape (new directory · a note per chat ·
  English · one commit per note)
- [001-value-semantics-lineage.md](001-value-semantics-lineage.md) — the
  lineage behind §4.10 (APL/J/K/BQN, R/MATLAB, Erlang, Swift, Hylo) and the
  five rejected alternatives with what each would have cost; why the K/APL
  entry in the historical appendix is not a contradiction
- [002-message-passing-and-separate-heaps.md](002-message-passing-and-separate-heaps.md)
  — what "separate heaps" means concretely for Part 7 item 13: a message is
  any Heroes value, two copy regimes that must not be unified, and the three
  dangling rules the text assumes design.md already states
- [003-comptime-and-macros.md](003-comptime-and-macros.md) — what "comptime"
  names once the word is split into seven capabilities, why four of them are
  refused by something other than Part 6, and why declining an *additive*
  mechanism beside *load-bearing* generics forfeits nothing a later version
  could not add
- [004-a-project-file.md](004-a-project-file.md) — the nine jobs a `Cargo.toml`
  does, seven of which already have homes with stated reasons; why Principle 0
  does not fund a manifest and the nearest refusals do not reach one; and the two
  decisions of 2026-08-14 that pull against each other about where a header is
- [005-packages-in-place-of-a-standard-library.md](005-packages-in-place-of-a-standard-library.md)
  — what "a mini standard library" names once §1.11's refusal and the ROADMAP's
  routing are read together; what the corpus has already written by hand; the two
  compiler facts that stand between Heroes and a web server, and the HTTP server
  that answered `curl` anyway; five shapes of conditional compilation against the
  record that already weighs on them

## A note on the vocabulary of 000–002

Notes `000`–`002` were written on 2026-08-10 and are dated records: they are
appended to, never rewritten (CLAUDE.md §14). Two things they name have since
changed, and the notes keep the old words.

- **`/debrief` was split in two on 2026-08-12** — `/learn` (what is true) and
  `/decide` (what should be true) — and `docs/debrief/QUEUE.md` became the
  record plus the index over `LEARN.md`, `DECIDE.md` and `SCHEDULED.md`; on
  2026-08-26 that record became `docs/work/DONE.md` and the three lists moved
  to `docs/learn/` and `docs/work/`. Where a note says `/debrief` or "the
  queue", read the table above.
- **Milestone identifiers became names on 2026-08-12** (CLAUDE.md §14). Note
  `002` keys its obligations to `M5a`, which is retired; the map from the old
  numbers to the names is in `docs/ROADMAP.md` § The names (`M5a` is
  `M-scalars-run`).

Note `003` is later than both and already uses the current vocabulary.
