- [ ] **M-ir-lowering.1** | A `for` loop is four blocks and a `while` is three. Which block does `continue` jump to in each, and what exactly happens to the program if a `for`'s `continue` lands on the test?

    **Where to look:** archive/bootstrap-rs/heroes/src/ir/control.rs (for_loop) · tests/golden/ir/adversarial-continue-steps.expected
    **Why it matters:** the wrong answer compiles, type-checks, passes every other test, and hangs
