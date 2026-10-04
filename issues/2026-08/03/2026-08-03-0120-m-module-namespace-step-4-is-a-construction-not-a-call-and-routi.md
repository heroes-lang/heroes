- [ ] **M-module-namespace step 4** | **`geom.Point(x: 3, y: 4)` is a construction, not a call**, and routing it through the call path produced C that asked clang to call a record. Question: which existing function in `types/calls.rs` already made that split for unqualified names, and why did the qualified branch have to repeat it rather than reuse the dispatch?

    **Where to look:** archive/bootstrap-rs/heroes/src/types/calls.rs · archive/bootstrap-rs/heroes/src/ir/calls.rs
    **Why it matters:** the qualified form is an ordinary name that says where it lives, so it needs every branch the ordinary one has
