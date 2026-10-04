- [ ] **M-checker-core** | Classify `break`/`continue`/`return`: they must NOT be typed `()`. RFC 1216 records that exact wrong turn ("some code in the compiler assigns type `()` to diverging expressions because it doesn't have a sensible type to assign to them"); under `()` an `int`-valued `match` with a `return` arm becomes a type error and panel 014 reconvenes

    **Where to look:** design.md §4.7 (the new bullets), docs/panel/014-match-arm-body.md
    **Why it matters:** Rust types them `!`, Kotlin `Nothing` — the historian's decisive row
