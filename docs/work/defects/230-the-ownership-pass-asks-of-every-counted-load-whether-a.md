- [ ] **230 — the ownership pass asks of every counted load whether a write in its block lets it survive, a walk of the block each time** | lane irverify's profile: `ir/own.run` → `place_store.load_survives_write`, 33 to 35% of `one-return-many-800`, `many-params-800` and `record-literal-2000` (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/ir/own.hero`, `selfhost/ir/place_store.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, the same profiles, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a cost past panel 184's R6 floor, which the shapes meet.

    **2026-10-03, lane b8-emit, the ownership pass costs a block's length**: repaired at `23579605`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The per-load walk was one of three costs of the square, and counted on the item's own shapes not the largest: rule 5's pushes copied the whole output block (a `.must()` in the pushed instruction kept the place store from growing it), 17,961,115 instructions copied on one-return-many-800 and 72,133,964 on record-literal-2000, now 58,870 and 90,479; `writes_root` 1,598,832 calls to 6.
