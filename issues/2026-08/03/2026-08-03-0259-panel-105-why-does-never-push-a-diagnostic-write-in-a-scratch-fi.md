- [ ] **panel 105** | **Why does `bind_missing` never push a diagnostic?** Write `n: i64 = wanted(what: "a")` in a scratch file and run `heroes check` on it. Count the diagnostics. Then read `bind_missing`'s doc comment and say which OTHER function reported the one you saw

    **Where to look:** selfhost/check/generics.hero (`bind_missing`), selfhost/check/walk.hero (`compare`)
    **Why it matters:** one mistake, one message — §4.5's promise, kept by having exactly one owner per message
