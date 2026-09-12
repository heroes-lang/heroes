- [ ] **sweep 001** | `Resolved::module_declaring` picks the **alphabetically first** module of N: it names one the file cannot see, attaches a fix that produces `wrong_arity` if followed, and cascades a false `unused_binding` telling the author to delete the `use` line that was the real fix. It had exactly one possible answer when there was one module. Question: how many other lookups in the resolver had one possible answer at M-ffi-ladder and now pick by sort order?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs:211 · defect 001 (docs/work/DONE.md) N9
    **Why it matters:** the only defect in the sweep that is neither a widened scope nor a hand-built position
