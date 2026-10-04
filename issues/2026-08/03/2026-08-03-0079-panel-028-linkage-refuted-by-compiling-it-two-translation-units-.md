- [ ] **panel 028** | **`weak` linkage, refuted by compiling it.** Two translation units, one mangled symbol, different bodies: zero diagnostics under `-Weverything`, exit 0, and one module printed `4` where its own source says `6`. Question: which existing hazard in this project is that the same shape as, and why does `_Static_assert(HERO_RUNTIME_ABI)` not catch it?

    **Where to look:** docs/panel/028 R3b · archive/bootstrap-rs/heroes-cli/src/commands/toolchain.rs
    **Why it matters:** it is the build's documented ghost promoted to a language mechanism
