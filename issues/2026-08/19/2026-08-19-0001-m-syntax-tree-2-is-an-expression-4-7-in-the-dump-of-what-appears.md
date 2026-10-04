- [ ] **M-syntax-tree.2** | `if` is an expression (§4.7). In the dump of `state = if t.done …`, what appears on the `bind` line, and where do the branches go?

    **Where to look:** archive/bootstrap-rs/heroes/src/printer/bodies.rs (write_valued), tests/bodies.rs (2026-08-19)
    **Why it matters:** one rule seen in two places is the claim; the printer is where it either holds or does not (the bootstrap's test tree, archived 2026-08-19)
