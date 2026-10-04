- [ ] **M-struct-passing close — the offers** | M-selfhost-port (the counter class) | Six sites in the port skipped a hand-kept index because a `continue` in a match arm is a loop jump. Given `for decl in decls` with `index @ index + 1` at the bottom, what does the language offer as a no-op arm — and why is `_ = index` not it?

    **Where to look:** selfhost/check/sized.hero::collect, selfhost/ir/verify.hero, docs/debrief/DECIDE.md's item (2026-08-26)
    **Why it matters:** the class cost a false `no_size`, every C type name off by one, and a verifier that refused every program with a `.must()` (`docs/debrief/` was re-cut into `docs/work/` on 2026-08-26)
