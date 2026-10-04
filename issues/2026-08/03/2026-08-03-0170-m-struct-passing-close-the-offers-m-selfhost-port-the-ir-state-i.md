- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the IR state (ir_build.hero) | **Three invariants are made hard to break, not checked later.** Find where each lives: a temporary assigned once, a block ending exactly once, nothing emitted after a terminator. Which ONE of the three silently drops work instead of refusing loudly, and why is that the right polarity here?

    **Where to look:** selfhost/ir/build.hero: fresh_value, terminate, push_inst
    **Why it matters:** the builder's shape IS the verifier's checklist — each hard-to-break rule is one check the verifier recomputes
