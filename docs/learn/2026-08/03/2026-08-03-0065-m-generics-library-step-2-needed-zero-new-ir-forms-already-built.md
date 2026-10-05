- [ ] **M-generics-library step 2** | `m[k] @ v` needed **zero new IR forms** — `ir/places.rs` already built the step. Count: how many files changed to make a map assignable, and how many would `set(m, k, v)` have needed? (The panel costed the second at three passes plus `Arg::InOut` on a builtin.)

    **Where to look:** docs/panel/026 R2 · archive/bootstrap-rs/heroes/src/emit/aggregate.rs (the map arm of write_element)
    **Why it matters:** the cheap spelling was already in the IR and the expensive one was in the proposal
