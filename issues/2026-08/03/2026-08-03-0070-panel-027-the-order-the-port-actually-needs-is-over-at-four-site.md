- [ ] **panel 027** | The order the port actually needs is **`sort_by_key` over `span.start`** at four sites (`types/mod.rs:197`, `resolve/mod.rs:195`, `ir/mod.rs:258`, `emit/gate.rs:108`), and **neither** branch of the proposal supplies it — a structural `cmp` would sort `Diagnostic` by *kind*. Task: write the Heroes signature that does serve those four sites, and say which M-generics-library step has to land before it can be written

    **Where to look:** archive/bootstrap-rs/heroes/src/diagnostics/mod.rs:71-83 · design.md §4.12
    **Why it matters:** the closure list has no comparator form, and this is the M-generics-library audit's real question
