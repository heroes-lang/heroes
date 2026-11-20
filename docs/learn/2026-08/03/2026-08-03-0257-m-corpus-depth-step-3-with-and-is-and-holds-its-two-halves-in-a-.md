- [ ] **M-corpus-depth step 3** | `record Node` with `left: Node` and `right: Node` is `error[no_size]`, and `examples/binarytrees/` holds its two halves in a `[Tree]` instead. Say what the compiler would have to know to give that record a size, and why `[T]` answers it when a second `Node` field does not — then say which ONE line of the spec is the whole rule

    **Where to look:** spec § Types (the last bullet) · examples/binarytrees/main.hero (the header, which quotes the diagnostic) · `packages/core/0.2.0/files/encoding/json.hero`'s `Value` (a variant recursive through both `[T]` and `{K: V}`, which examples/json/value.hero held until 2026-11-10)
    **Why it matters:** every value is an independent copy, and this is the one place where that guarantee is visible as a refusal rather than as a convenience

    **Noted 2026-11-20:** `core` left the tree with M-rebel-library (its decision 8 of 2026-11-15, its last version removed at `7ddb4ed74`), so no version of the file *Where to look* names stands; the same shape stands in Rebel's `json.Value` (`packages/rebel/0.1.0/files/json.hero`), a variant recursive through `[Value]` and `{str: Value}`. The question is unchanged.
