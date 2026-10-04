- [ ] **M-value-aggregates coverage** | Two rules bit while writing the coverage cases, and both are the language working rather than failing: `_` as a whole ARM over a variant is `wildcard_on_variant` ("name every case, so that adding one breaks this `match`"), and an unread parameter is `unused_binding`. Task: for a variant with five cases where three share a body, write the arm that is legal — and say what `|` costs against what `_` would have cost

    **Where to look:** spec lines 99-103 · tests/golden/run/variant-arms.hero
    **Why it matters:** the rule exists so that adding a case is a compile error, and it is only worth its cost if the legal form is writable
