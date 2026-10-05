- [ ] **M-module-namespace step 4** | Four rules arrived with **no new specification**: an unused `use`, a `use` colliding with a declaration, a module in value position, and one module named twice. Task: for each, name the spec line that already covered it, and say which of the four is the one that needed a *new* message anyway

    **Where to look:** spec lines 74 and 76 · archive/bootstrap-rs/heroes/src/resolve/errors_modules.rs
    **Why it matters:** "binds" was chosen as the verb precisely to make three of these free
