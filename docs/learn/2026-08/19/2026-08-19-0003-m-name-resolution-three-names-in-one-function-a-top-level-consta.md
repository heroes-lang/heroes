- [ ] **M-name-resolution** | Three names in one function: `MAX` (a top-level constant), `x` (a local), `print` (a built-in). Each records a different `Ref` variant. Which one is looked up FIRST, and what would break if the order were reversed?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/exprs.rs (`name`), tests/names.rs (2026-08-19)
    **Why it matters:** the lookup order is what makes "shadowing is an error" and "a built-in's name is taken" consistent instead of contradictory (the bootstrap's test tree, archived 2026-08-19)
