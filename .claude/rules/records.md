---
paths:
  - "docs/**"
  - "CLAUDE.md"
  - ".claude/**"
  - "site/README.md"
---

# The records

Home of CLAUDE.md § 14's naming, vocabulary and release rules since 2026-09-07.
What each rule cost to learn is in `docs/records/contract/case-law.md`, cited as
`CL-NNN`.

## A record is never rewritten

`docs/panel/`, `docs/records/journal/`, `docs/measurements/`, `docs/records/done/`,
`docs/records/book/beats/`, `docs/records/contract/`, `tests/golden/`,
`docs/design/DESIGN-LOG.md`, every
commit subject and the twelve legacy tags are append-only. Where a sentence in
one has since been falsified, the correction is **added underneath**, with its
date: a measurement that was right when it was taken is history, and a record
that quietly loses its inconvenient half is worth less than none.

**Appending to a dated record uses that record's vocabulary**, with the new name
in parentheses on first use. `tests/harness/suite_records.hero` knows which
trees are records and treats them accordingly, and `records/appended` is what
makes this section a rule rather than a sentence: it went unperformed until
2026-09-12, and `76799a18` had already removed a line from the record with every
suite green.

## A rotated record: the entries are files, the old file is a map

**A new entry in a rotated record is a NEW FILE**, `YYYY-MM-DD-HHMM-<slug>.md`
in the tree, never a line appended to the map. Two sessions then write two files
and never one line, which is the whole reason the shape exists.

The file that used to hold the entries stays at its path **with the line count
it had**, one row per entry naming the file that now holds it. That is what
keeps a `<file>:NNN` citation resolving: CL-037 says a line number is the one
citation shape no instrument can see, and this repository writes 161 of them
into its two largest records. A map makes them checkable instead —
`records/positions` reads every one and follows it.

The minute in the name is a POSITION for an entry written before its rotation: a
historical entry has no clock reading, and two entries of one day still have an
order. **No directory index**, because an index is a second place where truth
lives and the only tabulated one here drifted for nine closes in silence.

Rotated so far: `docs/records/book/beats.md` → `docs/records/book/beats/` (2026-09-12, 114
entries), `DESIGN-LOG.md` → `docs/records/log/` (620) and `docs/work/DONE.md` →
`docs/records/done/` (491), all three on 2026-09-12.
`ROTATED` in `tests/harness/suite_records.hero` is the live list.

## And the record says whose idea it was

Name the author where a question, a correction or a refusal of theirs is what
produced the finding. Name the panel seat where a seat found it. Name nobody
where the work was ordinary. **It cuts both ways**: crediting the author for
something the assistant found is the same falsehood wearing the flattering sign,
and nobody would check that one (CL-058).

## Milestone identifiers are names, not numbers

The algorithm, and this is its only home (CL-003; `docs/roadmap/names.md`
carries the map):

- **two words**, `M-<what-it-delivers>`, hyphenated and lowercase after the
  `M-`; the tag is the same string lowercased;
- **name the deliverable, never the area**, because the area must stay free for
  the second milestone that touches it. A one-word name appropriates a topic, and
  an identifier must make no claim a later milestone can falsify;
- prefer a phrase the ROADMAP entry or the milestone's journal slug already uses
  over an invented one;
- **an id is never renamed once it is in the record.** A milestone that changes
  shape gets a new id, and the old one is retired in § The names;
- **order lives in the ROADMAP's § The chain table and nowhere else.**
  `git tag --list --sort=creatordate` gives the chronology;
- an id never reaches a diagnostic or any user-visible output. The live
  assertions are `records/expected` and `records/numbered` in
  `tests/harness/suite_records.hero`.

## Release tags are a third namespace

Milestone tags are `m-*` and widen the CI matrix. The site's are `site-v*`. A
release of the language is `vX.Y.Z`, and nothing else starts with `v`. **A
release is a commit, never a milestone**, and it is the author's act on a clean
`main`. The number is one constant, `VERSION` in `selfhost/main.hero`, and it
moves only in the commit that carries the tag. While `X` is 0, `Y` moves when
`git diff vA vB -- spec/heroes-spec.md` is not empty and `Z` when it is. The CI
is the instrument, and no binary is uploaded. The reasoning and the six choices
are CL-067 and the `DESIGN-LOG.md` row of 2026-09-07.

## The lists

Four work lists and one comprehension list, all in one shape (CL-066):
`- [ ] **<first field>** | <what, in one line> | <where to look>`, an optional
body indented four spaces opening with `**Origin:**` and its date, and the item
region fenced between two lines of asterisks. `docs/work/DECIDE.md` carries the
rule in full and `records/lists` is its executor.

**The first field is what that file's instrument reads**, so it differs by file:
the milestone in its own `docs/work/milestones/` file, the padded `panel NNN` in
`docs/work/DECIDE.md`, the defect number in `docs/work/DEFECTS.md`, the origin in
`docs/learn/`. Put a sitting's number in a body instead and every pending
verdict reports as unqueued, silently.

**A line that carries an archived or never-written path carries its date on that
same physical line**, because the citation check reads one line at a time.

## A live list is a preamble, a count and its items — author instruction 2026-09-16

`docs/work/DECIDE.md`, `docs/work/DEFECTS.md` and the files under
`docs/work/milestones/` carry **a very short preamble**, then the `**OPEN: N**`
banner, then the items. `docs/ROADMAP.md` carries a very short preamble, then
the two open counts, then its tables. **No other story goes in any of them**,
and a story already there is MOVED rather than deleted — to `docs/records/done/`,
to the milestone's journal, or to a `docs/records/log/` entry, all three
append-only, which is where a thing that happened belongs anyway.

The reason is what these files are for. A list is opened by somebody about to
attack an item, and the ROADMAP by somebody asking what is next; both are opened
to find a number and a row. On the day this rule was given, `DEFECTS.md`'s
preamble ran to **28 lines and 982 words**, and **one paragraph of it** was a
single sentence of **4551 bytes** recording which sitting issued which defect
number since 2026-09-08 — true, hard-won, and standing in front of every reader
who only wanted to know what is broken today.

**Two things the move measured, and both argue for it.** That paragraph stated
the next number to issue **twice, with different values** — `grep -o` returns
**037** and **052** inside the one sentence, because it had been appended to for
weeks and nothing reads it: `records/numbering` computes the next number as one
above the highest issued, and its own comment says why. And it pointed **three
times** at `docs/work/DONE.md`, a file that has been a MAP since 2026-09-12 (two
hits there, one in `DECIDE.md`). A pointer nobody follows does not rot loudly.

**The gap this closes is that `list_offences` only ever policed the item
region.** It refuses a ticked item between the banners, prose between the
banners, an item outside them — and says nothing about what sits ABOVE the
opening banner, which is where all 982 words were. The instrument written after
`DEFECTS.md` grew 2753 bytes of prose about five already-repaired defects was
watching the half of the file that was not the problem. So the executor is
extended rather than the rule left as prose: `records/lists` measures the
preamble against a ceiling, and `records/counts` compares the ROADMAP's two
numbers to the banners the two lists state.

**And the ROADMAP's counts had already drifted when the rule was given**, which
is why they get an instrument and not a convention: § Where we are read *open
defects 0, open decisions 0* while `DEFECTS.md`'s own banner read `**OPEN: 3**`.
That is § A rotated record's lesson — an index is a second place where truth
lives — arriving in the one document the author opens to see where the project
is.

**The milestone files get a larger allowance than the two lists**, named
separately and for a reason rather than by generosity: their preamble IS the
milestone's reasoning, which `/step` § 1 calls the thing that must not be lost.
What the rule excludes there is the same thing it excludes everywhere — the
record of what has already happened.

## Two habits that come from being one checkout among several

**Re-read the chain and the log immediately before writing a scheduling fact**,
never from the copy read at session start (CL-047). For a multi-anchor edit to a
shared record: script the pairs, assert each anchor matches exactly once,
dry-run, then apply.

**A commit limits itself by PATHSPEC, never by what was added**:
`git commit -- <paths>`. CLAUDE.md § Hard stops asks that a commit carry only
this conversation's files, each named on the command line, and `git add <paths>`
followed by a bare `git commit` does not do that: the bare commit takes the
whole staging area, so anything a parallel session has staged rides along under
this session's message. Found on 2026-09-08, when a commit that named fourteen
paths carried sixteen and said in its own body that it had not. Read
`git status` first as the hard stop says, and then let the pathspec, not the
index, decide.

**The repository pushes to `origin`, github.com/heroes-lang/heroes.** The old
name redirects, and creating a repository there would kill the redirects
permanently, so that name is never reused (CL-033).

## Working in lanes, since 2026-09-12

A **lane** is one milestone, in one detached worktree, held by one session. The
rotations above are what make it possible: a lane writes its own milestone file,
its own entries under `docs/records/log/` and `docs/records/done/`, its own beat — every one of
them a file nobody else is writing.

- **The worktree lives outside this tree**, `~/Temp/heroes-lane-<milestone>` or
  the like, and **never** under `.claude/worktrees/`. A nested checkout made a
  tree-walking check report another tree's milestone names as this one's
  (2026-08-12), and the mere existence of that directory hid a citation defect
  for three red CI runs, on the one machine that could not see it (2026-09-07).
  A worktree has its own index, which is why it is safer than a second session
  here: CL-041 and CL-070 are both a shared index carrying away somebody's work.
- **Merge, not rebase.** This repository cites commit hashes inside records —
  `/step` § 3 asks for the fixing commit's hash — and a rebase rewrites them, so
  it falsifies in silence a citation a lane wrote about itself.
- **A lane does not close a milestone.** § Where we are is a measurement of the
  whole tree at an instant, re-measured and never carried (CLAUDE.md §1), and
  two lanes cannot both be right about it: one of the two sentences would be
  false as it was written. Closing is an act on the trunk, by one session, and
  closes accumulate into a train — four tags went onto one commit on 2026-09-11,
  so the shape already exists.
- **The clock is exclusive.** Parallel work is free on correctness and forbidden
  on duration (CL-025, `.claude/rules/verification.md`).

**A LANE IS ALSO HOW WORK CONTINUES WHILE THIS TREE IS OWNED** — author
instruction 2026-09-17, *learn this business of using lanes and write it down*.
Two things own the working tree and neither is rare: **a suite reading it**
(CL-025 — the full net is twenty minutes) and **a sitting between its briefs and
its synthesis** (`/panel`, and it binds the coordinator, not only the judges).
The reflex was to wait. **Waiting is a choice and usually the wrong one**: a
detached worktree has its own index, its own tree and its own compiler, so the
work goes on where nothing the gate reads can move.

What that bought on the night it was written: panel 160 sat on the frozen tree
while defect 049 was repaired, verified on two platforms and merged; then three
lanes ran at once, one per defect, and they met in **merges rather than
conflicts** because each touched files the others did not. A lane per DEFECT,
not per file — that is what makes the independence real and checkable in advance.

Five things a lane needs that this tree has and a fresh worktree does not:

- **its own compiler.** `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` inside it, a few seconds. The trunk's binary is the trunk's.
- **`.env`, which git ignores and a worktree therefore never gets.** `.` it from
  the trunk — `. /path/to/trunk/.env` — and print `${#ANTHROPIC_API_KEY}` if you
  need to know it loaded, never the value. Without it `measure --refresh` exits 2
  and a spec change cannot be priced on the instrument that judges it.
- **the seed regenerated in the lane** when it touches `selfhost/`, with the
  fixpoint verified there, because the seed is what the merge carries.
- **its own suite runs.** A green suite on the trunk says nothing about a lane.
- **removal when it is done**: `git worktree remove` and `git branch -d`, in the
  same session. A worktree left behind is a second tree a later check can walk,
  which is what § Working in lanes' first bullet is about.

And the ordering that makes it safe: **merge the lane before opening the next
one on the same files**, so the second starts from the first's result rather than
from a base that is already behind.
- **One `**OPEN**` row at a time, today.** `site/src/lib/chain.ts` throws when
  § The chain carries more than one, so two lanes cannot both open their row
  until that is decided — it is an outward-facing file. The question is filed in
  `docs/work/DECIDE.md`. Until it is settled, a second lane works with its row
  left `scheduled` and says so in its commits.

## Where a session's output goes

A conversation whose work is questions, with no file of code, spec or design
modified, **writes no note of its own** (CL-053). What it settled is an entry in
`docs/records/done/`; what it left open is an item in `docs/work/DECIDE.md` or
its milestone's file under `docs/work/milestones/`; a concept it explained is an entry in
`docs/ref/glossary/`; a question worth re-asking is a line in `docs/learn/`;
a change to the language is a panel; a decision taken is a `docs/records/log/` entry.
The path between them is the git history.
