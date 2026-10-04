- [ ] **229 — the emitter's type gate walks a deep type in full at each use** | lane irverify's profile: `emit/gate.check_type` → `check_element`, about 60% of `index-chain-2000`'s build after defect 218's repair (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/emit/gate.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, the same profiles, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a cost past panel 184's R6 floor, which the shape meets.

    **2026-10-03, lane b8-emit, the emitter answers each type's questions once per program**: repaired at `a387bee9`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The gate was one of four emitter walks of the cause: the arena loops of `typeorder`, `descriptor_set` and `ctype.with_options` asked `mentions_generic` of every type id in each of 394 units (8,905,311 calls in the compiler's own emission, now 0), and `synth.collect` and `extern_union.reach` walked again for every use. Counted on an instrumented copy of each compiler: `table.get` on index-chain-2000 66,249,770 to 2,178,748, map-literal-2000 126,311,868 to 167,817; the C byte-identical.
