# M-rotated-records — the records become directories *(closed 2026-09-12)*

**What it delivered**: the four documents two sessions could not both write are
now trees of one file per entry, and the files they were stay at their paths as
maps of the same height. `DESIGN-LOG.md` → `docs/log/` (620), `docs/work/DONE.md`
→ `docs/done/` (491), `docs/book/beats.md` → `docs/book/beats/` (114), and
§ The milestones, one by one → `docs/milestones/` (43), which absorbed
the 53 items `docs/work/SCHEDULED.md` held until 2026-09-12, so that one milestone is one file is one
lane.

**What warrants it, with the instrument**: CLAUDE.md §14. Measured over the 100
most recent commits before the work: `DESIGN-LOG.md` touched by 40,
`docs/ROADMAP.md` by 31, `docs/work/DONE.md` by 26, `docs/work/DECIDE.md` by 24 —
so two sessions meet in the registers and never in the compiler. Derived from
those rates, a pair of parallel steps collided on `DESIGN-LOG.md`'s last line
about 61% of the time.

**The reasoning a later milestone has to honour**, and it is one sentence: **the
entry moves and its line does not**. 161 citations by line number point into the
two large records, 124 of them from inside records §14 forbids anybody to
repoint, and CL-037 says no instrument can see one. A map of the same height
keeps them all true and makes them checkable — `records/positions`. Any future
rotation takes the same shape, and `.claude/rules/records.md` § A rotated record
is its home.

**What it did not deliver**: `docs/work/DECIDE.md` and `docs/work/DEFECTS.md`
stay single files, because `records/tagged` reads them at a tag with
`git show <tag>:<path>` and because CL-044 refused exactly that fragmentation for
a live list. `§ Where we are` stays serial by nature: its numbers are a
measurement of the whole tree at an instant, and two lanes cannot both be right
about it. One `**OPEN**` row at a time is still enforced by
`site/src/lib/chain.ts`, which is the open decision this milestone filed rather
than settled.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
