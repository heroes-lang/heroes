---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **606 — a gallery example its own compiler refuses passes the gate: ten of the gallery's fourteen programs are built, checked or run by no suite** | `examples/gallery/12-interpolation.hero` at `74722f82` (M-inferred-cell step 1 landed, its gate green) is refused by the compiler built from that same commit: `error[never_rebound]` at 31:5, `seen: {str: i64} @ {"Sirius": 2}` a cell nothing re-binds, `check` exit 1. The step's rule was right and the example wrong, and no suite said so: `corpus` and `emission` walk an example's entry point, `main.hero`, which the gallery has none of (`tests/harness/suite_corpus.hero`, `suite_emission.hero:105`); the harness names four gallery files by hand, `00-first.hero` (38 rows), `09-holes.hero` (5), `13-lease.hero` (2) and `01-points.hero` (1), measured by `grep -rhoE 'examples/gallery/[0-9]+-[a-z-]+\.hero' tests/harness/*.hero`; the other ten are read by `canonical` (`fmt` alone) and by `mutate`, neither of which asks whether the program compiles. The gallery is the site's teaching corpus, so a page can show a program the compiler refuses | `tests/harness/suite_corpus.hero` (its walk of `examples/*/main.hero`, where a walk of `examples/gallery/*.hero` through `check` belongs, each file its own program), `tests/harness/suite_emission.hero` (the same entry-point premise) · **class: adjacent**

    **Origin:** found by the landing lane of panel 209 on 2026-10-10 at 21:07, when M-inferred-cell step 2's census (`heroes check` over every tracked example and golden, one process per file) listed the one real refusal in the tree; HEAD's own compiler, built in a detached worktree at `74722f82` from the seed and `selfhost/`, gave the same message on HEAD's file.

    **Class: adjacent**, 2026-10-10: real, found beside the work, none of `blocking`'s shapes; the example is repaired in step 2's commit by the rule's own certain fix (`seen` bound with `=`), and the suite that would have seen it is the item here.
