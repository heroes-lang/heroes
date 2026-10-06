---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: 8aa7729fa75bd6582fdfef4a2f9384b89c05a6cb
github: none
---

- [ ] **353 — on Windows `args()` receives an argument holding a lone surrogate as valid UTF-8, so the spec's *one that is not UTF-8 aborts* is false there** | an argument `x<D800>y` reaches `args()` as `x?y` through the narrow door and as `x<U+FFFD>y` under the UTF-8 manifest (panel 191's runs at `7f4c0cc5`, carried); both are valid UTF-8, a wrong value with no failure, where on this Mac and Linux arm64 the WTF-8 bytes of the same argument abort `args()` and give `args_checked()` `not_text` (panel 192's ffi-pragmatist) | `spec/heroes-spec.md:324-325` · the runtime's `argv` on Windows (`hero_args_set`, `os.c`) · panel 192's R6 (the wide `argv` converted losslessly to WTF-8) · defect 238's `c-dirwide`, with which it composes · **class: blocking**

    **Origin:** panel 192, 2026-10-05, its Q5 (panel 191's Q-c), which the Windows box's absence left unrun; filed by the synthesis's R12 so that it is tracked.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a sentence of the spec false on one platform; never deferred.

    Repaired at `8aa7729f`, 2026-10-05, gated by its case on this Mac, the Windows box (red over the runtime before it, green after) and Linux arm64, and the compiler's own tests; the net is owed at the batch's close, and the push's platform legs before it closes.
