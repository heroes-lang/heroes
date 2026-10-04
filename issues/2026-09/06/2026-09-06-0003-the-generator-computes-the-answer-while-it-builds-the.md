- [ ] **M-generated-programs** | the generator computes the answer while it builds the program | `tests/harness/suite_corpus.hero` § configurations · `examples/montecarlo/main.hero`

    **Origin:** scheduled with the chain row, 2026-09-06, the author taking all
    five oracles over the two narrower options offered. The net's three
    configurations are the differential arm.

    **The two moves, so the milestone does not re-derive them.** Generation goes
    from the TYPE and never from the text — start at *an expression of type
    `i64` is needed* and descend among the forms that type admits — so the
    program type-checks by construction and a refusal from `heroes check` is
    itself a defect. And every node carries its value as it is built, so the
    generator writes the program and its `main.expected` together and needs no
    second compiler as judge; that is Csmith's checksum trick, and it is what
    makes the silent class visible at all.

    **Heroes has no undefined behaviour, which changes the job**: overflow,
    division by zero and an index out of range all abort by design, so there is
    nothing to steer around the way YARPGen must for C — there is a clean arm
    whose values are safe by construction, and a smaller declared arm whose
    expected result IS the abort and its message.

    **What already exists and must be reused rather than rebuilt**: the three
    configurations are `tests/harness/suite_corpus.hero::configurations()`,
    measured 2026-09-06 as `["-O0", "-O2", "--sanitize"]`, and their
    disagreement is the only differential oracle there is until M-qbe-backend
    gives the tree a second backend; a deterministic pseudo-random stream driven
    by an integer seed is in `examples/montecarlo/main.hero`, with a test saying
    why a seed must reproduce.

    **Where to look also:** `docs/ROADMAP.md` § M-generated-programs.
    **Why it matters:** exit 0 with a wrong number is the class nobody can write
    a golden case for in advance, and an oracle the generator carries is the
    only kind that scales with the programs.

    **Re-verified 2026-09-10: STILL OPEN, and both premises hold exactly.**
    `tests/harness/suite_corpus.hero:304-305` is still
    `function configurations() -> [str]` returning `["-O0", "-O2", "--sanitize"]`, and
    `examples/montecarlo/main.hero:41` still carries the seeded stream with its two
    tests at `:85-89`. No generator exists: the net registers 20 suites and none is
    generative.
