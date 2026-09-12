- [ ] **panel 029** | **A live defect, and the generated C said so itself.** `xs: [()] @ []` checks clean, builds, and aborts with `entered unreachable code — this is a compiler bug`; the emitted C reads `hero_unreachable(); /* not an array */`. Task: find the one line of `gate.rs` that lets it through, and say why `[ptr]` needs a *typed* annotation today but will be *inferred* after step 6

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/gate.rs:146 · docs/panel/029 R4b
    **Why it matters:** a comment in generated C nobody reads is a diagnostic nobody receives
