- [ ] **M-core-packages** step 1 | `strconv`, because six programs write the same digit loop by hand | `selfhost/check/builtins.hero:73-95` · `examples/json/`, `calculator/`, `ini/`, `spreadsheet/`, `csv/`, `interpreter/` · `docs/ROADMAP.md` § M-core-packages

    **Origin:** author decision 2026-09-10, § What production-ready means row 6,
    the first of its four silences. The package table gained a `strconv` row the
    same day.

    **What is missing and how it is known**: `to_i64` does not take a `str`
    (`selfhost/check/builtins.hero:73-95` — an integer or a float, and a `str`
    argument is a diagnostic), so **every program whose input is text builds its
    numbers digit by digit**: `json`, `calculator`, `ini`, `spreadsheet`, `csv`,
    `interpreter`. `tests/harness/strings.hero` carries a `to_number` of its own,
    which is the seventh copy. Go's tree has `strconv` for exactly this.

    **Width and precision belong here too, and only here for now.** There is no
    route to a padded integer or two decimal places, and the compiler hand-writes
    four padders (`cli/measure.hero:305`, `cli/doctor.hero:161`,
    `print/fmt.hero:145`). A `f64` to two places is integer arithmetic and a point,
    so it is this package's work. **It becomes a question about the language only
    if this package cannot do it** — a format spec inside an `f"…"` hole is the
    shape it would take, and `f"…"` landing on 2026-09-09 is what makes it
    thinkable — and that is a return condition rather than a plan.

    **It adds no built-in and no language form**: panel 097 condition 5 keeps
    `selfhost/library_source.hero` closed, and this is ordinary Heroes.
