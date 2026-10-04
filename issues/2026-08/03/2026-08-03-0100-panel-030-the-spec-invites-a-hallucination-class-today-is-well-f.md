- [ ] **panel 030** | The spec invites a hallucination class **today**: `extern function read_file(path: str) -> str` is well-formed Heroes, and its near-misses (`puts`, `system`, `getenv`, `atoi` taking a Heroes `str`) are real symbols that link, run, and are silently wrong. Question for M-ffi-ladder: does `str` become forbidden as an `extern` parameter type, and what does that cost?

    **Where to look:** docs/panel/030 § Q3 · design.md §4.19
    **Why it matters:** the FFI's silent-wrong-answer class, reachable before the FFI milestone exists
