- [ ] **231 — every return sweeps every owned slot, so a function of many returns and many slots grows its IR and its C as their product** | lane irverify: `slots-returns-400` builds in 64.57 s after defect 218's repair, and at 800 clang does not finish in 300 s (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/ir/` (the sweep at a return) · panel 106's design · **class: adjacent**

    **Origin:** lane irverify, 2026-10-03, beside defect 218, reported to the coordinator; not yet run by the coordinator.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): real, beside the work; an 800-return function is no shape of panel 184's R6, so no floor it sets is broken.

    **2026-10-04, lane b8-emit, measured and stopped, not repaired**: after `17322f0e`, `a387bee9` and `23579605` no step of the compiler grows faster than this shape's output (none at x4.5 or more from 200 to 400 while the output grows x3.99, on an instrumented copy). The output is the product by panel 021's R3, kept by panel 106: every returning block releases every owed slot, 481,201 `decref_slot` for 402 returns and 1,200 slots at 400, 1,922,401 and 5,815,408 lines of C at 800. A repair changes what the IR does at a return (one shared exit, or a sweep pruned by liveness, which 021 R3 refused) or the C's one-label-per-block rule, a question for a sitting (CLAUDE.md § 4).
