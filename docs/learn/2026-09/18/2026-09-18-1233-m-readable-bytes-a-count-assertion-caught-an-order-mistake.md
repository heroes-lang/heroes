- [ ] **M-readable-bytes exit quiz** | A new built-in was added to the inventory table and one assertion caught a mistake worse than the count. **Before reading: the table lists forty names. The first draft inserted the new one alphabetically, beside `to_f32`. Predict what broke.** | `selfhost/inventory.hero`, the module doc and the count test

    **Where to look after answering:** the module's own doc — *"`Ref`'s builtin
    case carries an INDEX into this table, so the order is meaning"* — and the
    test that reads `entries[37].name == "lease"`.

    **Why it matters.** The count assertion said `40` where `39` was expected,
    which is the boring half. The real damage was that `to_u8` would have moved
    from index 35 to 36 and every name after it with it. The fix is the same one
    `lease` and `end_lease` took: append, never insert.

    **The question to carry away.** Ask which of the two failures an instrument
    could have caught if the count had happened to stay right — and what that
    says about asserting a length when the thing you care about is an order.
