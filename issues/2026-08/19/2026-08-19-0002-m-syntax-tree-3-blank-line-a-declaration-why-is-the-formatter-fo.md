- [ ] **M-syntax-tree.3** | `# Just a remark.` + blank line + a declaration. Why is the formatter forbidden from closing that gap?

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/tests.rs (a_blank_line_between_comment_and_declaration_is_preserved), §4.1 (2026-08-19)
    **Why it matters:** in this language whitespace carries meaning twice: indentation, and this (the bootstrap's printer tests, archived 2026-08-19)
