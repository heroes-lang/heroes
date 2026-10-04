---
kind: decision
area: none
milestone: none
filed: 2026-10-02
commit: 0bc28e2d1410c0638ebe2c406f9e2624874ab5a4
github: none
---

# Item 130 cut in `8349d264` and restored: what cut it is unknown, the commit did not read its diff

2026-10-02, written at 21:41 by the clock (`date`). Found by panel 187's
completeness critic in its first pass over the sitting's briefs
(`docs/panel/187-reports/completeness-critic-briefs.md` § 2.1), confirmed by
the coordinator, and restored in the round's tree before its net
(`50644159`, which the trunk took by fast-forward at `6bec7c8c`).

**What was lost.** Item 130 of `docs/work/DEFECTS.md`, the open defect the
parser's recovery cluster has carried through seven batches, lost three
spans of text in commit `8349d264` (18:39, *Defects 165 and 166 filed*): its
first line went from 660 bytes to 355, cut twice inside the line (its title
from *`match` whose arm* to *in `f*, and its tail from *`z = 4 )`* to *the
enclosing statement's r*), so the line kept neither its title nor its *where
to look* field; and seven lines of its body became one, losing the sentence
about `recrod Point` and the head of the paragraph *Widened 2026-09-29 by
lane 133's agent*. The merge `b48d02b8` kept the trunk's side. `records`
read 24 passed after each, because no instrument reads an item's text
against its history.

**Restored** byte for byte from `d4fd12ef`, the round's tree of that
evening, whose item 130 differs from the damaged one in those two places
alone (the coordinator's `diff`, and the gate's script before and after);
`8349d264` against its parent `4b44f684` removed nothing else.

**What cut it is not known, and the record says what was searched.** The
script that wrote `8349d264` inserts the two new items before the list's
closing line and replaces the banner, and can cut neither span. The file
was whole at `4b44f684` (17:47). Between 17:47 and 18:39 no tool call of
this session, of the 70 agents whose transcripts it held at the search, or of any other Claude session's
transcript for this project wrote to the trunk's `docs/work/DEFECTS.md`
(the transcripts' tool calls naming the file, read by script; the one agent
that touched it, panel 187's facts agent, only read it). The file was open
in the author's editor at 17:59 (this session's IDE notice). So the cut came
from outside the Claude sessions, by a route not identified; it is not
attributed to anybody.

**What is the coordinator's**: `8349d264` committed the whole file by
pathspec without reading its diff. The script's own change was one banner
line removed and the new items added; the commit removed nine lines. A
check of the commit's own `git diff --numstat` against what the script
wrote would have refused it at 18:39 instead of a critic finding it at
20:28. **From this entry on, a scripted edit of a shared record asserts its
diff before the commit**: the lines it removes are the lines it meant to
remove (`.claude/rules/records.md` § Two habits, *script the pairs, assert
each anchor matches exactly once, dry-run, then apply*, of which this is the
step that was missing); the closings of 159 and 165 (`2caca562`) are the
first done that way, every removed line found in their closed records.

**Left for a lane, an `improvement`**: `records/lists` could refuse an item
line that lost its shape (`**NNN — <title>** | <what> | <where>`, the title
closed by `**` before the first ` | `), which would have caught the first
cut, not the second.
