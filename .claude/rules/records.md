---
paths:
  - "docs/**"
  - "DESIGN-LOG.md"
  - "CLAUDE.md"
  - ".claude/**"
  - "site/README.md"
---

# The records

Home of CLAUDE.md § 14's naming, vocabulary and release rules since 2026-09-07.
What each rule cost to learn is in `docs/contract/case-law.md`, cited as
`CL-NNN`.

## A record is never rewritten

`docs/panel/`, `docs/journal/`, `docs/measurements/`, `docs/work/DONE.md`,
`docs/book/beats/`, `docs/contract/`, `tests/golden/`, `DESIGN-LOG.md`, every
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

Rotated so far: `docs/book/beats.md` → `docs/book/beats/` (2026-09-12, 114
entries). `ROTATED` in `tests/harness/suite_records.hero` is the live list.

## And the record says whose idea it was

Name the author where a question, a correction or a refusal of theirs is what
produced the finding. Name the panel seat where a seat found it. Name nobody
where the work was ordinary. **It cuts both ways**: crediting the author for
something the assistant found is the same falsehood wearing the flattering sign,
and nobody would check that one (CL-058).

## Milestone identifiers are names, not numbers

The algorithm, and this is its only home (CL-003; `docs/ROADMAP.md` § The names
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
the milestone in `docs/work/SCHEDULED.md`, the padded `panel NNN` in
`docs/work/DECIDE.md`, the defect number in `docs/work/DEFECTS.md`, the origin in
`docs/learn/LEARN.md`. Put a sitting's number in a body instead and every pending
verdict reports as unqueued, silently.

**A line that carries an archived or never-written path carries its date on that
same physical line**, because the citation check reads one line at a time.

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

## Where a session's output goes

A conversation whose work is questions, with no file of code, spec or design
modified, **writes no note of its own** (CL-053). What it settled is an entry in
`docs/work/DONE.md`; what it left open is an item in `docs/work/DECIDE.md` or
`docs/work/SCHEDULED.md`; a concept it explained is an entry in
`docs/glossary/`; a question worth re-asking is a line in `docs/learn/LEARN.md`;
a change to the language is a panel; a decision taken is a `DESIGN-LOG.md` line.
The path between them is the git history.
