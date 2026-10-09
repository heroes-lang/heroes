---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 31cbf86a10f37053ad0391c2b98f719ffbf10827
github: none
---

- [x] **516 — `diag.hero`'s note on `machine_locked_path` says what defect 446 measured false** | it calls the name a thesis rule *because C would take the program on exactly one machine*; defect 446 measured ld64, GNU ld and lld-link reading a rooted `-l` as another path, never the one written (lane b15-box) | `selfhost/diag.hero` · defect 446 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-box's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a comment that states a premise measured false.

    Repaired at `31cbf86a`, 2026-10-09 (lane b16-compiler), gated by the `annotations` suite and the compiler's own tests; the net is owed at the batch's close. The note now says what holds for the names the code still refuses, that without the rule the tools read the name where one machine's files decide what it names, run that morning for two of them (clang opens a rooted header as written; `pkg-config` reads a `.pc` from the directory it runs in, and from its parent finds none), and that a rooted `link` is `unwritable_name` because no linker reads it as written (defect 446's measurement). A comment only.

## The repair

Repaired at `31cbf86a`, 2026-10-09 (lane b16-compiler), gated by the `annotations` suite and the compiler's own tests; the net is owed at the batch's close. The note now says what holds for the names the code still refuses, that without the rule the tools read the name where one machine's files decide what it names, run that morning for two of them (clang opens a rooted header as written; `pkg-config` reads a `.pc` from the directory it runs in, and from its parent finds none), and that a rooted `link` is `unwritable_name` because no linker reads it as written (defect 446's measurement). A comment only.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
