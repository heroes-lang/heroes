- [ ] **M-generics-library step 6** | `Checked::counted` is dense over the interner and its own comment says nothing may intern after it. Task: say what `[str]` interned by the pass would have read as without `extend_counted`, and why one instantiation is not enough to test it — `run/generics.hero` uses two on purpose

    **Where to look:** archive/bootstrap-rs/heroes/src/types/counted.rs · archive/bootstrap-rs/heroes/src/types/mod.rs
    **Why it matters:** "a leak that no test can see", in the comment's own words, and the test that sees it
