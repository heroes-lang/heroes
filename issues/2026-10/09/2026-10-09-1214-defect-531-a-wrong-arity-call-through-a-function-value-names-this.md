---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 73b1e9d3cddcddf926e3b8e2de7062fbb3fca597
github: none
---

- [ ] **531 — a wrong-arity call through a function value names `this function` as if it were a name** | the message reads *`this function` takes 1 argument(s), found 2*: a placeholder printed in backticks where a name stands (lane b16-compiler) | the arity diagnostic of a call through a function value, `selfhost/check/` · defect 510 · **class: adjacent**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `73b1e9d3`, 2026-10-09 (lane b16-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The placeholder had two more messages beside the card's, the same cause in `check/walk.hero`'s `indirect_call`: a label owed (*two of `this function`'s parameters*) and a label naming nothing (*`this function` has nothing called `to`*). All three now name the callee as the program wrote it (`check/callee_text.hero`), a parameter, a local, a field, a constant, a qualified one, an element or a call's result, and one written over several lines on one, its comment left out, as `members_below.on_one_line` reads a head. One new `check` case, its ten messages red on the base, and six whose `this function` became `move`, `g` or `h`; check 636 and 0, the compiler's own tests 1,519 passed, and `check` of 500 to 8,000 calls through a value unchanged in instructions.
