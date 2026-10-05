---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **355 — the runtime's failure printer and `heroes test` write a message's control characters raw, and cut it at a NUL** | a `.must()` panic or an `assert` whose message holds ESC writes `1b 5b 32 4a`, a clear screen, raw to stderr; `heroes test` prints a failing title's ESC raw; a message holding a NUL is cut there (panel 192's critic, 2026-10-04, F11; the ffi-pragmatist, F4) | `runtime/parts/failure.c:116`, `:135`, `:145` (`%s`) · the test runner's output · panel 192's R7 (one printer for both causes) · **class: adjacent**

    **Origin:** panel 192's critic, its first pass (F11), and the ffi-pragmatist; filed by the synthesis's R12.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): the reader's terminal driven by a message's bytes and a message cut short; not one of `blocking`'s list.
