- [ ] **M-generics-library close** | **Mutation drill offer**: delete the `extend_counted` call in `ir/mono.rs`, or the origin guard in `scope.rs`, and predict which instrument fires — the leak counter, ASan, clang, the phase check, or nothing

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/mono.rs · archive/bootstrap-rs/heroes/src/resolve/scope.rs
    **Why it matters:** the milestone's own record says the answer differs per line
