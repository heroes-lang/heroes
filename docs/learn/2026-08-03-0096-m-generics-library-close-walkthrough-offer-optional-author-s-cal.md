- [ ] **M-generics-library close** | **Walkthrough offer** (optional, author's call): one `map(xs, show)` from source to C — `types/calls.rs` recording the instantiation, `ir/mono.rs` cloning and substituting, `counted.rs` rebuilt, the mangled hash, and the library function it lands in

    **Where to look:** docs/records/journal/010 · archive/bootstrap-rs/heroes/src/ir/mono.rs
    **Why it matters:** it is the one path where four passes and a hash all have to agree on one name
