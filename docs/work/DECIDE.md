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

- [ ] M-declared-freer step 1, found while writing four module docs | **164 live source files cite a path that has not existed since 2026-08-19, and the instrument that exists to catch exactly this cannot see them.** Measured 2026-09-06: `crates/` appears in **145 files under `selfhost/`** (150 occurrences) and **19 under `tests/`**, always as a module doc's `Port of crates/heroes/src/…` line; `archive/bootstrap-rs/` is where that tree went at M-bootstrap-archive, and the rewrite resolves — 5 of 5 sampled paths exist under the archive. Two reasons the dead-citation check in `tests/harness/suite_records.hero` reports nothing: it reads the backticked paths in **CLAUDE.md**, not the compiler's own comments, and most of these are written in the **brace form** (`{decls,stmts,exprs}.rs`), which is one of the two shapes panel 086 R7 measured that it cannot read. CLAUDE.md itself is clean — its two `crates/` mentions are deliberate historical quotations of what a sentence *used to say*. `docs/` holds 46 more and they stay: §14 forbids rewriting a dated record. **The recommendation is (d): sweep the 164 AND teach the check to read them**, because three of the options are habits and only one is an instrument — (a) sweep `crates/` → `archive/bootstrap-rs/` in the 164 live files, one command, keeps the provenance several docs reason from; (b) delete the `Port of …` lines, which §5 argues against, since it keeps the Rust section *"as the record of what it bought"*; (c) leave them, which is the shape CLAUDE.md §7 has already recorded going wrong three times in one sentence; (d) = (a) plus extending the check to `selfhost/**` and `tests/**` and to the brace form, so the next tree that moves is caught by something rather than by somebody reading. **Where this list came from** (§1): the four options are reasoned, not enumerated from a corpus — what would have to be true for a fifth is that some of the 164 cite the Rust as *history* rather than as a live pointer, in which case they are records and (a) would be wrong for them; a sample of the wording would settle that and has not been taken | `grep -rl 'crates/' selfhost/ tests/` · `tests/harness/suite_records.hero` · `docs/panel/086` R7 · CLAUDE.md §5, §7, §14 | the compiler's own source is 164 pointers into nothing, and this is the third time in this file's history that a moved tree left a live citation behind
