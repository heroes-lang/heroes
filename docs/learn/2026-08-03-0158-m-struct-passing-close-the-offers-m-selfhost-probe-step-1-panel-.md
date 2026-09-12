- [ ] **M-struct-passing close — the offers** | M-selfhost-probe step 1 (panel 065) | At the fixpoint, A (Rust) builds B, B emits C.c, and `diff B.c C.c` must be empty. The port forgets one `sort` on a map walk that reaches the emitted C. Which comparison fails — B.c vs C.c, or a golden `.expected` — and why can a Heroes-vs-Heroes generation *never* show it?

    **Where to look:** docs/panel/065 § the paragraph the port author will rely on
    **Why it matters:** it decides where to look on the day the fixpoint breaks, and the answer is not "everywhere": sorted order vs fixed-seed hash order are two deterministic functions of the same data
