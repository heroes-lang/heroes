- [ ] **M-syntax-tree.1** | `function advance(@l: Lex)` dumps with a result type. Which one, and where did it come from, given the source never wrote it?

    **Where to look:** syntax/decl.rs (the arrow branch), printer/dump.rs
    **Why it matters:** a synthesised node is how later passes stop asking "was it written?"
