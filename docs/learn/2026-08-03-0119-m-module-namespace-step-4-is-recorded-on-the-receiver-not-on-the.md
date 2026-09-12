- [ ] **M-module-namespace step 4** | `Ref::Module` is recorded on the **receiver**, not on the call. Task: say why the check has to happen before the receiver is synthesised, then find the two later passes that read it and say what each would have done with `geom.dist2(a, b)` without it — one of them produced `hero_unreachable(t1, t2)` in real C during this step

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/qualified.rs · archive/bootstrap-rs/heroes/src/types/calls.rs · archive/bootstrap-rs/heroes/src/ir/calls.rs
    **Why it matters:** three tokens with two meanings, decided once because Part 5 erases one of them
