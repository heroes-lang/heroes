---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **518 — a Windows binary run by hand at `-O0` gets llvm-symbolizer's `.c` line in a sanitiser's report** | defect 509's repair sets `external_symbolizer_path=` for `run` and `test`; a binary run by hand keeps llvm-symbolizer, which names the last file block's line (`.c:122`) for every address of a function whose line table spans the `.hero` and the `.c`, and clang 23's dynamic ASan runtime ignores `__asan_default_options` (lane b15-runtime, on the box) | `selfhost/cli/symbolized.hero`, the emitted program's start · defect 509 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true report naming a less exact line.
