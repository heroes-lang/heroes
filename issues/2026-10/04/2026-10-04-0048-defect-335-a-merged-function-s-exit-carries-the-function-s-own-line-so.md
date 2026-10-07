---
kind: defect
area: ir
milestone: none
filed: 2026-10-04
commit: 9d8b07aed056cfebb6a7281f6998544f693c6e7d
github: none
---

- [ ] **335 — a merged function's exit carries the function's own line, so a debugger stepping out of a `return` lands on the function's head** | since defect 231's repair, the copy-outs, the retain and the sweep in a merged function's exit block carry the function's own source line in their `#line`, so lldb stepping from a `return` shows the `function` line instead of the `return`; the `lines` suite reads green (lane b10-ir, 2026-10-04, its reply's *Found beside*) | `selfhost/ir/exits.hero` (the exit block's line) · design.md §2's promise that lldb steps `.hero` lines · defect 231 · **class: improvement**

    **Origin:** lane b10-ir, 2026-10-04, beside 231; filed by the coordinator.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a debugging step less exact than it could be; no value moves.

    Repaired at `9d8b07ae`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every copy-out carries the span of what leaves, the `return`, the `?` or the end of the body, so the retain and the sweep after it do too, and a merged exit takes the copy-outs and the line of the way out written first in the source: lldb's `next` from a three-way function's first `return` now goes on to the releases where it stopped on the `function` line, and from its other two stops on the first `return`'s line, one exit serving every way out; 116 emissions moved, every changed line a `#line`.
