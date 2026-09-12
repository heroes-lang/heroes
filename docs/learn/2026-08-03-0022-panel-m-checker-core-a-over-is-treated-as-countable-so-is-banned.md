- [ ] **panel? M-checker-core** | A `match` over `bool` is treated as countable, so `_` is banned over it — but `true`/`false` are literals, not `.cases`, so an exhaustive `bool` match cannot be written at all today. Found while writing `patterns.rs`

    **Where to look:** archive/bootstrap-rs/heroes/src/types/patterns.rs (`wildcard`, `exhaustive`)
    **Why it matters:** the two rules that hold each other up (§4.7) do not fit the one type that is a two-case variant by construction
