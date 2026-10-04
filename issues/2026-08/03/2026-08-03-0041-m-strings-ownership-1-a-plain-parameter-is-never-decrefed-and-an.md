- [ ] **M-strings-ownership.1** | A plain `str` parameter is never decrefed and an `@ str` parameter is never swept. Both are correct and for different reasons. Give each reason in one sentence

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (rules 1 and 2) · design.md §3.1's `@` bullet
    **Why it matters:** the convention a judge got wrong on its first compiled program, with ASan as the referee
