- [ ] **panel 034** | Drill: the diagnostic is **root-dependent** — the same file errors rooted at `lex` and is clean rooted at `main`, because `modules::load` loads the root plus what it names. Say which other compiler answers already depend on the root, and which of them are defensible. One of them was a shipped miscompilation until this session

    **Where to look:** docs/panel/034 § Part 3 · archive/bootstrap-rs/heroes/src/modules/mod.rs
    **Why it matters:** root-dependence is not hypothetical in this compiler and the panel found it twice in one day
