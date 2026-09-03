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

- [ ] **panel 106 — ratify the resolution: slot sharing does not land, and the exit sweep's load temporary goes instead** | `docs/panel/106-the-frame-is-the-sweeps-own-temporaries.md`, soundness lane 2026-09-03, **both seats vetoed the proposal** | **The sitting refused what it was convened for and adopted what its own measurements found.** Refused: sharing a prologue declaration between two slots — the ffi-pragmatist compiled a `str` lent to SQLite with a null destructor and got **`length: 0` at exit 0**, with `--sanitize` **hiding** it, and a `setvbuf` buffer that became **SIGSEGV exit 139** with the stack guard rightly silent; the compiler-engineer measured the whole idea at **0.05%** of declared frame bytes, since 98.2% of slot bytes are refcounted and `ir/own.hero:257-262` reads every swept slot at every return, so no two are ever disjoint. Adopted instead: **`decref_place`**, an IR op minted by `own.hero` and consumed by `emit/inst.hero`, emitting `release(&slot)` and deleting the load temporary that is **58.5% of all declared frame bytes** — measured by rewriting the emitted C and rebuilding: interpreter frames **−52.3% at `-O0`**, **−62.7% at `-O2`**, nesting ceiling **191 → 311** and **347 → 581**, the seed **846,804 → 786,062 lines**, ASan and UBSan clean, zero warnings under the full flag set, and **a compiler built from the rewritten C emits byte-identical C** (30,371 pairs removed across 2,907 functions, the fixpoint preserved). Cost ≈ **+25 code lines over 20 files**, 3-5 raises in `suite_layout.hero`'s DECIDED table, and **86 of 186 emission traces re-blessed** with the diff read (§9). **The default while this is open** is the resolution as written, which M-corpus-depth's next step builds; a veto keeps the ceilings at 104/191/349 and closes the milestone's 500-deep requirement as *refused with a measured reason*. **This synthesis took the ROBUST resolution and not the conservative one, by author instruction of the same evening** — the conservative one was *do nothing*, since both seats vetoed | docs/panel/106 · selfhost/ir/own.hero:150, :257-262 · selfhost/emit/inst.hero · selfhost/emit/body.hero:10-13 (panel 021 R3) · examples/interpreter/run/eval.hero (the DEPTH this raises) | the recursion ceiling of every program in this language is set by an implementation detail nobody chose, and the sitting measured which one
