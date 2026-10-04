---
kind: task
area: harness
milestone: M-package-manager
filed: 2026-09-16
commit: none
github: none
---

- [ ] **M-package-manager** | nothing in the tree compiles or links what `--emit-c` wrote, so a defect in the artifact is invisible to every suite | `tests/harness/suite_emission.hero` · `docs/panel/157-the-artifact-of-a-build-that-never-happened.md` R4

    **Origin:** panel 157 R4, adopted 2026-09-16 and deliberately not built —
    defect 048's repair closed the two holes that sitting measured and left the
    instrument that would have FOUND them.

    **What is missing, stated as the assertion rather than as a gap**: the same
    program built both ways must give the same exit code and the same stdout,
    and it must **LINK** and not merely compile. Each half of that sentence was
    paid for. Compiling the artifact catches 20 of the 25 `fixedbugs/` cases the
    build refuses; it catches **none** of the five that were silent, because
    clang accepted their C — that is what the exit-code comparison is for. And
    linking is what `ffi-missing-link` needs: a library nobody named is a linker
    failure and no check that stops earlier can see it, which is why that one
    case still emits at 0 after defect 048's repair.

    **Why this milestone.** `emission` compares BYTES, and a byte comparison
    cannot see that the bytes do not compile: of 240 blessed emissions measured
    2026-09-16, **28** failed `-fsyntax-only` and **3** failed with *must use
    'struct' tag* while green in the suite. Defect 048 was already committed and
    blessed. The cost of the missing instrument is a whole class of defect that
    ships, and it becomes load-bearing exactly here, where a binding is
    something you **distribute** rather than something you build once.

    **The cost, priced at the sitting**: +20 to +30 lines in
    `tests/harness/suite_emission.hero`. What was NOT priced is the wall clock —
    building 240 programs twice, with a link each, is a different order from
    reading a blessed file, and whoever builds it should measure that first and
    say whether the leg belongs in `emission` or behind the push.

    **Why it matters:** the suite that exists to compare emissions cannot tell
    a good emission from one that will not build, and it said so for a year in
    its own comment without anybody hearing it.
