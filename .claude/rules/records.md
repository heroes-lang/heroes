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

`docs/panel/`, `docs/records/journal/`, `docs/measurements/`, a closed issue
on GitHub (§ The issues), `docs/records/book/beats/`,
`docs/records/contract/`, `tests/golden/`, every commit subject and the twelve
legacy tags are append-only. Where a sentence in
one has since been falsified, the correction is **added underneath**, with its
date: a measurement that was right when it was taken is history, and a record
that quietly loses its inconvenient half is worth less than none.

**Appending to a dated record uses that record's vocabulary**, with the new name
in parentheses on first use. `tests/harness/suite_records.hero` knows which
trees are records and treats them accordingly, and `records/appended` is what
makes this section a rule rather than a sentence: it went unperformed until
2026-09-12, and `5bf3ebdb` had already removed a line from the record with every
suite green.

## A tree of entries: one file each, and no map

**A new entry is a NEW FILE**, `YYYY-MM-DD-HHMM-<slug>.md`, in
`docs/records/book/beats/` or in `docs/learn/`'s folder of its day. Two
sessions then write two files and never one line, which is the whole reason
the shape exists. The minute in the name is a
POSITION for an entry written before its record became a tree: a historical
entry has no clock reading, and two entries of one day still have an order.
**No directory index**, because an index is a second place where truth lives
and the only tabulated one here drifted for nine closes in silence.
`records/entries` holds the beats' names.

**No map either, since 2026-10-05** — author instruction, meant as: *I prefer
cleanliness for the future to keeping the historical legacy*. From 2026-09-12
the three files whose entries became trees, `docs/records/book/beats.md`,
`DESIGN-LOG.md` (later `docs/design/DESIGN-LOG.md`) and `docs/work/DONE.md`,
stayed behind at the heights they had, one row per entry, so that a `<file>:NNN`
citation still landed where it always had (CL-037: a line number is the one
citation shape no instrument can see). That day all three were deleted. Every
living citation by line was resolved first to the file it named; a living file
names an entry by its path from then on, which `records/citations` checks. The
citations inside dated records read against the maps at `c911dd61`, the last
commit that held them, as the specification's line citations read against
`ce0eda8f` (`.claude/rules/spec-shape.md`). **The rule this leaves**: when a
record's shape changes, the citations into it that a living file holds are
rewritten to the new shape in the same change, and no file is kept only so an
old citation still resolves.

## The top of `docs/`: one document, then collections

Author's answers of 2026-10-05, on the coordinator's reading of `docs/`. **A
file at the top of `docs/` is the living document the contract names there,
`docs/design.md`**, and since 2026-10-24 **the ROADMAP stands at the root**,
`ROADMAP.md`, beside `README.md` and `CLAUDE.md` (the author's instruction of
2026-10-20, meant as: *move it from docs to the root, it is the one file I open
to follow the project*). Until that day it was the second document at the top
of `docs/`, and the root had kept `README.md` and `CLAUDE.md` alone since the
author's instruction of 2026-09-12; `MOVED` in `tests/harness/suite_records.hero`
keeps the old path followable. **Every folder is a collection of one kind**: `roadmap/` what a row of the chain cannot hold, with
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
ends with a `## Prior art` section, written into its draft by the session that
files the sitting:

- its first line names the sitting's file under `docs/panel/`, the historian's
  report under its `-reports/` folder or the synthesis's seat row where the
  sitting kept none, and the day every link below was re-read, `read
  YYYY-MM-DD`; where one issue ratifies several sittings the line names each
  of them, and the historian's having sat is asked of each;
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
CL-082. The executor is `tests/harness/credit.hero`, which `file` asks of a
draft before it opens the issue (§ The issues).

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
`git diff vA vB -- spec/heroes-spec.md` is not empty and `Z` when it is; from
`1.0.0` the same holds for `Y` and `Z`, and `X` moves only with a release that
breaks the compatibility promise and says which programs it breaks (panel 232's
R11, provisional, author ratification pending). The CI
is the instrument, and no binary is uploaded. The reasoning and the six choices
are CL-067 and the decision issue of 2026-09-07.

## The issues

**Every item this project tracks is one issue of `heroes-lang/heroes`, and the
issues live on GitHub alone** (the author's answers of 2026-10-12). From
2026-10-04 each was one file of `issues/`, and issues 1 to 2,029 were
transcribed from those files (panel 212); the folder left the tree with
M-issue-files' last work, and a dated record that cites one of its files reads
it against the last commit that held it (§ A tree of entries). **Opening,
commenting on, closing and correcting an issue goes through the program below
and is asked once** (CLAUDE.md § Hard stops); a milestone, a label and every
other write to GitHub stay asked every time.

**What an issue is**, its `kind:` label: a `defect`, a measured failure, with
its class (§ The lists); a `decision`, a question with the default it leaves
running, a sitting's ratification, or a decision taken, closed the day it is
opened; a `feature`, a milestone's item that changes what a Heroes program or
the `heroes` command does for somebody writing programs; a `task`, a
milestone's item that serves the project, its instruments, records, platforms
and site. **Not an issue**: a question of comprehension, a file of
`docs/learn/` (§ The lists); a milestone, which is its page in
`docs/roadmap/milestones/`, its row in the ROADMAP's chain and the GitHub
milestone `M-<name>`; a sitting, which stays a file of `docs/panel/` linked
from the decision that ratifies it and is never a GitHub Discussion (the
author's *6b*, 2026-10-05); a journal, a beat, the case law, a measurement.
Those are the account an issue links to.

**The labels**: `kind:<kind>` on every issue; `class:<class>` on a defect;
`area:<area>` where the area is not `none`, an area being a directory of
`selfhost/`, `compiler` for a module at its top, or a top-level area of the
tree (`AREAS` in `tests/harness/cards.hero`); and the GitHub milestone
`M-<name>` where the issue names one with a row in the chain, an open
`feature` or `task` naming an open one.

**An issue is born as a draft in the card's shape, outside the tracked tree**,
in the session's or the lane's scratch folder under `.claude/worktrees/`, which
git ignores. The program checks it, renders it, opens the issue and removes the
draft, so no copy of an issue stands in the tree beside it: a second place
where its truth lives is one that drifts (§ A tree of entries). The draft is
what a file of `issues/` was until 2026-10-12:

- **the card**, eight lines and a blank one: `---`, then `kind`, `area`,
  `milestone` (`M-<name>` or `none`), `filed` (the day it is filed, the day its
  `**Origin:**` names), `commit` (`none` while it is open, else the forty digits
  of the commit that settled it) and `github` (`none`), then `---`;
- **the item**, one line of three fields,
  `- [ ] **<first field>** | <what, in one line> | <where to look>` (CL-066),
  a defect's ending ` · **class: <name>**`; then an optional body indented four
  spaces opening with `**Origin:**` and its date, a defect's holding its
  `**Class:**` line (§ The lists), a sitting's decision its `## Prior art` where
  one is owed (§ And the record says whose idea it was);
- **a decision taken** is drafted after the commit that applies it, its item
  ticked, `- [x]`, and its card's `commit` that commit, and `file` closes it as
  it opens it.

**The program's words**, `heroes run tests/harness/issues.hero -- <word> ...`
from the repository's root: a program of the harness and no verb of `heroes`
(panel 212's R1, ratified 2026-10-11), every request to GitHub made through
`gh`, which holds the token:

- **`draft <draft.md>`** checks the draft as `file` does before its first
  request and prints the issue `file` would open, its title, labels,
  milestone, state and body, asking GitHub nothing: an issue is public the
  moment it exists, so its writer reads it here first (the coordinator's
  addition of 2026-10-12, on the first defect filed this way);
- **`file <draft.md>`** checks the draft, renders it by the rule below, opens
  the issue with its labels and milestone, removes the draft and prints the
  issue's URL; a defect's title opens `Defect <n>:`, `<n>` its issue number;
- **`close <number> <closing.md>`** appends the closing text to the issue, a
  defect's under `## The repair` and every other kind's under
  `## What was done`, ending with the commit's hash; posts `Fixed by <commit>`
  for a defect or `Settled by <commit>` for the rest; and closes it as
  `completed`;
- **`comment <number> <text.md>`** adds a comment;
- **`edit <number> <draft.md>`** makes an open issue's title, body, labels and
  milestone those of a corrected draft;
- **`open [--kind <kind>] [--milestone M-<name>] [--class <class>] [--json]`**
  lists the open issues, the one reader of the lists (§ The lists) and of the
  skills;
- **`link [--dry-run]`** makes every `defect NNN` and `panel NNN` a body of
  the project's own names a link (below, and `tests/harness/issue_links.hero`),
  each body that changes written by one `PATCH` and read back, a rerun writing
  nothing.

**A closed issue is a record** (§ A record is never rewritten): a sentence of
it since falsified is corrected underneath, by `comment` and with its date,
and never by `edit`, which corrects an open issue. **`link` is the one write
into a closed body**, panel 212's R9 item 6, ratified before this rule: it
makes words already there a link and changes no word.

**The numbers** (the author's fifth answer of 2026-10-12, CL-083): defects 001
to 677, filed while the issues were files, keep theirs, which their titles open
with, `Defect NNN:`, and which the organisation's issue field `Defect` holds
where it exists; `014`, issued twice, stands on two issues that name each
other. A defect filed since is named by its GitHub issue number, every one
above the numbers GitHub held that day, so the two series never meet. Either
way the number is the defect's name for ever, cited `defect <n>` by the code,
the tests and the commits; a new defect's draft has no number, its first field
being its title alone, and `file` gives it the issue's. **The order is
GitHub's**: an issue takes the next number, ours and a stranger's alike, and
issues 1 to 2,029 took theirs in the order of the files' names.

**What an issue becomes on GitHub**, the rendering rule in one place.
`tests/harness/issues.hero` renders every draft by it, as it rendered issues 1
to 2,029 from their files, and every reading is deterministic, so one draft
renders one set of bytes:

- **the title** is the draft's `# ` heading where it opens with one; else a
  defect's bold first field, its `NNN — ` taken off; else the item's second
  field; else, for an item of one field, its first; else, for a row of the old
  log, its second; else the first paragraph below the card, its lines joined
  and a leading day taken off with the ` · ` after it, since a line wrapped by
  hand is a fragment of a sentence. Its `**` markers are taken off; a title
  that is a code span holding a path and nothing else takes the slug words of
  the file's name, a path being a title a stranger cannot use; it is cut at a
  word boundary to at most 250 characters, never inside a code span, and a cut
  title stands whole as the body's line after the days, unless the body holds
  that text under a heading. **A sitting's ratification opens `Panel NNN: `**,
  where its first field is `**panel NNN**`, since nothing else would say which
  sitting it ratifies; **a defect's opens `Defect <n>: `**, a file of several
  `Defects 018 to 021: `, where the title does not open so already, because
  that number is the name the code, the tests and the commits use (CL-083);
- **an item line splits on ` | ` outside a code span**, a code span read as
  GitHub reads one, backtick runs paired by their length, so a line renders as
  GitHub would show it;
- **the days**: the body opens `Filed YYYY-MM-DD`, and `Filed YYYY-MM-DD ·
  closed YYYY-MM-DD` once closed, `closed` being the day a body line opening
  `Closed YYYY-MM-DD` gives, else the author date of the commit the card names,
  else `filed`, never before it. A transcribed issue holds those days as
  GitHub's own, each written as noon UTC, which every zone from UTC-12 to
  UTC+11 reads as that day: the import endpoint takes them from the request
  (`docs/measurements/041-the-import-endpoint-keeps-an-issues-own-days-and-an-imported-reference-leaves-no-backlink.md`);
  an issue `file` opens is dated by GitHub the day it runs;
- **the body**: the item under the headings of its kind's form in
  `.github/ISSUE_TEMPLATE/`, a defect's second field under *What happens* and
  its third under *Where to look* where the item has the three fields of
  2026-09-07's shape (CL-066), and every field after the first under the form's
  first heading, one paragraph each, where it has another count, so that no
  heading names what its text is not; then a defect's `**Origin:**`,
  `**Class:**` and *The repair* under headings of those names; a decision's
  question, default, recommendation and verdict, and its `## Prior art` under
  *Prior art*, after *Origin*; a feature's or a task's *What it delivers* and,
  once closed, *What was done*; any other paragraph under *Notes*; a second or
  later `- [ ]` line as a plain list line, so GitHub shows no task progress;
  then, for an issue transcribed from a file, `Record:` and the file's
  permalink, pinned at a commit already on `origin/main`. **Every section
  stating a repair or a settling ends with the commit's hash, written bare**,
  since GitHub links a bare hash and not one in a code span;
- **what a body must not do**: a bare `#NNN` and an `@word` are written inside
  a code span, since outside one the first links to our own issue of that
  number and the second notifies an account; a link to another project's issue
  or pull request goes through `redirect.github.com`, which leaves no backlink;
  and a literal backslash and `u` followed by the four digits of a C0 or C1
  control character (`0000` to `001f`, `0080` to `009f`) is written with its
  backslash as `&#92;`, a code span holding one becoming an HTML `<code>`
  element whose text is escaped as HTML, since GitHub stores such a sequence in
  caret notation (defect 675). **The oracle** is `POST /markdown`, mode `gfm`,
  context `heroes-lang/heroes`, which writes nothing: no rendered body may hold
  an `issue-link` or a `user-mention` anchor;
- **every `defect NNN` and every `panel NNN` a body names is a link**, to that
  defect's issue and to that sitting's ratification, where the prior art stands
  once, so no field is invented to say which sitting ruled a repair; a second
  pass after the last file, since a closing text may name an issue born after
  it (R9 item 6). **It ran on 2026-10-12, after the folder had left the tree**
  (the transcription's words never ran it, which the coordinator found that
  evening reading 1,594 bodies that named one and linked none), by the word
  `link`, on the author's yes the same evening. **A sitting no single
  ratification issue answers links its own file**, `docs/panel/<NNN>-<slug>.md`
  at a commit on `origin/main`: 71 of 210 sittings had one titled
  `Panel NNN:`, and R9 was silent on the rest (the coordinator's reading,
  where the alternative was to leave them text). A defect two issues answer
  (014), a number nobody holds, a day after the word, code, and what a body
  already links stay as they are. **The project's own issues alone, since
  2026-10-14** (defect 2063): an issue is the project's where GitHub calls its
  author `OWNER`, `MEMBER` or `COLLABORATOR` and its body opens `Filed
  YYYY-MM-DD`, the program's rendering (`github_issues.by_project`), so a
  stranger's body gains no link, its words being its writer's, and a
  stranger's issue answers no name but the defect number that is its own: a
  re-report titled `Defect 235:` leaves 235 linked to its issue, and a
  stranger's `Panel 216:` is never where `panel 216` links;
- **the route** of the transcription (R5): each file imported whole, its days,
  labels, milestone, closed state and closing comment in one request, read
  `imported` before the next so that the numbers keep the files' order, serial
  and paced by GitHub's limits; **a run stopped** between an import and the
  card's `github:` reconciled by listing the recent issues on their `Record:`
  permalinks, never by a second issue; and, while the files were the truth, an
  issue that differed from its file made to match it by `repair --yes` (R10, as
  the author amended it on 2026-10-12,
  https://github.com/heroes-lang/heroes/issues/2023).

## The lists

**Four lists are views of GitHub's issues** (§ The issues), each read with
`open`: the open defects, `--kind defect`; the decisions, `--kind decision`; a
milestone's work, the open `feature` and `task` issues of `--milestone
M-<name>`; and the record, the closed issues. **The learning list is the
fifth, and is not an issue** (the author's *1a 2a* of 2026-10-05): its
questions are files of `docs/learn/<year-month>/<day>/`, one each, in the
item's shape with no card above it, a question answered staying ticked where it
is, and `docs/learn/README.md` is its front page. It stays off GitHub by the
author's *5a* of the same day: none is ever ticked, so on a public tracker each
would stand open for ever.

**The first field is what an instrument reads**, so it differs by kind: a
defect's `**NNN — <title>**`, its number, or its title alone in a new draft;
the padded `**panel NNN**` of a sitting's ratification, which titles the issue
`Panel NNN:` and is how a pending verdict is found queued; the `**M-<name>**`
of a milestone's item, the milestone its card names; the origin of a question.
Put a sitting's number in a body instead and its pending verdict reads as
unqueued, silently.

**A defect's item ends its line with ` · **class: <name>**`** (`blocking`,
`adjacent`, `systemic` or `improvement`), which becomes its `class:` label, and
carries a `**Class:**` body line, since 2026-10-02 (the author's *D1a*;
`.claude/rules/verification.md` § Bounded discovery). That line opens
`**Class: <name>**, YYYY-MM-DD`, the class and the day it was classed in
digits, then its reason, and `records/tagged` ages an `adjacent` defect from
that day: it counts as `blocking` from the second tag placed since, a tag
counting when its day is later or the defect stood open at it.

**A line that carries an archived or never-written path carries its date on that
same physical line**, because the citation check reads one line at a time.

## A live list is a preamble and its items

Author instruction 2026-09-16. **The lists' items are GitHub's issues** (§ The
issues), and what this section
binds is the pages of the tree that front them. A milestone's page under
`docs/roadmap/milestones/` carries its reasoning alone, which `/step` § 1 calls
the thing that must not be lost, under no ceiling and with no item.
`ROADMAP.md` carries a very short preamble, then its tables, and no count
of the issues, GitHub's counts being the truth (the author's sixth answer of
2026-10-12). **No other story goes in either**, and a story already there is
MOVED rather than deleted: to a decision issue, to the milestone's journal, or
to the issue the story is about, which is where a thing that happened belongs
anyway.

The reason is what these pages are for: one is opened by somebody about to
attack an item, the ROADMAP by somebody asking what is next, both to find a row.
On the day this rule was given the defects' front page held 28 lines and 982
words of history in front of its items, one sentence of it stating the next
number to issue twice with two values; and a count a page carries beside the
list it counts drifts, as the ROADMAP's did that day, *open defects 0* against
the list's own 3 (CL-064). That is § A tree of entries' lesson, an index is a
second place where truth lives, arriving in the one document the author opens
to see where the project is.

## Two habits that come from being one checkout among several

**Re-read the chain and the decisions immediately before writing a scheduling fact**,
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

**Open sessions keep each other informed** (author instruction 2026-10-12,
meant as: *always talk with the other open chat, to stay aligned*). Before
touching a file another open session may touch, before filing or closing an
issue, and before a push, a session tells the open sessions what it will touch
and when, through `ListAgents` and `SendMessage`; it answers what they send; it
says in its own conversation what was agreed; and it never asks another session
to do what its own permissions refuse, a refusal binding the work and not only
the session it was met in. Two sessions that do not speak meet in the shared
index or on the trunk, which is what CL-041 and CL-070 cost.

## Working in lanes, since 2026-09-12

A **lane** is one milestone, in one detached worktree, held by one session. The
rotations above are what make it possible: a lane writes its own milestone file,
its own beat and its own issue drafts, every one of them a file nobody else is
writing, and the issues it opens each take GitHub's next number.

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
  **The side, since panel 222 landed (2026-10-17)**: the seed that defines
  `hero_program_sources`, the trunk's, never the seed of a lane opened before
  the landing, whose compiler would build a stage 2 that cannot link; the net's
  `seed` suite asks it. And the seed is regenerated only where the two-stage
  build goes red or a release closes, M-bounded-seed's rhythm
  (`.claude/rules/verification.md` § The batch).
- **A repair commit's body is about fifteen lines**: the class, the cases, the
  gate line. The story is written once, in the defect's issue at the batch's
  close, by `close` as its `## The repair`. Measured on that night: repair
  bodies of 101, 114 and 48 lines, and the defects' list at 2,936 words for
  five items.
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
**Each lane opens its own row.** More than one `**OPEN**` row may stand in
§ The chain since the author's decision of 2026-09-12, one per lane holding a
milestone, and `site/src/lib/chain.ts`, which asserted one until that day,
names every open row, since a page naming the first of two is wrong about the
second.

## Where a session's output goes

A conversation whose work is questions, with no file of code, spec or design
modified, **writes no note of its own** (CL-053). What it settled is an issue it
closes, or a decision issue closed the day it is opened; what it left open is an
open issue, a `decision` or its milestone's `feature` or `task`; a concept it
explained is an entry in `docs/learn/glossary/`; a question worth re-asking is a
file of `docs/learn/`; a change to the language is a panel. Every one but the
glossary's and the question's is an issue on GitHub (§ The issues). The path
between them is the git history and the commits each closing names.

**And what a session hands to the next is that list of open issues, never a note of
the assistant's memory** (author instruction 2026-10-17, meant as: *I have told you a
thousand times that I do not want the memory used, because I use more than one
computer*). A local note lives on one machine and a session opened on another never
sees it, so the work a session leaves for the next is filed on GitHub before it ends,
each item an issue of the milestone that owes it, and a session starts from `open`, never
from a memory file. On the evening it was given, the coordinator had written the next
session's start into the memory's index; issues 2132 and 2133 are where it went instead.
