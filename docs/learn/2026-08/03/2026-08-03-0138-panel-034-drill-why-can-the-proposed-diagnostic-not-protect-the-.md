- [ ] **panel 034** | Drill: why can the proposed diagnostic not protect the **self-hosted** compiler? The answer is in `archive/bootstrap-rs/heroes/src/diagnostics/mod.rs:74` and §1.0's error-accumulation idiom, and it says something about which of this project's instruments measure `examples/` rather than the thing being built

    **Where to look:** docs/panel/034 § Part 3 · design.md §1.0
    **Why it matters:** the harness protects the corpus, and the corpus is not the compiler
