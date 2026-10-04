- [ ] **262 — every `#line` the emitter writes escapes its file name again** | 25,545,276 calls for 1,155,601 directives in the compiler's own emission (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*) | `selfhost/emit/c_text.hero` (the `#line` name's escaping, which defect 240 also touches) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost per directive that one escape per file would pay once; no program refused or wrong.

    Repaired at `4084f886`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.
