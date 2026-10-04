---
kind: decision
area: records
milestone: M-issue-files
filed: 2026-10-04
commit: self
github: none
---

# Every item becomes an issue file, one for one GitHub issue, in `issues/`

2026-10-04, written at 23:08 by the clock (`date`), in lane `lane-issue-cards`
from the trunk at `99a67630`. The author, that evening, in four messages meant
as: *I want every defect, every decision and everything now in the record to
have, already, the structure to be copied one file to one GitHub issue, with a
categorisation thought through like the defects' classes, covering features and
improvements too*; then, on the plan put to them, *keep the sequence of the
issues, keep the link to the commit that settled each, use or change the issue
template we have on GitHub, and do it while the other chat closes defects*;
then, on a folder: *why not a top-level `issues` folder? it is more orderly;
we change the rules, but toward a final definition*. Recorded as a reading.

**What was decided**, and its one home is `.claude/rules/records.md` § The
issues:

- every item is one file under `issues/<year-month>/<day>/`, its path fixed for
  life: open and closed stand side by side and closing ticks the box where it
  stands, so a link from GitHub never breaks;
- every file opens with a card of six values, `kind` (`defect`, `decision`,
  `feature`, `task`, `learn`), `area`, `milestone`, `filed`, `commit` and
  `github`;
- a new file's name is its birth, so the names read in order are the sequence
  the author asked to keep; `filed` keeps it for the files that kept older names;
- `commit` names the commit that settled the issue, whole, so no pull request
  is needed: GitHub links a bare hash;
- `improvement` stays the fourth class of a defect (the author's answer to the
  plan's second question), and `feature` and `task` divide a milestone's work.

**Why a folder per day, and not the two shapes the author asked about.** GitHub
lists 1,000 entries a folder in its web view and in its contents API (its
documentation and its community answer, read 2026-10-04), and August 2026 alone
held 1,086 of the files that moved, so a folder per month would have hidden 86
of them; a day held at most 319. A folder per thousand numbers, the author's
second suggestion, needs one counter every filing shares, which two lanes take
at once and merge cleanly and wrongly, the story `records/numbering` was written
for; put to the author with that reason the same evening, and the day stands
until they answer. A folder per kind would move a file whenever its kind is
corrected.

**What moved, measured** (step 2, `49341422`): 777 files of `docs/records/done/`,
759 of `docs/records/log/` and 436 of `docs/learn/` kept their names; 78 open
defects took a birth stamp, `<day>-<position>-defect-NNN-<slug>.md`; the open
decision and the 51 open items of the milestone pages became files of their
own; `docs/work/DECIDE.md`, `docs/work/DEFECTS.md` and `docs/learn/README.md`
retired into `issues/README.md` and the rules. 2,102 issue files, every moved
one byte-identical below its card to its text at `99a67630`, every rename at
100% similarity.

**How the cards were derived** (step 3, `dd1c515e`), with the count of each
rule, and per file in `docs/measurements/039-every-card-and-the-rule-behind-each-of-its-values.md`:

- **kind**: from the tree for the log (759 decisions), the learning list (436),
  the open defects (78) and the decision list (1); for the 777 closed entries,
  five rules in order (a defect's shape 270, a sitting or a ratification 264, a
  verdict word 97, a comprehension question 25) and 121 read one by one, with a
  rule written down: a feature changes what a program or the command does for
  somebody writing programs, a task serves the project. Two records about a
  defect's decision read as the defect's own, and the first `records` run found
  them; their cards say `decision`.
- **filed**: the day the `**Origin:**` names (381); the earliest day an entry's
  line states, never after its stamp (462); never after the commit that settled
  it (39); the stamp's day otherwise (1,216). Measured first: not one closed
  entry's `**Origin:**` names a day after its stamp, and every `learn` file's
  equals its stamp.
- **commit**: `Repaired at` (91); the commit that wrote the file into the record
  (339); for the 1,111 entries the 2026-09-12 rotations wrote, the earliest
  commit that added the line before them (971), read past a heading the
  2026-08-26 move folded into the line as its first field (123), or the move
  itself where nothing earlier held the text and the entry is dated the move's
  day (16). **The blind review of the plan found the trap this avoids**:
  `git log -S` answers `1945659f`, a bulk move, for 281 of those entries. 1,535
  of the 1,536 closed entries name a commit; one says `none`.
- **milestone**: an item's first field where it names a row of the chain, an
  old `M<n>` through the chain's tag column; the page an item stood on.
- **area**: the first path into the code a line names, else the first path into
  the records, else `none` (298).

**The instrument** (step 1, `7c484eb8`): `tests/harness/cards.hero` and
`records/cards`, and every record check rewritten onto `issues/`. What it
corrected in itself on the first run over the real tree: `english` and
`citations` asked a path whether it was a record, and a path in `issues/` does
not say whether its issue is closed; and the defect register read a number out
of a name that only mentioned it.

**What it reverses**: the defect list's directory of the morning
(`2026-10-04-0206-the-defect-list-becomes-one-file-per-defect.md`, the
author's *5a*), whose files are now issues, and the five lists of CLAUDE.md
§ 3, which are now views of one folder. Neither is deleted: both entries keep
their words.

**2026-10-05, the author answered *2a*: the folder stays the day**
(`issues/2026-10/05/2026-10-05-0027-the-author-answers-1c-2a-3a-4a-5a-6b-7a.md`).
