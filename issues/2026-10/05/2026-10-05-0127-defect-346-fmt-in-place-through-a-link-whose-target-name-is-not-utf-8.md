---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: 805bae193f20f37e40b5f6f27ef80a3f0187d76d
github: none
---

- [ ] **346 — `fmt --in-place` through a link whose target name is not UTF-8 says *the operating system's own reason is 84*, a number the runtime set** | on Linux, `heroes fmt` over a link whose target's name holds a byte that is not UTF-8: the message gives *the operating system's own reason is 84*, and 84, `EILSEQ`, is set by the runtime itself (`runtime/parts/replace.c:94`), not by the system (lane b11-windows, Linux arm64, 2026-10-05, the lane's report) | `runtime/parts/replace.c:94` (the `errno` it sets) · the compiler's wording of a refusal's reason · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 238's link check (its final report, *Found beside*), read `adjacent` by the lane.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a false message, the reason attributed to the operating system being the runtime's own, which `blocking`'s list names; the lane read it `adjacent`.

    **2026-10-05:** Repaired at `805bae19`, gated by its case red first (1,218 tests, 1 failed) and the compiler's own tests on this Mac and on Linux arm64 (1,218, all passed each), its reproducer through `fmt --in-place` on Linux now telling the runtime's reason; the net is owed at the batch's close, and the Windows box its case.
