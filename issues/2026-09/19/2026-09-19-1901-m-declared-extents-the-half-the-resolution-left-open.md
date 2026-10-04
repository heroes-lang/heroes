- [ ] **M-declared-extents mutation drill** | The ratified resolution checked the extent *"only where it is a compile-time constant"*, and the step that landed it added a run-time compare the resolution does not mention. **Before reading: with the mark in place and no compare, `sum_n(p: s.name.ptr(), n: k)` — `k` a parameter — read 4096 bytes of the stack at exit 0. What did the sitting's own compiler-engineer make its approval conditional on, and what does that make the compare?** | `selfhost/emit/lend_extent.hero` § THE HALF NO ASSERTION CAN REACH

    **Where to look after answering:** *"a variable extent is unknown at check
    time and must be refused, not admitted. Admitting it silently is the defect
    wearing a mark."* The compare is that condition discharged — and it is
    discharged the ROBUST way rather than the literal one: refusing every
    non-constant extent at `check` would also refuse a count the program
    computes and gets right.

    **The cost, measured here rather than recalled from the sitting.** Two
    binaries from one emitted C, `-O2`, `noinline` callee, 200 million lends,
    the machine still: `user 0.81` both, `real 0.82` against `0.81`. The seat
    that proposed it had priced 1%; this run cannot see 1%.

    **The question to carry away.** A ratified resolution said *only where the
    extent is a constant*, and the implementation went further. Ask what makes
    that obedience rather than drift — and where the answer is written down.
