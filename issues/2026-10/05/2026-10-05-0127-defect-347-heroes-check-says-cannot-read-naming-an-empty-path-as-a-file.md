---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: 35ff902445ea96a7b9567267a14d8b8e5efde753
github: none
---

- [ ] **347 — `heroes check ''` says *cannot read ``*, naming an empty path as a file** | `heroes check ''`: *cannot read ``*, where an empty argument names no file at all (lane b11-windows, this Mac, 2026-10-05, the lane's report) | `selfhost/cli/input.hero` (the file read) · defect 295, which told a missing file from an unreadable one · **class: improvement**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 281's repair (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, about an argument nobody types on purpose.

    Repaired at `35ff9024`, 2026-10-05 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every verb that takes an operand said the same at the base, and `mutate` and `probe` *no file or directory is named* over nothing; each now refuses the empty word in the parser, at exit 2, as `-o ""` is.
