---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **560 — `heroes test` compiles a program as one C unit, and so computes or refuses another program than `build`** | panel 202's cases: `fp` (two modules, each header defining a `static inline twice` of another type) builds and prints `6 8`, and `test` exits 1; `macro3` builds to 8 and `test` computes 400; `cfg` builds and runs to `3 10`, and `test` fails a correct test with `left: 50 right: 10`; `shim2` (two modules each holding the identical `static inline` shim design.md `:568` prescribes) builds and prints `3 2`, and `test` refuses it, *redefinition of 'hero_WEXITSTATUS'* | `selfhost/cli/assemble.hero:99` (`site.is_tests`), and `emit/unit.hero`'s root shim, which must carry the test runner so the program's `main` does not run before the tests · panel 202 R3 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): `fp` the sitting's own case, `macro3` and `shim2` the ffi-pragmatist's, `cfg` the spec-warden's, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a correct program refused, by `test`.
