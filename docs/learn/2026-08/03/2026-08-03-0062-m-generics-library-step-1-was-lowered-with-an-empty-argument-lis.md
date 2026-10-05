- [ ] **M-generics-library step 1** | `abort must` was lowered with an **empty argument list**, so the only message available was "a `.must()` failed" — the one thing the reader already knows. The failure now travels with it. Question: why does the read of the failure need no synthetic slot, when panel 021 had to route `assert`'s counted operands through one? (The answer is one word about *where* the read happens.)

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/fallible.rs (the `must` arm) · archive/bootstrap-rs/heroes/src/ir/asserts.rs · docs/panel/021
    **Why it matters:** it is the same hazard in two constructs, and only one of them has it
