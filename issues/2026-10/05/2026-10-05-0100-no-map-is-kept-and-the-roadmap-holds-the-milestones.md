---
kind: decision
area: records
milestone: M-issue-files
filed: 2026-10-05
commit: self
github: none
---

# No map is kept, and `docs/roadmap/` holds the milestones' pages

2026-10-05, written at 01:00 by the clock (`date`). Asked what was left of the
record and why, the coordinator answered that `docs/work/DONE.md` stood as a
map of 4,071 lines so that 30 citations by line number still landed where they
had, and recommended keeping it. The author answered, meant as: *I prefer
cleanliness for the future to keeping the historical legacy, so delete it and
simplify, and merge the folders `roadmap` and `work` as well*. Recorded as a
reading; the rule is `.claude/rules/records.md` § A tree of entries.

**What was deleted**: the three maps the 2026-09-12 rotations left,
`docs/work/DONE.md`, `docs/design/DESIGN-LOG.md` and
`docs/records/book/beats.md`, and with them `records/rotated` and
`records/positions`. The coordinator applied the author's preference to all
three, because the three were one rule; the beats keep `records/entries`, which
holds their names.

**What was repointed first**, so nothing living points at a deleted line: 23
citations by line number in living files, each resolved through its map to the
file it named. One had drifted before the rotation: `docs/work/DONE.md:2415`
meant *reasoning 006 — what the chain lacked*, which stood two lines above, and
the map named the entry below; it points at the right file now. Code comments
that cited a defect as an entry of `docs/work/DONE.md` or `docs/work/DEFECTS.md`
cite it by its number, which is its identifier (defect 279). The texts that told
a reader to look in `DESIGN-LOG.md` (the README, CONTRIBUTING, SECURITY, the
question form, design.md Part 0, the book's README, CLAUDE.md § 4) name
`issues/`. The citations inside dated records stay as written and read against
the maps at `d8a9913e`, the last commit that held them.

**What moved**: the 52 milestone pages from `docs/work/milestones/` to
`docs/roadmap/milestones/`, so `docs/work/` no longer exists;
`docs/roadmap/milestones.md`, a note saying where the pages were, is deleted.
`MOVED` in `tests/harness/suite_records.hero` redirects the old paths, and a
file under a directory that moved now resolves under the directory it moved to.

**Measured**: the compiler's C regenerated from the edited sources is the seed
byte for byte, so no comment edited reaches a program; the first try was not,
because merging two comment lines into one in `selfhost/check/reaches.hero`
moved the `#line` directives below it, and the comment was put back on two
lines.
