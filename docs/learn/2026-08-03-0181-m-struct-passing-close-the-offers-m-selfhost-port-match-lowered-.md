- [ ] **M-struct-passing close — the offers** | M-selfhost-port, match lowered (ir_lower.hero) | **There is no match in the IR — find the two shapes it becomes.** Lower by hand: `match shape` on a 3-case variant, and `match n` on 0/1/_. How many blocks each, and why does only ONE of the two get a default-less dense switch?

    **Where to look:** ir_lower.hero's match section, the two match tests
    **Why it matters:** exhaustiveness is what buys the dense switch — the checker's proof becomes the emitter's freedom
