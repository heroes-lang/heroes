- [ ] **M-module-namespace step 3** | **Discovery diagnoses nothing.** A missing module and a cycle are both reported by `graph.rs` against the *finished* `Source`. Task: say why — in terms of what a `Span` may point into — and then say what makes discovery terminate on a cycle given that it is not the cycle check

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/mod.rs · archive/bootstrap-rs/heroes/src/modules/graph.rs
    **Why it matters:** the walk that terminates and the check that reports are two different things wearing one name
