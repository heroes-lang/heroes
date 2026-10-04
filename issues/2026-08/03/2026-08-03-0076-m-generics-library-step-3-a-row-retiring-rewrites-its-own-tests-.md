- [ ] **M-generics-library step 3** | **A row retiring rewrites its own tests, and that is the design.** Three unit tests used `sort` and `join` to demonstrate "not emitted"; both acquired entry points, so all three went red. Count: how many golden and unit cases changed in this step *because a refusal became a capability* rather than because behaviour changed?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/tests/gate.rs · tests/golden/unsupported/three-capabilities.hero (rewritten a seventh time)
    **Why it matters:** "a row nobody can make fire is a row nobody can retire" — the cost of that rule, paid
