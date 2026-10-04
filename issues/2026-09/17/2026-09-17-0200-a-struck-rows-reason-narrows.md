- [x] **055 — `design.md` strikes `has(m, k)` on a spelling panel 160 removes** | the reason given for having no presence test is `!m[k].is_err()`, which is refused on a `{K: V?}` under the adopted resolution | closed 2026-09-17, M-check-completeness step 14

    **Origin:** 2026-09-17, panel 160's compiler-engineer, handed on as an
    interaction outside its own seat.

    **The paragraph**, verbatim: *"`has(m, k)` is **struck** (panel 026, −17
    measured): map access returns `V?` always, so `has(m, k)` and
    `!m[k].is_err()` are two spellings of one predicate."* Under option E the
    second spelling does not exist for a map whose value is fallible, so on that
    map there is exactly **one** spelling and it is `match`.

    **Why it is a defect and not a footnote.** The struck row's own reason is
    that a second spelling would be redundant. Where the resolution removes the
    first, the reason is gone and the row is unargued — which is the shape that
    paragraph's own last sentence names: *the shape of a reason outliving the
    thing it argued against*.

    **What is owed.** A dated correction under it (the document is append-only),
    saying what a `{K: V?}` map's presence test is. The historian's prediction
    stands beside it: Go named that question `v, ok` and Kotlin named it
    `containsKey`, and both did so as an ADDITION beside a form that still
    compiled. If the answer is a distinct spelling, it is a sitting of its own.

    **The repair.** The correction is added underneath the struck row with its
    date, the document being append-only: the reason held for every map until
    panel 160 refused `.is_err()` on a value whose payload is itself fallible,
    and on a `{K: V?}` the second spelling is now a compile error — so there are
    not two spellings of one predicate there, there is one, and it is `match`.
    **The row stands and its reason narrows**: `has(m, k)` is redundant wherever
    `V` is not fallible, which is every map in this repository today.

    What it does not settle is whether the presence question on such a map
    deserves a spelling of its own. That is panel 160's open one, and the
    historian's prediction sits beside it.
