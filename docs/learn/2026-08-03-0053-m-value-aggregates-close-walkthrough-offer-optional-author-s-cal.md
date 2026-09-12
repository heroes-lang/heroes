- [ ] **M-value-aggregates close** | **Walkthrough offer** (optional, author's call): the path of one `g.rows[0].cells[0] @ 7` from source to C, through `sized.rs`'s order, `counted.rs`'s answer, `own.rs`'s incref, `aggregate.rs`'s lvalue walk, and the three runtime primitives

    **Where to look:** docs/records/journal/008-aggregates.md · tests/golden/run/adversarial-cow-per-step.hero
    **Why it matters:** it is the one path in the compiler where four passes each contribute one line to the same statement
