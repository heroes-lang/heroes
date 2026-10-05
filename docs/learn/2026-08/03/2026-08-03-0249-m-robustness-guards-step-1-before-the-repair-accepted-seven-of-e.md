- [ ] **M-robustness-guards step 1** | before the repair, `heroes check` accepted seven of eight `@`-on-an-immutable shapes and the eighth, `add(@m["k"], "x")`, was refused as `type_mismatch`. Which single fact about the language made one rule cover all seven, and why did the map case never need it?

    **Where to look:** spec:79-82 and :88 · design.md §4.10 (every place has exactly one root) · selfhost/resolve/writes.hero (`inout_root`)
    **Why it matters:** a rule that reads the root instead of the shape is one rule, not eight
