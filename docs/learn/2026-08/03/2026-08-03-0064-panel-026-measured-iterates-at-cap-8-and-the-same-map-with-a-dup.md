- [ ] **panel 026** | Measured: `{"a":1,"b":2,"c":3}` iterates `b a c` at cap 8, and the same map with a duplicated first key iterates `b c a` at cap 16 — yet `hero_map_eq` says they are equal. Question: which line of `emit/aggregate.rs` puts the *source text's* duplicate count into the data structure, and what else does that leak?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/aggregate.rs (build_map) · runtime/runtime.c (hero_map_new) · docs/panel/026
    **Why it matters:** a shipped guarantee was false for a reason nobody had looked at
