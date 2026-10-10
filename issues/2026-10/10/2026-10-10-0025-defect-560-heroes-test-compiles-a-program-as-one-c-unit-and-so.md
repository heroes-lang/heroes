---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: 960fcda70774cfdbbc75533e690805ce7ddca756
github: none
---

- [ ] **560 — `heroes test` compiles a program as one C unit, and so computes or refuses another program than `build`** | panel 202's cases: `fp` (two modules, each header defining a `static inline twice` of another type) builds and prints `6 8`, and `test` exits 1; `macro3` builds to 8 and `test` computes 400; `cfg` builds and runs to `3 10`, and `test` fails a correct test with `left: 50 right: 10`; `shim2` (two modules each holding the identical `static inline` shim design.md `:568` prescribes) builds and prints `3 2`, and `test` refuses it, *redefinition of 'hero_WEXITSTATUS'* | `selfhost/cli/assemble.hero:99` (`site.is_tests`), and `emit/unit.hero`'s root shim, which must carry the test runner so the program's `main` does not run before the tests · panel 202 R3 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): `fp` the sitting's own case, `macro3` and `shim2` the ffi-pragmatist's, `cfg` the spec-warden's, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a correct program refused, by `test`.

    Repaired at `960fcda70774cfdbbc75533e690805ce7ddca756`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `heroes test` compiles a unit per module as `build` does: each module's unit defines its own tests (a unit plan carries the build's target, `emit/unit_plan.hero`), and the root's `main` is the runner of every module's tests, each declared there (`emit/unit.hero`'s `root_shim`), so the program's `main` runs before none of them; the fused round is gone. `fp`, `shim2`, `macro3` and `cfg` pass `test` as they build (`fptest` 3 tests, no line of `main` printed), and `onedef` is refused `ffi_defined_twice` by `test` as by `build`. The `run` form asks `heroes test` of a case holding a test block. Cases `run/fixedbugs-560-*` (4); unsupported 212 and 0, corpus 55 and 0, emission 1114 and 0, the compiler's own tests 1,554 passed and the net's 326; the compiler's own tests 67.95 s and 80.21 s real fused, 111.96 s cold and 74.24 s warm per module.
