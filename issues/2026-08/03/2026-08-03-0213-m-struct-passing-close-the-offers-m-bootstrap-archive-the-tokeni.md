- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`measure`, the tokeniser) | Heroes cannot build a `str` out of arbitrary bytes, so the port could not decode the BPE table the way the Rust does. What does it do instead, and why is the answer sound rather than a trick?

    **Where to look:** selfhost/measure/bpe.hero's module doc, `spelled`
    **Why it matters:** the two implementations agree to the token on the real spec — 3440 and 3512 — and that agreement is the whole proof
