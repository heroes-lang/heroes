- [ ] **M-value-aggregates step 5** | Three defects in one step, each a *missing row* rather than wrong logic: `Ty::Array` absent from the field walk in `perfn.rs` (aborted at `hero_unreachable`), a variant's `hash` prototyped but never defined (undefined symbol at link), and a descriptor emitted after the function that names it (undeclared identifier). Task: say which of the three the "list every arm, never a catch-all" rule caught, and which two it did not — and what would have caught them

    **Where to look:** commits after ce2276a · archive/bootstrap-rs/heroes/src/emit/perfn.rs
    **Why it matters:** the rule has a shape, and knowing its edge is knowing when to add another instrument
