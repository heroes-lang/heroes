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
notice, and the thing that did notice was luck. That is the one item below.

- [ ] **Does the seed-freshness rule get an instrument that runs on every push, given the exact cheap check has a false-positive mode?** | found 2026-09-05, out of a stale seed that reddened `main` for a day (`docs/work/DONE.md`, the entry for the regeneration) | **What is true today.** `seed/README.md` says *"if the diff touches `selfhost/`, the seed is regenerated in the same commit"* and names three forcing conditions with one instrument each; the third's instrument is CI's *The seed is what today's source emits*, a `cmp` **on Linux at tags only**. Between tags the only thing that notices is the net going red — and it goes red only when the stale compiler's behaviour differs from a golden's expectation, which on 2026-09-05 it did **by luck rather than by design**: had defect 011's repair carried no golden, a stale seed would have passed every suite. **The default the compiler runs on**, which is the cost of leaving this open: *the seed may be a whole tag behind, silently, unless a golden happens to catch it.* **The cheap check is exact about git and cries wolf** — assert that the newest commit touching `selfhost/**` also touches `seed/heroes.c`; it would have fired on `44930209`, and it fires falsely on any `selfhost/` change whose emitted C is byte-identical, because git records no change to the seed and that commit never enters its log. **The expensive check has no false positive and is no longer expensive**: emit `selfhost/` and `cmp` it against the committed seed — measured twice on this Mac 2026-09-05, **21.13 s** and **21.89 s**, against the *"15m41s"* `seed/README.md` prices it at, which is the arithmetic that put it on tags. So the question is narrow and answerable: does 21 s belong in the net on every push, in `heroes doctor`, or where it is | seed/README.md § When this file must be regenerated · .github/workflows/ci.yml:508-510 · tests/harness/suite_records.hero · docs/work/DONE.md | the instrument that would have caught this exists and fires a milestone late, and a rule whose check is that late costs a day of red every time somebody forgets

- [ ] **Panel 111 R4 owes a new diagnostic class, `ffi_callback_type`, and the premise it rests on was measured false. Does the class still open?** | found 2026-09-05, in M-c-callbacks step 0, by running the case R4 describes | **What R4 says, verbatim**: *"a new diagnostic class — `ffi_callback_type`, exit 1 on the `.hero` line — so a wrong callback signature is the compiler's message and not clang's internal error at exit 2"*, and the ffi-pragmatist seat predicted a `qsort` binding would exit **2** *"because `ffi_declared.hero` classifies neither an argument-position function-pointer mismatch"*. **Measured with the permission in place**: `function atexit(f: (function(i64) -> ()))` and `function qsort(…, compar: (function(ptr, ptr) -> i32))` are both **`error[ffi_parameter_type]` at exit 1 on the author's own line**, naming the parameter and the header's exact type. clang's text is `incompatible function pointer types passing 'X' to parameter of type 'Y'`, and it contains ` to parameter of ` — which is panel 071's `MISMATCHED` needle, already in `emit/ffi_narrowed.hero`'s `family()`. **So the class's stated purpose was already met, and the seat's prediction is falsified.** What was genuinely wrong is the NOTE: it offered *"if it is a struct, declare it as a `record` inside this same `extern` group"* under `int (*)(const void *, const void *)`, which §4.17 forbids because the note is chosen by the type. **The default the compiler is running on**, taken provisionally in step 0 and reported: **no new class**, and a callback-specific note instead — resting on `selfhost/ffi_errors.hero`'s own written rule that *"a new diagnostic CLASS is a panel path which a second name for one question would be"*, which is why `ffi_parameter_position` reuses `ffi_parameter_type` rather than minting a code. **The cost of that default**: a reader cannot grep a callback mismatch apart from a width mismatch, and a ratified resolution stands unperformed in the record. **The alternative**: open `ffi_callback_type` as R4 says, on the argument that the two mistakes want different searches even when they want the same repair sentence. Either way the finding is the record's, because a sitting that predicted exit 2 and got exit 1 has told this project something about its own classifiers | docs/panel/111 R4 · selfhost/ffi_errors.hero:36-41 · selfhost/emit/ffi_narrowed.hero (`family`, `MISMATCHED`, and the new callback note) · tests/golden/fixedbugs/ffi-callback-signature-the-header-refutes.hero | a ratified resolution and a measurement disagree, and CLAUDE.md §12 says the measurement wins — but §4 says the author ratifies, and this one already did
