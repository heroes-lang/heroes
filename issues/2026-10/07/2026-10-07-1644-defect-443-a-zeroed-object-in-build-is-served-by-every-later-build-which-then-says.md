---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **443 — a zeroed object in `build/` is served by every later build, which then says clang refused the generated C** | zero `build/runtime-*.o` or a `build/tu-*/x-*.o` at its length and build again: every later build exits 2 with `ld: unknown file type` and then *error: clang refused the generated C*, which is false, until `build/` is deleted by hand; the recovery half of defect 357's object row, which was repaired only by flushing before the rename (lane b14-cli's measurement, reproducer in `<scratchpad>/batch14/cli/p435/shapes`, not re-run by the coordinator) | `selfhost/cli/`, the build cache's reuse of an object · defect 357 · **class: blocking**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 435).

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false message at exit 2, lasting until the cache is deleted by hand.
