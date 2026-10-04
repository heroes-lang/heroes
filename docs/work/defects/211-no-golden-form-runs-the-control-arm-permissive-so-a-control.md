- [ ] **211 — no golden form runs the control arm (`--permissive`), so a control-arm row cannot be pinned** | `tests/harness/suite_golden.hero` has no such word (the compiler-engineer's search); 131-22's open half and V1's five control-arm hides (defect 193) have no pin | `tests/harness/suite_golden.hero` · panel 187's R3 · **class: improvement**

    **Origin:** panel 187's R3 and R10, 2026-10-03.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    **2026-10-04, batch 9's harness lane**: repaired at `a3fb46d5`, gated by its cases and the net's own tests (no `selfhost/` line moved); the net is owed at the batch's close, and so is the line that makes `annotations` sweep `tests/golden/permissive` (`tests/harness/suite_annotations.hero`, another lane's file this batch).

    **2026-10-04, batch 9's annot lane**: the line that makes `annotations` sweep `tests/golden/permissive` repaired at `a6871b1d`, the thesis witness reading only the normal arm's directories. Gated by its case, the net's own tests and `annotations`, `records`, `canonical`, `unsupported`, `permissive` and `layout` whole (no `selfhost/` line moved); the net is owed at the batch's close.
