- [ ] **M-generics-library step 3** | The value is MOVED into a map and the key is COPIED, in the same call. Task: say which one `own.rs` increfed and why, then say what `hero_array_set`'s comment means by "the caller increfed before this call, which is what lets the value be something that lived inside the container the unshare just copied"

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (the indexed arm) · runtime/runtime.c (hero_map_set)
    **Why it matters:** two arguments to one function with opposite ownership rules, and the asymmetry is the correct one
