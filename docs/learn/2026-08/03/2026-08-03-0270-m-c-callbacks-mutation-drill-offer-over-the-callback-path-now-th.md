- [ ] **M-c-callbacks** | mutation drill offer: `heroes mutate` over the callback path, now that a function type may stand in an `extern` parameter and nowhere else — the interesting mutants are the ones that move a function type from a parameter into a result, an `@` out-parameter or an `extern constant`, which are the three refusals one argument covers

    **Where to look:** selfhost/check/ffi.hero · selfhost/emit/callback_guard.hero
    **Why it matters:** a position rule has three ways to be wrong and the sitting only named two of them
