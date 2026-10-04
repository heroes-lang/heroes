- [ ] **M-strings-ownership.2** | Three forms of the "every owned slot is released" check failed before one worked, and the third failure was invisible: the check stayed silent on a hand-broken sweep. Say what a store to a counted slot has in common with a sweep, and what separates them

    **Where to look:** ir/phases.rs (released_on_return)
    **Why it matters:** an invariant that cannot fail on a hand-made violation is not an invariant
