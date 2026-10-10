---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **556 — two non-static definitions of one C name in bound headers make `build` exit 2** | the critic's `onedef` (one header defining a non-static function, named by two modules) and `extdef` (two headers each defining a non-static `twice`): `build` exits 2, *internal error: linking failed: duplicate symbol '_twice'*, blaming the compiler; `onedef` passes `test`, the reverse of the two-header case | the link step's reading of a duplicate symbol, `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    **Widened 2026-10-10**, by panel 202 (R2, C2; `docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): a tentative definition, `int counter;` in a header two modules include, is the same cause, an external definition named by two units; it links on this Mac (`-fcommon`) and is a multiple definition on Linux, the verdict depending on the platform (the ffi-pragmatist's `c/link/tent`). The repair tells both at exit 1 before the link on every platform.
