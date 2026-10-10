---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: 4899e21a6ea48c7c1c07f33623050b4ed68d7a09
github: none
---

- [ ] **556 — two non-static definitions of one C name in bound headers make `build` exit 2** | the critic's `onedef` (one header defining a non-static function, named by two modules) and `extdef` (two headers each defining a non-static `twice`): `build` exits 2, *internal error: linking failed: duplicate symbol '_twice'*, blaming the compiler; `onedef` passes `test`, the reverse of the two-header case | the link step's reading of a duplicate symbol, `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

    **Widened 2026-10-10**, by panel 202 (R2, C2; `docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): a tentative definition, `int counter;` in a header two modules include, is the same cause, an external definition named by two units; it links on this Mac (`-fcommon`) and is a multiple definition on Linux, the verdict depending on the platform (the ffi-pragmatist's `c/link/tent`). The repair tells both at exit 1 before the link on every platform.

    Repaired at `4899e21a6ea48c7c1c07f33623050b4ed68d7a09`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where two units read headers of their own, each header list's declarations are read from clang's dump at its top level as C reads them together (`cli/defined_twice.hero`), and a name the units of two modules would each define, a function's body that is neither `static` nor a C99 inline definition, an object's definition, a tentative one included, is told at exit 1 before the link on every platform, `ffi_defined_twice`, naming the files, their lines and the modules, the note offering `static inline` or one module, or `extern` and one definition for an object; `onedef`, `extsame` and the tentative `counter` each refused so, `extdef` told by defect 555's two declarations first. Cases `unsupported/fixedbugs-556-*` (3); unsupported 200 and 0, the compiler's own tests 1,547 passed; a warm self-build 392.9e9 instructions before and 393.8e9 after.
