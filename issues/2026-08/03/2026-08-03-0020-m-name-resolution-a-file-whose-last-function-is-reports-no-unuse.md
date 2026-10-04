- [ ] **M-name-resolution** | A file whose last function is `function simplify(e: Expr) -> Expr` / `???` reports no unused bindings anywhere — not even in the function at the top. Which field decides it, and where is it computed (hint: not in the walk)?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`has_hole`), §4.16
    **Why it matters:** a hole at the bottom of the file suspends a rule at the top of it, so the decision cannot be made while walking
