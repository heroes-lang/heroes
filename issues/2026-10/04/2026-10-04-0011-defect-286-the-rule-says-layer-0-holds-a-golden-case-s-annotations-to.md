---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: 8d7b11dd25a3b73a7fe027503233fe4222c518b2
github: none
---

- [ ] **286 — the rule says layer 0 holds a golden case's annotations to its `.expected`, and no hook holds them** | `.claude/rules/verification.md:544` to `:545` lists among layer 0's checks *a `tests/golden/` case's `#~` annotations are held to its `.expected`*; `grep -l -E '#~|annotation|\.expected' .claude/hooks/*` finds no hook that does (read by the coordinator at `703af779`, 2026-10-04), so a case whose marks and expectation disagree waits for the `annotations` suite | `.claude/hooks/fmt_check.py` · `.claude/rules/verification.md` § A suite is the last judge · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, a reading of `fmt_check.py` whole); the coordinator's grep over every hook.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a rule that names a check nothing performs; the suite still judges, later than the rule says.

    Repaired at `8d7b11dd`, 2026-10-05 (the coordinator's lane b12-hook), the hook asks the `annotations` suite, narrowed to a golden case, on a write of its `.hero` or `.expected`; gated by the hook's own payloads run by hand, no instrument testing the hooks; the net is owed at the batch's close.
