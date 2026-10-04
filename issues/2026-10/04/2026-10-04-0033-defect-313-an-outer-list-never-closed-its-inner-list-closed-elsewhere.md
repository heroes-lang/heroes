- [ ] **313 — an outer list never closed, its inner list closed elsewhere, may give a spurious message, unrun** | defect 204's shape nested: an outer `[` never closed around an inner list the lexer closes elsewhere; the lane suspected a spurious message and did not run it (2026-10-04): a question until the shape is run | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defect 204 · **class: improvement**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), an unrun question.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed; if run and spurious, `adjacent`.
