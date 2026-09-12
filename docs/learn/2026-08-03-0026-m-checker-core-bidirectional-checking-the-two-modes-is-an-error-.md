- [ ] **M-checker-core** | Bidirectional checking, the two modes. `x = []` is an error and `xs: [int] = []` is not — which mode is each, and why can exactly four forms not synthesise?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/mod.rs (the table), expect.rs
    **Why it matters:** §4.5's promise that errors stay local is this table and nothing else
