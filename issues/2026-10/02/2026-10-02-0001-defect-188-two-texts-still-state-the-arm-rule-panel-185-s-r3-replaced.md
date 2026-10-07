---
kind: defect
area: check
milestone: none
filed: 2026-10-02
commit: e45fc34bdee7a3ad5b1913430cd4d3ea81bd50e5
github: none
---

- [ ] **188 — two texts still state the arm rule panel 185's R3 replaced** | `selfhost/check/lending.hero:108-109` quotes the spec as *An arm that does nothing is a block holding `_ = 0`*, where the spec now reads *An arm that does nothing holds `_ = 0`*; `tests/golden/check/fixedbugs-135-a-discard-that-is-the-line-s-one-reading.hero:8` reasons *`_ = ` on the arm's own line is `declaration_in_arm`*, false since R3 | the two lines · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/stale-arm-texts/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). design.md §4.7 owes nothing: R3 brought the spec to it.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): no program moves.

    Repaired at `e45fc34b`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `lending.hero` quotes spec § 8 as it stands, *An arm that does nothing holds `_ = 0` — `continue` is not one*, and the golden carries a dated correction below its program, in its `.hero` and `.fixed`, so no annotated line moved; the same premise, `_ = ` refused on an arm's line, was corrected beside it in `discard_errors.hero` and `statement_front.hero`, comments only, after the base compiler checked `0 => _ = compute(0)` and `1 => n @ compute(1)` at exit 0.
