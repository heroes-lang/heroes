---
kind: feature
area: print
milestone: M-guide-book
filed: 2026-09-04
commit: none
github: none
---

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
