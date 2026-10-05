- [ ] **M-generics-library step 6** | Count: for `first<T>` used at `[str]`, `[int]` and `[[str]]`, how many C functions does the emitter write, and how many does `first([10, 20])` followed by `first([30, 40])` add? Then say what the four-hex-digit suffix is a hash *of*, and why not of the `TyId`

    **Where to look:** tests/golden/run/generics.hero · archive/bootstrap-rs/heroes/src/emit/mangle.rs (`instance`)
    **Why it matters:** the answer to the second is a fixpoint argument, not a taste one
