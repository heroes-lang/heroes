- [ ] **M-module-namespace step 1** | **The spec-against-the-compiler test fired before any golden did.** Amending spec line 6 turned `measure::spec::the_spec_never_uses_a_word_the_compiler_rejects` red with `left: ["use"]`, because `use` was still in the lexer's foreign-word table. Question: which of the two tables in `lexer/keywords.rs` a word lives in decides *what kind of message* it gets — and why did moving one word between them cost exactly one line in each?

    **Where to look:** archive/bootstrap-rs/heroes/src/measure/spec.rs · archive/bootstrap-rs/heroes/src/lexer/keywords.rs
    **Why it matters:** the spec is tested against the compiler, and here it was the spec that moved first
