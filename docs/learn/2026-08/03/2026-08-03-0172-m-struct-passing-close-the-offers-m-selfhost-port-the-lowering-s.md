- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the lowering skeleton (ir_lower.hero) | **Flattening is the whole job.** `return a + b` becomes exactly three instructions and a terminator. Write them out by hand (what does each load produce, what does the add name), then check against the first test

    **Where to look:** selfhost/ir/lower.hero, first test
    **Why it matters:** if you can predict the instruction list, you understand why the C emitter is a printer and not a second compiler
