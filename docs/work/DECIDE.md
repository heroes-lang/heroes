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

- [ ] **panel 106 — ratify the resolution. The repair is BUILT and measured; what is still open is your yes** | `docs/panel/106-the-frame-is-the-sweeps-own-temporaries.md`, soundness lane 2026-09-03, **both seats vetoed the proposal**; the implementation landed 2026-09-04 | **This item was moved to the record on 2026-09-04 and moved back an hour later, and the reason is worth more than the item.** It was ticked because the WORK was done — and the item was never the work. It is the question standing in for the author, and `records/verdicts` said so immediately: the panel file's own § Author's verdict reads *"Pending. The open item is `docs/work/DECIDE.md`, naming `panel 106`"*, a sentence that was true when written and false the moment the item left. A sitting whose resolution is provisional and whose queue entry is gone is a sitting nothing will ever ratify. **What the sitting refused**: sharing a prologue declaration between two slots — the ffi-pragmatist compiled a `str` lent to SQLite that answered **`length: 0` at exit 0** with `--sanitize` **hiding** it, and a `setvbuf` buffer that became **SIGSEGV exit 139**; the compiler-engineer measured the whole idea at **0.05%** of declared frame bytes. **What it adopted, and what is now built and measured** (`docs/measurements/015-the-sweeps-own-temporaries.md`): `decref_slot`, which deletes the exit sweep's load temporary — the interpreter's frames **−54.0%**, the compiler's own **−35.6%**, the nesting ceiling **191 → 314** at `-O0` and **104 → 166** under `--sanitize`, the seed **846,804 → 719,955 lines**, the fixpoint holding byte for byte, 560 own tests and 97 harness tests green, 135 of 189 traces re-blessed and fifteen goldens updated by hand. **The default while this is open** is exactly what is in the tree: slot sharing refused in every form, `decref_slot` shipped, the store's `old` temporary (9.1% more) filed rather than done, panel 021 R3 untouched. **A no** would mean reverting a repair that is already measured, so the question is real rather than a formality: it asks whether an op that names a slot instead of a value is the shape this IR should have | docs/panel/106 · docs/measurements/015 · docs/work/DONE.md (the implementation entry) | a provisional resolution with no queue entry is a decision the author will never be asked to make

