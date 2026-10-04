---
kind: defect
area: emit
milestone: none
filed: 2026-09-30
commit: 6708ba0521af542def2be41ff0798db7b2e3d518
github: none
---

# Defect 140 closed: no walk over a record type recurses on its depth, and none gives up at one

- [x] **140 — records nested by value a thousand deep abort `build`** | 1,000 flat declarations, `record R<i>` holding `R<i-1>`, reached by `xs: [R999] = []`: `check` exit 0, `build` exit 134, `panic: stack exhausted in emitsynth.collect`; at 10,000 under a larger stack clang itself crashes on the C | `selfhost/emit/` (`emitsynth.collect`) · `selfhost/check/decls.hero` (`.record_decl`, where a bound would stand) · **closed 2026-10-01**

    **Origin:** panel 184's compiler-engineer (`184-reports/compiler-engineer.md`
    § A fourth kind of deep), 2026-09-30; reproduced by the coordinator at
    00:10 on 2026-10-01 on the integrated trunk at `3cc3b553`
    (`scratchpad/p184/newdef/deep1000.hero`; `deep700.hero` builds). No line of
    the source nests anything, so no counter of openers sees it.

    **Why it is a defect.** A program `check` accepts aborts `build`, outside
    the exit contract; panel 184's R7 files it apart from the source's depth.

    **2026-10-01, lane emit, every walk over a record type keeps its own
    stack, and the two bounded at 16 are total**: repaired at `4804d3f3`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **2026-10-01, lane emit, beside it: the union rule's reach descends a
    variant's cases**: repaired at `24bbb539`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane emit, `4804d3f3`, and beside it `24bbb539`. **The
    class**: every walk over a record type recursed once per record on the
    route, so records nested by value a thousand deep stopped `build` at 134
    in `emitsynth.collect`, and from seven hundred deep `check` itself,
    wherever a value of them was compared or used as a map key, in
    `checkpartial.reaches_within`. Eight walks keep a stack or a queue of
    their own now: `synth.collect`, `typeorder.visit` (the recursion's
    post-order, item for item), `partial.reaches_within`,
    `map_keys.reaches_float`, `reaches.walk` and `reaches.gather`, the gate's
    pointer walk and `extern_union.reach`. The last two were bounded at 16
    and silent past it: a key holding a `ptr` seventeen records down built
    and aborted at its first insertion with *entered unreachable code*, and
    `==` over a union sixteen down built and compared its bytes; both are
    total now and refused at every depth. Beside it, clang: the completeness
    probe's `{0}` for a nested record slot made clang SIGSEGV a thousand
    structs deep, so it is `{}`, measured to give the same verdicts in seven
    shapes. And beside that, `24bbb539`: the reach that finds the unions a
    compared or hashed type holds stopped at a variant, so `==` over a variant
    whose case holds a union built and printed `true`; it descends every
    case's fields now, the refusal `ffi_union_field` unchanged.

    **Cases**: `run/fixedbugs-140-records-`, `-variants-` and
    `-extern-records-a-thousand-deep-build`;
    `check/fixedbugs-140-a-partial-`, `-a-float-key-` and
    `-a-handle-key-a-thousand-deep-is-refused`;
    `unsupported/fixedbugs-140-a-pointer-key-seventeen-deep-`,
    `-a-union-compared-sixteen-deep-` and `-a-union-in-a-variant-case-is-refused`;
    the trunk fails or wrongly accepts all nine. **Clang's own edges,
    measured by the lane** (Apple clang 21, arm64 macOS): records nested
    7,000 deep build and 8,000 kill clang by signal, variants 2,000 and
    2,500. They are facts about the C compiler, not refusals of this one: a
    refusal by `check` at a depth would be a new diagnostic class, panel
    184's R7 files the depth of types apart, and none is landed. **Left
    open, reported**: a correct program that only reads a union record
    naming two members builds with clang's *excess elements in union
    initializer* warning, from the probe's `{0,0}`
    (`scratchpad/lane-emit/pass1/union2/read.hero`), lane emit's next item.

    **The gates.** Each repair by its cases and the compiler's own tests, and
    the lane's `extern` census (377 files) after each: 0 moved for 138, 204
    for 144 (the eight macro lines alone), 1 for 145 (the sqlite3 golden's
    note), 18 for 140 (23 probe lines alone), 0 for `24bbb539`. **The batch
    gate**, lane emit's closing commit `6df3d121`, the trunk merged twice,
    records only (`3c895b69`, `abf39e3d`): the seed regenerated once, the
    fixpoint by `cmp`; the compiler's own tests 967 and the net's own 184,
    all passed; `tests/emission` re-blessed and proved, 302 traces modified,
    2,416 lines the eight result macros and 23 lines a probe's `{0}` to
    `{}`, nothing else; the full net, 25 suites four at a time and `cache`
    alone, every one 0 failed (annotations 413 after its floor rose from
    1,652 to 2,148 as it asked, check 336, emission 646, fixes 586, run 215,
    unsupported 38, warnings 276 among them; `probe` 21 and 1 in the parallel
    pass on the harness's timeout, 24 and 0 alone); the census of `check
    --brief` over 1,484 files, both arms, 9 and 10 moved, every one the
    batch's own cases and the 138 origin above. The trunk fast-forwarded to
    `6df3d121` at 17:44 on 2026-10-01. **Linux x86-64** on `6df3d121`, under
    emulation, Debian clang 22.1.8: the compiler's own tests 967, all passed,
    and 18 of the 19 suites 0 failed; `run` read 210 and 1 in that pass,
    which records counts only, and 211 and 0 alone in the lane's second
    container pass, so the red did not stand alone and its cause is not
    measured. In that second pass: `unsupported` 36 and 0, `run
    fixedbugs-140` 3 and 0 (the three cases a thousand deep build),
    `fixedbugs-144` 1 and 0, `long double` into `f64` refused, the `{}`
    probe compiled with 0 warnings. **Owed before the push**: Linux arm64
    and the Windows box, on the trunk.
