# M-guide-book — the guide


**Scheduled**, and it is the one milestone after the fixpoint that has a warrant.
The classic language guide, the K&R shape: read it front to back and you can
write Heroes; open it in the middle and you find the thing you were looking for.

- **Organised by subject, not by chronology**: values and types, bindings and
  `@`, control flow, records and variants, `T?` and failure, generics, the
  module, the FFI, the test blocks — each with the smallest program that shows it
  and one that gets it wrong on purpose.
- **It is not the spec, and it must never try to be.** `spec/heroes-spec.md` is
  the control instrument, budgeted at 4096 tokens precisely so that it can never
  become a teaching text; the guide is where the explanations, the worked
  examples and the "why it is like this" live, at whatever length clarity needs.
- **Its warrant is §1.1**: comprehension is this project's objective, measured
  rather than asserted, and a guide the author can read to rebuild the reasoning
  is that objective's final artifact.
- Where the guide and the spec disagree, **the spec wins and the guide has the
  bug** (CLAUDE.md §12) — and where the guide and the *compiler* disagree, that
  is a defect report on one of them, which is what the M-documentation-site check
  over `examples/` is for.
- **Metric 2's held-out tasks land here** (author decision 2026-08-24): they must
  be author-written or they measure the assistant's priors, and this is the one
  milestone whose own work *is* the author writing programs.

*******************************************************************************
**OPEN: 3**

- [ ] **M-guide-book** | metric 2's held-out tasks, written by the author | `harness/tasks/README.md` · `docs/panel/011`

    **Origin:** author decision 2026-08-24, `/decide` answer `8b`. This item
    replaces both the 2026-08-11 original and the 2026-08-18 promotion to
    `DECIDE.md`, which had made three copies of one question.

    They must be author-written or they measure the assistant's priors rather
    than the thesis, and `n` scales with the paced protocol rather than 15 at
    once. Measured 2026-08-24: `harness/tasks/` holds **only `README.md`** —
    zero of the 15 — so nothing has been quietly accumulating, and metric 3 is
    unaffected (it has run on every commit since panel 011). **Why this
    milestone owns it**: M-guide-book's own work *is* the author writing
    programs in Heroes, so the held-out set comes out of work already happening
    instead of competing with the compiler for the same hours. Two deferrals of
    this item have already made their stretches unrecoverable, which is why the
    home is named rather than left as "later".

    **Where to look also:** `docs/debrief/DECIDE.md` (2026-08-18, the closed
    promotion; that directory is retired).
    **Why it matters:** metric 2 validity, and the thesis keeps two measured
    halves instead of one.

    **Re-verified 2026-09-10: STILL OPEN, nothing accumulated.**
    `harness/tasks/` holds one file, its own README, which says *"Status: 0 tasks"*.
    One clarification for whoever opens it: that README's target is **20** frozen
    tasks of which **15** must be author-written, so this item's 15 is the held-out
    subset and not the whole set.

- [ ] **M-guide-book** | a long signature has no continuation, and a printed page cannot hold one | `selfhost/print/fmt.hero:77`, `:524-550` · `selfhost/parse/members.hero:60-104` · `docs/panel/110`

    **Origin:** `docs/panel/110`'s historian, 2026-09-04, as the one
    recommendation the sitting docketed rather than refused. Scheduled at *row
    53* until 2026-09-05, 54 after the insert, and dropped for §14's reason: a
    reorder moves a number and never a name. This is the milestone whose pages
    are 72 to 80 columns wide.

    The form is legal today — all three multi-line shapes `check` and `run` at
    exit 0 — and `heroes fmt` **joins them onto one line** with no width test,
    deliberately, because panel 007 *"leaves nowhere else to break it, and a
    formatter that invents a continuation invents one the language does not
    have"*. Measured 2026-09-04: **167 of 1444 (11.6%)** signatures in
    `selfhost/` exceed the formatter's 120 columns, against **3 of 850 (0.4%)**
    in `examples/`, and the longest is **235 columns**.

    **Why this milestone and not sooner**: Principle 0 refuses it now, because
    nothing is blocked and ordinary programs barely have the shape — but a book
    page is 72 to 80 columns, so either the book invents a continuation the
    language does not have, which is §9's *formatter lies quietest* failure one
    level out, or those signatures are renamed shorter, or the language gains
    the break. M-doc-generator and M-lsp-server hit the same wall earlier, in
    hover text and generated docs.

    **The precedented shape, and it is convergent**: the author's **trailing
    comma steers the layout** — `zig fmt` puts a parameter list on one line
    unless a trailing comma is present, in which case one per line, and Black's
    *magic trailing comma* (20.8b0) does the same for brackets; two independent
    formatters reached it to avoid a width-driven algorithm guessing. It
    preserves design.md §4.15 because the trailing comma **is** the textual
    difference, it is canonical, and the round trip is stable — and Heroes
    already requires the comma, so the token is not new. `spec:174`'s
    newline-separated multi-line **literals** are the same instinct applied one
    construct over. **The seat's prediction to score at this milestone**: the
    11.6% figure **rises** rather than falls, because every new generic
    parameter and every `@` parameter lengthens a signature and nothing shortens
    one.

    **Where to look also:** `spec:174` · `docs/panel/095`.
    **Why it matters:** the formatter is the one tool whose silence makes every
    diff untrustworthy (§9), and the book is where a shape it cannot print stops
    being invisible.

    **Re-verified 2026-09-10: STILL OPEN, both percentages re-measured, and the
    seat's prediction is AMBIGUOUS.** `selfhost/` is now **180 of 1560 signatures
    over 120 columns (11.5%)** against the item's 167 of 1444 (11.6%), `examples/` is
    **3 of 932 (0.32%)** against 3 of 850, and the longest is still **235**.
    `selfhost/print/fmt.hero:77-78` is still `WIDTH` `120` and `:524-555` still joins
    a signature onto one line with no width test. **The registered prediction says
    the 11.6% figure RISES**: the absolute count rose 167 to 180 and the rate is flat
    to slightly down, 11.6% to 11.54%, so which of the two the seat meant decides
    whether it is met. **Resolve that before the sitting reads it as scored**, which
    is a question and not a task.

- [ ] **M-guide-book** | the held-out tasks must never be committed, now that the repository is public | `harness/tasks/README.md` · `design.md` Part 11

    **Origin:** M-open-repository, 2026-09-08, and nobody had written it down.

    Metric 2 is held out: 15 of the 20 tasks must be author-written, and the
    whole point is that the model reading them has not seen them. **A public
    repository makes committing them the act that burns them** — not only for a
    scraped training set, but for anybody who reads the file before taking the
    test. Nothing is burned today, because `harness/tasks/` holds **0 tasks**
    and the five assistant drafts were pruned on 2026-08-03.

    Owed at the moment the first task is written: a home outside this
    repository, and a line in `harness/tasks/README.md` naming it. What can be
    committed is the grading and the provenance, which say nothing about the
    task's content.

    **Why it matters:** a measurement whose instrument is public measures
    something else.

    **Re-verified 2026-09-10: STILL OPEN, and its factual premise is true
    today.** `harness/tasks/` holds one file and **zero** tasks, so nothing is burned
    yet, which is exactly the window the item exists to use. What is owed is still
    absent: `grep -in "outside this repository" harness/tasks/README.md` finds
    nothing, so no home outside this repository is named anywhere.

*******************************************************************************
