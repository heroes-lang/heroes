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

- [ ] **panel 108 — ratify the resolution: the unread-cell rule is REFUSED in both shapes, the ownership question is queued for a §4.19 sitting, and one CLAUDE.md sharpening is proposed** | `docs/panel/108-the-unread-cell-is-not-the-leak.md`, full panel 2026-09-04, no vetoes, two objections and two conditional approvals | **What your yes settles**: (1) the refusal of *a cell passed as `@` and never read is `unused_binding`*, full and `extern`-only, with its falsifier named — a measured real defect among the 54 fires (0 today), or an out-only `extern` parameter mode; (2) that the leak is an ownership fact and goes to its own §4.19 sitting (`design.md:2120-2125` already reserves the keyword), not to this rule; (3) the proposed amendment to CLAUDE.md § Commands: **an FFI program's pre-commit Linux run includes `--sanitize`**, because LeakSanitizer exists only there and is what found the defect — this one is yours to write, since the contract is amended by author instruction. **Recommended: ratify.** The two objecting seats counted (54 and 55, zero defects) and the two approving seats conceded in their own arguments that the rule does not touch the leak; the conservative resolution and the robust one coincide here, and the file says so rather than reaching for a change because a sitting was held. **What a no would mean**: landing the full rule costs 48 `_ = x` lines in `selfhost/`, a degraded `fixedbugs` diagnostic, and a spec sentence against §4.8's *"copy in, copy out"* — for a rule that `_ = error` satisfies while leaking | docs/panel/108-the-unread-cell-is-not-the-leak.md · selfhost/resolve/writes.hero:72-79 · design.md:2120-2125 · docs/work/DEFECTS.md (the `void *` pointee defect the sitting found) | until ratified, the refusal is provisional and the §4.19 sitting has no author's word behind it
