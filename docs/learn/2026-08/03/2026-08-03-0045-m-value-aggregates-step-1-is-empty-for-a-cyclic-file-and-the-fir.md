- [ ] **M-value-aggregates step 1** | `Checked::type_order` is empty for a cyclic file, and the first version published a partial order instead. Task: say what a *partial* topological order of a cyclic graph would have done downstream, and why "the emitter never runs on a program with diagnostics" is a weaker guarantee than an empty list

    **Where to look:** archive/bootstrap-rs/heroes/src/types/sized.rs (the tail of `walk`) · the test `a_cycle_leaves_no_order_to_mistake_for_one`
    **Why it matters:** it was a test that found this, one line after the code was written
