---
kind: feature
area: records
milestone: M-buildable-structs
filed: 2026-09-25
commit: none
github: none
---

- [ ] **M-buildable-structs** | a record passed by value loses defined padding on Linux x86-64, and C can write those bytes out | `docs/panel/178-reports/completeness-critic.md` § 1 finding 1 · `docs/panel/178-reports/completeness-critic-work/w/pad/`

    **Origin:** panel 178's completeness critic, 2026-09-25, under
    MemorySanitizer with a positive control: a `{char c; double d}` record
    passed by value to a Heroes function and handed to `write()` carries
    uninitialised bytes at offset 1, at `-O0` to `-O3` on x86-64; clean on
    Linux arm64; the same with today's `partial` construction. Not a defect,
    because nothing in the spec promises padding and Heroes' own `==` and
    `hash` never read it. The question is whether records should cross calls
    by address or by `memcpy`, so that C never receives uninitialised bytes:
    the information-leak class CERT DCL39-C names, and a change to how every
    record crosses a call.
