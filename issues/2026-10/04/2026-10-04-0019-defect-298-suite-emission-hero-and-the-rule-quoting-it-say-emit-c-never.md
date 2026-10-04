---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: 01e7934faaf10366e822de42301860bb734aade8
github: none
---

- [ ] **298 — `suite_emission.hero` and the rule quoting it say `--emit-c` never calls clang, and it does** | `tests/harness/suite_emission.hero:71` and `:244` and `.claude/rules/verification.md:166` say *`--emit-c` never calls clang*; `heroes build --emit-c` compiles every unit before it writes the C (`selfhost/cli/produce.hero:243` to `:272`, panel 190's critic, 2026-10-04), and lane b9-harness measured `build --emit-c` telling `ffi_missing_header` and `ffi_package` at exit 1 (2026-10-04) | the two comments and the rule's sentence · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its final reply's *found beside*), measured; panel 190's critic read the same in `produce.hero` the same morning.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a premise written in an instrument and a rule, false; the suite's verdicts still hold (a refused program with nothing blessed passes by defect 048's rule), the reason given for them does not.

    Repaired at `01e7934f`, 2026-10-05 (lane b11-misc), gated by `emission` whole and the net's own tests; the net is owed at the batch's close. The two comments are corrected and the paragraph between them, which said the round's tag half discards clang's verdict, with them; the rule's sentence is the coordinator's, its replacement line in the lane's report.
