- [ ] **M-closures-verdict step 1** | Three of the compiler's four production function values are in `selfhost/measure/pieces.hero`. Read `run_of` and the three predicates it is handed, and say what a single function taking a `kind` parameter buys over three near-identical loops. Then say which of the three predicates a closure would delete, and why the answer is none

    **Where to look:** selfhost/measure/pieces.hero § run_of, is_letter, is_digit, is_other
    **Why it matters:** §1.7's test is subtraction, and a feature that deletes nothing has to earn its place some other way
