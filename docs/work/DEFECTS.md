# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **057 — `warnings` blamed one program for another's clang warnings, once, and the reproducer has not been found** | `warnings/curl under build` reported three warnings naming `tests/golden/ir/regression-extern-parameter-types`, a program curl does not touch | `tests/harness/suite_warnings.hero:78-90` · `selfhost/cli/units.hero:111,199` · `selfhost/cli/toolchain.hero:210,259`

    **Origin:** 2026-09-17, the full net run before the CL-078 commit, after a
    session that had run many builds and three parallel worktrees. **It is filed
    without a reproducer on purpose**, which `CLAUDE.md` § RUN IT sanctions in
    those words: the evidence is measured and losing it would mean rediscovering
    it, and a later session meeting the same row should find this rather than
    start from nothing.

    **What was measured.** One net run, `warnings` 183 passed and 1 failed:

        FAIL warnings/curl under `build`
          examples/curl/main.hero produced clang warnings:
            build/tu-08401abe1c35d47d/regressionexternparametertypes.c:28:29: warning: absolute value function 'abs' …
            tests/golden/ir/regression-extern-parameter-types.hero:14:109: warning: …
            tests/golden/ir/regression-extern-parameter-types.hero:20:10: warning: …

    A clean re-run after `rm -rf build` reads **184 passed, 0 failed**, and the
    whole net reads 1864 passed, 0 failed.

    **Three explanations ruled out, each by a command:**

    - **The shared `clang-stderr.txt`.** Four sites write `build/clang-stderr.txt`
      and it is one file per build directory, so a stale read was the first
      guess. But `suite_warnings.hero:84` reads `said.must().err` — the
      invocation's OWN stderr — not that file. Ruled out by reading the suite.
    - **The warning came from building that fixture.** It cannot:
      `heroes build tests/golden/ir/regression-extern-parameter-types.hero` is
      exit **1**, `error[ffi_parameter_type]`, refused before clang sees it.
      Measured.
    - **Curl's build prints it today.** It does not: rebuilt with a warm cache,
      exit 0, 121 bytes of stderr, zero mentions of the other program. Measured.

    **What is still unexplained**, and it is the whole of the defect: a TU
    directory `build/tu-08401abe1c35d47d/` existed for a program the build
    refuses, and its warnings reached the stderr of a different program's
    invocation. Something compiled that TU, and something carried its output
    across.

    **What is owed.** A reproducer first, and the shape to try is the one this
    session had and a clean run does not: a `build/` cache carrying TUs from
    programs built by other worktrees or by a refused verb, then `warnings`
    over `examples/`. If it cannot be reproduced, the honest repair is to make
    it impossible instead — the suite could assert that every warning it reports
    names the program it is judging, which is a property of the text it already
    holds and would turn a wrong answer into a loud one.

*******************************************************************************
