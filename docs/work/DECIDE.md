# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

**It was empty for one morning, 2026-09-05.** The four spec silences carried from
the retired panel watch list were settled at `docs/panel/110` and are in
`docs/work/DONE.md`. **An empty list is not a finished list**, and CLAUDE.md's
own closing paragraph says why: this file reaching zero *"reads as tidiness and
was the symptom"* on 2026-09-04, because the item that had gone was the author's
ratification standing in for the author. What emptied it that time was a sitting
that ruled, two vetoes that refused a sentence, and one premise that measurement
destroyed — and what those left behind is **work**, which is in
`docs/work/SCHEDULED.md` under M-check-completeness and M-guide-book, not here.

**What refilled it the same evening was not a sitting but a red `main`**: the
seed had been two repairs behind for a day, nothing between tags was obliged to
notice, and the thing that did notice was luck.

**And it emptied again at M-c-callbacks step 1, the same session that asked**,
which is CLAUDE.md §3's rule that every answer is applied where it is given. Both
items are in `docs/work/DONE.md` with their verdicts: the seed-freshness check
moved from tags to every push on the Linux leg, because the 15m41s that had
funded the tag-only decision measured **21.53 s** today; and `ffi_callback_type`
opened **against** the coordinator's recommendation, on the author's ground that
two mistakes can want one repair and two searches. **This paragraph is corrected
rather than deleted, and the correction is the point**: it stood for a day saying
*that is the one item below* with nothing below it, which is the shape this
file's own second rule forbids — an entry is a claim from the day it was written,
and a preamble is an entry. A list that describes items it no longer holds is
worse than an empty one, because the reader cannot tell whether the item was
answered or lost.

- [ ] M-declared-freer step 1, and the sweep half is done — this is the half that is still a decision | **Should the dead-citation check read the compiler's own comments, at the price of seven exemptions?** The 164 files are swept (`docs/work/DONE.md`), so nothing is dead today; the question is whether an instrument keeps it that way or whether the next tree that moves waits for somebody to notice. **Measured 2026-09-06, over the swept tree**: the check's own anchored rule — first component is a repository directory OR the token ends in a file extension, no space, glob or brace — finds **208 distinct paths in 294 comment occurrences** across `selfhost/` and `tests/`, of which **8 do not exist, 3%**. Panel 086 refused the unanchored rule at **52%**; 3% is a check somebody would keep. **But 7 of those 8 are legitimate prose and would need exemptions**, which this suite's own doctrine prices as *"a dead exemption is not free"*: four are sentences that say the path is gone and say where the bytes are now (`site/log.html` ×4, `docs/debrief/QUEUE.md`), one is a hypothetical inside the check's own comment (`site/src/html/italy.md`), one a placeholder (`docs/measurements/NNN.md`), one a computed path in an illustration (`build/<hash>/nolink.c`) and one a shorthand list the sentence already dates. **The eighth was real and is repaired in this commit** — a `fixedbugs` golden calling the twin *"still open"* thirteen days after it was repaired, pointing at the retired directory. **The recommendation is yes, with the exemptions written as SUFFIX matches the way `RECORD` already is** — the cost is seven lines that each name a shape rather than a file, against a class this repository has now met three times; the alternative the measurement makes honest is *no*, on the ground that a check needing seven exemptions on day one is a check whose rule is not quite right yet, and that 3% may drift upward as the tree grows | `tests/harness/suite_records.hero` § `citations`, `path_tokens`, `anchored` · `docs/panel/086` R7 · CLAUDE.md §3's deliberately unspelled path | the check exists to catch a moved tree, and the one thing it cannot see is the compiler's own source
