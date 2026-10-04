---
kind: decision
area: records
milestone: none
filed: 2026-10-04
commit: 57ce29ad52cd2763ea46c23b718b94b3754a14e0
github: none
---

# The defect list becomes one file per defect: `docs/work/defects/`, with `DEFECTS.md` its front page

2026-10-04, written at 02:06 by the clock (`date`), in lane b8-defects from
batch 8's round tree at `a989ddf1`. The author, 2026-10-03, meant as: *since
DEFECTS.md holds a great many defects, restructure it, one defect one file, so
that we have no conflicts; and perhaps keep in DEFECTS.md only the IDs, as a
register, as was done for other things.* The coordinator's design was put as
*5a* and the author answered *5a* (lane b8-defects' brief, 2026-10-04).
Recorded as a reading.

**Why.** Batch 8's four lanes each edited the one file, and the merge of their
round had two conflicts, at the list's end and on its `**OPEN**` banner, both
resolved by hand. A file per defect is two lanes writing two files and never
one line; `docs/learn/` took the same shape on 2026-09-12 for the same reason.

**What moved.** The 52 open items of `docs/work/DEFECTS.md` became 52 files
`docs/work/defects/NNN-<slug>.md`, each holding its item exactly as the list
held it, its line first and its body indented, carried byte for byte: the
files, joined by the one blank line that stood between two items, with the
old preamble, banners and count, rebuild `git show HEAD:docs/work/DEFECTS.md`
(74,510 bytes) exactly, checked before writing and again from the disk. The
slug is the title's words, lowercased, every run of other characters one `-`,
cut at a word boundary to at most 60 characters. An empty `.gitkeep` keeps the
directory in every clone on the day it holds no defect, which is the goal.

**What the register became.** No index of IDs: a register every lane edits is
the same conflict made smaller, and `.claude/rules/records.md` § A rotated
record refuses a directory index. The directory's listing is the register;
`records/defects` holds its names (the directory there, `NNN-<slug>.md`
directly in it, no number on two files, which two lanes taking one number
would merge without a conflict), and the open count is the number of files.

**`DEFECTS.md` keeps its path as the front page**: what a defect file is, how
one is filed (a new file, with the number the coordinator issues, one above
the highest in the directory and the record) and how one closes (`git mv` into
`docs/records/done/` as `YYYY-MM-DD-HHMM-defect-NNN-<slug>.md`, ticked, with its
*The repair* section, so its history follows it). No item, no banner, no count.

**The checks** (`tests/harness/suite_records.hero`): `records/lists` holds each
file to one open item, its line first, its body indented, its number its
name's, and its class, the page to holding none of that, and every file of the
record's tree to holding no open item, since a closing `git mv` without its
tick is one command away; `records/numbering` reads the directory and the
record; `records/counts` the ROADMAP's line against the files counted;
`records/tagged` reads the directory at a tag whose tree has it and
`DEFECTS.md` at one whose tree has not, which is every tag placed before
today, both as one text with one `git ls-tree` and one `git cat-file --batch`
a tag.

**What it reverses.** CL-044, the author's instruction of 2026-09-03 (*make
one single file called DEFECTS.md inside work*), and the sentence of
`docs/work/milestones/M-rotated-records.md` that kept the list one file for
`records/tagged`'s sake; each carries a dated line underneath.
