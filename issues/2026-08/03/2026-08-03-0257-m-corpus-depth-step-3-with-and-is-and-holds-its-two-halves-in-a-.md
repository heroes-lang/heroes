- [ ] **M-corpus-depth step 3** | `record Node` with `left: Node` and `right: Node` is `error[no_size]`, and `examples/binarytrees/` holds its two halves in a `[Tree]` instead. Say what the compiler would have to know to give that record a size, and why `[T]` answers it when a second `Node` field does not — then say which ONE line of the spec is the whole rule

    **Where to look:** spec § Types (the last bullet) · examples/binarytrees/main.hero (the header, which quotes the diagnostic) · examples/json/value.hero (a variant recursive through both `[T]` and `{K: V}`)
    **Why it matters:** every value is an independent copy, and this is the one place where that guarantee is visible as a refusal rather than as a convenience
