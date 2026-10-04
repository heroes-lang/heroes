---
kind: feature
area: runtime
milestone: M-panic-location
filed: 2026-09-13
commit: none
github: none
---

- [ ] **M-panic-location** | the assert path is a FIFTH surface this milestone's census did not name, and it carries no file and no line either | `runtime/heroes_runtime.h:300-301` · `selfhost/emit/ffi.hero:58` · `docs/panel/136-the-item-named-three-ancestors-and-the-two-it-needed-were-elsewhere.md` § Found beside the sitting

    **Origin:** panel 136, 2026-09-13, measured by the compiler seat while
    pricing doctests and recorded here rather than repaired there.

    **The census of 2026-09-03 above measured `hero_panic`** and named four
    classes that funnel through it — an out-of-range index, an overflow, a
    `.must()` on an error, a slice that splits a character. **A failing `assert`
    does not go through it.** `runtime/heroes_runtime.h:300-301` is that path's
    whole surface:

        _Noreturn void hero_panic_assert(HeroStr text);
        _Noreturn void hero_panic_assert_sides(HeroStr text, HeroStr left, HeroStr right);

    No file, no line, either one. **What locates a failing assert today is the
    author-written title of the `test` block it sat in**, which is why the
    compiler's own source records the gap in its own words at
    `selfhost/emit/ffi.hero:58`: *"stopped the compiler with `assert failed:
    !found.is_err()` and no line"*.

    **Why it matters beyond this milestone**: panel 136 found that any future
    form carrying an assert with no title — a doctest, a documented test block —
    cannot be located at all until this is closed, so this item is on the path of
    a language decision and not only of a runtime one. The fix is the milestone's
    own: the emitted C already carries `#line`, so `__FILE__` and `__LINE__` at
    the assert site resolve to the `.hero` position and the runtime is not told.
    It is an ABI move on the same two-phase edit the rest of this milestone needs.
