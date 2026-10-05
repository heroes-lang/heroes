- [ ] **M-module-namespace step 1** | **A formatter test found a real defect on its first run**: the blank line between a file's header comment and the first `use` was being deleted, which under §4.1 turns a remark about the file into documentation for the line below. Task: say why the `Decl` arm already had the guard and the `Use` arm did not, then say why the same guard must NOT fire between two `use` lines

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/fmt.rs · archive/bootstrap-rs/heroes/src/printer/tests/modules.rs
    **Why it matters:** adjacency is meaning in this language, so a blank line is not whitespace
