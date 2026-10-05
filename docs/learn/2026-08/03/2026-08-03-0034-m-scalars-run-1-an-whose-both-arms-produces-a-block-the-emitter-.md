- [ ] **M-scalars-run.1** | An `if` whose both arms `return` produces a block the emitter does not print at all. Say what would happen in C if it printed the label but not the block, and what would happen if it printed neither the label nor the `goto`

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/decls.rs (reachable)
    **Why it matters:** C is physical, and this is the one place where that matters more than the CFG
