---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 73b1e9d3cddcddf926e3b8e2de7062fbb3fca597
github: none
---

- [x] **531 — a wrong-arity call through a function value names `this function` as if it were a name** | the message reads *`this function` takes 1 argument(s), found 2*: a placeholder printed in backticks where a name stands (lane b16-compiler) | the arity diagnostic of a call through a function value, `selfhost/check/` · defect 510 · **class: adjacent**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `73b1e9d3`, 2026-10-09 (lane b16-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The placeholder had two more messages beside the card's, the same cause in `check/walk.hero`'s `indirect_call`: a label owed (*two of `this function`'s parameters*) and a label naming nothing (*`this function` has nothing called `to`*). All three now name the callee as the program wrote it (`check/callee_text.hero`), a parameter, a local, a field, a constant, a qualified one, an element or a call's result, and one written over several lines on one, its comment left out, as `members_below.on_one_line` reads a head. One new `check` case, its ten messages red on the base, and six whose `this function` became `move`, `g` or `h`; check 636 and 0, the compiler's own tests 1,519 passed, and `check` of 500 to 8,000 calls through a value unchanged in instructions.

## The repair

Repaired at `73b1e9d3`, 2026-10-09 (lane b16-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The placeholder had two more messages beside the card's, the same cause in `check/walk.hero`'s `indirect_call`: a label owed (*two of `this function`'s parameters*) and a label naming nothing (*`this function` has nothing called `to`*). All three now name the callee as the program wrote it (`check/callee_text.hero`), a parameter, a local, a field, a constant, a qualified one, an element or a call's result, and one written over several lines on one, its comment left out, as `members_below.on_one_line` reads a head. One new `check` case, its ten messages red on the base, and six whose `this function` became `move`, `g` or `h`; check 636 and 0, the compiler's own tests 1,519 passed, and `check` of 500 to 8,000 calls through a value unchanged in instructions.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
