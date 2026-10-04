---
kind: feature
area: emit
milestone: M-core-packages
filed: 2026-09-11
commit: none
github: none
---

- [ ] **M-core-packages** | shape B-prime, the generated per-field walk with handlers as parameters, which is the only route to the ROT — scheduled with what it owes, and NOT adopted | `docs/panel/132-two-seats-invented-the-same-thing-and-two-others-invented-the-other.md` · `selfhost/emit/structural.hero` · `selfhost/ir/mono.hero`

    **Origin:** panel 132, 2026-09-11, where it was reached twice independently.
    The ffi-pragmatist found that a record of handlers cannot work because **a
    record cannot be generic**, so its accumulator is fixed at one type and every
    byte must buffer in a Heroes string before reaching C — design.md §1.11's
    level-3 glue inverted; it moved the handlers into parameters, freeing the
    accumulator, and compiled one walk driving both a Heroes string and a live
    `FILE *`, plus one builder consuming both a `Json` and a live SQLite row.
    Independently the historian, reading Ada's `T'Write` (ISO 1995) and
    `T'Put_Image` (ISO 2022), wrote that the precedent transfers *"only if the
    per-scalar writers are passed as ordinary function arguments"*.

    **Why it is scheduled and not adopted, and this is the distinction that cost
    panel 117 four days ago.** What was compiled is a **hand-written simulation of
    what the compiler would generate**, 78 lines of it, not the generation. The
    boundary half is measured and sound — `HeroDesc` still **5** members, the
    header's struct by value and unredeclared, `_Static_assert`s intact,
    monomorphised `HERO_TU_LOCAL` so dead-code elimination survives, **nothing in
    the value ever** — and the compiler side is unrun. Field-name literals go from
    **18 to 0**, which is the whole buy and it is real.

    **Three refusals its own proposing seat calls blocking, each compiled.** A
    walk over a `partial` record builds at exit 0 reporting 12 bytes of a measured
    `sizeof(struct stat) == 144`, and **consuming one is not merely an abort but
    impossible**, there being no honest construction call. `sort(keys(m))` on
    `{Point: i64}` is `error[unordered_element]`, and brute force found **four**
    pairs of `==`-equal maps marshalling to different bytes. And the ergonomic
    cost is a 13-argument call whose generated signature is a **577-character line
    the formatter does not wrap**.

    **Two predictions must be run before it lands**, both the historian's: that it
    **fails on a record whose field is a list of records**, because the nested walk
    must itself receive the writer set and the instantiation cannot be inferred at
    the inner call site; and that derivation is **linear** in field count at 8, 64
    and 512 fields, which scores that seat's own panel 131 warning — if the 512
    case exceeds 8x the 64 case, the quadratic disease it sourced for Haskell's
    `Generic` is in this shape too.

    **And Ada's own documented mistake is the third thing owed**: GNAT says the
    default derived format is *"deliberately not documented and subject to
    change"* and tells users not to depend on field order. A Heroes walk feeding a
    JSON package must fix field order in the document, or a golden will pin what
    the compiler never promised.

    **Rehomed 2026-09-12, at M-reflection-verdict's close**, rather than closed
    by assertion. It comes here because **step 4 is `encoding/json`**, the
    package that would consume it and the one whose Go original is
    reflection-based: the question *can the compiler write the conversion* is
    answered by writing that package with or without it. The two sittings that
    produced it are `docs/panel/131` and `docs/panel/132`, and the shape adopted
    in their place — `Point::x` — closes the typo and leaves **the rot** open,
    which is what this item is.
