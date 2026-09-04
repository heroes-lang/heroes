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



- [ ] **panel 107 — ratify the resolution. Nothing about the stack size changes; what needs your yes is the per-thread guard and the refusal of all three mechanisms** | `docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md`, full panel 2026-09-04, **two vetoes and a third scoped to the Linux spelling** | **Three refusals and one adoption, and the adoption is bigger than the question you asked.** Refused: the Linux flag (inert — two linkers ignore it, depth 388 both ways — plus a permanent build warning on every Linux compile forever); `main` on a created thread (rustc's answer, and it breaks `examples/sdl/`: `SDL_Init(VIDEO) failed: No available video device`); a depth number in the spec (the warden compiled a legal program that reaches 248 levels at 64 MB, and this compiler's own worst frame gets 1,016 — **1.6% from breaching the sentence**). Adopted: `runtime/parts/stack.c`'s bounds and alt-stack become **per thread**, so running out of stack is `panic: stack exhausted in <function>` at exit 134 on every thread of every platform instead of a silent 132. **That is the uniformity your question asked for, in the only axis measurement admits** — the number cannot be uniform on any of these three platforms and the sitting shows why six ways. **What your yes costs if you disagree**: the per-thread guard is unbuilt, so a no costs nothing already shipped; the two corrections in `selfhost/cli/flags.hero` and `DESIGN-LOG.md` stand either way, because both are measurements rather than rulings. **Recommended: ratify**, and note the one thing the sitting left you a choice about — if you want more depth on POSIX anyway, the number argued from this compiler's frames is **16 MB** (500 levels x 26 KB, +23% margin), Darwin-and-Windows only, landed as a driver default and never as a language promise, which is the compiler seat's own condition 2 | docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md · runtime/parts/stack.c:107-108, :253 · selfhost/cli/flags.hero:74 · DESIGN-LOG.md (2026-09-04) | until the guard is per-thread, a Heroes program that runs out of stack on any thread but the main one dies with no message at all, and `--sanitize` is no louder
