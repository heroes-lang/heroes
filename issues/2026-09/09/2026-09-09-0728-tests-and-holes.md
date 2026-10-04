- [x] **M-interpolation-verdict** | a hole rule below `any expression` makes the spec's *"valid anywhere"* false for `???`, and the sitting either excepts it or pays for it | `spec/heroes-spec.md` § Tests and holes · `examples/gallery/09-holes.hero:28` · `docs/measurements/021-what-would-stand-inside-a-hole.md`

    **Origin:** step 1's own count, 2026-09-08, the one hole of 2536 that no
    rule short of the widest admits. Filed rather than left in the measurement
    because the measurement is a record and the agenda is this list.

    **The spec says *anywhere* and a corpus program takes it at its word.**
    `spec/heroes-spec.md` § Tests and holes: *"`???` is a valid expression
    anywhere"*, and `examples/gallery/09-holes.hero:28` writes one as the last
    operand of a concatenation. Of the four candidate hole rules 021 priced,
    the three narrow ones admit a name, a `.field` run and a postfix run, and
    none of them admits `???`. So a hole is a place in a program where
    *anywhere* stops being true, and there are only two honest routes: the spec
    grows an exception, which costs tokens against **40** free, or the hole
    admits any expression, which is the rule the same measurement already
    recommends on its own grounds because the alternative costs seven holes.

    **Why it matters:** the language's own claim about `???` is the kind a model
    reads once and relies on, and the diagnostic for a rejected `{???}` would
    have to contradict a sentence the spec states without exception.

    **ANSWERED 2026-09-09 by panel 121 R1, ratified the same day.** A hole admits
    ANY expression, so `???` stands in one as it stands anywhere, and *"valid
    anywhere"* stays true without an exception clause. The measurement that
    filed this item is what decided it: the narrow rules admitted a name, a
    field run or a postfix run and refused seven holes of 2536 including this
    one, and cost MORE spec tokens to state than the permission (022).
