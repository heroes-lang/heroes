- [ ] **M-handle-verdict 3** | Run `./heroes mutate examples --operator swap-ptr` and read the row: 15 mutants, 8 killed, 53%. Then run it again with `--survivors` and read the seven programs it prints. Before looking anything up, say what the 53% measures. Then take one of the eight that died, `db: db` changed to `db: statement` in `examples/sqlite/main.hero`, run `heroes check` on it, and say whether the diagnostic you get has anything to do with C handles.

    **Where to look:** `docs/measurements/029-the-sixteenth-operator-and-a-kill-rate-that-means-the-opposite.md`;
    `selfhost/mutate/handles.hero`'s module comment; the row in
    `harness/mutations/operators.md`; and § Scored at step 3 of
    `docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md`.

    **Why it matters:** the operator exists to measure one thing — a C handle
    handed where another belongs, which is defect 029 — and its headline number
    says the compiler catches half of them. It catches none of them. The eight
    deaths are `unused_binding` and `aliased_mutable_arguments`: the swap
    orphaned a name, or put two `@` on one place. Both are real rules catching
    real mistakes and neither can tell a `sqlite3 *` from a `sqlite3_stmt *`.
    The number that means what it appears to mean is **0 of 7**, the seven where
    the swap is a pure use. The lesson is not about handles: it is that **a
    mutation operator's kill rate is only a measurement if every mutant it
    plants is the class it claims** — and this one plants two classes, so its
    rate is an average of a defence and an accident. Panel 011 already forbade
    pooling operators into one headline; this is the same error one level down,
    inside a single operator, and it would have been invisible if the survivors
    had not been read one by one.
