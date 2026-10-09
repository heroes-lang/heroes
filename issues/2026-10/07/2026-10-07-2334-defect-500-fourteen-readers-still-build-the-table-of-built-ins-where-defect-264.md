---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: c522515d58c2356c6c4e3011843b0d89e91ddf27
github: none
---

- [x] **500 — fourteen readers still build the table of built-ins where defect 264 left a constant** | `inventory.table()` is still read in `resolve/` (offered, built_marks), `ir/` (flatten, place_store, print) and `emit/` (ops, gate, unread): the last 18,030 table builds on the emission of a 2,000-long built-in chain; each a one-line switch to `inventory.name_of` or `NAMES`, after which `table()` may go (lane b14-check) | `selfhost/inventory.hero` and its fourteen readers · defect 264 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-check's final reports; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup.

    Repaired at `c522515d`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The fifteen readers found, eleven wanting one name, two the list of names, two counts in tests and one test helper an index, read `inventory.name_of`, `NAMES` and `index_of`: over a main of 4,000 lines of built-in calls the emission retires 18.963 billion instructions against 19.909, 4.8% fewer, the C byte-identical, and the compiler checking itself is unchanged, 71.067 billion before and after. `table()` stays, its doc saying why: the test holding `NAMES` to it, and `tests/harness/suite_spec.hero`, which reads its `Builtin(name: ` lines as text.

## The repair

Repaired at `c522515d`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The fifteen readers found, eleven wanting one name, two the list of names, two counts in tests and one test helper an index, read `inventory.name_of`, `NAMES` and `index_of`: over a main of 4,000 lines of built-in calls the emission retires 18.963 billion instructions against 19.909, 4.8% fewer, the C byte-identical, and the compiler checking itself is unchanged, 71.067 billion before and after. `table()` stays, its doc saying why: the test holding `NAMES` to it, and `tests/harness/suite_spec.hero`, which reads its `Builtin(name: ` lines as text.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
