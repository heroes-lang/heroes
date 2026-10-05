- [ ] **M-generics-library spec repair** | Count: 23 built-ins, three spellings the spec uses for a name (`` `join` ``, `range(`, `` `.must()` ``), and one English word that must not count as a mention. Question: which built-in is named in the spec by *only* the third form, and what would a bare word-boundary search have wrongly reported struck?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/tests/spec.rs (`mentions`)
    **Why it matters:** a test over prose is only as good as the form it matches, and the loose version passes while meaning nothing
