- [ ] **M-name-resolution** | `print(totl)` next to `total = 1` used to produce **two** errors. Which one was the consequence, and what does the resolver now remember in order to stay quiet about it?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/mod.rs (`near_names`, `suggested`), journal 003 §3
    **Why it matters:** the compiler was reporting a mistake it had itself proposed the repair for
