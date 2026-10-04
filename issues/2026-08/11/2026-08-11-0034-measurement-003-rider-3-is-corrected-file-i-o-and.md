---
kind: decision
area: none
milestone: none
filed: 2026-08-11
commit: 81402348f23e73819b60cf1975b49c6bb1214049
github: none
---

2026-08-11 | **Measurement 003 rider 3 is corrected: file I/O, `args()` and `exit(code)` are a route M7 decides, not settled plain `extern`s.** The audit concluded "the spec gains **nothing** for them"; compiled against the real headers with the emitter's own type mapping, `extern function exit(code: int)` is `conflicting types for 'exit'` (`int64_t` vs `int`), and `fopen`/`fread` fail on `FILE *` and `size_t`. The mortgage is **≥60 spec tokens**, and it is the first entry in panel 024 R2's ledger | panel 013's 9-of-9 callback finding, reproduced on the closure list's own last three rows: the boundary lacks `c_int`, `size_t` and `const` (Part 7 item 10), and the route that *appears* to work — dropping the `#include` — is the silent-wrong-answer class §4.19 exists to prevent, since `void *fopen(const char *, const char *)` links by accident on arm64 | §4.19, §4.20, §1.6 | 030, 013 |
