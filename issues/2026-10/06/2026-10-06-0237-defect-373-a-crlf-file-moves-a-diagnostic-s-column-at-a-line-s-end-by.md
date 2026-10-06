---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **373 — a CRLF file moves a diagnostic's column at a line's end by one** | 12 of 137 check cases converted to `\r\n` give a diagnostic at a line's end one column more than the same case with `\n`, `expected_expression` at 13:36 becoming 13:37 in `continuation-outside-brackets-in-other-words`, `expected_extern_header` at 17:34 becoming 17:35 in `fixedbugs-130-an-extern-group-whose-header-failed` (the base's compiler at `00217c39`, measured by the coordinator at 02:35 on 2026-10-06, over the critic's converted copies) | `selfhost/source.hero`, the column counted past the `\r` a line end holds · panel 193's R1, whose lines split at `\n` only · **class: adjacent**

    **Origin:** panel 193's completeness critic, its second pass (`docs/panel/193-reports/completeness-critic.md`), 2026-10-06; reproduced by the coordinator at 02:35 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, its column one past the line end on a Windows line end.
