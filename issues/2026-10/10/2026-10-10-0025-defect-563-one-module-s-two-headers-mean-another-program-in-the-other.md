---
kind: defect
area: spec
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **563 — one module's two headers mean another program in the other order, and spec § 4 says declaration order never matters** | the spec-warden's `cfgone`: one module binds `a.h` (`#define LIMIT 100`) and `b.h` (`#ifndef LIMIT` `#define LIMIT 10`, and `cap`); in the order a, b it prints `3 50`, swapped `3 10` with a clang `-Wmacro-redefined` warning on a correct program, both at exit 0, and spec § 4 (`:113`) reads *Declaration order never matters*; `one` (defect 538's own shape, one module, two groups whose headers cannot share a unit) is refused at `build` where the spec says nothing refuses it; per-module units (panel 202 R1) do not reach one module | spec § 4, and the routes panel 202 R4 names for a sitting of its own once R3's instrument exists: a canonical header order, R3's comparison over the two orders, per-group units, or a sentence · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`, R4): the spec-warden's `cfgone` and the critic's `one`, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): the spec false at exit 0 (CLAUDE.md § 12: spec beats compiler).
