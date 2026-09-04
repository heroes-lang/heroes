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



- [ ] **How much stack does a Heroes program get, and why does this compiler answer that on one platform out of three?** | measured 2026-09-04 while ratifying panel 106, because the sitting's own subject was the recursion ceiling and the ratification exposed what actually sets it | **`selfhost/cli/flags.hero::link_flags` passes `-Wl,/STACK:67108864` on Windows — 64 MB, ratified by panel 058 in August — and passes nothing of the kind on macOS or Linux, where a program gets whatever the operating system hands it: 8 MB on this Mac (`ulimit -s` 8176).** So the same program has an eightfold different ceiling depending on the platform, and nobody decided that: one half is a ruling and the other half is a default. **Measured on this Mac, `examples/interpreter/`, binary-searched, the same emitted C linked twice**: with the OS default, **314** levels at `-O0` and **166** under `--sanitize`; with `-Wl,-stack_size,0x4000000` — the same 64 MB Windows already gets — **2,537** and **1,347**. That is the whole of why M-corpus-depth's 500-deep requirement is unmet at 83: not the frames, which panel 106 already halved, and not the language. **What is at stake beyond one test case**: a Heroes program's recursion depth is part of what the language promises a person writing one, and design.md §1.12 makes not crashing a goal — a promise that currently means something different on each platform. **Three shapes.** (a) Pass the same 64 MB on every platform, so the answer is one number and panel 058's ruling stops being Windows-only — the consistent choice, and the one that makes the corpus's own 500-deep case reachable. (b) Leave it and write the asymmetry down as deliberate, with the reason a POSIX program should take the system's answer. (c) Make it a flag the author sets per program, which §10's stopping rule would have to admit first. **Recommended: (a)**, because the number already exists in this repository as a ratified decision and the only thing keeping it from the other two platforms is that nobody asked; it costs one arm in `link_flags` and it is measurable on the three machines the same day. **This is a language question and not a repair** — what a program is given, not a defect in what it does — so CLAUDE.md §4 sends it to a sitting rather than to a commit | selfhost/cli/flags.hero:112 (the Windows arm) · docs/panel/058 (where 64 MB was ratified) · docs/panel/106 § Author's verdict · docs/journal/032-corpus-depth.md (the unmet requirement) · docs/measurements/015 | the deepest a program can call itself is a promise the language makes, and it currently makes three different ones
