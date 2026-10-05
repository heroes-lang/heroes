- [ ] **M-struct-passing close — the offers** | M-selfhost-port, monomorphisation (ir_mono.hero) | **Termination is structural, not a depth limit.** State the exact test that refuses f<T> calling f<[T]> but allows f<i64> calling f<i64>, and name what Rust does instead (and why that is the warning)

    **Where to look:** ir_mono.hero::recursive, the grow test
    **Why it matters:** undecidable inference (Henglein 1993) means the compiler must refuse the SHAPE — a limit would make the error message a function of compiler internals
