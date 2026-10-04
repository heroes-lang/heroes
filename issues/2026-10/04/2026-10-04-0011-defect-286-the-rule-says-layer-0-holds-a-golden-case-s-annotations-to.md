---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **286 — the rule says layer 0 holds a golden case's annotations to its `.expected`, and no hook holds them** | `.claude/rules/verification.md:544` to `:545` lists among layer 0's checks *a `tests/golden/` case's `#~` annotations are held to its `.expected`*; `grep -l -E '#~|annotation|\.expected' .claude/hooks/*` finds no hook that does (read by the coordinator at `703af779`, 2026-10-04), so a case whose marks and expectation disagree waits for the `annotations` suite | `.claude/hooks/fmt_check.py` · `.claude/rules/verification.md` § A suite is the last judge · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, a reading of `fmt_check.py` whole); the coordinator's grep over every hook.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a rule that names a check nothing performs; the suite still judges, later than the rule says.
