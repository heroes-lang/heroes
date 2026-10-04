- [ ] **M-name-resolution** | `total: int @ 0` then `total @ 1` and nothing else: one diagnostic. Now add `print(total)` at the end: zero. Which counter changed, and why is the *initialiser* not counted as a write?

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/scope.rs (`report_unused`), tests/unused.rs (2026-08-19)
    **Why it matters:** if the initialiser counted, the unused rule would be a no-op for every cell in the language (the bootstrap's test tree, archived 2026-08-19)
