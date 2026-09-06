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

- [ ] panel 116, soundness lane, 2026-09-06 | **Where the `owned` release lives: adopted route (a) — built in the IR, with the freer call keeping its pointee.** Two vetoes in opposite directions, and the resolution is their intersection rather than a compromise. The FFI seat vetoed the IR route as proposed because **`ptr` emits as `void *`**, and at `void *` clang accepts **10 of 10** reachable freers silently while at `char *` it rejects **6 of 10** naming the header's own type — `fclose((FILE *)strdup(…))` is exit 139. The compiler seat vetoed the emitter route because it puts a second elaborator in the printer against Part 5, and measured that its only two arms are refused elsewhere: `h_library_validated` is not emitted (`call to undeclared function`, exit 2 on a legal program) and composing a `str?` in raw C is what design.md:476-480 already refused for `read_file`. **What you are ratifying**, and item 7 is the one to read first: the release is built in the lowering; the freer call strips `const` and nothing else so clang goes on checking it; the conversion is reachable only from the mark; all three `flatten.hero` hook sites are collapsed into one helper; the NULL guard is unconditional from one template, because the FFI seat measured that **no configuration of this Mac can see it missing** — `fclose(NULL)` survives and a guardless release exits 0 even under ASan; one hand-written `tests/golden/ir/` case; and **the panel interprets panel 109's "generated per-freer release" as per-freer at the C call site rather than a synthesised function per freer**, which is a reading of a ratified condition and is yours to overturn. **And one thing to read before you ratify anything**: the coordinator contaminated a seat's bench — the prebuilt compiler inside the frozen snapshot carried an uncommitted change, so one supporting sentence in the compiler seat's verdict was the coordinator's own change and not the tree's. It is corrected in the sitting, the verdict survives it, and it is written down rather than quietly fixed | `docs/panel/116-where-the-release-lives.md` | the emission step cannot start until this is settled, and it is the half of `owned` that actually frees memory
