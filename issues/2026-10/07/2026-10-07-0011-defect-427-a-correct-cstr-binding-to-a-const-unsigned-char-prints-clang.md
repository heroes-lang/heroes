---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 7a4090f2f1286c66462196a2954268c23773386f
github: none
---

- [ ] **427 — a correct `cstr` binding to a `const unsigned char *` prints clang warnings** | every `cstr` lent to `const unsigned char *`, `const signed char *` or `const uint8_t *` builds and runs and prints `-Wpointer-sign` warnings, 14 for the w411 run case's first draft, on the base too | the cast the emitter writes at a `cstr` lend, `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program is `blocking` by the class's own list.

    Repaired at `7a4090f2`, 2026-10-07 (lane b13-w411): a `cstr` passed to a `const` pointer spelled other than `const char *` or `const void *` is cast to the header's own type at the call and in the probe, 411's refusal unchanged since the pointee check runs before any unit compiles; a non-byte pointee told `ffi_parameter_type`; cases `run/fixedbugs-427-*` and two refused shapes; gated by its cases, the compiler's own tests, every form whole on this Mac and its cases on Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.
