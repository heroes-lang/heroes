- [ ] **M-strings-ownership.1** | The pass **moves** an owning temporary into a slot instead of releasing it at the end of its block. Read the two refuted wordings in `ir/phases.rs`'s doc and say what `.must()` does to an expression that made the second one false

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (rule 5) · ir/phases.rs
    **Why it matters:** this is panel 019's slots-over-phi decision being cashed, one milestone later
