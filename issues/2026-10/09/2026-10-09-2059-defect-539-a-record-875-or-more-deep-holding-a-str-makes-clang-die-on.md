---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **539 — a record 875 or more deep holding a `str` makes clang die on the emitter's `= {0}`** | `heroes build` exits 2, *clang died (Segmentation fault: 11)*, with a note blaming the C compiler's limit; depths 33 to 750 build, 875 and 1,000 die; an `i64` at the bottom builds at 7,000; the same C zeroed with `__builtin_memset` builds, links and prints at 1,000 (panel 200's compiler-engineer and critic) | the zeroing of a slot in `selfhost/emit/` · panel 200 R5 · panel 182 · defects 140 and 170 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program the emitter can avoid, and a note blaming the wrong limit.
