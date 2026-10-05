- [ ] **M-module-namespace close** | **Walkthrough offer** (optional, author's call): one `geom.dist2(a: p, b: q)` from source to C — the parser building a `Method` node, `resolve/qualified.rs` recording `Ref::Module` on the *receiver*, the checker refusing to prepend it, the IR lowering it as a plain call, and `h_geom_dist2` coming out of a component the file table owns

    **Where to look:** docs/records/journal/011 · archive/bootstrap-rs/heroes/src/resolve/qualified.rs
    **Why it matters:** it is the one path where the same three tokens have two meanings and four passes have to agree which
