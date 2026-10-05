- [ ] **M-struct-passing close — the offers** | M-selfhost-port, dominance (ir_values.hero) | **The first version of the invariant was WRONG and the assert lowering proved it.** State both versions — "read only in the defining block" vs "definition dominates every use" — and name the two blocks of an assert that tell them apart

    **Where to look:** ir_values.hero's module doc
    **Why it matters:** the weaker, TRUE statement is the one worth asserting: a lesson about invariants, not about IRs
