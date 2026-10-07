---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 40c055a33b268d375d33c382a403467fde54cab4
github: none
---

- [ ] **396 — a single value lent through `@` to a pointer C writes an array through** | `SHA256_Final(@md: u8, ...)` writes the digest's 32 bytes through a one-byte cell: `check` 0, `run` 0, and a record field after it, `after: 7`, prints 2531777658719584577, bytes 8 to 15 of SHA-256("abc") (the coordinator's re-run on the frozen tree, `docs/panel/194-evidence/new-defects/single-cell/md_field.hero`); `--sanitize` is silent, the store being inside libcrypto (the ffi-pragmatist's, on three legs) | the lend of one cell through `@` to a C pointer parameter · `.claude/rules/c-boundary.md` · `docs/panel/194-evidence/new-defects/single-cell/` · **class: systemic**

    **Origin:** panel 194's ffi-pragmatist, 2026-10-06 (*found alongside* 1); reproduced by the critic's second pass and the coordinator.

    **Class: systemic**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a memory fault at exit 0, and none of 092's routes reaches it, since no count is passed; what a lend of one cell promises when the header's pointer is written as an array is a ruling no rule reaches today, a sitting's.

    Repaired at `40c055a3`, 2026-10-07 (lane b13-land-buf), the last of panel 196's ratified R1 to R7 to land: R2 at `f21abe8d` (an unmarked `@x: T` is ONE element, in § 13), R5 at `1c583a26` (`counted_by` a sibling, a group's constant or a number), R3 at `5ddd6896` (`lent` read on an `@` parameter, a temporary only for `lent`), R4 at `bcdbf694` (the buffer C fills, `@md: [T] counted_by E lent`, in a per-thread guarded region off the stack, the runtime's ABI 28 to 29), R7 at `40c055a3` (a local lent through `@` to C lives in place in that region, a wrapper's `@` parameter followed to its caller's local), and `de1e0624` beside them (a count read from an element keeps the element's type); R1 is defect 413's repair. The reproducer `md_scalar.hero` aborts by name on this Mac, Linux arm64 and the Windows box; `md_field.hero`, a field lent as one cell, is the binding's word under R2 and pinned as such. Cases `run/fixedbugs-396-*` (15), gated by the lane on three platforms; a C-boundary defect, so it closes after the push's CI Windows leg (the author's decision of 2026-10-07 at 10:19); the card filled by the coordinator.
