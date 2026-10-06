---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **427 — a correct `cstr` binding to a `const unsigned char *` prints clang warnings** | every `cstr` lent to `const unsigned char *`, `const signed char *` or `const uint8_t *` builds and runs and prints `-Wpointer-sign` warnings, 14 for the w411 run case's first draft, on the base too | the cast the emitter writes at a `cstr` lend, `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program is `blocking` by the class's own list.
