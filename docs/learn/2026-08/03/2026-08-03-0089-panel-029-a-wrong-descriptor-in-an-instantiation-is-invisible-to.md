- [ ] **panel 029** | A wrong descriptor in an instantiation is invisible to **every** instrument this project has: clang, ASan, UBSan and the leak counter all pass, and `[-0.0] == [0.0]` prints `false`. Question: why is the memory well-formed, and which two of the five descriptor members are the only ones that differ between `int` and `f64`?

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/descriptors.rs · docs/panel/029 R4c
    **Why it matters:** the one place where "it runs clean" carries no information at all
