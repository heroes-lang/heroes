- [ ] **M-data-declarations** | A `match` used as a VALUE requires every arm to produce a value. Found by the llm-ergonomist in panel 014: the rule is orthogonal to the arm-body spelling, and today `tag = match t` with an arm whose body is `break` (or `v @ v + 1`, which does not even diverge) is silent under **both** readings — `=> assert false` inline and the same statement inside a block arm are equally unguarded

    **Where to look:** docs/panel/014-*.md, archive/bootstrap-rs/heroes/src/syntax/control.rs
    **Why it matters:** it is the only sentence in that panel that converts a silent wrong program into a loud one, and it belongs to the checker, not the grammar
