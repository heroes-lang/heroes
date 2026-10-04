---
kind: defect
area: compiler
milestone: none
filed: 2026-09-30
commit: b9fdb0a3d777829b2e0e62396944bd59283132cf
github: none
---

# Defect 146 closed: a file of foreign heads is read once to place its words, and the parser grows its arrays in place

- [x] **146 — the parser is quadratic on declaration heads written with a foreign word** | 1,500 `fn f<i>() {` heads parse in 0.80 s and 3,000 in 3.18 s, where 3,000 clean `function` heads take 0.06 s; lane 136 measured 6,000 at 28.5 s | the recovery after a foreign-word head (`selfhost/scan.hero`'s error token, `cursor.recover_to_next_decl`) · **closed 2026-10-02**

    **Origin:** lane 136 at its close, 2026-09-30
    (`scratchpad/lane-136/r2/big/`); timed by the coordinator at 00:09 on
    2026-10-01 on `3cc3b553`, load 1.6 (`scratchpad/p184/newdef/fn1500.hero`,
    `fn3000.hero`, `ok3000.hero`).

    **Why it is a defect.** A file of Rust habits a model writes whole costs
    time that grows with its square (twice the heads, 4.0 times the time,
    measured); at 30,000 heads that square gives about five minutes, an
    inference, unrun.

    **2026-10-01, lane recovery-b5, a file of foreign heads is read once to
    place its words, and the parser grows its arrays in place**: repaired at
    `a2af2a3f`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **The repair**, lane recovery-b5, `a2af2a3f`. **The class**, each cause
    found with `sample` on the trunk's compiler at `9d1c209d`: a foreign word
    that opens its line was placed by counting the text's brackets from its
    first byte, and one level in by a walk back to the margin
    (`word_place.inside_brackets`, `under_a_group`), several times per word;
    every parser diagnostic was appended through a field place, which copies
    the array; an `else if` chain's branches and a `match`'s arms were pushed
    with a `.must()` in the push, which keeps it from growing in place. So
    `fn f() {`, `const` in a body, `fn` members, `elif` and `import` lines cost
    their square, and so did any file of N parser mistakes or an `if` of N
    branches. Now the text's outline is read once, at the first word that asks
    (`selfhost/text_outline.hero`), its two far answers binary searches; the
    parser reads the certain swap the lexer wrote for each word
    (`selfhost/swap_words.hero`); every parser append goes through
    `cursor.push_diagnostic`, `push_held` and `push_span`; a broken `use` line
    asks the distance between two offsets, never their line numbers.

    **Cases**: `check/fixedbugs-146-a-file-of-foreign-heads-is-placed-once`
    (every shape's diagnostics, unchanged), and the compiler's own tests: the
    outline agrees with both walks at every line, is read once and only for a
    word that opens its line, and a module's head past the root's open
    bracket is read as its own lexer swapped it. **Time**, `check --brief`,
    user time beside real, measured by the lane at load about 6, so a busy
    machine's: 1,500 heads 1.25 to 0.07 s, 3,000 4.71 to 0.13 s, 6,000 16.13 to
    0.24 s, lane 136's 6,000-head file 35.45 to 0.28 s; clean files unchanged.
    The recovery instrument read the same on all 13,594 singles and 15,800
    pairs before and after this commit. **Left open, measured by the lane and
    filed with its next items**: the module loader is quadratic in `use`
    lines, and about 82 in-place pushes across the compiler cannot grow in
    place, a grep's count and not a measurement.

    **The gates.** Each repair by its cases and the compiler's own tests, the
    recovery instrument against the lane's base plan after each. **The batch
    gate**, lane recovery-b5's closing commit `a8b04ea8` (with C1, C2, C3 and
    C5 of defects 130 and 131, which stay open, and `538d70ac`, C3 redone as
    the later repair where it met lane 135c's `line_end_readings` at the
    merge), the trunk merged at `5416f0a0`, `1cdb7046` and `26765218`: the
    seed regenerated once, the fixpoint by `cmp`; the compiler's own tests
    993 and the net's own 184, all passed; the full net, 25 suites four at a
    time and `cache` alone, 4,240 passed and 0 failed, none red in the
    parallel pass; the census over 1,530 files, both arms, 21 moved, every
    one a case of the batch or panel 183's known-cost case; the recovery
    instrument, EXTRA 850 to 532, ONE 12,427 to 12,745, HIDDEN pairs 16 to 12,
    no class risen for any operator. The trunk fast-forwarded to `a8b04ea8`
    at 00:09 on 2026-10-02. **Linux x86-64** on `a8b04ea8`, its container
    read from `docker logs` after the host side lost it: the compiler's own
    tests 993, all passed, and the 19 suites 0 failed, `probe` 24 among them.
    **Speed**, on a still machine (load 1.5 to 1.7), the trunk's compiler
    before (`15d66dc4`) against after, interleaved, `check
    selfhost/main.hero`: user 4.49 and 4.50 s against 4.47 and 4.48 s (a first
    pair, real 5.15 and 4.96 against user 4.48 and 4.50, waited and is
    discarded). **Owed before the push**: Linux arm64 and the Windows box on
    the trunk.
