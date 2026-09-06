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



- [ ] **`compile "gfx.c"` — panel 036's deferral has found its acceptance test, and it is the author's own question** | author question 2026-09-06, *"if in future I want a graphics library wrapping three different libraries underneath, how will I do it?"*, asked while panel 114 was being written up | **The question is answered for the thin case and open for the real one, and the split is exactly where panel 036 left it.** THIN: one header the package ships, `#ifdef` inside, `static inline` wrappers — measured 2026-09-06 on macOS and on the Windows box, one `.hero` source, exit 0 on both, and `--emit-c` carries zero platform words. That is how every portable C library is already written: **SDL is literally this case** — Cocoa on macOS, X11 or Wayland on Linux, Win32 on Windows, one header, and `examples/sdl/main.hero` binds it today with no platform word anywhere in the file. The `link` half is `package`, and panel 050 said it *subsumes the platform axis panel 049 refused while putting no machine's name in any program*. REAL: a wrapper with hundreds of lines of implementation wants a `.c` file the compiler builds, and that is `compile`, which **panel 036 DEFERRED rather than vetoed** — `docs/panel/036:275`, *"not refused on their merits but deferred for want of an acceptance test. They wait on a rung that needs a shim."* Correction found by panel 114's spec-warden, 2026-09-06: the record had been read as a veto. **The author's `gfx` is that rung.** What to decide: whether `compile` lands, and if so whether it is a group clause (`extern "gfx.h" compile "gfx.c"`) or a package-level fact — panel 036 P2's own veto was of the group clause, so the second shape may be the one that was never judged. Not urgent and not this milestone: nothing in the closure list needs it, and the thin case covers everything the corpus does today | docs/panel/036-*.md:275 · docs/panel/050-*.md · docs/panel/114-the-question-was-not-which-platform.md · examples/sdl/main.hero · selfhost/cli/libraries.hero | the one question a Heroes user will ask that today has a good answer for small libraries and no answer for large ones, and the deferral that covers it has been waiting for a witness since August
