- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 4 (what stays silent, and why it must) | After step 4 the spec says two parameter mismatches still build: what C converts exactly (`i16` against int) and what a pointer points at. For each: is it unchecked by CHOICE or undetectable by clang? One of the two could be closed tomorrow if Heroes grew one feature — which feature, and which hole?

    **Where to look:** spec:205-207 · docs/panel/092 § What is still not checked
    **Why it matters:** a hole the document names is a contract; the pointer half waits on a typed pointer, and knowing that is knowing the FFI's roadmap
