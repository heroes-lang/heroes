---
paths:
  - "docs/**"
  - "issues/**"
  - "CLAUDE.md"
  - ".claude/**"
  - "site/README.md"
---

# The records

Home of CLAUDE.md § 14's naming, vocabulary and release rules since 2026-09-07.
What each rule cost to learn is in `docs/records/contract/case-law.md`, cited as
`CL-NNN`.

## A record is never rewritten

`docs/panel/`, `docs/records/journal/`, `docs/measurements/`, a closed issue in
`issues/` below its card (§ The issues), `docs/records/book/beats/`,
`docs/records/contract/`, `tests/golden/`, every commit subject and the twelve
legacy tags are append-only. Where a sentence in
one has since been falsified, the correction is **added underneath**, with its
date: a measurement that was right when it was taken is history, and a record
that quietly loses its inconvenient half is worth less than none.

**Appending to a dated record uses that record's vocabulary**, with the new name
in parentheses on first use. `tests/harness/suite_records.hero` knows which
trees are records and treats them accordingly, and `records/appended` is what
makes this section a rule rather than a sentence: it went unperformed until
2026-09-12, and `76799a18` had already removed a line from the record with every
suite green.

## A tree of entries: one file each, and no map

**A new entry is a NEW FILE**, `YYYY-MM-DD-HHMM-<slug>.md`, in `issues/` or in
`docs/records/book/beats/`. Two sessions then write two files and never one
line, which is the whole reason the shape exists. The minute in the name is a
POSITION for an entry written before its record became a tree: a historical
entry has no clock reading, and two entries of one day still have an order.
**No directory index**, because an index is a second place where truth lives
and the only tabulated one here drifted for nine closes in silence.
`records/entries` holds the beats' names and `records/cards` the issues'.

**No map either, since 2026-10-05** — author instruction, meant as: *I prefer
cleanliness for the future to keeping the historical legacy*. From 2026-09-12
the three files whose entries became trees, `docs/records/book/beats.md`,
`DESIGN-LOG.md` (later `docs/design/DESIGN-LOG.md`) and `docs/work/DONE.md`,
stayed behind at the heights they had, one row per entry, so that a `<file>:NNN`
citation still landed where it always had (CL-037: a line number is the one
citation shape no instrument can see). That day all three were deleted. Every
living citation by line was resolved first to the file it named; a living file
names an entry by its path from then on, which `records/citations` checks. The
citations inside dated records read against the maps at `d8a9913e`, the last
commit that held them, as the specification's line citations read against
`834d804f` (`.claude/rules/spec-shape.md`). **The rule this leaves**: when a
record's shape changes, the citations into it that a living file holds are
rewritten to the new shape in the same change, and no file is kept only so an
old citation still resolves.

## The top of `docs/`: two documents, then collections

Author's answers of 2026-10-05, on the coordinator's reading of `docs/`. **A
file at the top of `docs/` is one of the two living documents the contract
names**, `docs/ROADMAP.md` and `docs/design.md`; **every folder is a collection
of one kind**: `roadmap/` what a row of the chain cannot hold, with
`milestones/` one page per milestone; `learn/` the questions by day and its
`glossary/`; `platforms/` the Linux and Windows machines; `assets/` the front
page's pictures; `metrics/` design.md Part 11's instruments, the operator table,
the frozen prompt and the task suite's rules, which were the top-level
`harness/` until 2026-10-05; `measurements/`, `panel/` and `records/` the
account. A folder
holding one file, or files of unrelated kinds, is the shape this rule refuses:
until 2026-10-05 `docs/design/` held `design.md` alone, `DESIGN-LOG.md` deleted,
and until 2026-10-05 `docs/ref/` held the pictures, the machines and the glossary.

## And the record says whose idea it was

Name the author where a question, a correction or a refusal of theirs is what
produced the finding. Name the panel seat where a seat found it. Name nobody
where the work was ordinary. **It cuts both ways**: crediting the author for
something the assistant found is the same falsehood wearing the flattering sign,
and nobody would check that one (CL-058).

**A ratification is recorded as a reading** (CLAUDE.md § 4, the author's
instruction of 2026-09-19: *write down that I always read*), and `by
delegation` is written only where the author says so. Five ratifications before
that date say `by delegation` on the coordinator's assumption, never put to the
author, and stand at their dates. CL-058 is about the record being TRUE, not
about which claim is humbler: said again by the author 2026-09-21, on a
ratification recorded as *not a reading*, meant as *take the reading for
granted*. Moved here from CLAUDE.md § 4 on 2026-10-11, as what paid for that
section's pointer to this one and to CL-082 and CL-083 (`DELTA_GATE`).

**And where somebody outside this project got there first, the record says so
too** (author instruction 2026-10-07, meant as: *when the issues go to GitHub I
want them honest in crediting whoever solved these things before me: where a
decision was favoured by another language's earlier decision, or by something
found online, the issue says so and names it*; and, the same day, on the first
draft of this rule: *not something that always applies; only where a sitting
sat, the historian spoke, and what the historian said was taken into account;
backwards too, in those cases alone; elsewhere do not even write that the
search was not made*). So a `decision` issue of a sitting in which the
historian sat, whose resolution rests on a precedent the historian sourced,
ends with a `## Prior art` section, written in the session that files the
sitting:

- its first line names the sitting's file under `docs/panel/`, the historian's
  report under its `-reports/` folder or the synthesis's seat row where the
  sitting kept none, and the day every link below was re-read, `read
  YYYY-MM-DD`; where one card ratified several sittings the line names each of
  them, and the historian's having sat is asked of each (`records/prior-art`);
- then one line per precedent, in one of two forms and no other:
  `- **Taken from <language or project>**: [<reference>](<url>): <the rule or
  refusal adopted on their evidence>`, or `- **Reached first by <language or
  project>**: [<reference>](<url>): <what Heroes reached on its own and found
  already reached there>`;
- only a precedent the historian marked `verified` and the session re-read; a
  language the sitting merely compared itself with is not prior art, as it is
  not a borrowing on the site's thanks page;
- **each line says what the cited page itself states, and nothing of what Heroes
  did about it**, which the sitting's own file records. Measured when the
  backlog was written, 2026-10-11: of the 90 lines the four researchers
  proposed, three readers who had not seen their reports re-read every page and
  found **33 saying more than the page does** (a comparison put as an
  adoption, a mechanism the page never describes, a date it does not give), 57
  exact and none contradicted; a line is written from the page's own words after
  that second reading, a claim the page does not carry is cut, and a page that
  cannot be read is not cited.

**Where those conditions are not met the section does not exist**: no *not
searched* and no *nothing found*, because a sentence about an absence rests on
the searcher's vocabulary (CLAUDE.md § RUN IT) and the author asked for the
credit where it is owed and for silence everywhere else. The heading is
Rust's, the seventh section of the RFC template in `rust-lang/rfcs` (read
2026-10-07); Swift Evolution's closing *Acknowledgments* is what `**Origin:**`
already does for the people inside this project. What it cost to learn is
CL-082. The executor is `records/prior-art` (`tests/harness/credit.hero`),
which also holds a closed `feature`'s or `task`'s `## What was done` to its
card's commit, and the write hook asks it of an issue file as it is written
(`.claude/rules/verification.md` § A suite is the last judge).

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
are CL-067 and the decision issue of 2026-09-07.

## The issues — author instruction 2026-10-04

Meant as: *every defect, every decision and everything in the record should
already have the structure to be copied one file to one GitHub issue, keeping
their sequence and the commit that settled each*, and then: *everything goes into
a top-level `issues` folder, it is more orderly; we change the rules, toward a
final definition*. **Every item this project tracks is one file in `issues/`, and
one file is one GitHub issue.** `tests/harness/cards.hero` reads them, and
`records/cards`, `lists`, `defects`, `numbering`, `counts`, `verdicts`, `homes`,
`tagged`, `appended`, `prior-art`, `defects-cited` and `rendered` are this
section's executors.

**What an issue is**, the card's `kind`: a `defect`, a measured failure, its
class on its line (§ The lists); a `decision`, a question with the default it
leaves running, a sitting's ratification, or a decision taken, which is an issue
closed the day it is filed, as every entry of the old log is; a `feature`, a
milestone's item that changes what a Heroes program or the `heroes` command does
for somebody writing programs; a `task`, a milestone's item that serves the
project, its instruments, records, platforms and site. **Not an issue**: a
question of comprehension, `/learn`'s, which is a file of `docs/learn/` with no
card and is never transcribed (§ The lists; the author's *1a 2a* of
2026-10-05); a milestone, which is its page in
`docs/roadmap/milestones/` and its row in the ROADMAP's chain and becomes a GitHub
milestone named `M-<name>`; a sitting, a journal, a beat, the case law, a
measurement. Those are the account an issue links to.

**Where it lives, for ever**: `issues/<year-month>/<day>/<stamp>-<slug>.md`,
the folder being the day its name carries, and **nothing moves when an issue
closes**, so a link to the file, GitHub's included, stays good. A folder per day
because GitHub's web view and its contents API list 1,000 entries a folder
(GitHub's documentation, read 2026-10-04) and August 2026 alone held 1,086 files,
where a day held at most 319. Not a folder per kind: a kind is a classification
and is corrected, and a path that moved with a correction would break every link
to it. Not a bucket per thousand numbers: a counter every filing shares is the
number two lanes take at once, `records/numbering`'s own story.

**The name is the birth.** A new issue's stamp is the minute it is filed, so the
names read in order are the issues in the order they were born, which is the
order they are transcribed in. The slug is the title's words, lowercased, every
run of other characters one `-`, cut at a word boundary to at most 60
characters; a defect's opens `defect-NNN-`, its number, and a sitting's
ratification `panel-NNN-`. The files of `docs/records/done/`, `docs/records/log/`
and `docs/learn/` kept their names when they moved here on 2026-10-04 (the
questions went back to `docs/learn/` on 2026-10-05, keeping them again), a closed
entry's stamp being the day it closed, so a citation of one resolves by the rule,
the same name in its stamp's folder (`issue_redirect`); the one name two trees
shared, the log's `2026-09-23-1215-a-seats-copy-is-its-own.md`, became
`…-decided.md`. The 78 open defects, the open decision and the 51 open items of
the milestone pages took a birth stamp that day, the minute a position within
the day, as every rotation's is.

**The state is the item's box** (CL-032): an issue is open while it holds an
unticked item, `- [ ] `, whose line stands first under its card, and closed once
it holds none. A decision recorded as taken holds no box and is closed the day
it is filed. **Closing** ticks the box where it stands, adds what closed it (a
defect's *The repair* section), and fills `commit`.

**The card**, eight lines and a blank one, opens every file:

| field | value |
|---|---|
| `kind` | `defect`, `decision`, `feature` or `task` |
| `area` | a directory of `selfhost/`, `compiler` for a module at its top, a top-level area of the tree, or `none`; `AREAS` in `cards.hero`, held to the tree each run |
| `milestone` | `M-<name>` with a row in the chain, or `none`; an open `feature` or `task` names an open one |
| `filed` | the day it was filed: an open issue's is its name's day, a closed one's is not later, and where the body's `**Origin:**` names a day, it is that day |
| `commit` | the forty digits of the commit that settled it, a defect's being its `Repaired at`; `self` where that commit is the one writing the file, which git answers; `none` while it is open, or where the record does not say |
| `github` | `none`, or `https://github.com/heroes-lang/heroes/issues/<number>` once it is there |

**Three values state in a structured line what the item states in prose**, and
the card is their structured home: `commit` is one of the commits the body says
it was `Repaired at`, `filed` the day its `**Origin:**` names first, `milestone`
the milestone its first field names. `records/cards` holds them to agree, so
neither drifts from the other in silence. **The card is metadata**: it is
corrected where it stands, by a commit that says why. Below it a closed issue is
a record, append-only from the line under the card, and `records/appended` is
that rule's executor: the one exception to § A record is never rewritten has
one.

**Order**: `filed`, then the name, compared byte for byte. The promise is the
order, not the number: the repository is public, and an issue opened from
outside while ours are transcribed takes a number between two of them.

**What an issue becomes on GitHub**, the rule in one place. It is rendered by a
program of the harness and by no verb (panel 212's R1, ratified 2026-10-11):
`tests/harness/issues.hero`, run as `heroes run tests/harness/issues.hero --
<render|plan|push> ...`, whose rendering `records/rendered` holds over every
card (R2). Every reading below is deterministic, so one tree renders one set of
bytes:

- **the title** (R3's D1) is the file's `# ` heading where it opens with one;
  else a defect's bold first field with its `NNN — ` taken off; else the item's
  second field; else, for an item of one field, its first; else, for a row of
  the old log, its second field; **else the first paragraph of text below the
  card, its lines joined, a leading day taken off with the ` · ` after it where
  there is one**, since a card's lines are wrapped by hand and a first line is a
  fragment of a sentence (17
  cards reach this reading, measured 2026-10-11: 15 rows written `YYYY-MM-DD ·
  **...**`, one day with no ` | ` after it, one bold opening); a file of several
  items takes its first's. Its `**` markers are taken off; **a title that is a
  code span holding a path and nothing else takes the file's slug words
  instead**, and one that opens with such a span and goes on keeps its own
  words, the span with them (R3 said *opens with*; read literally it turned 52
  titles into slug words and many into broken phrases, the old slugs having
  dropped their code spans, measured 2026-10-11, so the coordinator narrowed it
  to the title a stranger cannot use, which is what the ergonomist found; the
  literal reading is the author's to restore); it is cut at a word boundary to
  at most 250 characters, never inside a code span, and a cut title stands whole
  as the body's line after the days, unless the body already holds that text
  under a heading (a title taken from the item's second field), so that no
  paragraph is read twice. **A sitting's ratification opens with its
  number too**, `Panel 212: <title>`, where its first field is `**panel NNN**`
  and the title does not open so already, since nothing else in the issue would
  say which sitting it ratifies. **A defect's title opens with its number as a word**, `Defect 235:
  <title>`, a file of several `Defects 018 to 021: <title>`, added only where
  the title does not open so already, because that number is the name the code,
  the tests and the commits use (6,925 citations of `defect NNN` under
  `selfhost/`, `runtime/`, `seed/` and `tests/`, 3,261 test files and 1,504
  commit subjects, measured 2026-10-11) and GitHub's number is an address no
  file knows until `github:` holds it (CL-083). **A defect's number** is the one
  its file name carries, else the `NNN — ` its first field opens with, and the
  title and the field `Defect` read the same one; a defect with neither takes no
  prefix and no field value (measured 2026-10-11 over 666 defect cards: 585 by
  the name, 34 by the first field, 47 with neither);
- **an item line splits on ` | ` outside a code span** (D2), a code span read
  as GitHub reads one, backtick runs paired by their length; four cards whose
  items write about backticks (237, 347, 414, 644) read as fewer fields than
  their writers meant, and are rendered as GitHub would show their lines, the
  coordinator's reading of 2026-10-12 against the alternative of counting single
  backticks, which mends one of the four and reads every other line as GitHub
  does not;
- **the labels** are `kind:<kind>`, `class:<class>` for a defect and
  `area:<area>`, where `area: none` and a defect with no class take no label
  (D5); the milestone is `M-<name>`, and `none` takes none. **The
  organisation's issue field `Defect`**, a text field visible to everyone
  holding the three-digit number, several separated by one space, so that
  `field.defect:235` finds it (GitHub's documentation, read 2026-10-11), is
  created once by an organisation administrator, `POST
  /orgs/heroes-lang/issue-fields`, the author's act, and **is set only where it
  exists when a run starts** (R6 item 5): the import cannot set it, so it is
  CL-083's second bridge and never one a run relies on. The two issues of the
  number issued twice, `014`, each hold it, and each body names the other;
- **the days**: the body opens `Filed YYYY-MM-DD · closed YYYY-MM-DD` on every
  route, and **GitHub's own dates are the card's days too**: the issue import
  endpoint, an unofficial preview, sets an issue's `created_at` and `closed_at`
  and its comment's from the request (measured 2026-10-11, `docs/measurements/041-the-import-endpoint-keeps-an-issues-own-days-and-an-imported-reference-leaves-no-backlink.md`;
  until then this line said a transcription cannot back-date them, and the
  author's instruction of 2026-10-06 asked it amended if the endpoint answered
  yes). `filed` is the card's; **`closed`** is the day a body line opening
  `Closed YYYY-MM-DD` gives (486 cards hold one, 2026-10-11), else the author date of the commit the card names,
  else the card's `filed`, and never a day before `filed`; `plan` prints which
  source each took (D3). A card holds days and no minutes, so **each day is
  written as noon UTC**, which every zone from UTC-12 to UTC+11 reads as that
  same day (the coordinator's reading of 2026-10-11, R3 being silent on the
  hour), the closing comment taking the `closed` instant;
- **the route**: an issue is imported whole, its days, labels, milestone, closed
  state and closing comment in one request, each import read `imported` before
  the next starts so that the numbers keep the files' order; the run's first
  request asks the endpoint again, and where it refuses the run stops: the REST
  route, which dates every issue the day it runs, is taken only on the author's
  word. Serial, at least one second between writes, and on a 403 or a 429 the
  `retry-after` seconds, else `x-ratelimit-reset`, else one minute, growing
  (R5; GitHub's documented limits are 80 content requests a minute and 500 an
  hour, so 2,017 imports are about four hours if an import counts as one,
  which is unrun);
- **the body**: the item under the headings of its kind's form in
  `.github/ISSUE_TEMPLATE/`, a defect's second field under *What happens* and
  its third under *Where to look*, **where its item has the three fields of the
  list's shape of 2026-09-07** (CL-066); where it has another number of fields,
  an older shape whose second field is often a day and whose third the whole
  account, its fields after the first stand under the form's first heading,
  one paragraph each, and no later heading names what its text is not. **This
  holds for every kind** (measured 2026-10-12: defects 566 of three fields and
  100 of another count, the other kinds 131 and 425); then a defect's `**Origin:**`, its `**Class:**` and
  *The repair* under headings of those names (D6); a decision's question,
  default, recommendation and verdict, and its `## Prior art` under *Prior
  art*, after *Origin* (§ And the record says whose idea it was); a feature's
  or a task's *What it delivers* and, once closed, its `## What was done` under
  *What was done*; any other paragraph under *Notes*; a card's second and later
  `- [ ]` lines become plain list lines, so GitHub shows no task progress (D7);
  then `Record:` and the file's permalink, pinned at a commit already on
  `origin/main`, given to the run as `--pin <hash>` and the same on every rerun
  (R5). **Every section stating a repair or a settling ends with the commit's
  hash, written bare**, since GitHub links a bare hash and not one in a code
  span (D6). **A card's `self` is the commit that wrote `commit: self` into
  it**, the earliest of `git log -S'commit: self' --format=%H -- <file>`; where
  that prints nothing or names a commit dated before the card's `filed`, the
  same asked with `--follow`, for a card moved after it was closed; and where
  that too prints nothing, the commit that brought the file to its path on the
  trunk, `git log --diff-filter=A --diff-merges=first-parent`, for a card that
  entered by a merge (D4). Measured 2026-10-11 over the 75 `self` cards: 73 by
  the first reading and 2 by the last (093 and panel 178's ratification, both
  brought by the merge `3088caa7`), none left. R3 had read D4 as the file's
  earliest commit, and that is the commit which OPENED a card closed later: it
  named the wrong commit for 25 of the 75, panel 212's ratification among them
  (its earliest commit `c1d49c62` filed it open, `b85be4ed` closed it); and
  `--follow` alone followed panel 191's card into an older file's history, nine
  days before it was filed. The guard is a fact of the card: a commit cannot
  settle an issue filed after it;
- **what a body must not do**: a bare `#NNN` and an `@word` are written inside a
  code span, since outside one the first links to our own issue of that number
  the moment it exists and the second notifies an account; a link to another
  project's issue or pull request is written with `redirect.github.com`, which
  leaves no backlink (GitHub's documentation, and measured on the REST route,
  041); and **a literal backslash and `u` followed by the four digits of a C0
  or C1 control character** (`0000` to `001f`, `0080` to `009f`) is written
  with its backslash as `&#92;`, a code span holding one becoming an HTML
  `<code>` element whose text is escaped as HTML, since GitHub stores such a
  sequence in caret notation on the import route and on the REST route alike
  (measured 2026-10-12: issues 1354 and 1643 held `\^A` and `\^[` for
  `\u0001` and `\u009b` after their import and again after a PATCH, while
  `\u007f` and `\u200b` stood as written; defect 675), and the oracle shows
  `<code>&#92;u0001</code>` displayed as the code span is. **The oracle** is `POST /markdown`, mode `gfm`, context
  `heroes-lang/heroes`, which writes nothing: no rendered body may hold an
  `issue-link` or a `user-mention` anchor;
- a closed issue whose card names a commit takes one comment, `Fixed by
  <commit>` for a defect and `Settled by <commit>` for the rest: GitHub links a
  bare hash to its commit, so no pull request is needed; a closed card whose
  `commit` is `none`, the record not saying, takes none;
- **every `defect NNN` and every `panel NNN` a body names becomes a link in a
  second pass**, to that defect's issue and to that sitting's ratification
  issue, where the prior art stands once; a run of its own after the last file
  is transcribed, because the issues are born in the order of their names, a
  closing text may name an issue born after it, and no address exists before
  its issue does (R9 item 6). So a repair a sitting ruled is linked to its
  ruling where its text names the sitting, and no field of the card is invented
  to say which sitting ruled it;
- **a run stopped between an import and the commit that fills its card's
  `github:` creates no second issue**: the address is written into the card
  once its import reads `imported` and before the next request, and a run
  killed between the two is reconciled by listing the repository's recent
  issues, a list and not a search, matched on the `Record:` permalink, an import
  that answered `pending` being read first (R5);
- `github:` in the file is the truth of the mapping, and **an issue that
  differs from its file is reported by `plan` and made to match it by `repair
  --yes`**, never the reverse (R10, amended by the author's answer of
  2026-10-12, `issues/2026-10/12/2026-10-12-1320-the-author-answers-four-the-repair-word-the-limit-the-two.md`):
  its title, body, labels and milestone rewritten where they differ, the body
  beyond its `Record:` pin, which stays the one it was imported with; its state
  made its card's, a card closed after its import closing the issue with its
  closing comment, dated the day `repair` runs, since the REST route sets no
  day; through the REST route, one request per change, paced as `push`; and
  every repaired issue read back, one GitHub still holds otherwise told and
  never asked again in silence. A day GitHub holds (`created_at`, `closed_at`)
  is reported and never changed, the REST route having no field for it. The
  first issues it met: two bodies the import stored otherwise than they were
  sent, `\^A` and `\^[` for a card's literal `\u0001` and `\u009b`
  (issues 1354 and 1643, defect 675);
- **a question of comprehension is not transcribed**, and since the author's
  *1a 2a* of 2026-10-05 it is not an issue at all (the *5a* of the same day had
  kept it out of GitHub: none is ever ticked, so on a public tracker each would
  stand open for ever);
- **a sitting is not a GitHub Discussion** (the author's *6b*, 2026-10-05): it
  stays a file of `docs/panel/`, linked from the decision issue that ratifies
  it.

How each value of the cards written on 2026-10-04 was derived, rule by rule and
with its count, is that day's decision issue,
`issues/2026-10/04/2026-10-04-2308-every-item-becomes-an-issue-file.md`.

## The lists

**Since 2026-10-04 four lists are views of `issues/`** (§ The issues): the
open defects are the open `defect` issues, the decision list the open `decision`
issues, a milestone's work the open `feature` and `task` issues naming it, and
the record the closed issues. Each item keeps the one shape the lists took on
2026-09-07 (CL-066), under its card:
`- [ ] **<first field>** | <what, in one line> | <where to look>`, its line
first, and an optional body indented four spaces opening with `**Origin:**` and
its date. `records/lists` is its executor.

**The learning list is the fifth, and not a view of `issues/` since
2026-10-05**: its questions are files of `docs/learn/<year-month>/<day>/`, one
each, in the same shape with no card above it, a question answered staying
ticked where it is, and `docs/learn/README.md` is its front page.

**The first field is what an instrument reads**, so it differs by kind: a
defect's `**NNN — <title>**`, the number its file's name carries after
`defect-`; the padded `**panel NNN**` of a sitting's ratification; the
`**M-<name>**` of a milestone's item, the milestone its card names; the origin
of a question. Put a sitting's number in a body instead and every pending
verdict reports as unqueued, silently.

**A defect's item ends its line with ` · **class: <name>**`** (`blocking`,
`adjacent`, `systemic` or `improvement`) and carries a `**Class:**` body line
with its date and reason, since 2026-10-02 (the author's *D1a*;
`.claude/rules/verification.md` § Bounded discovery). That body line opens
`**Class: <name>**, YYYY-MM-DD`, the line's own class and the day it was
classed in digits, then its reason, and `records/tagged` ages an `adjacent`
item from that day: it counts as `blocking` from the second tag placed since,
a tag counting when its day is later or the item stood open at it, the tag
judged included.

**A line that carries an archived or never-written path carries its date on that
same physical line**, because the citation check reads one line at a time.

## A live list is a preamble, a count and its items — author instruction 2026-09-16

**Since 2026-10-04 the lists' items are issues** (§ The issues), and what this
section binds is their front pages. `issues/README.md` carries **a very short
preamble** alone, under `PREAMBLE_CEILING`, and no item, no banner and no count:
the open counts are derived from the issues' cards. A milestone's page under
`docs/roadmap/milestones/` carries its reasoning alone, which `/step` § 1 calls the
thing that must not be lost, under no ceiling and with no item. `docs/ROADMAP.md`
carries a very short preamble, then the two open counts, then its tables. **No
other story goes in any of them**, and a story already there is MOVED rather
than deleted — to a decision issue, to the milestone's journal, or to the issue
the story is about, which is where a thing that happened belongs anyway.

Until that day `docs/work/DECIDE.md` and the milestone files carried the
preamble, the `**OPEN: N**` banner and the items, and `docs/work/DEFECTS.md` was
the defects' front page (2026-09-16 to 2026-10-04); the paragraphs below are
that rule's reasoning, which the issues kept.

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
That is § A tree of entries' lesson — an index is a second place where truth
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
its own issues under `issues/`, its own beat — every one of them a file nobody
else is writing.

- **The worktree lives inside this tree, under `.claude/worktrees/<lane>`, and
  never in a folder beside the project** (author instruction 2026-09-28, meant
  as: the lanes go inside the project's `.claude/worktrees`, not outside it in
  parallel folders; write it so it is known for good). **Until that day this
  bullet said the opposite**, *outside this tree, never under
  `.claude/worktrees/`*, for two measured reasons: a nested checkout made a
  tree-walking check report another tree's milestone names as this one's
  (2026-08-12), and the mere existence of that directory hid a citation defect
  for three red CI runs, on the one machine that could not see it (2026-09-07).
  Both were repaired in the instruments before the instruction was given: the
  record walks ask git which files are the project's (`shell.project_files`),
  the citation check asks `git check-ignore`, and `.claude/worktrees/` is in
  `.gitignore`. **Measured the day it changed**, with a worktree of the trunk at
  `.claude/worktrees/lane-rule-probe`: `git check-ignore` names it ignored,
  `git status` does not list it, and records 24, layout 4, order 3, canonical 2,
  surface 332, spec 20 and probe 24 read the same counts as without it, each 0
  failed. **What a nested lane still owes**, because the two old failures were
  a walk by the filesystem: any `find`, `tar` or copy of the tree excludes
  `.claude/worktrees` by name, the archive sent to Linux arm64 and the Windows
  box first, or it ships every lane inside it. A worktree has its own index,
  which is why it is safer than a second session here: CL-041 and CL-070 are
  both a shared index carrying away somebody's work.
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

**A lane holds a CLUSTER, and works it in sequence** — author instruction
2026-09-29, choosing between the two shapes put in front of them (CL-079). On
the night of 2026-09-28 to 29 five defects whose repairs shared
`open_line.hero` and `grammar_expr.hero` ran as five lanes, and cost 11 merges
for 10 repair commits, 3 of them with `seed/heroes.c` in conflict; over the two
weeks before, 21 of 58 merges carried that conflict. So: **one lane per group of
defects whose repairs share files, the defects taken one after another inside
it; parallel lanes only for groups whose file sets are disjoint**, which `git
diff --stat` against the base shows before the second lane opens. A widening
found beside a defect is the lane's next item, not a filing on the trunk. The
lane is the batch of `.claude/rules/verification.md` § The batch: repairs gated
by their cases, the full net once at its close. Four more rules from the same
reading, and a fifth from 2026-10-06:

- **`git merge --ff-only` first; a merge commit only when it refuses.** Three of
  the seven trunk merges of that night were made over a trunk that had not
  moved. A fast-forward rewrites no hash, so *merge, not rebase* is untouched.
- **The trunk is merged into the lane once, before the batch's gate**, and
  `seed/heroes.c` is never resolved by hand: take one side, build, regenerate,
  verify the fixpoint by `cmp`. The seed is regenerated at the batch's close
  and travels in the closing commit; a repair commit does not carry it.
- **A repair commit's body is about fifteen lines**: the class, the cases, the
  gate line. The story is written once, in the defect's issue at the batch's
  close, under its *The repair* section. Measured on that night: repair bodies of 101, 114 and 48
  lines, and `docs/work/DEFECTS.md` at 2,936 words for five items.
- **A lane is taken over only once its agent is known to have stopped**
  (journal 061: a lane resumed an hour before was still working when the
  coordinator committed, and the trunk's seed was not its source's fixpoint).
- **A lane's committed repairs are merged into the round as soon as they are
  committed, several lanes together when several are ready, never held until
  the lane is idle** (author instruction 2026-10-06, meant as: *put this
  merging of lanes down as an optimisation*, given on the question *can you
  not merge more lanes?*). Merging a branch is not taking a lane over: the
  merge takes the branch's commits and nothing in its worktree, so a lane whose
  agent has moved to its next item gives the round its finished repairs at
  once. Measured the night it was given: lane b13-zero401's two commits for
  defect 401 merged at 23:27 while its worktree held 412's uncommitted repair,
  and the round read records 28, layout 5, check 566 and full 16, all 0
  failed, on 401 beside the other lanes' merges. The lane's next item begins by
  merging the round into its own branch, so the two meet at a merge rather than
  at the gate.

**A lane's agent starts no paid run its brief does not name** (author
instruction 2026-09-30): no `claude -p` session, API call or cloud run of its
own initiative. One it finds worth running goes in its report, with its size,
for the coordinator to decide.

**And one thing measured NOT to help, 2026-09-29**: copying the trunk's
`build/` cache into a fresh lane (`cp -c -R`, 0.27 s for 316 entries) bought
nothing, `heroes build selfhost/main.hero` reading 41.6 s of CPU cold and 43.2 s
after the copy. A fresh lane pays one cold build of under a minute; the cache
is the lane's own from then on.

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
  until that is decided — it is an outward-facing file. The question is filed as
  a decision issue. Until it is settled, a second lane works with its row left
  `scheduled` and says so in its commits.

## Where a session's output goes

A conversation whose work is questions, with no file of code, spec or design
modified, **writes no note of its own** (CL-053). What it settled is an issue it
ticks, or a decision issue closed the day it is filed; what it left open is an
open issue, a `decision` or its milestone's `feature` or `task`; a concept it
explained is an entry in `docs/learn/glossary/`; a question worth re-asking is a
file of `docs/learn/`; a change to the language is a panel. Every one but the
glossary's and the question's is a file of `issues/` (§ The issues). The path
between them is the git history.
