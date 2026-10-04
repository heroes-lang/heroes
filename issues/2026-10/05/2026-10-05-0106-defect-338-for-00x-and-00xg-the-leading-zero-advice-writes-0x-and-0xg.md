---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: e703a84308526fadf15b7ef92b1a5c866705a549
github: none
---

- [ ] **338 — for `00x` and `00xg` the `leading_zero` advice writes `0x` and `0xg`, two literals the compiler refuses** | `x = 00x` and `x = 00xg`: `leading_zero`'s guess fix `0` applied writes `0x` and `0xg`, each refused at `check` (lane b11-misc's compiler, after `3f032bc7`, 2026-10-05, the lane's report) | `selfhost/leading_zeros.hero` (the advice) · defect 324, whose repair made every piece of the advice a literal the compiler accepts · defect 329 · **class: blocking**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 329's repair (its final report, *Found beside*); 324's cause, the advice writing a program the compiler refuses, at a shape 324's cases do not hold.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a `guess` fix that writes a program the compiler refuses, defect 324's own class reading (`.claude/rules/verification.md` § Bounded discovery), so never deferred: repaired in this batch.

    Repaired at `e703a843`, 2026-10-05 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside with this cause were repaired with it: a digit no number of the marker's base has (`00b2`, `00o9`), a fraction, and a separator that leads or trails after the marker.
