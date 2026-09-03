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

- [ ] **Should `_ = f()` be refused when `f()` is fallible — a discarded `T?` swallows its error in silence** | measurement 014's survivor listing, 2026-09-03, the first over the 78-file corpus: **all 9 `drop-question` survivors are one shape**, `_ = expect(line, 2)?` → `_ = expect(line, 2)` (six in `examples/assembler/assemble.hero`, one in `machine.hero`, two in `examples/maze/grid.hero`) | **Today the compiler accepts it, and the program runs past a failure it was written to stop at.** `spec:97-98` says a computed value is bound or discarded on purpose with `_ = f(x)`, and `_` accepts a `T?` like any value — so a `?` dropped from a discard is not a type error, where the same `?` dropped from a binding is (`x = f()` then `x + 1` is *expected i64, found i64?*). That asymmetry is exactly the mistake metric 3 exists to catch: the author *meant* to propagate, the line still reads as deliberate, and no diagnostic sees it. **The default while this is open** is the accepting one, and it is measured: 9 silent wrong programs in a corpus of 78. **Three shapes.** (a) `_ =` on a fallible value is an error naming the two legal spellings — `_ = f()?` to propagate, or a `match`/`.is_err()` to decide — which kills all nine and costs one sentence at `spec:97-98` and one diagnostic class (§4 panel path, since both are language). (b) Leave it: a discard is deliberate by definition, and the survivors are the price of `_`. (c) Refuse only `()?` — the result nobody could have wanted to keep — and keep `_ =` legal on a `T?` whose value is being thrown away on purpose. **Recommended: (a)**, because the thesis says a plausible mistake is a compile error and this one is measured plausible nine times; (c) is the smaller spec cost if the warden's count decides. Whichever lands, the nine survivors are its witnesses and re-score at the next `mutate` | docs/measurements/014-mutate-over-thirty-five.md § Appended · examples/assembler/assemble.hero:77-103 · spec:97-98, 141-152 · selfhost/check/walk.hero (where a discard is typed) | the one place `?` can be forgotten without the type system noticing, found by the instrument built to find exactly that
