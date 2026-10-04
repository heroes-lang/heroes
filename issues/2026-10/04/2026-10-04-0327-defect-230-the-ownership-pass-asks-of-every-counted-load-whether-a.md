---
kind: defect
area: ir
milestone: none
filed: 2026-10-03
commit: 2357960567ea8709f50796adb429acba1d8d61b4
github: none
---

- [x] **230 — the ownership pass asks of every counted load whether a write in its block lets it survive, a walk of the block each time** | lane irverify's profile: `ir/own.run` → `place_store.load_survives_write`, 33 to 35% of `one-return-many-800`, `many-params-800` and `record-literal-2000` (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/ir/own.hero`, `selfhost/ir/place_store.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, the same profiles, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a cost past panel 184's R6 floor, which the shapes meet.

    **2026-10-03, lane b8-emit, the ownership pass costs a block's length**: repaired at `23579605`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The per-load walk was one of three costs of the square, and counted on the item's own shapes not the largest: rule 5's pushes copied the whole output block (a `.must()` in the pushed instruction kept the place store from growing it), 17,961,115 instructions copied on one-return-many-800 and 72,133,964 on record-literal-2000, now 58,870 and 90,479; `writes_root` 1,598,832 calls to 6.

## The repair

Repaired at `23579605`. The ownership pass costs a block's length: a pushed instruction no longer copies the block, slots and values are added through no struct field, and the per-load walk is two backward walks per block (`selfhost/ir/survival.hero`): instructions copied fell from 17,961,115 to 58,870 on `one-return-many-800`. Timed at the gate, `build --emit-c`, trunk against round: `one-return-many-800` 6.30 s to 1.06, `many-params-800` 4.12 to 1.00, the same C. Every return sweeping every slot, beside it, is defect 231, which goes to a sitting.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
