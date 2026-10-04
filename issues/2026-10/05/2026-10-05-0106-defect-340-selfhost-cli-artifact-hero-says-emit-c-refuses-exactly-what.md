---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **340 — `selfhost/cli/artifact.hero` says `--emit-c` refuses exactly what `build` refuses, and `ffi-missing-link` is built by one and refused by the other** | `heroes build --emit-c` over `tests/golden/fixedbugs/ffi-missing-link.hero` exits 0 and writes C, where `heroes build` exits 1 at the link `--emit-c` does not run; the module's comment says the two refuse the same programs (lane b11-misc, 2026-10-05, measured, the lane's report) | `selfhost/cli/artifact.hero` (the comment) · defect 298, which corrected the same premise in `suite_emission.hero` · **class: adjacent**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 298's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a comment that is false of the compiler, no message or program moving.
