- [ ] **M-struct-passing close — the offers** | M-selfhost-port (ffi_record crossing) | Two rows about one struct: clang echoes `t.hero:2:12` for a probe under a `#line` — is that 2 the line in the whole concatenated text or the line within t.hero, and which one does `locate()` answer?

    **Where to look:** selfhost/emit/ffi_record.hero record_at_line, selfhost/source.hero line_of vs file_line_of
    **Why it matters:** the bootstrap compared the wrong denominator and every single-file test hid it — the port found it by having to choose between two functions whose names state the difference
