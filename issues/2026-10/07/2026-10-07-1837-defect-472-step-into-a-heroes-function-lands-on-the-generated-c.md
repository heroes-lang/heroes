---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: a3ebe9eb33051f073861bbbeafc5e0f886258e9d
github: none
---

- [ ] **472 — `step` into a Heroes function lands on the generated C** | `step` into `square` lands on `dbg.c:34:5` under every debug word (panel 197's compiler-engineer); design.md `:682`'s attribution of prologue code, which an author still sees as C | the emitted prologue's `#line` · design.md §3.1 `:682` · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7 and its critic.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a debugger step less exact than it could be.

    Repaired at `a3ebe9eb`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's R4: the thread guard, the prologue and the `goto` to the first block are one physical line under the function's own `#line` (`emit/prologue.hero`, out of `emit/body.hero`), the exit and cleanup staying generated (defect 335); design.md §3.1's sentence amended with its date. lldb on this Mac at `-O0`: `step` into `greet` and a breakpoint on it, `dbg.c:19:13` before and `dbg.hero:1:13` after; at `-O2` both on line 2, no line-1 row mid-function. The `lines` suite now asks every entry of the `run/` corpus to be its signature's line (425 of 426 red with the compiler before, 426 and 0 after); every emission re-blessed and read by kind; the compiler's C 48,311,651 to 47,091,837 bytes and 1,535,984 to 1,207,560 lines, where route D's line per `#line` grew the seed 28%.
