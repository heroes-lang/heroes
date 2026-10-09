---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **538 — `heroes test` tells two groups binding one C name with different types that one header does not compile** | two modules binding `a.h` and `b.h`, each defining a `static inline twice` of another type: `check` and `build` exit 0 and the program prints 6 and 8, while `heroes test` exits 1 with *`b.h`, the header this group names, does not compile*, and with the `use` lines swapped blames `a.h`; each header compiles alone; panel 200's route B for defect 453 would make `--emit-c` say the same; what `check`, `build` and `test` should say of such a program is a diagnostic class | `selfhost/emit/` and `selfhost/cli/whose.hero` (`ffi_header_refused`) · panel 200 R1 · defect 453 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the header blamed chosen by the order of the `use` lines.
