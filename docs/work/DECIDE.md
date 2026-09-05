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

- [ ] **panel 111 (`docs/panel/111-the-emitter-could-already-carry-it-the-runtime-could-not.md`) — ratification pending** | the M-isolated-threads opening sitting, 2026-09-05, full five seats, **two vetoes on design.md §1.12** | **The proposal was refused and the milestone was re-scoped, so this is a bigger ask than a usual ratification.** What the sitting adopted provisionally: **R1** the proposal as briefed is refused (two vetoes, each with a running program — `heap-use-after-free`, `attempting double-free`, `heap-buffer-overflow` and a silent SIGSEGV, every one reachable from one line of ordinary Heroes). **R2** M-isolated-threads delivers Part 7.13's isolation or it delivers nothing — the deferred half becomes the deliverable, which is the only route both compiling seats accept and the only one the historian's ledger has a precedent for (Erlang, the single surveyed language that needed no type-system change and paid with per-process heaps). **R3** data parallelism only, no mailbox, no scheduler — unchanged and unopposed. **R4** the callback permission LEAVES this milestone and becomes its own, proposed id **`M-c-callbacks`** — **and placing a new id in the chain is yours** (`docs/work/SCHEDULED.md`'s rule), which is the one thing in this item that cannot be defaulted. **R5** point 3 does not land in any form: its mechanism is measured broken on Darwin arm64 (a `_Thread_local` counter reads **0** inside a `pthread_key_t` destructor), it named 2 of 18 shared mutable objects, and it would delete the only instrument currently reporting the race. **R6** the refcount half of design.md's own v1 invariant (`:2578-2586`) is open work here. **R7** `design.md:2536` — *"no aliasing means no data race is expressible"* — is corrected: true of Heroes values, false of the refcount. **The conservative resolution, recorded so you can take it instead**: adopt nothing, leave the milestone open and unscoped, let the next sitting re-derive it. It changes least, and it leaves four measured corruption classes out of the record | docs/panel/111-the-emitter-could-already-carry-it-the-runtime-could-not.md · docs/measurements/017 · design.md §1.12, Part 7.13 · docs/ROADMAP.md § The chain | §4 says the panel never blocks and you always ratify, and this sitting refused the thing it was convened to adopt
