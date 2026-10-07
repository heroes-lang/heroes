---
kind: defect
area: ir
milestone: none
filed: 2026-10-04
commit: d15ab3fb5ab5546c95773436d52d448411ada1b7
github: none
---

- [ ] **335 — a merged function's exit carries the function's own line, so a debugger stepping out of a `return` lands on the function's head** | since defect 231's repair, the copy-outs, the retain and the sweep in a merged function's exit block carry the function's own source line in their `#line`, so lldb stepping from a `return` shows the `function` line instead of the `return`; the `lines` suite reads green (lane b10-ir, 2026-10-04, its reply's *Found beside*) | `selfhost/ir/exits.hero` (the exit block's line) · design.md §2's promise that lldb steps `.hero` lines · defect 231 · **class: improvement**

    **Origin:** lane b10-ir, 2026-10-04, beside 231; filed by the coordinator.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a debugging step less exact than it could be; no value moves.

    Repaired at `9d8b07ae`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every copy-out carries the span of what leaves, the `return`, the `?` or the end of the body, so the retain and the sweep after it do too, and a merged exit takes the copy-outs and the line of the way out written first in the source: lldb's `next` from a three-way function's first `return` now goes on to the releases where it stopped on the `function` line, and from its other two stops on the first `return`'s line, one exit serving every way out; 116 emissions moved, every changed line a `#line`.

    Repaired at `d15ab3fb`, 2026-10-07 (lane b14-emit), the residual, gated by its cases and the compiler's own tests; the net is owed at the batch's close. A merged exit's load of the return slot is written under the generated file, and so the releases and the `return` after it, as a one-exit function's cleanup is: `lldb --batch` on a function with three `return`s now goes from each to the cleanup, `three.c:122`, where from the second and third it landed on the first `return`'s line; the commit carries the one bless for defects 263 and 335, 475 emissions moved.
