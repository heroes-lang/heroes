- [ ] **M-name-resolution** | Order-free top level. `factor` and `group` call each other, and neither is declared first. How many passes does the resolver make over the declarations, and what exactly is in the table before the first body is walked?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/top.rs (module doc)
    **Why it matters:** this is why the language has no forward declarations — not "does not need", cannot express a use for
