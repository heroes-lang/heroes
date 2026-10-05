- [ ] **panel 024** | The other three costed gaps, none of which had been reported before: a map's **iteration order** (and whether it yields keys or pairs), the **sign of `%`** on negatives (`-7 % 3` is `-1` or `2`?), and **`slice` out of range** (clamp, abort, or `T?`). All three are silent when guessed wrong. Plus two the ergonomist reframed: **`T` → `T?` widening** is unspecified and used constantly, and **the spec never shows how to construct a variant case**

    **Where to look:** docs/panel/024 R5
    **Why it matters:** these are the whole silent-error surface a reader with only the spec could find
