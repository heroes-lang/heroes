---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: 27980034581ae2829aed9f484d3600fd9bad8e05
github: none
---

- [ ] **373 — a CRLF file moves a diagnostic's column at a line's end by one** | 12 of 137 check cases converted to `\r\n` give a diagnostic at a line's end one column more than the same case with `\n`, `expected_expression` at 13:36 becoming 13:37 in `continuation-outside-brackets-in-other-words`, `expected_extern_header` at 17:34 becoming 17:35 in `fixedbugs-130-an-extern-group-whose-header-failed` (the base's compiler at `00217c39`, measured by the coordinator at 02:35 on 2026-10-06, over the critic's converted copies) | `selfhost/source.hero`, the column counted past the `\r` a line end holds · panel 193's R1, whose lines split at `\n` only · **class: adjacent**

    **Origin:** panel 193's completeness critic, its second pass (`docs/panel/193-reports/completeness-critic.md`), 2026-10-06; reproduced by the coordinator at 02:35 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, its column one past the line end on a Windows line end.

    Repaired at `27980034`, 2026-10-06 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The column is counted in `selfhost/char_column.hero`, which `source.line_col` and the token dump's walk ask: a `\r\n`'s line feed takes the carriage return's column, the line's end, and a `\r` anywhere else stays a character. One golden moved by hand, `fixedbugs-135-crlf-...` line 39 from 15 to 14, the column the same file with `\n` gives.
