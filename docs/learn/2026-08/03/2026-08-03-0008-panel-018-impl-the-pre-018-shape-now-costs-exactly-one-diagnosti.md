- [ ] **panel 018 impl** | The pre-018 shape `MAX = constant: int` now costs exactly ONE diagnostic and the body below it vanishes with it. Which code fires, and what makes the recovery stop before the next declaration?

    **Where to look:** syntax/tests/recovery.rs (the_old_shape_costs_one_diagnostic) · syntax/decl.rs file()
    **Why it matters:** the migration path is itself a diagnostic, and the closed keyword set is the anchor
