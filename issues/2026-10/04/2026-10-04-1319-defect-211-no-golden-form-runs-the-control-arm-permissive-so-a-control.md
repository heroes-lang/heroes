- [x] **211 — no golden form runs the control arm (`--permissive`), so a control-arm row cannot be pinned** | `tests/harness/suite_golden.hero` has no such word (the compiler-engineer's search); 131-22's open half and V1's five control-arm hides (defect 193) have no pin | `tests/harness/suite_golden.hero` · panel 187's R3 · **class: improvement**

    **Origin:** panel 187's R3 and R10, 2026-10-03.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    **2026-10-04, batch 9's harness lane**: repaired at `a3fb46d5`, gated by its cases and the net's own tests (no `selfhost/` line moved); the net is owed at the batch's close, and so is the line that makes `annotations` sweep `tests/golden/permissive` (`tests/harness/suite_annotations.hero`, another lane's file this batch).

    **2026-10-04, batch 9's annot lane**: the line that makes `annotations` sweep `tests/golden/permissive` repaired at `a6871b1d`, the thesis witness reading only the normal arm's directories. Gated by its case, the net's own tests and `annotations`, `records`, `canonical`, `unsupported`, `permissive` and `layout` whole (no `selfhost/` line moved); the net is owed at the batch's close.

## The repair

Repaired at `a3fb46d5`, the annotations' sweep of the new directory at `a6871b1d`. A fifth golden form, `permissive`, runs `check --brief --permissive` over `tests/golden/permissive/`, so a row of the control arm can be pinned; four cases, each annotated with the control arm's own diagnostics, and a changed control-arm word turns the form red (a test of the net's own). Defect 193's control-arm half is the form's fifth case since the gate (`74863163`).

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
