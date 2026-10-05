- [ ] **panel 105** | **Where does the type of `wanted(what: "a")` come from?** Open `selfhost/check/walk.hero` at `user_call` and read the two lines that call `check_generics.bind` and `check_generics.bind_missing`. In `x: i64? = wanted(what: "a")`, which of the two binds `A`, and what value does `expected` hold when `wanted` is called as `print(wanted(what: "a"))`? Choose: (a) `bind`, from the argument `"a"` · (b) `bind_missing`, from `i64?` · (c) neither, and that is the error

    **Where to look:** selfhost/check/walk.hero (`user_call`), selfhost/check/generics.hero (`bind_missing`)
    **Why it matters:** the whole of panel 105 is one word — *else* — and this is where it lives
