# Panel 190 ratified: one exit where a function has two or more ways out, its return slot borrowing

2026-10-04, on the author's answer to the coordinator's recommendation of that
afternoon (*1a*, ratify R1 to R12, with lane b10-ir's exemption for a program
with holes named), in their words *"1a 2a 3a 4a 5a"*. **Recorded as a
reading**, CLAUDE.md § 4's default; not `by delegation`.

- [x] **panel 190** | ratify, amend or overturn R1 to R12 (route A-star: one exit only where a function has two or more ways out, made by a pass after lowering; the return slot borrowing, a slot kind and a verifier check at landing; the merge total or loud; the return-type check restored over the exit; coalescing slots and a liveness-pruned sweep refused; design.md Part 5's sentence; defect 231 made `blocking` on the Windows box's measurement) | `docs/panel/190-a-function-with-two-or-more-ways-out-leaves-by-one-exit-that-sweeps-once-its-return-slot-borrowing.md` § The resolution

    **Origin:** panel 190's synthesis, 2026-10-04 from 13:57, on the seats'
    archive of `703af779`. Until it is answered defect 231 stays open, and,
    `blocking` by R9, it lands in the next batch on the provisional
    resolution (CLAUDE.md § 4, *the panel never blocks*).

    **Recommendation: ratify R1 to R12**, on what was built and run:
    - A, G and A-star built, each fixpoint by `cmp`; A-star's own tests 1,158
      with one shape assertion owed;
    - every route's boundary columns the trunk's on this Mac and on Linux arm64
      under two clangs, Valgrind's count included;
    - A-star's IR and the compiler's own memory on the 800 shape linear (16 MB
      against 450 MB), the seed 14.5% smaller and 21.8% cheaper for clang at
      `-O2`;
    - on the Windows box, the trunk's 800 shape does not build and every exit
      route's does.

    **The price A-star pays**: 3.5 to 4.1% of the interpreter's `-O2` depth,
    and a fifth to a third more clang memory at `-O2 -g` on the extreme shape
    on Apple clang.

    **The conservative alternative, yours to choose instead**: route G. It
    costs no depth, but leaves the IR and the compiler's own memory quadratic,
    and needs `.claude/rules/generated-c.md`'s one-label rule and panel 021's
    R1 and R8 amended.

    **Ratified 2026-10-04**, on the author's answer between 16:31 and 16:33 by
    the clock read before and after it, in their words *"1a 2a 3a 4a 5a"*.
    **What the yes settles:**
    - R1 to R12 as the synthesis states them, route A-star over the
      conservative G;
    - **R3 as lane b10-ir landed it** at `6e616898`: the merge is total or
      loud, save one shape the verifier exempts by name. That shape is a
      program holding a hole `???`, whose function with a result keeps one
      valueless returning block, since the checker reports no missing return
      in a module with holes. It is pinned by
      `tests/golden/ir/fixedbugs-231-a-hole-leaves-its-function-unmerged`.
      Without the exemption the holes report became an internal error at exit
      2, measured by the lane, and no binary is ever made of such a program.

    **Recorded as a reading**, CLAUDE.md § 4's default; not `by delegation`.
    The landing is batch 10's lane b10-ir, `075b425d` to `052f639a`, gated
    with its round.
