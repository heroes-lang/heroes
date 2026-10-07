---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 3c8ca6668eea5fa733bf5ba109955804818958eb
github: none
---

- [ ] **313 — an outer list never closed, its inner list closed elsewhere, may give a spurious message, unrun** | defect 204's shape nested: an outer `[` never closed around an inner list the lexer closes elsewhere; the lane suspected a spurious message and did not run it (2026-10-04): a question until the shape is run | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defect 204 · **class: improvement**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), an unrun question.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed; if run and spurious, `adjacent`.

    Repaired at `3c8ca666` (2026-10-07, lane b14-parse), gated by its cases; the net is owed at the batch's close. Measured on `dad2da47`, six shapes, `check` and `check --permissive` alike: no spurious message. The outer `[` keeps the lexer's own report, still open where a line ends its reach, since an opener the inner list stands in is named with it only where a closer further down closes that opener too (`bracket_head.openers_around`, defect 307's rule); the binding's report names the inner `[` and where its `]` goes, and the stray closer is told. The case pins the 18 reports.
