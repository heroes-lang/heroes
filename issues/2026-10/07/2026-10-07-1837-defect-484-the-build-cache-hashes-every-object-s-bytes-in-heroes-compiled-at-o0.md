---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **484 — the build cache hashes every object's bytes in Heroes compiled at `-O0`** | defect 443's seal check costs about 1,158 instructions per byte of object, +13.4% on a warm `print(1)`, +14.6% on `examples/interpreter`, +4.0% on the compiler's own build; a runtime C function keying a file's bytes would cut it several times (lane b14-cli's inference, unmeasured) | `selfhost/cli/served.hero` (batch 14's round, unmerged on 2026-10-07) · defect 443 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-cli's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost the robust check pays; robustness kept, the price measurable.
