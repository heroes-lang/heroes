---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: 608715260c645afd8dacc15479247bb3396170f1
github: none
---

- [ ] **524 — a function head that fails over several lines holds back its missing-body message** | a failing head on one line is told 2 messages, the same head broken over several lines 1 (lane b15-parse) | `selfhost/parse/`, the head's recovery · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after the first is fixed.

    Repaired at `60871526`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The report that a failed head's body is missing is held back only where the head went on, inside its brackets, to a line at its body's margin, one level below its own, where its recovery may have read the body (`body_margin.crossed`); a head whose lines past its first all stand at another margin is told its missing body as on one line. A head broken at the body's own margin stays held back, reading as a swallowed body does. One new `check` case, its two heads red on the base; `fixedbugs-456-a-broken-type-in-a-head-last-in-its-file` moved as its own note foretold.
