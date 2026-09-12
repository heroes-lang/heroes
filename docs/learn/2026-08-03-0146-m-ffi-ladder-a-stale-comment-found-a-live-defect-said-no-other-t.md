- [ ] **M-ffi-ladder** | **A stale comment found a live defect.** `emit/decls.rs` said "no other type crosses the boundary: `ffi_type` refuses them in the checker" — and nothing did. `extern function weird(n: int) -> [int]` reached clang as `call to undeclared function` plus `incompatible integer to pointer conversion`, exit 2, the compiler blaming itself. Task: say which half of that comment was true when it was written, and why the premise's death was silent

    **Where to look:** archive/bootstrap-rs/heroes/src/types/decls.rs (`ffi_signature`) · tests/golden/check/ffi-type.hero
    **Why it matters:** CLAUDE.md §11's own example, found by reading the comment rather than the code
