- [ ] **M-module-namespace step 1** | **`use` is not in panel 007's ender list, and one recovery path had to learn it.** A bare `use` plants no terminator, so `recover_to_next_decl` ate the whole `function main()` below it — the failure panel 018's keyword-first shape was bought to prevent. Question: why was the repair a line-number comparison in `use_decl` rather than adding `KwUse` to `is_line_ender`, and what would the second have changed about the *language*

    **Where to look:** archive/bootstrap-rs/heroes/src/syntax/decl.rs · archive/bootstrap-rs/heroes/src/lexer/layout.rs (`is_line_ender`) · archive/bootstrap-rs/heroes/src/syntax/recover.rs
    **Why it matters:** the layout rule is a panel path and the parser's recovery is not
