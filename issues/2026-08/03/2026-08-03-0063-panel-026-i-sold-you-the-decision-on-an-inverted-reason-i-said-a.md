- [ ] **panel 026** | **I sold you the decision on an inverted reason.** I said a compiler needs incremental insertion for a symbol table. True, but `{K: V}` + insertion gives neither shadowing nor O(1) scope pop and the map has no delete — so it is the *wrong* structure for one, and `resolve/scope.rs` already chose a vector. Task: read `scope.rs` lines 41, 96, 107 and say what a map would have to gain to serve there

    **Where to look:** archive/bootstrap-rs/heroes/src/resolve/scope.rs · docs/panel/026 R5
    **Why it matters:** the funding still stands on other grounds, but not on the one I gave
