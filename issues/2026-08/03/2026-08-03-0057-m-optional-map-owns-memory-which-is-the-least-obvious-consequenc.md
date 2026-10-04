- [ ] **M-optional-map** | **`int?` owns memory**, which is the least obvious consequence in the language: its error side is two `str`s, so every `T?` is reference-counted whatever `T` is. Question: how many heap blocks does `half(8)` allocate when it returns `ok(4)`, and how many does `half(7)` allocate returning `fail("odd", "not divisible")`? (The second answer depends on something about literals.)

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/perfn.rs (option_bodies) · tests/golden/run/fallible.hero
    **Why it matters:** it is where "a scalar is free" stops being true, and the reason is a field nobody looks at
