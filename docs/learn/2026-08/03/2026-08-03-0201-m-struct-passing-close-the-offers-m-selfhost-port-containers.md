- [ ] **M-struct-passing close — the offers** | M-selfhost-port, containers | **This morning's bootstrap fixedbug is now pinned in the port's own test.** What are the exact three lines xs[i].f @ v emits, and which of the three was missing in each of the two broken versions?

    **Where to look:** emit_container.hero, the fixedbug test; tests/golden/run/fixedbugs-a-field-stored-behind-an-index
    **Why it matters:** the defect found by the port, fixed in the bootstrap, and now guarded in BOTH compilers — the full circle
