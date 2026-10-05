- [ ] **panel 029** | **Four well-typed programs, one source text, four outputs, zero diagnostics.** `words.fold("", cat)` where `cat` takes two `str`. Task: say why `types/builtins.rs:263`'s rule cannot tell the two orders apart, then say why the language's own defence against argument-order mistakes — mandatory labels on same-typed parameters — cannot fire here

    **Where to look:** archive/bootstrap-rs/heroes/src/types/builtins.rs (the `fold` rule) · docs/panel/029
    **Why it matters:** the protection is switched off exactly where the hazard is, and the reason is structural rather than an oversight
