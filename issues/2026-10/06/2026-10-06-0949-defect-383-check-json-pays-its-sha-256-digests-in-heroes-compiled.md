---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: c41525aec367702a815ee4b69cc9ce7e97db2d26
github: none
---

- [ ] **383 — `check --json` pays its SHA-256 digests in Heroes compiled without optimisation** | `check --json` on the compiler's own source retires 101.6 billion instructions against 86.4 billion before panel 193's R5 put a digest of every file read into the document, +17.5%, all of it the SHA-256 written in Heroes (lane cli12's measurement, its final report; not re-run by the coordinator) | `selfhost/cli/sha256.hero` · panel 193's R5 · **class: improvement**

    **Origin:** lane b12-cli12, 2026-10-06 (its final report); filed by the coordinator at 09:49.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost no rule promises against, the digest right; outside the batch under the author's instruction of 2026-10-05, the improvements stay out.

    Repaired at `c41525ae`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Whole blocks are read where they lie, one schedule array is reused and rotations are written out: 2,729 to 2,737 instructions a byte before, 1,686 to 1,695 after, at 64 KiB, 512 KiB and 4 MiB, and `check --json` over `check` on the compiler's own source +24.2% before, +15.4% after, the documents byte-identical and every digest `shasum -a 256`'s on 1,226 files.
