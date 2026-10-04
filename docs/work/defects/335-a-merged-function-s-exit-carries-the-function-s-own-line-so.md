- [ ] **335 — a merged function's exit carries the function's own line, so a debugger stepping out of a `return` lands on the function's head** | since defect 231's repair, the copy-outs, the retain and the sweep in a merged function's exit block carry the function's own source line in their `#line`, so lldb stepping from a `return` shows the `function` line instead of the `return`; the `lines` suite reads green (lane b10-ir, 2026-10-04, its reply's *Found beside*) | `selfhost/ir/exits.hero` (the exit block's line) · design.md §2's promise that lldb steps `.hero` lines · defect 231 · **class: improvement**

    **Origin:** lane b10-ir, 2026-10-04, beside 231; filed by the coordinator.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a debugging step less exact than it could be; no value moves.
