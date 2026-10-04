- [ ] **M-core-packages** | `sort_by` joins the library's six generic higher-order functions, and four hand-written sorts come out | `selfhost/library_source.hero:74-125` · `selfhost/modules.hero:167-190`, `selfhost/resolve.hero:82-101`, `selfhost/resolve/cycles.hero:77-98`, `selfhost/ir/verify.hero:150-168` · `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md`

    **Origin:** panel 137, 2026-09-13, adopted as the third route after the two
    the sitting was convened on were refused and deferred. **Two seats named
    M-generics-library and it closed 2026-08-11**, so this is the correction:
    the library grows at the milestone that builds packages over it.

    **What the sitting established, so nothing is re-derived.** A generic sort
    with a comparator **already compiles and runs** —
    `sort_by<A>(xs: [A], lt: (function(a: A, b: A) -> bool)) -> [A]` over a
    `[Point]` printed `1 2 3` at exit 0 — because generics are unconstrained and
    top-level functions are values. Nothing in the language has to change. The
    library already ships six of these (`map`, `filter`, `fold`, `find`, `any`,
    `all`), so this is the seventh row of an existing table.

    **The acceptance test is the compiler seat's own prediction**, and it is the
    measurement this item is judged by: **≤ 20 added lines** in
    `library_source.hero`, **≥ 60 deleted** across the four hand-written sorts
    above (24, 20, 22 and 19 lines), **no compiler module changed**, three suites
    green. If any compiler module must change, panel 137's veto on nominal traits
    was wrong and that row reopens.

    **Two things the sitting decided must be written into it rather than left to
    the implementer.** Stability is stated in the doc comment the way `fold`'s
    argument order had to be after panel 029 R6, because the wrong guess there is
    silent. And the parameter is named `lt` rather than `before` or `show`, which
    **collide with the top-level functions they would carry** — `shadowed_binding`,
    hit on the seat's first compile. One of the four sorts,
    `selfhost/ir/verify.hero:150-168`, is a plain oversight rather than a
    limitation: `sort([3, 1, 2])` compiles today.
