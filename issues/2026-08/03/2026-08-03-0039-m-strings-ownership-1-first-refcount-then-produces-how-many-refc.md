- [ ] **M-strings-ownership.1** | First refcount. `s: str @ "a"` then `s @ s + "b"` produces how many refcount instructions, and which of them is the one that would double-free if the pair were reversed?

    **Where to look:** tests/golden/emit/strings.expected · archive/bootstrap-rs/heroes/src/own.rs (rule 3)
    **Why it matters:** the order of two operations around a store is the whole rule
