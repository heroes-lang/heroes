- [ ] **209 — a `layout` run filtered to a name that matches no module reads 1 passed** | `./heroes run tests/harness/main.hero -- ./heroes layout <a name no module has>`: `1 passed, 0 failed`, because the harness's guard against an empty selection counts cases and the suite's file-wide checks are always one case | `tests/harness/suite_layout.hero` · the harness's selection guard · **class: improvement**

    **Origin:** the coordinator's agent finishing defect 167 in lane cb4, 2026-10-03, which then checked by hand that each of its six filters matched one module (`scratchpad/lane-cb4/progress.md`, 2026-10-03).

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument that can read green over nothing; no program moves.

    Repaired at `80cb060c`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.
