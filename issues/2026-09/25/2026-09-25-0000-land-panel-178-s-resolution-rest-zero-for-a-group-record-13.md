---
kind: feature
area: records
milestone: M-buildable-structs
filed: 2026-09-25
commit: none
github: none
---

- [ ] **M-buildable-structs** | land panel 178's resolution: `rest: zero` for a group record, § 13's `char` sentence, the header initialiser as a constant (defect 094's repair) and the fixed-element write (defect 091's) | `docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md` · `docs/panel/178-reports/compiler-engineer-work/r1-and-091.diff`

    **Origin:** panel 178's resolution, items 1 to 5, 2026-09-25, provisional.
    **The order is a condition of the sitting, not a preference**: a golden for
    `missing_fields` on a Heroes record and on a group record lands first,
    because that diagnostic has no test anywhere today (the compiler-engineer,
    confirmed by the critic); the three DECIDED rows of
    `tests/harness/suite_layout.hero` the prototype moves (`ast.hero`,
    `check/walk.hero`, `print/fmt.hero`) are named before the build, and the
    construction check leaves `check/walk.hero` for its own module; the
    emission is the compound literal, never member stores into a bare cell.
    The spec text is the +32 sentence the critic priced (8393 real, digest
    `36a1c34f7eaceaba`, on a base of 8361) plus the `char` sentence, priced on
    the real instrument at the landing. The base read **8805** when this file
    was written (`517b8e25`), and the record paragraph was unchanged, so
    +32 ± 2 above it is the prediction, unrun on that base.

    **Re-read 2026-10-06**, when this item left the page for its own file: defect 091's half is done, repaired by defect 097's repair (`d64da8ff`, 2026-09-25) and re-run on `bef739dd` (`issues/2026-09/24/2026-09-24-0000-defect-091-writing-one-element-of-a-fixed-array-field-through-a-cell-is.md`); defect 094's half is open. The sitting is sat again on that day's trunk, panel 194, by the author's instruction of 2026-10-06 (`issues/2026-10/06/2026-10-06-1118-panel-178-reaches-the-trunk-sat-again-on-the-trunk-of-today.md`), so what this item owes is read against that sitting's resolution.
