- [ ] **M-struct-passing close — the offers** | M-selfhost-port, fallibles lowered (ir_lower.hero) | **Only two of the five T? forms need an edge.** Sort them: ok/fail, .is_err(), .must(), .default(v), e? — which lower to straight-line instructions and which open blocks? Then say why is_err is the cheap one

    **Where to look:** ir_lower.hero, the T? section
    **Why it matters:** the cost model of error handling IS this table: a tag comparison is free, a branch is not, and ? is a whole exit edge
