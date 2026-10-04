---
kind: learn
area: examples
milestone: M-struct-passing
filed: 2026-08-03
commit: none
github: none
---

- [ ] **M-struct-passing close — the offers** | panel 092 (two declarations of one enum, both correct) | Fixing `examples/curl` needed `constant CURLOPT_URL: i32` and `function curl_easy_setopt(..., option: u32, ...)` — the same C enum declared two different ways in the same file, and the header says both. Say why, and what it means for `.to_u32().must()` appearing at the call

    **Where to look:** examples/curl/main.hero (the call and its comment) · spec:204
    **Why it matters:** the FFI's rules are C's rules, and C distinguishes an enumeration constant from an enumeration type
