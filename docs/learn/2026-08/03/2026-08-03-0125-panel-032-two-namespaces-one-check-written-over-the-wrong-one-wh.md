- [ ] **panel 032** | **Two namespaces, one check, written over the wrong one.** What you type before the dot and what reaches the linker have different collision sets: last-part components collide 22-way on `mod` over the port's 169 paths, whole-path concatenation collides zero times. Question: which of the two does `module_names_collide` compare today, and construct the legal program it lets through

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/graph.rs · archive/bootstrap-rs/heroes/src/source/files.rs
    **Why it matters:** the answer is a program that exits 2 saying the compiler is wrong
