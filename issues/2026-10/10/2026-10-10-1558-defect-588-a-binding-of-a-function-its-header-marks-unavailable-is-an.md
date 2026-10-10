---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 16cad3ffd9885fb1f67e7a21bafd4c69e17650af
github: none
---

- [x] **588 — a binding of a function its header marks `unavailable` is an internal error at exit 2** | a program binding a function the header marks `__attribute__((unavailable))`, never calling it, exits 2 with *internal error: compiling the generated C failed* and clang's text at the unit's lines, on this Mac and Linux arm64 (the seat's `repro/shapes/unav_bound.hero`); the program's own `extern` is the cause, so it belongs at exit 1 on the binding's line (`.claude/rules/c-boundary.md`, the six members; defects 152, 156, 158 and 164 were this shape and are closed) | the round's recovery of a clang failure into an `ffi_*` class, `selfhost/cli/` and `selfhost/emit/`; panel 208, landing with defect 584 · **class: blocking**

    **Origin:** found by panel 208's ffi-pragmatist beside defect 584 (`docs/panel/208-reports/ffi-pragmatist.md` § 3b, its runs under `.claude/worktrees/scratch-b15/208-ffi-pragmatist/`, ignored by git), filed by the coordinator at 15:58 on 2026-10-10, the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    Repaired at `16cad3ff`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R2: `emit/ffi_unavailable.hero` reads clang's `'<name>' is unavailable` wherever in the unit it lands, narrowed by `declaration()`, into `ffi_unavailable` on the binding, once, at exit 1, the header's own words in a note; the class's tenth member in `.claude/rules/c-boundary.md`. Measured: the seat's two shapes (`fixedbugs-588-*` under `tests/golden/unsupported/`, bound and not called, and called) exit 1, told once each; by hand beside them a bare `unavailable` with no words, an enumerator and a record by its tag, each exit 1 on its binding; unsupported whole 231 and 0, the compiler's own tests 1561 passed.

## The repair

Repaired at `16cad3ff`, 2026-10-10 (lane b19-dep), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 208's R2: `emit/ffi_unavailable.hero` reads clang's `'<name>' is unavailable` wherever in the unit it lands, narrowed by `declaration()`, into `ffi_unavailable` on the binding, once, at exit 1, the header's own words in a note; the class's tenth member in `.claude/rules/c-boundary.md`. Measured: the seat's two shapes (`fixedbugs-588-*` under `tests/golden/unsupported/`, bound and not called, and called) exit 1, told once each; by hand beside them a bare `unavailable` with no words, an enumerator and a record by its tag, each exit 1 on its binding; unsupported whole 231 and 0, the compiler's own tests 1561 passed.

**Closed 2026-10-10** with batch 19 (lanes b19-link, b19-dep and b19-pack, merged into the round `lane-round-b19` made from batch 18's closed round, with the trunk), its closing gate run on the round at `0edc5085`: the seed regenerated over two generations, the runtime's ABI at 30, 50,887,297 bytes, SHA-256 beginning `f091d8e3ca299237`, its fixpoint by `cmp`; the compiler's own tests 1,593, all passed; the net's own tests 332, all passed; the full net 8,036 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `spec/anchors` on a module doc of defect 588's repair whose words read as a spec citation, reworded on the same two lines (the fixpoint re-checked by `cmp`) and `spec` 23 and 0 after; six floors told outgrown and raised in the closing commit, `canonical`, `emission`, `lines`, `probe`, `records` and the net's own tests 2, 1,221, 497, 27, 28 and 332, all 0 failed, after it. Lane b19-dep's merge of lanes b19-dep and b19-pack found defect 585's true headline rewritten by defect 563's reader and repaired it in the round (`0071bdac`). A defect at the C boundary closes here under the optimistic chain; the platform its case needs judges it at the CI's legs after the push, a red leg filing a new `blocking` defect naming it.
