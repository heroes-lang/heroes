- [ ] **M-checker-core** | `tag = match t` with one arm `=> return 0`: legal, and `tag = match t` with *every* arm a `return`: an error. Which file decides that, and how many places in the compiler know what a diverging branch is?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/join.rs, stmts.rs (`Flow`)
    **Why it matters:** the answer is 1, and a judge's objection was withdrawn because of it
