- [ ] **M-reflection-verdict walkthrough** | `Point::x` is two characters of surface and sixty-eight places in the compiler. Walk the path one of them takes: from the two bytes the lexer reads to the string literal the C comes out holding.

    **Where to look:** `selfhost/scan.hero`'s two-byte rule, `selfhost/token.hero`'s
    `colon_colon`, `selfhost/grammar_expr.hero::after_colon_colon`,
    `selfhost/check/access.hero::field_name_type`, and
    `selfhost/ir/flatten.hero`'s `.field_name` arm, which is the one that makes
    the whole form disappear.

    **Why it matters:** it is the shortest complete tour of the compiler this
    project has, and it ends on the idea the project calls **sugar**: a form is
    sugar when exactly one function erases it on the way into the intermediate
    representation, and after that nothing downstream knows it ever existed. That
    is why the emitter, the runtime, the ownership pass and the monomorphiser
    have not one line about `::` — and why the sixty-eight places are all in
    front of that one function and none behind it.
