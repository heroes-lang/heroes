- [ ] **M-deferral-ledger 1** | `examples/sqlite/main.hero` with `sqlite3_step(db)` in place of `sqlite3_step(statement)` builds with zero diagnostics and exits 139 (defect 029). Say which spec § 3 row makes the call legal, which spec § 9 rule catches the SAME kind of slip inside one call and why it does not reach this one, and what the `_Static_assert` probe in the emitted C is comparing when it lets a `sqlite3 *` through a `sqlite3_stmt *` parameter.

    **Where to look:** `spec/heroes-spec.md` § 3 (the `ptr` `cstr` row) and § 9 (the
    same-typed-argument rule); `examples/sqlite/main.hero:46-48,74`;
    `selfhost/emit/callback_guard.hero` and the probe line in the emitted C
    (`heroes build --emit-c`); `docs/work/DEFECTS.md` item 029 and
    `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md`
    § Found beside the sitting.

    **Why it matters:** the language promises no segfault (design.md §1.12), and
    this one comes from the one type that stands for every C pointer at once. The
    § 9 rule already forces names when two parameters share a type, so a swap
    INSIDE a call is loud; a swap ACROSS calls — a handle from one producer handed
    to another consumer — has one parameter and nothing to compare. Seeing why the
    existing rule stops exactly there is what the repair's sitting will start from.
