---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 505f3dc0187fb2276617a378a22cdf37e1a62a92
github: none
---

- [x] **587 — a macro its header marks with `#pragma clang deprecated` prints clang's raw warning at the compiler's own lines** | a program binding as a `constant` a macro the header marks `#pragma clang deprecated(OLD_MACRO)` builds at exit 0 and prints clang's `-Wdeprecated-pragma` text at the emitted unit's static asserts and accessor (`macro.c:49:29`, `:50:37`, `:73:12`), a group defect 571's guard does not name; this Mac and Linux arm64 alike (the seat's `repro/shapes/macro.hero`) | `selfhost/emit/deprecation.hero` (defect 571's lines, in batch 18's round on 2026-10-10), the probe and accessor lines; panel 208, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

    Repaired at `505f3dc0`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R1, with defect 584: `-Wdeprecated-declarations`, `-Wdeprecated-pragma` and `-Wattribute-warning` ignored over the whole unit after the groups' close (`emit/macro_guard.hero`, `emit/deprecation.hero`), the program's own lines as the compiler's; defect 571's spoken line over the definitions and its prologue and accessor toggles removed; panel 205's header region and raised checks unchanged. Measured: `tests/golden/run/fixedbugs-587-a-macro-its-header-marks-with-pragma-clang-deprecated-builds-silent` exits 0 silent and prints 5, where the base compiler printed three warnings.

## The repair

Repaired at `505f3dc0`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R1, with defect 584: `-Wdeprecated-declarations`, `-Wdeprecated-pragma` and `-Wattribute-warning` ignored over the whole unit after the groups' close (`emit/macro_guard.hero`, `emit/deprecation.hero`), the program's own lines as the compiler's; defect 571's spoken line over the definitions and its prologue and accessor toggles removed; panel 205's header region and raised checks unchanged. Measured: `tests/golden/run/fixedbugs-587-a-macro-its-header-marks-with-pragma-clang-deprecated-builds-silent` exits 0 silent and prints 5, where the base compiler printed three warnings.

**Closed 2026-10-10** with batch 19 (lanes b19-link, b19-dep and b19-pack, merged into the round `lane-round-b19` made from batch 18's closed round, with the trunk), its closing gate run on the round at `0edc5085`: the seed regenerated over two generations, the runtime's ABI at 30, 50,887,297 bytes, SHA-256 beginning `f091d8e3ca299237`, its fixpoint by `cmp`; the compiler's own tests 1,593, all passed; the net's own tests 332, all passed; the full net 8,036 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `spec/anchors` on a module doc of defect 588's repair whose words read as a spec citation, reworded on the same two lines (the fixpoint re-checked by `cmp`) and `spec` 23 and 0 after; six floors told outgrown and raised in the closing commit, `canonical`, `emission`, `lines`, `probe`, `records` and the net's own tests 2, 1,221, 497, 27, 28 and 332, all 0 failed, after it. Lane b19-dep's merge of lanes b19-dep and b19-pack found defect 585's true headline rewritten by defect 563's reader and repaired it in the round (`0071bdac`). A defect at the C boundary closes here under the optimistic chain; the platform its case needs judges it at the CI's legs after the push, a red leg filing a new `blocking` defect naming it.
