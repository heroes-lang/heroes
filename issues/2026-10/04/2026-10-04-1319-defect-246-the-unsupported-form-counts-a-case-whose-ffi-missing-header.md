---
kind: defect
area: harness
milestone: none
filed: 2026-10-03
commit: 64112e4fe0e71a27c9e153c3b52f990b152aa27f
github: none
---

- [x] **246 — the `unsupported` form counts a case whose `ffi_missing_header` or `ffi_package` message changed as skipped for a missing library, so the whole form reads green over it** | `fixedbugs-232-a-missing-header-named-with-a-noncharacter.expected` with *clang looked and did not find it* changed to *clang looked and found nothing*, then `heroes run tests/harness/main.hero -- ./heroes unsupported fixedbugs-232-a-missing-header-named-with-a-noncharacter`: *1 of 1 cases were skipped for a missing library*, red only by the third's floor; in the whole form one such case of 137 is under that floor and reads green (batch 8's round compiler in a throwaway worktree of `1eb854c3`, 2026-10-04) | `tests/harness/suite_golden.hero:210` (the skip: the said text differs from the expected and holds one of the three missing-library spellings of `shell.machine_lacks_the_library`, `tests/harness/shell.hero:582`) · the comment beside it, whose aim is that *a case whose expectation IS a missing-library diagnostic is testing exactly that* · **class: blocking**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 6, read `improvement`); measured by the coordinator at batch 8's gate, 2026-10-04.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the net reads green over a changed message (`.claude/rules/diagnostics-and-goldens.md` § An instrument watches the world), in the class batch 8 repaired in `ffi_missing_header` and `ffi_package` (226, 232, 237).

    **2026-10-04, batch 9's harness lane**: repaired at `64112e4f`, gated by its cases and the net's own tests (no `selfhost/` line moved); the net is owed at the batch's close.

    **2026-10-04, batch 9's harness lane, on the coordinator's ruling that it is this item's own cause**: the same words-only skip in `run`, `lines`, `corpus`, `warnings`, `emission`, `determinism`, `special` and the surface rows repaired at `9de942c2`, gated by its cases, the net's own tests and each of those suites whole; `suite_annotations.hero`'s `machine_answered` and its `DIRECTORIES` line are owed at the merge, another lane's file this batch; the net is owed at the batch's close.

    **2026-10-04, batch 9's annot lane**: `suite_annotations.hero`'s step-aside repaired at `a6871b1d`, asking `absence.machine_lacks_among` and naming each skip, the mark readers moved to `tests/harness/mark_readers.hero`; the floor counts marks asked only, and a third of the run roots' marks stepped aside is red. Gated by its cases, the net's own tests and `annotations`, `records`, `canonical`, `unsupported`, `permissive` and `layout` whole (no `selfhost/` line moved); the net is owed at the batch's close.

## The repair

Repaired at `64112e4f` in the `unsupported` form and `9de942c2` in eight more suites, the annotations' at `a6871b1d`. A case is skipped for a missing library only where its expectation, its source and the machine say so, the machine asked directly (`pkg-config` does not start or does not know the package, or `clang -E` does not find the header on the compiler's own search path), and each skip is named; the three expectations quoting `pkg-config` are judged against this machine's own answer. In the Linux arm64 image, at the round's head before the gate, the form passed 136 and stepped one case aside by name, where the old form had skipped four unnamed (`<scratchpad>/probe-b9-unsupported/arm64-c22.log`, 2026-10-04 09:51).

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
