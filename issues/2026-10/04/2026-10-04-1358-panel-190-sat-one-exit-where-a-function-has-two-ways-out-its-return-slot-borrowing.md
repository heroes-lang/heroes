---
kind: decision
area: none
milestone: none
filed: 2026-10-04
commit: 7f4c0cc531e546f6880bc95cc4f569146d437afd
github: none
---

# Panel 190 sat: one exit where a function has two or more ways out, its return slot borrowing

2026-10-04, written at 13:58 by the clock (`date`). Panel 190 sat in the
soundness lane on defect 231: every block that returns sweeps every owned slot.
It was convened by the author's *4a* of the same morning
(`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`).

**The seats**: the compiler-engineer and the ffi-pragmatist, with the
completeness critic over the briefs and over the reports, and no paid run.

**The synthesis**:
`docs/panel/190-a-function-with-two-or-more-ways-out-leaves-by-one-exit-that-sweeps-once-its-return-slot-borrowing.md`.

**Its resolution, `provisional — author ratification pending`, R1 to R12:**
- **The route**: A-star, the compiler-engineer's. An exit block only where a
  function has two or more ways out, made by a pass after lowering, the
  return slot borrowing.
- **What the landing adds to the prototype**: the merge total or loud; a
  slot kind and a verifier check; the return-type check restored over the
  exit.
- **Refused**: coalescing slots and a sweep pruned by liveness, on the
  ffi-pragmatist's measured fault: a C library reading a string freed early,
  ASan silent on three platforms.
- **design.md Part 5's sentence** rewritten.
- **The prices named**: 3.5 to 4.1% of `-O2` depth, and more clang memory on
  the extreme shape on Apple clang.
- **The conservative alternative**: route G, the C-only label.
- **Defect 231 becomes `blocking`**: on the Windows box the trunk's
  800-return shape does not build (`clang died`, exit 2), and every exit
  route's does. It is therefore not deferred, and lands in the next batch on
  the provisional resolution.

Queued as the DECIDE item `panel 190`.

**Filed from beside the sitting**:
- **321**: no instrument in the net sees a C library read freed memory;
- **322**: `-gline-tables-only` for `-g`, the route the critic found nobody
  listed, a question about the flag list for a sitting of its own;
- **314** (filed earlier in batch 9's round tree): a doubled release on the
  Windows box, silent without ASan.

The checker's acceptance of a `str` extern parameter is recorded and not
filed. design.md `:2626` to `:2637` (panel 021) makes the by-value `str` the
FFI's guard, and a real header refuses it at build with
`ffi_parameter_type`.
