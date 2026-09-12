- [ ] **M-cstr-lifetime, mutation drill** | Break the rule four ways in `selfhost/check/lending.hero` and predict which suite goes red for each, before running any | `selfhost/check/lending.hero` · `.claude/rules/verification.md` § What gates what | the map exists so that "which suites" stops being a guess, and this is the drill that proves you can read it

    **Origin:** M-cstr-lifetime close, 2026-09-09.
    **The four mutations:** delete the early exit; change `argument_of` to
    record only a call's args and not a UFCS receiver; drop the `is_extern`
    guard in `no_cstr_out_of_heroes`; and make `no_cstr_in_a_record` ignore
    `rec.header`. For each, name the suite that catches it and the case that
    fires.
    **The question after:** exactly one of the four is caught by NO suite in
    the tree today. Say which, and what case would have to exist.
