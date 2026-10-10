---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 74e3d457befd161819214c5fadeae970a32fbe1f
github: none
---

- [ ] **540 — a release inside a line stops the debugger under the generated file** | with the prologue carrying the function's line, `step`, `next`, `next` stops at `dbg.hero:1`, `dbg.hero:2:10`, then `dbg.c:70:5`, a release inside line 2 mapped to the generated file (panel 200's compiler-engineer, lldb on this Mac) | the release's `#line` in `selfhost/emit/` · panel 200 R4 · defect 472 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a debugger stop less exact than it could be.

    Repaired at `74e3d457`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A retain or a release inside a line is written under that line (`emit/release.hero`'s `named`), and the exit keeps the generated file as defect 335 ruled: a merged exit's load, the retain of what a way out returns (`term.retains_returned`) and the sweep. lldb on this Mac, `next` through `greet`: before 2:10, dbg.c, 2:11, dbg.c, 2:12, dbg.c, 3:10; after 2:10, 3:10, 5:10, 7:11, then the exit. Case `emit/fixedbugs-540-…`; 313 blessed emissions and six `emit` goldens moved, `#line` lines only; `emission` 1086, `emit` 12, `lines` 426, the compiler's 1,545 tests, 0 failed; the compiler's C 47,130,136 to 48,295,204 bytes.
