---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **396 — a single value lent through `@` to a pointer C writes an array through** | `SHA256_Final(@md: u8, ...)` writes the digest's 32 bytes through a one-byte cell: `check` 0, `run` 0, and a record field after it, `after: 7`, prints 2531777658719584577, bytes 8 to 15 of SHA-256("abc") (the coordinator's re-run on the frozen tree, `docs/panel/194-evidence/new-defects/single-cell/md_field.hero`); `--sanitize` is silent, the store being inside libcrypto (the ffi-pragmatist's, on three legs) | the lend of one cell through `@` to a C pointer parameter · `.claude/rules/c-boundary.md` · `docs/panel/194-evidence/new-defects/single-cell/` · **class: systemic**

    **Origin:** panel 194's ffi-pragmatist, 2026-10-06 (*found alongside* 1); reproduced by the critic's second pass and the coordinator.

    **Class: systemic**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a memory fault at exit 0, and none of 092's routes reaches it, since no count is passed; what a lend of one cell promises when the header's pointer is written as an array is a ruling no rule reaches today, a sitting's.
