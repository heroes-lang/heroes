- [ ] **M-module-namespace step 3** | `is_library` split into two questions: **`is_root`** for the dumps and `fmt`, `is_library` for emission and `heroes test`. Task: say what `heroes test` would silently lose if it had taken `is_root`, and quote the CLAUDE.md sentence that decides which of the two each caller wants

    **Where to look:** archive/bootstrap-rs/heroes/src/source/mod.rs (`is_root`) · archive/bootstrap-rs/heroes/src/emit/decls.rs
    **Why it matters:** one predicate had been answering two questions since M-strings-ownership, and only one file made the difference visible
