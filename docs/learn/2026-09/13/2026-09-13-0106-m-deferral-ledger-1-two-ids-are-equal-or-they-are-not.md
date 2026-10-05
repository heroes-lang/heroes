- [ ] **M-deferral-ledger 1** | The checker never compares two types recursively. `selfhost/check/table.hero:5-9` says every distinct type gets exactly one id, so `a == b` on two ids IS type equality. Say at which single function a written type becomes an id, and why that one place is what would make `alias Env = {str: i64}` cost one match arm rather than a change at every place the compiler asks *is this the same type*. Then name one thing it does NOT make cheap.

    **Where to look:** `selfhost/check/table.hero` (`ty_key` at :128, `intern` at
    :225, `fits` at :313, the one non-identity relation); `selfhost/check/lower.hero:28-82`
    (`lower_ty` → `shape` → `named`); `selfhost/resolve/vocab.hero:62-71` (`TypeRef`,
    what a resolved name can be); the compiler-engineer's row in
    `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md`.

    **Why it matters:** the coordinator briefed the seat to find "where two types
    are decided to be the same" expecting a function, and there is none: the
    decision was made once, at design time, by interning. A representation choice
    made at M-typed-frontend priced a language feature nobody had proposed yet, and
    it priced it low. The half it did not price — the cycle walk, the formatter arm,
    the diagnostic that must print a name the type table has forgotten — is where
    the estimate's 140 to 180 lines actually live.
